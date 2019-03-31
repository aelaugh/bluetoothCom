#Bluetooth Communication - UART
Implementing an application that communicates between a micro:bit and another device, using either the  Bluetooth LE

This micro-bit program lets you send and receive strings between a BBC micro:bits and a blutetooth connected device via the UART serial interface.

UART stands for Universal Asynchronus receiver/transmitter) it is a form ofcommunication via hardware over a serial connection, in which the data format and transmission speeds are configurable.

As the BBC micro:bit does not have a keyboard and only has two buttons that we canuse as inputs, the program lets the user use morse code to input words and send it over blutooth to the device. The device can receive the message via an app that supports serial comunication over blutooth and is for bluetooth LE devices. I recommend uisng [Serial Bluetooth Terminal](https://play.google.com/store/apps/details?id=de.kai_morich.serial_bluetooth_terminal) for Android. The program also lets the device to sends strings to the BBC micro-bit, which is then scrlled through its LED matrix. 


##Setup
- Compile the `UART.cpp` and copy the `.HEX` file created to the micro:bit. This will cause the micro:bit to restart.
- Connect the micro bits to a power source (USB/ Battery)
- Install the app, [Serial Bluetooth Terminal](https://play.google.com/store/apps/details?id=de.kai_morich.serial_bluetooth_terminal) for Android or any other app that supports serial connections over bluetooth.
- Open the app on the phone and scan and pair the device and the micor:bit. the user will be notified by the phone app and a 'C' will be displayed on the micro-bit once the connection is established
- User can use the phone app textbox and keyboard to send string to the micro-bit and use the micro-bit button and morsecode to send string to the phone.

## Controls
- Press `Button B` to send a **DOT**
- Hold `Button B` to send a **DASH**
- Press `Button A` to send the translate signal to the receiving micro:bit to translate the sent code. Do wait for the animation to stop indicating the deivice is ready to send the second code.
- If the program can’t translate the code into a letter, All the LED's will blink and will be followed by a sad face. Restart the micro:bit and try again.
- If you try to send more than 5 codes to the receiver. An X will be displayed, and all the variables will be reset.
- Hold `Button B` to view the whole word thats has been transmitted.


