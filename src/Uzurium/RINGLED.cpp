#include "RINGLED.h"

CRGB leds[NUM_LEDS];
CRGB leds_before[NUM_LEDS];


RINGLED::RINGLED(){
}
void RINGLED::setup(int pin){
  //良い解決法求む
  //https://forum.arduino.cc/t/fastled-addleds-with-non-constant-pin-number/622162/8
  switch(pin){
    case 22:
      FastLED.addLeds<NEOPIXEL, 22>(leds,NUM_LEDS);
      break;
    case 26:
      FastLED.addLeds<NEOPIXEL, 26>(leds,NUM_LEDS);
      break;
    case 32:
      FastLED.addLeds<NEOPIXEL, 32>(leds,NUM_LEDS);
      break;
    default:
      Serial.println("RINGLED Unsupported Pin");
      break;
  }
  FastLED.setBrightness(25);
  Serial.println("RINGLED setup was completed.");
}
void RINGLED::setbrightness(int vol){
  FastLED.setBrightness(vol);
}

//fire
bool gReverseDirection = false;//fire
#define COOLING  55//fire
#define SPARKING 120//fire
void Fire2012(){
// Array of temperature readings at each simulation cell
  static uint8_t heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= NUM_LEDS - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUM_LEDS; j++) {
      CRGB color = HeatColor( heat[j]);
      int pixelnumber;
      if( gReverseDirection ) {
        pixelnumber = (NUM_LEDS-1) - j;
      } else {
        pixelnumber = j;
      }
      leds[pixelnumber] = color;
    }
}

//fire2
CRGBPalette16 gPal;
void Fire2012WithPalette(){
// Array of temperature readings at each simulation cell
  static uint8_t heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= NUM_LEDS - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUM_LEDS; j++) {
      // Scale the heat value from 0-255 down to 0-240
      // for best results with color palettes.
      uint8_t colorindex = scale8( heat[j], 240);
      CRGB color = ColorFromPalette( gPal, colorindex);
      int pixelnumber;
      if( gReverseDirection ) {
        pixelnumber = (NUM_LEDS-1) - j;
      } else {
        pixelnumber = j;
      }
      //leds[pixelnumber] = color;
      nblend(leds[pixelnumber],color,64);
    }
}
void RINGLED::fire2(int mode,int hue){
    // Add entropy to random number generator; we use a lot of it.
  random16_add_entropy( random());

  // Fourth, the most sophisticated: this one sets up a new palette every
  // time through the loop, based on a hue that changes every time.
  // The palette is a gradient from black, to a dark color based on the hue,
  // to a light color based on the hue, to white.
  //
  if(mode==0){
     static uint8_t hue_roop = 0;
     hue_roop++;
     CRGB darkcolor  = CHSV(hue_roop,255,192); // pure hue, three-quarters brightness
     CRGB lightcolor = CHSV(hue_roop,128,255); // half 'whitened', full brightness
     gPal = CRGBPalette16( CRGB::Black, darkcolor, lightcolor, CRGB::White);
  }
    // These are other ways to set up the color palette for the 'fire'.
  // First, a gradient from black to red to yellow to white -- similar to HeatColors_p
  if(mode==1){
     gPal = CRGBPalette16( CRGB::Black, CRGB::Red, CRGB::Yellow, CRGB::White);
  }
  if(mode==2){
  // Second, this palette is like the heat colors, but blue/aqua instead of red/yellow
     gPal = CRGBPalette16( CRGB::Black, CRGB::Blue, CRGB::Aqua,  CRGB::White);
  }
  // Third, here's a simpler, three-step gradient, from black to red to white
  if(mode ==3){
    gPal = CRGBPalette16( CRGB::Black, CRGB::Red, CRGB::White);
  }
    if(mode==4){
     hue+=150;//青色からスタートするためのオフセット
     CRGB darkcolor  = CHSV(hue,255,192); // pure hue, three-quarters brightness
     CRGB lightcolor = CHSV(hue,128,255); // half 'whitened', full brightness
     gPal = CRGBPalette16( CRGB::Black, darkcolor, lightcolor, CRGB::White);
  }
  Fire2012WithPalette(); // run simulation frame, using palette colors
  
  FastLED.show(); // display this frame
  FastLED.delay(1000 / FPS);
}


//pacifica
CRGBPalette16 pacifica_palette_1 = 
    { 0x000507, 0x000409, 0x00030B, 0x00030D, 0x000210, 0x000212, 0x000114, 0x000117, 
      0x000019, 0x00001C, 0x000026, 0x000031, 0x00003B, 0x000046, 0x14554B, 0x28AA50 };
CRGBPalette16 pacifica_palette_2 = 
    { 0x000507, 0x000409, 0x00030B, 0x00030D, 0x000210, 0x000212, 0x000114, 0x000117, 
      0x000019, 0x00001C, 0x000026, 0x000031, 0x00003B, 0x000046, 0x0C5F52, 0x19BE5F };
CRGBPalette16 pacifica_palette_3 = 
    { 0x000208, 0x00030E, 0x000514, 0x00061A, 0x000820, 0x000927, 0x000B2D, 0x000C33, 
      0x000E39, 0x001040, 0x001450, 0x001860, 0x001C70, 0x002080, 0x1040BF, 0x2060FF };




// Add one layer of waves into the led array
void pacifica_one_layer( CRGBPalette16& p, uint16_t cistart, uint16_t wavescale, uint8_t bri, uint16_t ioff)
{
  uint16_t ci = cistart;
  uint16_t waveangle = ioff;
  uint16_t wavescale_half = (wavescale / 2) + 20;
  for( uint16_t i = 0; i < NUM_LEDS; i++) {
    waveangle += 250;
    uint16_t s16 = sin16( waveangle ) + 32768;
    uint16_t cs = scale16( s16 , wavescale_half ) + wavescale_half;
    ci += cs;
    uint16_t sindex16 = sin16( ci) + 32768;
    uint8_t sindex8 = scale16( sindex16, 240);
    CRGB c = ColorFromPalette( p, sindex8, bri, LINEARBLEND);
    leds[i] += c;
  }
}
// Add extra 'white' to areas where the four layers of light have lined up brightly
void pacifica_add_whitecaps()
{
  uint8_t basethreshold = beatsin8( 9, 55, 65);
  uint8_t wave = beat8( 7 );
  
  for( uint16_t i = 0; i < NUM_LEDS; i++) {
    uint8_t threshold = scale8( sin8( wave), 20) + basethreshold;
    wave += 7;
    uint8_t l = leds[i].getAverageLight();
    if( l > threshold) {
      uint8_t overage = l - threshold;
      uint8_t overage2 = qadd8( overage, overage);
      leds[i] += CRGB( overage, overage2, qadd8( overage2, overage2));
    }
  }
}

// Deepen the blues and greens
void pacifica_deepen_colors()
{
  for( uint16_t i = 0; i < NUM_LEDS; i++) {
    leds[i].blue = scale8( leds[i].blue,  145); 
    leds[i].green= scale8( leds[i].green, 200); 
    leds[i] |= CRGB( 2, 5, 7);
  }
}
void pacifica_loop()
{
  // Increment the four "color index start" counters, one for each wave layer.
  // Each is incremented at a different speed, and the speeds vary over time.
  static uint16_t sCIStart1, sCIStart2, sCIStart3, sCIStart4;
  static uint32_t sLastms = 0;
  uint32_t ms = GET_MILLIS();
  uint32_t deltams = ms - sLastms;
  sLastms = ms;
  uint16_t speedfactor1 = beatsin16(3, 179, 269);
  uint16_t speedfactor2 = beatsin16(4, 179, 269);
  uint32_t deltams1 = (deltams * speedfactor1) / 256;
  uint32_t deltams2 = (deltams * speedfactor2) / 256;
  uint32_t deltams21 = (deltams1 + deltams2) / 2;
  sCIStart1 += (deltams1 * beatsin88(1011,10,13));
  sCIStart2 -= (deltams21 * beatsin88(777,8,11));
  sCIStart3 -= (deltams1 * beatsin88(501,5,7));
  sCIStart4 -= (deltams2 * beatsin88(257,4,6));

  // Clear out the LED array to a dim background blue-green
  fill_solid( leds, NUM_LEDS, CRGB( 2, 6, 10));

  // Render each of four layers, with different scales and speeds, that vary over time
  pacifica_one_layer( pacifica_palette_1, sCIStart1, beatsin16( 3, 11 * 256, 14 * 256), beatsin8( 10, 70, 130), 0-beat16( 301) );
  pacifica_one_layer( pacifica_palette_2, sCIStart2, beatsin16( 4,  6 * 256,  9 * 256), beatsin8( 17, 40,  80), beat16( 401) );
  pacifica_one_layer( pacifica_palette_3, sCIStart3, 6 * 256, beatsin8( 9, 10,38), 0-beat16(503));
  pacifica_one_layer( pacifica_palette_3, sCIStart4, 5 * 256, beatsin8( 8, 10,28), beat16(601));

  // Add brighter 'whitecaps' where the waves lines up more
  pacifica_add_whitecaps();

  // Deepen the blues and greens a bit
  pacifica_deepen_colors();
}

//clyon
void fadeall(){
  for(int i = 0; i < NUM_LEDS; i++){
    leds[i].nscale8(250);
  }
}
void RINGLED::inspection(int color){
  if(color==0){
    for(int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB(0 , 0, 255);
    }
  }else if(color==1){
    for(int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB(0 , 255, 0);
    }
  }else if(color==2){
    for(int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB(255 , 0, 0);
    }
  }
  FastLED.show(); 
}
void RINGLED::counter(int count){
  Serial.println(count);
  int color = 100 * count / NUM_LEDS;
  int turn = count/NUM_LEDS;
  int counter= count - turn * NUM_LEDS;
  leds[counter] =  CHSV(color, 255, 255);
//  for(int i = 1; i < NUM_LEDS;i++){
//    if(count<=i && i<NUM_LEDS)leds[i] = CHSV(0,0,0);
//	}
  if(count==0){
    for(int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CHSV(0 , 0, 0);
    }
  }
  FastLED.show(); 

}

void RINGLED::level(int count){
  for(int i = 0; i < NUM_LEDS;i++){
    if(0<=i && i<count)leds[i] = CHSV(i*10,255,255);
    if(count<=i && i<NUM_LEDS)leds[i] = CHSV(0,0,0);
	}
  FastLED.show(); 

}
void RINGLED::fade(){
  FastLED.show(); 
  fadeall();
  FastLED.delay(1000 / FPS);
}
void RINGLED::clyon(){
  static uint8_t hue = 0;
	//Serial.print("x");
	// First slide the led in one direction
	for(int i = 0; i < NUM_LEDS; i++) {
		// Set the i'th led to red 
		leds[i] = CHSV(hue++, 255, 255);
		// Show the leds
		FastLED.show(); 
		// now that we've shown the leds, reset the i'th led to black
		// leds[i] = CRGB::Black;
		fadeall();
		// Wait a little bit before we loop around and do it again
		//delay(10);
    FastLED.delay(1000 / FPS);
	}
	//Serial.print("x");
}
void RINGLED::fire(){

  Fire2012(); // run simulation frame
  
  FastLED.show(); // display this frame
  FastLED.delay(1000 / FPS);
}
void RINGLED::flash(int led_color){
  for(int i=0;i<NUM_LEDS;i++){
      leds[i] = CHSV(led_color, 255, 255);
      led_color = 100;
      led_color = constrain(led_color,0,255);
      if(led_color>=255)led_color=0;
    }
    //  led_color++;
  	//leds[0] = CRGB::White;
    FastLED.show();
    FastLED.delay(1000 / FPS);
}
void RINGLED::pacifica(){
    EVERY_N_MILLISECONDS( 20) {
    pacifica_loop();
    FastLED.show();
  }
}