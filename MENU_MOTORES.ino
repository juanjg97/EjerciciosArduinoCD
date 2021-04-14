//Controlador de puerta inteligente, JJJG
#include "PLD2020.h"

// Entradas

#define TiCLK_AS   23
#define TM         24
#define TS         25
#define TR         26


// Salidas
#define ToCLK_AS   40
#define TO0        41
#define TO1        42
#define TO2        43


#define TpCLK      44


// variables secuenciales, valores presentes, futuros. Salidas 
  bool O0 = 0, O1 = 0, O2 = 0, O3 = 0;
  bool O0t = 0, O1t = 0, O2t = 0, O3t = 0;

// Variables combinacionales, entradas
  bool oCLK_AS=0, iCLK_AS=0, M=0, S=0, R=0;
  
// -------------------------------------------------------
void setup()
{
  pld_ini();
  pld_555(32);  
}


void loop ()
{
digitalWrite(TpCLK,CLK);

// Lectura entradas o sensores
 
  M =   digitalRead(TM);
  S =   digitalRead(TS);
  R =   digitalRead(TR);

// Expresión para generar el pulso del reloj asíncrono e impresión del pulso (verdadero o falso)
   oCLK_AS = M | S | R ;
   digitalWrite(ToCLK_AS, oCLK_AS);
   
//Lectura del pulso para el reloj asíncrono
  iCLK_AS  = digitalRead(TiCLK_AS );



   
// Cálculo expresiones secuenciales con base en el reloj asíncrono
  if (!iCLK_AS)
  {
// Cálculo de variables secuenciales
  //Variables de estado (futuro) del controlador del menú
    O0t  = O2 | !O0&M | !O1&O0&!R | O1&!M;
    O1t  = O2&O1 | !O2&!O1&O0&M | O1&!M&!R;
    O2t  = S&!O2&O0 | !R&O2;
  }
  else
  {
// Memorización de variables secuenciales (presentes)
    O0  = O0t;
    O1  = O1t;
    O2  = O2t;
     
// Impresión de valores secuenciales
    
    digitalWrite(TO0, O0);
    digitalWrite(TO1, O1); 
    digitalWrite(TO2, O2);

  }

}
