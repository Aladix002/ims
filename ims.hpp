#include <simlib.h>

// Casove jednotky v minutach
#define HOUR 60
#define DAY 1440
#define WORK_SHIFT 720 // Pracovna doba 12 hodin

// Kapacity a vykony v kg
#define RAW_MATERIAL_STOCK_CAPACITY 50000 // Kapacita skladu surovin
#define MIXER_CAPACITY 500                // Kapacita miesacky na jednu davku
#define DISPENSER_CAPACITY 1000           // Kapacita davkovaca na jednu davku
#define DISPENSER_PERFORMANCE 10          // Cas potrebny na vydavkovanie (minuty)
#define EXTRUDER_PERFORMANCE 300          // Vykon extrudera za hodinu
#define COOLING_PERFORMANCE 300           // Vykon chladenia za hodinu
#define LAMINATION_PERFORMANCE 280        // Vykon laminacie za hodinu
#define CUTTING_PERFORMANCE 250           // Vykon rezania za hodinu
#define PACKING_PERFORMANCE 200           // Vykon balenia za hodinu
#define RECYCLE_CAPACITY 100              // Kapacita recyklacie za hodinu

// Pocet zariadeni
#define NUMBER_OF_MIXERS 2
#define NUMBER_OF_DISPENSERS 1
#define NUMBER_OF_EXTRUDERS 1
#define NUMBER_OF_LAMINATORS 1
#define NUMBER_OF_CUTTERS 1
#define NUMBER_OF_PACKERS 1

// Stroje (zariadenia)
Facility dispenser("Davkovac");
Facility mixer[NUMBER_OF_MIXERS];
Facility extruder("Extruder");
Facility cooler("Chladic");
Facility laminator("Laminator");
Facility cutter("Rezaci stroj");
Facility packer("Baliaci stroj");
Facility recycler("Recyklacny stroj");

// Sklady
Store raw_material_stock("Sklad surovin", RAW_MATERIAL_STOCK_CAPACITY);
Store finished_products("Sklad hotovych vyrobkov", 2000); // Maximalny pocet baleni
Store faulty_products("Sklad vadnych vyrobkov", 500);     // Maximalna kapacita vadnych vyrobkov

// Fronty
Queue raw_material_q("Cakanie na suroviny");
Queue mixing_q("Cakanie na miesacku");
Queue extruding_q("Cakanie na extruder");
Queue cooling_q("Cakanie na chladenie");
Queue laminating_q("Cakanie na laminator");
Queue cutting_q("Cakanie na rezanie");
Queue packing_q("Cakanie na baliaci stroj");
Queue recycling_q("Cakanie na recyklaciu");

// Statistiky
Stat Dispensing_time("Cas straveny v davkovaci");
Stat Mixing_time("Cas straveny v miesacke");
Stat Extrusion_time("Cas straveny v extruderi");
Stat Cooling_time("Cas straveny chladenim");
Stat Lamination_time("Cas straveny laminaciou");
Stat Cutting_time("Cas straveny rezanim");
Stat Packing_time("Cas straveny balenim");
Stat Recycling_time("Cas straveny recyklaciou");
unsigned long Total_packed_products = 0;
unsigned long Total_recycled_material = 0;

// Stavy
bool WorkShiftActive = true;

// Procesy
class Dispensing : public Process {
    void Behavior() override {
    }
};

class Mixing : public Process {
    void Behavior() override {
    }
};

class Extrusion : public Process {
    void Behavior() override {
    }
};

class Cooling : public Process {
    void Behavior() override {
    }
};

class Lamination : public Process {
    void Behavior() override {
    }
};

class Cutting : public Process {
    void Behavior() override {
    }
};

class Packing : public Process {
    void Behavior() override {
    }
};

class Recycling : public Process {
    void Behavior() override {
    }
};

class WorkShift : public Event {
    void Behavior() override {
    }
};

class StartProduction : public Event {
    void Behavior() override {
    }
};

