#include <Wire.h>
#include <INA226.h>
#include <Adafruit_SSD1306.h>

constexpr int SCREEN_WIDTH = 128;
constexpr int SCREEN_HEIGHT = 64;
constexpr uint8_t OLED_ADDRESS = 0x3C;
constexpr uint8_t INA226_ADDRESS = 0x40;
constexpr unsigned long UPDATE_INTERVAL_MS = 100;
constexpr char HEADER[] = "Bungle's Ammeter";

// Example calibration: verify the resistor fitted to your INA226 module.
constexpr float SHUNT_RESISTANCE_OHMS = 0.1f;
constexpr float MAX_CURRENT_A = 0.8f;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

INA226 ina226(INA226_ADDRESS);

void showSplashScreen() {
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);

    // Scrolling in "Bungle's"
    for (int x = SCREEN_WIDTH; x > 20; x -= 5) {
        display.clearDisplay();
        display.setCursor(x, 10);
        display.print("Bungle's");
        display.display();
        delay(50);
    }

    // Small gap & bouncing "Ammeter!"
    for (int y = 50; y >= 35; y -= 3) {
        display.clearDisplay();
        display.setCursor(20, 10);
        display.print("Bungle's");
        display.setCursor(30, y);
        display.print("Ammeter!");
        display.display();
        delay(50);
    }

    // Fun spark dots effect
    for (int i = 0; i < 10; i++) {
        display.clearDisplay();
        display.setCursor(20, 10);
        display.print("Bungle's");
        display.setCursor(30, 35);
        display.print("Ammeter!");

        // Random spark dots
        for (int j = 0; j < 5; j++) {
            display.drawPixel(random(10, 118), random(5, 58), SSD1306_WHITE);
        }

        display.display();
        delay(80);
    }

    delay(1000); // Pause before moving to main screen
}

void setup() {
    Serial.begin(115200);
    Wire.begin();

    if (!ina226.begin()) {
        Serial.println("Failed to find INA226");
        while (true) {
            delay(100);
        }
    }

    if (ina226.setMaxCurrentShunt(MAX_CURRENT_A, SHUNT_RESISTANCE_OHMS, false)
        != INA226_ERR_NONE) {
        Serial.println("INA226 calibration failed; check shunt and current settings");
        while (true) {
            delay(100);
        }
    }

    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
        Serial.println("SSD1306 initialization failed");
        while (true) {
            delay(100);
        }
    }

    display.setTextWrap(false);
    showSplashScreen();
}

void loop() {
    const float voltage = ina226.getBusVoltage();  // V
    const float current_mA = ina226.getCurrent_mA();
    const float power_mW = voltage * current_mA;  // V * mA = mW

    Serial.print("Voltage: ");
    Serial.print(voltage);
    Serial.print(" V, Current: ");
    Serial.print(current_mA);
    Serial.print(" mA, Power: ");
    Serial.print(power_mW);
    Serial.println(" mW");

    display.clearDisplay();

    // Header
    display.setTextSize(1);
    const int headerX = (SCREEN_WIDTH - (sizeof(HEADER) - 1) * 6) / 2;
    display.setCursor(headerX, 5);
    display.print(HEADER);

    // Voltage Display
    display.setTextSize(1);
    display.setCursor(10, 20);
    display.print("V:");
    display.setCursor(30, 20);
    display.print(voltage, 2);

    // Current Display - Shifted left, "mA" stays static on right
    display.setTextSize(2);
    const int currentX = 10;
    display.setCursor(currentX, 35);
    display.print(current_mA, 1);

    // Static "mA" on far right
    display.setCursor(SCREEN_WIDTH - 24, 35);
    display.print("mA");

    // Power Display
    display.setTextSize(1);
    display.setCursor(10, 55);
    display.print("P:");
    display.setCursor(30, 55);
    display.print(power_mW, 2);
    display.print(" mW");

    display.display();

    delay(UPDATE_INTERVAL_MS);
}
