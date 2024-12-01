#include <simlib.h>

// Casove jednotky v minutach
#define HOUR 60
#define DAY 1440

// Kapacity a vykony v kg
#define RAW_MATERIAL_STOCK_CAPACITY 200000 // Kapacita skladu surovin
#define DISPENSER_CAPACITY 1500            // Kapacita dávkovania
#define MIXER_CAPACITY 1200                 // Kapacita miešačky na jednu dávku
#define EXTRUDER_CAPACITY 800              // Kapacita extrúdera na jednu dávku
#define COOLER_CAPACITY 1000                // Kapacita chladiča
#define LAMINATOR_CAPACITY 900             // Kapacita laminátora
#define CUTTER_CAPACITY 450                // Kapacita rezačky

// Časy spracovania v minútach
#define DISPENSER_PERFORMANCE 10           // Čas dávkovania
#define MIXER_PERFORMANCE 30               // Čas miešania
#define EXTRUDER_PERFORMANCE 8            // Čas extrúzie 
#define COOLING_PERFORMANCE 20             // Čas chladenia
#define LAMINATION_PERFORMANCE 25          // Čas laminácie
#define CUTTING_PERFORMANCE 13             // Čas rezania


// Pocet zariadeni
#define NUMBER_OF_MIXERS 2
#define NUMBER_OF_DISPENSERS 1
#define NUMBER_OF_EXTRUDERS 2
#define NUMBER_OF_LAMINATORS 1
#define NUMBER_OF_CUTTERS 1

// Stroje (zariadenia)
Facility dispenser("Davkovac");
Facility mixer[NUMBER_OF_MIXERS];
Facility extruder("Extruder");
Facility cooler("Chladic");
Facility laminator("Laminator");
Facility cutter("Rezaci stroj");
Facility recycler("Recyklacny stroj");

// Sklady
Store raw_material_stock("Sklad surovin", RAW_MATERIAL_STOCK_CAPACITY);
Store finished_products("Sklad hotovych vyrobkov", 2000); // Maximalny pocet baleni
Store faulty_products("Sklad vadnych vyrobkov", 500);     // Maximalna kapacita vadnych vyrobkov

// Fronty
Queue dispensing_q("Rada na suroviny do davkovaca");
Queue mixing_q("Rada na miešacku");
Queue extruding_q("rada na extruder");
Queue cooling_q("rada na chladenie");
Queue laminating_q("rada na laminator");
Queue cutting_q("rada na rezanie");
Queue recycling_q("rada na recyklaciu");

// Statistiky
Stat Dispensing_time("Cas straveny v davkovaci");
Stat Mixing_time("Cas straveny v miesacke");
Stat Extrusion_time("Cas straveny v extruderi");
Stat Cooling_time("Cas straveny chladenim");
Stat Lamination_time("Cas straveny laminaciou");
Stat Cutting_time("Cas straveny rezanim");
Stat Recycling_time("Cas straveny recyklaciou");


// Procesy
class Production : public Process
{
public:
    void Behavior() override;
};

class Dispensing : public Process
{
public:
    void Behavior() override;
};

class Mixing : public Process
{
public:
    void Behavior() override;
};

class Extrusion : public Process
{
public:
    void Behavior() override;
};

class Cooling : public Process
{
public:
    void Behavior() override;
};

class Lamination : public Process
{
public:
    void Behavior() override;
};

class Cutting : public Process
{
public:
    void Behavior() override;
};

class Recycling : public Process
{
public:
    void Behavior() override;
};


class StartProduction : public Event
{
public:
    void Behavior() override;
};
