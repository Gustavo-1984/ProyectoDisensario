#include <WiFi.h>
#include <PubSubClient.h>
#include "SPI.h"
#include <Nextion.h>
#include "Adafruit_Thermal.h"
#include "SoftwareSerial.h"

const String serial_number = "Disp-01";

#define RXD2 16
#define TXD2 17
#define TX_PIN 6 
#define RX_PIN 5 

TaskHandle_t Task1;

const char* ssid = "HUAWEI P smart";
const char* password = "12345678";

//**************************
//*** CONFIGURACION MQTT ***
//**************************

const char *mqtt_server = "dispensario.ga";
const int   mqtt_port = 1883;
const char *mqtt_user = "";
const char *mqtt_pass = "121212";

WiFiClient espClient;
PubSubClient client(espClient);

long lastMsg = 0;
char msg[25];
bool send_access_query = false;

String rfid = "";

String user_name = "";

//********************************
//*** DECLARACION DE FUNCIONES ***
//********************************
void setup_wifi();
void callback(char* topic, byte* payload, unsigned int length);
void reconnect();

//**********************
//*** TAREA NUCLEO 2 ***
//**********************

void codeForTask1(void *parameter){
  for(;;){
    while(Serial2.available()){
      rfid += char(Serial2.read());
      }
      if(rfid!=""){
        Serial.println("Leyendo tag -> " +rfid);
        send_access_query = true;
        }
      vTaskDelay(10);
    }
  }

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  randomSeed(micros());
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  xTaskCreatePinnedToCore(
    codeForTask1,
    "Task_1",
    1000,
    NULL,
    1,
    &Task1,
    0);
}

void loop() {
  if(!client.connected()){
    reconnect();
    }

client.loop();

long now = millis();
  if(now - lastMsg > 2000){
    lastMsg = now;
    char topic[25];
    String topic_aux = serial_number;
    topic_aux.toCharArray(topic,25);
    client.publish(topic, msg);

    
    }

    if(send_access_query){
      String to_send = rfid;
      to_send.toCharArray(msg, 25);
      char topic[25];
      String topic_aux = serial_number + "/access_query";
      topic_aux.toCharArray(topic, 25);
      client.publish(topic, msg);
      rfid =""; 
      send_access_query = false;
      }
}

//*********************
//*** CONEXION WIFI ***
//*********************

void setup_wifi(){
  delay(10);

  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
    }

    Serial.println("");
    Serial.println("Conectado a red WiFi!");
    Serial.println("Direccion IP: ");
    Serial.println(WiFi.localIP());
  }
//****************
//*** DESPACHO ***
//****************
void despacho(){
 
}

  
//****************
//*** CALLBACK ***
//****************
void callback(char* topic, byte* payload, unsigned int length){
  String incoming = "";
  Serial.print(topic);
  Serial.println("Mensaje recibido desde -> ");
    for(int i = 0; i < length; i++){
      incoming += (char)payload[i];
      }
      
      incoming.trim();
      Serial.println("Mensaje -> " + incoming);

      String str_topic = (topic);

      if(str_topic == serial_number + "/command"){

          if(incoming == "despacho"){
            
            }
          if(incoming == "close"){
            
            }
          if(incoming == "granted"){
            
            }
        }
  }

//******************
//*** RECONECTAR ***
//******************

  void reconnect(){
    while(!client.connected()){
      Serial.print("Intentando conexion Mqtt...");
      //Creamos client ID
      String clientId = "esp32_";
      clientId += String(random(0xfff), HEX);
      //Intentamos conectar
      if(client.connect(clientId.c_str(),mqtt_user,mqtt_pass)){
        Serial.println("Conectado!");
        //Nos Suscribimos
        char topic[25];
        String topic_aux = serial_number + "/command";
        topic_aux.toCharArray(topic,25);
        client.subscribe(topic);

        char topic[25];
        String topic_aux2 = serial_number + "/user_name";
        topic_aux2.toCharArray(topic2,25);
        client.subscribe(topic2);
        }else{
          Serial.print("fallo :( con error ->");
          Serial.print(client.state());
          Serial.println("Intentamos de nuevo 5 segundos");

          delay(5000);
          }
      }
    }
