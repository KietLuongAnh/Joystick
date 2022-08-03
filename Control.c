#include <stdio.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <wiringPiI2C.h>
#include <stdio.h>
#include <stdint.h>			// type int lib
#include <math.h>

/* dinh nghia 6 thanh ghi can su dung cua MPU6050 */
//////////////////////////////////////////////////
#define Sample_rate		25                      //
#define Config			26                      //
#define Gyro_config		27                      //
#define Acc_config		28                      //
#define Interrupt		56                      //
#define PWR_Managment	107                     //
                                                //
#define Acc_X			59                      //
#define Acc_Y			61                      //
#define Acc_Z			63                      //
//////////////////////////////////////////////////
/* MPU Address and Rotate */
int mpu;
float pitch, roll,avg_pitch, avg_roll;

/* Other Airplane hardware Control */
int flap_up = 0;            // GPIO 25
int flap_down = 0;          // GPIO 24
int emergency = 0;          // GPIO 3
int pause = 0;              // GPIO 4
int brake_left = 0;         // GPIO 23
int brake_right = 0;        // GPIO 22
int rudder_left = 0;        // GPIO 5
int rudder_right = 0;       // GPIO 21
int throttle_up = 0;        // GPIO 0
int throtthel_down = 0;     // GPIO 2

/* Setup MPU Registers*/
void Init_6050(void)
{
	// register 25-28, 56, 107
	// Sample_rate 500Hz
    mpu = wiringPiI2CSetup(0x68);                   // I2C Adress 0x68
	wiringPiI2CWriteReg8(mpu, Sample_rate, 15);
	// Khong su dung nguon xung ngoai, tat DLFP
	wiringPiI2CWriteReg8(mpu, Config, 0);
	// Gyro FS : +- 500 o/s
	wiringPiI2CWriteReg8(mpu, Gyro_config, 0x08);
	// acc FS : +- 8g
	wiringPiI2CWriteReg8(mpu, Acc_config, 0x10);
	// mo Interrupt cua data ready
	wiringPiI2CWriteReg8(mpu, Interrupt, 1);
	// chon nguon xung Gyro X
	wiringPiI2CWriteReg8(mpu, PWR_Managment,0x01);

}
/* Read Acceleration */
int16_t read_Acc(unsigned char Acc_Direction)
{
	int16_t high, low, data;
	high = wiringPiI2CReadReg8(mpu, Acc_Direction);		// Acc_X = 59: thanh ghi 59
	low = wiringPiI2CReadReg8(mpu, Acc_Direction + 1);	// Acc_X + 1 = 60: thanh ghi 60 (datasheet)
	data = (high<<8) | low;
	return data;
}

/* Doc gia tri goc nghieng */
void Rotate()
{
    // doc gia tri do truc x
    float Ax = (float)read_Acc(Acc_X)/4096.0;
    float Ay = (float)read_Acc(Acc_Y)/4096.0;
    float Az = (float)read_Acc(Acc_Z)/4096.0;
    
    long int temp;
    // Xuất giá trị góc nghiêng mỗi 50ms
    if (millis() - temp > 50)
    {
        // doc gia tri cac goc nghieng
        pitch = atan2(Ax, sqrt(pow(Ay,2) + pow(Az,2)))*180/M_PI;
        roll = atan2(Ay, sqrt(pow(Ax, 2) + pow(Az, 2)))*180/M_PI;
        temp = millis();
    }
}
/* Other HardWare System */

void Flap_up()
{
    if(!digitalRead(25))
    {   flap_up = 1;
        delay(20);
        if(!digitalRead(25))
            delay(20);
    }
        
}
// flap_down
void Flap_down()
{
    if(!digitalRead(24))
    {
        flap_down = 1;
        delay(20);
        if(!digitalRead(24))        // Chống dội nút nhấn 
            delay(20);
    }

}
//emergency
void Emergency()
{
    if(!digitalRead(3))
    {
        emergency = 1;
        delay(20);
        if (!digitalRead(3))
            delay(20);
    }
}
// Pasue
void Pause()
{
    if (!digitalRead(4))
    {
        pause = 1;
        delay(20);
        if(!digitalRead(4))
            delay(20);
    }
}
// Rudder_left
void Rudder_left()
{
    if (!digitalRead(5))
    {
        rudder_left = 1;
        delay(20);
        if(!digitalRead(5))
            delay(20);
    }
}
// Brake_left
void Brake_left()
{
    if (!digitalRead(23))
    {
        brake_left = 1;
        delay(20);
        if(!digitalRead(23))
            delay(20);
    }
}
// Brake_right
void Brake_right()
{
    if (!digitalRead(22))
    {
        brake_right = 1;
        delay(20);
        if(!digitalRead(22))
            delay(20);
    }
}
// Rudder_right
void Rudder_right()
{
    if (!digitalRead(21))
    {
        rudder_right = 1;
        delay(20);
        if(!digitalRead(21))
            delay(20);
    }
}
// Throttle_up
void Throttle_up()
{
    if (!digitalRead(0))
    {
        throttle_up = 1;
        delay(20);
        if(!digitalRead(0))
            delay(20);
    }
}
// Throttle_down
void Throttle_down()
{
    if (!digitalRead(2))
    {
        throtthel_down = 1;
        delay(20);
        if(!digitalRead(2))
            delay(20);
    }
}

/* Reset Value */
/* Reset all button variable to 0 */
void Reset_value()
{
    flap_up = 0;
    flap_down = 0;
    emergency = 0;
    pause = 0;
    brake_left = 0;
    brake_right = 0;
    rudder_left = 0;
    rudder_right = 0;
    throttle_up = 0;
    throtthel_down = 0;
}
int main (void)
{
    /* Setup GPIO */
    wiringPiSetup ();
    /* Setup MPU */
    Init_6050();
    /* Setup Serial Port (AMA0), baud rate = 115200 */
    int fd ;
    fd = serialOpen ("/dev/ttyAMA0", 115200);
    printf("Lauching...
            \nSending data to CH340... \n");

    /* Interrupt *//////////////////////////////////
    wiringPiISR(25, INT_EDGE_FALLING, Flap_up);
    wiringPiISR(24, INT_EDGE_FALLING, Flap_down);
    wiringPiISR(3, INT_EDGE_FALLING, Emergency);
    wiringPiISR(4, INT_EDGE_FALLING, Pause);
    wiringPiISR(5, INT_EDGE_FALLING, Rudder_left);
    wiringPiISR(23, INT_EDGE_FALLING, Brake_left);
    wiringPiISR(22, INT_EDGE_FALLING, Brake_right);
    wiringPiISR(21, INT_EDGE_FALLING, Rudder_right);
    wiringPiISR(0, INT_EDGE_FALLING, Throttle_up);
    wiringPiISR(2, INT_EDGE_FALLING, Throttle_down);
    /////////////////////////////////////////////////

    while (1)
    {
        Rotate();
        /* Send data to CH340 */
        serialPrintf(fd, "%.2f %.2f %d %d %d %d %d %d %d %d %d %d\n"
        , pitch, roll, flap_up, flap_down, emergency, pause, rudder_left
        , brake_left, brake_right, rudder_right, throttle_up, throtthel_down);
        Reset_value();
        delay(50);
    }
    
    serialClose(fd) ;
    return 0 ;
}