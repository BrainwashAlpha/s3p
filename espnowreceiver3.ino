#include <ESP32Servo.h>
#include <Adafruit_NeoPixel.h>

/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp-now-esp32-arduino-ide/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include <esp_now.h>
#include <WiFi.h>


int pos = 0;

Adafruit_NeoPixel pixels(1, 21, NEO_GRB + NEO_KHZ800);

Servo myservo;

bool action = false;

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
  bool a;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  if (myData.a == true) {
    pixels.fill(0x008000);
    pixels.show();
    myservo.write(180);
  } else {
    pixels.fill(0x000000);
    pixels.show();
    myservo.write(0);
  }
}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);

	ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
	myservo.setPeriodHertz(50);    // standard 50 hz servo
	myservo.attach(2, 1000, 2000); // attaches the servo on pin 2 to the servo object

  pixels.begin();
  pixels.setBrightness(100);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {

}
