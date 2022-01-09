
#include <ESP8266WiFi.h>
#include <Adafruit_NeoPixel.h>
#include <ArduinoJson.h>
const char* ssid  = "ssid";
const char* password = "password";
const int port = 80;
WiFiServer server(port);
WiFiClient  Client;
#define PIN D8
Adafruit_NeoPixel PIXELS = Adafruit_NeoPixel(8, PIN, NEO_GRB + NEO_KHZ800);
StaticJsonDocument<200> doc;

int LED;
int R, G, B = 0;
bool begins = false;
bool finished = false;
char in;
String response = "";

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("Connecting to ");
  Serial.println(ssid);
  uint8_t i = 0;
  while (WiFi.status() != WL_CONNECTED && i++ < 20) {
    delay(500);
  }
  if (i == 21) {
    Serial.println("Could not connect to");
    Serial.println(ssid);
    while (1) {
      delay(500);
    }
  }
  server.begin();
  server.setNoDelay(true);
  Serial.println(WiFi.localIP());
  Serial.println("port:" + String(port));
  PIXELS.begin();
  initx();
}
void loop() {
  if (server.hasClient()) {
    if (!Client || !Client.connected()) {
      if (Client) {
        Client.stop();
      }
      Client = server.available();
    }
  }
  if (Client && Client.connected()) {
    if (Client.available()) {
      while (Client.available()) {
        getData() ;
        if (response.endsWith("}")) {
          DeserializationError error = deserializeJson(doc, response);
          if (error) {
            return;
          }
          R = doc["R"];
          G = doc["G"];
          B = doc["B"];
          setColorA(R, G, B);
        }
      }
    }
  }
}

void getData() {
  if (finished == false)
  {
    in = Client.read();
    if (in == '{')
    {
      response = "";
      begins = true;
    }
    if (begins)
    {
      response += (in);
    }
    if (in == '}')
    {
      begins = false;
    }
  }
}

void setColor(int LED, int RED_VALUE, int GREEN_VALUE, int BLUE_VALUE, int delayValue)
{
  PIXELS.setPixelColor(LED, PIXELS.Color(RED_VALUE, GREEN_VALUE, BLUE_VALUE));
  PIXELS.show();
  delay(delayValue);
}

void setColorA(int RED_VALUE, int GREEN_VALUE, int BLUE_VALUE)
{
  for (int i = 0; i < 8; i++) {
    PIXELS.setPixelColor(i, PIXELS.Color(RED_VALUE, GREEN_VALUE, BLUE_VALUE));
    PIXELS.show();
  }
}

void initx() {
  PIXELS.clear();
  for (LED = 0; LED <= 8; LED++)
  {
    setColor(LED, 255, 0, 0, 100);
  }
  for (LED = 0; LED <= 8; LED++)
  {
    setColor(LED, 0, 255, 255, 100);
  }
  for (LED = 0; LED <= 8; LED++)
  {
    setColor(LED, 255, 0, 255, 100);
  }
}
