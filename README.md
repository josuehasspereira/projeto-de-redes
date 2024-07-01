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

2. Abra o arquivo `esp.ino` no Arduino IDE.

3. Certifique-se de ter instalado as bibliotecas necessárias:
   - `ArduinoMqttClient de Arduino`: Para permitir que os usuários usem recursos de rede como rest e mqtt.
   - `PubSubClient de Nick O'Leary`: Para comunicação MQTT.

   Você pode instalar essas bibliotecas no Arduino IDE indo em `Sketch -> Incluir Biblioteca -> Gerenciar Bibliotecas`, e pesquisando pelos nomes das bibliotecas.

4. Configuração de Ferramentas:
   - Vá em `Ferramentas -> Upload Speed` e certifique-se que está configurado para `115200`, caso não esteja, altere para o mesmo!
   - Vá em `Ferramentas -> Partition Scheme` e altere para a opção `HugeApp`
   - Selecione a placa `ESP32 Dev Module`
   

5. Configure as credenciais do seu WiFi e do seu servidor MQTT no código, nas seguintes linhas:

```cpp
const char* ssid = "SEU_SSID";
const char* password = "SUA_SENHA_WIFI";
const char* mqtt_server = "BROKER_MQTT";
const char* mqtt_user = "USUARIO_MQTT";
const char* mqtt_password = "SENHA_MQTT";
``` 

6. Publicação das mensagens MQTT.
   - O código está configurado para enviar informações sobre dispositivos BLE encontrados para um tópico MQTT
   - Você visualizar essas informações através do app Iot MQTT Panel, disponível para Android e IOS
   - Baixe e instale, após isso, crie uma nova conexão, em `Connection Name` e `Client ID`, utilize o que for melhor para você, em `Broker Web/IP...`, utilize `broker.emqx.io`, crie um dashboard com qualquer nome e o resto deixe como está!.
   - Após criar a conexão com dashboard, entre nela e adicione o painel chamado `Text Log`, e após isso sua conexão está pronta para receber as mensagens MQTT.

7. Rodar o Código.
   - Clique no botão de upload no Arduino IDE para carregar o código no ESP32.
   - Durante o processo de upload, mantenha pressionado o botão de boot no ESP32 até que o upload esteja completo e o esboço comece a ser executado.
   - Após o upload bem-sucedido, abra o monitor serial no Arduino IDE `(Ferramentas -> Monitor Serial` ou pressione `Ctrl + Shift + M`) e certifique-se de que a taxa de transmissão está configurada para 115200 bps.
  
8. Monitorar as mensagens MQTT.
  - No aplicativo IoT MQTT Panel, que você configurou anteriormente, conecte-se ao broker MQTT.
  - No painel `Text Log`, você deverá ver as mensagens publicadas pelo ESP32, incluindo informações sobre os dispositivos BLE encontrados, como nome, endereço MAC, RSSI e distância calculada.

## Conclusão

Este projeto permite monitorar dispositivos BLE em tempo real e enviar essas informações para um servidor MQTT. Você pode visualizar os dados usando um cliente MQTT no seu computador ou dispositivo móvel. Certifique-se de seguir todos os passos de configuração e de ter as bibliotecas necessárias instaladas no Arduino IDE.

