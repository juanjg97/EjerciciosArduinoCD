//Contador decimal, JJJG

#include "PLD2020.h"

// Entradas
#define TX     22 //Dirección de giro, Si x=1, cuenta ascendente, combinacional

// Salidas para el display 7 segmentos
#define Ta     49
#define Tb     48
#define Tc     47
#define Td     46
#define Te     45
#define Tf     44
#define Tg     43


// Variables secuenciales, valores presentes y futuros
  bool Q0 = 0, Q1 = 0, Q2 = 0, Q3 = 0;
  bool Q0t = 0, Q1t = 0, Q2t = 0, Q3t = 0;

// Variables combinacionales
  bool  X,a,b,c,d,e,f,g;
 
  
// Configuración general
void setup()
{
  pld_ini();
  pld_555(5);  
}


void loop ()
{
// Lectura de entradas o sensores
  X =   digitalRead(TX);   
   
// Cálculo expresiones secuenciales con el reloj de arduino
if (!CLK)
 {
  // Variables secuenciales futuras, contador combinado con la expresión X
  Q0t= !Q0;
  Q1t= !X&Q1&Q0 | !X&Q2&!Q1&!Q0 | !X&Q3&!Q0 | X&!Q3&!Q1&Q0 | X&Q1&!Q0;
  Q2t= Q2&Q1&!Q0 | !X&Q2&Q0 | !X&Q3&!Q0 | X&!Q2&Q1&Q0 | X&Q2&!Q1;
  Q3t= !X&!Q3&!Q2&!Q1&!Q0 | !X&Q3&Q0 | X&Q3&!Q0 | X&Q2&Q1&Q0;
    
 }
else
 {
 
// Memorización valores, variables secuenciales presentes
    Q0  = Q0t;
    Q1  = Q1t;
    Q2  = Q2t;
    Q3  = Q3t;
    
//Sustituimos los bits del contador, en la expresión para transformar a BCD 7segmentos
//Variables combinacionales
    a = !(!Q3 & Q2 | Q2 & Q1)&!(Q0) | (!Q3 &  Q1 | Q3 & Q2 & !Q1) | (!Q3 & Q2 | Q2 & Q1)&(Q0)  | (Q3 & !Q2 & !Q1);
    b = !( !Q3 & Q2 | Q2 & Q1) | !( !Q3 &  Q1 | Q3 & Q2 & !Q1)&!( Q0) | (!Q3 &  Q1 | Q3 & Q2 & !Q1)&( Q0);
    c = !( !Q3 &  Q1 | Q3 & Q2 & !Q1) | (Q0) | (!Q3 & Q2 | Q2 & Q1);
    d = (Q3 & !Q2 & !Q1) | (!Q3 &  Q1 | Q3 & Q2 & !Q1)&!( Q0) | !( !Q3 & Q2 | Q2 & Q1)&( !Q3 &  Q1 | Q3 & Q2 & !Q1) | !( !Q3 & Q2 | Q2 & Q1)&!( Q0) | (!Q3 & Q2 | Q2 & Q1)&!( !Q3 &  Q1 | Q3 & Q2 & !Q1)&( Q0);
    e = !( !Q3 & Q2 | Q2 & Q1)&!( Q0) | (!Q3 &  Q1 | Q3 & Q2 & !Q1)&!( Q0);
    f = !( !Q3 &  Q1 | Q3 & Q2 & !Q1)&!( Q0) | (!Q3 & Q2 | Q2 & Q1)&!( !Q3 &  Q1 | Q3 & Q2 & !Q1) | (!Q3 & Q2 | Q2 & Q1)&!( Q0) | (Q3 & !Q2 & !Q1);
    g = !( !Q3 & Q2 | Q2 & Q1)&( !Q3 &  Q1 | Q3 & Q2 & !Q1) | (!Q3 & Q2 | Q2 & Q1)&!( !Q3 &  Q1 | Q3 & Q2 & !Q1) | (!Q3 & Q2 | Q2 & Q1)&!( Q0) | (Q3 & !Q2 & !Q1);
    
// Impresión de valores
    digitalWrite(Ta, a);
    digitalWrite(Tb, b);
    digitalWrite(Tc, c);
    digitalWrite(Td, d);
    digitalWrite(Te, e);
    digitalWrite(Tf, f);
    digitalWrite(Tg, g);
  }
}
