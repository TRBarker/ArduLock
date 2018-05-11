
#include <SPI.h>
#include <MFRC522.h>
 
#define SS_PIN 10
#define RST_PIN 9
#define BUZZER_PIN 5
#define MOTOR_PIN 3
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
 

#define LED_ON HIGH
#define LED_OFF LOW

constexpr uint8_t redLed = 7;   // Set Led Pins
constexpr uint8_t greenLed = 6;
constexpr uint8_t yellowLed = 4;

extern String authCard = "";
extern String lockState = "locked";

void setup() 
{
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(yellowLed, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(MOTOR_PIN, OUTPUT);
  
  cycleLeds();
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Standing by to scan card.");
  Serial.println();

}
void loop() 
{
  
  digitalWrite(redLed, LED_OFF);  // Make sure red LED is off
  digitalWrite(greenLed, LED_OFF);  // Make sure green LED is off
  digitalWrite(yellowLed, LED_OFF);  // Make sure yellow LED is off
  
  flashYellow();
  
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "40 07 6D A3") //change here the UID of the card/cards that you want to give access
  {
    masterCard();
  }
  else{
  if (content.substring(1) == authCard) //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Authorised access");
    Serial.println();
   digitalWrite(redLed, LED_OFF);  // Make sure red LED is off
    digitalWrite(greenLed, LED_ON);   // Make sure green LED is on
    digitalWrite(MOTOR_PIN, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
    delay(100);
    digitalWrite(BUZZER_PIN, LOW);
    delay(100);
    digitalWrite(MOTOR_PIN, LOW);
    delay(3000);
  }
 
 else   {
    Serial.println(" Access denied");
    digitalWrite(redLed, LED_ON);
    digitalWrite(greenLed, LED_OFF);  // Make sure green LED is off
    digitalWrite(BUZZER_PIN, HIGH);
    delay(200);
    digitalWrite(BUZZER_PIN, LOW);
    delay(200);
    digitalWrite(BUZZER_PIN, HIGH);
    delay(200);
    digitalWrite(BUZZER_PIN, LOW);
    delay(200);
    digitalWrite(BUZZER_PIN, HIGH);
    delay(200);
    digitalWrite(BUZZER_PIN, LOW);
  }
  }
} 
    /////////////////////////////////////////////
void masterCard(){
 
  Serial.println("Master card detected, scan a PICC to add or remove it and can the master card again to exit");
  Serial.println();
  int i = 0;

  for (i=0; ; ){
    cycleLeds();
    delay(3000);
    // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  cycleLeds();
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  cycleLeds();
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "40 07 6D A3") //change here the UID of the card/cards that you want to give access
  {
    Serial.print("Master card detected, exiting.");
    int j = 0;
    while(j<3){
    loopRed();
    j++;
    }
    i = 1;
    ;
  }
  else{
      if(content.substring(1) != authCard){
        authCard = content.substring(1);
        Serial.print("Card added.");
        Serial.println();
        i = 1;
      }
      else{
        authCard = "";
        Serial.print("Card removed.");
        Serial.println();
      }
  }
  }
  return;
}
    /////////////////////////////////////
void loopRed(){
     digitalWrite(redLed, LED_ON);
    digitalWrite(greenLed, LED_OFF);
    digitalWrite(yellowLed, LED_OFF);
    delay(200);
    digitalWrite(redLed, LED_OFF);
}

void cycleLeds() {
  digitalWrite(redLed, LED_OFF);  // Make sure red LED is off
  digitalWrite(greenLed, LED_ON);   // Make sure green LED is on
  delay(200);
  digitalWrite(redLed, LED_ON);   // Make sure red LED is on
  digitalWrite(greenLed, LED_OFF);  // Make sure green LED is off
  delay(200);
}

void flashYellow(){ //Low-power systems self-check to display whether power is running and the system is on
    digitalWrite(yellowLed, LED_ON);
    delay(150);
    digitalWrite(yellowLed, LED_OFF);
    delay(1500);
}