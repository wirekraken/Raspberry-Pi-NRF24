//#include <nRF24L01.h>
#include <RF24.h>

#define PIN_CE  4
#define PIN_CSN 2

uint8_t pipeNumber;
uint8_t payloadSize;

const uint64_t pipeNum = 0x7878787878LL;

RF24 radio(PIN_CE, PIN_CSN);

void setup() {

  Serial.begin(115200);
  radio.begin();
  radio.setChannel(5);
  radio.setDataRate (RF24_1MBPS);
  radio.setPALevel(RF24_PA_HIGH);
  radio.enableDynamicPayloads();

  radio.openWritingPipe(pipeNum);

}

char text[] = "Hello world";

void loop() {

  if (radio.write(&text, sizeof(text))) {
    Serial.println("Delivered " + String(sizeof(text)) + " byte");
  }
  else {
    Serial.println("Data not delivered");
  }
  delay(1000);

}

