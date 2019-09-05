#include <iostream>
//#include <RF24/nRF24L01.h>
#include <RF24/RF24.h> // SPI и работа с GPIO (BCM номерация) реализована уже в библиотеке

using namespace std;

#define PIN_CE 17 // (chip enable)
#define PIN_CSN 0 // (chip select not)

uint8_t pipeNumber;
uint8_t payloadSize;

int main() {

  RF24 radio(PIN_CE, PIN_CSN); // создаём объект radio (chip enable, chip select)
  radio.begin();
  // канал передачи данных (от 0 до 125), 5 - на частоте 2,405 ГГц
  radio.setChannel(5);
  // уровень усиления сигнала (RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_HIGH=-6dBm, RF24_PA_MAX=0dBm)
  radio.setPALevel(RF24_PA_HIGH);
  // скорость передачи данных (RF24_250KBPS, RF24_1MBPS, RF24_2MBPS), RF24_1MBPS - 1Мбит/сек
  radio.setDataRate(RF24_1MBPS);
  // открываем трубу с адресом 0x7878787878LL для приема данных (всего может быть труб 0 - 5)
  radio.openReadingPipe(0, 0x7878787878LL);

  // разрешаем размещать пользовательские данные в пакете подтверждения приема
  radio.enableAckPayload();

  // разрешаем отправку передатчику пакет подтверждения приема его данных на всех трубах (по умолчанию true)
  radio.setAutoAck(true);

  // разрешаем динамически изменяемый размер блока полезных данных на всех трубах (макс 32 байта)
  radio.enableDynamicPayloads();

  // вывод конфигурации модуля
  radio.printDetails();
  // включаем приемник, начинаем прослушивать открытые трубы
  radio.startListening(); 

  cout << "Start listening..." << endl;

  while (true) {

    // Если в буфере имеются принятые данные то передаем адрес переменной (должен быть типа uint8_t)
    // в которую требуется поместить номер трубы по которой были приняты данные
    if (radio.available(&pipeNumber)) {

      payloadSize = radio.getDynamicPayloadSize(); // получаем размер принятых полезных данных 
      char payload[payloadSize];
      string receivedData;

      // Читаем принятые данные в массив payload указав размер этого массива в байтах
      radio.read(&payload, payloadSize);

      for (uint8_t i = 0; i < payloadSize; i++) {
        receivedData += payload[i];
      }

      cout << "Pipe number : " << (int) pipeNumber << " ";
      cout << "Payload size : " << (int) payloadSize << " ";    
      cout << "Data: " << receivedData << endl;

      char ackData[] = "Data from buffer";

      // Помещаем данные в буфер FIFO. Как только будет получен пакет то данные из буфера
      // будут отправлены этому передатчику вместе с пакетом подтверждения приема его данных
      radio.writeAckPayload(0, &ackData, sizeof(ackData));
    }
  }
  return 0;
}
