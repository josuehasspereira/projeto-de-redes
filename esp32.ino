/*
Adaptado de:
ESP32 BLE Scanner Example. Disponível em: <
https://github.com/nkolban/ESP32_BLE_Arduino/tree/master/examples/BLE_scan >

Artigo Referência: Indoor Localization in BLE using Mean and Median Filtered RSSI Values - (Venkatesh.R, Vikas Mittal, Hrudaya Tammana)
*/

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
const char* mqtt_server = "broker.emqx.io"; // Usando Servidor público
const int mqtt_port = 1883;
const char* mqtt_topic = "localizacao"; //Usar mesmo topico para transmitir mensagens mqtt

// Inicialização do Cliente
WiFiClient espClient;
PubSubClient client(espClient);

BLEScan* pBLEScan;
int numDispositivos = 0;

// Variáveis do modelo de perda de caminho livre na propagação de sinais de rádio
const int rssi_ref = -69; 
const float N = 2; 

//Funções para envio de mensagem do mqtt e cálculo da distância
float calcularDistancia(int rssi);
void enviarMensagemMQTT(int numDispositivos, const char* name, const char* address, int rssi, float distancia);

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
        numDispositivos++;
        std::string name = advertisedDevice.getName();
        std::string address = advertisedDevice.getAddress().toString();
        int rssi = advertisedDevice.getRSSI();
        float distancia = calcularDistancia(rssi);

        // Exibe informações do dispositivo
        Serial.printf("Advertised Device \nName: %s \nAddress: %s \nrssi: %d\n", name.c_str(), address.c_str(), rssi);
        Serial.print("Distância: ");
        Serial.println(distancia);
        // Chama a função para enviar a mensangem MQTT - Exibida no celular
        enviarMensagemMQTT(numDispositivos, name.c_str(), address.c_str(), rssi, distancia);
    }
};

void setup() {
  Serial.begin(115200);

  // Faz a Conexão para o WIFI
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Conectando ao WiFi...");
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Conectado ao Wifi Com Sucesso!");
  }

  // Faz a Conexão MQTT usando o servidor broker.emqx.io
  client.setServer(mqtt_server, mqtt_port);
  while (!client.connected()) {
    Serial.println("Conectando ao servidor MQTT...");
    if (client.connect("")) {
      Serial.println("Conectado ao servidor MQTT\n");
      
      // Exibe as infos de config
      char infoMessage[256];
      snprintf(infoMessage, 256, "IP do ESP32: %s \nIP do broker: %s \nPorta do broker: %d", 
               WiFi.localIP().toString().c_str(), mqtt_server, mqtt_port);
      client.publish(mqtt_topic, infoMessage);
    } else {
      Serial.print("Falha na conexão com o servidor MQTT, rc=");
      Serial.print(client.state());
      Serial.println(" Tente novamente em 5 segundos");
      delay(5000);
    }
  }

  Serial.print("-----------------------------------------\n");
  Serial.print("IP do ESP32: ");
  Serial.println(WiFi.localIP());
  Serial.print("IP do broker: ");
  Serial.println(mqtt_server);
  Serial.print("Porta do broker: ");
  Serial.println(mqtt_port);
  Serial.print("-----------------------------------------\n\n");

  // Conexão BLE
  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); 
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setInterval(100);
  pBLEScan->setActiveScan(true);
}

void loop() {
  numDispositivos = 0; // Reinicia o contador de dispositivos a cada loop
  pBLEScan->start(1); // Não é necessário armazenar o resultado do escaneamento
  delay(2000); // Tempo de loop do scanner

  // Exibe a quantidade de dispositivos encontrados
  Serial.printf("Quantidade de dispositivos encontrados: %d\n", numDispositivos);
  Serial.print("-----------------------------------------\n\n");
}

// Calcula a distância dos dispositivos próximos usando a fórmula do modelo de caminho livre
float calcularDistancia(int rssi) {
  float distancia = pow(10, ((rssi_ref - rssi) / (10.0 * N)));
  return distancia;
}

// Envia a mensagem MQTT
void enviarMensagemMQTT(int numDispositivos, const char* name, const char* address, int rssi, float distancia) {
  char message[256];
  snprintf(message, 256, "Dispositivos: %d Nome: %s MAC: %s RSSI: %d Distância: %.2f", 
           numDispositivos, name, address, rssi, distancia);
  client.publish(mqtt_topic, message);
}
