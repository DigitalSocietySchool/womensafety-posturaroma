//////////////////////////////////////////////////////////////////
//©2011 bildr
//Released under the MIT License - Please reuse change and share
//Simple code for the ADXL335, prints calculated orientation via serial
//////////////////////////////////////////////////////////////////

//2014.1.14 by Shinichiro ITO | shinichiro.shin1ro@gmail.com


//Analog read pins
const int xPin = 0;
const int yPin = 1;
const int zPin = 2;

//The minimum and maximum values that came from
//the accelerometer while standing still
//You very well may need to change these
int minVal = 390;
int maxVal = 620;

int aroma = 5;
int ledPin = 13;// the small LED on the Board blinks for Checking it works or not

int process = 0;// to send the status to Processing

//to hold the caculated values
double x;
double y;
double z;

//Variables for calibration switch
const int  buttonPin = 3;    // the pin that the pushbutton is attached to

// Variables will change:
int buttonPushCounter = 0;   // counter for the number of button presses
int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button

int goodPostureZ = 0;
int badPostureZ = 0;
int diffZ = 0;

int goodPostureX = 0;
int badPostureX = 0;
int diffX = 0;

int goodPostureY = 0;
int badPostureY = 0;
int diffY = 0;

void setup(){
  Serial.begin(9600); 
  pinMode(buttonPin, INPUT_PULLUP); //D3 for switch
  pinMode(aroma,OUTPUT); //D5
  pinMode(ledPin,OUTPUT);//D13
  
  //Make sure the analog-to-digital converter takes its reference voltage from
  // the AREF pin
  analogReference(EXTERNAL);
}



void loop(){
  
        Serial.print("goodPostureZ:  ");
        Serial.println(goodPostureZ);  
        Serial.print("badPostureZ:  ");
        Serial.println(badPostureZ);

        Serial.print("goodPostureX:  ");
        Serial.println(goodPostureX);  
        Serial.print("badPostureX:  ");
        Serial.println(badPostureX);        
        
        Serial.print("goodPostureY:  ");
        Serial.println(goodPostureY);  
        Serial.print("badPostureY:  ");
        Serial.println(badPostureY);
        
        Serial.print("diffZ:  ");
        Serial.println(diffZ);

        Serial.print("diffX:  ");
        Serial.println(diffX);

        Serial.print("diffY:  ");
        Serial.println(diffY);


        Serial.print("number of button pushes:  ");
        Serial.println(buttonPushCounter);
        
       
  //read the analog values from the accelerometer
  int xRead = analogRead(xPin);
  int yRead = analogRead(yPin);
  int zRead = analogRead(zPin);
  Serial.print("xRead: ");
  Serial.print(xRead);
  Serial.print(" | yRead: ");
  Serial.print(yRead);
  Serial.print(" | zRead: ");
  Serial.print(zRead );

  //convert read values to degrees -90 to 90 - Needed for atan2
  int xAng = map(xRead, minVal, maxVal, -90, 90);
  int yAng = map(yRead, minVal, maxVal, -90, 90);
  int zAng = map(zRead, minVal, maxVal, -90, 90);
  
  Serial.print(" || ");
  Serial.print("xAng: ");
  Serial.print(xAng);
  Serial.print(" | yAng: ");
  Serial.print(yAng);
  Serial.print(" | zAng: ");
  Serial.print(zAng);
  
  //Caculate 360deg values like so: atan2(-yAng, -zAng)
  //atan2 outputs the value of -π to π (radians)
  //We are then converting the radians to degrees
  x = RAD_TO_DEG * (atan2(-yAng, -zAng) + PI);
  y = RAD_TO_DEG * (atan2(-xAng, -zAng) + PI);
  z = RAD_TO_DEG * (atan2(-yAng, -xAng) + PI);

  //Output the caculations
   Serial.print(" || ");
  Serial.print("x: ");
  Serial.print(x);
  Serial.print(" | y: ");
  Serial.print(y);
  Serial.print(" | z: ");
  Serial.println(z);
  
  //*************threshold***************
  
  if((diffX > diffY) && (diffX > diffZ)){
    if((abs(xAng - goodPostureX) > abs(xAng - badPostureX)) && buttonPushCounter > 2){
      digitalWrite(aroma,HIGH);
      digitalWrite(ledPin,HIGH);
      Serial.println(" | Judgement X : True ON ");
      
      process = 1;
      Serial.print(process);

    }else
    {
      digitalWrite(aroma,LOW);
      digitalWrite(ledPin,LOW);
      Serial.println(" | Judgement X : False OFF ");
      
      process = 0;
      Serial.print(process);
    }
    
  } else if((diffY > diffX) && (diffY > diffZ)){
    if((abs(yAng - goodPostureY) > abs(yAng - badPostureY)) && buttonPushCounter > 2){
      digitalWrite(aroma,HIGH);
      digitalWrite(ledPin,HIGH);
      Serial.println(" | Judgement Y : True ON ");
      
      process = 1;
      Serial.print(process);
      
    }else
    {
      digitalWrite(aroma,LOW);
      digitalWrite(ledPin,LOW);
      Serial.println(" | Judgement Y : False OFF ");
      process = 0;
      Serial.print(process);
      
    }
  } else if((diffZ > diffX) && (diffZ > diffY)){
    if((abs(yAng - goodPostureY) > abs(yAng - badPostureY)) && buttonPushCounter > 2){
      digitalWrite(aroma,HIGH);
      digitalWrite(ledPin,HIGH);
      Serial.println(" | Judgement Y : True ON ");
      process = 1;
      Serial.print(process);
      
    }else
    {
      digitalWrite(aroma,LOW);
      digitalWrite(ledPin,LOW);
      Serial.println(" | Judgement Y : False OFF ");
      process = 0;
      Serial.print(process);
      
    }
  } else {
    Serial.println(" Not started");
  }
  
  /*
  if((abs(zAng - goodPostureZ) > abs(zAng - badPostureZ)) && buttonPushCounter > 2){
  //digitalWrite(aroma,HIGH);
  digitalWrite(ledPin,HIGH);
  Serial.println(" | Judgement : True ON ");
  }else
  {
    //digitalWrite(aroma,LOW);
    digitalWrite(ledPin,LOW);
    Serial.println(" | Judgement : False OFF ");
  }
  */
  
  //*************Calibration***************
  // read the pushbutton input pin:
  buttonState = digitalRead(buttonPin);

  // compare the buttonState to its previous state
  if (buttonState != lastButtonState) {
    // if the state has changed, increment the counter
    if (buttonState == HIGH) {
      // if the current state is HIGH then the button
      // wend from off to on:
      buttonPushCounter++;
      Serial.println("on");
      Serial.print("number of button pushes:  ");
      Serial.println(buttonPushCounter);
      
      if(buttonPushCounter == 2){
        goodPostureZ = zAng;
        Serial.print("goodPostureZ:  ");
        Serial.println(goodPostureZ);
        goodPostureX = xAng;
        Serial.print("goodPostureX:  ");
        Serial.println(goodPostureX);
        goodPostureY = yAng;
        Serial.print("goodPostureY:  ");
        Serial.println(goodPostureY);
        
      }
      if(buttonPushCounter == 3){
        badPostureZ = zAng;
        Serial.print("badPostureZ:  ");
        Serial.println(badPostureZ);
        badPostureX = xAng;
        Serial.print("badPostureX:  ");
        Serial.println(badPostureX);
        badPostureY = yAng;
        Serial.print("badPostureY:  ");
        Serial.println(badPostureY);
        
        diffZ = abs( goodPostureZ-badPostureZ);  
        Serial.print("diffZ:  ");
        Serial.println(diffZ);  

        diffX = abs( goodPostureX-badPostureX);  
        Serial.print("diffX:  ");
        Serial.println(diffX);
        
        diffY = abs( goodPostureY-badPostureY);  
        Serial.print("diffY:  ");
        Serial.println(diffY);
      }      
      
    } 
    else {
      // if the current state is LOW then the button
      // wend from on to off:
      Serial.println("off"); 
    }
  }
  

  // save the current state as the last state, 
  //for next time through the loop
  lastButtonState = buttonState;

  
  delay(300);//just here to slow down the serial output - Easier to read
}
