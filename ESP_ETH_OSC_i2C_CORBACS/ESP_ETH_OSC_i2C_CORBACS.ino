/*---------------------------------------------------------------------------------------------------------------------------------------------
  OSC to I2C CORBACS Show  pour les amis nos morts         v0.1a 06/2022   06/2022
  https://www.facebook.com/LesAmisNosMorts/
  https://lesamisnosmorts.fr/
  https://www.youtube.com/watch?v=2guq3Z296mI&list=PLyRucERHaznPcJ69G39vllQ96x3izfdkT

  Conception artistique : Guillaume dalin
  lesamisnosmorts@gmail.com / +33676745128

  Code par Clément SAILLANT / EcObsolent
    c.saillant@gmail.com / +33625334420


    librairie :
  https://github.com/wemos/WEMOS_Motor_Shield_Arduino_Library
  https://github.com/UIPEthernet/UIPEthernet
  https://github.com/CNMAT/OSC
  https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library
  ---------------------------------------------------------------------------------------------------------------------------------------------*/


#include "config.h"  // fichier de Définition des valeur de servo
#include "defines.h" // config ethernet
// Ethernet
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>

#define ASYNC_UDP_ETHERNET_VERSION_MIN_TARGET      "AsyncUDP_Ethernet v1.2.1"
#define ASYNC_UDP_ETHERNET_VERSION_MIN             1002001

#include <Ticker.h>

// 600s = 10 minutes to not flooding, 60s in testing
#define UDP_REQUEST_INTERVAL     60

#define UDP_REMOTE_PORT         10000

AsyncUDP udp;

// OSC instance to let us send and receive OSC packet
// const unsigned int localPort = 10000; //port numbers
#include <OSCBundle.h>
#include <OSCBoards.h>
#include <OSCMessage.h>
OSCErrorCode error;

// I2C et carte PWM(servo)
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>


//Driver servo pwm :
Adafruit_PWMServoDriver board1 = Adafruit_PWMServoDriver(0x40);
Adafruit_PWMServoDriver board2 = Adafruit_PWMServoDriver(0x41);
Adafruit_PWMServoDriver board3 = Adafruit_PWMServoDriver(0x42);

//Servos :
#define nb_servo 61  // Nombre de servo controlés + 1
int servo_select = 0; // variable de selection servo
//Liste des adresse moteur
uint8_t servomoteur[nb_servo] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60};
//Liste des longueurs minimum d'impulsions pour - 90°
uint16_t servomoins90[nb_servo] = {
  MIN0 , MIN1  , MIN2  , MIN3  , MIN4  , MIN5  , MIN6  , MIN7  , MIN8  , MIN9  , MIN10 , MIN11 , MIN12 , MIN13 , MIN14 , MIN15 , MIN16 , MIN17 , MIN18 , MIN19 , MIN20 , MIN21 , MIN22 , MIN23 , MIN24 , MIN25 , MIN26 , MIN27 , MIN28 , MIN29 , MIN30 , MIN31 , MIN32 , MIN33 , MIN34 , MIN35 , MIN36 , MIN37 , MIN38 , MIN39 , MIN40 , MIN41 , MIN42 , MIN43 , MIN44 , MIN45 , MIN46 , MIN47 , MIN48, MIN49, MIN50, MIN51, MIN52, MIN53, MIN54, MIN55, MIN56, MIN57, MIN58, MIN59, MIN60
};
//Liste des longueurs minimum d'impulsions pour + 90°
uint16_t servoplus90[nb_servo] = {
  MAX0  , MAX1  , MAX2  , MAX3  , MAX4  , MAX5  , MAX6  , MAX7  , MAX8  , MAX9  , MAX10 , MAX11 , MAX12 , MAX13 , MAX14 , MAX15 , MAX16 , MAX17 , MAX18 , MAX19 , MAX20 , MAX21 , MAX22 , MAX23 , MAX24 , MAX25 , MAX26 , MAX27 , MAX28 , MAX29 , MAX30 , MAX31 , MAX32 , MAX33 , MAX34 , MAX35 , MAX36 , MAX37 , MAX38 , MAX39 , MAX40 , MAX41 , MAX42 , MAX43 , MAX44 , MAX45 , MAX46 , MAX47 , MAX48, MAX49, MAX50, MAX51, MAX52, MAX53, MAX54, MAX55, MAX56, MAX57, MAX58, MAX59, MAX60
};
//Liste des longueurs minimum d'impulsions pour + 90°
int servo_boot[nb_servo] = {
  BOOT0  , BOOT1 , BOOT2 , BOOT3 , BOOT4 , BOOT5 , BOOT6 , BOOT7 , BOOT8 , BOOT9 , BOOT10  , BOOT11  , BOOT12  , BOOT13  , BOOT14  , BOOT15  , BOOT16  , BOOT17  , BOOT18  , BOOT19  , BOOT20  , BOOT21  , BOOT22  , BOOT23  , BOOT24  , BOOT25  , BOOT26  , BOOT27  , BOOT28  , BOOT29  , BOOT30  , BOOT31  , BOOT32  , BOOT33  , BOOT34  , BOOT35  , BOOT36  , BOOT37  , BOOT38  , BOOT39  , BOOT40  , BOOT41  , BOOT42  , BOOT43  , BOOT44  , BOOT45  , BOOT46  , BOOT47  , BOOT48, BOOT49, BOOT50, BOOT51, BOOT52, BOOT53, BOOT54, BOOT55, BOOT56, BOOT57, BOOT58, BOOT59, BOOT560
};

// valeur min et max OSC pour mappage des valeurs vers angle SERVO
#define min_value 0
#define max_value 1000
#define USMIN  600 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX  2400 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates
int osc_value[nb_servo]; // pour stockage valeur par servo
int osc_value_old[nb_servo]; // pour stockage valeur osc
int pulse;

void initEthernet()
{
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV4);
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);

#if !USING_DHCP
  eth.config(localIP, gateway, netMask, gateway);
#endif

  eth.setDefault();

  if (!eth.begin())
  {
    Serial.println("No Ethernet hardware ... Stop here");

    while (true)
    {
      delay(1000);
    }
  }
  else
  {
    Serial.print("Connecting to network : ");

    while (!eth.connected())
    {
      Serial.print(".");
      delay(1000);
    }
  }

  Serial.println();

#if USING_DHCP
  Serial.print("Ethernet DHCP IP address: ");
#else
  Serial.print("Ethernet Static IP address: ");
#endif

  Serial.println(eth.localIP());
}

void setup() {
  Serial.begin(115200);
  initEthernet();
  // Déifinition des cartes de I2C/PWM
  board1.begin();
  board1.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~60 Hz updates

  board2.begin();
  board2.setPWMFreq(SERVO_FREQ);

  board3.begin();
  board3.setPWMFreq(SERVO_FREQ);

  delay(100);

  // centrage des servo
  Serial.println("CENTRAGE CORBAC");
  Serial.println("--------------- ---------------------");
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
    Serial.print(".");
  }

  delay(100);
  Serial.println("CORBAC I2C START");
  Serial.println("------------------------------------");
} // fin setup

void loop() { // Main loop
  OSCMessage msgIN;

  udp.onPacket([](AsyncUDPPacket packet)
  {
    msgIN.fill(parsePacket(packet));

    if (!msgIN.hasError()) {

      /*// -------------------------------------------------------------------------------------------------------------------------------------------
        // pour appel fonction reception adresse osc;

        definir dans le main loop :
        msgIN.route("/ADDRESSEOSC", ADDRESSEOSC);

        definir numero de moteur pour variable servo servo_select = NUMERO DE MOTEUR;;
        utilisé la même adresse OSC pour la fonction comme indiqué plus bas
        // ---------------------------------------------------------------------------------------------------------------------------------------------*/

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

      Serial.println("Reception d'un message OSC");
      //pour detection de mise à jour des valeur de servo
      for (int servo_select = 0; servo_select < nb_servo ; servo_select++) {
        if (osc_value_old[servo_select] != osc_value[servo_select]) // si servo doit etre mis à jour avec nouvelle valeur OSC reçu
        {
          int servo_value = map(osc_value[servo_select], min_value, max_value, -90, 90);
          pulse = map(servo_value, -90, 90, servomoins90[servo_select], servoplus90[servo_select]);
          //if (pulse[servo_select] != pulse_old[servo_select]) {
          Serial.println("------------------------------------");
          Serial.print("mise à jour position moteur n° : ");
          Serial.println(servo_select);
          Serial.print("valeur osc : ");
          Serial.print(osc_value[servo_select]);
          Serial.print("    ------    valeur pulse : ");
          Serial.println(pulse);
          // selection de la carte de sortie
          // moteur sur carte 1
          if (servo_select <= 15) {
            board1.setPWM(servomoteur[servo_select], 0, pulse);
          }
          // moteur sur carte 2
          if (servo_select >= 16 && servo_select < 32) {
            board2.setPWM(servomoteur[servo_select - 16], 0, pulse);
          }
          // moteur sur carte 3
          if (servo_select >= 32 && servo_select < 48) {
            board3.setPWM(servomoteur[servo_select - 32], 0, pulse);
          }
          //pulse_old[servo_select] = pulse[servo_select]; // mise a jour variable ancienne valeur de moteur
          //}

          osc_value_old[servo_select] = osc_value[servo_select];
        } // fin de mise à jour servo
      } // fin detection pour mise à jour servo
      //udp.stop();  //restart with new connection to receive packets from other clients
    } // fin message UDP OSC reçu
  });


}
} // fin mail loop



/*// -------------------------------------------------------------------------------------------------------------------------------------------
  // Fonction reception adresse osc;

  definir dans le main loop :
  msgIN.route("/ADDRESSEOSC", ADDRESSEOSC);

  definir numero de moteur pour variable servo servo_select = NUMERO DE MOTEUR;;
  utilisé la même adresse OSC définie comme indiqué plus haut dans le main

  void ADDRESSEOSC(OSCMessage &msg, int addrOffset ) {
  servo_select = NUMERO DE MOTEUR;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
  }

  // ---------------------------------------------------------------------------------------------------------------------------------------------*/


void gd1(OSCMessage & msg, int addrOffset ) {
  servo_select = 0;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
}
void gd2(OSCMessage & msg, int addrOffset ) {
  servo_select = 1;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
}
void gd3(OSCMessage & msg, int addrOffset ) {
  servo_select = 10;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
}
void gd4(OSCMessage & msg, int addrOffset ) {
  servo_select = 11;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
}
void gd5(OSCMessage & msg, int addrOffset ) {
  servo_select = 16;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
}
void gd6(OSCMessage & msg, int addrOffset ) {
  servo_select = 17;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
}
void gd7(OSCMessage & msg, int addrOffset ) {
  servo_select = 26;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
}
void gd8(OSCMessage & msg, int addrOffset ) {
  servo_select = 27;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
}



void c1(OSCMessage & msg, int addrOffset ) {
  servo_select = 4;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
}
void c2(OSCMessage & msg, int addrOffset ) {
  servo_select = 5;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
}
void c3(OSCMessage & msg, int addrOffset ) {
  servo_select = 6;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
}
void c4(OSCMessage & msg, int addrOffset ) {
  servo_select = 7;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
}
void c5(OSCMessage & msg, int addrOffset ) {
  servo_select = 12;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
}
void c6(OSCMessage & msg, int addrOffset ) {
  servo_select = 13;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
}
void c7(OSCMessage & msg, int addrOffset ) {
  servo_select = 14;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
}
void c8(OSCMessage & msg, int addrOffset ) {
  servo_select = 15;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
}
void c9(OSCMessage & msg, int addrOffset ) {
  servo_select = 20;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
}
void c10(OSCMessage & msg, int addrOffset ) {
  servo_select = 21;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
}
void c11(OSCMessage & msg, int addrOffset ) {
  servo_select = 22;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
}
void c12(OSCMessage & msg, int addrOffset ) {
  servo_select = 23;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
}
void c13(OSCMessage & msg, int addrOffset ) {
  servo_select = 28;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
}
void c14(OSCMessage & msg, int addrOffset ) {
  servo_select = 29;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
}
void c15(OSCMessage & msg, int addrOffset ) {
  servo_select = 30;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
}
void c16(OSCMessage & msg, int addrOffset ) {
  servo_select = 31;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
}


void sheep(OSCMessage & msg, int addrOffset ) {
  servo_select = 32;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
}
void badger(OSCMessage & msg, int addrOffset ) {
  servo_select = 33;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
}
void cow1(OSCMessage & msg, int addrOffset ) {
  servo_select = 34;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
}
void cow2(OSCMessage & msg, int addrOffset ) {
  servo_select = 35;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
}
