// CONTROLADOR DE UNA PUERTA INTELIGENTE
//SOLO CONTADOR DE 0 A 5 [S]

#include "PLD2020.h"

// Entradas
#define TSM   54   //conservan mismo número en físico
#define TPA   55   
#define TPC   56   
#define TBA   57   
#define TBC   58

// Salidas
#define TaD 22  //conservan mismo nombre y número    
#define TbD 23
#define TcD 24    
#define TdD 25    
#define TeD 26    
#define TfD 27    
#define TgD 28    

#define TaQ 31  //A1 en el arduino MEGA    
#define TbQ 32  //A2  
#define TcQ 33  //A3  
#define TdQ 34  //A4 
#define TeQ 35  //A5  
#define TfQ 36  //A6  
#define TgQ 37  //A7  

#define TdecQ 38  //A0

#define TX   4  //A9   
#define TNX  5  //A10  
#define TLED 46 //A8


// Configuración variables internas globales, valores presentes, futuros (salidas) y las entradas
  bool Q0 = 0, Q1 = 0, Q2 = 0, Q3 = 0;
  bool Q0t = 0, Q1t = 0, Q2t = 0, Q3t = 0;
  bool aQ=0, bQ=0, cQ=0, dQ=0, eQ=0, fQ=0, gQ=0, decQ=0; 
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
  bool aD = 0, bD = 0, cD = 0, dD = 0, eD = 0, fD = 0, gD = 0;      
//------------------------------------------------------------
//DECLARAMOS LAS VARIABLES PARA OTRAS SALIDAS
  bool X = 0, NX = 0;
  bool Xt = 0, NXt = 0;
  bool LED = 0, LEDt = 0;
//------------------------------------------------------------
//DECLARAMOS LAS ENTRADAS Y iCRO, éste nos ayuda a saber la frecuencia del reloj del cronómetro
  bool SM, PA, PC, BA, BC, BE, RC,RST;
//------------------------------------------------------------
//CONVERTIDOR CODIGO GRAY A BINARIO
  bool G0, G1, G2, G3, L0, L1, L2, L3;
  bool G0t, G1t, G2t, G3t, L0t, L1t,L2t,L3t;
  
void setup()
{
  pld_ini();
  pld_555(4);
}

// PROGRAMA PRINCIPAL
void loop()
{  
// Lectura de entradas
  
  SM =   digitalRead(TSM);
  PA =   digitalRead(TPA);
  PC =   digitalRead(TPC);
  BA =   digitalRead(TBA);
  BC =   digitalRead(TBC);  


// Cálculo expresiones secuenciales reloJ asíncrono


  if (!CLK)
  {
 //Expresiones lógicas del contador hexadecimal (Estados del controlador de la puerta)
    Q0t = Q2 | Q0&!SM  | !Q3&Q0 | !Q3&Q1&!BA | Q3&Q1&!Q0&(d3&!d2&!d1&!d0);
    Q1t = !Q3&!Q0&BA | Q3&!Q1&!Q0&PA | Q3&Q1&Q0&!SM&!PC | Q1&!Q0&!SM | !Q3&Q1&!Q0;
    Q2t = Q3&Q2&!PC  |  !Q3&Q2 | !Q3&Q0&PA ;
    Q3t = Q2&BC | Q3&!BA|  Q3&Q2 | Q3&!Q0 | Q3&Q1 | Q1&Q0&PC ;
 //-----------------------------------------------------------------
 //Contador DE 5 BITS para el cronómetro lograr frecuencia de 1Hz en el cronómetro
    C0t = !C0;  
    C1t = C1&!C0 | !C1&C0; 
    C2t = C2&!C1 | C2&!C0 | !C2&C1&C0;  
    C3t = C3&!C2 | C3&!C1 | C3&!C0| !C3&C2&C1&C0;   
    C4t = C4&!C3 | C4&!C2 | C4&!C1| C4&!C0|!C4&C3&C2&C1&C0;     
 //-----------------------------------------------------------------
 // Expresiones para la salida del cronómetro (Estados del cronómetro) AQUI VAN LAS d

    RST=!(Q3&!Q2&!Q1&!Q0);//---> Verificamos que no se encuentra en el estado 8
 
    d0t = (!d3&!d1&d0&Q1 | !d3&d2&d0&Q1 | d3&!d2&d1&d0&Q1 | !d1&!d0&C4&Q1 | !d3&d2&C4&Q1 | d3&!d2&d1&!d0&C4&Q1 | d0&!C4&Q1)&RST;
    d1t = (!d3&d1&Q1 | d2&Q1 | d1&!d0&Q1 | !d3&d0&!C4&Q1 | d1&C4&Q1)&RST;
    d2t = (!d3&d2&Q1 | d2&d0&Q1 | !d3&d1&!d0&!C4&Q1 | d2&C4&Q1)&RST;
    d3t = (d3&Q1 | d2&d0&!C4&Q1)&RST;

    Z0t = (!d3&!d2&d1 | d3&d2 | d3&!d1);
    Z1t = (d2);
    Z2t = (d3&!d2); 
 //-----------------------------------------------------------------
 // Otras salidas  
    Xt  = Q3&Q2 | Q3&Q1&Q0;
    NXt = !Q3&!Q2&!Q1&Q0 | Q3&!Q1&!Q0;
    
    decQ = Q3&Q2 | Q3&Q1;
    
    LEDt = (!Q3&!Q2&!Q1&!Q0)&(C4);
     
  }
  
  else
  {    
 
//----------------------------------------------------------------- 
//Salidas del divisor de frecuencia
    C0 = C0t;  
    C1 = C1t;
    C2 = C2t; 
    C3 = C3t;   
    C4 = C4t;    
   
//----------------------------------------------------------------------------
//Estados del cronómetro AQUI VAN LAS d
    d0 = d0t;
    d1 = d1t;
    d2 = d2t;
    d3  =d3t;

//-----------------------------------------------------------------
//Dirección de giro del motor, led.
    X  = Xt;
    NX = NXt;      
    LED = LEDt;
    RC = !Q1;
    
  
    digitalWrite(TX, X);
    digitalWrite(TNX, NX);
    
    digitalWrite(TLED, LED);
    
    digitalWrite(TdecQ, decQ);
//----------------------------------------------------------------------------
//Salidas al display de estados   
    Q0  = Q0t;
    Q1  = Q1t;
    Q2  = Q2t;
    Q3  = Q3t;  
    
    aQ = !(!Q3 & Q2 | Q2 & Q1)&!(Q0) | (!Q3 &  Q1 | Q3 & Q2 & !Q1) | (!Q3 & Q2 | Q2 & Q1)&(Q0)  | (Q3 & !Q2 & !Q1);
    bQ = !(!Q3 & Q2 | Q2 & Q1) | !( !Q3 &  Q1 | Q3 & Q2 & !Q1)&!( Q0) | (!Q3 &  Q1 | Q3 & Q2 & !Q1)&( Q0);
    cQ = !(!Q3 &  Q1 | Q3 & Q2 & !Q1) | (Q0) | (!Q3 & Q2 | Q2 & Q1);
    dQ = (Q3 & !Q2 & !Q1) | (!Q3 &  Q1 | Q3 & Q2 & !Q1)&!( Q0) | !( !Q3 & Q2 | Q2 & Q1)&( !Q3 &  Q1 | Q3 & Q2 & !Q1) | !( !Q3 & Q2 | Q2 & Q1)&!( Q0) | (!Q3 & Q2 | Q2 & Q1)&!( !Q3 &  Q1 | Q3 & Q2 & !Q1)&( Q0);
    eQ = !(!Q3 & Q2 | Q2 & Q1)&!( Q0) | (!Q3 &  Q1 | Q3 & Q2 & !Q1)&!( Q0);
    fQ = !(!Q3 &  Q1 | Q3 & Q2 & !Q1)&!( Q0) | (!Q3 & Q2 | Q2 & Q1)&!( !Q3 &  Q1 | Q3 & Q2 & !Q1) | (!Q3 & Q2 | Q2 & Q1)&!( Q0) | (Q3 & !Q2 & !Q1);
    gQ = !(!Q3 & Q2 | Q2 & Q1)&( !Q3 &  Q1 | Q3 & Q2 & !Q1) | (!Q3 & Q2 | Q2 & Q1)&!( !Q3 &  Q1 | Q3 & Q2 & !Q1) | (!Q3 & Q2 | Q2 & Q1)&!( Q0) | (Q3 & !Q2 & !Q1);
       
    digitalWrite(TaQ, aQ);
    digitalWrite(TbQ, bQ);
    digitalWrite(TcQ, cQ);
    digitalWrite(TdQ, dQ);
    digitalWrite(TeQ, eQ);
    digitalWrite(TfQ, fQ);
    digitalWrite(TgQ, gQ);           
//-----------------------------------------------------------------
//Salidas para el display del cronómetro (Cuenta de 5 seg.) 
    Z0 = Z0t;
    Z1 = Z1t;
    Z2 = Z2t;
          
    aD = !Z2&!Z0 | Z1 | Z2&Z0;
    bD = !Z2 | !Z1&!Z0 | Z1&Z0;
    cD = !Z1 | Z0 | Z2;
    dD = Z1&!Z0 | !Z2&Z1 | !Z2&!Z0 | Z2&!Z1&Z0;
    eD = !Z2&!Z0 | Z1&!Z0;
    fD = !Z1&!Z0 | Z2&!Z1 | Z2&!Z0;
    gD = !Z2&Z1 | Z2&!Z1 | Z2&!Z0;    

    digitalWrite(TaD, aD);
    digitalWrite(TbD, bD);
    digitalWrite(TcD, cD);
    digitalWrite(TdD, dD);
    digitalWrite(TeD, eD);
    digitalWrite(TfD, fD);
    digitalWrite(TgD, gD); 
  }
           


}
