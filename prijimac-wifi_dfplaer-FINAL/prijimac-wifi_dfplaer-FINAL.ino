
#include <SPI.h>
#include "RF24.h"
#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"


RF24 myRadio (7, 8); // CE, CS (custom)
byte addresses[][6] = {"Sx001"};

SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);

struct dataStruct {
  short saberPrefix;
  short soundType;
  short soundNumber;
} dataReceived;

void setup() 
{
  
  mySoftwareSerial.begin(9600);
  
  Serial.begin(115200);
  delay(1000);


        pinMode(7, OUTPUT);     
        digitalWrite(7, LOW);
        pinMode(8, OUTPUT);     
        digitalWrite(8, LOW);

        SPI.begin();
  
  Serial.println(F("RF24+dfplayermini-recever"));

  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  
  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }
  Serial.println(F("DFPlayer Mini online."));

  myDFPlayer.volume(20);  //Set volume value. From 0 to 30
  myDFPlayer.play(1);  //Play the first mp3

  //----------------------------
  delay(1000);

  myRadio.begin();  // Start up the physical nRF24L01 Radio
  myRadio.setChannel(108);  // Above most Wifi Channels
  // Set the PA Level low to prevent power supply related issues since this is a
  // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate(RF24_250KBPS);
  //  myRadio.setPALevel(RF24_PA_MAX);  // Uncomment for more power

  myRadio.openReadingPipe(1, addresses[0]); // Use the first entry in array 'addresses' (Only 1 right now)
  myRadio.startListening();

  Serial.println(F("RF24 online."));

}//--(end setup )---


void loop()   /****** LOOP: RUNS CONSTANTLY ******/
{
  myDFPlayer.loop(3);
  
  if ( myRadio.available()) // Check for incoming data from transmitter
  {
    while (myRadio.available())  // While there is data ready
    {
      myRadio.read( &dataReceived, sizeof(dataReceived) ); // Get the data payload (You must have defined that already!)
    }

    Serial.println("loop");
    myDFPlayer.loop(3);

    if (dataReceived.saberPrefix == 11001)
    {


      
      int playingData = dataReceived.soundNumber;
      Serial.println("yes");

      if (myDFPlayer.available()) {
        Serial.println(myDFPlayer.readType());
        Serial.println(myDFPlayer.read());
      }

      myDFPlayer.play(playingData);
    }

    
    Serial.println(dataReceived.saberPrefix);
    Serial.println(dataReceived.soundType);
    Serial.println(dataReceived.soundNumber);

    Serial.println("-----------------------------");
  } //END Radio available

}//--(end main loop )---

/*-----( Declare User-written Functions )-----*/

//None yet
//*********( THE END )***********
