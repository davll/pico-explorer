#ifndef ST7789_H_
#define ST7789_H_

#include <stdint.h>
#include <hardware/spi.h>

// SPI interface
struct st7789_port
{
    bool swap_endian;
    int spi;
    int dma;
    int tx_pin;
    int cs_pin;
    int dc_pin;
    int sck_pin;
    uint8_t madctl;
};
typedef struct st7789_port st7789_port_t;

struct st7789_buf
{
    int width;
    int height;
    size_t size;
    void* data;
};
typedef struct st7789_buf st7789_buf_t;

void st7789_cmd(st7789_port_t* port, uint8_t cmd, const void* p, size_t n);
void st7789_init(st7789_port_t* port, bool dma);
void st7789_present(st7789_port_t* port, st7789_buf_t* fb);

#define ST7789_NOP              0x00
#define ST7789_SWRESET          0x01
#define ST7789_SLPIN            0x10
#define ST7789_SLPOUT           0x11
#define ST7789_INVOFF           0x20
#define ST7789_INVON            0x21
#define ST7789_GAMSET           0x26
#define ST7789_DISPOFF          0x28
#define ST7789_DISPON           0x29

// CASET: Column Address Set
//
//   Set the range of columns (from XS to XE)
//
// 1st param
//   D[7-0]: XS[15-8]
// 2nd param
//   D[7-0]: XS[7-0]
// 3rd param
//   D[7-0]: XE[15-8]
// 4th param
//   D[7-0]: XE[7-0]
//
#define ST7789_CASET            0x2A

// RASET: Row Address Set
//
//   Set the range of rows (from YS to YE)
//
// 1st param
//   D[7-0]: YS[15-8]
// 2nd param
//   D[7-0]: YS[7-0]
// 3rd param
//   D[7-0]: YE[15-8]
// 4th param
//   D[7-0]: YE[7-0]
//
#define ST7789_RASET            0x2B

// RAMWR: Memory Write
//
//   Write data to the frame memory, the access is controlled by CASET, RASET, MADCTL
//
#define ST7789_RAMWR            0x2C

#define ST7789_RAMRD            0x2E
#define ST7789_TEOFF            0x34

// TEON: Tearing Effect Line On
//
// 1st param:
//   D[7-1]: 0
//   D[0]: 0 = only emit v-blanking
//         1 = emit v-blanking and h-blanking
#define ST7789_TEON             0x35

// MADCTL: Memory Data Access Control
//
// 1st param:
//   D[7]: MY - Page Address Order
//         0 = top to bottom
//         1 = bottom to top
//   D[6]: MX - Column Address Order
//         0 = left to right
//         1 = right to left
//   D[5]: MV - Page/Column Order
//         0 = normal mode
//         1 = reverse mode
//   D[4]: ML - Line Address Order
//         0 = refresh top to bottom
//         1 = refresh bottom to top
//   D[3]: RGB - RGB/BGR Order
//         0 = RGB
//         1 = BGR
//   D[2]: MH - Display Data Latch Order
//         0 = refresh left to right
//         1 = refresh right to left
//   D[1-0]: 0
//
// Read 'Section 8.12 Address Control' in the datasheet
#define ST7789_MADCTL           0x36

// COLMOD: Interface Pixel Format
//
// 1st param:
//   D[7]: 0
//   D[6-4]: 101 = 65K of RGB interface
//           110 = 262K of RGB interface
//   D[3]: 0
//   D[2-0]: 011 = 12 bit/pixel
//           101 = 16 bit/pixel
//           110 = 18 bit/pixel
//           111 = 16M truncated
#define ST7789_COLMOD           0x3A

#define ST7789_RAMWRC           0x3C
#define ST7789_RAMRDC           0x3E
#define ST7789_RAMCTRL          0xB0
#define ST7789_RGBCTRL          0xB1

#endif // ST7789_H_
