

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


#define SPARKING 120
#define COOLING  80
bool gReverseDirection = false;
void Fire2012()
{

  if (addressableStrip == true) {
    // Array of temperature readings at each simulation cell
    static byte heat[NUM_LEDS];

    
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





