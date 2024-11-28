// #include <simlib.h>
#include <cstdio>
#include "ims.hpp"
// === Globální proměnné ===
double Warehouse_material = RAW_MATERIAL_STOCK_CAPACITY; // Počáteční množství materiálu ve skladu (kg)
double Waiting_material_in_mixer = 0.0;
double Waiting_material_in_extruder = 0.0;
double Waiting_material_in_cooler = 0.0;
double Waiting_material_in_laminator = 0.0;
double Waiting_material_in_cutter = 0.0;
double Waiting_material_in_packer = 0.0;
double Recycled_material = 0.0;          // Materiál vrácený do recyklace
unsigned long Total_waste_material = 0;  // Celkové množství odpadu (kg)
unsigned long Total_packed_products = 0; // Celkový počet zabalených produktů

// === Systémové proměnné ===
// bool WorkShiftActive = false;

// TODO: poziatocne generovanie

// === Procesy ===
void ActivateQueue(Queue &q)
{
    if (q.Empty())
    {
        return;
    }
    q.GetFirst()->Activate();
}

void Dispensing::Behavior()
{

    Seize(dispenser);
    Wait(DISPENSER_PERFORMANCE);
    Dispensing_time(Time);
    Release(dispenser);

    if (Warehouse_material >= DISPENSER_CAPACITY)
    {
        Warehouse_material -= DISPENSER_CAPACITY;
        Waiting_material_in_mixer += DISPENSER_CAPACITY;
    }
    else
    {

        Passivate();
    }
    (new Dispensing)->Activate();
    (new Mixing)->Activate();
    printf("Aktivujem proces dispensing\n");
}
void Mixing::Behavior()
{
    if (Waiting_material_in_mixer < MIXER_CAPACITY)
    {
        // maybe sa tu ma dat tiez do fronty nie som si ista
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

    // Ak sme našli voľnú miešačku
    if (mixer_found)
    {
        Seize(mixer[selected_mixer]);
        Waiting_material_in_mixer -= MIXER_CAPACITY;
        Wait(MIXER_PERFORMANCE);
        Mixing_time(Time);
        Release(mixer[selected_mixer]);

        Waiting_material_in_extruder += MIXER_CAPACITY;
        ActivateQueue(mixing_q);
        (new Extrusion)->Activate();

        // ma to tu byt? aby sa znova activovala druha miesacka
        (new Mixing)->Activate();
    }
    else
    {
        mixing_q.Insert(this);
        this->Passivate();
    }
    printf("Aktivujem proces mixing\n");
}

void Extrusion::Behavior()
{
    if (extruder.Busy())
    {
        extruding_q.Insert(this);
        this->Passivate();
    }

    if (Waiting_material_in_extruder >= 50) // tu by mal byt extruder capacity asi
    {
        Seize(extruder);
        Waiting_material_in_extruder -= 50; // tu by mal byt extruder capacity asi
        Wait(EXTRUDER_PERFORMANCE);
        Extrusion_time(Time);
        Release(extruder);

        if (Random() < 0.05)
        {
            Recycled_material += 50 * 0.05;
        }
        else
        {
            Waiting_material_in_cooler += 50; // tu ma byt asi tiez extruder capacity
        }
        ActivateQueue(extruding_q);
        (new Recycling)->Activate();

        (new Cooling)->Activate();
        (new Extrusion)->Activate(); // aby sa znova spojazdnil extruder
    }
    else
    {
        Passivate();
    }
    printf("Aktivujem proces extrusion\n");
}

void Cooling::Behavior()
{

    if (cooler.Busy())
    {
        cooling_q.Insert(this);
        this->Passivate();
    }

    if (Waiting_material_in_cooler >= 10) // cooler capacity?
    {
        Seize(cooler);
        Waiting_material_in_cooler -= 10; // cooler capacity?
        Wait(COOLING_PERFORMANCE);
        Cooling_time(Time);
        Release(cooler);

        Waiting_material_in_laminator += 10; // cooler capacity?
    }
    else
        Passivate();

    ActivateQueue(cooling_q);

    (new Lamination)->Activate();
    (new Cooling)->Activate(); // aby sa znova spojazdnil chladic
    printf("Aktivujem proces extrusion\n");
}

void Lamination::Behavior()
{
    if (laminator.Busy())
    {
        laminating_q.Insert(this);
        this->Passivate();
    }

    if (Waiting_material_in_laminator >= 30) // tu by mal byt laminator capacity asi
    {
        Seize(laminator);
        Waiting_material_in_laminator -= 30; // tu by mal byt laminator capacity asi
        Wait(LAMINATION_PERFORMANCE);
        Lamination_time(Time);
        Release(laminator);

        if (Random() < 0.07) // vadna folia
        {
            double defective_material = 30 * 0.07; // Celkové množstvo vadného materiálu
            double recycle_ratio = Random();       // Náhodné číslo medzi 0 a 1

            // Časť ide do recyklácie
            double recycled_part = defective_material * recycle_ratio;
            Recycled_material += recycled_part;

            // Zvyšok ide do odpadu
            double waste_part = defective_material - recycled_part;
            Total_waste_material += waste_part;
        }
        else
        {
            Waiting_material_in_cutter += 30; // tu ma byt asi tiez laminator capacity
        }
        ActivateQueue(laminating_q);
        (new Recycling)->Activate();

        (new Lamination)->Activate();
        (new Cutting)->Activate();
    }
    else
    {
        Passivate();
    }
    printf("Aktivujem proces lamin\n");
}

void Cutting::Behavior()
{
    if (cutter.Busy())
    {
        cutting_q.Insert(this);
        this->Passivate();
    }
    if (Waiting_material_in_cutter >= 15)
    {
        Seize(cutter);
        Waiting_material_in_cutter -= 15;
        Wait(10);
        Cutting_time(Time);
        Release(cutter);
        ActivateQueue(cutting_q);
        if (Random() < 0.02)
        { // 2 % pravdepodobnosť vzniku odpadu
            Total_waste_material += 15 * 0.002;
            Waiting_material_in_packer -= 15 * 0.002;
        }
        else
        {
            Waiting_material_in_packer += 15;
        }

        ActivateQueue(cutting_q);
        (new Cutting)->Activate();
        (new Packing)->Activate();
    }
    else
        Passivate();
    printf("Aktivujem proces cutt\n");
}

void Packing::Behavior()
{
    if (Waiting_material_in_packer >= 35)
    {
        Seize(packer);
        Waiting_material_in_packer -= 35;
        Wait(10);
        Packing_time(Time);
        Release(packer);

        Total_packed_products += 35;
    }
    // (new Packing)->Activate();
    if (WorkShiftActive)
    {
        (new Dispensing)->Activate();
    }
    printf("Aktivujem proces packing\n");
}

void Recycling::Behavior()
{
    if (Recycled_material > 0)
    {
        Waiting_material_in_extruder += Recycled_material;
        (new Extrusion)->Activate();
        Recycled_material = 0;
    }
    else
        Passivate();
    printf("Aktivujem proces rec\n");
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
    printf("\n=== Statistiky simulácie ===\n");
    Dispensing_time.Output();
    Mixing_time.Output();
    Extrusion_time.Output();
    Cooling_time.Output();
    Lamination_time.Output();
    Cutting_time.Output();
    Packing_time.Output();
    printf("\nCelkový počet zabalených produktov: %lu\n", Total_packed_products);
    printf("Celkové množstvo odpadu: %lu kg\n", Total_waste_material);
}
