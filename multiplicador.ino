#include "PLD2020.h"

//Entradas 
#define TR0    20
#define TR1    21
#define TR2    22
#define TR3    23
#define TR4    24

#define TT0    25
#define TT1    26
#define TT2    27
#define TT3    28
#define TT4    29

#define TCP    30
#define TGO    31
#define TLV    32
#define TLSC   33
#define TCC    34
#define TIni   35
#define TLDQ   36
#define TSnD   37
 
//Salidas
#define TUV0   54
#define TUV1   55
#define TUV2   56
#define TUV3   57 
#define TDV0   58

#define TDV1   59
#define TUW0   60

#define TUW1   61
#define TUW2   62
#define TUW3   63

#define TDW0   64 
#define TDW1   65

#define TQ0    39
#define TQ1    40
#define TQ2    41
#define TQ3    42
#define TQ4    43

#define TU0    44
#define TU1    45
#define TU2    46
#define TU3    47
#define TU4    48

// Configuración variables internas globales
bool CP=0, GO=0, LV=0, LSC=0, oCC=0,oCCt=0, Ini=0,LDS=0,LDQ=0, SnD=0,Z=0,CC=0,CC5=0;

bool UV0=0,UV1=0,UV2=0,UV3=0,DV0=0,DV1=0;

bool UW0=0,UW1=0,UW2=0,UW3=0,DW0=0,DW1=0;

bool N0=0,N1=0,N2=1;

bool U0=0,U1=0,U2=0,U3=0,U4=0;

bool U0t=0,U1t=0,U2t=0,U3t=0,U4t=0;

bool R0,R1,R2,R3,R4;

bool M0=0,M1=0,M2=0;

bool M0t=0,M1t=0,M2t=0;

bool S0,S1,S2,S3,S4,S5;

bool T0,T1,T2,T3,T4;

bool Q0=0,Q1=0,Q2=0,Q3=0,Q4=0;

bool Q0t=0,Q1t=0,Q2t=0,Q3t=0,Q4t=0;

bool V0=0,V1=0,V2=0,V3=0,V4=0;

bool V0t=0,V1t=0,V2t=0,V3t=0,V4t=0;



void setup() {

  pld_ini();
  pld_555(16);

}

void loop() {
  //Lectura de entradas
  R0 = digitalRead(TR0);
  R1 = digitalRead(TR1);
  R2 = digitalRead(TR2);
  R3 = digitalRead(TR3);
  R4 = digitalRead(TR4);
  
  T0 = digitalRead(TT0);
  T1 = digitalRead(TT1);
  T2 = digitalRead(TT2);
  T3 = digitalRead(TT3);
  T4 = digitalRead(TT4);
  
  CP = digitalRead(TCP);
  GO = digitalRead(TGO);
  LV = digitalRead(TLV);
  LSC = digitalRead(LSC);
  CC = digitalRead(TCC);
  Ini = digitalRead(TIni);
  LDQ = digitalRead(TLDQ);
  SnD = digitalRead(TSnD);

  if(!CLK)
  {
//DECREMENTADOR CON CARGA PARALELA 

    M0t = M0&!LDQ&!SnD | N0&LDQ&!SnD | (M1&M0 | M2&M0)&!LDQ&SnD;
    M1t = M1&!LDQ&!SnD | N1&LDQ&!SnD | (M1&M0 | M2&!M0&!M0)&!LDQ&SnD;
    M2t = M2&!LDQ&!SnD | N2&LDQ&!SnD | (M2&M0 | M2&M1)&!LDQ&SnD;
    
//REGISTRO V CON CARGA PARALELA

    V0t=LV&T0 | !LV&V0;
    V1t=LV&T1 | !LV&V1;
    V2t=LV&T2 | !LV&V2;
    V3t=LV&T3 | !LV&V3;
    V4t=LV&T4 | !LV&V4;

//FLIP FLOP CON CLEAR
    oCCt=  CC5&!CC&LSC;

//REGISTRO DE CORRIMIENTO U CON CARGA PARALELA
  //LCS-- Señal de carga
    
    U0t=!LSC&!SnD&U0&!Ini |  LSC&!SnD&S0&!Ini | !LDS&SnD&U1&!Ini;
    U1t=!LSC&!SnD&U1&!Ini |  LSC&!SnD&S1&!Ini | !LDS&SnD&U2&!Ini;
    U2t=!LSC&!SnD&U2&!Ini |  LSC&!SnD&S2&!Ini | !LDS&SnD&U3&!Ini;
    U3t=!LSC&!SnD&U3&!Ini |  LSC&!SnD&S3&!Ini | !LDS&SnD&U4&!Ini;
    
    U4t=!LSC&!SnD&U4&!Ini |  LSC&!SnD&S4&!Ini | !LDS&SnD&oCC&!Ini;

//REGISTRO DE CORRIMIENTO Q CON CARGA PARALELA
   //LDQ-- Señal de carga   
    Q0t=!LDQ&!SnD&Q0 | LDQ&!SnD&R0 | !LDQ&SnD&Q1;
    Q1t=!LDQ&!SnD&Q1 | LDQ&!SnD&R1 | !LDQ&SnD&Q2;
    Q2t=!LDQ&!SnD&Q2 | LDQ&!SnD&R2 | !LDQ&SnD&Q3;
    Q3t=!LDQ&!SnD&Q3 | LDQ&!SnD&R3 | !LDQ&SnD&Q4;
    Q4t=!LDQ&!SnD&Q4 | LDQ&!SnD&R4 | !LDQ&SnD&U0;

    }
    else
    {
 //DECREMENTADOR CON CARGA PARALELA Y DETECTOR 0    
    M0=M0t;  
    M1=M1t;
    M2=M2t;
    
    oCC=oCCt;
 //REGISTRO DE CORRIMIENTO U CON CARGA PARALELA      
    U0=U0t;
    U1=U1t; 
    U2=U2t; 
    U3=U3t; 
    U4=U4t;  
 //REGISTRO DE CORRIMIENTO Q CON CARGA PARALELA    
    Q1=Q1t;
    Q2=Q2t;
    Q3=Q3t;
    Q4=Q4t;
//DETECTOR 0   
    Z=!M0&!M1&!M2;
    
//REGISTRO V CON CARGA PARALELA  
    V0=V0t;
    V1=V1t;
    V2=V2t;
    V3=V3t;
    V4=V4t;
    
//SUMADOR PARALELO CON ACARREO ADELANTADO

    S1 = !U1&!V1&U0&V0 | !U1&V1& (!U0 | !V0) | U1&!V1& (!U0 | !V0) | U1&V1&U0&V0;
    S2 = !U2&!V2&U1&V1 | !U2&V2& (!U1 | !V1) | U2&!V2& (!U1 | !V1) | U2&V2&U1&V1;
    S3 = !U3&!V3&U2&V2 | !U3&V3& (!U2 | !V2) | U3&!V3& (!U2 | !V2) | U3&V3&U2&V2;
    S4 = !U4&!V4&U3&V3 | !U4&V4& (!U3 | !V3) | U4&!V4& (!U3 | !V3) | U4&V4&U3&V3;
    S5 = !U0&!V0&U4&V4 | !U0&V0& (!U4 | !V4) | U0&!V0& (!U4 | !V4) | U0&V0&U4&V4;
    
    UV0=T0;
    UV1=!T4&!T3&T1 | !T4&T3&T2&!T1 | T4&T3&!T2&T1 | T4&!T3&!T2&!T1 | T4&!T3&T2&T1;
    UV2=!T4&!T3&T2 | !T4&T2&T1 | T4&T3&!T2 | T4&!T2&!T1;
    UV3=!T4&T3&!T2&!T1 | T4&T3&T2&!T1 | T4&!T3&!T2&T1; 
    
    DV0=!T4&T3&T1 | !T4&T3&T2 | T3&T2&T1 | T4&!T3&!T2;
    DV1= T4&T3 | T4&T2;
    
    UW0=R0;
    UW1=!R4&!R3&R1 | !R4&R3&R2&!R1 | R4&R3&!R2&R1 | R4&!R3&!R2&!R1 | R4&!R3&R2&R1;
    UW2=!R4&!R3&R2 | !R4&R2&R1 | R4&R3&!R2 | R4&!R2&!R1;
    UW3=!R4&R3&!R2&!R1 | R4&R3&R2&!R1 | R4&!R3&!R2&R1;
    
    DW0=!R4&R3&R1 | !R4&R3&R2 | R3&R2&R1 | R4&!R3&!R2;
    DW1=R4&R3 | R4&R2;
    
 //IMPRESIÓN DE VALORES 

    digitalWrite(TU0, U0);
    digitalWrite(TU1, U1);
    digitalWrite(TU2, U2);
    digitalWrite(TU3, U3);
    digitalWrite(TQ0, Q0);
    digitalWrite(TQ1, Q1);
    digitalWrite(TQ2, Q2);
    digitalWrite(TQ3, Q3);
    
 
    digitalWrite(TUV0, UV0);
    digitalWrite(TUV1, UV1);
    digitalWrite(TUV2, UV2);
    digitalWrite(TUV3, UV3);
    
    digitalWrite(TDV0, DV0);
    digitalWrite(TDV1, DV1);
    
    digitalWrite(TUW0, UW0);
    digitalWrite(TUW1, UW1);
    digitalWrite(TUW2, UW2);
    digitalWrite(TUW3, UW3);
    
    digitalWrite(TDW0, DW0);
    digitalWrite(TDW1, DW1);
      }
}
