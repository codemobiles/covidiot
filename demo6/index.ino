#include <ESP8266WiFi.h>
#define WIFI_SSID "cm_guest"
#define WIFI_PASSWORD "87654321"

// MQTT
#include <PubSubClient.h>
const char *mqtt_server = "broker.mqtt-dashboard.com";
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
const char *topic = "cmlek/infrareds/ir1";

// Infrared
#define IRsensor 5 // 5 = D1, 4 = D2
long lastTimestamp = 0;
int count = 0;
int lastCount = 0;

void setup()
{
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);
    getWiFiIPAddress();

    // infrared
    pinMode(IRsensor, INPUT_PULLUP);
    attachInterrupt(IRsensor, react, CHANGE);

    // Connect MQTT Server
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
}

void loop()
{
    if (!client.connected())
    {
        reconnect();
    }
    client.loop();

    // Publish Infrared begin
    if (lastCount != count)
    {
        snprintf(msg, 75, "{\"Counter\" : %ld}", count);
        client.publish(topic, msg);
        lastCount = count;
    }
    // Publish Infrared end
}

// MQTT Code begin
void callback(char *topic, byte *payload, unsigned int length)
{
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i = 0; i < length; i++)
    {
        Serial.print((char)payload[i]);
    }
    Serial.println();

    // Switch on the LED if an 1 was received as first character
    if ((char)payload[0] == '1')
    {
        digitalWrite(BUILTIN_LED, LOW); // Turn the LED on (Note that LOW is the voltage level
                                        // but actually the LED is on; this is because
                                        // it is acive low on the ESP-01)
    }
    else
    {
        digitalWrite(BUILTIN_LED, HIGH); // Turn the LED off by making the voltage HIGH
    }
}

void reconnect()
{
    // Loop until we're reconnected
    while (!client.connected())
    {
        Serial.print("Attempting MQTT connection...");
        // Create a random client ID
        String clientId = "ESP8266Client-";
        clientId += String(random(0xffff), HEX);
        // Attempt to connect
        if (client.connect(clientId.c_str()))
        {
            Serial.println("connected");
            // Once connected, publish an announcement...
            client.publish(topic, "online");
            // ... and resubscribe
            client.subscribe("cmlek/mobiles/switch1");
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

// MQTT Code end

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