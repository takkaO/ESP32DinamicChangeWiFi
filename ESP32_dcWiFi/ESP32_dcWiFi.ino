#include <WiFi.h>
#include <WebServer.h>
#include <SPIFFS.h>

enum OP_MODE {
    SERVER,
    CLIENT
};
enum OP_MODE op_mode = SERVER;

// PIN
const int MODE_SWITCH = 22;

// Wi-Fi Config file
const char *settings = "/wifi_settings.txt";
const char ssid[] = "ESP32-AP"; // SSID
const char pass[] = "12345678"; // password

WebServer server(80);

IPAddress local_ip(192, 168, 4, 1);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);

void serverSetup(){
    WiFi.softAP(ssid, pass);                      // setup ssid and pass
    delay(100);                                   // IMPORTANT wait
    WiFi.softAPConfig(local_ip, gateway, subnet); // setup ip, gateway, subnet_mask
    delay(100);                                   // IMPORTANT wait

    // bind handler
    server.on("/", HTTP_GET, handle_OnRootGet);
    server.on("/", HTTP_POST, handle_OnRootPost);
    server.onNotFound(handle_NotFound);

    server.begin(); // launch Server

    /* Show Infomation */
    Serial.print("SSID: ");
    Serial.println(ssid);
    Serial.print("AP IP address: ");
    Serial.println(local_ip);
    Serial.println("HTTP server started");
}

void clientSetup(){
    File f = SPIFFS.open(settings, "r");
    String ssid = f.readStringUntil('\n');
    String pass = f.readStringUntil('\n');
    f.close();

    ssid.trim();
    pass.trim();

    Serial.println("SSID: " + ssid);
    Serial.println("PASS: " + pass);

    WiFi.begin(ssid.c_str(), pass.c_str());

    while (WiFi.status() != WL_CONNECTED){
        delay(500);
        Serial.print(".");
    }
    Serial.print(" CONNECTED!");
}

void setup() {
    Serial.begin(115200);    
    delay(1000);

    // Initialize file system
    if (!SPIFFS.begin(true)){
        Serial.println("An Error has occurred while mounting SPIFFS");
        while(1);
    }

    pinMode(MODE_SWITCH, INPUT);
    if (digitalRead(MODE_SWITCH) == LOW) {
        serverSetup();
        op_mode = SERVER;
    }
    else{
        clientSetup();
        op_mode = CLIENT;
    }
}

void loop() {
    if (op_mode == SERVER) {
        /* If server mode */
        server.handleClient();
        return;
    }

    /* If client mode */
    // pass

}

void handle_OnRootGet(){
    String html = "<!DOCTYPE html><html><head><meta name=\"viewport\" content=\"width=device-width,initial-scale=1.0\"><meta charset=\"UTF-8\"><title> WiFi Setting </title></head>";
    html += "<body style=\"max-width: 600px; margin: auto;\">";
    html += "<h1 style=\"text-align: center;\">ESP32 WiFi Settings</h1>";
    html += "<h3 style=\"text-align: center; color: red; font-weight: bold;\"> Be careful! </br> Only 2.4GHz WiFi can be connected! </h3>";
    html += "<form method='post' style=\"text-align: center;\">";
    html += "<input type='text' name='ssid' placeholder='ssid'></br></br>";
    html += "<input type='text' name='pass' placeholder='pass'></br></br>";
    html += "<input type='submit'><br>";
    html += "</form></body></html>";
    server.send(200, "text/html", html);
}

void handle_OnRootPost(){
    String ssid = server.arg("ssid");
    String pass = server.arg("pass");

    Serial.println(ssid);

    File f = SPIFFS.open(settings, "w");
    f.println(ssid);
    f.println(pass);
    f.close();

    String html = "<!DOCTYPE html><html><head><meta name=\"viewport\" content=\"width=device-width,initial-scale=1.0\"><meta charset=\"UTF-8\"><title> WiFi Setting </title></head>";
    html += "<body style=\"max-width: 600px; margin: auto;\">";
    html += "<h1 style=\"text-align: center;\">ESP32 WiFi Settings</h1>";
    html += "<h3 style=\"text-align: center;\"> Complete! </h3>";
    html += ssid + "<br>";
    html += pass + "<br>";
    html += "</body></html>";
    server.send(200, "text/html", html);
}

void handle_NotFound()
{
    server.send(404, "text/plain", "Not found");
}
