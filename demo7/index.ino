/* CM_IoT Ambient Temperature / Humidity  
  
   using NodeMCU with DHT11 Temperature/Humidity sensor and Firebase
   
   DHT11 temperature/humidity sensor -> NodeMCU
   VCC -> VIN (5VDC)
   DATA -> D2 (4.7k resistor between VCC and DATA)
   Pin 3 null
   GND -> GND
*/
#include <SimpleDHT.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#define FIREBASE_HOST "iotfirebase.firebaseio.com"
#define FIREBASE_AUTH "SPrtCtGTHOrPrf6VCNoxtVgGpAVu6ph0UnfWaYY4"
#define WIFI_SSID "cm_guest" 
#define WIFI_PASSWORD "87654321"

// DHT11
int pinDHT11 = D2;
SimpleDHT11 dht11;
int tempValue, humidValue;

// Infrared
#define IRsensor 5 // 5 = D1, 4 = D2
long lastTimestamp = 0;
int count = 0;

// Ultrasonic
#define echoPin D7         // Echo Pin
#define trigPin D6         // Trigger Pin
#define LEDPin LED_BUILTIN // Onboard LED

int maximumRange = 200;  // Maximum range needed
int minimumRange = 0;    // Minimum range needed
long duration, distance; // Duration used to calculate distance

void setup()
{
  Initialization();
  WiFiConnection();
  delay(1000);

  if (Firebase.failed())
  {
    Serial.print("Firebase failed");
    Serial.println(Firebase.error());
  }
}

void loop()
{
  sendDHT11();
  sendInfrared();
  sendUltrasonic();
  delay(50);
}

void sendUltrasonic()
{
  //The following trigPin/echoPin cycle is used to determine the
  //distance of the nearest object by bouncing soundwaves off of it.
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);

  //Calculate the distance (in cm) based on the speed of sound.
  distance = duration / 58.2;

  if (distance >= maximumRange || distance <= minimumRange)
  {
    Serial.println("Out of range");
    distance = -1;
  }
  else
  {
    Serial.print(distance);
    Serial.println(" cm.");
  }

   // update value to Firebase
   Firebase.setFloat("UltrasonicRawDistance", distance);
}
void sendDHT11()
{
  // read data from DHT11
  byte temperature = 0;
  byte humidity = 0;
  //dht.read(data pin, temperature output byte, humidity output byte
  if (dht11.read(pinDHT11, &temperature, &humidity, NULL))
  {
    Serial.print("Read DHT11 failed.");
    return;
  }

  //convert into integer
  tempValue = (int)temperature;
  humidValue = (int)humidity;
  Serial.print(tempValue);
  Serial.println(" *C");
  Serial.print(humidValue);
  Serial.println(" %");

  Firebase.setInt("Temperature", tempValue);
  Firebase.setInt("Humidity", humidValue);
}

void sendInfrared()
{
  Firebase.setInt("IRSensorStatus", count);
  Serial.print("Found: ");
  Serial.println(count);

  if (Firebase.getBool("IRSensorReset") == true)
  {
    count = 0;
  }

  Serial.print("Data: ");
  Serial.println(Firebase.getInt("Data"));
}

//When interrupt orrcured, activate react function.
void react()
{
  long currentTimestamp = millis();
  if ((currentTimestamp - lastTimestamp) > 200)
  {
    count++;
  }
  lastTimestamp = currentTimestamp;
}

void Initialization()
{
  Serial.begin(9600);

  // dht no needed

  // infrared
  pinMode(IRsensor, INPUT_PULLUP);
  attachInterrupt(IRsensor, react, CHANGE);

  // ultrasonic
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(LEDPin, OUTPUT);
}

void WiFiConnection()
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

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}
