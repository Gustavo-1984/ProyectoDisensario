#include "Nextion.h"
#include "thermalprinter.h"
#include "SoftwareSerial.h"


#define RX_PIN 2
#define TX_PIN 15

int pinA = 5;
int pinB = 18;
float Pos = 0;
float precio = 21.50;
float lts;
float vta;

int skinner = 4;
int contac = 0;
int folio = 0;
char fecha [] = "12/02/2020";
char hora [] = "12:00";
//SoftwareSerial mySerial(RX_PIN, TX_PIN);
Epson TM88 = Epson(RX_PIN, TX_PIN); 

unsigned long times;
unsigned long t;
bool C = true;
bool D = true;

NexText preset = NexText (1, 12, "t4_preset");
NexText litros  =  NexText(3, 12, "t4_vta");
NexText venta  =  NexText(3, 13, "t5_lts");
NexButton b0_parar =  NexButton(3, 14, "b0_parar");
NexButton b1_inicio = NexButton(3, 15, "b1_inicio");
NexButton b0_aceptar = NexButton (1, 13, "b0_aceptar");
NexPage page0    = NexPage(1, 0, "page0");
NexButton inicio  =  NexButton(0, 1, "bi");
NexPage page1    = NexPage(0, 0, "page1");
char buffer[10] = {0};


NexTouch *nex_listen_list[] =
{
  
  &inicio,
  &b1_inicio,
  &b0_aceptar,
  &b0_parar,
  &page0,
  &page1,
  
  NULL,
};

void presets(void *ptr){
  digitalWrite(contac, HIGH);
  delay(2000);
  digitalWrite(skinner, HIGH);
  memset(buffer, 0, sizeof(buffer));
  preset.getText(buffer, sizeof(buffer));
  float pst = atof(buffer);
  }
  
void iniciar(void *ptr){
 digitalWrite(contac, HIGH);
 delay(2000);
 digitalWrite(skinner, HIGH);

}

void parar(void *ptr){
  Pos = 0;
  page0.show();
  delay (2000);  
}

void setup() {
  nexInit(); 
  Serial.begin(9600);
 
  
  TM88.start();
  pinMode(pinA, INPUT);
  pinMode(pinB, INPUT);
  pinMode(skinner, OUTPUT);
  pinMode(contac, OUTPUT);
  b0_parar.attachPop(impresion, &b0_parar);
  b1_inicio.attachPop(parar, &b1_inicio);
  b0_aceptar.attachPop(presets, &b0_aceptar);
  

      
//   String ventas = "";
//
// ventas += String(folio);
// ventas += ";";
// ventas += String(fecha);
// ventas += ";";
// ventas += String(hora);
// ventas += ";";
// ventas += String(lts);
// ventas += ";";
// ventas += String(vta);
// ventas += ";";
 
}

void loop() {

  nexLoop(nex_listen_list);

  times = millis();

  if(digitalRead(pinA) == LOW){
    t = times;
    if(C == true){
      Pos++;
        float lts = (Pos * .101);
        float vta = (lts * precio);
        char conversion[10];
        char conversion2[10];
        dtostrf(lts, 8, 2, conversion); 
        dtostrf(vta, 8, 2, conversion2); 
        litros.setText(conversion);
        venta.setText(conversion2);
        C = false;
        D = false;
    }
  }

  if(digitalRead(pinB) == LOW){
    t = times;

    if(D == true){
      Pos++;
        float lts = (Pos * .101);
        float vta = (lts * precio);
        char conversion[10];
        char conversion2[10];
        dtostrf(lts, 8, 2, conversion); 
        dtostrf(vta, 8, 2, conversion2); 
        litros.setText(conversion);
        venta.setText(conversion2);
        D= false;
        C= false;
    }
  }

  if (times-t>5){
    C = true;
    D = true;
  }
 
}



void impresion(void *ptr){ 
  digitalWrite(contac, LOW);
  digitalWrite(skinner, LOW);
  delay(3000);
  memset(buffer, 0, sizeof(buffer));
  litros.getText(buffer, sizeof(buffer));
  float lts = atof(buffer);
  memset(buffer, 0, sizeof(buffer));
  venta.getText(buffer, sizeof(buffer));
  float vta = atof(buffer);
  folio ++;
  TM88.justifyCenter();
  TM88.doubleHeightOn();
  TM88.println("Grupo Petrolero Arca"); 
  TM88.doubleHeightOff();
  TM88.justifyCenter();
  TM88.println("Grupo Petrolero Arca S.A. de C.V.");
  TM88.println("========================================");
  TM88.feed(2);
  TM88.justifyLeft();
  TM88.print("Fecha: ");
  TM88.print(fecha);
  TM88.println();
  TM88.print("Hora: ");
  TM88.print(hora);
  TM88.println();
  TM88.print("Folio:   ");
  TM88.print(folio);
  TM88.feed(2);
  TM88.println("==========================================");
  TM88.feed(2);
  TM88.print("Litros...    ");
  TM88.print(lts);
  TM88.println();
  TM88.print("Venta....   $");
  TM88.print(vta);
  TM88.println();
  TM88.println("$*Litro: 21.50");
  TM88.feed(2);
  TM88.println("==========================================");
  TM88.feed(2);
  TM88.justifyCenter();
  TM88.doubleHeightOn();
  TM88.println("Gracias por su preferencia !!");
  TM88.doubleHeightOff();
  TM88.feed(2);
  TM88.cut(); 
}
