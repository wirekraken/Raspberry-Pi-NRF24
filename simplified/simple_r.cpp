#include <iostream>
//#include "../RF24/nRF24L01.h"
#include "../RF24/RF24.h"

using namespace std;

#define PIN_CE  17
#define PIN_CSN 0

uint8_t pipeNumber;
uint8_t payloadSize;

int main() {

  RF24 radio(PIN_CE, PIN_CSN);

  radio.begin();
  radio.setChannel(5);
  radio.setPALevel(RF24_PA_HIGH);
  radio.setDataRate(RF24_1MBPS);
  radio.enableDynamicPayloads();

  radio.openReadingPipe(0, 0x7878787878LL);

  radio.printDetails();

  radio.startListening(); 

  cout << "Start listening..." << endl;

  while (true) {

    if (radio.available(&pipeNumber)) {

      payloadSize = radio.getDynamicPayloadSize();
      char payload[payloadSize];
      string receivedData;
      radio.read(&payload, sizeof(payload));

      for (uint8_t i = 0; i < payloadSize; i++) {
          receivedData += payload[i];
      }
      cout << "Pipe : " << (int) pipeNumber << " ";
      cout << "Size : " << (int) payloadSize << " ";
      cout << "Data : " << receivedData << endl;

      delay(100);
    }
  }
  return 0;
}
