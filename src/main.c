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

//#define F_CPU 8000000UL         /* 8MHz crystal oscillator */

#define BUTTON_PORT PORTB       /* PORTx - register for button output */
#define BUTTON_PIN PINB         /* PINx - register for button input */
#define BUTTON_BIT PB3          /* bit for button input/output */

#define DEBOUNCE_TIME 25        /* time to wait while "de-bouncing" button */
#define LOCK_INPUT_TIME 250     /* time to wait after a button press */

#define  StripLength      2

struct cRGB led[StripLength];

struct Color {  uint8_t red; uint8_t green; uint8_t blue; };

int buttonstate = 0;


void setColor(struct Color color, int from, int to) 
{
    
    for (int i = 0; i <= StripLength; i++) {
        if ( i >= from &&  i < to )
        {
            led[i].r=color.green;
            led[i].g=color.red;
            //led[i].r=color.red;
            //led[i].g=color.green;
            led[i].b=color.blue;  
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

void init_io() 
{
   
    CLKPR=_BV(CLKPCE);
    CLKPR=0;			// set clock prescaler to 1 (attiny 25/45/85/24/44/84/13/13A)
    
    /* define Pin as configured in ws2812_config.h as LEDstrip Pin */
    DDRB|=_BV(ws2812_pin);
    
    /* turn on internal pull-up resistor for the switch */
    BUTTON_PORT |= _BV(BUTTON_BIT);
    

    struct Color black = {0,0,0};
    setColor(black ,0,StripLength);
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


int main(void)
{
    
    init_io();
    
    while(1)
    {
        if (button_is_pressed())
        {
            struct Color rgb;
            rgb.red = 255;
            rgb.green = 0;
            rgb.blue = 0;
            setColor(rgb,0,StripLength);
            _delay_ms(500);                         // wait for 500ms.
            
            rgb.red = 0;
            rgb.green = 0;
            rgb.blue = 255;
            setColor(rgb,1,2);
            _delay_ms(500);
            
            rgb.red = 0;
            rgb.green = 255;
            rgb.blue = 0;
            setColor(rgb,0,1);
            _delay_ms(500);
            
            
            rgb.red = 0;
            rgb.green = 0;
            rgb.blue = 0;
            setColor(rgb,0,StripLength);
            _delay_ms(500);
        }
        
    }
}
