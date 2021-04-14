//CONTROLADOR DECODIFICADOR NÚMERO DE 8 BITS A 3 7 SEGMENTOS_JUAN JOSE JASSO GARDUNO

#include "PLD2020.h"

#define TiCLK 21//Entrada del rejo

#define ToCLK 49 //Salida del reloj configurada en 49
#define TUNI  54
#define TDCN  55
#define TCEN  56
#define TA8   58
#define TA9   59

//Variables globales, A8 Y A9 No pueden ser globales por la configuracion de arduino

//Señal presente
bool iCLK,UNI=0,DCN=0,CEN=0;
//Señales futura
bool UNIt=0,DCNt=0,CENt=0;

void setup() {
  pld_ini();
  pld_555(40);
}

void loop() {
  bool A8,A9; 
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
    
  }else
  {
    UNI= UNIt;
    DCN= DCNt;
    CEN= CENt;

    A8 = DCN;
    A9 = CEN;
    
    digitalWrite(TUNI,UNI);
    digitalWrite(TDCN,DCN);
    digitalWrite(TCEN,CEN);
    digitalWrite(TA8,A8);
    digitalWrite(TA9,A9);   
  }
  
}
