int trigPin = 6;
int echoPin = 7;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  float distance = (float)readUltrasonic();
  if (distance >= 200 || distance <= 0)
  {
    Serial.println("Out of range");
  }
  else
  {
    Serial.print(distance);
    Serial.println(" cm");
    blink(distance * 20);
  }
  delay(50);
}

long readUltrasonic()
{
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(10);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1;
  return distance;
}

void blink(int delayMiill)
{
  digitalWrite(LED_BUILTIN, HIGH);
  delay(delayMiill);
  digitalWrite(LED_BUILTIN, LOW);
  delay(delayMiill);
}
