//CONTROLADOR DECODIFICADOR NÚMERO DE 8 BITS A 3 7 SEGMENTOS_JUAN JOSE JASSO GARDUNO

#include "PLD2020.h"

#define TiCLK 21//Entrada del reloj
#define TA0 22
#define TA1 23
#define TA2 24
#define TA3 25
#define TA4 26
#define TA5 27
#define TA6 28
#define TA7 29
#define TA8 52
#define TA9 51

#define ToCLK 49 //Salida del reloj configurada en 49
#define TUNI  54
#define TDCN  55
#define TCEN  56
#define TSMIL  58


//Variables globales, A8 Y A9 No pueden ser globales por la configuracion de arduino

//Señal presente
bool iCLK,UNI=0,DCN=0,CEN=0,SMIL=0;
//Señales futura
bool UNIt=0,DCNt=0,CENt=0,SMILt=0;

void setup() {
  pld_ini();
  pld_555(40);
}

void loop() {
  
  bool A0,A1,A2,A3,A4,A5,A6,A7,A8,A9; 
  
A0=digitalRead(TA0);
A1=digitalRead(TA1);
A2=digitalRead(TA2);
A3=digitalRead(TA3);
A4=digitalRead(TA4);
A5=digitalRead(TA5);
A6=digitalRead(TA6);
A7=digitalRead(TA7);
A8=digitalRead(TA8);
A9=digitalRead(TA9);



    //Sacamos la señal
  digitalWrite(ToCLK,CLK);
  //Lectura del reloj
  iCLK=digitalRead(TiCLK); //Leemos la terminal TiClk, o sea la 21

  //Parte secuencial

   if(!iCLK) //Cuando el reloj está en bajo leemos las señales...
  {
    UNIt = !DCN&!UNI;
    DCNt = !CEN&!DCN&UNI;
    CENt=  !CEN&DCN&!UNI;
    SMILt= A9&A8&A7&A6;
  }else
  {
    UNI= UNIt;
    DCN= DCNt;
    CEN= CENt;
    SMIL=SMILt;
    
    digitalWrite(TUNI,UNI);
    digitalWrite(TDCN,DCN);
    digitalWrite(TCEN,CEN);
    digitalWrite(TSMIL,SMIL);
     
  }
  
}
