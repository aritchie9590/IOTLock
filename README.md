# IOTLock

=Team=
*Alex Ritchie
*Bao Bui
*Erik Lomas
*Trung Nguyen

= Overview =

This is an IOT Door Lock for mbed. We allow for multiple ways to lock and unlock the door utilizing Wi-Fi, Bluetooth, IMU, and Fingerprint functionality. Additionally, our project allows for dynamically adding fingerprints for access, and custom knock sequences.

Each mBed LPC1768 has 3 UART serial communication channels, while this project requires a serial communication channel for each of the four necessary peripherals. In order to overcome this, we employ two mBed modules and have them communicate via DigitalOut/DigitalIn mBed APIs.

The first mBed (mBed 1) communicates with the wiFly module and the TTL GT-511C3 to hand the websocket and fingerprint scanner functionality. The code for this mBed is contained in the project IOTLOCK. The second mBed (mBed 2) communicates with the Bluefruit module and the IMU to handle the bluetooth and secret knock functionality. The code for this mBed is contained in the project IOTLOCK2. 

= Features =
*Lock/Unlock via Bluetooth Phone Application
*Lock/Unlock via Websocket
*Lock/Unlock via Fingerprint Scanner
*Lock/Unlock via Secret Knock Password
*Add new fingerprints for unlocking
*Change the lock password

= Components =
*[[https://developer.mbed.org/users/beanmachine44/notebook/fingerprint-scanner1/|TTL GT-511C3 Fingerprint Scanner]]
*[[https://developer.mbed.org/components/LSM9DS1-IMU/|LSM9DS1 IMU ]]
*[[https://developer.mbed.org/users/4180_1/notebook/adafruit-bluefruit-le-uart-friend---bluetooth-low-/|Adafruit Bluefruit Bluetooth Module]]
*[[https://developer.mbed.org/components/Roving-Networks-WiFly-RN-171-XV/|WiFly RN-171-XV Wi-Fi Module]]
*mBed LPC1768 (2)

=Wiring=
==mBed 1==
|=mbed 1|=WiFly|=TTL GT-511C3|SPDT switch|=mbed 2|
|gnd|gnd||||
|Vout(3.3V)|VDD_3V3||||
|p9|UART_RX||||
|p10|UART_TX||||
|p26|GPIO6||||
|p19|reset||||
|Vout(3.3V)||VCC|||
|gnd||gnd|||
|p28||RX|||
|p27||TX|||
|p22|||pole||
|Vout (3V)|||throw1||
|gnd|||throw2||
|p21||||p8|

==mBed 2==
|=mBed 2|=Bluefruit|=IMU|=Servo|=mBed 1|
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

{{/media/uploads/aritchie9590/screen_shot_2016-11-29_at_3.41.21_pm.png}} 

=Code=
<<program  /users/xemnas214/code/IOTLOCK/>>

<<program  /users/xemnas214/code/IOTLOCK2/>>

 {{/media/uploads/aritchie9590/img_4975.jpg}} 

{{https://youtu.be/mMZ7KekosEs}}
