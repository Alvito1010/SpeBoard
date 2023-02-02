#define BLYNK_TEMPLATE_ID "TMPLhD9RHVT9"
#define BLYNK_DEVICE_NAME "MQ2 Gas Sensor and Water detector"
#define BLYNK_AUTH_TOKEN "LBToK7uirV4QPxgF3ClYjV5_I8nUzAG0"

// Your WiFi Credentials.
// Set password to "" for open networks.
char ssid[] = "";
char pass[] = "";

// #define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <device.h>
#include <firebase.h>
#include <WiFiManager.h>
BlynkTimer timer;

int MQ2_SENSOR_Value = 0;
int RAIN_SENSOR_Value = 0;
bool isconnected = false;
char auth[] = BLYNK_AUTH_TOKEN;
String MacAdd = WiFi.macAddress();
String MacAdd1 = MacAdd;
String MacAdd2 = MacAdd;

String addres1 = "/";
String address2 = "/";

String rtdbPath2 = "/users/devices/sensorName/Water Sensor/isWarning";
String rtdbPath1 = "/users/devices/sensorName/Gas Sensor/isWarning";





#define VPIN_BUTTON_1 V1
#define VPIN_BUTTON_2 V2

void getSensorData()
{

  MQ2_SENSOR_Value = map(analogRead(MQ2_SENSOR), 0, 4095, 0, 100);
  RAIN_SENSOR_Value = digitalRead(RAIN_SENSOR);

  



  if (MQ2_SENSOR_Value > 50)
  {
    if (RAIN_SENSOR_Value == 0)
    {
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(RED_LED, HIGH);
      Firebase.RTDB.setBool(&fbdo, address2, true) ? "ok" : fbdo.errorReason().c_str();
      Firebase.RTDB.setBool(&fbdo, addres1, true) ? "ok" : fbdo.errorReason().c_str();
      

    }
    else if (RAIN_SENSOR_Value == 1)
    {
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(RED_LED, HIGH);
      Firebase.RTDB.setBool(&fbdo, address2, false) ? "ok" : fbdo.errorReason().c_str();
      Firebase.RTDB.setBool(&fbdo, addres1, true) ? "ok" : fbdo.errorReason().c_str();
    }
  }
  else if (MQ2_SENSOR_Value < 50)
  {
    if (RAIN_SENSOR_Value == 0)
    {
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(RED_LED, HIGH);
      Firebase.RTDB.setBool(&fbdo, address2, true) ? "ok" : fbdo.errorReason().c_str();
      Firebase.RTDB.setBool(&fbdo, addres1, false) ? "ok" : fbdo.errorReason().c_str();
    }
    else if (RAIN_SENSOR_Value == 1)
    {
      digitalWrite(GREEN_LED, HIGH);
      digitalWrite(RED_LED, LOW);
      Firebase.RTDB.setBool(&fbdo, address2, false ) ? "ok" : fbdo.errorReason().c_str();
      Firebase.RTDB.setBool(&fbdo, addres1, false ) ? "ok" : fbdo.errorReason().c_str();
    }
  }
}
void sendData()
{
  Blynk.virtualWrite(VPIN_BUTTON_1, MQ2_SENSOR_Value);
  if (MQ2_SENSOR_Value > 50)
  {
    if (RAIN_SENSOR_Value == 0)
    {
      Blynk.logEvent("gas", "Gas Detected!");
      Blynk.logEvent("rain", "Water Detected!");
      Blynk.virtualWrite(VPIN_BUTTON_2, "Water Detected!");
    }
    else if (RAIN_SENSOR_Value == 1)
    {
      Blynk.logEvent("gas", "Gas Detected!");
      Blynk.logEvent("rain", "No Water Detected!");
      Blynk.virtualWrite(VPIN_BUTTON_2, "No Water Detected.");
    }
  }
  else if (MQ2_SENSOR_Value < 50)
  {
    if (RAIN_SENSOR_Value == 0)
    {
      Blynk.logEvent("gas", "Gas Not Detected!");
      Blynk.logEvent("rain", "Water Detected!");
      Blynk.virtualWrite(VPIN_BUTTON_2, "Water Detected!");
    }
    else if (RAIN_SENSOR_Value == 1)
    {
      Blynk.logEvent("gas", "Gas Not Detected!");
      Blynk.logEvent("rain", "No Water Detected!");
      Blynk.virtualWrite(VPIN_BUTTON_2, "No Water Detected.");
    }
  }
}

void checkBlynkStatus()
{ // called every 2 seconds by SimpleTimer
  isconnected = Blynk.connected();
  if (isconnected == true)
  {
    digitalWrite(WIFI_LED, HIGH);
    sendData();
    // Serial.println("Blynk Connected");
  }
  else
  {
    digitalWrite(WIFI_LED, LOW);
    Serial.println("Blynk Not Connected");
  }
}

void setup()
{
  WiFi.mode(WIFI_STA); 
    Serial.begin(115200);
    WiFiManager wm;

    // reset settings - hapus credential tersimpan untuk testing
    // uncomment baris di bawah ini saat test
    wm.resetSettings();

    // Automatically connect using saved credentials,
    // if connection fails, it starts an access point with the specified name ( "AutoConnectAP"),
    // if empty will auto generate SSID, if password is blank it will be anonymous AP (wm.autoConnect())
    // then goes into a blocking loop awaiting configuration and will return success result

    bool res;
    // res = wm.autoConnect(); // auto generated AP name from chipid
    // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
    res = wm.autoConnect("SensorManagerSPE","taucewuyenshosepa"); // password protected ap

    if(!res) {
        Serial.println("Failed to connect");
        // ESP.restart();
    } 
    else {
        //if you get here you have connected to the WiFi    
        Serial.println("Connected to Wifi");
    }

  Serial.begin(115200);

  pinMode(MQ2_SENSOR, INPUT);
  pinMode(RAIN_SENSOR, INPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(WIFI_LED, OUTPUT);

  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, LOW);
  digitalWrite(WIFI_LED, LOW);

  pinMode(RED_LED, OUTPUT);
  pinMode(PIN_SW, INPUT_PULLUP);
  // WifiConnect();
  Firebase_Init("cmd");
  Serial.println("System ready.");
  Serial.println(MacAdd);

  addres1+=MacAdd1;
  address2+=MacAdd2;

  addres1+=rtdbPath1;
  address2+=rtdbPath2;

  WiFi.begin(ssid, pass);
  timer.setInterval(2000L, checkBlynkStatus); // check if Blynk server is connected every 2 seconds
  Blynk.config(auth);
  delay(1000);
}

void loop()
{
  getSensorData();
  // Serial.println("**");
  // Serial.println("");
  // Serial.println(addres1);
  // Serial.println("");
  // Serial.println("");
  // Serial.println(address2);
  // Serial.println("");
  // Serial.println("//");
  // Serial.println("");

  Blynk.run();
  timer.run();
}

#include <Arduino.h>
#include <Ticker.h>
#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include "firebase.h"
#include "device.h"

void WifiConnect();

int nMsgCount = 0;

void onFirebaseStream(FirebaseStream data)
{
  Serial.printf("onFirebaseStream: %s %s %s %s\n", data.streamPath().c_str(),
                data.dataPath().c_str(), data.dataType().c_str(), data.stringData().c_str());

  if (data.dataType() == "int")
  {
    String strDev = data.dataPath().substring(1);
    byte nValue = data.stringData().charAt(0) - '0';
    Serial.printf("Device: %s -> %d\n", strDev.c_str(), nValue);

    if (nValue >= 0 && nValue <= 1)
    {
      if (strDev == "LedRed")
        digitalWrite(RED_LED, nValue);
    }
  }
}

void WifiConnect()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  while (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
  Serial.print("System connected with IP address: ");
  Serial.println(WiFi.localIP());
  Serial.printf("RSSI: %d\n", WiFi.RSSI());
}