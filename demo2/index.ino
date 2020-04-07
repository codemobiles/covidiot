// Define variables
const int BUTTON_PIN = 3;
boolean lastSignal = LOW;
boolean toggle = false;

void setup()
{
    // Initial
    pinMode(BUTTON_PIN, INPUT);
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(115200);
}

int count = 0;
void loop()
{

    boolean currentSignal = digitalRead(BUTTON_PIN);
    Serial.println(currentSignal);


    if (currentSignal && lastSignal == LOW)
    {
        count = count + 1;
        toggle = !toggle;
        digitalWrite(LED_BUILTIN, toggle);
        Serial.println(count);
    }

    lastSignal = currentSignal;
    delay(10);
}
