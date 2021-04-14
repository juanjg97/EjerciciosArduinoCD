//Ejercicio 46, Juanjo
 
  byte thisPin;
  int delayTime=20;
  const byte ledPins[]={54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,49,48,47,46,45,44,43,42};   
  const byte ledCount=24;
  
void setup() {
 
  int i=0;
  for(i=54;i<=69;i++)
  {
      pinMode(i,OUTPUT);
  }
   for(i=42;i<=49;i++)
  {
      pinMode(i,OUTPUT);
  }

}

void loop() {
  
   for(thisPin=0;thisPin<ledCount-1;thisPin++)
  {
    digitalWrite(ledPins[thisPin],HIGH);
    delay(delayTime);
    digitalWrite(ledPins[thisPin],LOW);
   
  }
       

   for(thisPin=ledCount-1;thisPin>0;thisPin--)
    {
    digitalWrite(ledPins[thisPin],HIGH);
    delay(delayTime);
    digitalWrite(ledPins[thisPin],LOW);
    }

}
