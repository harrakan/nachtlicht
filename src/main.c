/*
 * Light_WS2812 library example - RGB_blinky
 *
 * cycles one LED through red, green, blue
 *
 * This example is configured for a ATtiny85 with PLL clock fuse set and
 * the WS2812 string connected to PB4.
 */

#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "light_ws2812.h"

#define F_CPU 8000000UL         /* 8MHz crystal oscillator */

#define BUTTON_PORT PORTB       /* PORTx - register for button output */
#define BUTTON_PIN PINB         /* PINx - register for button input */
#define BUTTON_BIT PB3          /* bit for button input/output */

#define DEBOUNCE_TIME 25        /* time to wait while "de-bouncing" button */
#define LOCK_INPUT_TIME 250     /* time to wait after a button press */

#define  StripLength      2
#define  Delay_time_ms    20
#define  Button_count    10

struct cRGB led[StripLength];
struct Color {  uint8_t red; uint8_t green; uint8_t blue; };

int button_pressed_counter;
int dimmer_counter;
struct Color global_color = {0,0,0};
unsigned char brigtness;

struct Color black = {0,0,0};    
struct Color red = {255,0,0};  
struct Color white = {255,255,255};

void setColor(struct Color color, int from, int to, unsigned char brightness) 
{
    
    for (int i = 0; i <= StripLength; i++) {
        if ( i >= from &&  i < to )
        {
            led[i].r=color.green * brightness/255.0;
            led[i].g=color.red * brightness/255.0;
            /*
             * original colors; depends on LEDs
            led[i].r=color.red * brightness/255;
            led[i].g=color.green * brightness/255;
            */
            led[i].b=color.blue * brightness/255.0;  
        }
        else
        {
            led[i].r=0;
            led[i].g=0;
            led[i].b=0;  
        }
    }
    ws2812_setleds(led,StripLength);
}

void init() 
{
    
    CLKPR=_BV(CLKPCE);
    CLKPR=0;			// set clock prescaler to 1 (attiny 25/45/85/24/44/84/13/13A)
    
    /* define Pin as configured in ws2812_config.h as LEDstrip Pin */
    DDRB|=_BV(ws2812_pin);
    
    /* turn on internal pull-up resistor for the switch */
    BUTTON_PORT |= _BV(BUTTON_BIT);
    
    
    setColor(black ,0,StripLength,0);
}

int button_is_pressed()
{
    /* the button is pressed when BUTTON_BIT is clear */
    if (bit_is_clear(BUTTON_PIN, BUTTON_BIT))
    {
        _delay_ms(DEBOUNCE_TIME);
        if (bit_is_clear(BUTTON_PIN, BUTTON_BIT)) return 1;
    }
    
    return 0;
}


void loop()
{
  if (button_is_pressed() )
  {
    button_pressed_counter ++;
  }
  else
  {if (button_pressed_counter == 0 ) // no button
    {
      dimmer_counter ++;
      if (brigtness > 0 && dimmer_counter > 10) {
      brigtness--;
      dimmer_counter  =0;      
      setColor(global_color, 0, StripLength, brigtness);
      }
    }
    else
    {
      if (button_pressed_counter < Button_count)
      {
        if (brigtness == 0) {
          brigtness = 200;
          global_color = red;
        }
        else {
          brigtness = 0;
        }
      }
      else
      {
        global_color = white;
        brigtness = 200;
      }
      button_pressed_counter = 0;
      setColor(global_color, 0, StripLength, brigtness);
    }
  }
  _delay_ms(Delay_time_ms);
}
      

int main(void)
{    
    init();
    
    while(1)
    {
        loop();               
    }
}
