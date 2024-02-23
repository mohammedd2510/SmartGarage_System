# **    SmartHome Project Made by Mohamed Osama  **

## Hardware used :
 1.  Atmega32
 2.  PIC18F4620
 3.  LCD 20x4
 4.  AT24C08 EEPROM
 5.  SG90 Servo Motor
 6.  Green Led
 7.  Red LED
 8.  PIR Sensor
 9.  Buzzer 
 10. UltraSonic Sensor HC-SR04
 11. Tempertaure Sensor LM35
 12. Keypad Module
 
 ## Software used :
 ### MCAL LAYER :
 #### For Atmega32:
 1. DIO DRIVER
 2. PORT DRIVER
 3. IRQH DRIVER
 4. UART DRIVER
 5. ADC Driver
 #### For PIC18F4620:
 1. GPIO Driver
 2. Interrupts Driver
 3. CCP Driver
 4. Timer0 Driver
 5. Timer1 Driver
 6. Timer3 Driver
 7. EUSART Druver
 8. I2C Driver

### HAL LAYER :
#### For Atmega32:
1. PIR Driver
2. BUZZER Driver
3. Temperature Sensor LM35 Driver
#### For PIC18F4620
1. LCD DRIVER
2. LED DRIVER
3. AT24C08 EEPROM DRIVER
4. SERVO MOTOR DRIVER
5. ULTRASONIC DRIVER
6. Keypad Driver

 ## Features:
This SmartGarage project consists of 7 modes:                                                                  
1. <u>__Enter Garage Pin Mode:__</u>                                                                           
   In this mode, a walking user can enter the garage by inputting the correct pin.                             
   The user has 3 trials to enter the pin correctly.                                                           
   After each failed attempt,there is a cooldown period of 1 minute before gaining 1 additional trial.        
   The number of trials is saved in EEPROM.A back button is available to return to the main menu.              

2. <u>**Set New Mode:**</u>   
    Requires entering the admin pin to set a new pin.                                                          
    Failed attempts trigger the same mechanism as in the Enter Garage Pin Mode.                                
    The new pin is saved in EEPROM.                                                                            

3. <u>**Show Number of Cars Mode:**</u> Displays the number of cars inside the garage on the LCD.
   
4. <u>**Show Available Spaces Mode:**</u> Shows the number of free spaces inside the garage on the LCD.
   
5. <u>**Car Detected Mode:**</u>                                                                               
   When the ultrasonic sensor detects a car entering the garage within a distance threshold of 30 cm or less, the MCU checks for available spaces. If space is available, the pin is requested. Failed attempts follow the same mechanism as in the Enter Garage Pin Mode. If the correct pin is entered, the garage door opens. The door closes once the car enters (ultrasonic sensor doesn't detect nearby cars).

6. Car Exiting Mode:
    When the internal MCU detects motion using the PIR sensor, it sends a signal via UART to the external MCU to open the garage. If no motion is detected afterward, indicating that the car has left, a signal is sent to close the garage. The number of garage spaces increments upon car exit, and the value is saved in EEPROM.

7. Fire Detected Mode: 
   The internal MCU continuously monitors the temperature using the LM35 sensor. If the temperature exceeds 50°C, indicating a fire, the buzzer rings, a signal is sent via UART to open the garage, and a fire detected message appears on the external LCD. The garage door remains open until the internal MCU sends a signal to close it, triggered by a decrease in temperature back to normal levels
