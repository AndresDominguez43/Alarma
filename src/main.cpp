#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ESPmDNS.h>

// Replace with your network credentials
const char* ssid = "Internet_Services_1418";
const char* password = "meia242113";

// NTP client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", -10800); // UTC-3 timezone

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

void notifyClients() {
  ws.textAll(timeClient.getFormattedTime());
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    if (strcmp((char*)data, "toggle") == 0) {
      notifyClients();
    }
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void handleRoot(AsyncWebServerRequest *request) {
  String content = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>Hora Actual</title>
</head>
<body>
  <p>Fecha: <span id='fecha'></span></p>
  <p>Hora: <span id='hora'></span></p>
  <p>Ajustar hora (HH:MM:SS): 
    <input type="text" id="targetTime"/>
    <button onclick="setTargetTime()">Set</button></p>
  <script>
    var gateway = `ws://${window.location.hostname}/ws`;
    var websocket;
    function initWebSocket() {
      console.log('Trying to open a WebSocket connection...');
      websocket = new WebSocket(gateway);
      websocket.onopen    = onOpen;
      websocket.onclose   = onClose;
      websocket.onmessage = onMessage;
    }
    function onOpen(event) {
      console.log('Connection opened');
      websocket.send('toggle');
    }
    function onClose(event) {
      console.log('Connection closed');
      setTimeout(initWebSocket, 2000);
    }
    function onMessage(event) {
      var date = new Date();
      document.getElementById('fecha').innerHTML = date.toLocaleDateString();
      document.getElementById('hora').innerHTML = event.data;
    }
    function setTargetTime(){
      var targetTime = document.getElementById('targetTime').value;
      var xhttp = new XMLHttpRequest();
      xhttp.open("POST", "/set-time", true);
      xhttp.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
      xhttp.send("time=" + targetTime);
    }
    window.onload = function(event) {
      initWebSocket();
    }
  </script>
</body>
</html>
)rawliteral";
  request->send(200, "text/html", content);
}

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

bool ledState = 0;
const int lightPin = 2;

String targetTime = "00:00";

void handleSetTime(AsyncWebServerRequest *request) {
  if (request->hasParam("time", true)) {
    targetTime = request->getParam("time", true)->value();
    Serial.println("Time set to: " + targetTime); // Mostrar el valor de targetTime en el monitor serial
    request->send(200, "text/plain", "Time set to: " + targetTime);
  } else {
    request->send(400, "text/plain", "Time parameter missing");
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(lightPin, OUTPUT);
  digitalWrite(lightPin, LOW);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  if (!MDNS.begin("Alarma")) {  // http://alarma.local
    Serial.println("Error setting up MDNS responder!");
  } else {
    Serial.println("mDNS responder started");
  }

  server.on("/", HTTP_GET, handleRoot);
  server.on("/set-time", HTTP_POST, handleSetTime);

  initWebSocket();
  server.begin();
  timeClient.begin();
}

void loop() {
  ws.cleanupClients();
  timeClient.update();
  String currentTime = timeClient.getFormattedTime();

  if (targetTime != "" && currentTime == targetTime) {
    digitalWrite(lightPin, HIGH);
  } else {
    digitalWrite(lightPin, LOW);
  }
  
  notifyClients();
  delay(1000); 
}
