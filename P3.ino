//PRÁCTICA 3
#include "PLD2020.h"

// Entradas
#define TLg0 10
#define TLg1 11
#define TLg2 12
#define TVF  13
#define TP0  18
#define TP1  19

#define TR0  20
#define TR1  21
#define TR2  22
#define TR3  23
#define TR4  24

#define TT0  25
#define TT1  26
#define TT2  27
#define TT3  28
#define TT4  29

#define TCP  30
#define TGO  31

#define TCC  34
#define TIni 35

// Salidas
#define TUNI  15
#define TDCN  16
#define TCEN  17

#define TQ0   39
#define TQ1   40
#define TQ2   41
#define TQ3   42
#define TQ4   43
#define TU0   44
#define TU1   45
#define TU2   46
#define TU3   47
#define TU4   48  

#define ToCLK 49

#define TUV0  54
#define TUV1  55
#define TUV2  56
#define TUV3  57
#define TDV0  58
#define TDV1  59
#define TUW0  60
#define TUW1  61
#define TUW2  62
#define TUW3  63
#define TDW0  64
#define TDW1  65

#define TO0   67
#define TO1   68
#define TO2   69
        
// Configuración variables internas globaleS
  bool Lg0 = 0, Lg1 = 0, Lg2 = 0, VF = 0, P0 = 0, P1 = 0;
  bool R0, R1, R2, R3, R4, T0, T1, T2, T3, T4;
  
  bool CP = 0, GO = 0, CC = 0, Ini = 0;
  bool UNI = 0, DCN = 0, CEN = 0, UNIt = 0, DCNt = 0, CENt = 0;
  
  bool Q0 = 0, Q1 = 0, Q2 = 0, Q3 = 0, Q4 = 0;  
  bool Q0t = 0, Q1t = 0, Q2t = 0, Q3t = 0, Q4t = 0;
  
  bool U0 = 0, U1 = 0, U2 = 0, U3 = 0, U4 = 0;
  bool U0t = 0, U1t = 0, U2t = 0, U3t = 0, U4t = 0;
  
  bool oCLK = 0, CC5 = 0, CC4 = 0, CC3 = 0, CC2 = 0, CC1 = 0, oCC = 0, oCCt = 0;
  
  bool UV0, UV1, UV2, UV3, DV0, DV1;
  bool UW0, UW1, UW2, UW3, DW0, DW1;
  
  bool O2 = 0, O1 = 0, O0 = 0;
  bool O2t = 0, O1t = 0, O0t = 0;
  
  bool V0, V1, V2, V3, V4, V0t, V1t, V2t, V3t, V4t;
  bool S0 = 0, S1 = 0, S2 = 0, S3 = 0, S4 = 0;  
  
  bool N2 = 1, N1 = 0, N0 = 0, Z;  
  bool MUX = 0, NE = 0;   
  
  bool M0 = 1, M1 = 0, M2 = 0, M0t, M1t, M2t;
  bool LSC = 0, LDQ = 0, SnD = 0,LV = 0;
  
void setup()
{
  pld_ini();
  pld_555(32);
}


void loop()
{  
  // Lectura de entradas  
  R0 =   digitalRead(TR0);  
  R1 =   digitalRead(TR1);
  R2 =   digitalRead(TR2);
  R3 =   digitalRead(TR3);
  R4 =   digitalRead(TR4);
   
  T0 =   digitalRead(TT0);
  T1 =   digitalRead(TT1);
  T2 =   digitalRead(TT2);
  T3 =   digitalRead(TT3);
  T4 =   digitalRead(TT4);
   
  CP =  digitalRead(TCP);
  GO =  digitalRead(TGO);

  CC =  digitalRead(TCC);
  Ini = digitalRead(TIni);

  Lg0 = digitalRead(TLg0);
  Lg1 = digitalRead(TLg1);
  Lg2 = digitalRead(TLg2);
  VF = digitalRead(TVF);
  P0 = digitalRead(TP0);
  P1 = digitalRead(TP1);

  //RELOJ 
  digitalWrite(ToCLK, CLK);
  CP =   digitalRead(TCP);     
   
  if (!CP) //SECUENCIALES
  {
//EXPRESIONES LÓGICAS DEL CONTADOR CON CARGA PARALELA
    O0t  = !O0&!MUX&!VF | !O0&MUX&VF | Lg0&MUX&!VF | Lg0&!MUX&VF;         
    O1t  = O1&!O0&!MUX&!VF | !O1&O0&!MUX&!VF | O1&!O0&MUX&VF | !O1&O0&MUX&VF | Lg1&MUX&!VF | Lg1&!MUX&VF;    
    O2t  = !O2&O1&O0&!MUX&!VF | O2&!O1&!MUX&!VF | O2&!O0&!MUX&!VF | !O2&O1&O0&MUX&VF | O2&!O1&MUX&VF | O2&!O0&MUX&VF | Lg2&MUX&!VF | Lg2&!MUX&VF;
    
//SALIDAS
    UNIt = !DCN&!UNI ;
    DCNt = !CEN&!DCN&UNI ;
    CENt = !CEN&DCN&!UNI ;
    
//DECREMENTADOR CON CARGA PARALELA Y DETECTOR DE CERO
    M0t = M0&!LDQ&!SnD | N0&LDQ&!SnD | (M1&!M0 | M2&!M0)&!LDQ&SnD; 
    M1t = M1&!LDQ&!SnD | N1&LDQ&!SnD | (M1&M0 | M2&!M1&!M0)&!LDQ&SnD; 
    M2t = M2&!LDQ&!SnD | N2&LDQ&!SnD | (M2&M0 | M2&M1)&!LDQ&SnD;
    
//REGISTRO V CON CARGA PARALELA
    V0t = LV&T0 | !LV&V0;
    V1t = LV&T1 | !LV&V1;
    V2t = LV&T2 | !LV&V2;
    V3t = LV&T3 | !LV&V3;
    V4t = LV&T4 | !LV&V4;      
    
//FLIP-FLOP C CON CLEAR
    oCCt = CC5&!CC&LSC; 
    
//REGISTRO DE CORRIMIENTO U CON CARGA PARALELA
    U0t = !LSC&!SnD&U0&!Ini | LSC&!SnD&S0&!Ini | !LSC&SnD&U1&!Ini;
    U1t = !LSC&!SnD&U1&!Ini | LSC&!SnD&S1&!Ini | !LSC&SnD&U2&!Ini;
    U2t = !LSC&!SnD&U2&!Ini | LSC&!SnD&S2&!Ini | !LSC&SnD&U3&!Ini;
    U3t = !LSC&!SnD&U3&!Ini | LSC&!SnD&S3&!Ini | !LSC&SnD&U4&!Ini;
    U4t = !LSC&!SnD&U4&!Ini | LSC&!SnD&S4&!Ini | !LSC&SnD&oCC&!Ini;
    
//REGISTRO DE CORRIMIENTO Q CON CARGA PARALELA
    Q0t = !LDQ&!SnD&Q0 | LDQ&!SnD&R0 | !LDQ&SnD&Q1;
    Q1t = !LDQ&!SnD&Q1 | LDQ&!SnD&R1 | !LDQ&SnD&Q2;
    Q2t = !LDQ&!SnD&Q2 | LDQ&!SnD&R2 | !LDQ&SnD&Q3;
    Q3t = !LDQ&!SnD&Q3 | LDQ&!SnD&R3 | !LDQ&SnD&Q4;
    Q4t = !LDQ&!SnD&Q4 | LDQ&!SnD&R4 | !LDQ&SnD&U0;
  }  
  else
  {                 
    
//EXPRESIONES COMBINACIONALES 
  NE = (!O2&!O1&O0 | !O2&O1&!O0 | O2&!O1&O0);
  MUX = !P1&!P0&GO | Z&P1&!P0 | Q0&!P1&P0 | NE&P1&P0;

//señales de control
  LV = !O2&!O1&O0;
  LSC = O2&!O1&O0;
  LDQ = !O2&O1&!O0;
  SnD = !O2&O1&O0;

//ESTADOS
    O0 = O0t;
    O1 = O1t;
    O2 = O2t;       
    
//Salidas
    UNI = UNIt;
    DCN = DCNt;
    CEN = CENt;

//Decrementador
    M0 = M0t;
    M1 = M1t;
    M2 = M2t;      
    
//Detector de CERO
    Z = !M0&!M1&!M2;
    
//Registro V con carga paralela
    V0 = V0t;
    V1 = V1t;
    V2 = V2t;
    V3 = V3t;
    V4 = V4t;
    
//Resgistro de corrimiento con carga paralela
    U0 = U0t;
    U1 = U1t;
    U2 = U2t;
    U3 = U3t;
    U4 = U4t;    
    
//Acarreo
    CC1 = U0&V0;
    CC2 = U1&V1 | U1&CC1 | V1&CC1;
    CC3 = U2&V2 | U2&CC2 | V2&CC2;
    CC4 = U3&V3 | U3&CC3 | V3&CC3;
    CC5 = U4&V4 | U4&CC4 | V4&CC4; 
    
//Sumador paralelo con acarreo adelantado
    S0 = !U0&V0 | U0&!V0;
    S1 = !U1&!V1&CC1 | !U1&V1&!CC1 | U1&!V1&!CC1 | U1&V1&CC1;   
    S2 = !U2&!V2&CC2 | !U2&V2&!CC2 | U2&!V2&!CC2 | U2&V2&CC2;
    S3 = !U3&!V3&CC3 | !U3&V3&!CC3 | U3&!V3&!CC3 | U3&V3&CC3;
    S4 = !U4&!V4&CC4 | !U4&V4&!CC4 | U4&!V4&!CC4 | U4&V4&CC4;    
    
//Flip-flop C con clear
    oCC = oCCt;   
    
//Registro de corrimiento Q con carga paralela
    Q0 = Q0t;
    Q1 = Q1t;
    Q2 = Q2t;
    Q3 = Q3t;
    Q4 = Q4t;
    
//Salidas decodificador 5bits a 7 segmentos
    UV0 = R0; 
    UV2 = !R4&!R3&R2 | !R4&R2&R1 | R4&R3&!R2 | R4&!R2&!R1; 
    UV1 = !R4&!R3&R1 | !R4&R3&R2&!R1 | R4&R3&!R2&R1 | R4&!R3&!R2&!R1 | !R3&R2&R1; 
    UV3 = !R4&R3&!R2&!R1 | R4&R3&R2&!R1 | R4&!R3&!R2&R1; 
     
    UW0 = T0; 
    UW2 = !T4&!T3&T2 | !T4&T2&T1 | T4&T3&!T2 | T4&!T2&!T1; 
    UW1 = !T4&!T3&T1 | !T4&T3&T2&!T1 | T4&T3&!T2&T1 | T4&!T3&!T2&!T1 | !T3&T2&T1; 
    UW3 = !T4&T3&!T2&!T1 | T4&T3&T2&!T1 | T4&!T3&!T2&T1;

    DV0 = !R4&R3&R1 | !R4&R3&R2 | R3&R2&R1 | R4&!R3&!R2;
    DV1 = R4&R3 | R4&R2;
     
    DW0 = !T4&T3&T1 | !T4&T3&T2 | T3&T2&T1 | T4&!T3&!T2;
    DW1 = T4&T3 | T4&T2;        
    
    
//Impresión de salidas 

    digitalWrite(ToCLK, oCLK);
    
    digitalWrite(TO0, O0);
    digitalWrite(TO1, O1);
    digitalWrite(TO2, O2);
   
    digitalWrite(TQ0, Q0);    
    digitalWrite(TQ1, Q1); 
    digitalWrite(TQ2, Q2); 
    digitalWrite(TQ3, Q3); 
    digitalWrite(TQ4, Q4); 
    
    digitalWrite(TU0, U0); 
    digitalWrite(TU1, U1);
    digitalWrite(TU2, U2);
    digitalWrite(TU3, U3);
    digitalWrite(TU4, U4);

//DISPLAYS 
    digitalWrite(TUV0, UV0);
    digitalWrite(TUV1, UV1);
    digitalWrite(TUV2, UV2);
    digitalWrite(TUV3, UV3);
    
    digitalWrite(TUW0, UW0);
    digitalWrite(TUW1, UW1);
    digitalWrite(TUW2, UW2);
    digitalWrite(TUW3, UW3);

    digitalWrite(TDV0, DV0);
    digitalWrite(TDV1, DV1);
        
    digitalWrite(TDW0, DW0);
    digitalWrite(TDW1, DW1);
    
 
    digitalWrite(TUNI, UNI);    
    digitalWrite(TDCN, DCN);
    digitalWrite(TCEN, CEN);
    
 

  }           
}
