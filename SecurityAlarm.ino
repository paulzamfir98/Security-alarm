
#include <Keypad.h>

int led = 2; // the pin the LED is connected to
int buzzer = 3;
int securitySensorTrig = 5;
int securitySensorEcho = 4;
int distance;
long time;

const byte numRows= 4; 
const byte numCols= 4;

//keymap defines the key pressed according to the row and columns just as appears on the keypad
char keymap[numRows][numCols]= 
{
{'1', '2', '3', 'A'}, 
{'4', '5', '6', 'B'}, 
{'7', '8', '9', 'C'},
{'*', '0', '#', 'D'}
};


//Code that shows the the keypad connections to the arduino terminals
byte rowPins[numRows] = {26,27,28,29}; //Rows 0 to 3
byte colPins[numCols]= {22,23,24,25}; //Columns 0 to 3

//initializes an instance of the Keypad class
Keypad myKeypad= Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);

char keyPressed;

void setup() {
  Serial.begin(9600);
  pinMode(led, OUTPUT); // Declare the LED as an output
  pinMode(buzzer, OUTPUT);
  pinMode(securitySensorTrig,OUTPUT);
  pinMode(securitySensorEcho,INPUT);
  distance= 100;
}

void loop() {
    getDistance();
    keyPressed = myKeypad.getKey();
    if(keyPressed)
    {
        Serial.println(keyPressed);
    }
     if(distance<30)
    {
        digitalWrite(led, HIGH); // Turn the LED on
        digitalWrite(buzzer, HIGH);
        delay(500);
        digitalWrite(led,LOW);
        digitalWrite(buzzer,LOW);
        delay(500);
        
    }
   
}

void getDistance(){
  
    //Turn off the Trig, in case it was on
    digitalWrite(securitySensorTrig,LOW);
  
    //Turn on the Trig, then turn off the pin
    digitalWrite(securitySensorTrig,HIGH);
    digitalWrite(securitySensorTrig,LOW);
  
    //Get the time
    time=pulseIn(securitySensorEcho,HIGH);
  
    //Convert the time in distance.
    //The speed of sound is 340m/s => 0.034cm/microseconds
    //Sound wave reflects from the object, so we have to divide the time by 2
    
    distance=time*0.034/2;
}
  
