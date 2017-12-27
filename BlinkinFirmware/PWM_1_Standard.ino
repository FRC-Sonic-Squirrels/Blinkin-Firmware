

void rainbow_RGB(){rainbow(RainbowColors_p);}
void rainbow_Party(){rainbow(PartyColors_p);}
void rainbow_Ocean(){rainbow(OceanColors_p);}
void rainbow_Cloud(){rainbow(CloudColors_p);}
void rainbow_Lava(){rainbow(LavaColors_p);}
void rainbow_Forest(){rainbow(ForestColors_p);}


void rainbow(CRGBPalette16 palette)
{
  if (addressableStrip == true) {

    uint8_t colorIndex = startIndex; //startIndex+1;
    uint8_t patternLength = map(patternAdj, 0, 30, 1, 10);
    
    // FastLED's built-in rainbow generator
    for( int i = 0; i < stripLength; i++) {
        leds[i] = ColorFromPalette( palette, colorIndex, brightness, currentBlending);
        colorIndex += patternLength; 
        //fill_rainbow( leds, NUM_LEDS, gHue, 7);
    }
  }
  else {
    //gHue = gHue + 1;
    gHue = gHue + (patternSpeed/5);

    displaySolid( ColorFromPalette(palette, gHue ));
    
    delay(5);
  }
}


void rainbowWithGlitter()
{
  if (addressableStrip == true) {
    // built-in FastLED rainbow, plus some random sparkly glitter
    rainbow(RainbowColors_p);
    //addGlitter(80, CRGB::White);
    addGlitter(255, CRGB::White);
  }
  else {
    //gHue = gHue + 1;
    // Use FastLED automatic HSV->RGB conversion

    rainbow(RainbowColors_p);
  }
}

void addGlitter( fract8 chanceOfGlitter, CRGB gColor)
{
  if (addressableStrip == true) {
    if ( random8() < chanceOfGlitter) {
      //leds[ random16(NUM_LEDS) ] += CRGB::Black;
      leds[ random8(stripLength) ] = gColor;
    }
  }
  else {

  }
}

void confetti()
{
  if (addressableStrip == true) {
    // random colored speckles that blink in and fade smoothly
    fadeToBlackBy( leds, stripLength, 10);
    int pos = random8(stripLength);
    leds[pos] += CHSV( gHue + random8(64), 200, 255);
  }
  else {
    //flash bright rand colors
  
  }

}

//void sinelon()
//{
//  // a colored dot sweeping back and forth, with fading trails
//  fadeToBlackBy( leds, NUM_LEDS, 20);
//  int pos = beatsin16( 13, 0, NUM_LEDS-1 );
//  leds[pos] += CHSV( gHue, 255, 192);
//}

void sinelon()
{
  if (addressableStrip == true) 
  {
      // a colored dot sweeping back and forth, with fading trails
      fadeToBlackBy( leds, stripLength, 40);
      int pos = beatsin8(15,0,stripLength-1);
      static int prevpos = 0;
      if( pos < prevpos ) 
      { 
        fill_solid( leds+pos, (prevpos-pos)+1, CHSV(gHue,220,255));
      } 
      else 
      { 
        fill_solid( leds+prevpos, (pos-prevpos)+1, CHSV( gHue,220,255));
      }
      prevpos = pos;
  }
}

void bpm_RGB(){bpm(RainbowColors_p);}
void bpm_Party(){bpm(PartyColors_p);}
void bpm_Ocean(){bpm(OceanColors_p);}
void bpm_Cloud(){bpm(CloudColors_p);}
void bpm_Lava(){bpm(LavaColors_p);}
void bpm_Forest(){bpm(ForestColors_p);}


void bpm(CRGBPalette16 palette)
{
  if (addressableStrip == true) {
    // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
    uint8_t BeatsPerMinute = map(patternSpeed, 0, 30, 30, 140);//62;
    uint8_t patternLength = map(patternAdj, 0, 30, 5, 30);
        
    //CRGBPalette16 palette = PartyColors_p;
    uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
    //uint8_t beat = beatsin8( map(patternSpeed, 0, 30, 10, 200), 0, 255);
    for ( int i = 0; i < stripLength; i++) { 
      //leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i *30));
      leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * patternLength));
    }
  }
  else {
    gHue = gHue + (patternSpeed/5);

    displaySolid( ColorFromPalette(palette, gHue ));
    
    delay(5);
  }
}


//Adapted from Fire2012 by Mark Kriegsman
//FastLED Demo Code: https://github.com/FastLED/FastLED/tree/master/examples/Fire2012

void Fire2012_Low()   { Fire2012(120 , 80);   }
void Fire2012_High()  { Fire2012(90, 60);    }


void Fire2012(uint8_t SPARKING, uint8_t COOLING)
{

  if (addressableStrip == true) {
    // Array of temperature readings at each simulation cell
    static byte heat[NUM_LEDS];
    
    // SPARKING: What chance (out of 255) is there that a new spark will be lit?
    // Higher chance = more roaring fire.  Lower chance = more flickery fire.
    // Default 120, suggested range 50-200.
    //uint8_t SPARKING  = map(patternAdj, 0, 30, 50, 200); //120
    
    // COOLING: How much does the air cool as it rises?
    // Less cooling = taller flames.  More cooling = shorter flames.
    // Default 50, suggested range 20-100 
    //uint8_t COOLING = map(patternSpeed, 0, 30, 20,100); //80

    bool gReverseDirection = false;
    
    // Step 1.  Cool down every cell a little
    for ( int i = 0; i < stripLength; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / stripLength) + 2));
    }

    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for ( int k = stripLength - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }

    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if ( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160, 255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for ( int j = 0; j < stripLength; j++) {
      CRGB color = HeatColor( heat[j]);
      int pixelnumber;
      if ( gReverseDirection ) {
        pixelnumber = (stripLength - 1) - j;
      } else {
        pixelnumber = j;
      }
      leds[pixelnumber] = color;
    }
  }
  else {
    //flash firey pulses
  }
}


//Adapted from Mark Kriegsman's code "TwinkleFOX"
//https://gist.github.com/kriegsman/756ea6dcae8e30845b5a
void drawTwinkles_RGB(){      drawTwinkles(RainbowColors_p);  }
void drawTwinkles_Party(){    drawTwinkles(PartyColors_p);    }
void drawTwinkles_Ocean(){    drawTwinkles(OceanColors_p);    }
void drawTwinkles_Cloud(){    drawTwinkles(CloudColors_p);    }
void drawTwinkles_Lava(){     drawTwinkles(LavaColors_p);     }
void drawTwinkles_Forest(){   drawTwinkles(ForestColors_p);   }


// Background color for 'unlit' pixels
// Can be set to CRGB::Black if desired.
CRGB gBackgroundColor = CRGB::Black; 
// Example of dim incandescent fairy light background color
// CRGB gBackgroundColor = CRGB(CRGB::FairyLight).nscale8_video(16);

// If COOL_LIKE_INCANDESCENT is set to 1, colors will 
// fade out slighted 'reddened', similar to how
// incandescent bulbs change color as they get dim down.
#define COOL_LIKE_INCANDESCENT 1

//  This function loops over each pixel, calculates the 
//  adjusted 'clock' that this pixel should use, and calls 
//  "CalculateOneTwinkle" on each pixel.  It then displays
//  either the twinkle color of the background color, 
//  whichever is brighter.


void drawTwinkles( CRGBPalette16 palette )
{
  currentPalette = palette;
  
  // "PRNG16" is the pseudorandom number generator
  // It MUST be reset to the same starting value each time
  // this function is called, so that the sequence of 'random'
  // numbers that it generates is (paradoxically) stable.
  uint16_t PRNG16 = 11337;
  
  uint32_t clock32 = millis();

  // Set up the background color, "bg".
  CRGB bg = gBackgroundColor; // just use the explicitly defined background color

  uint8_t backgroundBrightness = bg.getAverageLight();
  
  for( int i = 0; i < stripLength; i++) {//CRGB& pixel: leds) {
    PRNG16 = (uint16_t)(PRNG16 * 2053) + 1384; // next 'random' number
    uint16_t myclockoffset16= PRNG16; // use that number as clock offset
    PRNG16 = (uint16_t)(PRNG16 * 2053) + 1384; // next 'random' number
    // use that number as clock speed adjustment factor (in 8ths, from 8/8ths to 23/8ths)
    uint8_t myspeedmultiplierQ5_3 =  ((((PRNG16 & 0xFF)>>4) + (PRNG16 & 0x0F)) & 0x0F) + 0x08;
    uint32_t myclock30 = (uint32_t)((clock32 * myspeedmultiplierQ5_3) >> 3) + myclockoffset16;
    uint8_t  myunique8 = PRNG16 >> 8; // get 'salt' value for this pixel

    // We now have the adjusted 'clock' for this pixel, now we call
    // the function that computes what color the pixel should be based
    // on the "brightness = f( time )" idea.
    CRGB c = computeOneTwinkle( myclock30, myunique8);

    uint8_t cbright = c.getAverageLight();
    int16_t deltabright = cbright - backgroundBrightness;
    if( deltabright >= 32 || (!bg)) {
      // If the new pixel is significantly brighter than the background color, 
      // use the new color.
      leds[i] = c;
      //pixel = c;
    } else if( deltabright > 0 ) {
      // If the new pixel is just slightly brighter than the background color,
      // mix a blend of the new color and the background color
      leds[i] = blend( bg, c, deltabright * 8);
      //pixel = blend( bg, c, deltabright * 8);
    } else { 
      // if the new pixel is not at all brighter than the background color,
      // just use the background color.
      leds[i] = bg;
      //pixel = bg;
    }
  }
}

//  This function takes a time in pseudo-milliseconds,
//  figures out brightness = f( time ), and also hue = f( time )
//  The 'low digits' of the millisecond time are used as 
//  input to the brightness wave function.  
//  The 'high digits' are used to select a color, so that the color
//  does not change over the course of the fade-in, fade-out
//  of one cycle of the brightness wave function.
//  The 'high digits' are also used to determine whether this pixel
//  should light at all during this cycle, based on the TWINKLE_DENSITY.
CRGB computeOneTwinkle( uint32_t ms, uint8_t salt)
{
  // Overall twinkle density.
  // 0 (NONE lit) to 8 (ALL lit at once).  
  // Default is 5.
  uint8_t TWINKLE_DENSITY = map(patternAdj, 0, 30, 1, 8);

  // Overall twinkle speed.
  // 0 (VERY slow) to 8 (VERY fast).  
  // 4, 5, and 6 are recommended, default is 4.
  uint8_t TWINKLE_SPEED = map(patternSpeed, 0, 30, 3,7);

  
  uint16_t ticks = ms >> (8-TWINKLE_SPEED);
  uint8_t fastcycle8 = ticks;
  uint16_t slowcycle16 = (ticks >> 8) + salt;
  slowcycle16 += sin8( slowcycle16);
  slowcycle16 =  (slowcycle16 * 2053) + 1384;
  uint8_t slowcycle8 = (slowcycle16 & 0xFF) + (slowcycle16 >> 8);
  
  uint8_t bright = 0;
  if( ((slowcycle8 & 0x0E)/2) < TWINKLE_DENSITY) {
    bright = attackDecayWave8( fastcycle8);
  }

  uint8_t hue = slowcycle8 - salt;
  CRGB c;
  if( bright > 0) {
    c = ColorFromPalette( currentPalette, hue, bright, NOBLEND);
    if( COOL_LIKE_INCANDESCENT == 1 ) {
      coolLikeIncandescent( c, fastcycle8);
    }
  } else {
    c = CRGB::Black;
  }
  return c;
}


// This function is like 'triwave8', which produces a 
// symmetrical up-and-down triangle sawtooth waveform, except that this
// function produces a triangle wave with a faster attack and a slower decay:
//
//     / \ 
//    /     \ 
//   /         \ 
//  /             \ 
//

uint8_t attackDecayWave8( uint8_t i)
{
  if( i < 86) {
    return i * 3;
  } else {
    i -= 86;
    return 255 - (i + (i/2));
  }
}

// This function takes a pixel, and if its in the 'fading down'
// part of the cycle, it adjusts the color a little bit like the 
// way that incandescent bulbs fade toward 'red' as they dim.
void coolLikeIncandescent( CRGB& c, uint8_t phase)
{
  if( phase < 128) return;

  uint8_t cooling = (phase - 128) >> 4;
  c.g = qsub8( c.g, cooling);
  c.b = qsub8( c.b, cooling * 2);
}


//Adapted from Mark Kriegsman's code "ColorWavesWithPalettes"
//https://gist.github.com/kriegsman/8281905786e8b2632aeb
void colorwaves_RGB()     {   colorwaves(RainbowColors_p);   }
void colorwaves_Party()   {   colorwaves(PartyColors_p);     }
void colorwaves_Ocean()   {   colorwaves(OceanColors_p);     }
void colorwaves_Cloud()   {   colorwaves(CloudColors_p);     }
void colorwaves_Lava()    {   colorwaves(LavaColors_p);      }
void colorwavess_Forest() {   colorwaves(ForestColors_p);    }


void colorwaves(CRGBPalette16 palette )//CRGB* ledarray, uint16_t numleds, CRGBPalette16& palette) 
{
  static uint16_t sPseudotime = 0;
  static uint16_t sLastMillis = 0;
  static uint16_t sHue16 = 0;
 
  uint8_t sat8 = beatsin88( 87, 220, 250);
  uint8_t brightdepth = beatsin88( 341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88( 203, (25 * 256), (40 * 256));
  uint8_t msmultiplier = beatsin88(147, 23, 60);

  uint16_t hue16 = sHue16;//gHue * 256;
  uint16_t hueinc16 = beatsin88(113, 300, 1500);
  
  uint16_t ms = millis();
  uint16_t deltams = ms - sLastMillis ;
  sLastMillis  = ms;
  sPseudotime += deltams * msmultiplier;
  sHue16 += deltams * beatsin88( 400, 5,9);
  uint16_t brightnesstheta16 = sPseudotime;
  
  for( uint16_t i = 0 ; i < stripLength; i++) {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 / 256;
    uint16_t h16_128 = hue16 >> 7;
    if( h16_128 & 0x100) {
      hue8 = 255 - (h16_128 >> 1);
    } else {
      hue8 = h16_128 >> 1;
    }

    brightnesstheta16  += brightnessthetainc16;
    uint16_t b16 = sin16( brightnesstheta16  ) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);
    
    uint8_t index = hue8;
    //index = triwave8( index);
    index = scale8( index, 240);

    CRGB newcolor = ColorFromPalette( palette, index, bri8);

    uint16_t pixelnumber = i;
    pixelnumber = (stripLength-1) - pixelnumber;
    
    nblend( leds[pixelnumber], newcolor, 128);
  }
}



//void CylonBounce(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay){
//
//  if (addressableStrip == true) 
//  {
//    for(int i = 0; i < stripLength-EyeSize-2; i++) {
//      setAll(0,0,0);
//      setPixel(i, red/10, green/10, blue/10);
//      for(int j = 1; j <= EyeSize; j++) {
//        setPixel(i+j, red, green, blue); 
//      }
//      setPixel(i+EyeSize+1, red/10, green/10, blue/10);
//      showStrip();
//      delay(SpeedDelay);
//    }
//  
//    delay(ReturnDelay);
//  
//    for(int i = NUM_LEDS-EyeSize-2; i > 0; i--) {
//      setAll(0,0,0);
//      setPixel(i, red/10, green/10, blue/10);
//      for(int j = 1; j <= EyeSize; j++) {
//        setPixel(i+j, red, green, blue); 
//      }
//      setPixel(i+EyeSize+1, red/10, green/10, blue/10);
//      showStrip();
//      delay(SpeedDelay);
//    }
//    
//    delay(ReturnDelay);
//  }
//}

