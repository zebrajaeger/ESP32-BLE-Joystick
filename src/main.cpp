#include <Arduino.h>
#include <BleGamepad.h>

BleGamepad bleGamepad("Panohead Joystick", "zebrajaeger.de", 100);

#define ADC_X_PIN 39
#define ADC_Y_PIN 36
#define ADC_BATTERY_PIN 4
#define BATTERY_CHECK_PERIOD_MS 1000

unsigned long nextTriggerTimeMs = 0;

void readAndSendBatteryLevel();

void setup() {
  Serial.begin(115200);

  bleGamepad.begin(0,      // buttonCount
                   0,      // hatSwitchCount
                   true,   // includeXAxis
                   true,   // includeYAxis
                   false,  // includeZAxis
                   false,  // includeRzAxis
                   false,  // includeRxAxis
                   false,  // includeRyAxis
                   false,  // includeSlider1
                   false,  // includeSlider2
                   false,  // includeRudder
                   false,  // includeThrottle
                   false,  // includeAccelerator
                   false,  // includeBrake
                   false   // includeSteering
  );
  bleGamepad.setAutoReport(false);
}

void loop() {
  if (bleGamepad.isConnected()) {
    while (millis() > nextTriggerTimeMs) {
      readAndSendBatteryLevel();
      nextTriggerTimeMs = millis() + BATTERY_CHECK_PERIOD_MS;
    }

    int x = map(analogRead(ADC_X_PIN), 0, 4095, 32737, -32737);
    int y = map(analogRead(ADC_Y_PIN), 0, 4095, 32737, -32737);
    bleGamepad.setLeftThumb(x, y);

    bleGamepad.sendReport();
    delay(1);
  }
}

void readAndSendBatteryLevel() {
  Serial.println("readAndSendBatteryLevel");
  
  int sensorValue = analogRead(ADC_BATTERY_PIN);
  float u = sensorValue * (5.12 / 4095.0);
  int percent = (u / 4.2) * 100;

  Serial.print(u);
  Serial.print("V ||");
  Serial.print(percent);
  Serial.println("%");
  bleGamepad.setBatteryLevel(percent);
}