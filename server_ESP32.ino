/**
 * Exemple de serveur web ESP32
 * Ce programme crée un point d'accès WiFi et un serveur web simple
 * permettant de contrôler une LED et d'afficher des données de capteur
 */

#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>

// Définition des constantes
#define LED_PIN 2  // LED intégrée de l'ESP32
#define ANALOG_SENSOR_PIN 34  // Exemple avec un capteur analogique

// Configuration du point d'accès WiFi
const char* ssid = "ESP32_AP";
const char* password = "esp32password";

// Création d'une instance de serveur web sur le port 80
WebServer server(80);

// Variables pour stocker l'état
bool ledState = false;
unsigned long lastUpdateTime = 0;
const long updateInterval = 5000;  // Intervalle de mise à jour en ms

// Fonction d'initialisation
void setup() {
  // Initialisation de la communication série
  Serial.begin(115200);
  Serial.println("Démarrage du serveur ESP32...");
  
  // Configuration des broches
  pinMode(LED_PIN, OUTPUT);
  pinMode(ANALOG_SENSOR_PIN, INPUT);
  
  // Initialisation de la LED (éteinte au démarrage)
  digitalWrite(LED_PIN, LOW);
  
  // Configuration du point d'accès WiFi
  WiFi.softAP(ssid, password);
  
  IPAddress IP = WiFi.softAPIP();
  Serial.print("Adresse IP du point d'accès: ");
  Serial.println(IP);
  
  // Configuration des routes du serveur
  server.on("/", HTTP_GET, handleRoot);
  server.on("/led", HTTP_POST, handleLED);
  server.on("/sensor", HTTP_GET, handleSensor);
  server.on("/api/data", HTTP_GET, handleAPIData);
  
  // Gestionnaire pour les ressources non trouvées
  server.onNotFound(handleNotFound);
  
  // Démarrage du serveur
  server.begin();
  Serial.println("Serveur HTTP démarré");
}

// Boucle principale
void loop() {
  // Gestion des requêtes client
  server.handleClient();
  
  // Mise à jour périodique des données (si nécessaire)
  unsigned long currentTime = millis();
  if (currentTime - lastUpdateTime >= updateInterval) {
    lastUpdateTime = currentTime;
    // Ici vous pouvez mettre à jour des variables, lire des capteurs périodiquement, etc.
  }
  
  // Courte pause pour éviter une utilisation excessive du CPU
  delay(10);
}

// Gestionnaire de la page d'accueil
void handleRoot() {
  String html = "<!DOCTYPE html><html>";
  html += "<head><meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<title>ESP32 Serveur Web</title>";
  html += "<style>";
  html += "body { font-family: Arial; text-align: center; margin: 0px auto; padding: 20px; }";
  html += "h1 { color: #0F3376; margin: 50px auto 30px; }";
  html += ".button { display: block; background-color: #4CAF50; border: none; color: white; padding: 15px 32px; ";
  html += "text-decoration: none; font-size: 16px; margin: 20px auto; cursor: pointer; border-radius: 8px; }";
  html += ".off { background-color: #D3435C; }";
  html += ".sensor { font-size: 24px; margin: 20px; }";
  html += "</style>";
  html += "<script>";
  html += "function toggleLED() {";
  html += "  var xhr = new XMLHttpRequest();";
  html += "  xhr.open('POST', '/led', true);";
  html += "  xhr.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');";
  html += "  xhr.onreadystatechange = function() {";
  html += "    if(xhr.readyState == 4 && xhr.status == 200) {";
  html += "      document.getElementById('ledButton').classList.toggle('off');";
  html += "      if(xhr.responseText.includes('ON')) {";
  html += "        document.getElementById('ledButton').innerText = 'Eteindre LED';";
  html += "      } else {";
  html += "        document.getElementById('ledButton').innerText = 'Allumer LED';";
  html += "      }";
  html += "    }";
  html += "  };";
  html += "  xhr.send();";
  html += "}";

  html += "function updateSensor() {";
  html += "  var xhr = new XMLHttpRequest();";
  html += "  xhr.open('GET', '/sensor', true);";
  html += "  xhr.onreadystatechange = function() {";
  html += "    if(xhr.readyState == 4 && xhr.status == 200) {";
  html += "      document.getElementById('sensorValue').innerHTML = xhr.responseText;";
  html += "    }";
  html += "  };";
  html += "  xhr.send();";
  html += "}";

  html += "setInterval(updateSensor, 2000);"; // Mise à jour toutes les 2 secondes
  html += "</script>";
  html += "</head>";
  html += "<body>";
  html += "<h1>Serveur Web ESP32</h1>";
  
  // Bouton pour contrôler la LED
  if (ledState) {
    html += "<button class='button' id='ledButton' onclick='toggleLED()'>Eteindre LED</button>";
  } else {
    html += "<button class='button off' id='ledButton' onclick='toggleLED()'>Allumer LED</button>";
  }
  
  // Affichage de la valeur du capteur
  html += "<div class='sensor'>Valeur du capteur: <span id='sensorValue'>--</span></div>";
  
  html += "</body></html>";
  
  server.send(200, "text/html", html);
}

// Gestionnaire pour contrôler la LED
void handleLED() {
  ledState = !ledState;
  digitalWrite(LED_PIN, ledState ? HIGH : LOW);
  
  if (ledState) {
    server.send(200, "text/plain", "LED ON");
  } else {
    server.send(200, "text/plain", "LED OFF");
  }
}

// Gestionnaire pour lire la valeur du capteur
void handleSensor() {
  int sensorValue = analogRead(ANALOG_SENSOR_PIN);
  server.send(200, "text/plain", String(sensorValue));
}

// Gestionnaire pour l'API JSON
void handleAPIData() {
  StaticJsonDocument<200> doc;
  
  doc["led"] = ledState;
  doc["analog_sensor"] = analogRead(ANALOG_SENSOR_PIN);
  doc["uptime"] = millis() / 1000;
  
  String jsonString;
  serializeJson(doc, jsonString);
  
  server.send(200, "application/json", jsonString);
}

// Gestionnaire pour les pages non trouvées
void handleNotFound() {
  String message = "Page non trouvée\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMéthode: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  
  server.send(404, "text/plain", message);
}