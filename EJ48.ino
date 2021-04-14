//EJERCICIO 48 tarea

//****************************************************
//Variables globales
const int  retardo = 9;

//Pines para decenas, sentenas y unidades
const byte SelDisp[] = {63,64,65}; //Selecciona el display para unidades, decentas o sentenas
const byte numSelDisp = 3;//Sólo tenemos 3 opciones de display


//**************************************************************
  //Configuración para los displays

const byte inDisp[] = {43,44,45,46,47,48,49};//Bits de entrada a los displays

const byte numDisp = 7; //Número de entradas del display

const boolean dispOut[][7] = {
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
//**************************************************************


// Variable muda
byte pinActual;


//***************************************************************

void setup() {
  //Se configuran únicamente las terminales digitales
  
for (pinActual = 0; pinActual < numDisp; pinActual++)
  pinMode (inDisp[pinActual],OUTPUT);



for (pinActual = 0; pinActual < numSelDisp; pinActual++)
  pinMode (SelDisp[pinActual],OUTPUT);

  pinMode (dischPin,OUTPUT);

}
//***************************************************************
void loop() {
  //Rutina que se estará realizando continuamente

  int x = 0;
  
  digitalWrite (dischPin, HIGH);
  delay (50);
  
  digitalWrite (dischPin, LOW);
  long initialTime = millis ();

  do{
  x = analogRead (VCpin);
  }while (x < thrshldV);
  
  long finalTime = millis ();
  
//***************************************************************
// Cálculo de la capacitancia
    int capacitance = finalTime - initialTime;
    
// Obtención de las centenas, decenas y unidades a desplegar
    byte hunds = capacitance/100;
    byte tens = (capacitance/10)%10;
    byte units = capacitance%10;
    
//***************************************************************  
  
  do{
  // Escritura de las UNIDADES en el despliegue de siete segmentos
    digitalWrite (SelDisp[0],HIGH);
    digitalWrite (SelDisp[1],LOW);
    digitalWrite (SelDisp[2],LOW);
   
    for (pinActual = 0; pinActual < numDisp; pinActual++)
     {
     digitalWrite (inDisp[pinActual],dispOut[units][pinActual]);
     }
     
     delay (delayTime);

  //***************************************************************    
  // Escritura de las decenas en el despliegue de siete segmentos
    digitalWrite (SelDisp[0],LOW);
    digitalWrite (SelDisp[1],HIGH);
    digitalWrite (SelDisp[2],LOW);
   
    for (pinActual = 0; pinActual < numDisp; pinActual++)
     {
     digitalWrite (inDisp[pinActual],dispOut[tens][pinActual]);
     }
     
     delay (delayTime);
 //*************************************************************** 
  // Escritura de las CENTENAS en el despliegue de siete segmentos
      digitalWrite (SelDisp[0],LOW);
      digitalWrite (SelDisp[1],LOW);
      digitalWrite (SelDisp[2],HIGH);
   
    for (pinActual = 0; pinActual < numDisp; pinActual++)
     {
     digitalWrite (inDisp[pinActual],dispOut[hunds][pinActual]);
     }
     
     delay (delayTime);
  
  
  }
  while (1);


}
