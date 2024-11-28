#include <simlib.h>
#include "ims.hpp"

// Štatistika pre odpadový materiál
unsigned long Total_waste_material = 0;

// Simulácia dávkovania
void Dispensing::Behavior() {
    Seize(dispenser);
    Wait(DISPENSER_PERFORMANCE); // Čas na dávkovanie
    Dispensing_time(Time);
    Release(dispenser);

    // Aktivácia ďalšieho procesu
    (new Mixing)->Activate();
}

void Mixing::Behavior() {
    int selected_mixer = static_cast<int>(Random() * NUMBER_OF_MIXERS);
    Seize(mixer[selected_mixer]);
    Wait(30); // Čas na zmiešanie
    Mixing_time(Time);
    Release(mixer[selected_mixer]);

    // Aktivácia ďalšieho procesu
    (new Extrusion)->Activate();
}

void Extrusion::Behavior() {
    Seize(extruder);
    Wait(20); // Čas na extrúziu
    Extrusion_time(Time);
    Release(extruder);

    // Simulujeme vznik odpadu počas extrúzie
    if (Random() < 0.05) { // 5 % pravdepodobnosť vzniku odpadu
        Total_waste_material++;
    }

    // Aktivácia ďalšieho procesu
    (new Cooling)->Activate();
}

void Cooling::Behavior() {
    Seize(cooler);
    Wait(15); // Čas na chladenie
    Cooling_time(Time);
    Release(cooler);

    // Aktivácia ďalšieho procesu
    (new Lamination)->Activate();
}

void Lamination::Behavior() {
    Seize(laminator);
    Wait(25); // Čas na lamináciu
    Lamination_time(Time);
    Release(laminator);

    // Simulujeme vznik odpadu počas laminácie
    if (Random() < 0.03) { // 3 % pravdepodobnosť vzniku odpadu
        Total_waste_material++;
    }

    // Aktivácia ďalšieho procesu
    (new Cutting)->Activate();
}

void Cutting::Behavior() {
    Seize(cutter);
    Wait(10); // Čas na rezanie
    Cutting_time(Time);
    Release(cutter);

    // Simulujeme vznik odpadu počas rezania
    if (Random() < 0.02) { // 2 % pravdepodobnosť vzniku odpadu
        Total_waste_material++;
    }

    // Aktivácia ďalšieho procesu
    (new Packing)->Activate();
}

void Packing::Behavior() {
    Seize(packer);
    Wait(10); // Čas na balenie
    Packing_time(Time);
    Release(packer);

    Total_packed_products++;
}

void Production::Behavior() {
    while (WorkShiftActive) {
        (new Dispensing)->Activate();
        Passivate();
        (new Mixing)->Activate();
        Passivate();
        (new Extrusion)->Activate();
        Passivate();
        (new Cooling)->Activate();
        Passivate();
        (new Lamination)->Activate();
        Passivate();
        (new Cutting)->Activate();
        Passivate();
        (new Packing)->Activate();
        Passivate();
    }
}

void WorkShift::Behavior() {
    WorkShiftActive = true; // Začiatok pracovnej zmeny
    Wait(WORK_SHIFT);       // Pracovná doba (12 hodín)
    WorkShiftActive = false; // Koniec pracovnej zmeny
    Wait(DAY - WORK_SHIFT);  // Čas mimo pracovnej zmeny
    (new WorkShift)->Activate(); // Spusti ďalšiu pracovnú zmenu
}


int main() {
    Init(0, DAY * 5); // Simulácia na 5 dní

    // Spustenie pracovnej zmeny a produkcie
    (new WorkShift)->Activate();
    (new Production)->Activate();

    Run();

    // Výstup štatistík
    printf("\n=== Statistiky simulácie ===\n");
    Dispensing_time.Output();
    Mixing_time.Output();
    Extrusion_time.Output();
    Cooling_time.Output();
    Lamination_time.Output();
    Cutting_time.Output();
    Packing_time.Output();
    printf("\nCelkový počet zabalených produktov: %lu\n", Total_packed_products);
    printf("Celkové množstvo odpadu: %lu\n", Total_waste_material);

    return 0;
}





