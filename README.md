# IOTLock

## Team
- *Alex Ritchie*
- Bao Bui
- Erik Lomas
- Trung Nguyen

## Overview 

This is an IOT Door Lock for mbed. We allow for multiple ways to lock and unlock the door utilizing Wi-Fi, Bluetooth, IMU, and Fingerprint functionality. Additionally, our project allows for dynamically adding fingerprints for access, and custom knock sequences.

Each mBed LPC1768 has 3 UART serial communication channels, while this project requires a serial communication channel for each of the four necessary peripherals. In order to overcome this, we employ two mBed modules and have them communicate via DigitalOut/DigitalIn mBed APIs.

The first mBed (mBed 1) communicates with the wiFly module and the TTL GT-511C3
to hand the websocket and fingerprint scanner functionality. The code for this
mBed is contained in the folder fp_web. The second mBed (mBed 2) communicates
with the Bluefruit module and the IMU to handle the bluetooth and secret knock
functionality. The code for this mBed is contained in the folder bt_knock. 

## Features 
- Lock/Unlock via fingerprint scanner
- Register new fingerprints
- Lock/Unlock via custom "secret knock"
- Register new secret knock pattern
- Lock/Unlock via bluetooth
- Lock/Unlock via websocket

## Components
- [TTL GT-511C3 Fingerprint Scanner](https://developer.mbed.org/users/beanmachine44/notebook/fingerprint-scanner1/) 
- [LSM9DS1 IMU](https://developer.mbed.org/components/LSM9DS1-IMU/)
- [Adafruit Bluefruit Bluetooth Module](https://developer.mbed.org/users/4180_1/notebook/adafruit-bluefruit-le-uart-friend---bluetooth-low-/) 
- [WiFly RN-171-XV Wi-Fi Module](https://developer.mbed.org/components/Roving-Networks-WiFly-RN-171-XV/)
- [mBed LPC1768](https://os.mbed.com/platforms/mbed-LPC1768/) (x2)

## Wiring
### mBed 1
mbed 1|WiFly|TTL GT-511C3|SPDT switch|mbed 2
:---:|:---:|:---:|:---:|:---:
gnd|gnd||||
Vout(3.3V)|VDD_3V3||||
p9|UART_RX||||
p10|UART_TX||||
p26|GPIO6||||
p19|reset||||
Vout(3.3V)||VCC|||
gnd||gnd|||
p28||RX|||
p27||TX|||
p22|||pole||
Vout (3V)|||throw1||
gnd|||throw2||
p21||||p8|

### mBed 2
|mBed 2|Bluefruit|IMU|Servo|mBed 1|
|gnd|gnd||||
|Vu (5V)|Vin||||
|p14 (Serial RX)|TXO||||
|p13 (Serial TX)|RXI||||
|gnd|CTS||||
|gnd||gnd|||
|Vout (3.3V)||VDD|||
|p9||SDA|||
|p10||SCL|||
|p21|||control||
|p8||||p21|

[<img src="https://img.youtube.com/vi/mMZ7KekosEs/maxresdefault.jpg" width="50%">](https://youtu.be/mMZ7KekosEs)
