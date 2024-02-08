#include <SevSegShift.h>
#include <LiquidCrystal_I2C.h>

const int SHIFT_PIN_SHCP = 6;
const int SHIFT_PIN_STCP = 5;
const int SHIFT_PIN_DS = 4;

int secTimer = 0;
int hourMinTimer = 0;


SevSegShift sevseg(SHIFT_PIN_DS, SHIFT_PIN_SHCP, SHIFT_PIN_STCP, 1, true);
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x3F for a 16 chars and 2 line display


unsigned long previousMillis = 0;
unsigned long interval = 1000;


void setup() {
  byte numDigits = 4;
  byte digitPins[] = {12, 11, 10, 9}; // These are the PINS of the ** Arduino **
  byte segmentPins[] = {0, 1, 2, 3, 4, 5, 6, 7}; // these are the PINs of the ** Shift register **
  bool resistorsOnSegments = false; // 'false' means resistors are on digit pins
  byte hardwareConfig = COMMON_CATHODE; // See README.md for options
  bool updateWithDelays = false; // Default 'false' is Recommended
  bool leadingZeros = true; // Use 'true' if you'd like to keep the leading zeros
  bool disableDecPoint = false; // Use 'true' if your decimal point doesn't exist or isn't connected
  
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments, updateWithDelays, leadingZeros, disableDecPoint);
  sevseg.setBrightness(100);

  lcd.init();
  lcd.clear();         
  lcd.backlight();      // Make sure backlight is on
}

void loop() {
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;
    incSeconds();
  }
  hourMins();
  secDisplay();
}

void hourMins() {
  sevseg.setNumber(hourMinTimer);
  sevseg.refreshDisplay(); // Must run repeatedly; don't use blocking code (ex: delay()) in the loop() function or this won't work right
}

void secDisplay() {
  lcd.setCursor(0,0);   //Set cursor to character 0 on line 0
  lcd.print(secTimer);
}

void incHourMin() {
  hourMinTimer++;
  if (hourMinTimer%100 == 60) {
    hourMinTimer += 40;
  }
  if (hourMinTimer == 1260) {
    hourMinTimer = 0;
  }
}

void incSeconds() {
  secTimer++;
  if (secTimer >= 60) {
    lcd.clear();
    secTimer = 0;
    incHourMin();
  }
}