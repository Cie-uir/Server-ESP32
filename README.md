# Serveur Web ESP32

Un serveur web embarqué simple et léger pour ESP32, permettant de contrôler des composants et de lire des données de capteurs via une interface web intuitive.

![ESP32 Serveur Web](https://via.placeholder.com/800x400?text=ESP32+Serveur+Web)

## 📋 Caractéristiques

- Point d'accès WiFi intégré
- Interface web réactive
- Contrôle d'une LED en temps réel
- Lecture de données de capteurs
- API REST pour l'intégration avec d'autres systèmes
- Interface JSON pour les données structurées

## 🛠️ Prérequis

### Matériel
- Carte ESP32 (toutes variantes)
- Câble USB
- LED (optionnel, la LED intégrée est utilisée par défaut)
- Capteur analogique (optionnel)

### Logiciels
- [Arduino IDE](https://www.arduino.cc/en/software)
- [Gestionnaire de cartes ESP32](https://github.com/espressif/arduino-esp32)
- Bibliothèques:
  - WiFi (incluse avec ESP32)
  - WebServer (incluse avec ESP32)
  - ArduinoJson (à installer via le gestionnaire de bibliothèques)

## 📥 Installation

1. Clonez ce dépôt :
   ```bash
   git clone https://github.com/votre-utilisateur/esp32-serveur-web.git
   ```

2. Ouvrez le fichier `ESP32_Serveur_Web.ino` dans l'IDE Arduino

3. Installez les bibliothèques requises via le gestionnaire de bibliothèques Arduino:
   - ArduinoJson

4. Sélectionnez votre carte ESP32 dans le menu Outils > Type de carte

5. Téléversez le code vers votre ESP32

## 📡 Utilisation

1. Après le téléversement, ouvrez le moniteur série (115200 bauds) pour voir l'adresse IP du point d'accès

2. Connectez-vous au réseau WiFi créé par l'ESP32:
   - **SSID**: ESP32_AP
   - **Mot de passe**: esp32password

3. Ouvrez un navigateur web et accédez à l'adresse `192.168.4.1`

4. Utilisez l'interface web pour:
   - Contrôler la LED (allumer/éteindre)
   - Voir les données du capteur en temps réel

## 🔌 Broches utilisées

| Fonction | Broche | Description |
|----------|--------|-------------|
| LED | 2 | LED intégrée de l'ESP32 (peut être modifiée) |
| Capteur analogique | 34 | Entrée analogique pour le capteur |

## 📊 API

### Contrôle de la LED
- **URL**: `/led`
- **Méthode**: POST
- **Réponse**: "LED ON" ou "LED OFF"

### Lecture du capteur
- **URL**: `/sensor`
- **Méthode**: GET
- **Réponse**: Valeur numérique du capteur

### Données JSON
- **URL**: `/api/data`
- **Méthode**: GET
- **Réponse**: JSON avec l'état de la LED, valeur du capteur et temps de fonctionnement
  ```json
  {
    "led": true,
    "analog_sensor": 2048,
    "uptime": 305
  }
  ```

## 🔧 Personnalisation

### Modification des identifiants WiFi
Modifiez les lignes suivantes dans le code:

```cpp
const char* ssid = "ESP32_AP";
const char* password = "esp32password";
```

### Ajout de nouveaux capteurs
1. Définissez la broche du nouveau capteur
2. Lisez la valeur dans la fonction appropriée
3. Ajoutez la valeur à la réponse JSON dans `handleAPIData()`

## 🚀 Améliorations possibles

- Ajouter une connexion à un réseau WiFi existant (mode station)
- Implémenter une authentification pour sécuriser l'accès
- Stocker les données dans une mémoire flash (SPIFFS)
- Ajouter d'autres capteurs (température, humidité, etc.)
- Créer des règles automatisées basées sur les valeurs des capteurs
- Intégration avec des services cloud (MQTT, ThingSpeak, etc.)

## 📄 Licence

Ce projet est distribué sous la licence MIT. Voir le fichier `LICENSE` pour plus d'informations.

## 🤝 Contributions

Les contributions sont les bienvenues! N'hésitez pas à ouvrir une issue ou à soumettre une pull request.

1. Forkez le projet
2. Créez votre branche de fonctionnalité (`git checkout -b feature/AmazingFeature`)
3. Committez vos changements (`git commit -m 'Add some AmazingFeature'`)
4. Poussez vers la branche (`git push origin feature/AmazingFeature`)
5. Ouvrez une Pull Request

## 📞 Contact

Votre Nom - [@votre_twitter](https://twitter.com/votre_twitter) - email@exemple.com

Lien du projet: [https://github.com/votre-utilisateur/esp32-serveur-web](https://github.com/votre-utilisateur/esp32-serveur-web)
