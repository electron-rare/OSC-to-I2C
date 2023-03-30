/*---------------------------------------------------------------------------------------------------------------------------------------------
  OSC to I2C CORBACS Show pour les amis nos morts         v0.1a 06/2022   06/2022
  https://www.facebook.com/LesAmisNosMorts/
  https://lesamisnosmorts.fr/
  https://www.youtube.com/watch?v=2guq3Z296mI&list=PLyRucERHaznPcJ69G39vllQ96x3izfdkT

  Conception artistique : Guillaume dalin
  lesamisnosmorts@gmail.com / +33676745128

  Code par Clément SAILLANT / l'électron rare
    c.saillant@gmail.com / +33625334420
  ---------------------------------------------------------------------------------------------------------------------------------------------*/

#define DEBUG // pour activer le debug serie

// ethernet
#include <SPI.h>
#include <NativeEthernet.h>
#include <NativeEthernetUdp.h>

byte mac[] = {0x90, 0xA2, 0xDA, 0x0E, 0x9B, 0x7F}; // mac address
byte ip[] = {192, 168, 2, 115};                    // ip address
const int osc_server_port = 10000;                 // port d'écoute OSC

EthernetUDP Udp;

// OSC
#include <OSCBundle.h>
#include <OSCBoards.h>

// I2C et carte PWM(servo)
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// Driver :
Adafruit_PWMServoDriver board1 = Adafruit_PWMServoDriver(0x40);
Adafruit_PWMServoDriver board2 = Adafruit_PWMServoDriver(0x41);
Adafruit_PWMServoDriver board3 = Adafruit_PWMServoDriver(0x42);

#include "variables.h" // Définition des variables
#include "sender.h"      // fonction d'envoie sur servo
#include "OSCMessage.h" // Définition des message OSC

void setup()
{

#ifdef DEBUG
  // Debug Mode enable in case DEBUG equal 1
  Serial.begin(115200);
  delay(5000);
  Serial.println("CORBAC I2C START");
#endif

  // start the Ethernet connection:
  Ethernet.begin(mac, ip);
  Udp.begin(osc_server_port);

  // Déifinition des cartes de I2C/PWM
  board1.begin();
  board1.setPWMFreq(SERVO_FREQ); // Analog servos run at ~60 Hz updates

  board2.begin();
  board2.setPWMFreq(SERVO_FREQ);

  board3.begin();
  board3.setPWMFreq(SERVO_FREQ);

  // centrage des servo
  for (int i = 0; i < nb_servo; i++)
  {
    int pulse = map(servo_boot[i], -90, 90, servomoins90[i], servoplus90[i]);

    // selection de la carte de sortie
    if (i < nb_out_board) // 0 à 15
    {
      board1.setPWM(servomoteur[i], 0, pulse);
    }
    else if (i < nb_out_board * 2) // 16 à 31
    {
      board2.setPWM(servomoteur[i - 16], 0, pulse);
    }
    else if (i < nb_out_board * 3) // 32 à 47
    {
      board3.setPWM(servomoteur[i - 32], 0, pulse);
    }
  }

#ifdef DEBUG
  // print your local IP address:
  Serial.print("Arduino IP address: ");
  Serial.println(Ethernet.localIP());
  Serial.print("OSC Port ");
  Serial.println(osc_server_port);
  Serial.println("Setup done.");
#endif
}

void loop()
{ // Main loop
  OSCMessage msgIN;
  int size;

  if ((size = Udp.parsePacket()) > 0)
  {
    while (size--)
      msgIN.fill(Udp.read());

    if (!msgIN.hasError())
    {

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
    }
  }
}
