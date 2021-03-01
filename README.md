## Tempmperature and Humidity Logger - (Arduino)
---------
The intention of project was to create data logger for tracking the enviroment conditions (air temperature and humidity) and object temperature. Collected data were further use to calculate the dew point and estimate risk of moisture formation on object surface.

Hardware:
1. Controllr: Arduino Nano (https://nettigo.pl/products/klon-arduino-nano-v3-atmega328p-ch340)
2. Temperature & humidity sensor: DHT22 (https://nettigo.pl/products/czujnik-wilgotnosci-i-temperatury-dht22)
3. Temperature sensor: DS18B20 (https://nettigo.pl/products/czujnik-temperatury-ds18b20-interfejs-onewire)
4. Real time clock with I2C interface (https://nettigo.pl/products/modul-zegara-rtc-ds1307-z-eeprom-32k-bit)
5. LCD Display 2x16 with I2C interface (https://nettigo.pl/products/wyswietlacz-lcd-2x16-i2c-niebieski-1602a)
6. Micro SD card reader - SPI interface (https://nettigo.pl/products/modul-czytnika-kart-micro-sd)

Measuremets can be taken with adjusted interval. Data are stored on micro SD card in date stamped folders (start day of data acquisition) and saved in CSV files (name = start time of data acquisition).
