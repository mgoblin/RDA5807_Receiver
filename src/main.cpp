#include <Arduino.h>
#include <OneButtonTiny.h>

#include <RDA5807.h> 
#include <GyverOLED.h>

#include <SplashScreen.h>

#include <EEPROM.h>

#include <settings.h>
#include <debug.h>

const uint8_t antenna[] PROGMEM = { 0x06, 0x09, 0x11, 0xff, 0xff, 0x11, 0x09, 0x06 };
const uint8_t signal100[] PROGMEM = { 0x00, 0xe0, 0x00, 0xf0, 0x00, 0xfc, 0x00, 0xff };

typedef const char Page[10];
/*
  Functions forward declaration
*/
void initRDA5807();
void initCtrl();
void initDisplay();
void showRadioState();
void clearDisplay();
void renderSplashPage(const Page*);
void saveFrequency();
uint16_t readFrequency();

using namespace mg;

#define SPLASH_SCREENS_COUNT 1
const Page PROGMEM greeting = "MG FM 1.0";
const Screen<Page> screen1
{
	.page = &greeting,
	.displayIntervalMs = 3000
};

Screen<Page> screensArray[] = { screen1 };
SplashScreen<Page> splashScreen
(
  screensArray, (byte) SPLASH_SCREENS_COUNT, clearDisplay, renderSplashPage
);

RDA5807 rx; // FM Reciver module
GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oled; // OLED 128x64

OneButtonTiny buttonFreqUp(CTRL_BUTTON_FREQ_UP_PIN, CTRL_BUTTON_FREQ_UP_PRESS_IS_LOW, CTRL_BUTTON_FREQ_UP_ACTIVATE_INTERNAL_PULLUP); // Freq up button
OneButtonTiny buttonFreqDown(CTRL_BUTTON_FREQ_DOWN_PIN, CTRL_BUTTON_FREQ_DOWN_PRESS_IS_LOW, CTRL_BUTTON_FREQ_DOWN_ACTIVATE_INTERNAL_PULLUP); // Freq down button

bool isRadioStateShow = false;

void setup() 
{
  debug_init(DEBUG_UART_SPEED);

  #ifdef DISPLAY_ENABLED
  initDisplay();
  debug_print(F("Start splash screen"));
  splashScreen.display();
  #endif

  initRDA5807();
  initCtrl();
}

void loop() 
{
  buttonFreqUp.tick();
  buttonFreqDown.tick();
  
  #ifdef DISPLAY_ENABLED
  auto displayStatus = splashScreen.tick();
  if (!isRadioStateShow && displayStatus.state == State::FINISHED)
  {
    debug_print(F("Finish splash screen"));
    showRadioState();
    isRadioStateShow = true;
  }
  #endif
}

void initDisplay()
{
  debug_print(F("Init display"));

  oled.init();
  clearDisplay();

  oled.setScale(2);
  oled.setCursor(0, 4);

}

/*
  Initiialize RDA5807 FM receiver module.

  Set initial frequency and sound volume
*/
void initRDA5807() 
{
  debug_print(F("Init rx chip"));

  rx.setup();
  
  rx.setBand(RADIO_DEFAULT_BAND);
  
  rx.setFrequency(readFrequency());

  rx.setVolume(RADIO_DEFAULT_VOLUME);
  rx.setMono(false);
  rx.setMute(false);
  rx.setAFC(true);

  String strFreq = String(rx.getFrequency());
  debug_print(F("rx chip frequency "));
  debug_print(strFreq);
}

uint16_t readFrequency()
{
  debug_print(F("Read frequency from EEPROM"));
  uint16_t frequency = 0;
  EEPROM.get(FREQ_SAVING_ADDRESS, frequency);
  return frequency == 65535 ? RADIO_DEFAULT_FREQUENCY : frequency;
}

/*
  Save FM receiver current frequence to EERPOM
 */
void saveFrequency()
{
  debug_print(F("Save frequency to EEPROM"));
  EEPROM.put(FREQ_SAVING_ADDRESS, rx.getFrequency());

  #ifdef DISPLAY_ENABLED
  oled.clear();
  oled.setScale(2);
  oled.setCursor(0, 4);

  oled.print(F("Saved"));
  delay(1000);

  showRadioState();
  #endif
}

/*
  Initialize control buttons - frequency up and down buttons

  Click - select next/previous radio 
*/
void initCtrl() 
{
  debug_print(F("Init control buttons"));

  buttonFreqUp.attachClick([]() 
  {
    debug_print(F("Frequency up"));
    
    rx.seek(RDA_SEEK_WRAP, RDA_SEEK_UP, showRadioState);
    #ifdef DISPLAY_ENABLED
    if (splashScreen.getState() == State::DISPLAYING) 
    {
      splashScreen.finish();
    }
    #endif

    #ifdef DEBUG_ENABLED
    debug_print(String(rx.getFrequency()));
    #endif
  });

  buttonFreqDown.attachClick([]() 
  {
    debug_print(F("Frequency down"));
    
    rx.seek(RDA_SEEK_WRAP, RDA_SEEK_DOWN, showRadioState);
    
    #ifdef DISPLAY_ENABLED
    if (splashScreen.getState() == State::DISPLAYING) 
    {
      splashScreen.finish();
    }
    #endif

    #ifdef DEBUG_ENABLED
    debug_print(String(rx.getFrequency()));
    #endif
  });

  buttonFreqDown.attachLongPressStart(saveFrequency);
}

/*
  Print FM frequency LCD
*/
void showRadioState() 
{
  #ifdef DISPLAY_ENABLED
  clearDisplay();
  oled.home();
  oled.setScale(2);
  oled.setCursor(0, 4);

  oled.print((double)rx.getRealFrequency() / 100);
  oled.print(F(" MHz"));

  oled.drawBitmap(0, 0, antenna, 8, 8);
  oled.drawBitmap(10, 0, signal100, 8, 8);
  #endif

}

void clearDisplay()
{
  oled.clear();
}


void renderSplashPage(const Page* page)
{
  oled.home();
  oled.setScale(2);
  oled.setCursor(0, 4);

  char buffer[11];
  strcpy_P(buffer, *page); 

  oled.print(buffer);
}
