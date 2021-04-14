//Controlador de puerta inteligente, JJJG
#include "PLD2020.h"

// Entradas

#define TSM    23
#define TPA    24
#define TPC    25
#define TBA    26
#define TBC    27
#define TBE    28
#define TC4    29
#define TiCLK  30

// Salidas
#define TQ3     40
#define TQ2     41
#define TQ1     42
#define TQ0     43
#define TX      44
#define TNX     45
#define Td0     46
#define Td1     47
#define Td2     48
#define Td3     49
#define TCR     55


// Configuración variables internas globales 
  bool Q0 = 0, Q1 = 0, Q2 = 0, Q3 = 0;
  bool Q0t = 0, Q1t = 0, Q2t = 0, Q3t = 0;

  bool d0 = 0, d1 = 0, d2 = 0, d3 = 0;
  bool d0t = 0, d1t = 0, d2t = 0, d3t = 0;

  bool X = 0, NX = 0;
  bool Xt = 0, NXt = 0;
  
  bool  iCLK,SM, PA, PC, BA, BC, BE, C4,CR;
 
  
// Función de configuración de la tarjeta
void setup()
{
// Configuración de pines
  pld_ini();

// Configuración de la frecuencia del pulso de reloj
  pld_555(32);  
}


void loop ()
{
// Configuración de variables internas secuenciales

// Lectura de entradas
  SM =   digitalRead(TSM);
  PA =   digitalRead(TPA);
  PC =   digitalRead(TPC);
  BA =   digitalRead(TBA);
  BC =   digitalRead(TBC);
  BE =   digitalRead(TBE);
  C4 =   digitalRead(TC4);

// Realimentación pulso del reloj principal
  iCLK = digitalRead(TiCLK);
  
// Salida expresiones combinacionales
   
   // Cálculo expresiones secuenciales reloj principal
  if (!iCLK)
  {
  // Expresiones lógicas del contador hexadecimal

    Q0t = Q2 | Q0&!SM  | !Q3&Q0 | !Q3&Q1&!BA | Q3&Q1&!Q0&(d3&!d2&!d1&d0);
    Q1t = !Q3& !Q0&BA | Q3& !Q1& !Q0&PA | Q3&Q1&Q0& !SM& !PC | Q1&!Q0&!SM | !Q3&Q1&!Q0  ;
    Q2t = Q3&Q2&!PC  |  !Q3&Q2 | !Q3&Q0&PA ;
    Q3t = Q2&BC | Q3&!BA|  Q3&Q2 | Q3&!Q0 | Q3&Q1 | Q1&Q0&PC ;

    d0t = !d1&d0&Q1 | !d3&d2&d0&Q1 | d3&!d2&d0&Q1 | !d1&C4&Q1| !d3&d2&C4&Q1| d3&!d2&C4&Q1 | d0&!C4&Q1; 
    d1t = !d3&d1&Q1 | d2&Q1 | d1&!d0&Q1 | !d1&d0&!C4&Q1|d3&C4&Q1;
    d2t = !d3&d2&Q1 | d2&d0&Q1 |!d3&d1&!d0&!C4&Q1 | d2&C4&Q1;
    d3t = d3&Q1 | d2&d0&!C4&Q1;

    Xt  = Q3&Q2 | Q3&Q1&Q0;
    NXt = !Q3&!Q2&!Q1&Q0 | Q3&!Q1&!Q0;

   if(BE==1)
   {
    Q0t=0;
    Q1t=0;
    Q2t=0;
    Q3t=0;
   }
  }
  else
  {
  // Memorización valores
    Q0  = Q0t;
    Q1  = Q1t;
    Q2  = Q2t;
    Q3  = Q3t;
   
    d0 = d0t;
    d1 = d1t;
    d2 = d2t;
    d3  =d3t;

    X  = Xt;
    NX = NXt;

  // Cálculo expresiones combinacionales 
    
     CR=!Q1;
     
  // Salida expresiones combinacionales
    digitalWrite(TCR, CR);
    
  // Salida valores secuenciales
    digitalWrite(TX, X);
    digitalWrite(TNX, NX);
    
    digitalWrite(TQ0, Q0);
    digitalWrite(TQ1, Q1);
    digitalWrite(TQ2, Q2);
    digitalWrite(TQ3, Q3);
    
    digitalWrite(Td0, d0);
    digitalWrite(Td1, d1);
    digitalWrite(Td2, d2);
    digitalWrite(Td3, d3);
  }
}
