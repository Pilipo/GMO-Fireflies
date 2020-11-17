#include <SPI.h>
#include <Adafruit_DotStar.h>

// Update this to represent total number of pixels.
#define NUM_PIXELS 3
// Update this to represent total of pixels that can light simultaneously.
#define MAX_GLOWING_PIXELS 1
// 0 - production; 1 - all LEDs full on; 2 - LEDs chase
#define TEST_MODE 0

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

int currentLED = 0;

#define DATAPIN    11 //7
#define CLOCKPIN   13 //8
#define BUTTONPIN  7
//DOTSTAR_BRG can be changed to RGB or whatever..

//Adafruit_DotStar strip(NUM_PIXELS, DATAPIN, CLOCKPIN, DOTSTAR_GBR);
Adafruit_DotStar strip(NUM_PIXELS, DOTSTAR_GBR); //(this line omits the pins because it uses SPI)

Firefly ff[NUM_PIXELS];

void setup() {
//  Serial.begin(9600);
  // I'm not sure if this is doing anything??
//  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000L)
//    clock_prescale_set(clock_div_1); // Enable 16 MHz on Trinket
//  #endif

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
    int glowingPixelCount = 0;
    // do we have enough lit pixels?
    for(int i = 0; i < NUM_PIXELS; i++) {
      if(ff[i].isGlowing()) {
        glowingPixelCount++;
      }
    }

    for(int i = 0; i < NUM_PIXELS; i++) {
      switch (TEST_MODE) {
        case 1:
          ff[i].ignite();
          glowingPixelCount++;
          break;

        case 2:
          if(currentLED == i && !ff[currentLED].isGlowing() && glowingPixelCount == 0) {
            ff[currentLED].ignite();
            glowingPixelCount++;
            if(currentLED == NUM_PIXELS-1) {
              currentLED = 0;
            } else {
              currentLED++;
            }
          }
          break;
          
        default:
          if(!ff[i].isGlowing() && glowingPixelCount < MAX_GLOWING_PIXELS && random(100) < 5) {
            ff[i].ignite();
            glowingPixelCount++;
          }
          break;        
      }
      ff[i].animate();
      strip.setPixelColor(i, ff[i].getColor());
    }
    strip.show();
    delay(10);
}
