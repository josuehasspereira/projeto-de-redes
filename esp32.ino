#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <math.h> // pow().

// Variáveis para WIFI e MQTT
const char* ssid = "CLARO_2G590AE4";
const char* password = "g1T#4bR3_p(";
const char* mqtt_server = "broker.emqx.io"; // Alterado para o servidor MQTT público
const int mqtt_port = 1883;
const char* mqtt_topic = "localizacao";

// Inicialização do Cliente
WiFiClient espClient;
PubSubClient client(espClient);

BLEScan* pBLEScan;

// Variáveis do modelo de perda de caminho livre na propagação de sinais de rádio
const int rssi_ref = -69; 
const float N = 2.0; 

// Declaração das funções para envio de mensagem do mqtt e calculo da distanca
float calcularDistancia(int rssi);
void enviarMensagemMQTT(int numDispositivos, float distancia);

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
        std::string name = advertisedDevice.getName();
        if (name.empty()) {
            name = "N/A"; // Nome não disponível
        }
        Serial.printf("Advertised Device: Name: %s, Address: %s, rssi: %d\n", name.c_str(), advertisedDevice.getAddress().toString().c_str(), advertisedDevice.getRSSI());
        float distancia = calcularDistancia(advertisedDevice.getRSSI());
        enviarMensagemMQTT(1, distancia);
        Serial.print("Distância: ");
        Serial.println(distancia);
    }
};


void setup() {
  Serial.begin(115200);

  // Conexão para o WIFI
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Conectando ao WiFi...");
  }

  // Faz a Conexão MQTT usando o servidor broker.emqx.io
  client.setServer(mqtt_server, mqtt_port);
  while (!client.connected()) {
    Serial.println("Conectando ao servidor MQTT...");
    if (client.connect("")) {
      Serial.println("Conectado ao servidor MQTT");
    } else {
      Serial.print("Falha na conexão com o servidor MQTT, rc=");
      Serial.print(client.state());
      Serial.println(" Tente novamente em 5 segundos");
      delay(5000);
    }
  }

  // Conexão BLE
  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); 
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setInterval(100);
  pBLEScan->setActiveScan(true);
}

void loop() {
  pBLEScan->start(1); // Não é necessário armazenar o resultado do escaneamento

  delay(2000); // Tempo de loop do scanner
}

// Calcula a distância dos dispositivos próximos usando a fórmula do modelo de caminho livre
float calcularDistancia(int rssi) {
  float distancia = pow(10, ((rssi_ref - rssi) / (10.0 * N)));
  return distancia;
}

//Envia a mensagen mqtt 
void enviarMensagemMQTT(int numDispositivos, float distancia) {
  char message[100];
  snprintf(message, 100, "{\"num_dispositivos\": %d, \"distancia\": %.2f}", numDispositivos, distancia);
  client.publish(mqtt_topic, message);
}
