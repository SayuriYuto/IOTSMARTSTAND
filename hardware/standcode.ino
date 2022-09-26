void setup() { 
    Serial.begin(9600); 
    pinMode(2,OUTPUT);
    }
void loop()
{
    char c;
    if (Serial.available())
    {
        digitalWrite(2,HIGH);
        c = Serial.read();
        Serial.print(c);
        delay(1000);

    }
    else{
        digitalWrite(2,LOW);
        delay(1000);
    }
}