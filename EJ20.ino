//EJERCICIO 48



//RETARDO
const int  retardo = 10;

// Variable muda
byte pinActual;

//DECLARACIÓN DE ENTRADAS
const byte entradasDip[]{22, 23, 24, 25, 26, 27, 28, 29, 20, 21};
byte TINI =21, TISENT=20; 


//DECLARACIÓN DE SALIDAS
  //PWM,SENTIDO, VELOCIDAD Y DISPLAYS
const byte salCent[]{42,43,44,45,46,47,48,49};//Bits de entrada al display centenas
const byte salDec []{54,55,56,57,58,59,60,61};//Bits de entrada al display decenas
const byte salUni []{62,63,64,65,66,67,68,69};//Bits de entrada al display unidades

byte TPWM=13,TOSENT=31,TVEL=30;
  //Pines para decenas, sentenas y unidades

byte  TDISPC=39,TDISPD=40,TDISPU=41; //Selecciona el display para unidades, decentas o sentenas


//CONFIGURACIÓN PARA LOS SEGMENTOS DE LOS DISPLAYS
  //CENTENAS

const byte numDisp = 7;//Número de entradas del display
const boolean dispCent[][7] = {
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
//CONFIGURACIÓN PARA LOS SEGMENTOS DE LOS DISPLAYS
  //DECENAS

const boolean dispDec[][7] = {
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
//CONFIGURACIÓN PARA LOS SEGMENTOS DE LOS DISPLAYS
  //DECENAS

const boolean dispUni[][7] = {
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
       pinMode (salDec[pinActual],OUTPUT);
       pinMode (salUni[pinActual],OUTPUT);
   }
    pinMode (TPWM,  OUTPUT);
    pinMode (TOSENT,OUTPUT);
    pinMode (TVEL,  OUTPUT);
    pinMode (TDISPC,OUTPUT);
    pinMode (TDISPD,OUTPUT);
    pinMode (TDISPU,OUTPUT);
    


}
//***************************************************************
//***************************************************************
 //Rutina que se estará realizando continuamente
void loop() {
  
//LEEMOS ENTRADAS
  boolean dipSwitch[8];
  boolean INI=0,ISENT=0;
  
  for(pinActual=0; pinActual<8;pinActual++)
   {
       dipSwitch[pinActual]=digitalRead(entradasDip[pinActual]);//Obtenemos el número ingresado mediante el DipSwitch
   }
    INI  =digitalRead(TINI ); //debemos hacer un if para el inicio
    ISENT=digitalRead(TISENT );//leemos la dirección del sentido
    
//Convertimos el arreglo bool a arreglo int para las operaciones

  int dipSInt[8];
  
  for(int i=0; i<8;i++)
  {
    if(dipSwitch[i]){
      dipSInt[i]=1;
    }else{
      dipSInt[i]=0;
    }
  }

//Convertimos el arreglo int binario a un número decimal
  
  int conversion=0;
  int res=1; //Acumulador para la multiplicación
 
  for(int i=7; i>=0; i--)
  {
    conversion=(res*2)+dipSInt[i];
    res=conversion;
  }

//Obtenemos las unidades, decenas y centenas por separado
  
  
  int centenas=(conversion/100);
  int decenas=(conversion- ( centenas*100))/10;
  int unidades=conversion-( centenas*100 + decenas*10 );

// Escritura de las UNIDADES en el despliegue de siete segmentos
    digitalWrite (TDISPU,HIGH);

    for (pinActual = 0; pinActual < numDisp; pinActual++)
     {
     digitalWrite (salUni [pinActual],dispUni[unidades][pinActual]);
     }
     
     delay (retardo);

// Escritura de las DECENAS en el despliegue de siete segmentos
    digitalWrite (TDISPD,HIGH);

    for (pinActual = 0; pinActual < numDisp; pinActual++)
     {
     digitalWrite (salDec [pinActual],dispUni[decenas][pinActual]);
     }
     
     delay (retardo);

// Escritura de las CENTENAS en el despliegue de siete segmentos
    digitalWrite (TDISPC,HIGH);

    for (pinActual = 0; pinActual < numDisp; pinActual++)
     {
     digitalWrite (salCent [pinActual],dispUni[centenas][pinActual]);
     }
     
     delay (retardo);


    

}
