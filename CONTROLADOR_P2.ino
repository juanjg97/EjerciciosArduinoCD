//Controlador de puerta inteligente, JJJG / GCLE
#include "PLD2020.h"
// Entradas
//Botón
#define TSM    54

//Opto
#define TPA    55
#define TPC    56

//Botones físicos
#define TBA    57
#define TBC    58

// Salidas
#define TiCRO   48 // Salida del cronómetro

#define TaD 22
#define TbD 23
#define TcD 24
#define TdD 25
#define TeD 26
#define TfD 27
#define TgD 28

#define TaQ 31
#define TbQ 32
#define TcQ 33
#define TdQ 34
#define TeQ 35
#define TfQ 36
#define TgQ 37

#define TdecQ 38

#define TX      4 //41
#define TNX     5 //42

#define TLED    46



// Configuración variables internas globales, valores presentes, futuros (salidas) y las entradas
  bool Q0 = 0, Q1 = 0, Q2 = 0, Q3 = 0;
  bool Q0t = 0, Q1t = 0, Q2t = 0, Q3t = 0;
  bool aQ=0,bQ=0,cQ=0,dQ=0,eQ=0,fQ=0,gQ=0,decQ=0; 
//------------------------------------------------------------
//DECLARAMOS LAS VARIABLES PARA EL CRONÓMETRO
  bool C0 = 0, C1 = 0, C2 = 0, C3 = 0, C4 = 0;
  bool C0t = 0, C1t = 0, C2t = 0, C3t = 0, C4t = 0;
  bool d0 = 0, d1 = 0, d2 = 0, d3 = 0;
  bool d0t = 0, d1t = 0, d2t = 0, d3t = 0;
//------------------------------------------------------------
    //DECLARAMOS LAS VARIABLES PARA LAS LAS ENTRADAS AL DISPLAY
       bool Z0 = 0, Z1 = 0, Z2 = 0, Z3 = 0, Z4 = 0;
       bool Z0t = 0, Z1t = 0, Z2t = 0, Z3t = 0, Z4t = 0;     
       bool aD=0,bD=0,cD=0,dD=0,eD=0,fD=0,gD=0; 
      
//------------------------------------------------------------
//DECLARAMOS LAS VARIABLES PARA OTRAS SALIDAS
  bool X = 0, NX = 0;
  bool Xt = 0, NXt = 0;
  bool LED = 0, LEDt = 0;
//------------------------------------------------------------
//DECLARAMOS LAS ENTRADAS Y iCRO, éste nos ayuda a saber la frecuencia del reloj del cronómetro
  bool SM, PA, PC, BA, BC, BE, iCRO=0; 
 
void setup()
{
  pld_ini();
  pld_555(4); //Frecuencia de32 Hz
}


void loop ()
{
  SM =   digitalRead(TSM);
  PA =   digitalRead(TPA);
  PC =   digitalRead(TPC);
  BA =   digitalRead(TBA);
  BC =   digitalRead(TBC);
  
  if (!CLK)
  {
 //Expresiones lógicas del contador hexadecimal (Estados del controlador de la puerta)
    Q0t = Q2 | Q0&!SM  | !Q3&Q0 | !Q3&Q1&!BA | Q3&Q1&!Q0&(d3&!d2&!d1&d0);
    Q1t = !Q3& !Q0&BA | Q3& !Q1& !Q0&PA | Q3&Q1&Q0& !SM& !PC | Q1&!Q0&!SM | !Q3&Q1&!Q0  ;
    Q2t = Q3&Q2&!PC  |  !Q3&Q2 | !Q3&Q0&PA ;
    Q3t = Q2&BC | Q3&!BA|  Q3&Q2 | Q3&!Q0 | Q3&Q1 | Q1&Q0&PC ;
 //-----------------------------------------------------------------
 //Contador DE 5 BITS para el cronómetro lograr frecuencia de 1Hz en el cronómetro
    C0t = !C0;  
    C1t = C1&!C0 | !C1&C0; 
    C2t = C2&!C1 | C2&!C0 | !C2&C1&C0;  
    C3t = C3&!C2 | C3&!C1 | C3&!C0| !C3&C2&C1&C0;   
    C4t = C4&!C3 | C4&!C2 | C4&!C1| C4&!C0|!C4&C3&C2&C1&C0;
     
// Expresiones para la salida del cronómetro (Estados del cronómetro)
    d0t = !d1&d0&Q1 | !d3&d2&d0&Q1 | d3&!d2&d0&Q1 | !d1&C4&Q1| !d3&d2&C4&Q1| d3&!d2&C4&Q1 | d0&!C4&Q1; 
    d1t = !d3&d1&Q1 | d2&Q1 | d1&!d0&Q1 | !d1&d0&!C4&Q1|d3&C4&Q1;
    d2t = !d3&d2&Q1 | d2&d0&Q1 |!d3&d1&!d0&!C4&Q1 | d2&C4&Q1;
    d3t = d3&Q1 | d2&d0&!C4&Q1;

 // Otras salidas  
    Xt  = Q3&Q2 | Q3&Q1&Q0;
    NXt = !Q3&!Q2&!Q1&Q0 | Q3&!Q1&!Q0;
    decQ = Q3&Q2 | Q3&Q1;
    LEDt = (!Q3&!Q2&!Q1&!Q0)&(C4);
  }
  else
  { 
    C0 = C0t;  
    C1 = C1t;
    C2 = C2t; 
    C3 = C3t;   
    C4 = C4t;
    iCRO=C4;
 //----------------------------------------------------------------------------   
    Q0  = Q0t;
    Q1  = Q1t;
    Q2  = Q2t;
    Q3  = Q3t;  
    //Salidas al display de estados
    aQ = !(!Q3 & Q2 | Q2 & Q1)&!(Q0) | (!Q3 &  Q1 | Q3 & Q2 & !Q1) | (!Q3 & Q2 | Q2 & Q1)&(Q0)  | (Q3 & !Q2 & !Q1);
    bQ = !(!Q3 & Q2 | Q2 & Q1) | !( !Q3 &  Q1 | Q3 & Q2 & !Q1)&!( Q0) | (!Q3 &  Q1 | Q3 & Q2 & !Q1)&( Q0);
    cQ = !(!Q3 &  Q1 | Q3 & Q2 & !Q1) | (Q0) | (!Q3 & Q2 | Q2 & Q1);
    dQ = (Q3 & !Q2 & !Q1) | (!Q3 &  Q1 | Q3 & Q2 & !Q1)&!( Q0) | !( !Q3 & Q2 | Q2 & Q1)&( !Q3 &  Q1 | Q3 & Q2 & !Q1) | !( !Q3 & Q2 | Q2 & Q1)&!( Q0) | (!Q3 & Q2 | Q2 & Q1)&!( !Q3 &  Q1 | Q3 & Q2 & !Q1)&( Q0);
    eQ = !(!Q3 & Q2 | Q2 & Q1)&!( Q0) | (!Q3 &  Q1 | Q3 & Q2 & !Q1)&!( Q0);
    fQ = !(!Q3 &  Q1 | Q3 & Q2 & !Q1)&!( Q0) | (!Q3 & Q2 | Q2 & Q1)&!( !Q3 &  Q1 | Q3 & Q2 & !Q1) | (!Q3 & Q2 | Q2 & Q1)&!( Q0) | (Q3 & !Q2 & !Q1);
    gQ = !(!Q3 & Q2 | Q2 & Q1)&( !Q3 &  Q1 | Q3 & Q2 & !Q1) | (!Q3 & Q2 | Q2 & Q1)&!( !Q3 &  Q1 | Q3 & Q2 & !Q1) | (!Q3 & Q2 | Q2 & Q1)&!( Q0) | (Q3 & !Q2 & !Q1);
//----------------------------------------------------------------------------
    //Estados del cronómetro
    d0 = d0t;
    d1 = d1t;
    d2 = d2t;
    d3  =d3t;
    //Dirección de giro del motor
    X  = Xt;
    NX = NXt;  
    LED = LEDt;

    digitalWrite(TiCRO, iCRO);//Sacamos la señal del cronómetro para medir su frecuencia en Proteus.
 
    digitalWrite(TdecQ, decQ);
    digitalWrite(TX, X);
    digitalWrite(TNX, NX);
    digitalWrite(TLED, LED);

    digitalWrite(TaQ, aQ);
    digitalWrite(TbQ, bQ);
    digitalWrite(TcQ, cQ);
    digitalWrite(TdQ, dQ);
    digitalWrite(TeQ, eQ);
    digitalWrite(TfQ, fQ);
    digitalWrite(TgQ, gQ);      
  }

//----------------------------------------------------------------------------  
//Contador para el display del cronómetro (Cuenta de 5 seg.) 
    if(!C4)
 {
    //Contador DE 4 BITS para la salida del display del cronómetro, puede contar hasta el 9, pero lo limitamos a 5
    Z0t = !Z0;  
    Z1t = Z1&!Z0 | !Z1&Z0; 
    Z2t = Z2&!Z1 | Z2&!Z0 | !Z2&Z1&Z0;  
    Z3t = Z3&!Z2 | Z3&!Z1 | Z3&!Z0| !Z3&Z2&Z1&Z0; 
  }
else
{ 
    if(Q3&!Q2&Q1&!Q0) //Sólo mostrará la cuenta mientras esté la señal T5
    {
    Z0 = Z0t;  
    Z1 = Z1t; 
    Z2 = Z2t;  
    Z3 = Z3t;

    aD = !(!Z3 & Z2 | Z2 & Z1)&!(Z0) | (!Z3 &  Z1 | Z3 & Z2 & !Z1) | (!Z3 & Z2 | Z2 & Z1)&(Z0)  | (Z3 & !Z2 & !Z1);
    bD = !(!Z3 & Z2 | Z2 & Z1) | !( !Z3 &  Z1 | Z3 & Z2 & !Z1)&!( Z0) | (!Z3 &  Z1 | Z3 & Z2 & !Z1)&( Z0);
    cD = !(!Z3 &  Z1 | Z3 & Z2 & !Z1) | (Z0) | (!Z3 & Z2 | Z2 & Z1);
    dD = (Z3 & !Z2 & !Z1) | (!Z3 &  Z1 | Z3 & Z2 & !Z1)&!( Z0) | !( !Z3 & Z2 | Z2 & Z1)&( !Z3 &  Z1 | Z3 & Z2 & !Z1) | !( !Z3 & Z2 | Z2 & Z1)&!( Z0) | (!Z3 & Z2 | Z2 & Z1)&!( !Z3 &  Z1 | Z3 & Z2 & !Z1)&( Z0);
    eD = !(!Z3 & Z2 | Z2 & Z1)&!( Z0) | (!Z3 &  Z1 | Z3 & Z2 & !Z1)&!( Z0);
    fD = !(!Z3 &  Z1 | Z3 & Z2 & !Z1)&!( Z0) | (!Z3 & Z2 | Z2 & Z1)&!( !Z3 &  Z1 | Z3 & Z2 & !Z1) | (!Z3 & Z2 | Z2 & Z1)&!( Z0) | (Z3 & !Z2 & !Z1);
    gD = !(!Z3 & Z2 | Z2 & Z1)&( !Z3 &  Z1 | Z3 & Z2 & !Z1) | (!Z3 & Z2 | Z2 & Z1)&!( !Z3 &  Z1 | Z3 & Z2 & !Z1) | (!Z3 & Z2 | Z2 & Z1)&!( Z0) | (Z3 & !Z2 & !Z1);
    
    digitalWrite(TaD, aD);
    digitalWrite(TbD, bD);
    digitalWrite(TcD, cD);
    digitalWrite(TdD, dD);
    digitalWrite(TeD, eD);
    digitalWrite(TfD, fD);
    digitalWrite(TgD, gD);
      }else
        { //Limpiamos los valores para que la cuenta se muestre hasta 5 otra vez
          Z0 = 0;  
          Z1 = 0; 
          Z2 = 0;  
          Z3 = 0;
        }
  }
  
  
}
