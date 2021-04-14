#include <LiquidCrystal.h>
LiquidCrystal lcd(15,14,49,48,47,46,45,44,43,42); 

byte sign1CharNum = 49;



void setup() {
  lcd.begin(16,2);


}

void loop() {
    lcd.setCursor(0,1);
    lcd.autoscroll ();
    char sign1[] = "                Set dipswitch and press GO button ";
    lcd.clear ();
    for (byte thisChar; thisChar < sign1CharNum; thisChar++)
    {
      lcd.write (sign1[thisChar]); 
      delay (100);
    }



}
