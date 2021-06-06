#include <st7789.h>
#include <pico/stdlib.h>
#include <hardware/dma.h>
#include <hardware/gpio.h>
#include <hardware/spi.h>

#define ST7789_BAUD 16000000

static spi_inst_t* SPI[2] = { spi0, spi1 };

void st7789_cmd(st7789_port_t* port, uint8_t cmd, const void* p, size_t n)
{
    spi_inst_t* spi = SPI[port->spi];

    // begin transmission
    gpio_put(port->cs_pin, 0);

    // command mode
    gpio_put(port->dc_pin, 0);
    spi_write_blocking(spi, &cmd, 1);

    // data mode
    if (p && n) {
        gpio_put(port->dc_pin, 1);
        spi_write_blocking(spi, (const uint8_t*)p, n);
    }

    // end transmission
    gpio_put(port->cs_pin, 1);
}

static void st7789_init_spi(st7789_port_t* port)
{
    spi_inst_t* spi = SPI[port->spi];

    spi_init(spi, ST7789_BAUD);

    gpio_set_function(port->dc_pin, GPIO_FUNC_SIO);
    gpio_set_function(port->cs_pin, GPIO_FUNC_SIO);
    gpio_set_function(port->sck_pin,  GPIO_FUNC_SPI);
    gpio_set_function(port->tx_pin, GPIO_FUNC_SPI);

    gpio_set_dir(port->dc_pin, GPIO_OUT);
    gpio_set_dir(port->cs_pin, GPIO_OUT);
}

void st7789_init(st7789_port_t* port, bool dma)
{
    // Init SPI
    st7789_init_spi(port);

    // Init DMA
    if (dma) {
        port->dma = dma_claim_unused_channel(false);
    } else {
        port->dma = -1;
    }

    // Reset the device
    st7789_cmd(port, ST7789_SWRESET, NULL, 0);
    sleep_ms(150);

    // Set color mode to 16-bit per pixel
    st7789_cmd(port, ST7789_COLMOD, "\x55", 1);

    // Set inversion mode
    st7789_cmd(port, ST7789_INVON, NULL, 0);

    // Wake up the device
    st7789_cmd(port, ST7789_SLPOUT, NULL, 0);

    // Turn on display
    st7789_cmd(port, ST7789_DISPON, NULL, 0);
}

static void st7789_present_dma(st7789_port_t* port, st7789_buf_t* fb)
{
    spi_inst_t* spi = SPI[port->spi];
    uint dma = port->dma;

    // swap endian
    if (port->swap_endian) {
        for (size_t i = 0, n = fb->size >> 1; i < n; ++i) {
            uint16_t* p = (uint16_t*)fb->data + i;
            *p = __builtin_bswap16(*p);
        }
    }

    // end transmission
    gpio_put(port->cs_pin, 1);

    // configure DMA
    dma_channel_config cfg = dma_channel_get_default_config(dma);
    channel_config_set_transfer_data_size(&cfg, DMA_SIZE_8);
    channel_config_set_bswap(&cfg, port->swap_endian);
    switch (spi_get_index(spi)) {
        case 0:
            channel_config_set_dreq(&cfg, DREQ_SPI0_TX);
            break;
        case 1:
            channel_config_set_dreq(&cfg, DREQ_SPI1_TX);
            break;
    }
    void* rd_addr = fb->data;
    volatile void* wr_addr = &spi_get_hw(spi)->dr;
    dma_channel_configure(dma, &cfg, wr_addr, rd_addr, fb->size, false);

    // begin transmission
    gpio_put(port->cs_pin, 0);

    // command mode
    static const uint8_t cmd = ST7789_RAMWR;
    gpio_put(port->dc_pin, 0);
    spi_write_blocking(spi, &cmd, 1);

    // data mode
    gpio_put(port->dc_pin, 1);
    dma_channel_start(dma);
    //dma_channel_wait_for_finish_blocking(dma);
}

static void st7789_present_cpu(st7789_port_t* port, st7789_buf_t* fb)
{
    // Swap endian
    if (port->swap_endian) {
        for (size_t i = 0, n = fb->size >> 1; i < n; ++i) {
            uint16_t* p = (uint16_t*)fb->data + i;
            *p = __builtin_bswap16(*p);
        }
    }

    // Wrtie to frame memory
    st7789_cmd(port, ST7789_RAMWR, fb->data, fb->size);
}

void st7789_present(st7789_port_t* port, st7789_buf_t* fb)
{
    // wait for DMA
    if (port->dma != -1)
        dma_channel_wait_for_finish_blocking(port->dma);

    // Set memory access control (direction)
    st7789_cmd(port, ST7789_MADCTL, &port->madctl, 1);

    // Set column range
    uint8_t caset[4] = {
        0, 0,
        (((fb->width-1) >> 8) & 0xFF),
        ((fb->width-1) & 0xFF)
    };
    st7789_cmd(port, ST7789_CASET, caset, 4);

    // Set row range
    uint8_t raset[4] = {
        0, 0,
        (((fb->height-1) >> 8) & 0xFF),
        ((fb->height-1) & 0xFF)
    };
    st7789_cmd(port, ST7789_RASET, raset, 4);

    if (port->dma != -1)
        st7789_present_dma(port, fb);
    else
        st7789_present_cpu(port, fb);
}
