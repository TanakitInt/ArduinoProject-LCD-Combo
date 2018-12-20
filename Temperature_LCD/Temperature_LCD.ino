//**5V in**

//Coded by TanakitInt.

//LCD----------------------------------------------------------------------------------
//Libraries 
#include <Wire.h>  
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address, if it's not working try 0x27 or 0x3F.

/*checking I2C port*/

/*

// I2C Scanner
// Written by Nick Gammon
// Date: 20th April 2011

#include <Wire.h>

void setup() {
  Serial.begin (9600);
  Serial.println ("I2C scanner. Scanning ...");
  byte count = 0;
  
  Wire.begin();
  for (byte i = 8; i < 120; i++)
  {
    Wire.beginTransmission (i);
    if (Wire.endTransmission () == 0)
      {
      Serial.print ("Found address: ");
      Serial.print (i, DEC);
      Serial.print (" (0x");
      Serial.print (i, HEX);
      Serial.println (")");
      count++;
      delay (1);  // maybe unneeded?
      } // end of good response
  } // end of for loop
  Serial.println ("Done.");
  Serial.print ("Found ");
  Serial.print (count, DEC);
  Serial.println (" device(s).");
}  // end of setup

void loop() {}

*/

//TEMPERATURE---------------------------------------------------------------------------
//set analog pin
  int ThermistorPin = A0;

//set constant resistance for know resistor
  float R1 = 10000;

//initial value for Steinhart-Hart equation
//used for convert the resistance of the thermistor to a temperature reading
  int Vo;
  float logR2, R2, T;
  float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

//PHOTO RESISTOR------------------------------------------------------------------------
  int sensorPin = A7;   // select the analog input pin for the photoresistor
  int threshold = 400;  // analog input trigger level from photoresistor
  int sensorValue = 0;  // photoresistor value read from analog input pin
  int ledPin = 52;

void setup() 
{
  //LCD---------------------------------------------------------------------------------
  lcd.begin(16,2);   // iInit the LCD for 16 chars 2 lines
  
  lcd.backlight();   // Turn on the backligt (try lcd.noBaklight() to turn it off)
  
  //TEMPERATURE-------------------------------------------------------------------------
  Serial.begin(9600);

  //PHOTO RESISTOR----------------------------------------------------------------------
  pinMode(ledPin, OUTPUT);
}

void loop() 
{
  //TEMPERATURE-------------------------------------------------------------------------
  //get analogRead from analog pin
  Vo = analogRead(ThermistorPin);

  //Steinhart-Hart equation
  //the T ouput is Kelvin
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));

  //Convert Kelvin to Celsius
  T = T - 273.15;

  //print the value
  Serial.print("Temp : "); 
  Serial.print(T);
  Serial.println(" C ,"); 
  
  //Setup LCD line
  
  lcd.setCursor(0,0); //First line
  
  lcd.print("Temp : "); 
  lcd.print(T);
  lcd.println(" C  "); 

  //PHOTO RESISTOR---------------------------------------------------------------------
  lcd.setCursor(0,1); //Second line
  
  Serial.print("Light: ");
  Serial.print(analogRead(sensorPin));
  Serial.println(" lux ,");
  
  lcd.print("Light: ");
  lcd.print(analogRead(sensorPin));
  
  if (analogRead(sensorPin) < threshold) 
  {    
    digitalWrite(ledPin, HIGH);    
    Serial.println("ON!  ");

    lcd.print(" ON!  ");
  }
  else 
  {    
    digitalWrite(ledPin, LOW);
    Serial.println("OFF  ");
    
    lcd.print(" OFF  ");
  }

  //---------------------------------------------------------------------------------
  //set delay output in millisecond (ms)
  delay(1000);
  
}
