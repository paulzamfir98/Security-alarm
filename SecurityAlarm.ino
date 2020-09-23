

#include <Keypad.h>
#include <LiquidCrystal.h>
//Sensor, led and buzzer
const int securityLed = A5;
const int securitySensorTrig = A2;
const int securitySensorEcho = A3;
const int securityBuzzer = A4;
long time;
long distance;


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
byte rowPins[numRows] = {9,8,7,6}; //Rows 0 to 3
byte colPins[numCols]= {5,4,3,2}; //Columns 0 to 3

//initializes an instance of the Keypad class
Keypad myKeypad= Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);
  
// Creates an LC object. Parameters: (rs, enable, d4, d5, d6, d7) 
LiquidCrystal lcd(A0, A1, 10, 11, 12, 13); 

char keyPressed;
String password="1234";
String intermediatePassword="";
boolean alarmActivated = false;
boolean breaking = false;
boolean currentPasswordEntered = false;
byte page=0;
int numChar=0;
int numTries=0;



void setup(){
  Serial.begin(9600);
    lcd.begin(16,2);
  
    //Sensor, led and buzzer
    pinMode(securityLed,OUTPUT);
  pinMode(securityBuzzer,OUTPUT);
  pinMode(securitySensorTrig,OUTPUT);
    pinMode(securitySensorEcho,INPUT);
}

void loop(){    
  
  
    if(!alarmActivated){
        switch(page){
          case 0:
                showMenu();
                break;
            case 1:
              activateAlarm();
              break;
            case 2:
              changePassword();
              break;
            default:
              break;
        }
    }
    else{
        if(!breaking){
            digitalWrite(securityLed,LOW);
            noTone(securityBuzzer);
            getDistance();
            delay(100);
            if(distance<30){
                breaking=true;
                lcd.clear();
            }
        }
        else{
            digitalWrite(securityLed,HIGH);
            tone(securityBuzzer,1000);
      delay(100);
            digitalWrite(securityLed,LOW);
            noTone(securityBuzzer);
            delay(100);
          alarmPassword();
        }
    }
}

void showMenu(){
    lcd.setCursor(0, 0);
    lcd.print("A - Activate ");
    lcd.setCursor(0, 1);
    lcd.print("B - Change Pass");
    keyPressed = myKeypad.getKey();
    if(keyPressed == 'A'){
       lcd.clear();
       page = 1;
    }else if(keyPressed == 'B'){
       page = 2;
       lcd.clear();
    }
}

void activateAlarm() {
    lcd.setCursor(0,0);
    lcd.print("Alarm will be");
    lcd.setCursor(0,1);
    lcd.print("activated in 10");
    delay(1000);
  
    lcd.setCursor(14,1);
    lcd.print(" ");
  
    lcd.setCursor(0,1);
    lcd.print("activated in ");
    byte counter = 9;
    while(counter > 0){
        lcd.setCursor(13,1);
        lcd.print(counter);
        delay(1000);
      counter--;
    
    }
    alarmActivated = true;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Alarm activated!");
    delay(2000);
    
}

void changePassword(){
    if(!currentPasswordEntered){
        lcd.setCursor(0, 0);
        lcd.print("Current Password");
        lcd.setCursor(0, 1);
        lcd.print("Pass:"); 
        
        enterPassword();
      
        if(keyPressed == '*'){
            lcd.clear();
            if(password.compareTo(intermediatePassword) == 0){
              currentPasswordEntered = true;
            }else{
              lcd.setCursor(0, 0);
                lcd.print("Wrong Password!");
                lcd.setCursor(0, 1);
                lcd.print("Try Again!");
                delay(2000);
            }
            numChar = 0;
            intermediatePassword = "";
            lcd.clear();
            
        }

    }
    else{
        lcd.setCursor(0, 0);
        lcd.print("New Password");
        lcd.setCursor(0, 1);
        lcd.print("Pass:"); 
        
      
        enterPassword();
      
        if(keyPressed == '*' && intermediatePassword.length()>0){
          lcd.clear();
            password = intermediatePassword;
            lcd.setCursor(0, 0);
            lcd.print("Password changed!");
            delay(2000);
          
            numChar = 0;
            intermediatePassword = "";
            currentPasswordEntered = false;
            lcd.clear();
            page = 0;
        }

    }
  
}

void alarmPassword (){
    if(numTries <2){
        lcd.setCursor(0, 0);
        lcd.print(" *** ALARM ***");
        lcd.setCursor(0,1);
        lcd.print("Pass:");
      
      enterPassword();
      
        if(keyPressed == '*'){
            lcd.clear();
            lcd.setCursor(0, 0);
            if(password.compareTo(intermediatePassword) == 0){
                breaking = false;
                alarmActivated = false;
                numTries = 0;
                page = 0;
                lcd.setCursor(0,0);
                lcd.print(" *** ALARM ***");
                lcd.setCursor(0,1);
                lcd.print(" *DEACTIVATED*");
                delay(2000);
            }else{
                lcd.print("Wrong Password!");
                lcd.setCursor(0, 1);
                lcd.print("Try Again!");
                delay(5000);
                numTries++;
            }
            numChar = 0;
            intermediatePassword = "";
            lcd.clear();
        }
    }
    else{
    lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(" *** ALARM ***");
    }
              
}
  
void enterPassword(){
    lcd.setCursor(5 + numChar, 1);
    keyPressed = myKeypad.getKey();
    if(keyPressed == '0' || keyPressed == '1'  || keyPressed == '2' || keyPressed == '3' || keyPressed == '4' ||
       keyPressed == '5' || keyPressed == '6'  || keyPressed == '7' || keyPressed == '8' || keyPressed == '9'){
        lcd.print("*");
        numChar++;
        intermediatePassword+= keyPressed;
    }
    
    if(keyPressed == 'C'){
        numChar--;
        lcd.setCursor(5 + numChar, 1);
        lcd.print(" ");
        intermediatePassword.remove(intermediatePassword.length()-1);
    }
}


//Computes the distance from sensor.
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
  
  
  
  
  
