#include <simlib.h>

// Casove jednotky v minutach
#define HOUR 60
#define DAY 1440

// Kapacity a vykony v kg
#define RAW_MATERIAL_STOCK_CAPACITY 500000 // Kapacita skladu surovin
#define DISPENSER_CAPACITY 1500            // Kapacita davkovania
#define MIXER_CAPACITY 1000                // Kapacita miesacky na jednu davku
#define EXTRUDER_CAPACITY 800              // Kapacita extrudera na jednu davku
#define COOLER_CAPACITY 1000               // Kapacita chladica
#define LAMINATOR_CAPACITY 900             // Kapacita laminatora
#define CUTTER_CAPACITY 450                // Kapacita rezacky

// Casy spracovania v minutach
#define DISPENSER_PERFORMANCE 10           // Cas potrebny na davkovanie
#define MIXER_PERFORMANCE 30               // Cas miesania
#define EXTRUDER_PERFORMANCE 8             // Cas extrudovania
#define COOLING_PERFORMANCE 20             // Cas chladenia
#define LAMINATION_PERFORMANCE 25          // Cas laminacie
#define CUTTING_PERFORMANCE 13             // Cas rezania

// Pocet zariadeni
#define NUMBER_OF_MIXERS 2
#define NUMBER_OF_DISPENSERS 1
#define NUMBER_OF_EXTRUDERS 2
#define NUMBER_OF_LAMINATORS 1
#define NUMBER_OF_CUTTERS 1

// Stroje (zariadenia)
Facility dispenser("Davkovac");        // Zariadenie na davkovanie surovin
Facility mixer[NUMBER_OF_MIXERS];      // Miesacky na miesanie materialu
Facility extruder("Extruder");         // Extruder na vytvaranie materialu
Facility cooler("Chladic");            // Chladic na schladenie vyrobku
Facility laminator("Laminator");       // Laminator na laminovanie
Facility cutter("Rezaci stroj");       // Rezacka na delenie hotoveho materialu
Facility recycler("Recyklacny stroj"); // Stroj na recyklaciu odpadu

// Sklady
Store raw_material_stock("Sklad surovin", RAW_MATERIAL_STOCK_CAPACITY); // Sklad na suroviny
Store finished_products("Sklad hotovych vyrobkov", 2000);               // Sklad na hotove produkty
Store faulty_products("Sklad vadnych vyrobkov", 500);                   // Sklad na vadne produkty

// Fronty
Queue dispensing_q("Rada na suroviny do davkovaca");  // Fronta na davkovanie
Queue mixing_q("Rada na miesacku");                  // Fronta na miesacky
Queue extruding_q("Rada na extruder");               // Fronta na extruder
Queue cooling_q("Rada na chladenie");                // Fronta na chladic
Queue laminating_q("Rada na laminator");             // Fronta na laminator
Queue cutting_q("Rada na rezanie");                  // Fronta na rezacku
Queue recycling_q("Rada na recyklaciu");             // Fronta na recyklaciu

// Statistiky
Stat Dispensing_time("Cas straveny v davkovaci");    // Cas straveny davkovanim
Stat Mixing_time("Cas straveny v miesacke");         // Cas straveny miesanim
Stat Extrusion_time("Cas straveny v extruderi");     // Cas straveny extrudovanim
Stat Cooling_time("Cas straveny chladenim");         // Cas straveny chladenim
Stat Lamination_time("Cas straveny laminaciou");     // Cas straveny laminovanim
Stat Cutting_time("Cas straveny rezanim");           // Cas straveny rezanim
Stat Recycling_time("Cas straveny recyklaciou");     // Cas straveny recyklaciou

// Procesy
class Production : public Process
{
public:
    void Behavior() override; // Proces vyroby, zacina s davkovanim
};

class Dispensing : public Process
{
public:
    void Behavior() override; // Proces davkovania surovin
};

class Mixing : public Process
{
public:
    void Behavior() override; // Proces miesania materialu
};

class Extrusion : public Process
{
public:
    void Behavior() override; // Proces extrudovania materialu
};

class Cooling : public Process
{
public:
    void Behavior() override; // Proces chladenia produktu
};

class Lamination : public Process
{
public:
    void Behavior() override; // Proces laminovania
};

class Cutting : public Process
{
public:
    void Behavior() override; // Proces rezania na hotove produkty
};

class Recycling : public Process
{
public:
    void Behavior() override; // Proces recyklacie odpadu
};

class StartProduction : public Event
{
public:
    void Behavior() override; // Udalost spustenia vyroby
};

