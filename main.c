#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pico/stdio.h>
#include <pico/stdlib.h>
#include <explorer.h>

#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/spi.h"
#include "hardware/dma.h"

static uint16_t COLORS[6] = {
    0xF803,
    0xFD25,
    0xFFE8,
    0x0403,
    0x001F,
    0x800F,
};
static uint16_t scanline[TFT_WIDTH];

int main()
{
    stdio_init_all();
    explorer_init();

    for (int x = 0; x < TFT_HEIGHT; ++x) {
        scanline[x] = COLORS[x / 40];
    }

    int bg = 0;
    int delay = 50;
    while (true) {
        // Wipe in
        for (int s = 0; s < TFT_HEIGHT; s += 6) {
            uint16_t* fb = explorer_framebuf();
            for (int y = 0; y < s; ++y) {
                memcpy(fb, scanline, TFT_WIDTH * 2);
                fb += TFT_WIDTH;
            }
            memset(fb, bg, (TFT_HEIGHT - s) * TFT_WIDTH * 2);
            explorer_present();
            sleep_ms(delay);
        }

        // Full
        {
            uint16_t* fb = explorer_framebuf();
            for (int y = 0; y < TFT_HEIGHT; ++y) {
                memcpy(fb, scanline, TFT_WIDTH * 2);
                fb += TFT_WIDTH;
            }
            explorer_present();
            sleep_ms(delay);
        }

        // Change background
        bg = ~bg;

        // Wipe out
        for (int s = 1; s < TFT_HEIGHT; s += 6) {
            uint16_t* fb = explorer_framebuf();
            memset(fb, bg, s * TFT_WIDTH * 2);
            fb += s * TFT_WIDTH;
            for (int y = s; y < TFT_HEIGHT; ++y) {
                memcpy(fb, scanline, TFT_WIDTH * 2);
                fb += TFT_WIDTH;
            }
            explorer_present();
            sleep_ms(delay);
        }
    }

    return 0;
}
