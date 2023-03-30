// Servos :
#define nb_servo 49     // Nombre de servo controlés
#define nb_board 3      // Nombre de carte de controle de servo
#define nb_out_board 16 // Nombre de sortie par carte de controle de servo

// valeur min et max OSC pour mappage des valeurs vers angle SERVO
#define min_osc_value 0
#define max_osc_value 1000

// Depending on your servo make, the pulse width min and max may vary, you
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!
#define USMIN 600     // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX 2400    // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

int servo_select = 0;        // variable de selection servo
int osc_value[nb_servo];     // pour stockage valeur par servo
int osc_value_old[nb_servo]; // pour comparaison si changement de valeur

int servomoteur[nb_servo] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48}; // voie du driver de chaque servomoteurs

// Définition des valeurs de calibration des servo
#include "config.h"     // Définition des valeur de calibration des servo

// Liste des longueurs minimum d'impulsions pour - 90°
int servomoins90[nb_servo] = {
    MIN0, MIN1, MIN2, MIN3, MIN4, MIN5, MIN6, MIN7, MIN8, MIN9, MIN10, MIN11, MIN12, MIN13, MIN14, MIN15, MIN16, MIN17, MIN18, MIN19, MIN20, MIN21, MIN22, MIN23, MIN24, MIN25, MIN26, MIN27, MIN28, MIN29, MIN30, MIN31, MIN32, MIN33, MIN34, MIN35, MIN36, MIN37, MIN38, MIN39, MIN40, MIN41, MIN42, MIN43, MIN44, MIN45, MIN46, MIN47, MIN48};

// Liste des longueurs maximum d'impulsions pour + 90°
int servoplus90[nb_servo] = {
    MAX0, MAX1, MAX2, MAX3, MAX4, MAX5, MAX6, MAX7, MAX8, MAX9, MAX10, MAX11, MAX12, MAX13, MAX14, MAX15, MAX16, MAX17, MAX18, MAX19, MAX20, MAX21, MAX22, MAX23, MAX24, MAX25, MAX26, MAX27, MAX28, MAX29, MAX30, MAX31, MAX32, MAX33, MAX34, MAX35, MAX36, MAX37, MAX38, MAX39, MAX40, MAX41, MAX42, MAX43, MAX44, MAX45, MAX46, MAX47, MAX48};

// Liste des longueurs d'impulsions de démarage
int servo_boot[nb_servo] = {
    BOOT0, BOOT1, BOOT2, BOOT3, BOOT4, BOOT5, BOOT6, BOOT7, BOOT8, BOOT9, BOOT10, BOOT11, BOOT12, BOOT13, BOOT14, BOOT15, BOOT16, BOOT17, BOOT18, BOOT19, BOOT20, BOOT21, BOOT22, BOOT23, BOOT24, BOOT25, BOOT26, BOOT27, BOOT28, BOOT29, BOOT30, BOOT31, BOOT32, BOOT33, BOOT34, BOOT35, BOOT36, BOOT37, BOOT38, BOOT39, BOOT40, BOOT41, BOOT42, BOOT43, BOOT44, BOOT45, BOOT46, BOOT47, BOOT48};
