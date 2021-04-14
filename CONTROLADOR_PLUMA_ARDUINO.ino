//------------------------------------Controlador de la pluma
#include "PLD2020.h"

// Entradas
  #define TREC   20
  #define TINF   21      
  #define TSUP   22
  #define TPSO   23
   
// Salidas 
  #define TLED       49
  #define TENABLE    48
  #define TA         47
  #define TNA        46
  #define TB         45
  #define TNB        44
  #define TX         43
  #define TENC       42
  #define TPulsoCLK  41



// Variables secuenciales, valores presentes y futuros
  
  //Variables de estado del controlador
  bool Q0 = 0, Q1 = 0;
  bool Q0t = 0, Q1t = 0;

  //Variables para el motor PP
    bool A = 0, B = 0;
    bool At = 0, Bt = 0;

// Variables combinacionales
  
  //Variables para el motor pp
  bool NA=0,NB=0;
  
  //Variables de entrada
  bool REC=0,INF=0,SUP=0,PSO=0;
  
  //Variables de salida
  bool LED=0,ENABLE=0,ENC,X=0;
//---------------------------------------------------------- 
  
// Configuración general
void setup()
{
  pld_ini();
  pld_555(10);  
}

void loop ()
{
  digitalWrite(TPulsoCLK,CLK);
// Lectura de entradas o sensores
 
  REC =   digitalRead(TREC); 
  INF =   digitalRead(TINF); 
  SUP =   digitalRead(TSUP); 
  PSO =   digitalRead(TPSO);    
   
// Cálculo expresiones secuenciales con el reloj de arduino
if (!CLK)
 {
// Variables secuenciales futuras
  // Variables de estado del controlador 
  Q0t = !Q0&REC | Q0&!PSO;
  Q1t = Q1&Q0 | Q0&PSO | Q0&SUP | Q1&!INF&!REC;

  //Variables para el motor pp
  At = (!X&!B | X&B);
  Bt = (!X&A | X&!A);
 }
else
 { 
// Memorización valores, variables secuenciales presentes
    Q0  = Q0t;
    Q1  = Q1t;
    
    A = At ;
    B = Bt;
    
//Cálculo de variables combinacionales

  ENC    = !Q1&Q0 | Q1&!Q0;
  X      = Q1&!Q0;
  ENABLE = (Q1&!Q0)|(!Q1&Q0);
  LED    = !Q1&!Q0;  
  NA     = !A;
  NB     = !B;

    
// Impresión de valores
    digitalWrite(TENC, ENC);
    digitalWrite(TX, X);
    digitalWrite(TENABLE, ENABLE);
    digitalWrite(TLED, LED);
    digitalWrite(TA, A);
    digitalWrite(TNA, NA);
    digitalWrite(TB, B);
    digitalWrite(TNB, NB);
    
  }
}
