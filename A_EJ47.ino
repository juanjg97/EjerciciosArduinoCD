//EJERCICIO 46

//****************************************************
//Variables globales
const byte VCpin = A0;
const byte numSsLeds = 7;
const byte numCntrlPins = 3;
const byte ssLedPins[] = {43,44,45,46,47,48,49};
const byte cntrlPins[] = {63,64,65};
const byte dischPin = 62;
const byte ledPin = 67;
const int  delayTime = 9;
const int  thrshldV = 647;

const boolean sevenSegOut[][7] = {{1,1,1,1,1,1,0}, // 0x7E
{0,1,1,0,0,0,0},  // 0x30
{1,1,0,1,1,0,1},  // 0x6D
{1,1,1,1,0,0,1},  // 0x79
{0,1,1,0,0,1,1},  // 0x33
{1,0,1,1,0,1,1},  // 0x5B
{1,0,1,1,1,1,1},  // 0x5F
{1,1,1,0,0,0,0},  // 0x70
{1,1,1,1,1,1,1},  // 0x7F
{1,1,1,1,0,1,1}}; // 0x7B

// Variable muda
byte thisPin;
//***************************************************************

void setup() {
  //Se configuran únicamente las terminales digitales
for (thisPin = 0; thisPin < numSsLeds; thisPin++)
  pinMode (ssLedPins[thisPin],OUTPUT);

for (thisPin = 0; thisPin < numCntrlPins; thisPin++)
  pinMode (cntrlPins[thisPin],OUTPUT);

pinMode (dischPin,OUTPUT);

}
//***************************************************************
void loop() {
  //Rutina que se estará realizando continuamente

  int x = 0;
  
  digitalWrite (dischPin, HIGH);
  delay (50);
  
  digitalWrite (dischPin, LOW);
  long initialTime = millis ();

  do{
  x = analogRead (VCpin);
  }while (x < thrshldV);
  
  long finalTime = millis ();
  
//***************************************************************
// Cálculo de la capacitancia
    int capacitance = finalTime - initialTime;
    
// Obtención de las centenas, decenas y unidades a desplegar
    byte hunds = capacitance/100;
    byte tens = (capacitance/10)%10;
    byte units = capacitance%10;
    
//***************************************************************  
  
  do{
  // Escritura de las UNIDADES en el despliegue de siete segmentos
    digitalWrite (cntrlPins[0],HIGH);
    digitalWrite (cntrlPins[1],LOW);
    digitalWrite (cntrlPins[2],LOW);
   
    for (thisPin = 0; thisPin < numSsLeds; thisPin++)
     {
     digitalWrite (ssLedPins[thisPin],sevenSegOut[units][thisPin]);
     }
     
     delay (delayTime);

  //***************************************************************    
  // Escritura de las decenas en el despliegue de siete segmentos
    digitalWrite (cntrlPins[0],LOW);
    digitalWrite (cntrlPins[1],HIGH);
    digitalWrite (cntrlPins[2],LOW);
   
    for (thisPin = 0; thisPin < numSsLeds; thisPin++)
     {
     digitalWrite (ssLedPins[thisPin],sevenSegOut[tens][thisPin]);
     }
     
     delay (delayTime);
 //*************************************************************** 
  // Escritura de las CENTENAS en el despliegue de siete segmentos
      digitalWrite (cntrlPins[0],LOW);
      digitalWrite (cntrlPins[1],LOW);
      digitalWrite (cntrlPins[2],HIGH);
   
    for (thisPin = 0; thisPin < numSsLeds; thisPin++)
     {
     digitalWrite (ssLedPins[thisPin],sevenSegOut[hunds][thisPin]);
     }
     
     delay (delayTime);
  
  
  }
  while (1);


}
