/***********************************ARCHIVOS DE BIBLIOTECA (1)**********************/
#include <Stepper.h>
#include <Servo.h>
#include <LiquidCrystal.h>
/***********************************INICIALIZAR MOTORES Y LCD(2)**********************/
//Para el Motor PP
const int stepsPerRevolution = 4; 
Stepper miStepper(stepsPerRevolution,62,63,64,65);  //Declaramos las terminales de las bobinas del motor

//Para el ServoMotor
Servo miServo;
int pos = 90;

//Para la pantalla LCD
LiquidCrystal lcd (56,57,58,59,60,61);    //RS,EN,D4,D5,D6,D7

/***********************************DECLARACIÓN DE VARIABLES A USAR (3)**********************/
//Global
byte numSW       = 8;
byte sentido     = -1;
byte initialTime = 0;
byte finalTime   = 0;
byte elapsedTime = 0;
byte value       = 0;   //Es lo mismo que (pos)
byte servoDelayTime = 0;
bool DIR            = LOW;
bool AngErroneo     = LOW;
bool BitSigno       = LOW;
byte LCDdelayTime   = 50;
byte num            = 0;
byte numAbs         = 0;
byte numServo       = 0;
bool numNegativo    =false;
byte x=0;

//Entradas
boolean SEL = LOW;     //entran por el pin 21
boolean GO = LOW;      //entran por el pin 20

//Entrada Dipswitch
const byte entradaSW[]{49,48,47,46,45,44,43,42};

//Salidas
byte ONled  = 40;
byte SELled = 41;
byte PWM    = 0;     //***por si se usa***


//Para la conversión
const byte Conv[]{1,2,4,8,16,32,64,128};

//Variable muda
byte thisPin;
byte thisChar;

//********Salidas CENTENAS, DECENAS Y UNIDADES
const byte unidades[]{53,52,51,50,10,11,12};
const byte decenas[]{37,36,35,34,33,32,31};
const byte centenas[]{22,23,24,25,26,27,28};

//Display
const byte numSsLeds = 7;
const boolean sevenSegOut[][7]={
  {1,1,1,1,1,1,0},  //muestra 0
  {0,1,1,0,0,0,0},  //muestra 1
  {1,1,0,1,1,0,1},  //muestra 2
  {1,1,1,1,0,0,1},  //muestra 3
  {0,1,1,0,0,1,1},  //muestra 4
  {1,0,1,1,0,1,1},  //muestra 5
  {1,0,1,1,1,1,1},  //muestra 6
  {1,1,1,0,0,0,0},  //muestra 7
  {1,1,1,1,1,1,1},  //muestra 8
  {1,1,1,1,0,1,1}}; //muestra 9


/******************************Configuración de enradas y salidas,/digitales, analogicas (4)**********************/
/**********Terminal de control del servo, Caracteres LCD, Encender el cursor y Configurar las interrupciones (4)*************/

void setup() {

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//ENTRADAS
  pinMode(21, INPUT_PULLUP);   //señal del botón SEL
  pinMode(20, INPUT_PULLUP);   //señal del botón GO

    //entradas Dipswitch
  for(thisPin = 0; thisPin < numSW; thisPin++)  
  {
    pinMode(entradaSW[thisPin],INPUT_PULLUP);
  }

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//SALIDAS


//Salida de los botones           
  pinMode(ONled,OUTPUT);     //ONled
  pinMode(SELled,OUTPUT);    //SELled
  pinMode(66,OUTPUT);  
  pinMode(16,OUTPUT);  
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//Visualización del número del DIPSWITCH
  for(thisPin = 0; thisPin<numSsLeds; thisPin++)
    {
    pinMode(unidades[thisPin],OUTPUT);
    pinMode(decenas [thisPin],OUTPUT);
    pinMode(centenas[thisPin],OUTPUT);    
    }
 
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//Servo 
  miServo.attach(13);    //Pin 13 es la salida al servo
  miServo.write(pos);  //Manda al servo a la posición pos
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//LCD 
  lcd.begin(16,2);            //16 caracteres por línea, 2 líneas
  lcd.cursor();

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//Configuración de las características de interrupción 
//  attachInterrupt(digitalPinToInterrupt(21), SELbtnPress,FALLING);
//  attachInterrupt(digitalPinToInterrupt(20), GObtnPress ,FALLING);
}


//***************************************************Rutinas de servicio**************************************
/*************************************PROGRAMA PRINCIPAL(5)*******************************/
void loop() 
{

 NumDip();
 muestraAng();//Muestra DipSwitch
 moveStepper ();    
  
}




//***************************************************Rutinas de servicio**************************************
//***************************************************Rutinas de servicio**************************************

 

//+++++++++++++++++++++++++++++
//Rutina para el motor PP
void moveStepper ()
  { 
     digitalWrite(16,HIGH);    
     if(num<128)
        {
          //Giro Levógiro
          DIR=1;
          numAbs=128-num;
        }
        if(num>128)
        {
          //Giro Dexógiro
          numAbs=num-128;
          DIR=0;
        }
        int motorSpeed = map (numAbs, 0, 127, 0, 180); 
        miStepper.setSpeed (motorSpeed);
        if (DIR)
        (miStepper).step (stepsPerRevolution/4); 
        else
        (miStepper).step (-stepsPerRevolution/4);
          
}
  




//+++++++++++++++++++++++++++
//Obtenemos números de dipswitch

void NumDip()
  {
     num = 0;
     for(thisPin = 0; thisPin < numSW; thisPin++)
     {
       boolean pinVal = !digitalRead(entradaSW[thisPin]);     
       num += byte(pinVal)*Conv[thisPin];
          
     }
     
  }

//+++++++++++++++++++++++++++
//Muestra números en los displays

void muestraAng()
{      
  //Unidades, Decenas y Centenas
  int cent = (num/100);
  int dec  = (num/10)%10;
  int uni  = (num%10);
  
  //salida UNIDADES
  for(thisPin = 0; thisPin < numSsLeds; thisPin++)
    {
    digitalWrite(unidades[thisPin],sevenSegOut[uni][thisPin]);
    }
   //Salida DECENAS
  for(thisPin = 0; thisPin < numSsLeds; thisPin++)
    {
    digitalWrite(decenas[thisPin],sevenSegOut[dec][thisPin]);
    }
    //Salida CENTENAS
  for(thisPin = 0; thisPin < numSsLeds; thisPin++)
    {
    digitalWrite(centenas[thisPin],sevenSegOut[cent][thisPin]);
    }
}
