#include <iostream>
//#include <RF24/nRF24L01.h>
#include <RF24/RF24.h> // SPI и работа с GPIO (BCM нумерация) реализована уже в библиотеке

using namespace std;

#define PIN_CE 17 // (chip enable)
#define PIN_CSN 0 // (chip select not)

int main() {
  
  RF24 radio(PIN_CE, PIN_CSN); // создаём объект radio
  radio.begin();
  // канал передачи данных (от 0 до 125), 5 - на частоте 2,405 ГГц
  radio.setChannel(5);
  // уровень усиления передатчика (RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_HIGH=-6dBm, RF24_PA_MAX=0dBm)
  radio.setPALevel(RF24_PA_HIGH);
  // скорость передачи данных (RF24_250KBPS, RF24_1MBPS, RF24_2MBPS), RF24_1MBPS - 1Мбит/сек
  radio.setDataRate(RF24_1MBPS);
  // разрешаем получение пакета подтверждения приема данных (по умолчанию true)
  radio.setAutoAck(true);
  // разрешаем динамически изменяемый размер блока полезных данных
  radio.enableDynamicPayloads();
  
  // Если передатчик не получил ответ от приёмника, то он повторит отправку данных.
  // Ожидание между каждой повторной попыткой, в кратных 250,
  // максимальное составляет 15. 0 означает 250 мкс, (15*250)+250 = 4000мкс,
  // и количество попыток перед сдачей, максимальное 15
  radio.setRetries(15, 15); // метод доступен только для передатчика 

  // открываем трубу с уникальным ID (одновременно может быть открыта только одна труба для передачи данных)
  radio.openWritingPipe(0x7878787878LL);

  // блок полезных данных может быть до 32 байт
  char text[] = "Hello world!";
  char ackData[24];

  while (true) {

    // По умолчанию передатчик запрашивает у приемника подтверждение получения данных,
    // а приёмник получив данные и проверив CRC, возвращает передатчику пакет подтверждения приема данных.
    
    // Передаем адрес переменной и его размер в байтах, значение которой будет в качестве полезных данных
    // И проверяем наличие возвращенного пакета подтверждения приема данных
    if (radio.write(&text, sizeof(text))) {
      cout << "Delivered " << (int) sizeof(text) << " byte" << endl;
    }
    else {
      cout << "Data not delivered" << endl; // данные передатчика не приняты или дошли с ошибкой CRC
    }

    // Если в буфере имеются принятые данные из пакета подтверждения приема
    if (radio.isAckPayloadAvailable()) {
      // читаем данные из буфера в массив ackData указывая сколько всего байт может поместиться в массив
      radio.read(&ackData, sizeof(ackData));
      string FIFO;
      for (uint8_t i = 0; i < sizeof(ackData); i++) {
        FIFO += ackData[i];
      }
      Serial.println(FIFO);

      // Если все три буфера FIFO заполнены то очищаем
      if (radio.rxFifoFull()) {
        cout << "Buffer full" << endl;
        radio.flush_rx(); // очиска буфера
      }
    }  

    delay(1000);
  }

}

