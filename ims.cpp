#include <simlib.h>
#include <cstdio>
#include "ims.hpp"

// === Globálne premenné ===
int Warehouse_material = RAW_MATERIAL_STOCK_CAPACITY; // Počátečné množstvo materiálu vo sklade (kg)
int Waiting_material_in_mixer = 0;
int Waiting_material_in_extruder = 0;
int Waiting_material_in_cooler = 0;
int Waiting_material_in_laminator = 0;
int Waiting_material_in_cutter = 0;
int Recycled_material = 0;             // Materiál vrátený do recyklácie
unsigned long Total_waste_material = 0;  // Celkové množstvo odpadu (kg)
unsigned long Total_packed_products = 0; // Celkový počet zabalených produktov
unsigned long Total_recycled_materials = 0; // Celkové množstvo recyklovaných materiálov
bool RecycleNextCycle = false;         // Indikátor recyklácie pre každý druhý cyklus

// === Procesy ===
void ActivateQueue(Queue &q) {
    if (!q.Empty()) {
        q.GetFirst()->Activate();
    }
}

void Dispensing::Behavior() {
    if (dispenser.Busy()) {
        dispensing_q.Insert(this);
        this->Passivate();
    }

    if (Warehouse_material >= DISPENSER_CAPACITY) {
        Warehouse_material -= DISPENSER_CAPACITY;
        Waiting_material_in_mixer += DISPENSER_CAPACITY;
        Seize(dispenser);
        Wait(Uniform(DISPENSER_PERFORMANCE * 0.9, DISPENSER_PERFORMANCE * 1.1));
        Dispensing_time(Time);
        Release(dispenser);
        ActivateQueue(dispensing_q);
        (new Dispensing)->Activate();
        (new Mixing)->Activate();
    } else {
        Passivate();
    }
}

void Mixing::Behavior() {
    if (Waiting_material_in_mixer < MIXER_CAPACITY) {
        Passivate();
        return;
    }

    bool mixer_found = false;
    int selected_mixer = -1;

    for (int i = 0; i < NUMBER_OF_MIXERS; i++) {
        if (!mixer[i].Busy()) {
            selected_mixer = i;
            mixer_found = true;
            break;
        }
    }

    if (mixer_found) {
        Seize(mixer[selected_mixer]);
        Waiting_material_in_mixer -= MIXER_CAPACITY;

        double waste = MIXER_CAPACITY * (Random() * 0.03 + 0.02); // 2% až 5% odpadu
        if (RecycleNextCycle) {
            Recycled_material += waste;
            Total_recycled_materials += waste;
        } else {
            Total_waste_material += waste;
        }

        Wait(Uniform(MIXER_PERFORMANCE * 0.95, MIXER_PERFORMANCE * 1.05));
        Mixing_time(Time);
        Release(mixer[selected_mixer]);

        Waiting_material_in_extruder += MIXER_CAPACITY - waste;
        ActivateQueue(mixing_q);
        (new Extrusion)->Activate();
        (new Mixing)->Activate();
    } else {
        mixing_q.Insert(this);
        this->Passivate();
    }
}

void Recycling::Behavior() {
    if (Recycled_material > 0) {
        Waiting_material_in_extruder += Recycled_material;
        Recycled_material = 0;
        ActivateQueue(extruding_q);
    }
}

void Extrusion::Behavior() {
    if (extruder.Busy()) {
        extruding_q.Insert(this);
        this->Passivate();
    }

    if (Waiting_material_in_extruder >= EXTRUDER_CAPACITY) {
        Seize(extruder);
        Waiting_material_in_extruder -= EXTRUDER_CAPACITY;

        Wait(Uniform(EXTRUDER_PERFORMANCE * 0.9, EXTRUDER_PERFORMANCE * 1.1));

        if (Random() < 0.05) { // 5% šanca na chybnú viskozitu
            Total_waste_material += EXTRUDER_CAPACITY;
        } else {
            Waiting_material_in_cooler += EXTRUDER_CAPACITY;
            (new Cooling)->Activate();
        }

        Release(extruder);
        ActivateQueue(extruding_q);
        (new Extrusion)->Activate();
        RecycleNextCycle = !RecycleNextCycle; // Prepínač recyklácie
        if (RecycleNextCycle) {
            (new Recycling)->Activate();
        }
    } else {
        Passivate();
    }
}

void Cooling::Behavior() {
    if (cooler.Busy()) {
        cooling_q.Insert(this);
        this->Passivate();
    }

    if (Waiting_material_in_cooler >= COOLER_CAPACITY) {
        Seize(cooler);
        Waiting_material_in_cooler -= COOLER_CAPACITY;

        Wait(Uniform(COOLING_PERFORMANCE * 0.95, COOLING_PERFORMANCE * 1.05));
        Cooling_time(Time);
        Release(cooler);

        Waiting_material_in_laminator += COOLER_CAPACITY;
        ActivateQueue(cooling_q);
        (new Lamination)->Activate();
        (new Cooling)->Activate();
    } else {
        Passivate();
    }
}

void Lamination::Behavior() {
    if (laminator.Busy()) {
        laminating_q.Insert(this);
        this->Passivate();
    }

    if (Waiting_material_in_laminator >= LAMINATOR_CAPACITY) {
        Seize(laminator);
        Waiting_material_in_laminator -= LAMINATOR_CAPACITY;

        double waste = LAMINATOR_CAPACITY * 0.03; // 3% odpadu
        Total_waste_material += waste;

        Wait(LAMINATION_PERFORMANCE);
        Lamination_time(Time);
        Release(laminator);

        Waiting_material_in_cutter += LAMINATOR_CAPACITY - waste;
        ActivateQueue(laminating_q);
        (new Cutting)->Activate();
        (new Lamination)->Activate();
    } else {
        Passivate();
    }
}

void Cutting::Behavior() {
    if (cutter.Busy()) {
        cutting_q.Insert(this);
        this->Passivate();
    }

    if (Waiting_material_in_cutter >= CUTTER_CAPACITY) {
        Seize(cutter);
        Waiting_material_in_cutter -= CUTTER_CAPACITY;

        double waste = CUTTER_CAPACITY * (Random() * 0.02 + 0.01); // Rozsah 1% až 3%
        Total_waste_material += waste;

        Wait(CUTTING_PERFORMANCE); // Čas spracovania rezania
        Cutting_time(Time);        // Zaznamenanie času stráveného rezaním
        Release(cutter);

        // Pridanie počtu zabalených produktov (znížený o odpad)
        Total_packed_products += CUTTER_CAPACITY - waste;

        // Aktivácia ďalších procesov v rade
        ActivateQueue(cutting_q);

        // Aktivácia nového procesu Cutting pre ďalšie spracovanie
        (new Cutting)->Activate();
    } else {
        Passivate();
    }
}

void Production::Behavior() {
    while (true) {
        (new Dispensing)->Activate();
        Passivate();
    }
}

// === Hlavní funkce ===
int main() {
    Init(0, DAY); // Simulacia na den
    RandomSeed(time(nullptr));
    (new Production)->Activate();

    Run();
    dispenser.Output();
    mixer[0].Output();
    mixer[1].Output();
    extruder.Output();
    cooler.Output();
    laminator.Output();
    cutter.Output();

    printf("Celkový počet zabalených produktov: %lu kg\n",Total_packed_products + Total_recycled_materials);
    printf("Celkové množstvo odpadu: %lu kg\n", Total_waste_material - Total_recycled_materials);
    printf("Celkové množstvo recyklovaných materiálov: %lu kg\n", Total_recycled_materials);
    return 0;
}





