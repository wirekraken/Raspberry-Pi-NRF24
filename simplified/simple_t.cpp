#include <iostream>
//#include "../RF24/nRF24L01.h"
#include "../RF24/RF24.h"

using namespace std;

#define PIN_CE  17
#define PIN_CSN 0

const uint64_t pipeNum = 0x7878787878LL;

int main() {

  RF24 radio(PIN_CE, PIN_CSN);

  radio.begin();
  radio.setChannel(5);
  radio.setDataRate (RF24_1MBPS);
  radio.setPALevel(RF24_PA_HIGH);
  radio.enableDynamicPayloads();

  radio.openWritingPipe(pipeNum);

  char text[] = "Hello world";

  while (true) {

    if (radio.write(&text, sizeof(text))) {
      cout << "Delivered " << sizeof(text) << " byte" << endl;
    }
    else {
      cout << "Data not delivered" << endl;
    }
    delay(1000);

   }
   return 0;
}

