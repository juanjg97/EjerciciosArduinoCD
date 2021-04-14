// CONTROLADOR DE VELOCIDAD DE MOTORES CON EL ATMEGA 2560
#include "PLD2020.h"

// Entradas
#define TiCLK_AS 29 //Entrada para el reloj asíncrono
#define TM       22
#define TS       23
#define TR       24
#define TV       25

// Salidas
#define TMOTPP  39 //Combinacional
#define TMOTCD  40 //Combinacional
#define TINI    41 //Combinacional
#define TSELPP  42 //Combinacional
#define TSELCD  43 //Combinacional

#define TIN2    44 //Combinacional
#define TIN1    45 //Combinacional
#define TDIR    46 //Secuencial
#define ToCLK_AS  48 //Salida de los pulsos para el reloj asíncrono
#define TV0    54 //Secuencial
#define TV1    55 //Secuencial
#define TV2    56 //Secuencial
#define TV3    57 //Secuencial

#define TA     2 //Secuencial
#define TNA    3 //Combinacional
#define TB     4 //Secuencial
#define TNB    5 //Combinacional

// Variables globales: Secuenciales presentes y futuras 
  
  //Estados del menú
  bool O0t = 0, O1t = 0, O2t = 0;  
  bool O0 = 0, O1 = 0, O2 = 0;
  
  //Estados del contador para PWM
  bool Q0 = 0, Q1 = 0, Q2 = 0, Q3 = 0;
  bool Q0t = 0, Q1t = 0, Q2t = 0, Q3t = 0;
  
  //Expresiones para el motor PP y dirección
  bool A = 0, B = 0, DIR = 0;
  bool At = 0, Bt = 0, DIRt = 0;

  //Selección del motor CD y PP
  bool MCD = 0, MPP = 0;
  bool MCDt = 0, MPPt = 0;
  
 //Velocidades
  bool V0=0, V1=0, V2=0, V3=0;
  bool V0t=0, V1t=0, V2t=0, V3t=0;


// Variables de entrada y relojes.
  bool M, S, R, V, iCLK_AS, oCLK_AS, CLK_PP ;

// Variables de salida combinacionales 
  bool IN1, IN2, INI = 1, MOTCD, MOTPP, NA, NB, PWM, SELCD, SELPP;
  
// Configuración inicial, métodos para entradas, frecuencia y que siempre esté ini al principio cuando oprimimos reset
void setup()
{
  pld_ini();
  pld_555(150);
  digitalWrite(TINI, INI);
}


void loop()
{  
// Lectura de entradas
  M    = digitalRead(TM);
  S    = digitalRead(TS);
  R    = digitalRead(TR);
  V    = digitalRead(TV);
// Expresión para generar el pulso del reloj asíncrono e impresión del pulso (verdadero o falso)
   oCLK_AS = M | S | R ;
   digitalWrite(ToCLK_AS, oCLK_AS);
   
//Lectura del pulso para el reloj asíncrono
  iCLK_AS  = digitalRead(TiCLK_AS );
  
// Cálculo expresiones secuenciales reloj asíncrono, hacemos el recorrido por los estados del menú
  if (!iCLK_AS)
  {
 // Cálculo de variables secuenciales
  //Variables de estado (futuro) del controlador del menú
  
    O0t  = O2 | !O0&M | !O1&O0&!R | O1&!M;
    O1t  = O2&O1 | !O2&!O1&O0&M | O1&!M&!R;
    O2t  = S&!O2&O0 | !R&O2;

   //Variables de estado (futuro) dirección, son secuenciales porque no sólo dependen del estado en el que estén y de varialbes combinacionales
    DIRt = O2&O0&(!DIR&S | DIR&!S&!R);
    MCDt = SELCD&(!MCD&M | MCD&!M&!R);
    MPPt = SELPP&(!MPP&M | MPP&!M&!R);
    
  //Variables de estado (futuro) para el comparador de velocidad
    V0t  = (O2&O0);
    V1t  = (O2&O0)&(!V3&V2 | !V1&V0&V | !V3&V1&!R |  V3&!R&!V);
    V2t  = (O2&O0)&(!V2&V1&V | !V3&V2&!R | V3&!R&!V);
    V3t  = (O2&O0)&(!V3&V2&V | V3&!R&!V);
  }
  else
  {
    // Memorización variables secuenciales, valores presentes.
    O0   = O0t;
    O1   = O1t;
    O2   = O2t;
    
    V0   = V0t;
    V1   = V1t;
    V2   = V2t;
    V3   = V3t;
    
    DIR  = DIRt;
    MCD  = MCDt;
    MPP  = MPPt;

    // Cálculo de variables combinacionales
    INI   = !O2&!O1&!O0;
    
    MOTCD = !O2&!O1&O0;
    MOTPP = !O2&O1&O0;
    
    SELCD = O2&!O1&O0;
    SELPP = O2&O1&O0;
  
    // Impresion de valores secuenciales y combinacionales
    digitalWrite(TV0, V0);
    digitalWrite(TV1, V1);
    digitalWrite(TV2, V2);
    digitalWrite(TV3, V3);
    digitalWrite(TDIR, DIR);
    
    digitalWrite(TINI, INI);
    digitalWrite(TMOTCD, MOTCD);
    digitalWrite(TMOTPP, MOTPP);
    digitalWrite(TSELCD, SELCD);
    digitalWrite(TSELPP, SELPP);
  } 


//-----------------------------------------------------------
  // Cálculo expresiones secuenciales reloj para el motor CD, usando el reloj de arduino
  if (!CLK)
  {
  // Expresiones lógicas del contador hexadecimal, secuenciales futuras
    Q0t  = !Q0;
    Q1t  = !Q1&Q0 | Q1&!Q0;
    Q2t  = Q2&!Q1 | Q2&!Q0 | !Q2&Q1&Q0;
    Q3t  = Q3&!Q1 | Q3&!Q0 | Q3&!Q2 | !Q3&Q2&Q1&Q0;
  }
  else
  {
  // Memorización valores, secuenciales presentes
    Q0  = Q0t;
    Q1  = Q1t;
    Q2  = Q2t;
    Q3  = Q3t;
    
  // Cálculo expresiones combinacionales, comparador
  
    PWM  = (V3&!Q3 | V3&Q3&V2&!Q2 | !V3&!Q3&V2&!Q2 |V3&Q3&V2&Q2&V1&!Q1 | V3&Q3&!V2&!Q2&V1&!Q1 
          |!V3&!Q3&V2&Q2&V1&!Q1 | !V3&!Q3&!V2&!Q2&V1&!Q1 |V3&Q3&V2&Q2&V1&Q1&V0&!Q0 
          | V3&Q3&V2&Q2&!V1&!Q1&V0&!Q0 |V3&Q3&!V2&!Q2&V1&Q1&V0&!Q0 | V3&Q3&!V2&!Q2&!V1&!Q1&V0&!Q0 
          |!V3&!Q3&V2&Q2&V1&Q1&V0&!Q0 | !V3&!Q3&V2&Q2&!V1&!Q1&V0&!Q0 | !V3&!Q3&!V2&!Q2&V1&Q1&V0&!Q0 
          | !V3&!Q3&!V2&!Q2&!V1&!Q1&V0&!Q0)&MCD;

    IN1  = DIR&PWM;
    IN2  = !DIR&PWM;
    
  // Impresión expresiones combinacionales

    digitalWrite(TIN1, IN1);
    digitalWrite(TIN2, IN2);
  }


// Expresión lógica reloj motor PP, depende de las Qs del CD y las V, siempre y cuando esté activado MPP
   CLK_PP = (!V3&!V2&!V1&V0&Q3 | !V3&!V2&V1&Q2 |!V3&V2&Q1 | V3&Q0)&MPP;

  if (!CLK_PP)
  {
   Cálculo variables secuenciales para el motor PP
    At  = !DIR&!B | DIR&B;
    Bt  = !DIR&A | DIR&!A;
  }
  else
  {
  // Memorización variables secuenciales motor pp, estado futuro
    A   = At;
    B   = Bt;

  // Cálculo de variables combinacionales (dependen de las combinacionales)
    NA  = !A;
    NB  = !B;
    
  // Salida valores secuenciales y combinacionales
    digitalWrite(TA, A);
    digitalWrite(TNA, NA);
    
    digitalWrite(TB, B);
    digitalWrite(TNB, NB);
  }
}
