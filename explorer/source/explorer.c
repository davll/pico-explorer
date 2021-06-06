#include <explorer.h>
#include <st7789.h>
#include <pico/binary_info.h>
#include <hardware/adc.h>
#include <hardware/dma.h>
#include <hardware/gpio.h>
#include <hardware/i2c.h>
#include <hardware/pwm.h>
#include <hardware/spi.h>

#define SPI_BAUD 16000000

static void init_led();
static void init_buttons();
static void init_adcs();
static void init_motors();
static void init_i2c();
static void init_spi();
static void init_screen();

static uint16_t g_screen_buf[2][TFT_WIDTH * TFT_HEIGHT];
static int g_screen_idx = 0;

static st7789_port_t g_screen = {
    .swap_endian = true,
    .spi = 0,
    .dma = -1,
    .tx_pin = SPI_MOSI_PIN,
    .cs_pin = SPI_CS_PIN,
    .dc_pin = SPI_DC_PIN,
    .sck_pin = SPI_SCK_PIN,
    .madctl = 0,
};
bi_decl(bi_2pins_with_names(SPI_DC_PIN, "SPI0 DC", SPI_CS_PIN, "SPI0 CS"));
    bi_decl(bi_2pins_with_func(SPI_SCK_PIN, SPI_MOSI_PIN, GPIO_FUNC_SPI));

static st7789_buf_t g_framebuf = {
    .width = TFT_WIDTH,
    .height = TFT_HEIGHT,
    .size = TFT_WIDTH * TFT_HEIGHT * 2,
    .data = g_screen_buf[0],
};

void explorer_init()
{
    init_led();
    init_buttons();
    init_adcs();
    init_motors();
    init_i2c();
    init_screen();
}

static void init_led()
{
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    bi_decl(bi_1pin_with_name(LED_PIN, "LED"));
}

static void init_buttons()
{
    gpio_set_function(BTN_A_PIN, GPIO_FUNC_SIO);
    gpio_set_function(BTN_B_PIN, GPIO_FUNC_SIO);
    gpio_set_function(BTN_X_PIN, GPIO_FUNC_SIO);
    gpio_set_function(BTN_Y_PIN, GPIO_FUNC_SIO);

    gpio_set_dir(BTN_A_PIN, GPIO_IN);
    gpio_set_dir(BTN_B_PIN, GPIO_IN);
    gpio_set_dir(BTN_X_PIN, GPIO_IN);
    gpio_set_dir(BTN_Y_PIN, GPIO_IN);

    gpio_pull_up(BTN_A_PIN);
    gpio_pull_up(BTN_B_PIN);
    gpio_pull_up(BTN_X_PIN);
    gpio_pull_up(BTN_Y_PIN);

    bi_decl(bi_4pins_with_names(BTN_A_PIN, "BTN A", BTN_B_PIN, "BTN B", BTN_X_PIN, "BTN X", BTN_Y_PIN, "BTN Y"));
}

static void init_adcs()
{
    adc_init();

    adc_gpio_init(ADC0_PIN);
    adc_gpio_init(ADC1_PIN);
    adc_gpio_init(ADC2_PIN);

    bi_decl(bi_3pins_with_names(ADC0_PIN, "ADC 0", ADC1_PIN, "ADC 1", ADC2_PIN, "ADC 2"));
}

static void init_motors()
{
    pwm_config motor_pwm_cfg = pwm_get_default_config();
    pwm_config_set_wrap(&motor_pwm_cfg, 255);

    pwm_init(pwm_gpio_to_slice_num(MOTOR1N_PIN), &motor_pwm_cfg, true);
    pwm_init(pwm_gpio_to_slice_num(MOTOR1P_PIN), &motor_pwm_cfg, true);
    pwm_init(pwm_gpio_to_slice_num(MOTOR2N_PIN), &motor_pwm_cfg, true);
    pwm_init(pwm_gpio_to_slice_num(MOTOR2P_PIN), &motor_pwm_cfg, true);

    gpio_set_function(MOTOR1N_PIN, GPIO_FUNC_PWM);
    gpio_set_function(MOTOR1P_PIN, GPIO_FUNC_PWM);
    gpio_set_function(MOTOR2N_PIN, GPIO_FUNC_PWM);
    gpio_set_function(MOTOR2P_PIN, GPIO_FUNC_PWM);

    bi_decl(bi_2pins_with_names(MOTOR1N_PIN, "MOTOR 1-", MOTOR1P_PIN, "MOTOR 1+"));
    bi_decl(bi_2pins_with_names(MOTOR2N_PIN, "MOTOR 2-", MOTOR2P_PIN, "MOTOR 2+"));
}

static void init_i2c()
{
    i2c_init(I2C_INST, I2C_BAUD);

    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);

    gpio_set_function(I2C_INT_PIN, GPIO_FUNC_SIO);
    gpio_set_dir(SPI_DC_PIN, GPIO_IN);

    bi_decl(bi_2pins_with_func(I2C_SDA_PIN, I2C_SCL_PIN, GPIO_FUNC_I2C));
    bi_decl(bi_1pin_with_name(I2C_INT_PIN, "I2C0 INT"));
}

static void init_screen()
{
    st7789_init(&g_screen, true);
    sleep_ms(100);
}

uint16_t* explorer_framebuf()
{
    return g_screen_buf[g_screen_idx];
}

void explorer_present()
{
    // Swap buffers
    g_framebuf.data = g_screen_buf[g_screen_idx];
    g_screen_idx = (g_screen_idx + 1) & 0x1;

    // Send pixels
    st7789_present(&g_screen, &g_framebuf);
}
