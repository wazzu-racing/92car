#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SdFat_Adafruit_Fork.h>  // SD card & FAT filesystem library
#include <Adafruit_ImageReader.h>
#include <SPI.h>
#include <Adafruit_ST77XX.h>
#include <HT16K33Disp.h>
#include "constants.hpp"
#include <FlexCAN_T4.h>

FlexCAN_T4<TEENSY_CAN_SENSOR, RX_SIZE_256, TX_SIZE_16> can_sensor;


#define SD_CS 7 // SD card select pin
#define TFT_CS 10
#define TFT_RST 3 // Or set to -1 and connect to Arduino RESET pin
#define TFT_DC 26

#define SHIFT_PIN 1
#define WARN_PIN 2


#define TEXT_SIZE 7

// Custom color for orange (16-bit RGB565)
#define ST77XX_ORANGE 0xFD00  

HT16K33Disp disp;
byte add = 0x70;

SdFat SD;// SD card filesystem
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
Adafruit_ImageReader reader(SD);         

char low_oil_pressure_warning[] = "LOW OIL\nPRESSURE";
char high_oil_pressure_warning[] = "HIGH OIL\nPRESSURE";
char high_coolant_temp_warning[] = "HIGH\nCOOL\nTEMP";

int oil_pressure = 110;   //update with real variable
int coolant_temp = 200; //update with real variable
int battery_voltage = 12;   //update with real variable
int rpm = 500; //update with real variables


const int BUTTON_PIN = 16; //update with real pin number

// Screen state
enum ScreenState { LOGO, HOME, WARNING };
ScreenState currentScreen = HOME;

// Warning event structure
struct WarningEvent {
  const char* text;
  int* value;
  uint16_t color;
};

WarningEvent warningQueue[5];
int queueCount = 0;

// Timing for warnings (non-blocking)
unsigned long warningStartTime = 0;
const unsigned long WARNING_DURATION = 5000; // ms
bool warningActive = false;

// Forward declarations
void display_text(const char* text, int *int_value, uint16_t color);
void showHomeScreen();
void checkAndEnqueueWarnings();
void processWarnings();
bool isWarningConditionActive(const WarningEvent& w);
bool isWarningAlreadyQueued(const char* text);
bool isCurrentWarning(const char* text);
void showLogo();
void updateHomeScreen();


void setup() {
    can_sensor.begin();
  can_sensor.setBaudRate(500000);
  can_sensor.setMaxMB(16);
  can_sensor.enableFIFO();
  can_sensor.setFIFOFilter(ACCEPT_ALL);

  pinMode(BREAKOUT_TEENSY_PIN_AIR_TEMP, INPUT);
  pinMode(BREAKOUT_TEENSY_PIN_BRAKE_1, INPUT);
  pinMode(BREAKOUT_TEENSY_PIN_BRAKE_2, INPUT);
  pinMode(BREAKOUT_TEENSY_PIN_STEERING, INPUT);
  pinMode(BREAKOUT_TEENSY_PIN_SUSP_POT_FL, INPUT);
  pinMode(BREAKOUT_TEENSY_PIN_SUSP_POT_FR, INPUT);

  pinMode(SHIFT_PIN, OUTPUT);
  pinMode(WARN_PIN, OUTPUT);
  digitalWrite(SHIFT_PIN, HIGH);
  digitalWrite(WARN_PIN, HIGH);

  // Serial.begin(9600);

  tft.init(240, 320);

  if(!SD.begin(SD_CS, SD_SCK_MHZ(10))) { // Breakouts require 10 MHz limit due to longer wires
    // Serial.println(F("SD begin() failed"));
    for(;;); // Fatal error, do not continue
  }

   Wire.begin();
  disp.Init(add, 8);
  disp.Clear(add);
  delay(1000);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  tft.setRotation(1);
  tft.fillScreen(ST77XX_BLACK);

  tft.setCursor(0, 0);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(TEXT_SIZE);
  tft.setTextWrap(false);

  reader.drawBMP("/wsuRLogo.bmp", tft, 0, 0);
  delay(5000);

  currentScreen = HOME;
  // showHomeScreen();
  

}

void loop() {
  // Serial.println("loop");

  // digitalWrite(SHIFT_PIN, (millis()%20000) > 10000 ? LOW : HIGH);
  // digitalWrite(WARN_PIN, (millis()%20000) > 10000 ? LOW : HIGH);

  //   disp.Text(add, "PLAY");
  // delay(1000);
  // disp.Clear(add);
  // delay(1000);
  // disp.Text(add, "C=");
  // //------------------------------
  // for(byte MSD=0; MSD<=9; MSD++)
  // {
  //   disp.Num(add, 2, MSD);
  //   for(byte LSD=0; LSD<=9; LSD++)
  //   {
  //     disp.Num(add, 3, LSD);
  //     delay(200);
  //   }
  // }
  // //------------------------------
  // delay(1000);
  // disp.Clear(add);
  // delay(1000);

  // // In the future, oil_pressure and coolant_temp will be updated from datalogger

  // // 1) Check current sensor values and enqueue any new warnings
  // checkAndEnqueueWarnings();

  // // 2) Process warning state machine (non-blocking timing, queue, home screen)
  // processWarnings();

  // //shows logo when button is pressed
  // if (!warningActive && digitalRead(BUTTON_PIN) == LOW) {
  //   currentScreen = LOGO;
  //   showLogo();
  // }

  // //uopdates home screen when data changes
  // if (!warningActive && currentScreen == HOME) {
  //   updateHomeScreen();
  // }

  // can_sensor.events();

  CAN_message_t msg;
  int val;
  msg.id = SENSOR_CAN_ID_AMBAIR;
  msg.len = 8;
  val = analogRead(BREAKOUT_TEENSY_PIN_AIR_TEMP);
  memcpy(msg.buf, &val, sizeof(val));
  can_sensor.write(msg);
  Serial.println("w1");

  msg.id = SENSOR_CAN_ID_BRAKE_1;
  msg.len = 8;
  val = analogRead(BREAKOUT_TEENSY_PIN_BRAKE_1);
  memcpy(msg.buf, &val, sizeof(val));
  can_sensor.write(msg);
  Serial.println("w2");

  msg.id = SENSOR_CAN_ID_BRAKE_2;
  msg.len = 8;
  val = analogRead(BREAKOUT_TEENSY_PIN_BRAKE_2);
  memcpy(msg.buf, &val, sizeof(val));
  can_sensor.write(msg);

  msg.id = SENSOR_CAN_ID_STEERING;
  msg.len = 8;
  val = analogRead(BREAKOUT_TEENSY_PIN_STEERING);
  memcpy(msg.buf, &val, sizeof(val));
  can_sensor.write(msg);

  msg.id = SENSOR_CAN_ID_SUSPOT_FL;
  msg.len = 8;
  val = analogRead(BREAKOUT_TEENSY_PIN_SUSP_POT_FL);
  memcpy(msg.buf, &val, sizeof(val));
  can_sensor.write(msg);

  msg.id = SENSOR_CAN_ID_SUSPOT_FR;
  msg.len = 8;
  val = analogRead(BREAKOUT_TEENSY_PIN_SUSP_POT_FR);
  memcpy(msg.buf, &val, sizeof(val));
  can_sensor.write(msg);


}

// // ---------------------- Warning logic ----------------------

// float getMinOilPressure(int rpm) {

//     if (rpm <= 500) return 15;

//     else if (rpm <= 2000) return 15;

//     else if (rpm <= 4000) return 17;

//     else if (rpm <= 6000) return 20;

//     else if (rpm <= 8000) return 25;

//     else if (rpm <= 10000) return 27;

//     return 32; // above 10k
// }


// void checkAndEnqueueWarnings() {

//   float minOil = getMinOilPressure(rpm);

//   // Low oil pressure ( based on rpm) -> YELLOW
//   if (oil_pressure < minOil) {
//     if (!isWarningAlreadyQueued(low_oil_pressure_warning) &&
//         !isCurrentWarning(low_oil_pressure_warning)) {
//       warningQueue[queueCount++] = { low_oil_pressure_warning, &oil_pressure, ST77XX_YELLOW };
//     }
//   }

//   // High oil pressure (> 550 kPa/ 80 psi) -> ORANGE
//   if (oil_pressure > 80) {
//     if (!isWarningAlreadyQueued(high_oil_pressure_warning) &&
//         !isCurrentWarning(high_oil_pressure_warning)) {
//       warningQueue[queueCount++] = { high_oil_pressure_warning, &oil_pressure, ST77XX_ORANGE };
//     }
//   }

//   // High coolant temp (> 230 F) -> RED
//   if (coolant_temp > 230) {
//     if (!isWarningAlreadyQueued(high_coolant_temp_warning) &&
//         !isCurrentWarning(high_coolant_temp_warning)) {
//       warningQueue[queueCount++] = { high_coolant_temp_warning, &coolant_temp, ST77XX_RED };
//     }
//   }
// }

// void processWarnings() {
//   if (!warningActive) {
//     // No active warning
//     if (queueCount > 0) {
//       // Start showing the first warning in the queue
//       WarningEvent &w = warningQueue[0];
//       display_text(w.text, w.value, w.color);
//       warningStartTime = millis();
//       warningActive = true;
//       currentScreen = WARNING;
//     } else {
//       // No warnings queued -> ensure home screen is shown
//       if (currentScreen != HOME) {
//         showHomeScreen();
//         currentScreen = HOME;
//       }
//     }
//   } 
//   else {
//     // A warning is currently active
//     WarningEvent &w = warningQueue[0];

//     // Always update displayed value (live update)
//     display_text(w.text, w.value, w.color);

//     // Has the minimum display time passed?
//     if (millis() - warningStartTime >= WARNING_DURATION) {

//       //  CASE 1: More than one warning in queue → ALWAYS advance
//       if (queueCount > 1) {
//         // Remove current warning
//         for (int i = 1; i < queueCount; i++) {
//           warningQueue[i - 1] = warningQueue[i];
//         }
//         queueCount--;
//         warningActive = false;
//         return;  // Next warning will start on next loop
//       }

//       //  CASE 2: Only one warning → clear ONLY if condition is normal
//       if (!isWarningConditionActive(w)) {
//         // Remove warning
//         for (int i = 1; i < queueCount; i++) {
//           warningQueue[i - 1] = warningQueue[i];
//         }
//         queueCount--;
//         warningActive = false;

//         // Return to home screen
//         showHomeScreen();
//         currentScreen = HOME;
//       } 
//       else {
//         // Condition still active → restart timer, keep showing
//         warningStartTime = millis();
//       }
//     }
//   }
// }


// bool isWarningConditionActive(const WarningEvent& w) {

//   float minOil = getMinOilPressure(rpm);

//   if (w.text == low_oil_pressure_warning) {
//     return (*w.value < minOil);
//   } else if (w.text == high_oil_pressure_warning) {
//     return (*w.value > 80);
//   } else if (w.text == high_coolant_temp_warning) {
//     return (*w.value > 230);
//   }
//   return false;
// }

// bool isWarningAlreadyQueued(const char* text) {
//   for (int i = 0; i < queueCount; i++) {
//     if (warningQueue[i].text == text) return true;
//   }
//   return false;
// }

// bool isCurrentWarning(const char* text) {
//   if (!warningActive || queueCount == 0) return false;
//   return (warningQueue[0].text == text);
// }

// // ---------------------- Display helpers ----------------------

// void showHomeScreen() {
//   tft.fillScreen(ST77XX_BLACK);

//   tft.setCursor(0, 0);
//   tft.setTextColor(ST77XX_WHITE);
//   tft.setTextSize(6);

//   tft.print("Vltg: ");
//   tft.println(battery_voltage);

//   tft.setCursor(0, 90);
//   tft.print("Oil:  ");
//   tft.println(oil_pressure);

//   tft.setCursor(0, 180);
//   tft.print("Clnt: ");
//   tft.println(coolant_temp);

// }

// void updateHomeScreen() {
//   static int lastBattery = -9999;
//   static int lastOil = -9999;
//   static int lastCoolant = -9999;

//   // Only redraw if something changed
//   if (battery_voltage != lastBattery ||
//       oil_pressure != lastOil ||
//       coolant_temp != lastCoolant) {

//     tft.fillScreen(ST77XX_BLACK);
//     tft.setCursor(0, 0);
//     tft.setTextColor(ST77XX_WHITE);
//     tft.setTextSize(6);   // your chosen size

//     tft.print("Vltg: ");
//     tft.println(battery_voltage);

//     tft.setCursor(0, 90);
//     tft.print("Oil:  ");
//     tft.println(oil_pressure);

//     tft.setCursor(0, 180);
//     tft.print("Clnt: ");
//     tft.println(coolant_temp);


//     lastBattery = battery_voltage;
//     lastOil = oil_pressure;
//     lastCoolant = coolant_temp;
//   }
// }


void showLogo() {
  tft.fillScreen(ST77XX_BLACK);
  reader.drawBMP("/wsuRLogo.bmp", tft, 0, 0);
}


// void display_text(const char* text, int *int_value, uint16_t color) {
//   static const char* lastText = "";
//   static int lastValue = -999999;
//   static uint16_t lastColor = 0;

//   // Only redraw if something changed
//   if (lastText != text || lastValue != *int_value || lastColor != color) {

//     tft.fillScreen(ST77XX_BLACK);

//     tft.setCursor(0, 0);
//     tft.setTextColor(color);
//     tft.setTextSize(TEXT_SIZE);
//     tft.setTextWrap(false);

//     tft.println(text);

//     char str_buf[10];
//     sprintf(str_buf, "%d", *int_value);
//     tft.println(str_buf);

//     lastText = text;
//     lastValue = *int_value;
//     lastColor = color;
//   }
// }
