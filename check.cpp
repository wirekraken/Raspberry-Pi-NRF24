#include <iostream>
// #include <nRF24L01.h>
#include <RF24.h>

using namespace std;

#define PIN_CE 17
#define PIN_CSN 0

int main() {

	RF24 radio(PIN_CE, PIN_CSN);
	radio.begin();

	// Проверка на поддержку модуля библиотекой RF24 (при правильном подключении)
	if (radio.isPVariant()) {
		cout << "The module is supported" << endl;
		radio.printDetails(); 
	}
	else {
		cout << "Unknown module" << endl;
	}
}