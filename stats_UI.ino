#include <LiquidCrystal.h>// include the library code
/**********************************************************/
String title = "Super Accurate Micrometer";
String calibrate = "Set Micrometer and Press Left to Start";

int tim = 300; //the value of delay time
int menuStatus = 0;
int horizontalPos = 16;
int startPos, endPos = 0; 
const int leftButton = 53;
const int midButton = 51;
const int rightButton = 49;
const int measurePot = 0;
int zeroVal = 0;
int timer = 0;
float upperLimit = 310;
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(52, 50, 48, 46, 44, 42);

/*********************************************************/
void setup()
{
lcd.begin(16, 2); // set up the LCD's number of columns and rows: 
pinMode(leftButton, INPUT);
pinMode(midButton, INPUT);
pinMode(rightButton, INPUT);
Serial.begin(9600);

}
/*********************************************************/
void loop() 
{
while(menuStatus == 0)
{
  lcd.setCursor(horizontalPos,0);
  lcd.print(title.substring(startPos, endPos));
  lcd.setCursor(0,1); 
  lcd.print("Meas");
  lcd.setCursor(5,1);
  lcd.print("Calib");
  lcd.setCursor(11,1);
  lcd.print("Debug");
  
  if (digitalRead(leftButton) == HIGH)
  {
    while(digitalRead(leftButton) == HIGH){}
    lcd.clear();
    menuStatus = 1;
  }
    
  else if (digitalRead(midButton) == HIGH)
  {
    while(digitalRead(midButton) == HIGH){}
    lcd.clear();
    startPos = 0;
    endPos = 0;
    horizontalPos = 16;
    menuStatus = 2;
  }

  else if (digitalRead(rightButton) == HIGH)
  {
    while(digitalRead(rightButton) == HIGH){}
    lcd.clear();
    menuStatus = 3;
  }
  
  if(startPos == 0 && horizontalPos > 0) {
    horizontalPos--;
    endPos++;
  }
  else if(startPos == endPos) {
    startPos = endPos = 0;
    horizontalPos = 16;
  }
  else if (endPos == title.length() && horizontalPos == 0) {
    startPos++;
  } else {
    startPos++;
    endPos++;
  }
  delay(300);
  lcd.clear();
}


while (menuStatus == 1)
{
  int measurement = analogRead(measurePot);
  int measureStats[5] = {0};
  measurement = map(measurement, 0, 1023, 0, upperLimit);
  lcd.setCursor(0,0);
  lcd.print("Distance");
  
  lcd.setCursor(12,0);
  lcd.print("EXIT");

  lcd.setCursor(0,1);
  lcd.print(float(measurement-zeroVal)/100);
  lcd.setCursor(5,1);
  lcd.print("mm");
  if(digitalRead(midButton) == HIGH)
  {
    zeroVal = measurement;
  }

  delay(1000);
  lcd.clear();
  

  if (digitalRead(rightButton) == HIGH)
  {
    while(digitalRead(rightButton) == HIGH){}
    lcd.clear();
    timer = 0;
    menuStatus = 0;
  }  
}

while(menuStatus == 2 || menuStatus == 4) {
  
  int cursorLocation = 0;
  
  while (digitalRead(rightButton) == LOW)
  {
    lcd.setCursor(0,0);
    lcd.print("Calibrate");
    
    lcd.setCursor(12,0);
    lcd.print("EXIT");

    lcd.setCursor(horizontalPos,1);
    lcd.print(calibrate.substring(startPos, endPos));

    if(startPos == 0 && horizontalPos > 0) {
      horizontalPos--;
      endPos++;
      }
      else if(startPos == endPos) {
        startPos = endPos = 0;
        horizontalPos = 16;
      }
      else if (endPos == calibrate.length() && horizontalPos == 0) {
        startPos++;
      } else {
        startPos++;
        endPos++;
      }
      delay(300);
      lcd.clear();

      double measurementRaw = analogRead(measurePot);
      int measurement = map(measurementRaw, 0 ,1023, 0, floor(upperLimit));
      int measureAdjust[3] = {0};
      measureAdjust[0] = floor(measurement/100);  
      measureAdjust[1] = floor((measurement - measureAdjust[0]*100)/10); 
      measureAdjust[2] = floor(measurement - measureAdjust[0]*100 - measureAdjust[1]*10);
      
     if (digitalRead(leftButton) == HIGH && measurement > 10 )
    {
      while(digitalRead(leftButton) == HIGH){}
      menuStatus = 4;

    }
      
      while(menuStatus == 4)
      {
        int leftPos = 3; 
        lcd.setCursor(0,0);
        lcd.print("Calibrate");
        lcd.setCursor(12,0);
        lcd.print("EXIT");
        
        int tempMeas = measureAdjust[0]*100 + measureAdjust[1]*10 + measureAdjust[2];     
        lcd.setCursor(leftPos,1);
        lcd.print(measureAdjust[0]);
        lcd.setCursor(leftPos+1,1);
        lcd.print(measureAdjust[1]);
        lcd.setCursor(leftPos+2,1);
        lcd.print(measureAdjust[2]);
        lcd.setCursor(leftPos+4,1);
        lcd.print("x10 um");
        delay(250);
        lcd.setCursor(leftPos + (cursorLocation%3),1);
        lcd.print(" ");
        delay(250);
        
        if (digitalRead(leftButton) == HIGH)
        {
          while(digitalRead(leftButton) == HIGH){}
          cursorLocation++;
        }
        
        if (digitalRead(midButton) == HIGH)
        {
          while(digitalRead(midButton) == HIGH){}
          measureAdjust[cursorLocation%3] = (measureAdjust[cursorLocation%3]+1)%10;
        }   

        if (digitalRead(rightButton) == HIGH)
        {
          while(digitalRead(rightButton) == HIGH){}
          upperLimit = 1023.0*tempMeas/measurementRaw;
//          delay(500);
          lcd.clear();
          menuStatus = 0;
        } 
      
    } 
  }  

   if (digitalRead(rightButton) == HIGH)
    {
      while(digitalRead(rightButton) == HIGH){}
      lcd.clear();
      menuStatus = 0;
    } 
}

while(menuStatus == 3) {
  int measurementRaw = analogRead(measurePot);
  int measureStats[5] = {0};
  int measurement = map(measurementRaw, 0, 1023, 0, upperLimit);
  lcd.setCursor(0,0);
  lcd.print("UL");
  lcd.setCursor(0,1);
  lcd.print(int(upperLimit));
  
  lcd.setCursor(6,0);
  lcd.print("MR");
  lcd.setCursor(6,1);
  lcd.print(measurementRaw);
  

  lcd.setCursor(12,0);
  lcd.print("MV");
  lcd.setCursor(12,1);
  lcd.print(measurement);

 
  delay (500);
  lcd.clear();
  if (digitalRead(rightButton) == HIGH)
  {
    while(digitalRead(rightButton) == HIGH){}
    lcd.clear();
    menuStatus = 0;
  } 
}


}



/************************************************************/
