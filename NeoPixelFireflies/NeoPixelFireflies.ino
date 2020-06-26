#include <SPI.h>
#include <Adafruit_DotStar.h>

// Update this to represent total number of pixels.
#define NUM_PIXELS 1
// Update this to represent total of pixels that can light simultaneously.
#define MAX_GLOWING_PIXELS 1

class Firefly {
  private:
    float _brightness;
    float _fadeSpeed;
    float _hue;
    boolean _isGlowing;
    boolean _isFull;

  public:
    Firefly ();

    float hueCenter;
    float hueRange;
    float baseSpeed;
    boolean isGlowing();
    uint32_t getColor();

    void ignite();
    void full();
    void animate();
};

int off = 0;
int firefly = 1;
int full = 2;
int routine;
int lastRoutine = off;

#define DATAPIN    11 //7
#define CLOCKPIN   13 //8
#define BUTTONPIN  7
//DOTSTAR_BRG can be changed to RGB or whatever..

//Adafruit_DotStar strip(NUM_PIXELS, DATAPIN, CLOCKPIN, DOTSTAR_GBR);
Adafruit_DotStar strip(NUM_PIXELS, DOTSTAR_GBR); //(this line omits the pins because it uses SPI)

Firefly ff[NUM_PIXELS];

int buttonState;
int lastButtonState = LOW;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

void setup() {
  Serial.begin(9600);
  // I'm not sure if this is doing anything??
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000L)
    clock_prescale_set(clock_div_1); // Enable 16 MHz on Trinket
  #endif

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  pinMode(BUTTONPIN, INPUT);
}

void loop() {
  int reading = digitalRead(BUTTONPIN);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == HIGH) {
        if (routine < 2) {
          routine++;
        } else {
          routine = 0;
        }
        Serial.println(routine);
      }
    }
  }

  //////////////////////////////////////
  // System is turned off,
  // so fade to 0
  //////////////////////////////////////

  if (routine == off) {
    int glowingPixelCount = 0;

    for(int i = 0; i < NUM_PIXELS; i++) {
      ff[i].animate();
      strip.setPixelColor(i, ff[i].getColor());
    }
    strip.show();
    delay(10);
  }

  //////////////////////////////////////
  // System is turned to firefly,
  // so do the things
  //////////////////////////////////////

  if (routine == firefly) {
    int glowingPixelCount = 0;
    // do we have enough lit pixels?
    for(int i = 0; i < NUM_PIXELS; i++) {
      if(ff[i].isGlowing()) {
        glowingPixelCount++;
      }
    }

    for(int i = 0; i < NUM_PIXELS; i++) {
      if(!ff[i].isGlowing() && glowingPixelCount < MAX_GLOWING_PIXELS && random(100) < 5) {
        ff[i].ignite();
        glowingPixelCount++;
      }
      ff[i].animate();
      strip.setPixelColor(i, ff[i].getColor());
    }
    strip.show();
    delay(10);
  }

  //////////////////////////////////////
  // System is turned full bright,
  // so fade all to 255
  //////////////////////////////////////

  if (routine == full) {
    for(int i = 0; i < NUM_PIXELS; i++) {
      ff[i].full();
      ff[i].animate();
      strip.setPixelColor(i, ff[i].getColor());
    }
    strip.show();
    delay(10);
  }
  lastButtonState = reading;
}
