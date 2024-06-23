#include <MFRC522.h>

#define RST_PIN         9
#define SS_PIN          10    

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

// Define the UIDs of your tags here
const byte TAG_UIDS[3][4] = {
  {0x12, 0x34, 0x56, 0x78}, // WASHING_TAG_UID
  {0x9A, 0xBC, 0xDE, 0xF0}, // BED_TAG_UID
  {0x23, 0x45, 0x67, 0x89}  // FOOD_TAG_UID
};

int currentTagUid = 0;

void setup() {
  Serial.begin(9600);        // Initialize serial communications with arduino
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

  // Write the current tag UID to the card
  byte buffer[18];
  byte block;
  MFRC522::StatusCode status;

  block = 0;
  memcpy(buffer, TAG_UIDS[currentTagUid], 4); // Copy the UID to the buffer
  status = mfrc522.MIFARE_Ultralight_Write(block, buffer, 4); // Write the buffer to the card
  if (status != MFRC522::STATUS_OK) {
    Serial.print("MIFARE_Ultralight_Write() failed: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  Serial.print("Tag ");
  Serial.print(currentTagUid + 1);
  Serial.println(" written");
  // Increment the current tag UID, wrapping around to 0 if it exceeds the number of UIDs
  currentTagUid = (currentTagUid + 1) % 3;
}