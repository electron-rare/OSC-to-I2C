/*---------------------------------------------------------------------------------------------------------------------------------------------
  OSC to I2C CORBACS Show  pour les amis nos morts         v0.1a 06/2022   06/2022
  https://www.facebook.com/LesAmisNosMorts/
  https://lesamisnosmorts.fr/
  https://www.youtube.com/watch?v=2guq3Z296mI&list=PLyRucERHaznPcJ69G39vllQ96x3izfdkT

  Conception artistique : Guillaume dalin
  lesamisnosmorts@gmail.com / +33676745128

  Code par Clément SAILLANT / EcObsolent
    c.saillant@gmail.com / +33625334420
  ---------------------------------------------------------------------------------------------------------------------------------------------*/


#include "config.h"  // fichier de Définition des valeur de servo

#define DEBUG 0 // 1 pour activer le debug serie

// ethernet et OSC
#include <SPI.h>
#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif
#include <WiFiUdp.h>

//Use youre mac and ip here---------------------------
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0E, 0x9B, 0x7F    };
byte ip[] =  { 192, 168, 2, 115  };
//----------------------------------------------------

char ssid[] = "La-clic";          // your network SSID (name)
char pass[] = "clic clac";                    // your network password

// A UDP instance to let us send and receive packets over UDP
WiFiUDP Udp;


// OSC--------------------------
//port numbers
const unsigned int localPort = 10000;
#include <OSCBundle.h>
#include <OSCBoards.h>
#include <OSCMessage.h>

OSCErrorCode error;
unsigned int ledState = LOW;              // LOW means led is *on*

#ifndef BUILTIN_LED
#ifdef LED_BUILTIN
#define BUILTIN_LED LED_BUILTIN
#else
#define BUILTIN_LED 13
#endif
#endif
//----------------------------------------------------





// valeur min et max OSC pour mappage des valeurs vers angle SERVO
#define min_value 0
#define max_value 1000



// I2C et carte PWM(servo)
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// Depending on your servo make, the pulse width min and max may vary, you
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!
#define USMIN  600 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX  2400 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

//Driver :
Adafruit_PWMServoDriver board1 = Adafruit_PWMServoDriver(0x40);
Adafruit_PWMServoDriver board2 = Adafruit_PWMServoDriver(0x41);
Adafruit_PWMServoDriver board3 = Adafruit_PWMServoDriver(0x42);


//Servos :
#define nb_servo 49  // Nombre de servo controlés

int servo_select = 0; // variable de selection servo
int osc_value[nb_servo]; // pour stockage valeur par servo
int osc_value_old[nb_servo]; // pour comparaison si changement de valeur
int pulse = 0;
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



void setup() {

 pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, ledState);    // turn *on* led

  Serial.begin(115200);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Starting UDP");
  Udp.begin(localPort);
  Serial.print("Local port: ");
#ifdef ESP32
  Serial.println(localPort);
#else
  Serial.println(Udp.localPort());
#endif

  // Déifinition des cartes de I2C/PWM
  board1.begin();
  board1.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~60 Hz updates

  board2.begin();
  board2.setPWMFreq(SERVO_FREQ);

  board3.begin();
  board3.setPWMFreq(SERVO_FREQ);

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
  }

#ifdef  DEBUG
  //Debug Mode enable in case DEBUG equal 1
  //Serial.begin(115200);
  delay(100);
  Serial.println("CORBAC I2C START");

#endif


}

void led(OSCMessage &msg) {
  ledState = msg.getInt(0);
  digitalWrite(BUILTIN_LED, ledState);
  Serial.print("/led: ");
  Serial.println(ledState);
}

void loop() { // Main loop
  OSCMessage msgIN;
  int size;

  if ( (size = Udp.parsePacket()) > 0)
  {
    while (size--)
      msgIN.fill(Udp.read());

    if (!msgIN.hasError()) {

      msgIN.route("/gd1", gd1);
      msgIN.route("/gd2", gd2);
      msgIN.route("/gd3", gd3);
      msgIN.route("/gd4", gd4);
      msgIN.route("/gd5", gd5);
      msgIN.route("/gd6", gd6);
      msgIN.route("/gd7", gd7);
      msgIN.route("/gd8", gd8);

      msgIN.route("/c1", c1);
      msgIN.route("/c2", c2);
      msgIN.route("/c3", c3);
      msgIN.route("/c4", c4);
      msgIN.route("/c5", c5);
      msgIN.route("/c6", c6);
      msgIN.route("/c7", c7);
      msgIN.route("/c8", c8);
      msgIN.route("/c9", c9);
      msgIN.route("/c10", c10);
      msgIN.route("/c11", c11);
      msgIN.route("/c12", c12);
      msgIN.route("/c13", c13);
      msgIN.route("/c14", c14);
      msgIN.route("/c15", c15);
      msgIN.route("/c16", c16);

      msgIN.route("/sheep", sheep);
      msgIN.route("/badger", badger);
      msgIN.route("/cow1", cow1);
      msgIN.route("/cow2", cow2);

      //Serial.println("OSC alive! ");
    }

  }
  // mise à jour des valeur de servo

  for (int servo_select = 0; servo_select < nb_servo ; servo_select++) {
    if (osc_value_old[servo_select] != osc_value[servo_select])
    {
#ifdef  DEBUG
      //Serial.println("mise à jour position moteur");
#endif

      osc_value_old[servo_select] = osc_value[servo_select];
      int servo_value = map(osc_value[servo_select], min_value, max_value, -90, 90);
      pulse = map(servo_value, -90, 90, servomoins90[servo_select], servoplus90[servo_select]);
      // selection de la carte de sortie
      if (servo_select <= 15) {
        board1.setPWM(servomoteur[servo_select], 0, pulse);
#ifdef  DEBUG
        //Serial.println("sur carte 1");
#endif
      }
      if (servo_select >= 16 && servo_select < 32) {
        board2.setPWM(servomoteur[servo_select - 16], 0, pulse);
#ifdef  DEBUG
        //Serial.println("sur carte 2");
#endif
      }
      if (servo_select >= 32 && servo_select < 48) {
        board3.setPWM(servomoteur[servo_select - 32], 0, pulse);
#ifdef  DEBUG
        //Serial.println("sur carte 3");
#endif
      }
#ifdef  DEBUG
      //Serial.print("moteur : ");
      //Serial.println(servo_select);
      //Serial.print("valeur osc : ");
      //Serial.println(osc_value[servo_select]);
      //Serial.print("valeur pulse : ");
      //Serial.println(pulse);
#endif
    }
  }


}




// Fonction reception adresse osc;
//definir numero de servo servo_select = 0;

void gd1(OSCMessage &msg, int addrOffset ) {
  servo_select = 0;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
}
void gd2(OSCMessage &msg, int addrOffset ) {
  servo_select = 1;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
}
void gd3(OSCMessage &msg, int addrOffset ) {
  servo_select = 10;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
}
void gd4(OSCMessage &msg, int addrOffset ) {
  servo_select = 11;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
}
void gd5(OSCMessage &msg, int addrOffset ) {
  servo_select = 16;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
}
void gd6(OSCMessage &msg, int addrOffset ) {
  servo_select = 17;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
}
void gd7(OSCMessage &msg, int addrOffset ) {
  servo_select = 26;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
}
void gd8(OSCMessage &msg, int addrOffset ) {
  servo_select = 27;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
}



void c1(OSCMessage &msg, int addrOffset ) {
  servo_select = 4;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
}
void c2(OSCMessage &msg, int addrOffset ) {
  servo_select = 5;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
}
void c3(OSCMessage &msg, int addrOffset ) {
  servo_select = 6;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
}
void c4(OSCMessage &msg, int addrOffset ) {
  servo_select = 7;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
}
void c5(OSCMessage &msg, int addrOffset ) {
  servo_select = 12;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
}
void c6(OSCMessage &msg, int addrOffset ) {
  servo_select = 13;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
}
void c7(OSCMessage &msg, int addrOffset ) {
  servo_select = 14;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
}
void c8(OSCMessage &msg, int addrOffset ) {
  servo_select = 15;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
}
void c9(OSCMessage &msg, int addrOffset ) {
  servo_select = 20;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
}
void c10(OSCMessage &msg, int addrOffset ) {
  servo_select = 21;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
}
void c11(OSCMessage &msg, int addrOffset ) {
  servo_select = 22;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
}
void c12(OSCMessage &msg, int addrOffset ) {
  servo_select = 23;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
}
void c13(OSCMessage &msg, int addrOffset ) {
  servo_select = 28;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
}
void c14(OSCMessage &msg, int addrOffset ) {
  servo_select = 29;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
}
void c15(OSCMessage &msg, int addrOffset ) {
  servo_select = 30;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
}
void c16(OSCMessage &msg, int addrOffset ) {
  servo_select = 31;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
}


void sheep(OSCMessage &msg, int addrOffset ) {
  servo_select = 32;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
}
void badger(OSCMessage &msg, int addrOffset ) {
  servo_select = 33;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
}
void cow1(OSCMessage &msg, int addrOffset ) {
  servo_select = 34;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
}
void cow2(OSCMessage &msg, int addrOffset ) {
  servo_select = 35;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
}
