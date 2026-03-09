#include "pico/stdlib.h"

#define D1 0x3
#define D2 0x2
#define D3 0x0
#define D4 0x10

#define G 0x11

#define A 0x4
#define B 0x1
#define C 0x12
#define D 0x14
#define E 0x15
#define F 0x5

/*
7 Segment Display 4 digits 20mm.
________________________________
This project aims to teach how to use interrupts & timers on the Raspberry Pi Pico by utilizing a 4-digit 7-segment display.
The display is connected to the Pico using GPIO pins,
and the program will update the display to show a counting sequence from 0000 to 9999.
The counting will be done using a timer interrupt that triggers every second,
allowing the display to update without blocking the main loop.
This project will also demonstrate how to handle multiple digits and manage the timing of updates effectively.


*/

// Timer interrupt less stress on the MCU, this allows us to do watch out for our delays in the background without constant checks
volatile int current_display[4] = {1, 0, 3, 2};
int lookuptbl_digit[4] = {D1, D2, D3, D4};
char lookuptbl_led[7] = {A, B, C, D, E, F, G};

int Lookuptbl_nums[10][7] = {
    {A, B, C, D, E, F, 255},        // 0
    {B, C, 255, 255, 255, 255, 255}, // 1
    {A, B, G, E, D, 255, 255},       // 2
    {A, B, G, C, D, 255, 255},       // 3
    {F, G, B, C, 255, 255, 255},    // 4
    {A, F, G, C, D, 255, 255},      // 5
    {A, F, G, E, C, D, 255},        // 6
    {A, B, C, 255, 255, 255, 255},   // 7
    {A, B, C, D, E, F, G},          // 8
    {A, B, C, 255, 255, F, G},      // 9
};

bool repeating_timer_callback(__unused struct repeating_timer *t)
{
    static int pos = 0;
    int selected_number = current_display[pos];

    for (int i = 0; i < 4; i++)
        gpio_put(lookuptbl_digit[i], 1);

    for (int d = 0; d < 7; d++)
    {
        int segment_pin = lookuptbl_led[d];
        int state = false;
        for (int x = 0; x < 7; x++)
        {
            if (Lookuptbl_nums[selected_number][x] == segment_pin) state = true;
        }
        gpio_put(segment_pin, state);
    }

    gpio_put(lookuptbl_digit[pos], 0);
    pos = (pos + 1) % 4;

    return true;
}

int main()
{
    stdio_init_all();

    uint32_t start_value_timer = timer_hw->timelr + 20000;

    // Bulk initialization of GPIO pins for the 7-segment display
    uint32_t mask_enable_led = ((1 << A) | (1 << B) | (1 << C) | (1 << D) | (1 << E) | (1 << F) | (1 << G));
    uint32_t mask_enable_digit = ((1 << D1) | (1 << D2) | (1 << D3) | (1 << D4));

    gpio_init_mask(mask_enable_digit | mask_enable_led);
    gpio_set_dir_out_masked(mask_enable_led | mask_enable_digit);

    gpio_put_masked(mask_enable_digit, mask_enable_digit); // Set all digits HIGH (OFF)
    gpio_put_masked(mask_enable_led, 0);

    struct repeating_timer timer;
    add_repeating_timer_ms(-5, repeating_timer_callback, NULL, &timer);

    int counter = 0;
    while (true)
    {
        tight_loop_contents();
    }
}
