// Libs
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// definições pinos ponte H
#define IN1 5
#define IN2 4
#define IN3 0
#define IN4 2

// Vars
const char* SSID = "PASID-VIRTEX"; // rede wifi
const char* PASSWORD = "reidomel2"; // senha da rede wifi

const char* BROKER_MQTT = "m24.cloudmqtt.com"; // ip/host do broker
int BROKER_PORT =   13278; // porta do broker

// prototypes
void initPins();

void initSerial();
void initWiFi();
void initMQTT();
void frente();
void para();
void viraresquerda();
void virardireita();

WiFiClient espClient;
PubSubClient MQTT(espClient); // instancia o mqtt

// setup
void setup() {
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW); 
  
  initPins();
  initSerial();
  initWiFi();
  initMQTT();
}

void loop() {
  
  if (!MQTT.connected()) {
    Serial.println("MQTT Reconnect");
    reconnectMQTT();
  }

  
  recconectWiFi();
  MQTT.loop();
}

// implementacao dos prototypes

void initPins() {
  //pinMode(D5, OUTPUT);
  //digitalWrite(D5, 0);
}

void initSerial() {
  Serial.begin(115200);
}
void initWiFi() {
  delay(10);
  Serial.println("Conectando-se em: " + String(SSID));

  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Conectado na Rede " + String(SSID) + " | IP => ");
  Serial.println(WiFi.localIP());
}

// Funcão para se conectar ao Broker MQTT
void initMQTT() {
  MQTT.setServer(BROKER_MQTT, BROKER_PORT);
  MQTT.setCallback(mqtt_callback);
}

//Função que recebe as mensagens publicadas
void mqtt_callback(char* topic, byte* payload, unsigned int length) {

  String message;
  for (int i = 0; i < length; i++) {
    char c = (char)payload[i];
    message += c;
  }
  Serial.println("Topico => " + String(topic) + " | Valor => " + String(message));
  if(message == "0"){
    para();
  }
  if(message == "1"){
    frente();
  }
  if(message == "2"){
    viraresquerda(); 
  }
  if(message == "3"){
    virardireita();
  }
  Serial.flush();
}

void reconnectMQTT() {
  while (!MQTT.connected()) {
    Serial.println("Tentando se conectar ao Broker MQTT: " + String(BROKER_MQTT));
    if (MQTT.connect("teste02", "zrupqoay", "nloxRPMtkJIb")) {
      Serial.println("Conectado");
      MQTT.subscribe("motor");

    } else {
      Serial.println("Falha ao Reconectar");
      Serial.println("Tentando se reconectar em 2 segundos");
      delay(2000);
    }
  }
}

void recconectWiFi() {
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
}

//função para ir para frente
void frente(){
  Serial.println("Frente...");
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

// função para parar
void para(){
  Serial.println("Parando...");
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

// função para virar a esquerda
void viraresquerda(){
  delay(100);
  Serial.println("Esquerda...");
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  delay(350);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
 
// função para virar a direita
void virardireita(){
  delay(100);
  Serial.println("Direita...");
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  delay(350);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
