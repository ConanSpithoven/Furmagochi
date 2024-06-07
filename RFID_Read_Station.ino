#include <SPI.h> //RFID
#include <MFRC522.h> 
#include <Dabble.h> //bluetooth
#include <Servo.h>
#include <TinyGPS++.h> //GPS
#include <SoftwareSerial.h>

#define SS_PIN 10 //RFID
#define RST_PIN 9
byte readCard[4];
String Clean_UID = "E312CF5";  // Replace this with the UID of the tag!!!
String Feed_UID = "635962EE";
String tagID = "";
MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance

static const  int RXPin = 4, TXPin = 5; //GPS
TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);

void setup()
{
  //RFID
  Serial.begin(115200);
  SPI.begin(); // SPI bus
  mfrc522.PCD_Init(); // Initialise MFRC522
  mfrc522.PCD_DumpVersionToSerial();

  //bluetooth
  Dabble.begin(9600);
	Dabble.processInput();
  Terminal.println("starting");

  //GPS
  ss.begin(9600);
  gps.encode(ss.read());
  Terminal.println();
  Terminal.print("Latitude"); 
  Terminal.println(gps.location.lat(), 6);
  Terminal.print(" Longitude"); 
  Terminal.println(gps.location.lng(), 6);
}
void loop()
{
  //process bluetooth
  Dabble.processInput();

  //Wait until new tag is available
  if (readID())
    {
      if (tagID == Feed_UID)
      {
        Terminal.println("Voed Station");
      } else if (tagID = Clean_UID)
      {
        Terminal.println("Schoonmaak Station");
      } else {
        Terminal.println(tagID);
      }
    }

  if (ss.available() > 0)
    {
      gps.encode(ss.read());
      Serial.println(gps.satellites.value());
      if (gps.location.isUpdated()){
        Terminal.print("Latitude=  "); 
        Terminal.println(gps.location.lat(), 6);
        Terminal.print(" Longitude=  "); 
        Terminal.println(gps.location.lng(), 6);
    }
  }
}

  //Read new tag if available
boolean readID()
{
  //Check if a new tag is detected or not. If not return.
  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return false;
  }
  //Check if a new tag is readable or not. If not return.
  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return false;
  }
  tagID = "";
  // Read the 4 byte UID
  for ( uint8_t i = 0; i < 4; i++)
  {
    //readCard[i] = mfrc522.uid.uidByte[i];
    tagID.concat(String(mfrc522.uid.uidByte[i], HEX)); // Convert the UID to a single String
  }
  tagID.toUpperCase();
  mfrc522.PICC_HaltA(); // Stop reading
  return true;
}