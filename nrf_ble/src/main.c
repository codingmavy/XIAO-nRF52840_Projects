#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include "test.h"
#include "new_led.h"
// #include <zephyr/logging/log.h>

// LOG_MODULE_REGISTER(main,LOG_LEVEL_INF);
int main(void)
{
        // LOG_INF("System booted successfully");
        printk("System booted successfully\n");
        while(1){
          new_led();
          k_msleep(1000);
        }
        return 0;
}
