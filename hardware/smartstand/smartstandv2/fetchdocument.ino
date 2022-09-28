
/**
 * Created by K. Suwatchai (Mobizt)
 *
 * Email: k_suwatchai@hotmail.com
 *
 * Github: https://github.com/mobizt/Firebase-ESP-Client
 *
 * Copyright (c) 2022 mobizt
 *
 */

// This example shows how to run a query. This operation required Email/password, custom or OAUth2.0 authentication.

#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif

#include <Firebase_ESP_Client.h>
#include "secretF.h"

// Provide the token generation process info.
#include <addons/TokenHelper.h>


// Define Firebase Data object
FirebaseData fbdo;
FirebaseJson query;
FirebaseAuth auth;
FirebaseConfig config;
FirebaseJson json;       // or constructor with contents e.g. FirebaseJson json("{\"a\":true}");
FirebaseJsonArray arr;   // or constructor with contents e.g. FirebaseJsonArray arr("[1,2,true,\"test\"]");
FirebaseJsonData result; //object that keeps the deserializing result

unsigned long dataMillis = 0;
int count = 0;

void setup()
{

    Serial.begin(115200);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(300);
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();

    Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

    /* Assign the project host and api key (required) */
    config.api_key = API_KEY;

    /* Assign the user sign in credentials */
    auth.user.email = USER_EMAIL;
    auth.user.password = USER_PASSWORD;

    /* Assign the callback function for the long running token generation task */
    config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h

#if defined(ESP8266)
    // In ESP8266 required for BearSSL rx/tx buffer for large data handle, increase Rx size as needed.
    fbdo.setBSSLBufferSize(2048 /* Rx buffer size in bytes from 512 - 16384 */, 2048 /* Tx buffer size in bytes from 512 - 16384 */);
#endif

    // Limit the size of response payload to be collected in FirebaseData
    fbdo.setResponseSize(2048);

    Firebase.begin(&config, &auth);

    Firebase.reconnectWiFi(true);
}

void loop()
{

    if (Firebase.ready() && (millis() - dataMillis > 60000 || dataMillis == 0))
    {
        dataMillis = millis();

        Serial.print("Query a Firestore database... ");
        

        query.set("select/fields/fieldPath", "task");
        query.set("from/collectionId", "task");
        query.set("from/allDescendants", true);
        query.set("limit", 1);

        if (Firebase.Firestore.runQuery(&fbdo, FIREBASE_PROJECT_ID, "" /* databaseId can be (default) or empty */, "" /* The document path */, &query /* The FirebaseJson object holds the StructuredQuery data */))
            {
              Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
              arr.setJsonArrayData(fbdo.payload());
              arr.get(result,0);
              String str;
              arr.toString(str, true /* prettify option */);

      Serial.println("\n---------");
    Serial.println(str);
    Serial.println(result.to<String>());
    FirebaseJson json2;
    FirebaseJsonData result2;
    json2.setJsonData(result.to<String>());
    json2.get(result2,"document/fields/task/stringValue");
    Serial.println(result2.to<String>());
//    json2.get(result2 /* FirebaseJsonData */, "document" /* key or path */,true);
    
            }
        else
            Serial.println(fbdo.errorReason());
    }
}
