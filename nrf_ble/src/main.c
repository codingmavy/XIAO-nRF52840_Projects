#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include "test.h"
#include "new_led.h"

int main(void)
{
        while(1){
          new_led();
          k_msleep(1000);
          printk("main code running\n");
        }
        return 0;
}
