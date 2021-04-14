// PROYECTO 2
// Incluir archivos de biblioteca

#include <Stepper.h>
#include <Servo.h>
#include <LiquidCrystal.h>

// Creación del objeto para control del servomotor
Servo servoMotor1;

// Para el Motor PP
const int stepsPerRevolution = 64;  // cambiar 64
Stepper miStepper(stepsPerRevolution,7,6,5,4);  //Declaramos las terminales de las bobinas del motor

// Para LCD
LiquidCrystal lcd (54,55,56,57,58,59); //RS,EN,D4,D5,D6,D7


// Motor CD #1  BANDA
const byte IN1Pin = 10;
const byte IN2Pin = 9;

// Motor CD #2  Mov Horizontal
const byte IN3Pin = 11;
const byte IN4Pin = 12;

// Entradas
const byte homeBtnPin = 18;    
const byte uppBtnPin = 19;     
const byte entBtnPin = 20;
boolean HOME = LOW;     
boolean UPP  = LOW;
boolean ENT = LOW;

const byte LFTPin  = 25;    
const byte RGTPin  = 26; 
boolean LFT  = LOW;     
boolean RGT  = LOW;

const byte PSSPin = 7;   //analog 7 y 61
int PSS = 0;

const byte DIP4 []={62,63,64,65};
const byte numDIP4 = 4;

// Salidas
const byte HOMEled= 24;
const byte EMGled = 23;
const byte PSled = 22;

byte  PWM1 = 255;
byte  PWM2 = 255;//128

// Constantes globales
const byte delayTime      = 100;
const byte servoDelayTime = 500;
boolean RGTF = LOW;
boolean LFTF = LOW;
byte value  = 0;
int pasos = 0;
const byte Deco[]{1,2,4,8};
int QR = 0;

bool FLAG = LOW;
bool DIR = LOW;

byte thisChar;

#include "FUNCIONES_PROYECTO2.h"


void setup()
{
  //Display
  //Pantalla LCD
   lcd.begin(16,2);            //16 caracteres por línea, 2 líneas
   lcd.cursor();
  
  // Definición de la terminal de control del servo
  servoMotor1.attach(13);
  servoMotor1.write (90);

  // Entradas activa baja
  pinMode (homeBtnPin, INPUT_PULLUP);
  pinMode (uppBtnPin, INPUT_PULLUP); 
  pinMode (entBtnPin, INPUT_PULLUP);
  pinMode (LFTPin,INPUT); 
  pinMode (RGTPin,INPUT); 
  for(int i = 0; i < numDIP4; i++)
    pinMode(DIP4[i],INPUT_PULLUP);

  //Salidas           
  pinMode(HOMEled,OUTPUT);       
  pinMode(EMGled ,OUTPUT);
  pinMode(PSled,OUTPUT);       
  pinMode(IN1Pin ,OUTPUT);
  pinMode(IN2Pin ,OUTPUT);
  pinMode(IN3Pin ,OUTPUT);
  pinMode(IN4Pin ,OUTPUT); 
  pinMode(49 ,OUTPUT); 

  attachInterrupt (digitalPinToInterrupt (homeBtnPin), homeBtnPress, FALLING);



   
}

void loop()
{
  Stop();
  delay(1000);

ETQ_CICLO: 
  OpenGripper();  
  delay(1000);  
  VerifyUpp();  // MPP upp 
  delay(1000);

ETQ_QR:  
  VerifyCD();   // DC motor RGT y DC motor LFT
  delay(1000);

  Home();
  delay(1000);
  
  ConveyorBelt();  
  VerifyPSS();
  delay(1000);

  CDmoveRGT();
  delay(1000);

  QR = Scanning();   // PSled     
  if(QR == 8)
  {
    LFTF = 1;
    RGTF = 0;
    goto ETQ_MPPdown;
    }
    else if(QR == 1)
    {
      LFTF = 0;
      RGTF = 1;
      goto ETQ_MPPdown;
      }
      else
      {
        digitalWrite(PSled,LOW);
        goto ETQ_QR;
        }

ETQ_MPPdown: 
  digitalWrite(PSled,LOW);     
  delay(1000);
  MPPdown();
  delay(1000);

  CloseGripper();
  delay(1000);

  VerifyUpp();
  delay(1000);

  FLAG = VerifyFlag();  // Función booleana
  RGTF = 0;
  LFTF = 0;
  if(FLAG == 0)
  {
    goto ETQ_RGT;
    }
    else
    {
      goto ETQ_LFT;
      } 

ETQ_RGT:
  CDlimiteRGT();
  goto ETQ_Delay;

ETQ_LFT:
  CDlimiteLFT();

ETQ_Delay:
  delay(1000);
  MPPdown();
  delay(1000);  
  goto ETQ_CICLO;
}

void homeBtnPress()
{
  HOME=!HOME;
  delay(50); 
}
