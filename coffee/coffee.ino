#include <SPI.h>
#include <PN532_SPI.h>
#include <PN532.h>

#define LED_PIN 3

PN532_SPI pn532spi(SPI, 10);
PN532 nfc(pn532spi);

uint8_t nfcUID[7] = { 0, 0, 0, 0, 0, 0, 0 };
uint8_t nfcUIDLength = 0;
String nfcUIDs = "";
uint8_t nfcKey[6] = { 0x41, 0x5A, 0x54, 0x45, 0x4B, 0x4D };
uint8_t errorNb = 0;

class T_TAG {
public:
  String uid;
  String block36;
  String block37;
  String block38;
  String block40;
  String block41;
  String block42;
  String block44;
  String block45;
};

T_TAG allTags[2];

void init_allTags(void) {
  T_TAG * tag = NULL;
  tag = &allTags[0];
  tag->uid = String("25D563EC000000");
  tag->block36 = String("F0023832B41FFC81C1AF34BEF8226C76");
  tag->block37 = String("E020717650F2D593E020717650F2D593");
  tag->block38 = String("D4B89D54703564C2F69C002CC2F2C838");
  tag->block40 = String("8E8F424A935FA1C1E29DAF628122BEA8");
  tag->block41 = String("AC3985C4248AB5BC78B1D969D43D03A6");
  tag->block42 = String("84DF4D24C1D8B787E020717650F2D593");
  tag->block44 = String("D4B89D54703564C252C3E15D0ADECE01");
  tag->block45 = String("ED60075C61370AB377920F577569897D");
  tag = &allTags[1];
  tag->uid = String("F1EE3D25000000");
  tag->block36 = String("0FB650669C1740FEFFDB21B18E5BCD1C");
  tag->block37 = String("649AF198AA004C6C2AED16BA1537D106");
  tag->block38 = String("B0D2D23353FDC6BDB05DF0E29BB74E2B");
  tag->block40 = String("21DFAE40990044D9B5B9B20318DCAF6E");
  tag->block41 = String("EC8200DDBAC2B612FFDB21B18E5BCD1C");
  tag->block42 = String("351C7F2C12AADE2C2AED16BA1537D106");
  tag->block44 = String("B0D2D23353FDC6BDB05DF0E29BB74E2B");
  tag->block45 = String("21DFAE40990044D9581A1A07CDEC5744");
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);

  nfc.begin();
  nfc.SAMConfig();

  init_allTags();
}

void printBlock(uint8_t blockNb) {
  uint8_t data_[17] = {0};
  if(1 == nfc.mifareclassic_ReadDataBlock(blockNb, data_)) {
    Serial.print(F("DATA Block ")); Serial.print(blockNb); Serial.print(F(" read: "));
    for(uint8_t i=0; i<16; i++) { Serial.print(data_[i]/16, HEX); Serial.print(data_[i]%16, HEX); }
    Serial.println(); Serial.flush();
  } else { Serial.print(F("ERROR: Reading Block ")); Serial.print(blockNb); Serial.println(F(" failed")); errorNb++; }
}

void writeBlock(uint8_t blockNb, String sdata_) {
  uint8_t data[16] = {0};
  for(uint8_t i=0; i<(sizeof(data)/sizeof(data[0])); i++) {
    data[i] = strtoul((String(sdata_.charAt(2*i)) + String(sdata_.charAt((2*i)+1))).c_str(), NULL, 16);
  }
  if(1 == nfc.mifareclassic_WriteDataBlock(blockNb, data)) {
    Serial.print(F("DATA Block ")); Serial.print(blockNb); Serial.print(F(" written: "));
    for(uint8_t i=0; i<(sizeof(data)/sizeof(data[0])); i++) { Serial.print(data[i]/16, HEX); Serial.print(data[i]%16, HEX); }
    Serial.println(); Serial.flush();
  } else { Serial.print(F("ERROR: Writing Block ")); Serial.print(blockNb); Serial.println(F(" failed")); errorNb++; }
}

void loop() {
  digitalWrite(LED_PIN, LOW);
  errorNb = 0;
  for(uint8_t i=0; i<7; i++) { nfcUID[i] = 0; }
  nfcUIDLength = 0;
  nfcUIDs = "";
  if(1 == nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, nfcUID, &nfcUIDLength, 100, false)) {
    nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, nfcUID, &nfcUIDLength);
    Serial.print(F("UID: "));
    for(uint8_t i=0; i<7; i++) {
      String _uidHs = String(nfcUID[i]/16, HEX);
      _uidHs.toUpperCase();
      String _uidLs = String(nfcUID[i]%16, HEX);
      _uidLs.toUpperCase();
      nfcUIDs = nfcUIDs + _uidHs + _uidLs;
    }
    Serial.println(nfcUIDs); Serial.flush();
  }
  T_TAG * tag = NULL;
  for(uint8_t i=0; i<(sizeof(allTags)/sizeof(allTags[0])); i++) {
    if(nfcUIDs == allTags[i].uid) {
      tag = &allTags[i];
    }
  }
  if(NULL != tag) {
    Serial.println(F("#### BLOCKS 36 -> 39 ####"));
    //nfcKey[0] = 0x41; nfcKey[1] = 0x5A; nfcKey[2] = 0x54; nfcKey[3] = 0x45; nfcKey[4] = 0x4B; nfcKey[5] = 0x4D;
    if(1 == nfc.mifareclassic_AuthenticateBlock(nfcUID, nfcUIDLength, 36, 1, nfcKey)) {
      printBlock(36);
      printBlock(37);
      printBlock(38);
      printBlock(39);
      writeBlock(36, tag->block36);
      writeBlock(37, tag->block37);
      writeBlock(38, tag->block38);
      printBlock(36);
      printBlock(37);
      printBlock(38);
      printBlock(39);
    }
    else { Serial.println(F("ERROR: Authenticate Block 36 failed")); errorNb++; }

    Serial.println(F("#### BLOCKS 40 -> 43 ####"));
    //nfcKey[0] = 0x41; nfcKey[1] = 0x5A; nfcKey[2] = 0x54; nfcKey[3] = 0x45; nfcKey[4] = 0x4B; nfcKey[5] = 0x4D;
    if(1 == nfc.mifareclassic_AuthenticateBlock(nfcUID, nfcUIDLength, 40, 1, nfcKey)) {
      printBlock(40);
      printBlock(41);
      printBlock(42);
      printBlock(43);
      writeBlock(40, tag->block40);
      writeBlock(41, tag->block41);
      writeBlock(42, tag->block42);
      printBlock(40);
      printBlock(41);
      printBlock(42);
      printBlock(43);
    }
    else { Serial.println(F("ERROR: Authenticate Block 40 failed")); errorNb++; }

    Serial.println(F("#### BLOCKS 44 -> 47 ####"));
    //nfcKey[0] = 0x41; nfcKey[1] = 0x5A; nfcKey[2] = 0x54; nfcKey[3] = 0x45; nfcKey[4] = 0x4B; nfcKey[5] = 0x4D;
    if(1 == nfc.mifareclassic_AuthenticateBlock(nfcUID, nfcUIDLength, 44, 1, nfcKey)) {
      printBlock(44);
      printBlock(45);
      printBlock(46);
      printBlock(47);
      writeBlock(44, tag->block44);
      writeBlock(45, tag->block45);
      printBlock(44);
      printBlock(45);
      printBlock(46);
      printBlock(47);
    }
    else { Serial.println(F("ERROR: Authenticate Block 44 failed")); errorNb++; }

    if(0 == errorNb) { Serial.println(F("No error")); digitalWrite(LED_PIN, HIGH); }
    else { Serial.println(F("Some errors found...")); digitalWrite(LED_PIN, LOW); }
    while(nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, nfcUID, &nfcUIDLength, 100, false));
  }
  else {
    if(String("") != nfcUIDs) {
      Serial.print(F("Tag UNKNOWN ! : ")); Serial.println(nfcUIDs); Serial.flush();
    }
  }
  delay(1);
}
