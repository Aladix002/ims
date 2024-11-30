// #include <simlib.h>
#include <cstdio>
#include "ims.hpp"
// === Globální proměnné ===
int Warehouse_material = RAW_MATERIAL_STOCK_CAPACITY; // Počáteční množství materiálu ve skladu (kg)
int Waiting_material_in_mixer = 0.0;
int Waiting_material_in_extruder = 0.0;
int Waiting_material_in_cooler = 0.0;
int Waiting_material_in_laminator = 0.0;
int Waiting_material_in_cutter = 0.0;
int Waiting_material_in_packer = 0.0;
int Recycled_material = 0.0;             // Materiál vrácený do recyklace
unsigned long Total_waste_material = 0;  // Celkové množství odpadu (kg)
unsigned long Total_packed_products = 0; // Celkový počet zabalených produktů
unsigned long Total_recycled_materials = 0;
// === Systémové proměnné ===
// bool WorkShiftActive = false;

// TODO: poziatocne generovanie

// === Procesy ===
void ActivateQueue(Queue &q)
{
    if (!q.Empty())
    {
        q.GetFirst()->Activate();
    }
}

void Dispensing::Behavior()
{
    if (dispenser.Busy())
    {
        dispensing_q.Insert(this);
        this->Passivate();
    }
    

    double waste = DISPENSER_CAPACITY * 0.01; // 1% odpadu
    Total_waste_material += waste;           // Pridanie odpad
    Waiting_material_in_mixer += DISPENSER_CAPACITY;

    Seize(dispenser);
    Wait(DISPENSER_PERFORMANCE);
    Dispensing_time(Time);
    Release(dispenser);

    ActivateQueue(dispensing_q);
    (new Dispensing)->Activate();
    (new Mixing)->Activate();
}



void Mixing::Behavior()
{
    if (Waiting_material_in_mixer <= MIXER_CAPACITY)
    {
        Passivate();
        return;
    }

    bool mixer_found = false;
    int selected_mixer = -1;

    for (int i = 0; i < NUMBER_OF_MIXERS; i++)
    {
        if (!mixer[i].Busy())
        {
            selected_mixer = i;
            mixer_found = true;
            break;
        }
    }

    if (mixer_found)
    {
        double waste = MIXER_CAPACITY * 0.02; // 2% odpadu
        Waiting_material_in_mixer -= MIXER_CAPACITY + waste;
        Total_waste_material += waste;

        Seize(mixer[selected_mixer]);
        Wait(MIXER_PERFORMANCE);
        Mixing_time(Time);
        Release(mixer[selected_mixer]);

        Waiting_material_in_extruder += MIXER_CAPACITY;
        ActivateQueue(mixing_q);
        (new Extrusion)->Activate();
        (new Mixing)->Activate();
    }
    else
    {
        mixing_q.Insert(this);
        this->Passivate();
    }
}


void Extrusion::Behavior()
{
    if (extruder.Busy())
    {
        printf("plny extruder %d kg\n", Waiting_material_in_extruder);
        extruding_q.Insert(this);
        this->Passivate();
    }

    if (Waiting_material_in_extruder >= EXTRUDER_CAPACITY)
    { // Kapacita extrúdera
        Seize(extruder);
        Waiting_material_in_extruder -= EXTRUDER_CAPACITY;
        Wait(EXTRUDER_PERFORMANCE);
        Extrusion_time(Time);
        Release(extruder);

        // Rozhodovací bod pre nesprávnu viskozitu
        if (Random() < 0.05) // 5 % šanca na zlú viskozitu
        {
            Total_waste_material += EXTRUDER_CAPACITY; // Celá dávka ide do odpadu
        }
        else
        {
            Waiting_material_in_cooler += EXTRUDER_CAPACITY; // Pokračuje do chladenia
        }

        // Aktivácia ďalších procesov
        ActivateQueue(extruding_q);
        (new Cooling)->Activate();
        (new Extrusion)->Activate();
    }
    else
    {
        Passivate();
    }
}



void Cooling::Behavior()
{

    if (cooler.Busy())
    {
        printf("plny cooler %d kg\n", Waiting_material_in_cooler);

        cooling_q.Insert(this);
        this->Passivate();
    }

    if (Waiting_material_in_cooler >= COOLER_CAPACITY) // cooler capacity?
    {
        Seize(cooler);
        Waiting_material_in_cooler -= COOLER_CAPACITY; // cooler capacity?
        Wait(COOLING_PERFORMANCE);
        Cooling_time(Time);
        Release(cooler);

        Waiting_material_in_laminator += COOLER_CAPACITY; // cooler capacity?
    }
    else
        Passivate();

    ActivateQueue(cooling_q);

    (new Lamination)->Activate();
    (new Cooling)->Activate(); // aby sa znova spojazdnil chladic
    //("Aktivujem proces extrusion\n");
}

void Lamination::Behavior()
{
    if (laminator.Busy())
    {
        laminating_q.Insert(this);
        this->Passivate();
    }

    if (Waiting_material_in_laminator >= LAMINATOR_CAPACITY)
    {
        Seize(laminator);
        double waste = LAMINATOR_CAPACITY * 0.03; // 3% odpadu
        Waiting_material_in_laminator -= LAMINATOR_CAPACITY + waste;
        Total_waste_material += waste;

        Wait(LAMINATION_PERFORMANCE);
        Lamination_time(Time);
        Release(laminator);

        Waiting_material_in_cutter += LAMINATOR_CAPACITY;
        ActivateQueue(laminating_q);
        (new Cutting)->Activate();
        (new Lamination)->Activate();
    }
    else
    {
        Passivate();
    }
}


void Cutting::Behavior()
{
    if (cutter.Busy())
    {
        cutting_q.Insert(this);
        this->Passivate();
    }

    if (Waiting_material_in_cutter >= CUTTER_CAPACITY)
    {
        Seize(cutter);
        double waste = CUTTER_CAPACITY * 0.02; // 2% odpadu
        Waiting_material_in_cutter -= CUTTER_CAPACITY + waste;
        Total_waste_material += waste;

        Wait(CUTTING_PERFORMANCE);
        Cutting_time(Time);
        Release(cutter);

        Waiting_material_in_packer += CUTTER_CAPACITY;
        ActivateQueue(cutting_q);
        (new Packing)->Activate();
        (new Cutting)->Activate();
    }
    else
    {
        Passivate();
    }
}


void Packing::Behavior() // treba dat queue ked je zabrata
{
    if (packer.Busy())
    {
        printf("plny packer %d kg\n", Waiting_material_in_packer);

        packing_q.Insert(this);
        this->Passivate();
    }
    if (Waiting_material_in_packer >= PACKER_CAPACITY)
    {
        Seize(packer);
        Waiting_material_in_packer -= PACKER_CAPACITY;
        Wait(PACKING_PERFORMANCE);
        Packing_time(Time);
        Release(packer);
        Total_packed_products += PACKER_CAPACITY;
        ActivateQueue(packing_q);
        (new Packing)->Activate();
    }
    else
        Passivate();
    if (WorkShiftActive)
    {
        if (Warehouse_material != 0)
            (new Dispensing)->Activate();
    }
    //("Aktivujem proces packing\n");
}

void Recycling::Behavior()
{
    printf("je v recycling\n");
    if (recycler.Busy())
    {
        recycling_q.Insert(this);
        this->Passivate();
    }
    if (Recycled_material > 0)
    {
        Waiting_material_in_extruder += Recycled_material;
        ActivateQueue(recycling_q);
        (new Extrusion)->Activate();
        Total_recycled_materials += Recycled_material;
        Recycled_material = 0;
    }
    else
        Passivate();
    //("Aktivujem proces rec\n");
}

void WorkShift::Behavior()
{
    WorkShiftActive = true;
    Wait(WORK_SHIFT);
    WorkShiftActive = false;
}

void Production::Behavior()
{
    while (WorkShiftActive)
    {
        (new Dispensing)->Activate();
        Passivate();
    }
}

// === Hlavní funkce ===
int main()
{
    Init(0, DAY * 5); // Simulace na 5 dní

    // Spuštění procesů
    (new WorkShift)->Activate();
    (new Production)->Activate();

    Run();

    // Výstup statistik
    //("\n=== Statistiky simulácie ===\n");
    dispensing_q.Output();
    mixer->Output();
    mixing_q.Output();
    extruding_q.Output();
    cooling_q.Output();
    laminating_q.Output();
    cutting_q.Output();
    packing_q.Output();
    recycling_q.Output();

    printf("Celkový počet zabalených produktov: %lu\n", Total_packed_products);
    printf("Celkové množstvo odpadu: %lu kg\n", Total_waste_material);
}
