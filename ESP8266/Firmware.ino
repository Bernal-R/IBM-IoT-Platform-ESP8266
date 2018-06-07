/*
Código elaborado por Bernal Rojas.
Conectar un esp8266 a la plataforma IBM IoT https://www.ibm.com/internet-of-things.
Utiliza publish PubSubClient para conectarse al API que genera desde la plataforma.
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h> // https://github.com/knolleary/pubsubclient/releases/tag/v2.3
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson/releases/tag/v5.0.7

//-------- Variables customizables-----------
const char* ssid = "ssid";      //Cambiar por el ssid
const char* password = "password";  //Cambiar por el password

#define ORG "org"           //Cambiar por el org
#define DEVICE_TYPE "type"  //Cambiar por el type
#define DEVICE_ID "id"      //Cambiar por el id
#define TOKEN "token"       //Cambiar por el token


//-------- Varibles para Bluemix --------
char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;

const char eventTopic[] = "iot-2/evt/status/fmt/json";
const char cmdTopic[] = "iot-2/cmd/led/fmt/json";


//-------- Varibles para Wifi --------
WiFiClient wifiClient;

//-------- Varibles para PubSubClient --------
PubSubClient client(server, 1883, callback, wifiClient);
int publishInterval = 1000; // Intervalo para envio de datos al cloud. Sustituir por el que se ajuste a su necesidad
long lastPublishMillis;


//-------- Función para Callback --------
void callback(char* topic, byte* payload, unsigned int payloadLength) { //Recibe un Json
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < payloadLength; i++) { //Lee el Json
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Pregunta sobre el mensaje en el Json. 1 para encender, 0 para apagar
  if ((char)payload[7] == '1') {
    digitalWrite(BUILTIN_LED, HIGH);   // Enciende  el LED
  } else if ((char)payload[7] == '0') {
    digitalWrite(BUILTIN_LED, LOW);  // Apaga el LED
  }

}

//-------- Función Set Up --------
void setup() {
  Serial.begin(115200); Serial.println();
  pinMode(LED_BUILTIN, OUTPUT);
  wifiConnect();
  mqttConnect();
}

//-------- Función Loop --------
void loop() {
  //Envia datos cada 1 segundo
  if (millis() - lastPublishMillis > publishInterval) {
    publishData();
    lastPublishMillis = millis();
  }

  //Revisa conección
  if (!client.loop()) {
    mqttConnect();
  }

}

  //-------- Función para Wifi --------
void wifiConnect() {
  Serial.print("Connecting to "); Serial.print(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("nWiFi connected, IP address: "); Serial.println(WiFi.localIP());

}

//-------- Función para protocolo MQTT --------
void mqttConnect() {
  if (!!!client.connected()) {
    Serial.print("Reconnecting MQTT client to "); Serial.println(server);
    while (!!!client.connect(clientId, authMethod, token)) {
      Serial.print(".");
      delay(500);
    }
    if (client.subscribe(cmdTopic)) {
      Serial.println("subscribe to responses OK");
    } else {
      Serial.println("subscribe to responses FAILED");
    }
    Serial.println();
  }
}


//-------- Función para publicar datos en el IBM IoT Platform --------
void publishData() {
  int sensorValue = analogRead(A0); //Sustituir por el valor que desea enviar
  String payload = "{\"d\":{\"adc\":";
  payload += String(sensorValue, DEC);
  payload += "}}";
  Serial.print("Sending payload: "); Serial.println(payload);
  if (client.publish(eventTopic, (char*) payload.c_str())) {
    Serial.println("Publish OK");
  } else {
    Serial.println("Publish FAILED");
  }
}
