#pragma once

#include "ThreadedManager.hpp"


#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>
#include <SdFat_Adafruit_Fork.h>
#include <Adafruit_ImageReader.h>

class DashboardManager: public ThreadedManager {
public:
    DashboardManager();
    void loop();
    void setup();
private:
    Adafruit_ST7789 tft;
    Adafruit_ImageReader reader;
    SdFat SD;

    char low_oil_pressure_warning[20] = "LOW OIL\nPRESSURE";
    char high_oil_pressure_warning[20] = "HIGH OIL\nPRESSURE";
    char high_coolant_temp_warning[20] = "HIGH\nCOOL\nTEMP";

    int oil_pressure = 90;
    int coolant_temp = 200;
    int battery_voltage = 12;

    const int BUTTON_PIN = 2;

    enum ScreenState { LOGO_SCREEN, HOME, WARNING};
    ScreenState currentScreen = HOME;

    // Warning event structure
    struct WarningEvent {
      const char* text;
      int* value;
      uint16_t color;
    };

    WarningEvent warningQueue[5];
    int queueCount = 0;

    unsigned long warningStartTime = 0;
    const unsigned long WARNING_DURATION = 5000; // ms
    bool warningActive = false;


    void display_text(const char* text, int *int_value, uint16_t color);
    void showHomeScreen();
    void checkAndEnqueueWarnings();
    void processWarnings();
    bool isWarningConditionActive(const WarningEvent& w);
    bool isWarningAlreadyQueued(const char* text);
    bool isCurrentWarning(const char* text);
    void showLogo();
    void updateHomeScreen();
};
