//#include <nRF24L01.h>
#include <RF24.h>

#define PIN_CE  4
#define PIN_CSN 2

uint8_t pipeNumber;
uint8_t payloadSize;

RF24 radio(PIN_CE, PIN_CSN);

void setup() {

  Serial.begin(115200);
  radio.begin();
  radio.setChannel(5);
  radio.setDataRate(RF24_1MBPS);
  radio.setPALevel(RF24_PA_HIGH);
  radio.enableDynamicPayloads();

  radio.openReadingPipe (0, 0x7878787878LL);
  
  radio.printDetails();

  radio.startListening();

  Serial.println("Start listening...");

}

void loop() {

  if (radio.available(&pipeNumber)) {
  
    payloadSize = radio.getDynamicPayloadSize(); 
    char payload[payloadSize];
    String receivedData;
    radio.read(&payload, sizeof(payload));
 
    for (uint8_t i = 0; i < payloadSize; i++) {
        receivedData += payload[i];
    }
    
    Serial.print("Pipe : " + String(pipeNumber) + " ");
    Serial.print("Size : " + String (payloadSize) + " ");
    Serial.println("Data : " + receivedData);
    
    delay(100);    
  }
}
