#include <simlib.h>
#include "ims.hpp"

// Štatistiky
unsigned long Total_waste_material = 0;
unsigned long Total_packed_products = 0;

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

    // Simulácia vzniku odpadu
    if (Random() < 0.05) { // 5 % pravdepodobnosť vzniku odpadu
        Total_waste_material += DISPENSER_CAPACITY * 0.005;
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

    // Simulácia vzniku odpadu
    if (Random() < 0.03) { // 3 % pravdepodobnosť vzniku odpadu
        Total_waste_material += DISPENSER_CAPACITY * 0.003;
    }

    // Aktivácia ďalšieho procesu
    (new Cutting)->Activate();
}

void Cutting::Behavior() {
    Seize(cutter);
    Wait(10); // Čas na rezanie
    Cutting_time(Time);
    Release(cutter);

    // Simulácia vzniku odpadu
    if (Random() < 0.02) { // 2 % pravdepodobnosť vzniku odpadu
        Total_waste_material += DISPENSER_CAPACITY * 0.002;
    }

    // Aktivácia ďalšieho procesu
    (new Packing)->Activate();
}

void Packing::Behavior() {
    Seize(packer);
    Wait(10); // Čas na balenie
    Packing_time(Time);
    Release(packer);

    Total_packed_products += PACKING_PERFORMANCE; // Zvýšenie počtu zabalených produktov

    // Spusti ďalší proces ak je pracovná zmena aktívna
    if (WorkShiftActive) {
        (new Dispensing)->Activate();
    }
}

void Production::Behavior() {
    // Počas pracovnej zmeny neustále opakuje procesy
    while (WorkShiftActive) {
        (new Dispensing)->Activate();
        Passivate(); // Čaká na ukončenie aktuálneho cyklu
    }
}

void WorkShift::Behavior() {
    WorkShiftActive = true;  // Začiatok pracovnej zmeny
    Wait(WORK_SHIFT);        // 12-hodinová pracovná zmena
    WorkShiftActive = false; // Koniec pracovnej zmeny
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
    printf("Celkové množstvo odpadu: %lu kg\n", Total_waste_material);

    return 0;
}






