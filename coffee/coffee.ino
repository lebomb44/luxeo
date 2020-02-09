#include <SPI.h>
#include <PN532_SPI.h>
#include <PN532.h>

#define LED_PIN 3

PN532_SPI pn532spi(SPI, 10);
PN532 nfc(pn532spi);

uint8_t nfcUID[7] = { 0, 0, 0, 0, 0, 0, 0 };
uint8_t nfcUIDLength = 0;
uint8_t nfcKey[6] = { 0x41, 0x5A, 0x54, 0x45, 0x4B, 0x4D };
uint8_t errorNb = 0;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);

  nfc.begin();
  nfc.SAMConfig();
}

void printBlock(uint8_t blockNb) {
  uint8_t data_[17] = {0};
  if(1 == nfc.mifareclassic_ReadDataBlock(blockNb, data_)) {
    Serial.print("DATA Block "); Serial.print(blockNb); Serial.print(" read: ");
    for(uint8_t i=0; i<16; i++) { Serial.print(data_[i]/16, HEX); Serial.print(data_[i]%16, HEX); }
    Serial.println(); Serial.flush();
  } else { Serial.print("ERROR: Reading Block "); Serial.print(blockNb); Serial.println(" failed"); errorNb++; }
}

void writeBlock(uint8_t blockNb, uint8_t* data_) {
  if(1 == nfc.mifareclassic_WriteDataBlock(blockNb, data_)) {
    Serial.print("DATA Block "); Serial.print(blockNb); Serial.print(" written: ");
    for(uint8_t i=0; i<16; i++) { Serial.print(data_[i]/16, HEX); Serial.print(data_[i]%16, HEX); }
    Serial.println(); Serial.flush();
  } else { Serial.print("ERROR: Writing Block "); Serial.print(blockNb); Serial.println(" failed"); errorNb++; }
}

void loop() {
  digitalWrite(LED_PIN, LOW);
  errorNb = 0;
  for(uint8_t i=0; i<7; i++) { nfcUID[i] = 0; }
  if(1 == nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, nfcUID, &nfcUIDLength, 100, false)) {
    nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, nfcUID, &nfcUIDLength);
    Serial.print("UID: ");
    for(uint8_t i=0; i<7; i++) { Serial.print(nfcUID[i]/16, HEX); Serial.print(nfcUID[i]%16, HEX); }
    Serial.println(); Serial.flush();

    Serial.println("#### BLOCKS 36 -> 39 ####");
    nfcKey[0] = 0x41; nfcKey[1] = 0x5A; nfcKey[2] = 0x54; nfcKey[3] = 0x45; nfcKey[4] = 0x4B; nfcKey[5] = 0x4D;
    if(1 == nfc.mifareclassic_AuthenticateBlock(nfcUID, nfcUIDLength, 36, 1, nfcKey)) {
      printBlock(36);
      printBlock(37);
      printBlock(38);
      printBlock(39);
      uint8_t data[16] = {0};
      data[0]=0xF0; data[1]=0x02; data[2]=0x38; data[3]=0x32; data[4]=0xB4; data[5]=0x1F; data[6]=0xFC; data[7]=0x81;
      data[8]=0xC1; data[9]=0xAF; data[10]=0x34; data[11]=0xBE; data[12]=0xF8; data[13]=0x22; data[14]=0x6C; data[15]=0x76;
      writeBlock(36, data);
      data[0]=0xE0; data[1]=0x20; data[2]=0x71; data[3]=0x76; data[4]=0x50; data[5]=0xF2; data[6]=0xD5; data[7]=0x93;
      data[8]=0xE0; data[9]=0x20; data[10]=0x71; data[11]=0x76; data[12]=0x50; data[13]=0xF2; data[14]=0xD5; data[15]=0x93;
      writeBlock(37, data);
      data[0]=0xD4; data[1]=0xB8; data[2]=0x9D; data[3]=0x54; data[4]=0x70; data[5]=0x35; data[6]=0x64; data[7]=0xC2;
      data[8]=0xF6; data[9]=0x9C; data[10]=0x00; data[11]=0x2C; data[12]=0xC2; data[13]=0xF2; data[14]=0xC8; data[15]=0x38;
      writeBlock(38, data);

      printBlock(36);
      printBlock(37);
      printBlock(38);
      printBlock(39);
    }
    else { Serial.println("ERROR: Authenticate Block 36 failed"); errorNb++; }

    Serial.println("#### BLOCKS 40 -> 43 ####");
    nfcKey[0] = 0x41; nfcKey[1] = 0x5A; nfcKey[2] = 0x54; nfcKey[3] = 0x45; nfcKey[4] = 0x4B; nfcKey[5] = 0x4D;
    if(1 == nfc.mifareclassic_AuthenticateBlock(nfcUID, nfcUIDLength, 40, 1, nfcKey)) {
      printBlock(40);
      printBlock(41);
      printBlock(42);
      printBlock(43);
      uint8_t data[16] = {0};
      data[0]=0x8E; data[1]=0x8F; data[2]=0x42; data[3]=0x4A; data[4]=0x93; data[5]=0x5F; data[6]=0xA1; data[7]=0xC1;
      data[8]=0xE2; data[9]=0x9D; data[10]=0xAF; data[11]=0x62; data[12]=0x81; data[13]=0x22; data[14]=0xBE; data[15]=0xA8;
      writeBlock(40, data);
      data[0]=0xAC; data[1]=0x39; data[2]=0x85; data[3]=0xC4; data[4]=0x24; data[5]=0x8A; data[6]=0xB5; data[7]=0xBC;
      data[8]=0x78; data[9]=0xB1; data[10]=0xD9; data[11]=0x69; data[12]=0xD4; data[13]=0x3D; data[14]=0x03; data[15]=0xA6;
      writeBlock(41, data);
      data[0]=0x84; data[1]=0xDF; data[2]=0x4D; data[3]=0x24; data[4]=0xC1; data[5]=0xD8; data[6]=0xB7; data[7]=0x87;
      data[8]=0xE0; data[9]=0x20; data[10]=0x71; data[11]=0x76; data[12]=0x50; data[13]=0xF2; data[14]=0xD5; data[15]=0x93; 
      writeBlock(42, data);
      
      printBlock(40);
      printBlock(41);
      printBlock(42);
      printBlock(43);
    }
    else { Serial.println("ERROR: Authenticate Block 40 failed"); errorNb++; }

    Serial.println("#### BLOCKS 44 -> 47 ####");
    nfcKey[0] = 0x41; nfcKey[1] = 0x5A; nfcKey[2] = 0x54; nfcKey[3] = 0x45; nfcKey[4] = 0x4B; nfcKey[5] = 0x4D;
    if(1 == nfc.mifareclassic_AuthenticateBlock(nfcUID, nfcUIDLength, 44, 1, nfcKey)) {
      printBlock(44);
      printBlock(45);
      printBlock(46);
      printBlock(47);
      uint8_t data[16] = {0};
      data[0]=0xD4; data[1]=0xB8; data[2]=0x9D; data[3]=0x54; data[4]=0x70; data[5]=0x35; data[6]=0x64; data[7]=0xC2;
      data[8]=0x52; data[9]=0xC3; data[10]=0xE1; data[11]=0x5D; data[12]=0x0A; data[13]=0xDE; data[14]=0xCE; data[15]=0x01;
      writeBlock(44, data);
      data[0]=0xED; data[1]=0x60; data[2]=0x07; data[3]=0x5C; data[4]=0x61; data[5]=0x37; data[6]=0x0A; data[7]=0xB3;
      data[8]=0x77; data[9]=0x92; data[10]=0x0F; data[11]=0x57; data[12]=0x75; data[13]=0x69; data[14]=0x89; data[15]=0x7D;
      writeBlock(45, data);

      printBlock(44);
      printBlock(45);
      printBlock(46);
      printBlock(47);
    }
    else { Serial.println("ERROR: Authenticate Block 44 failed"); errorNb++; }

    if(0 == errorNb) { Serial.println("No error"); digitalWrite(LED_PIN, HIGH); }
    else { Serial.println("Some errors found..."); digitalWrite(LED_PIN, LOW); }
    while(nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, nfcUID, &nfcUIDLength, 100, false));
  }
  delay(1);
}
