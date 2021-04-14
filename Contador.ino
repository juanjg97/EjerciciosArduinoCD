//PROGRAMA CON CONTADOR Y DECODIFICADOR PARA ANIMACIÓN
#include "PLD2020.h"
//Entradas

//Salidas
//    CONTADOR

#define TQ0 54
#define TQ1 55
#define TQ2 56
//    DECODIFICADOR
#define TM0 58
#define TM1 59
#define TM2 60
#define TM3 61
#define TM4 62
#define TM5 63
#define TM6 64
#define TM7 65

 //Q3 sin salida pero sí se calcula
#define TQ4 66
#define TQ5 67
#define TQ6 68
#define TQ7 69

//contador
bool  Q0 = 0, Q1 = 0, Q2 = 0, Q3=0, Q4=0, Q5=0, Q6=0, Q7=0;
//contador futuro
bool Q0t = 0, Q1t = 0, Q2t = 0, Q3t=0, Q4t=0, Q5t=0, Q6t=0, Q7t=0;
//columnas
bool M0, M1, M2 ,M3, M4, M5, M6, M7;

void setup() {
  pld_ini();    //
  pld_555(200); //Frecuencia del reloj, un uno real debe ser 200

}

void loop() {

 if(!CLK)
{
 //Expresiones para un contador de 3 bits, revisar video para explicación
 Q0t = !Q0;
 Q1t = Q1&!Q0 | !Q1&Q0;
 Q2t = Q2&!Q1 | Q2&!Q0 | !Q2&Q1&Q0;
 Q3t = Q3&!Q2 | Q3&!Q1 | Q3&!Q0| !Q3&Q2&Q1&Q0;
 Q4t = Q4&!Q3 | Q4&!Q2 | Q4&!Q1| Q4&!Q0|!Q4&Q3&Q2&Q1&Q0;
 Q5t = Q5&!Q4 | Q5&!Q3 | Q5&!Q2| Q5&!Q1| Q5&!Q0|!Q5&Q4&Q3&Q2&Q1&Q0;
 Q6t = Q6&!Q5 | Q6&!Q4 | Q6&!Q3| Q6&!Q2| Q6&!Q1| Q6&!Q0|!Q6&Q5&Q4&Q3&Q2&Q1&Q0;
 Q7t = Q7&!Q6 | Q7&!Q5 | Q7&!Q4| Q7&!Q3| Q7&!Q2| Q7&!Q1|Q7&!Q0| !Q7&Q6&Q5&Q4&Q3&Q2&Q1&Q0;
  
}else
{
  Q0 = Q0t; 
  Q1 = Q1t;
  Q2 = Q2t;
  Q3 = Q3t; 
  Q4 = Q4t;
  Q5 = Q5t;
  Q6 = Q6t;
  Q7 = Q7t;

  M0 = !Q2&!Q1&!Q0   ;
  M1 = !Q2&!Q1& Q0   ;
  M2 = !Q2& Q1&!Q0   ;
  M3 = !Q2& Q1& Q0   ;
  M4 =  Q2&!Q1&!Q0   ;
  M5 =  Q2&!Q1& Q0   ;
  M6 =  Q2& Q1&!Q0   ;
  M7 =  Q2& Q1& Q0   ;

   digitalWrite(TQ0,Q0);
   digitalWrite(TQ1,Q1);
   digitalWrite(TQ2,Q2);
   
   digitalWrite(TQ4,Q4);
   digitalWrite(TQ5,Q5);
   digitalWrite(TQ6,Q6);
   digitalWrite(TQ7,Q7);
  
  digitalWrite(TM0, M0);
  digitalWrite(TM1, M1);
  digitalWrite(TM2, M2);
  digitalWrite(TM3, M3);
  digitalWrite(TM4, M4);
  digitalWrite(TM5, M5);
  digitalWrite(TM6, M6);
  digitalWrite(TM7, M7);
  
  }


}
