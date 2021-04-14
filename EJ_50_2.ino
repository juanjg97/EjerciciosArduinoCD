#include <Servo.h> //Se incluye la biblioteca de servo

Servo servoUno;  // Crea un objeto servo
int pos = 90;    // Variable para configurar la posición incial del servo
bool sum;
bool res;



void setup() {
pinMode(55, INPUT_PULLUP);
pinMode(56, INPUT_PULLUP);
servoUno.attach(13);  // Establece la conexión de la terminal de control del servomotor con la salida analógica (PWM)

}

void loop() {
//Leemos entradas

sum=!digitalRead(55);
res=!digitalRead(56);



  if(sum)
  {
      do{
      sum=!digitalRead(55);
      }while(sum);
      
      pos=pos+1;
      servoUno.write(pos);
      delay(15);     
  }

  if(res)
  {
    do{
      res=!digitalRead(56);
      }while(res);
   
    pos=pos-1;
    servoUno.write(pos);
    delay(15);  
  }


}
