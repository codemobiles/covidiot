#include <SimpleDHT.h>
#include <ESP8266WiFi.h>
#define WIFI_SSID "cm_guest"
#define WIFI_PASSWORD "87654321"
String LINE_TOKEN = "017KwJGa0tY8i3URv3CLdKL66qytVcE0h7NZJEDlHIZ";


// SimpleDHT11 dht11;
// int pinDHT11 = D2; // DHT11 data pin
// int oldTemp = 0;

// int timer = 0;
// byte temperature = 0;
// byte humidity = 0;




// DHT11 Demo
void setup()
{
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  getWiFiIPAddress();
  sendLineNotification("CodeMobiles Ready...");
}

void loop()
{
  if (dht11.read(pinDHT11, &temperature, &humidity, NULL))
  {
    Serial.println("Read DHT11 failed.");
    delay(2000);
    return;
  }

  //convert into integer
  int newTemp = (int)temperature; // btye to int
  int humidValue = (int)humidity; // btye to int
  Serial.print(newTemp);
  Serial.println(" *C");
  Serial.print(humidValue);
  Serial.println(" %");

  if (newTemp != oldTemp)
  {
    String message = "Temp: " + String(newTemp) + " *C," + "Humidity: " + String(humidValue) + " Percent"; // cannot use %
    sendLineNotification(message);
  }
  oldTemp = newTemp;
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

void sendLineNotification(String message)
{

  String msg = String("message=") + message;

  WiFiClientSecure client;
  if (!client.connect("notify-api.line.me", 443))
  {
    Serial.println("connection failed");
    return;
  }

  String req = "";
  req += "POST /api/notify HTTP/1.1\r\n";
  req += "Host: notify-api.line.me\r\n";
  req += "Content-Type: application/x-www-form-urlencoded\r\n";
  req += "Authorization: Bearer " + String(LINE_TOKEN) + "\r\n";
  req += "Content-Length: " + String(msg.length()) + "\r\n";
  req += "\r\n";
  req += msg;

  client.print(req);

  unsigned long timeout = millis();
  while (client.available() == 0)
  {
    if (millis() - timeout > 5000)
    {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }

  // Read all the lines of the reply from server and print them to Serial
  while (client.available())
  {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }

  Serial.println("closing connection");
}