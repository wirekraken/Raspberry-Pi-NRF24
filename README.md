## Подключение и тестирование модуля nRF24L01+
##### Для работы с модулем, предварительно нужно включить SPI
Чтобы включить SPI, используйте `raspi-config` или раскомментируйте в файле `/boot/config.txt`  строку `dtparam=spi=on`. После перезагрузитесь.
При успешной загрузке драйвера SPI вы увидите устройство `/dev/spidev0.0`.
#### Компиляция и сборка :
```sh
$ g++ -Wall -lrf24 scanner.cpp -o scanner
```
#### Схема подключения nRF24L01+ к Raspberry Pi :
![scheme](https://github.com/wirekraken/Raspberry-Pi-NRF24/blob/master/scheme/scheme.png)

