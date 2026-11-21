#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
// #include <zephyr/logging/log.h>

// LOG_MODULE_REGISTER(new_led, LOG_LEVEL_INF);

#define set_delay 500
int new_led(){
    const struct gpio_dt_spec my_pin = GPIO_DT_SPEC_GET(DT_ALIAS(new_led), gpios);
    gpio_pin_configure_dt(&my_pin, GPIO_OUTPUT_INACTIVE);
    // LOG_INF("Battery LED toggling thread running");
    printk("Battery LED toggling thread running\n");
    gpio_pin_set_dt(&my_pin, 1);
    k_msleep(set_delay);
    gpio_pin_set_dt(&my_pin, 0);
    k_msleep(set_delay);
    return 0;
}
void new_led_thread(void *p1, void *p2, void *p3)
{
        while (1)
        {
            new_led();
        }
}
K_THREAD_DEFINE(new_led_thread_id, 1024, new_led_thread, NULL, NULL, NULL, 7, 0, 0);