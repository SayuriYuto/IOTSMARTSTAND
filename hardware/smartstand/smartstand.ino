#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#include "secretkeys.h"


FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1    // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

unsigned long dataMillis = 0;
int count = 0;
unsigned long sendDataPrevMillis = 0;
unsigned long lastFBcheck = 0;
bool timermode = LOW;
String currentDate;
String currentTime;

#define switchPin D5
int prevState = LOW;
#define LED_BUILTIN 2

#include <NTPClient.h>
#include <WiFiUdp.h>
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

void studybuddytext();
void wifibar(bool flag);
void firebaseConnected(bool flag);
void showclock(char timec[], bool flag); 
void studying(bool flag, char timec[]);

void setup()
{

    Serial.begin(115200);
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
    {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
            ; // Don't proceed, loop forever
    }
    display.clearDisplay();
    display.display();
    studybuddytext();
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        wifibar(LOW);
        delay(300);
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();
    Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

    /* Assign the api key (required) */
    config.api_key = API_KEY;

    /* Assign the user sign in credentials */
    auth.user.email = USER_EMAIL;
    auth.user.password = USER_PASSWORD;

    /* Assign the callback function for the long running token generation task */
    config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h

    Firebase.begin(&config, &auth);

    Firebase.reconnectWiFi(true);

    // For sending payload callback
    // config.cfs.upload_callback = fcsUploadCallback;
    pinMode(switchPin, INPUT_PULLUP);
    pinMode(LED_BUILTIN, OUTPUT);

    timeClient.begin();
    timeClient.setTimeOffset(19800);
}

void loop()
{
    if (millis() - lastFBcheck > 1000)
    {
        if (WiFi.status() != WL_CONNECTED)
        {
            wifibar(LOW);
            firebaseConnected(LOW);
        }
        else
        {
            wifibar(HIGH);
            firebaseConnected(HIGH);
        }
        if (timermode == HIGH)
        {
            char pp[5];
            //      int hourspassed = (sendDataPrevMillis)/3600000;
            int minutespassed = (millis() - sendDataPrevMillis) / 60000;
            int secondspassed = ((millis() - sendDataPrevMillis)-(minutespassed*60000)) / 1000;
            sprintf(pp, "%02d:%02d", minutespassed, secondspassed);
//            Serial.println(tt);
            showclock(pp, HIGH);
            
            timeClient.update();
            time_t epochTime = timeClient.getEpochTime();
            struct tm *ptm = gmtime ((time_t *)&epochTime);
            int monthDay = ptm->tm_mday;
            int currentMonth = ptm->tm_mon+1;
            int currentYear = ptm->tm_year+1900;
            currentTime = timeClient.getFormattedTime();
            currentDate = String(monthDay)+ "-" + String(currentMonth) + "-" + String(currentYear);
            
            char tt[5];
            sprintf(tt, "%02d:%02d", timeClient.getHours(), timeClient.getMinutes());
            studying(HIGH, tt);
        }
        else
        {
            timeClient.update();
            char tt[5];
            sprintf(tt, "%02d:%02d", timeClient.getHours(), timeClient.getMinutes());
            //      Serial.println(tt);
            showclock(tt, HIGH);
            studying(LOW, tt);
        }
        lastFBcheck = millis();
    }

    FirebaseJson content;
    if (digitalRead(switchPin) == LOW && prevState == HIGH)
    {
        Serial.println("Button was pressed");
//        studying(HIGH);
        sendDataPrevMillis = millis();
        Serial.println(millis());
        digitalWrite(LED_BUILTIN, LOW);
        timermode = HIGH;
        prevState = LOW;
    }

    //    Button unpressed
    if (digitalRead(switchPin) == HIGH && prevState == LOW)
    {
        Serial.println("Button is unpressed");
        Serial.println(sendDataPrevMillis);
        Serial.println(millis() - sendDataPrevMillis);
        if ((millis() - sendDataPrevMillis > 15000) && Firebase.ready() && sendDataPrevMillis != 0)
        {
            String documentPath = "History/" + String(millis());
            content.set("fields/amount/integerValue", String((millis() - sendDataPrevMillis) / 1000));
            content.set("fields/date/stringValue", String(currentDate));
            content.set("fields/time/stringValue", String(currentTime));


            String doc_path = "projects/";
            doc_path += FIREBASE_PROJECT_ID;
            doc_path += "/databases/(default)/documents/coll_id/doc_id";

            if (Firebase.Firestore.createDocument(&fbdo, FIREBASE_PROJECT_ID, "" /* databaseId can be (default) or empty */, documentPath.c_str(), content.raw()))
                Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
            else
                Serial.println(fbdo.errorReason());
            Serial.println("Set bool... OK Button unpressed");
            digitalWrite(LED_BUILTIN, HIGH);
        }
        prevState = HIGH;
        sendDataPrevMillis = 0;
        timermode = LOW;
    }
}