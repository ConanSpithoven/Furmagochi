#include <MFRC522.h>

#define RST_PIN         9          
#define SS_PIN          10        

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

// Define the UIDs of your tags here
const String WASHING_TAG_UID = "UID1";
const String BED_TAG_UID = "UID2";
const String FOOD_TAG_UID = "UID3";

void setup() {
  Serial.begin(9600);        // Initialize serial communications with the PC
  SPI.begin();               // Init SPI bus
  mfrc522.PCD_Init();        // Init MFRC522 card
}

void loop() {
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // Show UID on serial monitor
  String uid = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    uid += mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ";
    uid += String(mfrc522.uid.uidByte[i], HEX);
  }
  uid.trim();

  if (uid == WASHING_TAG_UID) {
    Serial.println("Washing tag detected");
    // Add your code for washing tag here
  } else if (uid == BED_TAG_UID) {
    Serial.println("Bed tag detected");
    // Add your code for bed tag here
  } else if (uid == FOOD_TAG_UID) {
    Serial.println("Food tag detected");
    // Add your code for food tag here
  } else {
    Serial.println("Unknown tag detected");
  }
}