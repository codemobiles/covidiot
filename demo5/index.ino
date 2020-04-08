#include <ESP8266WiFi.h>
#define WIFI_SSID "cm_guest"
#define WIFI_PASSWORD "87654321"
#include <PubSubClient.h>
#include <SimpleDHT.h>


SimpleDHT11 dht11;
int tempValue;
int humidValue;
int oldTempValue = 0;
int pinDHT11 = D2; 

const char* mqtt_server = "broker.mqtt-dashboard.com";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
const char* topic = "cm/sensors/dht11";


void setup()
{
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);
    getWiFiIPAddress();
}

void loop()
{
    
}


void getWiFiIPAddress()
{
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
}