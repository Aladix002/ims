//Projekt do IMS - Vyroba PVC folii
// Filip Botlo - xbotlo01
// Katarína Kozáková - xkozak20

#include <simlib.h>
#include <cstdio>
#include "ims.hpp"

// === Globalne premenne ===
int Warehouse_material = RAW_MATERIAL_STOCK_CAPACITY; 
int Waiting_material_in_mixer = 0;                    
int Waiting_material_in_extruder = 0;             
int Waiting_material_in_cooler = 0;            
int Waiting_material_in_laminator = 0;               
int Waiting_material_in_cutter = 0;                 
int Recycled_material = 0;                       
unsigned long Total_waste_material = 0;            
unsigned long Total_packed_products = 0;          
unsigned long Total_recycled_materials = 0;        
bool RecycleNextCycle = false;          

// === Procesy ===
void ActivateQueue(Queue &q) {
    // Aktivuje proces z fronty
    if (!q.Empty()) {
        q.GetFirst()->Activate();
    }
}

void Dispensing::Behavior() {
    // Proces davkovania materialu
    if (dispenser.Busy()) {
        dispensing_q.Insert(this);
        this->Passivate();
    }

    if (Warehouse_material >= DISPENSER_CAPACITY) {
        Warehouse_material -= DISPENSER_CAPACITY;
        Waiting_material_in_mixer += DISPENSER_CAPACITY;
        Seize(dispenser);
        Wait(Uniform(DISPENSER_PERFORMANCE * 0.9, DISPENSER_PERFORMANCE * 1.1)); // Variacia casu
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
    // Proces miesania materialu
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

        Wait(Uniform(MIXER_PERFORMANCE * 0.95, MIXER_PERFORMANCE * 1.05)); // Variacia casu
        Mixing_time(Time);
        Release(mixer[selected_mixer]);

        Waiting_material_in_extruder += MIXER_CAPACITY;
        ActivateQueue(mixing_q);
        (new Extrusion)->Activate();
        (new Mixing)->Activate();
    } else {
        mixing_q.Insert(this);
        this->Passivate();
    }
}

void Recycling::Behavior() {
    // Proces recyklacie materialu
    if (Recycled_material > 0) {
        Waiting_material_in_extruder += Recycled_material;
        Recycled_material = 0;
        ActivateQueue(extruding_q);
    }
}

void Extrusion::Behavior() {
    // Proces extruzie materialu
    if (extruder.Busy()) {
        extruding_q.Insert(this);
        this->Passivate();
    }

    if (Waiting_material_in_extruder >= EXTRUDER_CAPACITY) {
        Seize(extruder);
        Waiting_material_in_extruder -= EXTRUDER_CAPACITY;

        Wait(Uniform(EXTRUDER_PERFORMANCE * 0.9, EXTRUDER_PERFORMANCE * 1.1)); // Variacia casu

        if (Random() < 0.03) { // 3% sanca na chybu
            double waste = EXTRUDER_CAPACITY;
            if (RecycleNextCycle) {
                double recyclable = waste * 0.7; // 70% recyklovatelne
                Recycled_material += recyclable;
                Total_recycled_materials += recyclable;
                Total_waste_material += waste - recyclable;
            } else {
                Total_waste_material += waste;
            }
            RecycleNextCycle = !RecycleNextCycle;
        } else {
            Waiting_material_in_cooler += EXTRUDER_CAPACITY;
            (new Cooling)->Activate();
        }

        Release(extruder);
        ActivateQueue(extruding_q);
        (new Extrusion)->Activate();
        (new Recycling)->Activate();
    } else {
        Passivate();
    }
}

void Cooling::Behavior() {
    // Proces chladenia materialu
    if (cooler.Busy()) {
        cooling_q.Insert(this);
        this->Passivate();
    }

    if (Waiting_material_in_cooler >= COOLER_CAPACITY) {
        Seize(cooler);
        Waiting_material_in_cooler -= COOLER_CAPACITY;

        Wait(Uniform(COOLING_PERFORMANCE * 0.95, COOLING_PERFORMANCE * 1.05)); // Variacia casu
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
    // Proces laminacie materialu
    if (laminator.Busy()) {
        laminating_q.Insert(this);
        this->Passivate();
    }

    if (Waiting_material_in_laminator >= LAMINATOR_CAPACITY) {
        Seize(laminator);
        Waiting_material_in_laminator -= LAMINATOR_CAPACITY;

        double waste = LAMINATOR_CAPACITY * 0.03; // 3% odpadu
        if (RecycleNextCycle) {
            double recyclable = waste * 0.7; // 70% recyklovatelne
            Recycled_material += recyclable;
            Total_recycled_materials += recyclable;
            Total_waste_material += waste - recyclable;
        } else {
            Total_waste_material += waste;
        }
        RecycleNextCycle = !RecycleNextCycle;

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
    // Proces rezania materialu
    if (cutter.Busy()) {
        cutting_q.Insert(this);
        this->Passivate();
    }

    if (Waiting_material_in_cutter >= CUTTER_CAPACITY) {
        Seize(cutter);
        Waiting_material_in_cutter -= CUTTER_CAPACITY;

        double waste = CUTTER_CAPACITY * (Random() * 0.03 + 0.02); // Rozsah 2% az 5%
        if (RecycleNextCycle) {
            double recyclable = waste * 0.7; // 70% recyklovatelne
            Recycled_material += recyclable;
            Total_recycled_materials += recyclable;
            Total_waste_material += waste - recyclable;
        } else {
            Total_waste_material += waste;
        }
        RecycleNextCycle = !RecycleNextCycle;

        Wait(CUTTING_PERFORMANCE);
        Cutting_time(Time);
        Release(cutter);

        Total_packed_products += CUTTER_CAPACITY - waste;
        ActivateQueue(cutting_q);
        (new Cutting)->Activate();
    } else {
        Passivate();
    }
}

void Production::Behavior() {
    // Proces zaciatku vyroby
    while (true) {
        (new Dispensing)->Activate();
        Passivate();
    }
}

// === Hlavna funkcia ===
int main() {
    Init(0, DAY); // Simulacia na jeden den
    RandomSeed(time(nullptr));
    (new Production)->Activate();

    Run();

    printf("Celkovy pocet zabalenych produktov: %lu kg\n", Total_packed_products + Total_recycled_materials);
    printf("Celkove mnozstvo odpadu: %lu kg\n", Total_waste_material);
    printf("Celkove mnozstvo recyklovanych materialov: %lu kg\n", Total_recycled_materials);
    return 0;
}








