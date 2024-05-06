# Localizador de Objetos com ESP32, Bluetooth Low Energy e MQTT

Este projeto implementa um localizador de objetos em uma distância curta de 10 metros usando Bluetooth Low Energy (BLE) com monitoramento IoT e o protocolo MQTT. O projeto é desenvolvido para o uso do microcontrolador ESP32 e utiliza o Arduino IDE para programação do código e sua implementação.

## Pré-requisitos

- Arduino IDE instalado no seu computador.
- Conexão estável com a Internet.
- Um microcontrolador ESP32 com capacidade BLE.
- Acesso a um servidor MQTT (você pode usar um serviço online ou configurar um servidor local).

## Configuração do Projeto

1. Clone este repositório para o seu computador:
Crie uma pasta onde desejar rodar o projeto. Após isso abra essa pasta através do CMD e use o seguinte comando:
```cpp
git clone https://github.com/seu-usuario/nome-do-repositorio.git
```

3. Abra o arquivo `esp.ino` no Arduino IDE.

4. Certifique-se de ter instalado as bibliotecas necessárias:
   - `ArduinoMqttClient de Arduino`: Para permitir que os usuários usem recursos de rede como rest e mqtt.
   - `PubSubClient de Nick O'Leary`: Para comunicação MQTT.

   Você pode instalar essas bibliotecas no Arduino IDE indo em `Sketch -> Incluir Biblioteca -> Gerenciar Bibliotecas`, e pesquisando pelos nomes das bibliotecas.

5. Configure as credenciais do seu WiFi e do seu servidor MQTT no código, nas seguintes linhas:

```cpp
const char* ssid = "SEU_SSID";
const char* password = "SUA_SENHA_WIFI";
const char* mqtt_server = "BROKER_MQTT";
const char* mqtt_user = "USUARIO_MQTT";
const char* mqtt_password = "SENHA_MQTT";
``` 

## EM BREVE
