# ESP32 MAIN BOARD IO DOCUMENTATION

#### GPIO PINS
|PIN|PROTOCOL|DT node|REMARK|DESCRIPTION|
|:-:|:-:|:-:|:-:|:-|
|0|GPIO||INPUT_UP|:x:|
|1|UART0||Tx||
|2|GPIO||OUTPUT|LED indicator|
|3|UART0||Rx||
|4|||||
|5|||||
|6|SPI||SCK|FLASH :x:|
|7|SPI||SD0|FLASH :x:|
|8|SPI||SD1|FLASH :x:|
|9|UART1||Tx|FLASH :x:|
|10|UART1||Rx|FLASH :x:|
|11|SPI||CMD|FLASH :x:|
|12|||||
|13|GPIO||OUTPUT|LED indicator|
|14|||||
|15|||||
<<<<<<< HEAD
|16|UART2||Tx|STM3 communication|
|17|UART2||Rx|STM32 communication|
|18|GPIO||OUTPUT|Grinder DIR|
|19|GPIO||OUTPUT|Grinder PWM|
|21|GPIO||||
|22|GPIO||||
=======
|16|UART2||Tx||
|17|UART2||Rx||
|18|GPIO||OUTPUT|Grinder DIR|
|19|GPIO||OUTPUT|Grinder PWM|
|21|GPIO||INPUT|START Button|
|22|GPIO||INPUT|RESET Button|
>>>>>>> development
|23|GPIO||OUTPUT|Crusher DIR|
|25|GPIO||OUTPUT|Crusher PWM|
|26|GPIO||INPUT|Weight Sensor|
|27|GPIO||INPUT|Photoelectric Sensor|
|32|GPIO||OUTPUT|Blower Relay|
<<<<<<< HEAD
|33|GPIO||||
=======
|33|GPIO||INPUT|Moisture Sensor|
>>>>>>> development
|34|GPIO||||
|35|GPIO||||
|36|GPIO||||
|39|GPIO||||
## [Pin functionability reference](https://randomnerdtutorials.com/esp32-pinout-reference-gpios/)
