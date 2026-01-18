
#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>
#include <PZEM004Tv30.h>
#include <Dali.h>

// Configuración WiFi y Firebase
#define FIREBASE_HOST "prueba-f2d4b-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "RgQYwOWfhhb087Btlr7VpkuWnizYCbiLo6Rz4NqI"
#define WIFI_SSID "IngElectrica"
#define WIFI_PASSWORD "C@IngEle#U120"

// Pines
PZEM004Tv30 pzem(D6, D7); // Define los pines RX y TX
#define BTN_RESET 14
#define DALI_TX 4
#define DALI_RX_A A0

// Definiciones DALI
#define BROADCAST_DP 0b11111110
#define BROADCAST_C 0b11111111
#define ON_DP 0b11111110
#define OFF_DP 0b00000000
#define ON_C 0b00000101
#define OFF_C 0b00000000
#define QUERY_STATUS 0b10010000
#define RESET 0b00100000

// Instancias Firebase, Auth y PZEM
FirebaseData firebaseData1;
FirebaseConfig firebaseConfig;
FirebaseAuth firebaseAuth;

// Rutas Firebase
String nombre = "Casa";
String path = "/TFG/Equipos/" + nombre;
String TensionPath = "/Tension";
String IntensidadPath = "/Intensidad";
String PotenciaPath = "/Potencia";
String Brillopath = "/Brillo";
String Onoffpath = "/Estado";
String ActivaSectorPath = "/ActivaSector";
String SectorPath = "/Sector";

// Variables para datos
float tension = 0.0;
float intensidad = 0.0;
float potencia = 0.0;
int brillo = 50;  // Brillo inicializado a 50
int Onoff = 0;
int ActivaSector = 0;
String Sector = "ninguno";
int estado;
int brilloS;
void setup() {
    Serial.begin(19200);

    // Conectar a WiFi
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Conectando a Wi-Fi");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(300);
    }
    Serial.println();
    Serial.print("Conectado con IP: ");
    Serial.println(WiFi.localIP());

    // Configuración de Firebase
    firebaseConfig.host = FIREBASE_HOST;
    firebaseConfig.signer.tokens.legacy_token = FIREBASE_AUTH;

    // Inicializar Firebase
    Firebase.begin(&firebaseConfig, &firebaseAuth);
    Firebase.reconnectWiFi(true);

    // Verificar conexión Firebase
    if (Firebase.ready()) {
        Serial.println("Conexión a Firebase exitosa.");
    } else {
        Serial.println("No se pudo conectar a Firebase.");
    }

    // Inicializar valores en Firebase
    Firebase.setFloat(firebaseData1, path + TensionPath, tension);
    Firebase.setFloat(firebaseData1, path + IntensidadPath, intensidad);
    Firebase.setFloat(firebaseData1, path + PotenciaPath, potencia);
    Firebase.setInt(firebaseData1, path + Brillopath, brillo);
    Firebase.setInt(firebaseData1, path + Onoffpath, Onoff);
    Firebase.setInt(firebaseData1, path + ActivaSectorPath, ActivaSector);
    Firebase.setString(firebaseData1, path + SectorPath, Sector);

    // Configuración DALI
    dali.setupTransmit(DALI_TX);
    dali.setupAnalogReceive(DALI_RX_A);
    dali.busTest();
    dali.msgMode = true;

    pinMode(BTN_RESET, INPUT);
}

void loop() {
    // Leer datos de PZEM
    tension = pzem.voltage();
    intensidad = pzem.current();
    potencia = pzem.power();

    if (isnan(tension)) {
        Serial.println("Error leyendo el voltaje");
        tension = 0.0;
    } else {
        Serial.print("Voltage: ");
        Serial.print(tension);
        Serial.println(" V");
    }

    if (isnan(intensidad)) {
        Serial.println("Error leyendo la corriente");
        intensidad = 0.0;
    } else {
        Serial.print("Current: ");
        Serial.print(intensidad);
        Serial.println(" A");
    }

    if (isnan(potencia)) {
        Serial.println("Error leyendo la potencia");
        potencia = 0.0;
    } else {
        Serial.print("Power: ");
        Serial.print(potencia);
        Serial.println(" W");
    }

    // Actualizar Firebase
    Firebase.setFloat(firebaseData1, path + TensionPath, tension);
    Firebase.setFloat(firebaseData1, path + IntensidadPath, intensidad);
    Firebase.setFloat(firebaseData1, path + PotenciaPath, potencia);

    // Leer Sector
    if (Firebase.getString(firebaseData1, path + SectorPath)) {
        Sector = firebaseData1.stringData();
        Sector.replace("\\", "");
        Sector.replace("\"", "");
        Serial.println("Sector leído como String: " + Sector);
    } else {
        Serial.println("Error al leer el sector desde Firebase.");
    }

    

    

    // Leer ActivaSector
    if (Firebase.getString(firebaseData1, path + ActivaSectorPath)) {
        String ActivaSectorStr = firebaseData1.stringData();
        ActivaSectorStr.replace("\\", "");
        ActivaSectorStr.replace("\"", "");
        ActivaSector = ActivaSectorStr.toInt();
        Serial.println("ActivaSector leído: " + ActivaSectorStr + " -> int: " + String(ActivaSector));
    } else {
        Serial.println("Error al leer ActivaSector desde Firebase.");
    }

    // Si ActivaSector está activo, leer brillo del sector específico
    if (ActivaSector == 1) {
        String BrilloSector = "/TFG/Sectores/" + Sector + Brillopath;
        String EstadoSector = "/TFG/Sectores/" + Sector + Onoffpath;
        Serial.println("Ruta construida para brillo: " + BrilloSector);

        if (Firebase.getString(firebaseData1, EstadoSector)) {
        String onoffStr = firebaseData1.stringData();
        onoffStr.replace("\\", "");
        onoffStr.replace("\"", "");
        Onoff = onoffStr.toInt();
        Serial.println("Estado leído: " + onoffStr + " -> int: " + String(Onoff));
    } else {
        Serial.println("Error al leer el estado desde Firebase.");
    }

    Serial.println("Brillo final enviado: " + String(brillo));

    // Leer Brillo
    if (Firebase.getString(firebaseData1, BrilloSector)) {
        String brilloStr = firebaseData1.stringData();
        brilloStr.replace("\\", "");
        brilloStr.replace("\"", "");
        brillo = brilloStr.toInt();
        Serial.println("Brillo leído: " + brilloStr + " -> int: " + String(brillo));
    } else {
        Serial.println("Error al leer el brillo desde Firebase.");
    }
     
    // Control de DALI con brillo y estado
    if (Onoff == 1) {
        int brilloDali = map(brillo, 0, 100, 0, 254);
        dali.transmit(0b00000000, brilloDali);
        Serial.println("DALI: Brillo enviado");
    } else {
        dali.transmit(0b00000000, OFF_C);
        Serial.println("DALI: Apagado");
    }
    }else{

      // Leer Estado
    if (Firebase.getString(firebaseData1, path + Onoffpath)) {
        String onoffStr = firebaseData1.stringData();
        onoffStr.replace("\\", "");
        onoffStr.replace("\"", "");
        Onoff = onoffStr.toInt();
        Serial.println("Estado leído: " + onoffStr + " -> int: " + String(Onoff));
    } else {
        Serial.println("Error al leer el estado desde Firebase.");
    }

    Serial.println("Brillo final enviado: " + String(brillo));

    // Leer Brillo
    if (Firebase.getString(firebaseData1, path + Brillopath)) {
        String brilloStr = firebaseData1.stringData();
        brilloStr.replace("\\", "");
        brilloStr.replace("\"", "");
        brillo = brilloStr.toInt();
        Serial.println("Brillo leído: " + brilloStr + " -> int: " + String(brillo));
    } else {
        Serial.println("Error al leer el brillo desde Firebase.");
    }
     
    // Control de DALI con brillo y estado
    if (Onoff == 1) {
        int brilloDali = map(brillo, 0, 100, 0, 254);
        dali.transmit(0b00000000, brilloDali);
        Serial.println("DALI: Brillo enviado");
    } else {
        dali.transmit(0b00000000, OFF_C);
        Serial.println("DALI: Apagado");
    }
    }

    delay(500);
}
