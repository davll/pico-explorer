#ifndef PIMORONI_PICO_EXPLORER_H_
#define PIMORONI_PICO_EXPLORER_H_

#include <stdio.h>

#define UNUSED_PIN      0xFFFF

#define LED_PIN         25 // PICO_DEFAULT_LED_PIN

#define I2C_BAUD        400000
#define I2C_SDA_PIN     20
#define I2C_SCL_PIN     21
#define I2C_INT_PIN     22

#define SPI_MOSI_PIN    19 // PICO_DEFAULT_SPI_TX_PIN
#define SPI_MISO_PIN    16 // PICO_DEFAULT_SPI_RX_PIN
#define SPI_SCK_PIN     18 // PICO_DEFAULT_SPI_SCK_PIN
#define SPI_DC_PIN      SPI_MISO_PIN
#define SPI_CS_PIN      17 // PICO_DEFAULT_SPI_CSN_PIN

#define TFT_WIDTH       240
#define TFT_HEIGHT      240

#define BTN_A_PIN       12
#define BTN_B_PIN       13
#define BTN_X_PIN       14
#define BTN_Y_PIN       15

#define ADC0_PIN        26
#define ADC1_PIN        27
#define ADC2_PIN        28

#define MOTOR1N_PIN     8
#define MOTOR1P_PIN     9
#define MOTOR2N_PIN     10
#define MOTOR2P_PIN     11

#define GP0_PIN         0
#define GP1_PIN         1
#define GP2_PIN         2
#define GP3_PIN         3
#define GP4_PIN         4
#define GP5_PIN         5
#define GP6_PIN         6
#define GP7_PIN         7

#define I2C_INST        i2c0
#define SPI_INST        spi0

#define BTN_A_PRESSED   (!gpio_get(BTN_A_PIN))
#define BTN_B_PRESSED   (!gpio_get(BTN_B_PIN))
#define BTN_X_PRESSED   (!gpio_get(BTN_X_PIN))
#define BTN_Y_PRESSED   (!gpio_get(BTN_Y_PIN))

void explorer_init();

uint16_t* explorer_framebuf();
void explorer_present();

#endif // PIMORONI_PICO_EXPLORER_H_
