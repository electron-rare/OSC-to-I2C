//Bibliothèques LCD Keypad Shield
#include <LiquidCrystal.h>
// Création de l'objet lcd (avec les différents ports numériques qu'il utilise)
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

//Bibliothèques servo moteur:
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>


// Variables
int lcd_key     = 0;
int adc_key_in  = 0;
// Constantes
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

// Depending on your servo make, the pulse width min and max may vary, you
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!
#define USMIN  600 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX  2400 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

#define nb_servo 49
#define nb_board 3
#define PAUSE 200

// Définition valeur servo, utilisé le fichier excel pour modifier les valeur :
#define  MIN0  130
#define MAX0  620
#define BOOT0 0
#define MIN1  130
#define MAX1  620
#define BOOT1 0
#define MIN2  0
#define MAX2  0
#define BOOT2 0
#define MIN3  0
#define MAX3  0
#define BOOT3 0
#define MIN4  130
#define MAX4  375
#define BOOT4 -90
#define MIN5  130
#define MAX5  375
#define BOOT5 -90
#define MIN6  130
#define MAX6  375
#define BOOT6 -90
#define MIN7  130
#define MAX7  375
#define BOOT7 -90
#define MIN8  0
#define MAX8  0
#define BOOT8 0
#define MIN9  0
#define MAX9  0
#define BOOT9 0
#define MIN10 130
#define MAX10 620
#define BOOT10  0
#define MIN11 130
#define MAX11 620
#define BOOT11  0
#define MIN12 130
#define MAX12 375
#define BOOT12  -90
#define MIN13 130
#define MAX13 375
#define BOOT13  -90
#define MIN14 130
#define MAX14 375
#define BOOT14  -90
#define MIN15 130
#define MAX15 375
#define BOOT15  -90
#define MIN16 130
#define MAX16 620
#define BOOT16  0
#define MIN17 130
#define MAX17 620
#define BOOT17  0
#define MIN18 0
#define MAX18 0
#define BOOT18  0
#define MIN19 0
#define MAX19 0
#define BOOT19  0
#define MIN20 130
#define MAX20 375
#define BOOT20  -90
#define MIN21 130
#define MAX21 375
#define BOOT21  -90
#define MIN22 130
#define MAX22 375
#define BOOT22  -90
#define MIN23 130
#define MAX23 375
#define BOOT23  -90
#define MIN24 0
#define MAX24 0
#define BOOT24  0
#define MIN25 0
#define MAX25 0
#define BOOT25  0
#define MIN26 130
#define MAX26 620
#define BOOT26  0
#define MIN27 130
#define MAX27 620
#define BOOT27  0
#define MIN28 130
#define MAX28 375
#define BOOT28  -90
#define MIN29 130
#define MAX29 375
#define BOOT29  -90
#define MIN30 130
#define MAX30 375
#define BOOT30  -90
#define MIN31 130
#define MAX31 375
#define BOOT31  -90
#define MIN32 130
#define MAX32 375
#define BOOT32  0
#define MIN33 130
#define MAX33 375
#define BOOT33  0
#define MIN34 130
#define MAX34 620
#define BOOT34  0
#define MIN35 130
#define MAX35 620
#define BOOT35  0
#define MIN36 0
#define MAX36 0
#define BOOT36  0
#define MIN37 0
#define MAX37 0
#define BOOT37  0
#define MIN38 0
#define MAX38 0
#define BOOT38  0
#define MIN39 0
#define MAX39 0
#define BOOT39  0
#define MIN40 0
#define MAX40 0
#define BOOT40  0
#define MIN41 0
#define MAX41 0
#define BOOT41  0
#define MIN42 0
#define MAX42 0
#define BOOT42  0
#define MIN43 0
#define MAX43 0
#define BOOT43  0
#define MIN44 0
#define MAX44 0
#define BOOT44  0
#define MIN45 0
#define MAX45 0
#define BOOT45  0
#define MIN46 0
#define MAX46 0
#define BOOT46  0
#define MIN47 0
#define MAX47 0
#define BOOT47  0
#define MIN48 0
#define MAX48 0
#define BOOT48  0

int servo_select = 16;
int mode_select = 0;
int angle_servo = 0;
int pulse = 0;

//Driver :
Adafruit_PWMServoDriver board1 = Adafruit_PWMServoDriver(0x40);
Adafruit_PWMServoDriver board2 = Adafruit_PWMServoDriver(0x41);
Adafruit_PWMServoDriver board3 = Adafruit_PWMServoDriver(0x42);

//Servos :
uint8_t servomoteur[nb_servo] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48}; // défini sur quelle voie du driver sera branché chaque servomoteurs

//Liste des longueurs minimum d'impulsions pour - 90°
uint16_t servomoins90[nb_servo] = {
  MIN0 , MIN1  , MIN2  , MIN3  , MIN4  , MIN5  , MIN6  , MIN7  , MIN8  , MIN9  , MIN10 , MIN11 , MIN12 , MIN13 , MIN14 , MIN15 , MIN16 , MIN17 , MIN18 , MIN19 , MIN20 , MIN21 , MIN22 , MIN23 , MIN24 , MIN25 , MIN26 , MIN27 , MIN28 , MIN29 , MIN30 , MIN31 , MIN32 , MIN33 , MIN34 , MIN35 , MIN36 , MIN37 , MIN38 , MIN39 , MIN40 , MIN41 , MIN42 , MIN43 , MIN44 , MIN45 , MIN46 , MIN47 , MIN48
};

//Liste des longueurs minimum d'impulsions pour + 90°
uint16_t servoplus90[nb_servo] = {
  MAX0  , MAX1  , MAX2  , MAX3  , MAX4  , MAX5  , MAX6  , MAX7  , MAX8  , MAX9  , MAX10 , MAX11 , MAX12 , MAX13 , MAX14 , MAX15 , MAX16 , MAX17 , MAX18 , MAX19 , MAX20 , MAX21 , MAX22 , MAX23 , MAX24 , MAX25 , MAX26 , MAX27 , MAX28 , MAX29 , MAX30 , MAX31 , MAX32 , MAX33 , MAX34 , MAX35 , MAX36 , MAX37 , MAX38 , MAX39 , MAX40 , MAX41 , MAX42 , MAX43 , MAX44 , MAX45 , MAX46 , MAX47 , MAX48
};

//Liste des longueurs minimum d'impulsions pour + 90°
int servo_boot[nb_servo] = {
  BOOT0  , BOOT1 , BOOT2 , BOOT3 , BOOT4 , BOOT5 , BOOT6 , BOOT7 , BOOT8 , BOOT9 , BOOT10  , BOOT11  , BOOT12  , BOOT13  , BOOT14  , BOOT15  , BOOT16  , BOOT17  , BOOT18  , BOOT19  , BOOT20  , BOOT21  , BOOT22  , BOOT23  , BOOT24  , BOOT25  , BOOT26  , BOOT27  , BOOT28  , BOOT29  , BOOT30  , BOOT31  , BOOT32  , BOOT33  , BOOT34  , BOOT35  , BOOT36  , BOOT37  , BOOT38  , BOOT39  , BOOT40  , BOOT41  , BOOT42  , BOOT43  , BOOT44  , BOOT45  , BOOT46  , BOOT47  , BOOT48
};
void setup()
{

  board1.begin();
  board1.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~60 Hz updates

  board2.begin();
  board2.setPWMFreq(SERVO_FREQ);

  board3.begin();
  board3.setPWMFreq(SERVO_FREQ);

  lcd.begin(16, 2);              // Démarrage de l'écran
  lcd.clear();

  // centrage des servo
  for (int i = 0; i < nb_servo ; i++) {
    pulse = map(servo_boot[i], -90, 90, servomoins90[i], servoplus90[i]);

    // selection de la carte de sortie
    if (i <= 15) {
      board1.setPWM(servomoteur[i], 0, pulse);
    }
    if (i >= 16 && i < 32) {
      board2.setPWM(servomoteur[i - 16], 0, pulse);
    }
    if (i >= 32 && i < 48) {
      board3.setPWM(servomoteur[i - 32], 0, pulse);
    }

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("SERVO ");
    lcd.print(i);
    lcd.setCursor(0, 1);
    lcd.print("CENTER");
    delay(PAUSE / 10);
  }

  lcd.clear();
  lcd.setCursor(0, 0);           // Positionnement du curseur au début
  lcd.print("CORBACS MIDI CC"); // Message
  lcd.setCursor(0, 1);
  lcd.print("INIT OK");

}
// Fonction de lecture des touches
int read_LCD_buttons()
{
  adc_key_in = analogRead(0);   // Lecture du port analogique
  // Les valeurs qui suivent doivent être adaptées au shield
  if (adc_key_in > 900) return btnNONE;   // En principe 1023 quand aucune touche n'est pressée
  if (adc_key_in < 50)   return btnRIGHT;     // 0
  if (adc_key_in < 195)  return btnUP;        // 99
  if (adc_key_in < 380)  return btnDOWN;      // 255
  if (adc_key_in < 555)  return btnLEFT;      // 409
  if (adc_key_in < 790)  return btnSELECT;    // 640
  return btnNONE;
}

void loop()
{
  lcd_key = read_LCD_buttons();  // Lecture des touches
  switch (lcd_key)               // Action en cas de touche pressée
  {
    case btnRIGHT: // incrémente l'angle de 1°
      {
        if (angle_servo < 90) {
          angle_servo++;
        }
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("SERVO ");
        lcd.print(servo_select);
        lcd.print(" ANG ");
        lcd.print(angle_servo);
        servo_boot[servo_select] = angle_servo;
        pulse = map(angle_servo, -90, 90, servomoins90[servo_select], servoplus90[servo_select]);

        // selection de la carte de sortie
        if (servo_select <= 15) {
          board1.setPWM(servomoteur[servo_select], 0, pulse);
        }
        if (servo_select >= 16 && servo_select < 32) {
          board2.setPWM(servomoteur[servo_select - 16], 0, pulse);
        }

        if (servo_select >= 32 && servo_select < 48) {
          board3.setPWM(servomoteur[servo_select - 32], 0, pulse);
        }


        delay(PAUSE);
        lcd.setCursor(0, 1);
        lcd.print("PULSE : ");
        lcd.print(pulse);
        break;
      }

    case btnLEFT: // diminue l'angle de 1°
      {
        if (angle_servo > -90) {
          angle_servo--;
        }
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("SERVO ");
        lcd.print(servo_select);
        lcd.print(" ANG ");
        lcd.print(angle_servo);
        // mets a jour la valeur d'angle par servo
        servo_boot[servo_select] = angle_servo;
        pulse = map(angle_servo, -90, 90, servomoins90[servo_select], servoplus90[servo_select]);

        // selection de la carte de sortie
        if (servo_select <= 15) {
          board1.setPWM(servomoteur[servo_select], 0, pulse);
        }
        if (servo_select >= 16 && servo_select < 32) {
          board2.setPWM(servomoteur[servo_select - 16], 0, pulse);
        }

        if (servo_select >= 32 && servo_select < 48) {
          board3.setPWM(servomoteur[servo_select - 32], 0, pulse);
        }

        delay(PAUSE);
        lcd.setCursor(0, 1);
        lcd.print("PULSE : ");
        lcd.print(pulse);
        break;
      }

    case btnDOWN: // change de servo ++
      {
        if (servo_select < nb_servo - 1) {
          servo_select++;
        }
        else if (servo_select >= nb_servo - 1) {
          servo_select = 0;
        }
        // mets a jour la valeur d'angle par servo
        angle_servo = servo_boot[servo_select];
        pulse = map(angle_servo, -90, 90, servomoins90[servo_select], servoplus90[servo_select]);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("SERVO ");
        lcd.print(servo_select);
        lcd.print(" ANG ");
        lcd.print(angle_servo);
        lcd.setCursor(0, 1);
        lcd.print("PULSE : ");
        lcd.print(pulse);
        delay(PAUSE);
        break;
      }


    case  btnUP:  // change de servo --
      {
        if (servo_select > 0) {
          servo_select--;
        }
        else if (servo_select <= 0) {
          servo_select = nb_servo - 1;
        }
        // mets a jour la valeur d'angle par servo
        angle_servo = servo_boot[servo_select];
        pulse = map(angle_servo, -90, 90, servomoins90[servo_select], servoplus90[servo_select]);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("SERVO ");
        lcd.print(servo_select);
        lcd.print(" ANG ");
        lcd.print(angle_servo);
        lcd.setCursor(0, 1);
        lcd.print("PULSE : ");
        lcd.print(pulse);
        delay(PAUSE);
        break;
      }



    case btnSELECT:         // centrage des servo
      {
        break;
      }


    case btnNONE:
      {

        break;
      }
  }
}



