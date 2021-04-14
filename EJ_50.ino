#include <Servo.h> //Se incluye la biblioteca de servo

Servo myservo;  // Crea un objeto servo


int pos = 0;    // Variable para configurar la posición incial del servo

void setup() {
  myservo.attach(7);  // Conecta el servo en el pin 9 al objeto servo
}

void loop() {
  for (pos = 0; pos <= 180; pos += 1) { // Rango de 0 a 180 grados
    myservo.write(pos);              
    delay(15);                       // Espera 15ms para que el servo alcance la posición
  }
  
  for (pos = 180; pos >= 0; pos -= 1) { // Ve de 180 grados a 0 grados
    myservo.write(pos);              
    delay(15);                      // Espera 15ms para que el servo alcance la posición
  }

}
