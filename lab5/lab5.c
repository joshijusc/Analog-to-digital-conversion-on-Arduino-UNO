/********************************************
 *
 *  Name: Jason Joshi
 *  Email: joshij@usc.edu
 *  Section: 11am Friday
 *  Assignment: Lab 5 - Analog-to-digital conversion
 *
 ********************************************/

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>

#include "lcd.h"
#include "adc.h"

void rand_init(void);

int main(void)
{

    // Initialize the LCD
    lcd_init();
    lcd_writecommand(1);
    // /* Call lcd_stringout to print out your name */
    // char* name = "Jason Joshi";
    // lcd_stringout(name);

    // /* Use snprintf to create a string with your birthdate */
    // int x=11; int y=05; int z=2003;
    // char bday[11];
    // snprintf(bday, 12, "%d-%d-%d", x, y, z);
    // /* Use lcd_moveto to start at an appropriate column 
    //     in the bottom row to appear centered */
    // lcd_moveto(1, 0);

    // /* Use lcd_stringout to print the birthdate string */
    // lcd_stringout(bday);
    // /* Delay 1 second */
    // _delay_ms(1000);



    // Initialize the ADC
    adc_init();


    // Initialize the random number function
    rand_init();

    // Write splash screen and delay for 1 second
    char buf[12];
    char* title = "EE109 Lab 5";
    snprintf(buf, 12, "%s", title);
    lcd_moveto(0, 0);
    lcd_stringout(buf);
    char* name = "Jason Joshi";
    snprintf(buf, 12, "%s", name);
    lcd_moveto(1, 0);
    lcd_stringout(buf);
    _delay_ms(1000);
    lcd_writecommand(1);


    // Find three random numbers and display on top line
    int min = 0; int max = 31;
    int range = max - min + 1;
    int x = rand() % range + min;
    int y = rand() % range + min;
    int z = rand() % range + min;
    int x_; int y_; int z_;

    char randNums[15];
    snprintf(randNums, 15, "%d %d %d", x, y, z);
    lcd_stringout(randNums);
    lcd_moveto(1, 0);

    // Show the number selector ('>') on the bottom line
    char numSelect[2];
    snprintf(numSelect, 2, "%s", ">");
    lcd_stringout(numSelect);
    lcd_moveto(1, 13);


    int pos = 0;
    while (1) {                 // Loop forever
        uint8_t adc_result = adc_sample(3)/8;
        char buf[5];
        snprintf(buf, 5, "%4d", adc_result);
        lcd_moveto(1, 12);
        lcd_stringout(buf);


	// Convert ADC channel for buttons to 0-255

        // If right or left button pressed, move '>' to new position
        if (adc_sample(0) > 150 && adc_sample(0) < 160 && pos > 1){ //Left button pressed
            lcd_moveto(1, pos);
            lcd_stringout(" "); //Erase cursor
            pos -= 3;
            lcd_moveto(1, pos);
            lcd_stringout(">");
            _delay_ms(200);
        } else if (adc_sample(0) > -10 && adc_sample(0) < 10 && pos < 6){ //Right button pressed
            lcd_moveto(1, pos);
            lcd_stringout(" "); //Erase cursor
            pos += 3;
            lcd_moveto(1, pos);
            lcd_stringout(">");
            _delay_ms(200);
        } else if (adc_sample(0) > 200 && adc_sample(0) < 210){ //Select button pressed
            char place[5];
            snprintf(place, 5, "%d", adc_result);
            lcd_moveto(1, pos+1);
            lcd_stringout(place);
            if (pos == 0){
                x_ = adc_result;
            } else if (pos == 3){
                y_ = adc_result;
            } else if (pos == 6){
                z_ = adc_result;
            }
            _delay_ms(200);
        }

        // Read potentiometer ADC channel

	// Convert ADC channel for potentiometer to 0-255, change to 0-31 and display


	// If select button pressed copy number to one of the lock inputs positions


	// Check to see if all three lock inputs match the combination
        if (x == x_ && y == y_ && z == z_){
            char unlock[15];
            snprintf(unlock, 15, "%s", "Unlock");
            lcd_moveto(0, 10);
            lcd_stringout(unlock);
            while(1){}
        }
    }

    return 0;   /* never reached */
}

void rand_init()
{
    int16_t seed = 0;
    uint8_t i, j, x;

    // Build a 15-bit number from the LSBs of an ADC
    // conversion of the channels 1-5, 3 times each
    for (i = 0; i < 3; i++) {
	for (j = 1; j < 6; j++) {
	    x = adc_sample(j);
	    x &= 1;	// Get the LSB of the result
	    seed = (seed << 1) + x; // Build up the 15-bit result
	}
    }
    srand(seed);	// Seed the rand function
}
