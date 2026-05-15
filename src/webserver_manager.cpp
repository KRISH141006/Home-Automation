#include <WebServer.h>

#include "webserver_manager.h"
#include "relay_manager.h"

WebServer server(80);

const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <title>SmartGruh - Local</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        body { font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; background-color: #f1f5f9; margin: 0; display: flex; flex-direction: column; align-items: center; justify-content: center; height: 100vh; }
        .card { background: white; padding: 40px; border-radius: 30px; box-shadow: 0 10px 25px rgba(0,0,0,0.1); text-align: center; border: 8px solid #cbd5e1; }
        h1 { color: #0f172a; margin-bottom: 30px; }
        .switch-btn { width: 80px; height: 140px; border-radius: 20px; background: #9ca3af; cursor: pointer; display: inline-block; position: relative; transition: 0.3s; box-shadow: inset 0 5px 10px rgba(255,255,255,0.8), 0 4px 8px rgba(0,0,0,0.1); }
        .switch-btn.active { background: #22c55e; box-shadow: 0 0 20px rgba(34,197,94,0.6); }
        .icon { font-size: 50px; margin-top: 20px; }
        .label { margin-top: 15px; font-weight: bold; color: #475569; }
    </style>
</head>
<body>
    <div class="card">
        <h1>SmartGruh Local</h1>
        <div id="btn" class="switch-btn" onclick="toggle()"></div>
        <div class="icon">💡</div>
        <div class="label">BULB</div>
    </div>
    <script>
        function updateUI(state) {
            const btn = document.getElementById('btn');
            if(state == 'ON') btn.classList.add('active');
            else btn.classList.remove('active');
        }
        function toggle() {
            fetch('/toggle').then(r => r.text()).then(state => updateUI(state));
        }
        setInterval(() => {
            fetch('/status').then(r => r.text()).then(state => updateUI(state));
        }, 2000);
        toggle(); // Initial state fetch
    </script>
</body>
</html>
)rawliteral";

void setupWebServer() {

    server.on("/", []() {
        server.send(200, "text/html", INDEX_HTML);
    });

    server.on("/toggle", []() {
        toggleRelay();
        server.send(200, "text/plain", getRelayState() ? "ON" : "OFF");
    });

    server.on("/status", []() {
        server.send(200, "text/plain", getRelayState() ? "ON" : "OFF");
    });

    server.begin();
}

void handleWebServer() {
    server.handleClient();
}
