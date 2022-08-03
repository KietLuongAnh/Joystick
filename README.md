# Joystick
17/6/2022.
This is my Final Project of Embedded System Subject
Make a Joystick to control the plane in Flight Simulator game of Google Earth
The Joystick is control by Raspberry Pi and MPU6050. Raspberry Pi read the angle of the peak of the Joystick (where MPU6050 sensor placed) and tranfer it to PC by UART Protocol.
The code read data from sensor and tranfer it to the PC by raspberry pi is Control.c file
The code receive data from raspberry is uart.py, which using PyAutoGUI to Control the plane.
