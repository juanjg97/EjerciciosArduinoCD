// CONTROLADOR DE VELOCIDAD DE MOTORES CON EL ATMEGA 2560
#include "PLD2020.h"

// Entradas
#define TiCLK 29
#define TM    22
#define TS    23
#define TR    24
#define TV    25
#define TCKCD 26

// Salidas
#define TMOTPP 39
#define TMOTCD 40
#define TINI   41
#define TSELPP 42
#define TSELCD 43
#define TIN2   44
#define TIN1   45
#define TDIR   46
#define TCKAS  48
#define ToCLK  49
#define TV0    54
#define TV1    55
#define TV2    56
#define TV3    57
#define TA     2
#define TNA    3
#define TB     4
#define TNB    5
//-----------------------------------------------------------

// Variables globales presentes, futuras y de selección
  bool A = 0, B = 0, DIR = 0;
  bool At = 0, Bt = 0, DIRt = 0;
  
  bool MCD = 0, MPP = 0, O0 = 0, O1 = 0, O2 = 0;
  bool MCDt = 0, MPPt = 0, O0t = 0, O1t = 0, O2t = 0;
  
  bool Q0 = 0, Q1 = 0, Q2 = 0, Q3 = 0;
  bool Q0t = 0, Q1t = 0, Q2t = 0, Q3t = 0;
  
  bool V0, V1, V2, V3;
  bool V0t, V1t, V2t, V3t;

  bool CKAS, CKPP, CKCD, iCLK, M, S, R, V;
  bool IN1, IN2, INI = 1, MOTCD, MOTPP, NA, NB, PWM, SELCD, SELPP;
//-----------------------------------------------------------
  
// Configuración inicial, métodos para entradas, frecuencia y que siempre esté ini al principio cuando oprimimos reset
void setup()
{
  pld_ini();
  pld_555(150);
  digitalWrite(TINI, INI);
}

// Programa a repetir
void loop()
{  
// Configuración de variables internas secuenciales

                             // Lectura de entradas
  M    = digitalRead(TM);
  S    = digitalRead(TS);
  R    = digitalRead(TR);
  V    = digitalRead(TV);

                            // Generador pulso de reloj asíncrono
  CKAS = M | S | R | V;
  
                            // Realimentación pulso de relojes asíncrono y principal
  digitalWrite(TCKAS, CKAS);
  digitalWrite(ToCLK, CLK);
  iCLK = digitalRead(TiCLK);
  CKCD = digitalRead(TCKCD);
  
//-----------------------------------------------------------
            // Salida expresiones combinacionales
            // Cálculo expresiones secuenciales reloy asíncrono
  if (!iCLK)
  {
                                        // Cálculo valores futuros
    DIRt = O2&O0&(!DIR&S | DIR&!S&!R);
    MCDt = SELCD&(!MCD&M | MCD&!M&!R);
    MPPt = SELPP&(!MPP&M | MPP&!M&!R);
    O0t  = O2 | !O0&M | !O1&O0&!R | O1&!M;
    O1t  = O2&O1 | !O2&!O1&O0&M | O1&!M&!R;
    O2t  = S&!O2&O0 | !R&O2;
    V0t  = (O2&O0);
    V1t  = (O2&O0)&(!V3&V2 | !V1&V0&V | !V3&V1&!R |  V3&!R&!V);
    V2t  = (O2&O0)&(!V2&V1&V | !V3&V2&!R | V3&!R&!V);
    V3t  = (O2&O0)&(!V3&V2&V | V3&!R&!V);
  }
  else
  {
                  // Memorización valores
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
    O0   = O0t;
    O1   = O1t;
    O2   = O2t;

                            // Cálculo expresiones combinacionales
    INI   = !O2&!O1&!O0;
    MOTCD = !O2&!O1&O0;
    MOTPP = !O2&O1&O0;
    SELCD = O2&!O1&O0;
    SELPP = O2&O1&O0;
  
                          // Salida valores secuenciales
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
  // Cálculo expresiones secuenciales reloj principal
  if (!CKCD)
  {
  // Expresiones lógicas del contador hexadecimal
    Q0t  = !Q0;
    Q1t  = !Q1&Q0 | Q1&!Q0;
    Q2t  = Q2&!Q1 | Q2&!Q0 | !Q2&Q1&Q0;
    Q3t  = Q3&!Q1 | Q3&!Q0 | Q3&!Q2 | !Q3&Q2&Q1&Q0;
  }
  else
  {
  // Memorización valores
    Q0  = Q0t;
    Q1  = Q1t;
    Q2  = Q2t;
    Q3  = Q3t;
   //-----------------------------------------------------------
  // Cálculo expresiones combinacionales
    PWM  = (V3&!Q3 | V3&Q3&V2&!Q2 | !V3&!Q3&V2&!Q2 |
           V3&Q3&V2&Q2&V1&!Q1 | V3&Q3&!V2&!Q2&V1&!Q1 |
           !V3&!Q3&V2&Q2&V1&!Q1 | !V3&!Q3&!V2&!Q2&V1&!Q1 |
           V3&Q3&V2&Q2&V1&Q1&V0&!Q0 | V3&Q3&V2&Q2&!V1&!Q1&V0&!Q0 |
           V3&Q3&!V2&!Q2&V1&Q1&V0&!Q0 | V3&Q3&!V2&!Q2&!V1&!Q1&V0&!Q0 |
           !V3&!Q3&V2&Q2&V1&Q1&V0&!Q0 | !V3&!Q3&V2&Q2&!V1&!Q1&V0&!Q0 |
           !V3&!Q3&!V2&!Q2&V1&Q1&V0&!Q0 | !V3&!Q3&!V2&!Q2&!V1&!Q1&V0&!Q0)&MCD;

    IN1  = DIR&PWM;
    IN2  = !DIR&PWM;
    
  // Salida expresiones combinacionales
  if(MCD)
  {
    digitalWrite(TA, 0);
    digitalWrite(TNA, 0);
    digitalWrite(TB, 0);
    digitalWrite(TNB, 0);
    }
    digitalWrite(TIN1, IN1);
    digitalWrite(TIN2, IN2);
  }

 //-----------------------------------------------------------
// Expresión lógica reloj motor PP
  CKPP = (!V3&!V2&!V1&V0&Q3 | !V3&!V2&V1&Q2 |
         !V3&V2&Q1 | V3&Q0)&MPP;
//-----------------------------------------------------------
// Cálculo expresiones secuenciales reloj 2
  if (!CKPP)
  {
  // Expresiones lógicas de los bits del motor PP
    At  = !DIR&!B | DIR&B;
    Bt  = !DIR&A | DIR&!A;
  }
  else
  {
  // Memorización valores
    A   = At;
    B   = Bt;
 //-----------------------------------------------------------

  // Cálculo salidas combinacionales
    NA  = !A;
    NB  = !B;
    
  // Salida valores secuenciales

    digitalWrite(TA, A);
    digitalWrite(TNA, NA);
    digitalWrite(TB, B);
    digitalWrite(TNB, NB);
    
    
  }
}
