#include <Stepper.h>

const int stepsPerRevolution = 4;  //El motor de proteus es de 4 pasos por revolución
bool ONsignal=LOW; //Botón para la señal de interrupción encendido-apagado
bool DIRsignal=LOW;//Botón para la señal de interrupción cambio de sentido de giro
int  sentido=1;//Variable para controlar el signo, en el sentido de giro

Stepper myStepper(stepsPerRevolution, 62, 63, 64, 65); //Declaramos las terminales de las bobinas del motor


void setup() {
  pinMode(2, INPUT_PULLUP);//Entrada pull up para el botón encendido-apagado
  pinMode(3, INPUT_PULLUP);//Entrada pull up para el botón cambio de sentido
  pinMode(55,OUTPUT);//Salida para activar o desactivar el botón

  //Configuración de las características de interrupción 
  attachInterrupt (digitalPinToInterrupt (2), ONbtnPress, FALLING);
  attachInterrupt (digitalPinToInterrupt (3), DIRbtnPress,FALLING);
  
}

void loop() {

GiroMotor (); //Trabajamos con una función en lugar de escribir el código completo en el loop

 
}

//**********************Funciones***********************************//

//**********************ENCENDER-APAGAR***********************************//
void ONbtnPress () //Función o rutina para la interrupción de encendido-apagado
{                  //Se ejecuta cuando se presiona el botón, de encendido apagado

ONsignal = !ONsignal;       // Cambia el estado de la variable ONsignal 
delay (600);                // Retardo de tiempo para filtrar los “rebotes” 
digitalWrite (55, ONsignal);// Señal que sale para encender o apagar el motor
}

//**********************CAMBIO DE DIRECCIÓN***********************************//

void DIRbtnPress ()//Función o rutina para la interrupción de cambio de sentido de giro
{
DIRsignal = !DIRsignal;// Cambia el estado de la variable DIRsignal 
delay (600);// Retardo de tiempo para filtrar los “rebotes” 

 //Con estas condiciones hacemos que cambie la variable que establece el signo, para el sentido
 //de giro del motor
  if(DIRsignal)
  {
    sentido=1;
  }
  else if(!DIRsignal)
    {
      sentido=-1;
  
    }

}
//**********************GIRO DEL MOTOR ***********************************//

void GiroMotor ()
{
  int sensorReading = analogRead(A0);//Leemos la señal del potenciometro
  int motorSpeed = map(sensorReading, 0, 1023, 0, 180);//Mapea el valor para ajustarlo a un rango de 0 a 180 rpm 

  if (motorSpeed > 0) //Si el valor del potenciometro hace que motor speed sea >0, comienza a girar el motor
    {
      myStepper.setSpeed(motorSpeed);//Define la velocidad a girar
      myStepper.step((sentido)*stepsPerRevolution / 4);//Damos el sentido de giro, con ayuda de la variable auxiliar
    }
}
