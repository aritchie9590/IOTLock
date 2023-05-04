#include "mbed.h"
#include "rtos.h"
#include "WiflyInterface.h"
#include "Websocket.h"
#include "FPScanner.h"
#include "lib.h"
/* wifly interface:
*     - p9 and p10 are for the serial communication
*     - p19 is for the reset pin
*     - p26 is for the connection status
*     - "mbed" is the ssid of the network
*     - "password" is the password
*     - WPA is the security
*/
Serial pc(USBTX, USBRX);
FPScanner finger(p28,p27);
DigitalIn isEnroll(p22);
DigitalOut myled1(LED1);
DigitalOut myled2(LED2);
DigitalOut myled4(LED4);
DigitalOut otherLock(p21);
WiflyInterface wifly(p9, p10, p18, p19, "SuperWifi", "00000000", WPA);
float position = 0;
float oldPosition = 0;
void fingerLock(void const *agrs){
    while(1) {
        //fp_enroll(&finger, &debug);
        if (isEnroll){
            printf("Enrolling\n\r");
            fp_enroll(&finger);
            printf("Enrolled\n\r");
        }
        if(fp_auth(&finger)){
                myled1 = !myled1;
                position = 1.0 - position;
                otherLock = position;
        }   
        myled2 = !myled2;
        Thread::wait(500);
    }
} 
int main() {
    char recv[100];
    printf("Started\n\r");
    wifly.init(); //Use DHCP
    printf("Init\n\r");
    while(!wifly.connect());
    printf("Wifly Connected\n\r");
    printf("IP Address is %s\n\r", wifly.getIPAddress());
    Websocket ws("ws://192.168.43.154:8888/ws");
    while(!ws.connect());
    printf("Socket connected\n\r");
    ws.send(new char('0'));
    
    finger.Open();
    Thread thread1(fingerLock); 
    printf("Finger created\n\r");
    
    while (1) {
        if (ws.read(recv)) {
            sscanf(recv,"%f",&position);
        }
        if (oldPosition != position){
            ws.send(new char(position + '0'));
        }
        oldPosition = position;
        otherLock = position;
        myled4 = otherLock;
    }
}