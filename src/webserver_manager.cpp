#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

#include "webserver_manager.h"
#include "relay_manager.h"
#include "config.h"

WebServer server(80);

// prevents false interrupt after web control
extern unsigned long ignoreInterruptUntil;

// =====================================================
// HTML
// =====================================================
String getHTML() {

return R"rawliteral(

<!DOCTYPE html>
<html>

<head>

<meta name="viewport" content="width=device-width, initial-scale=1">

<title>ESP32 Relay</title>

<style>

body{
  background:#121212;
  color:white;
  font-family:Arial;
  text-align:center;
  margin-top:50px;
}

.card{
  background:#1f1f1f;
  width:320px;
  margin:auto;
  padding:30px;
  border-radius:20px;
}

button{
  width:120px;
  height:55px;
  font-size:20px;
  border:none;
  border-radius:12px;
  margin:10px;
  cursor:pointer;
}

.on{
  background:green;
  color:white;
}

.off{
  background:red;
  color:white;
}

#status{
  font-size:30px;
  margin:20px;
}

</style>

</head>

<body>

<div class="card">

<h1>ESP32 Relay</h1>

<div id="status">Loading...</div>

<button class="on" onclick="setRelay(1)">
ON
</button>

<button class="off" onclick="setRelay(0)">
OFF
</button>

</div>

<script>

function setRelay(state){

  fetch("/relay?state=" + state)
  .then(response => response.text())
  .then(data => {

    updateStatus();

  });

}

function updateStatus(){

  fetch("/status")
  .then(response => response.text())
  .then(data => {

    if(data == "1"){
      document.getElementById("status").innerHTML =
      "Relay ON";
    }
    else{
      document.getElementById("status").innerHTML =
      "Relay OFF";
    }

  });

}

setInterval(updateStatus, 500);

updateStatus();

</script>

</body>
</html>

)rawliteral";

}

// =====================================================
// ROUTES
// =====================================================

void handleRoot() {

    server.send(200, "text/html", getHTML());
}

void handleRelayRoute() {

    if(server.hasArg("state")) {

        String state = server.arg("state");

        if(state == "1") {
            setRelay(true);
        }
        else {
            setRelay(false);
        }

        ignoreInterruptUntil = millis() + 500;
    }

    server.send(200, "text/plain", "OK");
}

void handleStatus() {

    server.send(
        200,
        "text/plain",
        getRelayState() ? "1" : "0"
    );
}

// =====================================================
// INIT
// =====================================================

void initWebServer() {

    WiFi.softAP(AP_SSID, AP_PASSWORD);

    Serial.println("AP Started");

    Serial.println(WiFi.softAPIP());

    server.on("/", handleRoot);

    server.on("/relay", handleRelayRoute);

    server.on("/status", handleStatus);

    server.begin();

    Serial.println("WebServer Started");
}

// =====================================================
// LOOP
// =====================================================

void handleWebServer() {

    server.handleClient();
}