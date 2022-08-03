from turtle import delay
import serial # import module for serial communcation
import pyautogui # import module for keyboard/mouse control
import io # import module to to use universal newline mode
import time

alive = True
# open Serial Port 4
ser = serial.Serial()
ser.baudrate = 115200 # baud rate
ser.port = 'COM4'# set port
ser.timeout = 0.1 # set timeout in seconds
ser.open() # open serial communication
print('COM4  Open: ', ser.is_open)
sio = io.TextIOWrapper(io.BufferedRWPair(ser, ser))
ser.flush() # wait until all data is written

pyautogui.FAILSAFE = False # set failsafe for manual exit
width, height = pyautogui.size() # set size of screen
print('Press Ctrl-C to quit.')
pyautogui.moveTo(960, 540); # set mouse to middle

flap_up_prev = 0
flap_down_prev = 0
pause_prev = 0
landing_gear_prev = 0
throttle_prev = 0
rudder_prev = 0
xn_prev = 0
yn_prev = 0
brake_left_prev = 0
brake_right_prev = 0
count = 0

while (alive): 
    if ser.in_waiting:
        line = ser.readline().decode('utf')[:-1]
        data = line.split()
       
        # print(line)
        # Fill data
        if len(data) == 12:
            # Control
            # Pitch: Up, Down
            pitch = float(data[0])
            # Extend the angle limit up
            # Góc nâng tối đa ~ 13 độ, tăng góc nâng lên để dê dàng cất cánh
            if pitch > 10:
                pitch = 30
            # Roll: Left, Right
            roll = float(data[1])
            # Move the pointer to the center of the screen + roll, pitch angle
            # Full Screen Resolution 1920x1080
            x = roll*10 + 960
            y = pitch*6 +540
            if count == 0:
                pyautogui.moveTo(x,y)
            count += count
            if count == 8:
                count = 0      
            # flap up
            if int(data[2]) == 1:
                pyautogui.keyDown(']')  

            # flap down 
            if int(data[3]) == 1:
                pyautogui.keyDown('[')
            
            # emergency
            # escape flight simulator
            # stop programe
            if int(data[4]) == 1:
                pyautogui.keyDown('esc')
                alive = False
            # pause
            if int(data[5]) == 1:
                pyautogui.keyDown('space')

            # # gear -- SR22 no gear
            # if int(data[6]) == 1:
            #     pyautogui.keyDown()

            # Brake_left
            if int(data[7]) == 1:
                pyautogui.keyDown('.')
            else:
                pyautogui.keyUp('.')
            
            # Brake_right
            if int(data[8]) == 1:
                pyautogui.keyDown(',')
            else:
                pyautogui.keyUp(',')
            
            # Rudder right and left
            if int(data[9]) == 1:
                pyautogui.keyDown('shift')
                pyautogui.keyDown('right')
            if int(data[6]) == 1:
                pyautogui.keyDown('shift')
                pyautogui.keyDown('left')

            # Throttle up
            if int(data[10]) == 1:
                pyautogui.keyDown('pageup')
            
            # Throttle down
            if int(data[11]) == 1:
                pyautogui.keyDown('pagedown')
            print(data)



            
     
