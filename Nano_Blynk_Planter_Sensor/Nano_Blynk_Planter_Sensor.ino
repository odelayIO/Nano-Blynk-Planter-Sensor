// Fill-in your Template ID 
#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME ""
#define BLYNK_AUTH_TOKEN ""

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "";
char pass[] = "";

bool fetch_blynk_state = true;  //true or false

//#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>


// define the LEDs
#define LEDPin    LED_BLUE
#define wifiLed   LED_BUILTIN

//Change the virtual pins according the rooms
#define BLYNK_SW    V0 

// LED State
bool led_state      = HIGH; // HIGH: LED Off, LOW: LED On
int uptime          = 0;
int moisture_sensor = 0;

char auth[] = BLYNK_AUTH_TOKEN;

BlynkTimer timer;


// When App button is pushed - switch the state
BLYNK_WRITE(BLYNK_SW) {
  led_state = !param.asInt();  // Invert App Button since HIGH: Led Off, LOW: Led On
  digitalWrite(LEDPin, led_state);

  if(led_state == 1) {
    Serial.println("LED has been turned OFF");
  } else {
    Serial.println("LED has been turned ON");
  }
}


void checkBlynkStatus() { // called every 2 seconds by SimpleTimer

  // Update Moisture Sensor Reading
  moisture_sensor = 4095-analogRead(A0);
  Blynk.virtualWrite(V2, moisture_sensor);
  Serial.print("Moisture Sensor Reading : ");
  Serial.println(moisture_sensor);
  //if(moisture_sensor < 1300) {
  //  Blynk.logEvent("water_required");
  //}
  

  // Update App with uptime
  uptime = millis()/1000;
  Blynk.virtualWrite(V1, uptime);
  Serial.print("Update Time : ");
  Serial.print(uptime);
  Serial.println(" seconds");

  bool isconnected = Blynk.connected();
  if (isconnected == false) {
    Serial.println("Blynk Not Connected");
    digitalWrite(wifiLed, LOW);
  }
  if (isconnected == true) {
    digitalWrite(wifiLed, HIGH);
    Serial.println("Blynk Connected");
  }
}

BLYNK_CONNECTED() {
  // Request the latest state from the server
  if (fetch_blynk_state){
    Blynk.syncVirtual(BLYNK_SW);
  }
}

void setup()
{
  Serial.begin(9600);

  pinMode(LEDPin, OUTPUT);
  pinMode(wifiLed, OUTPUT);

  //During Starting LED should TURN OFF
  digitalWrite(LEDPin, led_state);

  digitalWrite(wifiLed, LOW);

  WiFi.begin(ssid, pass);
  timer.setInterval(2000L, checkBlynkStatus); // check if Blynk server is connected every 2 seconds
  Blynk.config(auth);
  delay(1000);
  
}

void loop()
{ 
  Blynk.run();
  timer.run();
}

