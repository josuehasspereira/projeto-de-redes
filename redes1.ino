#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <PubSubClient.h>
#include <WiFi.h>

// WiFi e MQTT
const char* ssid = "CLARO_2G590AE4";
const char* password = "g1T#4bR3_p(";
const char* mqtt_server = "179.222.233.198";
const int mqtt_port = 1883;
const char* mqtt_user = "";
const char* mqtt_password = "";
const char* mqtt_topic = "localizacao";

WiFiClient espClient;
PubSubClient client(espClient);

// BLE
BLEScan* pBLEScan;
int numDispositivos = 0;
float distanciaTotal = 0;

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
        Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
    }
};

void setup() {
  Serial.begin(115200);

  // Inicialização WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Conectando ao WiFi...");
  }

  // Inicialização MQTT
  client.setServer(mqtt_server, mqtt_port);
  while (!client.connected()) {
    Serial.println("Conectando ao servidor MQTT...");
    if (client.connect("", "", "")) {
      Serial.println("Conectado ao servidor MQTT");
    } else {
      Serial.print("Falha na conexão com o servidor MQTT, rc=");
      Serial.print(client.state());
      Serial.println(" Tente novamente em 5 segundos");
      delay(5000);
    }
  }

  // Inicialização BLE
  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); 
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setInterval(100);
  pBLEScan->setActiveScan(true);
}

void loop() {
  BLEScanResults foundDevices = pBLEScan->start(1);

  for (int i = 0; i < foundDevices.getCount(); i++) {
    BLEAdvertisedDevice device = foundDevices.getDevice(i);
    int rssi = device.getRSSI();
    distanciaTotal += calcularDistancia(rssi);
    numDispositivos++;
  }

  if (numDispositivos > 0) {
    float distanciaMedia = distanciaTotal / numDispositivos;
    enviarMensagemMQTT(numDispositivos, distanciaMedia);
    Serial.println("Mensagem MQTT enviada:");
    Serial.print("Número de dispositivos: ");
    Serial.println(numDispositivos);
    Serial.print("Distância média: ");
    Serial.println(distanciaMedia);
  }

  pBLEScan->clearResults();
  delay(2000);
}

float calcularDistancia(int rssi) {
  // Implemente seu cálculo de distância aqui
  // Este é apenas um exemplo simples
  float distancia = 10 - (rssi / 10.0); // Simulação de um cálculo de distância
  return distancia;
}

void enviarMensagemMQTT(int numDispositivos, float distanciaMedia) {
  char message[100];
  snprintf(message, 100, "{\"num_dispositivos\": %d, \"distancia_media\": %.2f}", numDispositivos, distanciaMedia);
  client.publish(mqtt_topic, message);
}
