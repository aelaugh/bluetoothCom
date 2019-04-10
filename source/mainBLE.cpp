//MICORBIT: MorseCode wirelsss, Serial communication via Bluetooth (IOT Challenge 3)
//Date: 31 March 2019
//Name: Abdulla Elaaf
//Student ID: S1701225

#include "MicroBit.h"
#include "MicroBitSamples.h"
#include "MicroBitUARTService.h"
 
#ifdef MICROBIT_UART
 
MicroBit uBit;
MicroBitUARTService *uart;
MicroBitImage image;
MicroBitDisplay display;
int connected = 0;

//Images of the dot and the dash to output to the screen
MicroBitImage dot("0,0,0,0, 0\n0,0,0,0,0\n0,0,255,0,0\n0,0,0,0,0\n0,0,0,0,0\n");
MicroBitImage dash("0,0,0,0, 0\n0,0,0,0,0\n0,255,255,255,0\n0,0,0,0,0\n0,0,0,0,0\n");

//Handles the morse code array sent by the Master(Sending) microbit
char morseArray[6]; 
char tempString;
short loc = 0;
//array to store the word 
char letterString[50] = "";
  
//Animations
void errorAnim() {
    const uint8_t fullLED[] =  { 1, 1, 1, 1, 1,  0, 0, 0, 0, 0, 
                                 1, 1, 1, 1, 1,  0, 0, 0, 0, 0,  
                                 1, 1, 1, 1, 1,  0, 0, 0, 0, 0,  
                                 1, 1, 1, 1, 1,  0, 0, 0, 0, 0,  
                                 1, 1, 1, 1, 1,  0, 0, 0, 0, 0, };

    const uint8_t sadLED[] =  { 0, 1, 0, 1, 0,  0, 1, 0, 1, 0, 
                                0, 1, 0, 1, 0,  0, 1, 0, 1, 0,  
                                0, 0, 0, 0, 0,  0, 0, 0, 0, 0,  
                                1, 1, 1, 1, 1,  0, 1, 1, 1, 0,  
                                0, 0, 0, 0, 0,  1, 0, 0, 0, 1, };
    MicroBitImage i(10,5,fullLED);
    MicroBitImage j(10,5,sadLED);  
    display.animate(i,200,5);
    display.animate(i,200,5);
    display.animate(j,600,5);
}

void startAnim() {
    const uint8_t crossLED[] = { 0, 0, 1, 1, 0,  0, 0, 1, 1, 0, 
                                 0, 0, 1, 0, 1,  1, 0, 1, 0, 1,  
                                 0, 0, 1, 1, 0,  0, 1, 1, 1, 0,  
                                 0, 0, 1, 0, 1,  1, 0, 1, 0, 1,  
                                 0, 0, 1, 1, 0,  0, 0, 1, 1, 0, };
    MicroBitImage i(10,5,crossLED); 
    display.animate(i,200,5);
    display.animate(i,200,5);
    display.animate(i,200,5);
}

void crossAnim() {
    const uint8_t crossLED[] = { 1, 0, 0, 0, 1,  0, 0, 0, 0, 0, 
                                 0, 1, 0, 1, 0,  0, 0, 0, 0, 0,  
                                 0, 0, 1, 0, 0,  0, 0, 0, 0, 0,  
                                 0, 1, 0, 1, 0,  0, 0, 0, 0, 0,  
                                 1, 0, 0, 0, 1,  0, 0, 0, 0, 0, };
    MicroBitImage i(10,5,crossLED); 
    display.animate(i,80,5);
    display.animate(i,80,5);
    display.animate(i,80,5);
}

void connectedAnim() {
    const uint8_t crossLED[] = { 0, 0, 0, 0, 0,  0, 0, 0, 0, 1, 
                                 0, 0, 0, 0, 0,  0, 0, 0, 1, 0,  
                                 1, 0, 0, 0, 0,  1, 0, 1, 0, 0,  
                                 0, 1, 0, 0, 0,  0, 1, 0, 0, 0,  
                                 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, };
    MicroBitImage i(10,5,crossLED); 
    display.animate(i,250,5);
}



//Array that contains all the morese code combinations
const char* morseCode[36] = { ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", 
                            ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-", 
                            ".--", "-..-", "-.--", "--..", "-----", ".----", "..---", "...--", "....-", 
                            ".....", "-....", "--...", "---..", "----."};

 
//Array of alphapbets and numbers so we can retreive it by looking at its index
const char letter[36] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
                    'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '0', '1', '2', 
                    '3', '4', '5', '6', '7', '8', '9'};

//Adds the dots and dashes to an array that is used to convert into a letter
void addCodetoArray() {
  morseArray[loc] = tempString;
  morseArray[loc+1] = '\0';
  loc++;

  //check whether we have gone aboove the digit limit and resets the array
  if (loc > 5) {
    morseArray[0] = 0;
    morseArray[1] = 0;
    morseArray[2] = 0;
    morseArray[3] = 0;
    morseArray[4] = 0;
    morseArray[5] = 0;
    loc = 0;

    crossAnim();
    uBit.sleep(1000);
    uBit.display.clear();
  }
}


//Appends the letter into a car array that contains the word
void appendLetter(char* s, char c) {
        int len = strlen(s);
        s[len] = c;
        s[len+1] = '\0';
}

//Empty's the array that holds the word when its sent to the device
void clearLetterString(char* s) {
    int len = strlen(s);
    for( int a = 0; a < len; a++ ) {
      s[a] = '\0';
   }
}

//Displays the word currently stored in the device
void printMorseString() {
    display.clear();
    display.scroll(letterString);
    display.clear();
}

//function that sends a dot to the receiving micorbit
void DotButtonB(MicroBitEvent)
{
  tempString = '.';
  display.print(dot);
  addCodetoArray();
}

//function that sends a dash to the receiving micorbit
void DashButtonB(MicroBitEvent)
{
  tempString = '-';
  display.print(dash);
  addCodetoArray();
}

//function that used to convert the morse codes into a letter
void convert(MicroBitEvent)
{

  loc = 0;
  display.clear();

  int i = -1;
  bool match = false;
  while (match == false){
    i++;
    if (strcmp(morseArray, morseCode[i]) == 0){
      match = true;

      //output the letter
      display.print(letter[i]);
      //appends the letter to the end of the current word that is being transmitted
      appendLetter(letterString, letter[i]);
      uBit.sleep(4000);
    }
    else if (i > 35){
      //Animation when the microbit cant translate the code sent
      errorAnim();
    }
  }
}

//Prints the word on the micorbit
void getWord(MicroBitEvent)
{
  printMorseString();
}

void onConnected(MicroBitEvent e)
{
    connectedAnim();
    display.print("C");
    connected = 1;
    //sets the end of message as the return escape char, so once the user press sends
    //the device pushes it to the micor:bit
    ManagedString endPoint("\r");
 
    while (1)
    {
        ManagedString msg = uart->readUntil(endPoint);
        display.scroll(msg);
    }
}
 
void onDisconnected(MicroBitEvent e)
{
    crossAnim();
    display.print("D");
    connected = 0;
}
 
void onButtonAB(MicroBitEvent e)
{
    if (connected == 0)
    {
        crossAnim();
        //Not Connected
        display.scroll("NOT CONCT");
        return;
    }
    uart->send("word: ");
    uart->send(letterString);
    uart->send("\n"); //new line

    //display the word on the micor:bit
    display.scroll(letterString);
    
    //Clear the word array so its ready to send a new word
    clearLetterString(letterString);
}
 
 
int main()
{
    uBit.init();
    uBit.messageBus.listen(MICROBIT_ID_BLE, MICROBIT_BLE_EVT_CONNECTED, onConnected);
    uBit.messageBus.listen(MICROBIT_ID_BLE, MICROBIT_BLE_EVT_DISCONNECTED, onDisconnected);

    uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_CLICK, DotButtonB);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_LONG_CLICK, DashButtonB); 
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_CLICK, convert); 
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_LONG_CLICK, getWord); 
    
    //Send the word to the device
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_AB, MICROBIT_BUTTON_EVT_CLICK, onButtonAB);
 
    uart = new MicroBitUARTService(*uBit.ble, 32, 32);
    startAnim();
 
    release_fiber();
}
 
#endif