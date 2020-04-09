#include <ESP8266WiFi.h>
#define WIFI_SSID "cm_guest"
#define WIFI_PASSWORD "87654321"

// Infrared
#define IRsensor 5 // 5 = D1, 4 = D2
long lastTimestamp = 0;
int count = 0;

void setup()
{
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);
    getWiFiIPAddress();

    // infrared
    pinMode(IRsensor, INPUT_PULLUP);
    attachInterrupt(IRsensor, react, CHANGE);
}

void loop()
{
}

//When interrupt orrcured, activate react function.
void react()
{
    long currentTimestamp = millis();
    if ((currentTimestamp - lastTimestamp) > 200)
    {
        count++;
        Serial.println(count);
    }
    lastTimestamp = currentTimestamp;
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