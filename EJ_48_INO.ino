//EJERCICIO 48
//***************************************************************
int tiempo=0;
boolean encendido=0;
int retardo=0;
int j=0;
int numMax=0;
int dif=0;

//***************************************************************
// Variable muda
byte pinActual;

//DECLARACIÓN DE ENTRADAS
const byte entradasDip[]{22, 23, 24, 25, 26, 27, 28, 29, 20};
byte TINI =21, TISENT=20; 

//DECLARACIÓN DE SALIDAS
  //PWM,SENTIDO, VELOCIDAD Y DISPLAYS
const byte salCent[]{42,43,44,45,46,47,48,49};//Bits de entrada al display centenas
const byte salDec []{54,55,56,57,58,59,60,61};//Bits de entrada al display decenas
const byte salUni []{62,63,64,65,66,67,68,69};//Bits de entrada al display unidades

byte TPWM=13,TOSENT=31, TONSENT=32;
  //Pines para decenas, sentenas y unidades
byte  TDISPC=39,TDISPD=40,TDISPU=41; //Selecciona el display para unidades, decentas o sentenas


//CONFIGURACIÓN PARA LOS SEGMENTOS DE LOS DISPLAYS
 
const byte numDisp = 7;//Número de entradas del display
const boolean displays[][7] = {
        {1,1,1,1,1,1,0},  // Despliegue para 0
        {0,1,1,0,0,0,0},  // Despliegue para 1
        {1,1,0,1,1,0,1},  // Despliegue para 2
        {1,1,1,1,0,0,1},  // Despliegue para 3
        {0,1,1,0,0,1,1},  // Despliegue para 4
        {1,0,1,1,0,1,1},  // Despliegue para 5
        {1,0,1,1,1,1,1},  // Despliegue para 6
        {1,1,1,0,0,0,0},  // Despliegue para 7
        {1,1,1,1,1,1,1},  // Despliegue para 8
        {1,1,1,1,0,1,1}}; // Despliegue para 9

  boolean dipSwitch[8];
  boolean ISENT=0;

//***************************************************************
//***************************************************************
  //CONFIGURACIÓN DEL PROGRAMA, Se configuran únicamente las terminales digitales

void setup() {
 
 //DECLARAMOS ENTRADAS
 for(pinActual=0; pinActual<(sizeof(entradasDip));pinActual++)
   {
       pinMode (entradasDip[pinActual],INPUT);
   }
    pinMode (TINI,INPUT);
    pinMode (TISENT,INPUT);

//DECLARAMOS SALIDAS
 for(pinActual=0; pinActual<(sizeof(salCent));pinActual++)
   {
       pinMode (salCent[pinActual],OUTPUT);
       pinMode (salDec [pinActual],OUTPUT);
       pinMode (salUni [pinActual],OUTPUT);
   }
      pinMode (TPWM,  OUTPUT);
      pinMode (TOSENT,OUTPUT);
      pinMode (TONSENT,OUTPUT);
      pinMode (TDISPC,OUTPUT);
      pinMode (TDISPD,OUTPUT);
      pinMode (TDISPU,OUTPUT);
      pinMode (6,OUTPUT);
      pinMode (7,OUTPUT);
      pinMode (8,OUTPUT);   
  }

//***************************************************************
//***************************************************************
 //Rutina que se estará realizando continuamente

void loop() {

//LEEMOS ENTRADAS
  
  for(pinActual=0; pinActual<8;pinActual++)
   {
       dipSwitch[pinActual]=digitalRead(entradasDip[pinActual]);//Obtenemos el número ingresado mediante el DipSwitch
   }
    encendido=digitalRead(TINI); //debemos hacer un WHILE PARA EL GIRO DEL MOTOR, CON ESTA VARIABLE   
    ISENT=digitalRead(TISENT);    //leemos la dirección del sentido
    

//Convertimos el arreglo bool a arreglo int para las operaciones
  int dipSInt[8];
  for(int i=0; i<8;i++)
  {
    if(dipSwitch[i]==1){
      dipSInt[i]=0;
    }else{
      dipSInt[i]=1;
    }
  }

//Convertimos el arreglo int binario a un número decimal
  
  int conversion=0;
  int res=0; 
  
  for(int i=7; i>=0; i--)
  {
    conversion=(res*2)+dipSInt[i];
    res=conversion;
  }
//Obtenemos los diferentes valores para los distintos casos
  
  tiempo=conversion;
  numMax=(tiempo*10)/2;
  dif=((tiempo-51)*100)/2;

//Obtenemos las unidades, decenas y centenas por separado para mostrarlas en el display
  int centenas=(conversion/100);
  int decenas=(conversion-(centenas*100))/10;
  int unidades=conversion-(centenas*100+decenas*10);

// Escritura de las UNIDADES en el despliegue de siete segmentos
    for (pinActual = 0; pinActual < numDisp; pinActual++)
     {
     digitalWrite (salUni[pinActual],displays[unidades][pinActual]);
     }   

// Escritura de las DECENAS en el despliegue de siete segmentos
    for (pinActual = 0; pinActual < numDisp; pinActual++)
     {
     digitalWrite (salDec[pinActual],displays[decenas][pinActual]);
     }  

// Escritura de las CENTENAS en el despliegue de siete segmentos
    for (pinActual = 0; pinActual < numDisp; pinActual++)
     {
     digitalWrite (salCent[pinActual],displays[centenas][pinActual]);
     }



 //*************************************CÓDIGO PARA PWM*******************************************
              
digitalWrite(TOSENT, LOW);
digitalWrite(TONSENT,LOW);       

    

while(!encendido)
{
 digitalWrite(TOSENT,ISENT);
 digitalWrite(TONSENT,!ISENT);
//*****************************CASO 0 *********************************
if(tiempo>=0 && tiempo<=10)
  {
    encendido=!false;
    analogWrite(13,0);
  }
 
//*****************************CASO 1*********************************

if(tiempo>10 && tiempo<20)
  {
   encendido=!false;
   analogWrite(13,76);
   retardo=(tiempo*100)/2;   
   delay(retardo);
   analogWrite(13,0);
  }

//*****************************CASO 2*********************************
if(tiempo>=20 && tiempo<=51)
  {
    encendido=!false;   
    for(j=0; j<=numMax;j++)        
     {                                                            
       analogWrite(13,j);
       delay(5);     
     }

  for(j=255; j>=0; j--)        
      {                                                 
       analogWrite(13,j);
       delay(5);       
      }
  analogWrite(13,0);
  }
 
 
//*****************************CASO 3*********************************

  if(tiempo>51)
  { 
    encendido=!false;
    for(j=0;j<=255;j++)
      {
        analogWrite(13,j);
        delay(5);
      }
     
     analogWrite(13,255);
     delay(dif);
     
     for(j=255;j>=0;j--)
      {
        analogWrite(13,j);
        delay(5);
      }    
    analogWrite(13,0);
  }

}
 
}
