// Сканнер ISM диапазона частот (от 2400 МГц до 2527 МГц)
#include <iostream>
// #include <RF24/nRF24L01.h>
#include <RF24/RF24.h>
 
using namespace std;

#define PIN_CE 17
#define PIN_CSN 0
#define NUM_CHANNELS 126
 
uint8_t values[NUM_CHANNELS];
const int num_reps = 100;
 
int main() {

  RF24 radio(PIN_CE, PIN_CSN);
  radio.begin();
  radio.setAutoAck(false); // запрет на автоматическую отправку пакетов подтверждения приема для всех труб
 
  radio.startListening(); // вход в режиме приемника
  radio.stopListening();
 
  radio.printDetails(); // вывод конфигурации
 
  // Вывод верхнего заголовка
  for (int i = 0; i < NUM_CHANNELS; ++i) {
    cout << hex << (i >> 4);
  }

  cout << endl;

  // Вывод нижнего заголовка
  for (int i = 0; i < NUM_CHANNELS; ++i) {
    cout << hex << (i & 0xf);
  }

  cout << endl;
 
  while (true) {

    memset(values, 0, sizeof(values)); // все значения каналов обнуляем
 
    // Сканирование всех каналов num_reps
    for (int k = 0; k < num_reps; ++k) {
      for (int i = 0; i < NUM_CHANNELS; ++i) {

        radio.setChannel(i);
 
        radio.startListening();
        delayMicroseconds(130); // т.к. время переключения между модуля каналами состовляет 130 мкс
        radio.stopListening();
 
        // Проверка наличия несущей частоты на выбранном канале (частоте)
        if (radio.testCarrier()) { 
          ++values[i];
        }
      }
    }
  
    // Распечатка измерения канала в одну шестнадцатеричную цифру
    for (int i = 0; i < NUM_CHANNELS; ++i) {
      cout << hex << min(0xf, (values[i] & 0xf));
    }
    cout << endl;
  }
  return 0;
}