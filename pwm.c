#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define MOTOR 22
#define PWM_FREQ 50
#define CLOCK_DIV 125.0f

//O valor de wrap é essa por que: 
//125MHz é a frequencia do raspberry pi
//50Hz é frequencioa desejada
//WRAP = (125.000.000 / (50 * 125.0)) - 1

#define WRAP 19999

void setup_pwm() {

    gpio_set_function(MOTOR, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(MOTOR);

    pwm_set_clkdiv(slice, CLOCK_DIV);
    pwm_set_wrap(slice, WRAP);
    pwm_set_enabled(slice, true);
}

void motor_angle(uint slice, uint16_t pulse_width_us) {
    uint16_t level = (pulse_width_us * WRAP) / 20000;
    pwm_set_gpio_level(MOTOR, level);
}

int main() {

    stdio_init_all();
    setup_pwm();

    uint slice = pwm_gpio_to_slice_num(MOTOR);

    // Posição de 180 graus (2.400 microssegundos)
    motor_angle(slice, 2400);
    sleep_ms(5000);

    // Posição de 90 graus (1.470 microssegundos)
    motor_angle(slice, 1470);
    sleep_ms(5000);

    // Posição de 0 graus (500 microssegundos)
    motor_angle(slice, 500);
    sleep_ms(5000);

        while (true) {

        // Articulação de 0 a 180 graus
        for (uint16_t pulse_width = 500; pulse_width <= 2400; pulse_width += 5) {
            motor_angle(slice, pulse_width);
            sleep_ms(10);
        }

        // Articulação de 180 a 0 graus
        for (uint16_t pulse_width = 2400; pulse_width >= 500; pulse_width -= 5) {
            motor_angle(slice, pulse_width);
            sleep_ms(10);
        }
    }
}