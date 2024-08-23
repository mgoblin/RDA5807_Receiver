#ifndef RDA5807_RECEIVER_SETTINGS_H
#define RDA5807_RECEIVER_SETTINGS_H

/*########################
* Features enable/disable
#########################*/
#define DISPLAY_ENABLED
#define DEBUG_ENABLED
#define DEBUG_UART_SPEED 9600

/*########################
*  RDA Receiver settings
#########################*/
#define RADIO_DEFAULT_BAND RDA_FM_BAND_USA_EU // initial band 87–108 MHz (US/Europe)
#define RADIO_DEFAULT_FREQUENCY 10470 // initial radio station frequency 104.7MHz 
#define RADIO_DEFAULT_VOLUME 15 // initial sound volume. Sound volumes range is 0..15

/*########################
*   Buttons settings
#########################*/
#define CTRL_BUTTON_FREQ_UP_PIN 3
#define CTRL_BUTTON_FREQ_UP_PRESS_IS_LOW true
#define CTRL_BUTTON_FREQ_UP_ACTIVATE_INTERNAL_PULLUP true

#define CTRL_BUTTON_FREQ_DOWN_PIN 4
#define CTRL_BUTTON_FREQ_DOWN_PRESS_IS_LOW true
#define CTRL_BUTTON_FREQ_DOWN_ACTIVATE_INTERNAL_PULLUP false
 


/*#######################
*   OLED display settings
#########################*/
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET    -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C

/*#######################
*   EERPROM settings
#########################*/
#define FREQ_SAVING_ADDRESS 0

#endif