#include <SPI.h>
#include <Adafruit_DotStar.h>

#define PIN 6
#define NUM_PIXELS 1
#define MAX_GLOWING_PIXELS 1

class Firefly {
  private:
    float _brightness;
    float _fadeSpeed;
    float _hue;
    boolean _isGlowing;
    
  public:
    Firefly ();
	
    float hueCenter;
    float hueRange;
    float baseSpeed;
    boolean isGlowing();    
    uint32_t getColor();
    
    void ignite();
    void animate();    
};

#define DATAPIN    7
#define CLOCKPIN   8
Adafruit_DotStar strip(NUM_PIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);

Firefly ff[NUM_PIXELS];

void setup() {
  // I'm not sure if this is doing anything??
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000L)
    clock_prescale_set(clock_div_1); // Enable 16 MHz on Trinket
  #endif

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