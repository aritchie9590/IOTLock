#include "mbed.h"
#include "Servo.h" 
#include "rtos.h"
#include "LSM9DS1.h"
#include <vector>
Servo myservo(p21);
Serial pc(USBTX, USBRX);
Serial Blue(p13,p14);
DigitalIn otherLock(p8);
DigitalOut myled1(LED1);
DigitalOut myled2(LED2);
DigitalOut myled4(LED4);
LSM9DS1 imu(p9, p10, 0xD6, 0x3C); //sda, scl, xgaddr, maddr
Mutex MUT;
int waitTime;
float position = 0;
int oldOther = 0;
float oldPosition = 0;
void bluetooth(void const *args)
{
    while (1){
        int bnum = Blue.getc(); //button number
        if (bnum == 1) {
            position = 1;
        }else 
        if (bnum == 0){
            position = 0;
        }
        Thread::wait(500);
    }
}
void lock(void const *agrs){
    while (1){
        if (oldOther != otherLock){
            oldOther = otherLock;
            myled4 = oldOther;
        } else if (oldPosition != position){
            oldPosition = position;
            myled4 = position;
        }
    }
}    
int main() {
    Thread thread1(bluetooth);  
    Thread thread2(lock);          
    double knockSequence[200] = {0.0}; //array to store 5 second knock sequence
    double passwordSequence[200] = {1.0}; 
    int knockLength = 0;
    
    passwordSequence[0] = 1.0;
    passwordSequence[26] = 1.0;
    passwordSequence[44] = 1.0;
    passwordSequence[50] = 1.0;
    passwordSequence[76] = 1.0;
    passwordSequence[126] = 1.0;
    passwordSequence[150] = 1.0;
    
    for (int i = 0; i<200; i++){
        //pc.printf("%f \n\r", passwordSequence[i]);
        }
    
    
    //initialization of the IMU
    LSM9DS1 imu(p9, p10, 0xD6, 0x3C); //sda, scl, xgaddr, maddr
    imu.begin();
    if (!imu.begin()) {
        pc.printf("Failed to communicate with LSM9DS1.\n");
    }
    imu.calibrate();
    
    //from testing it has been determined that imu.ax is the most sensitive to a
    //knock when the unit is in both the flat table top position, and the postion
    //it would likely be in if it were attached to a door
    
    //this while loop will continuously measure until it sees a knock then record
    //the knock sequence for five seconds before stopping
    
    //start of knock thread
    while(1){
        bool knockStarted = false;
        int count = 0;
        bool keepMeasuring = true;
        while(keepMeasuring){
            imu.readAccel(); //read the accelerometer
            int knock = abs(imu.ax); //consider only the reading from imu.ax for a knock
            
            if (int(knock)>700){
                //start recording the knock sequence only if we see a first knock
                if (!knockStarted){
                    waitTime = 25;
                    knockStarted = true;
                    pc.printf("Knock Started\n\r");
                    count = 0;
                    }
                knockSequence[count] = 1.0;
                knockLength = count;
                }
            else{
                if (knockStarted){
                    knockSequence[count] = 0.0;
                    }
                }
            
            //increment the count which indexes the knock sequence array and also
            //if the count is higher than the highest index of the knock sequence
            //array reset the count to zero and reset knockStarted to false
            count++;
            if (count>199 and knockStarted){
                keepMeasuring = false;
                pc.printf("Knock Finished\n\r");
                }
            if (count>199){ 
                count=0;
                }
            
            Thread::wait(25); //sample at 40 hz
            // test to print timer to the terminal
            if (count % 19 == 0){
                pc.printf("%d\n\r", count/19);
                }    
        }
        
    
        //compare the measured knock to the password knock
        //count the relative distances (in sample time) between knocks
        int passwordDist[201] = {0};
        int distCount = 0;
        int distIDX = 0;
        for (int i = 0; i < 200; i++){
            if (passwordSequence[i] != 0.0){
                if (distCount != 0){
                    passwordDist[distIDX] = distCount;
                    distIDX++;
                    }
                distCount = 0;
                }
            else{
                distCount++;
                }
            }
            
        int knockDist[201] = {0};
        distCount = 0;
        distIDX = 0;
        for (int i = 0; i < 200; i++){
            if (knockSequence[i] != 0.0){
                if (distCount != 0){
                    knockDist[distIDX] = distCount;
                    distIDX++;
                    }
                distCount = 0;
                }
            else{
                distCount++;
                }
            }
        
        // now we have the distances (in sample time) between each knock for both
        // the password and the knock sequence so lets compare them
        
        bool passwordMatch = true;
        for (int i = 0; i<200; i++){
            //only keep measuring while there are still inter knock distances to 
            //compare
            if ((passwordDist[i] != 0) && (passwordDist[i+1] !=0)){
                double passRatio = double(passwordDist[i])/150;
                double knockRatio = double(knockDist[i])/double(knockLength);
                pc.printf("%f, %f \n\r", passRatio, knockRatio);
                
                if ( !( (knockRatio>=(0.5*passRatio)) && (knockRatio<=(2.2*passRatio)) ) ){
                    passwordMatch = false;
                    }
                }
            }
                
        //set correct/incorect password flag
        if(passwordMatch){
            pc.printf("MATCH!!");
            position = 0;
            }
        else{
            pc.printf("NO WAY JOSE!");
            }
        }


}