//SOLO SERVO


//Inlcuir archivos de biblioteca
#include <Stepper.h>
#include <Servo.h>
#include <LiquidCrystal.h>

// Creación del objeto para control del servomotor
Servo servoMotor1;

//Para el Motor PP
const int stepsPerRevolution = 64; 
Stepper miStepper(stepsPerRevolution,62,63,64,65);  //Declaramos las terminales de las bobinas del motor

//Para la pantalla LCD
LiquidCrystal lcd (56,57,58,59,60,61);    //RS,EN,D4,D5,D6,D7

// Entradas
const byte selBtnPin = 18;      //SEL
const byte goBtnPin = 20;      //GO
boolean SEL = LOW;     
boolean GO = LOW;

// Entradas dipswitch
const byte valSWpins[] = {49,48,47,46,45,44,43,42};
const byte numSWs = 8;

// Salidas
const byte ONled = 40;
const byte SELled = 41;

// Constantes globales
const byte delayTime = 100;
const byte servoDelayTime = 15;
byte value = 0;
byte num = 0;
byte angulo = 0;
byte numPP = 0;
byte numAbs = 0;
byte x = 0;
boolean pinVal;
boolean DIR = LOW;
boolean BitSigno = LOW;
boolean AngError = LOW;
byte LCDdelayTime = 650;
bool aux1;
bool aux2;

//Para la conversión
const byte Conv[]{1,2,4,8,16,32,64,128};

// Variable "muda"
byte thisPin;
byte thisChar;

void setup()
{
  // Definición de la terminal de control del servo
  servoMotor1.attach(13);
  servoMotor1.write(90);

  //Pantalla LCD
  lcd.begin(16,2);            //16 caracteres por línea, 2 líneas
  lcd.cursor();

  // Entradas activa baja
  pinMode (selBtnPin, INPUT_PULLUP);
  pinMode (goBtnPin, INPUT_PULLUP);  
  for (thisPin = 0; thisPin < numSWs; thisPin++)
    pinMode (valSWpins[thisPin], INPUT_PULLUP);

  //Salidas           
  pinMode(ONled,OUTPUT);       
  pinMode(SELled,OUTPUT);
  pinMode(16,OUTPUT);      

  // Configuración de las características de interrupción 
  attachInterrupt (digitalPinToInterrupt (selBtnPin), selBtnPress, FALLING);
  attachInterrupt (digitalPinToInterrupt (goBtnPin), goBtnPress, FALLING);  
}

void loop()
{

   
  writeLeds();
  checkGOandAngle();
  delay (delayTime);
}

//rutina de servicio de interrupción al oprimir SEL
void selBtnPress ()
{
  /*
  aux1=!digitalRead(selBtnPin);
  if(aux1)
  {
    do{
    aux1=!digitalRead(selBtnPin);
    }while(aux1);
  }*/
  SEL = !SEL;
  writeLeds();
  delay(delayTime);  
}

//rutina de servicio de interrupción al oprimir GO
void goBtnPress ()
{
  /*aux2=!digitalRead(goBtnPin);
  if(aux2)
  {
    do{
    aux2=!digitalRead(goBtnPin);
    }while(aux2);
  }
  */
  GO = !GO;
  x = 1;
  writeLeds();
  delay (delayTime);
}

//enciende/apaga los leds indicadores de SEL y GO
void writeLeds()        
{
  delay(100);
  digitalWrite(SELled,SEL);
  digitalWrite(ONled,GO);
}

// Función de Revisar GO y el ángulo
/*
Muestra el letrero de ajustar valor y oprimir GO

Muestra el letrero ANIMADO del motor seleccionado

Verifica el estado de la señal GO

Calcula el ángulo 

Manda mensaje de ángulo inválido

No permite avanzar mientras el ángulo sea erróneo (G = 0)
*/
void checkGOandAngle()
{  
  do{  
  showSignal1();  
  if(SEL == HIGH)
  {    
    CalculoAng();
    if(angulo > 90)
    {
      showSignal2();
      GO = LOW;
      digitalWrite(ONled,GO);
      AngError = HIGH;
    }
    else if(angulo <= 90)
    {
      AngError = LOW;    
    }    
  }   
  }while(GO == LOW || AngError == HIGH);

  if(GO == HIGH)
  {    
    moveMotor();    
  }
  
}  
    
// Calcula el valor del ángulo
void CalculoAng()
{
  // Conversión de la entrada del Dipswitch
  angulo = 0;
  for(thisPin = 0; thisPin < numSWs-1; thisPin++)
  {
    boolean pinVal = !digitalRead(valSWpins[thisPin]);     
    angulo += byte(pinVal)*Conv[thisPin];    
  }    
}

//si SEL = 0, mover motor PP 5´s
//si SEL = 1, mover servomotor
void moveMotor()        
{        
  lcd.clear();                 
  if(SEL == HIGH)
  { 
    showStrngServo();    
    delay(1000);
    moveServo();    
  }
  else
  {        
    // Manda a llamar el letrero FIJO en moveStepper();
    moveStepper();    
  }
  checkGO();
}

//rutina para mover el servomotor
void moveServo()
{  
      digitalWrite(62,LOW);
      digitalWrite(63,LOW);
      digitalWrite(64,LOW);
      digitalWrite(65,LOW);
  
  // Conversión de la entrada del Dipswitch
  num = 0;
  for(thisPin = 0; thisPin < numSWs-1; thisPin++)
  {
    boolean pinVal = !digitalRead(valSWpins[thisPin]);     
    num += byte(pinVal)*Conv[thisPin];    
    }    
  //bit Signo
  BitSigno = digitalRead(valSWpins[7]);     
  if (BitSigno)  
    value = 90 - num;  
  else  
    value = 90 + num;
  
  // Escritura de value para envío señal PWM
  servoMotor1.write (value);  
  delay (servoDelayTime);   
}

// Verificar la señal GO
void checkGO()          
{
   do
   {
     showSignal3();                
    }while(GO == HIGH);
}

void showSignal1 ()
{
 lcd.autoscroll ();
 char sign1[] = "                Set dipswitch, press GO ";
 byte sign1CharNum = 40;
 lcd.home ();
 for (thisChar = 0; thisChar < sign1CharNum; thisChar++)
 {
 lcd.write (sign1[thisChar]);
 delay (LCDdelayTime);
 }
 if(SEL == LOW)
 {
  showSgnStepper ();
 }
 else if (SEL == HIGH)
 {
  showSgnServo ();
 } 
}

void showSignal2 ()
{
 lcd.autoscroll ();
 char sign2[] = "                Angle must be 90 or less";
 byte sign2CharNum = 40;
 lcd.home ();
 for (thisChar = 0; thisChar < sign2CharNum; thisChar++)
 {
 lcd.write (sign2[thisChar]);
 delay (LCDdelayTime);
 }
  showSgnServo (); 
}

void showSignal3 ()
{
 lcd.autoscroll ();
 char sign3[] = "                Change function press GO";
 byte sign3CharNum = 40;
 lcd.home ();
 for (thisChar = 0; thisChar < sign3CharNum; thisChar++)
 {
 lcd.write (sign3[thisChar]);
 delay (LCDdelayTime);
 }
 if(SEL == LOW)
 {
  showSgnStepper ();
 }
 else if (SEL == HIGH)
 {
  showSgnServo ();
 } 
}

void showSgnStepper ()
{
 lcd.autoscroll ();
 char sgnStepper[] = "                Stepper motor";
 byte sgnStepperCharNum = 29;
 lcd.setCursor (0,1);
 for (thisChar = 0; thisChar < sgnStepperCharNum; thisChar++)
 {
 lcd.write (sgnStepper[thisChar]);
 delay (LCDdelayTime);
 }
}

void showSgnServo ()
{
 lcd.autoscroll ();
 char sgnServo[] = "                Servomotor   ";
 byte sgnServoCharNum = 30;
 lcd.setCursor (0,1);
 for (thisChar = 0; thisChar < sgnServoCharNum; thisChar++)
 {
 lcd.write (sgnServo[thisChar]);
 delay (LCDdelayTime);
 }
}

void showStrngStepper()
{
 lcd.noAutoscroll ();
 char stepperMot[] = " Stepper motor ";
 byte stepCharNum = 16;
 lcd.setCursor (0,1);
 // Escritura del letrero “ Stepper motor “ en el LCD
 for (thisChar = 0; thisChar < stepCharNum; thisChar++)
 lcd.write (stepperMot[thisChar]);
}

void showStrngServo()
{
 lcd.noAutoscroll ();
 char servoMot[] = "   Servomotor   ";
 byte servoCharNum = 16;
 lcd.setCursor (0,1);
 // Escritura del letrero “ Servomotor “ en el LCD
 for (thisChar = 0; thisChar < servoCharNum; thisChar++)
 lcd.write (servoMot[thisChar]);
}


void moveStepper ()
{  
   numAbs = 0;
   if((!SEL && GO)&& x==1)
    {
      NumDip();
      x = 0;
  
      digitalWrite(16,HIGH);
        
     if(numPP<128)
     {
        //Giro Levógiro
        DIR=1;
        numAbs=128-numPP;
      }
      if(numPP>128)
      {
        //Giro Dexógiro
        numAbs=numPP-128;
        DIR=0;
      }

      if(numAbs<4)
      {
        numAbs=numAbs+4;
      }

      long initialTime = millis();
      long elapsedTime = 0;
      long finalTime = millis();         
      int motorSpeed = map (numAbs, 0, 127, 0, 180); 
      miStepper.setSpeed (motorSpeed);

      showStrngStepper();       
      do{         
         if (DIR)
         (miStepper).step (-stepsPerRevolution/4); 
         else
         (miStepper).step (+stepsPerRevolution/4);
         finalTime = millis();
         elapsedTime = finalTime - initialTime;
       } while(elapsedTime < 5000);       
       digitalWrite(16,LOW);        
    }             
}

void NumDip()
{
   numPP = 0;
   for(thisPin = 0; thisPin < numSWs; thisPin++)
   {
     boolean pinVal = !digitalRead(valSWpins[thisPin]);     
     numPP += byte(pinVal)*Conv[thisPin];   
   }     
}
