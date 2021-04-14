//SOLO SERVO

//Inlcuir archivos de biblioteca
#include <Stepper.h>
#include <Servo.h>


// Creación del objeto para control del servomotor
Servo servoMotor1;

//Para el Motor PP
const int stepsPerRevolution = 64; //cambiar a 64
Stepper miStepper(stepsPerRevolution,62,63,64,65);  //Declaramos las terminales de las bobinas del motor


// Entradas
const byte homeBtnPin = 20;    
const byte uppBtnPin  = 21;     
boolean HOME = LOW;     
boolean UPP  = LOW;

const byte LFTPin  = 25;    
const byte RGTPin  = 26; 
boolean LFT  = LOW;     
boolean RGT  = LOW;

// Salidas
const byte HOMEled= 23;
const byte EMGled = 22;
//const byte MPPled = 24;

bool   MPP=LOW;
byte   PWM=128;

const byte IN1Pin = 11;
const byte IN2Pin = 12;

// Constantes globales
const byte delayTime      = 100;
const byte servoDelayTime = 500;
byte value  = 0;
boolean DIR = LOW;




void setup()
{
  // Definición de la terminal de control del servo
  servoMotor1.attach(13);
  servoMotor1.write (90);



  // Entradas activa baja
  pinMode (homeBtnPin, INPUT_PULLUP);
  pinMode (uppBtnPin, INPUT_PULLUP); 
  pinMode (LFTPin,INPUT); 
  pinMode (RGTPin,INPUT); 


  //Salidas           
  pinMode(HOMEled,OUTPUT);       
  pinMode(EMGled ,OUTPUT);
         
  pinMode(IN1Pin ,OUTPUT);
  pinMode(IN2Pin ,OUTPUT); 

attachInterrupt (digitalPinToInterrupt (homeBtnPin), homeBtnPress, FALLING);
   
}

void loop()
{
  EMGLED();

  delay(1000);
  moveServo();
  
  delay(1000);
  revisaUpp();
  
  delay(1000);
  moveCD();
  
}


void homeBtnPress()
{
  HOME=!HOME;
  delay(50); 
}



    
void EMGLED()
{
  digitalWrite(HOMEled,LOW);
   do
   {
     digitalWrite(EMGled,HIGH);
     delay(500);
     digitalWrite(EMGled,LOW);
     delay(500);
   }while(!HOME);

  digitalWrite(EMGled,LOW);
}





//rutina para mover el servomotor
void moveServo()
{  
      digitalWrite(62,LOW);
      digitalWrite(63,LOW);
      digitalWrite(64,LOW);
      digitalWrite(65,LOW);

     analogWrite(IN1Pin,0);
     analogWrite(IN2Pin,0);
       
  delay (servoDelayTime); 
  // Escritura de value para envío señal PWM
  servoMotor1.write (180);  
  delay (servoDelayTime);   
}



void revisaUpp ()
{  
      UPP=!digitalRead(uppBtnPin);
      if(UPP==1)
      {
        
      }else if(UPP==0)
      {
        analogWrite(IN1Pin,0);
        analogWrite(IN2Pin,0);
         int motorSpeed = 180; 
         miStepper.setSpeed (motorSpeed);
           
       do{   
    
         (miStepper).step (stepsPerRevolution/4);
          UPP=!digitalRead(uppBtnPin);
         }while(UPP==0);
      
      }     
                 
}




void moveCD()
{
      analogWrite(13,0);
      digitalWrite(62,LOW);
      digitalWrite(63,LOW);
      digitalWrite(64,LOW);
      digitalWrite(65,LOW);
      
 LFT=digitalRead(LFTPin);
 RGT=digitalRead(RGTPin);

 if(!RGT&&!LFT)
 {

  do
  {
  analogWrite(IN1Pin,PWM);
  analogWrite(IN2Pin,0);
  
   RGT=digitalRead(RGTPin);
  }while(!RGT);
  delay(400);
  analogWrite(IN1Pin,0);
  analogWrite(IN2Pin,0);
  delay(1000);
    
  do
   {
    analogWrite(IN1Pin,0);
    analogWrite(IN2Pin,PWM);
   
   LFT=digitalRead(LFTPin);
   }while(!LFT);
   HOMELed2();
   
 }else if(RGT)
 {
  delay(1000);
  //digitalWrite(IN1Pin,LOW);
  //digitalWrite(IN2Pin,HIGH);
  do
  {
    analogWrite(IN1Pin,0);
    analogWrite(IN2Pin,PWM);
   
   LFT=digitalRead(LFTPin);
  }while(!LFT);
  HOMELed2();
  
  
 }else if(LFT)
 {
  HOMELed2();
  
 }
 
  
}



void HOMELed()
  {
  do
  {
  digitalWrite(HOMEled,HIGH);
  analogWrite(IN1Pin,0);
  analogWrite(IN2Pin,0);

  }while(HOME==1);
 
  }

  void HOMELed2()
  {
  do
  {
  digitalWrite(HOMEled,HIGH);
  analogWrite(IN1Pin,0);
  analogWrite(IN2Pin,0);
  }while(true);
 
  }
