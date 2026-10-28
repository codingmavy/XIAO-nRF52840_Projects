#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include "test.h"

int main(void)
{
        while(1){
          k_busy_wait(250000);
          k_msleep(5000);
          printk("main code running\n");
        }
        return 0;
}
