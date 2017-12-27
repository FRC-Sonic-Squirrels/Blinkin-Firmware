#include "BlinkinFirmware.h"

void setup() {
  delay(1000); // 1 second delay for start-up

  // Setup a default palette and blend style
  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND; //or NOBLEND;

  // Hardware enabled inturrupt pin for PWM input
  pinMode(2, INPUT);
  digitalWrite(2, INPUT_PULLUP);

  // RGB Common Annode Strip pins -- connected to transistor, active high
  pinMode(REDPIN,   OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(BLUEPIN,  OUTPUT);

  digitalWrite(REDPIN, LOW);
  digitalWrite(GREENPIN, LOW);
  digitalWrite(BLUEPIN, LOW);

  // Status LED pins, common annode RGB LED, active low
  pinMode(sREDPIN,   OUTPUT);
  pinMode(sGREENPIN, OUTPUT);
  pinMode(sBLUEPIN,  OUTPUT);

  digitalWrite(sREDPIN, HIGH);
  digitalWrite(sGREENPIN, HIGH);
  digitalWrite(sBLUEPIN, LOW);

  pinMode(MODE_PIN, INPUT);
  digitalWrite(MODE_PIN, INPUT_PULLUP);  // set pullup on analog pin 0
  pinMode(SS_PIN, INPUT);
  digitalWrite(SS_PIN, INPUT_PULLUP);  // set pullup on analog pin 0

  initEEPROM();
  
  // Set-up the custom palette using the team colors COLOR1 and COLOR2
  SetupCustomPalette(colorList[COLOR1], colorList[COLOR2]);

  // Fill the pattern history buffer with values
  for (int i = 0 ; i < patternHistory.capacity() ; i++) {
    patternHistory.unshift(noSignalPatternDisplay);
  }

//  // FastLED declaration
//  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
//  // Enable at a safe starting brightness
//  FastLED.setBrightness( brightness );
//  //FastLED.setBrightness(map(analogRead(LENGTH_PIN), 0, 1023, 10, 255));

  FastLED.addLeds<LED_TYPE,LED_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(brightness);

  // Dual LEDs for indicating which type of LED strip is selected
  //LOW for 5V, HIGH for 12V
  pinMode(IND_PIN, OUTPUT);

  if (addressableStrip == true) {
    digitalWrite(IND_PIN, LOW);
  }
  else {
    digitalWrite(IND_PIN, HIGH);
    fill_solid( leds, NUM_LEDS, CRGB::Black );
    FastLED.show();
  }

  //FastLED.setMaxRefreshRate(200, true);

  //call timer set-up routine from interrupt tab
  timerConfiguration();

  // when pin D2(Servo input) goes high, call the rising function
  attachInterrupt(digitalPinToInterrupt(2), ISRrising, RISING);
}

//List of patterns.  Each is defined as a separate function in the specified tab.
typedef void (*FunctionList)();
typedef void (*CommandList)(char);

//List of commands.  Each is defined as a separate function in the specified tab.
//typedef void (*SimpleCommandList[])();
CommandList gCommands[] = {
  cmdNoStrip,                   //PWM_0_Command
  cmdNoStrip,                   //PWM_0_Command
  cmdNoStrip,                  //PWM_0_Command
  cmd5VStrip,                  //PWM_0_Command
  cmd12VStrip,                  //PWM_0_Command
  cmdIncreaseBrightness,                  //PWM_0_Command
  cmdDecreaseBrightness,                  //PWM_0_Command
  cmdChangeColor1,                  //PWM_0_Command
  cmdChangeColor2,                  //PWM_0_Command
  cmdChangeDefaultPattern,                  //PWM_0_Command
  cmdSetLinearBlend,                  //PWM_0_Command
  cmdSetNoBlend,                  //PWM_0_Command
  cmdChangeColor1,                  //PWM_0_Command
  cmdChangeColor1,                  //PWM_0_Command
  cmdChangeColor1,                  //PWM_0_Command
  cmdChangeColor1,                  //PWM_0_Command
  cmd12VStrip,                  //PWM_0_Command
  cmd12VStrip,                  //PWM_0_Command
  cmd5VStrip,                  //PWM_0_Command
  cmd5VStrip,                  //PWM_0_Command
};

FunctionList gPatterns[] = {
  rainbow_RGB,                  //PWM_1_Standard
  rainbow_Party,                  //PWM_1_Standard
  rainbow_Ocean,                  //PWM_1_Standard
  rainbow_Lava,                  //PWM_1_Standard
  rainbow_Cloud,                  //PWM_1_Standard
  rainbow_Forest,                  //PWM_1_Standard
  rainbowWithGlitter,           //PWM_1_Standard
  confetti,                     //PWM_1_Standard
  sinelon,                      //PWM_1_Standard
  bpm,                          //PWM_1_Standard
  bpm_RGB,                  //PWM_1_Standard
  bpm_Party,                  //PWM_1_Standard
  bpm_Ocean,                  //PWM_1_Standard
  bpm_Lava,                  //PWM_1_Standard
  rainbow_Cloud,                  //PWM_1_Standard
  bpm_Forest,                  //PWM_1_Standard
  Fire2012,                     //PWM_1_Standard
  EndtoEndStaticBlend_1,        //PWM_2_Color1
  EndtoEndBlend_2,              //PWM_3_Color2
  EndtoEndStaticBlend_2,        //PWM_4_Color2
  teamSparkle,                //PWM_4_Color1_2
  rainbow_Team,                 //PWM_4_Color1_2
  bpm_Team,                     //PWM_4_Color1_2
  EndtoEndBlend,                //PWM_4_Color1_2
  EndtoEndStaticBlend,          //PWM_4_Color1_2
  testPattern,                  //PWM_5_Solid
  Red,                          //PWM_5_Solid
  Blue,                         //PWM_5_Solid
  Green,                        //PWM_5_Solid
  Red,                          //PWM_5_Solid
  Blue,                         //PWM_5_Solid
  Green,                        //PWM_5_Solid
  Red,                          //PWM_5_Solid
  Blue,                         //PWM_5_Solid
  Green,                        //PWM_5_Solid
  Red,                          //PWM_5_Solid
  Blue,                         //PWM_5_Solid
  Green,                        //PWM_5_Solid
  Red,                          //PWM_5_Solid
  Blue,                         //PWM_5_Solid
  Green,                        //PWM_5_Solid
  Red,                          //PWM_5_Solid
  Blue,                         //PWM_5_Solid
  Green,                        //PWM_5_Solid
  Blue,                          //PWM_5_Solid
  Blue,                         //PWM_5_Solid
  Green,                        //PWM_5_Solid
  Blue ,                          //PWM_5_Solid
  Blue,                         //PWM_5_Solid
  Green,                        //PWM_5_Solid
  Blue,                          //PWM_5_Solid
  Blue,                         //PWM_5_Solid
  Green,                        //PWM_5_Solid
  Red,                          //PWM_5_Solid
  Blue,                         //PWM_5_Solid
  Green,                        //PWM_5_Solid
  Red,                          //PWM_5_Solid
  Blue,                         //PWM_5_Solid
  Green,                        //PWM_5_Solid
  Red,                          //PWM_5_Solid
  Blue,                         //PWM_5_Solid
  Green,                        //PWM_5_Solid
  Red,                          //PWM_5_Solid
  Blue,                         //PWM_5_Solid
  Green,                        //PWM_5_Solid
  Red,                          //PWM_5_Solid
  Blue,                         //PWM_5_Solid
  Green,                        //PWM_5_Solid
  Red,                          //PWM_5_Solid
  Blue,                         //PWM_5_Solid
  Green,                        //PWM_5_Solid
  Red,                          //PWM_5_Solid
  Blue,                         //PWM_5_Solid
  Green,                        //PWM_5_Solid
  Red,                          //PWM_5_Solid
  Blue,                         //PWM_5_Solid
  Violet,                        //PWM_5_Solid
  Violet,                        //PWM_5_Solid
  HotPink,                      //PWM_5_Solid
  DarkRed,                      //PWM_5_Solid
  Red,                          //PWM_5_Solid
  RedOrange,                    //PWM_5_Solid
  Orange,                       //PWM_5_Solid
  Gold,                         //PWM_5_Solid
  Yellow,                       //PWM_5_Solid
  LawnGreen,                    //PWM_5_Solid
  Lime,                         //PWM_5_Solid
  DarkGreen,                    //PWM_5_Solid
  Green,                        //PWM_5_Solid
  BlueGreen,                    //PWM_5_Solid
  Aqua,                         //PWM_5_Solid
  SkyBlue,                      //PWM_5_Solid
  DarkBlue,                     //PWM_5_Solid
  Blue,                         //PWM_5_Solid
  BlueViolet,                   //PWM_5_Solid
  Violet,                       //PWM_5_Solid
  White,                        //PWM_5_Solid
  Gray,                         //PWM_5_Solid
  DarkGray,                     //PWM_5_Solid
  Black                        //PWM_5_Solid
};


void loop() {

    //ledUpdate();
    
  if ((inPulse == false) && (updatedLEDs == false)) {
    detachInterrupt(2);
    ledUpdate();
  }


  
  EVERY_N_MILLISECONDS( 20 ) {
    startIndex = startIndex + patternSpeed; /* motion speed */ 

    // slowly cycle the "base color" through the rainbow, used for some patterns
    gHue++;

    buttonHandler();
   
  }


  // do some periodic updates
  EVERY_N_MILLISECONDS( 200 ) {

    if (inSetup) {
      readUserInputs();
    }

    //Pot 1 - User set pattern density, not used for all patterns
    patternAdj = constrain(map(analogRead(COLOR1_PIN), 0, 1024, 0, 30), 0, 30);

    //Pot 2 - User set pattern movement speed, not used for all patterns
    patternSpeed = constrain(map(analogRead(COLOR2_PIN), 0, 1024, 0, 30), 0, 30);
    //startIndex = startIndex + patternSpeed; /* motion speed */

    if ((noSignal == true))
      setStatusNoSignal();
    else
      setStatusMode();

    //Pot 3 - user set strip brightness
    if ((cmdBrightness == false) && (inSetup == false)) {
      brightness = map(analogRead(LENGTH_PIN), 0, 1024, 10, calculate_max_brightness_for_power_vmA(leds, stripLength, 255, 5, 4800));
      FastLED.setBrightness(brightness);
    }
  }
}


void ledUpdate()
{

  patternStable = true;

  //check that the pattern value has been stable. Pattern value is the pattern requested by the user via PWM pulse width measurement.
  for (int i = 0 ; i < patternHistory.capacity() ; i++) {
    if (patternHistory[0] != patternHistory[i])
      patternStable = false; //if any of the elements of the patternHistory buffer don't match, the pattern is not stable
  }

  if ((cmdDisableOutput == false) && (inSetup == false))
  {
    if (patternStable) //if the pattern is stable, output and update the current pattern value
    {
      currentPattern = patternHistory[0];
      gPatterns[currentPattern]();
    }
    else //if pattern is not stable, use the previous stable pattern
    {
      gPatterns[currentPattern]();
    }
  }
  else
  {
    gPatterns[testPatternDisplay]();
  }

  // send the 'leds' array out to the actual LED strip
//  //30uS per LED for addressable
  if (addressableStrip == true) {
    //update LED display
    FastLED.show();
  }


  updatedLEDs = true; //set status flag to show that LEDs have been updates to latest requested value

  attachInterrupt(digitalPinToInterrupt(2), ISRrising, RISING);
}


// This function takes the incoming RGB values and outputs the values
// on three analog PWM output pins to the r, g, and b values respectively.
void showAnalogRGB( const CRGB& rgb)
{
  analogWrite(REDPIN,   rgb.r );
  analogWrite(GREENPIN, rgb.g );
  analogWrite(BLUEPIN,  rgb.b );
}

void displaySolid( CRGB rgb)
{
  if (addressableStrip == true) {
//    fill_solid( leds, NUM_LEDS, rgb );
    fill_solid( leds, stripLength, rgb );
  }
  else {
    showAnalogRGB( rgb.fadeLightBy(255 - map(analogRead(LENGTH_PIN), 0, 1024, 20, 255)) );
  }
}


void toggleStripSelect()
{
  cmdDisableOutput = false;
  if (addressableStrip)
  {
    Black();
    FastLED.show();
    digitalWrite(IND_PIN, HIGH);
    addressableStrip = false;

    //EEPROM write takes 3.3ms
    if(writeEEPROM)
        EEPROM.write(SS_EE, addressableStrip);
  }
  else
  {
    Black();
    digitalWrite(IND_PIN, LOW);
    addressableStrip = true;

    //EEPROM write takes 3.3ms
    if(writeEEPROM)
      EEPROM.write(SS_EE, addressableStrip);
  }

}


void setStripSelect(bool newStripState)
{
  cmdDisableOutput = false;
  if (newStripState != addressableStrip)
    toggleStripSelect();
}


