# Bluetooth Communication - UART
This micro:bit program lets you send and receive strings between a BBC micro:bits and a Bluetooth connected device via the UART serial interface.

UART stands for Universal Asynchronous receiver/transmitter) it is a form of communication via hardware over a serial connection, in which the data format and transmission speeds are configurable.

As the BBC micro:bit does not have a keyboard and only has two buttons that we can use as inputs, the program lets the user use Morse code to input words and send it over Bluetooth to the device. The device can receive the message via an app that supports serial communication over Bluetooth and is for Bluetooth LE devices. I recommend using [Serial Bluetooth Terminal](https://play.google.com/store/apps/details?id=de.kai_morich.serial_bluetooth_terminal) for Android. The program also lets the device to sends strings to the BBC micro:bit, which is then scrolled through its LED matrix. 

The main two function `onConnected()` and `onDisonnected()` is used when the micro:bit detects the Bluetooth events `MICROBIT_BLE_EVT_CONNECTED` and `MICROBIT_BLE_EVT_DISCONNECTED` is detected.


## Setup
- Compile the `UART.cpp` and copy the `.HEX` file created to the micro:bit. This will cause the micro:bit to restart.
- Connect the micro bits to a power source (USB/ Battery)
- Install the app, [Serial Bluetooth Terminal](https://play.google.com/store/apps/details?id=de.kai_morich.serial_bluetooth_terminal) for Android or any other app that supports serial connections over Bluetooth.
- Open the app on the phone and scan and pair the device and the micor:bit. the user will be notified by the phone app and a 'C' will be displayed on the micro:bit once the connection is established
- User can use the phone app textbox and keyboard to send string to the micro:bit and use the micro:bit button and Morse code to send string to the phone.

## Controls
### micro:bit to Device
- Press `Button B` for a **DOT**
- Hold `Button B` for a **DASH**
- Press `Button A` to save the letter and use the `Button B` again to input the next letter of the string that you want to send.
- If the program can’t translate the code into a letter, All the LED's will blink and will be followed by a sad face. Restart the micro:bit and try again.
- If you try to input more than 5 codes to the receiver. An X will be displayed, and all the variables will be reset.
- Hold `Button B` to print the whole word
- And finally press `Button A` and `Button B` together to send the word to the device.

### Device to micro:bit
- Open the app and scan for devices
- Allow administrative privileges if you’re prompted by the device
- One the device is paired use the keyboard to write the word and press send to send it over to the micro:bit
- The word or string scrolls through the micro:bits display.

#### Notes
In order to make the testing and pairing easier i have made few changes to the `MicroBitConfig.h` and are optional, so you can change them back to bring back the pairing.

```c
//line 190 - original value 0
#define MICROBIT_BLE_OPEN	1 

//line 198
#define MICROBIT_BLE_DEFAULT_TX_POWER	6

//line 216, disable whitelist - original value 1
#define MICROBIT_BLE_WHITELIST	0 
```

Made few changed to the heap size and GATT table since Bluetooth tends to eat up a lot of memory

```c
#define MICROBIT_NESTED_HEAP_SIZE	0.50 

#define MICROBIT_SD_GATT_TABLE_SIZE	0x300
```
