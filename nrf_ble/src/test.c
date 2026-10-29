#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>

#define set_delay 500

#define led1_xiao DT_ALIAS(led0)
#define led2_xiao DT_ALIAS(led1)
#define led3_xiao DT_ALIAS(led2)
#define irq_handler DT_ALIAS(nrf_irq) // where it is??

static const struct gpio_dt_spec blue_led = GPIO_DT_SPEC_GET(led1_xiao, gpios);
static const struct gpio_dt_spec red_led = GPIO_DT_SPEC_GET(led2_xiao, gpios);
static const struct gpio_dt_spec green_led = GPIO_DT_SPEC_GET(led3_xiao, gpios);

static const struct gpio_dt_spec irq_pin = GPIO_DT_SPEC_GET(irq_handler, gpios);
static struct gpio_callback irq_cb_data;

K_SEM_DEFINE(led_sem, 0, 1);

static void nrf_isr_handler(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
        k_sem_give(&led_sem);
}

int init_led(void)
{
        gpio_pin_configure_dt(&red_led, GPIO_OUTPUT_INACTIVE);
        gpio_pin_configure_dt(&green_led, GPIO_OUTPUT_INACTIVE);
        gpio_pin_configure_dt(&blue_led, GPIO_OUTPUT_INACTIVE);
        return 0;
}
int led_toggle(const struct gpio_dt_spec *led, const char *name)
{
        gpio_pin_set_dt(led, 1);
        printk("%s is on\n", name);
        k_msleep(set_delay);
        gpio_pin_set_dt(led, 0);
        printk("%s is off\n", name);
        k_msleep(set_delay);
        return 0;
}
int test_run(void)
{

        led_toggle(&red_led, "red led");
        led_toggle(&green_led, "green led");
        led_toggle(&blue_led, "blue led");
        return 0;
}
void test_thread(void *p1, void *p2, void *p3)
{
        gpio_pin_configure_dt(&irq_pin, GPIO_INPUT);
        gpio_pin_interrupt_configure_dt(&irq_pin, GPIO_INT_EDGE_FALLING);
        gpio_init_callback(&irq_cb_data, nrf_isr_handler, BIT(irq_pin.pin));
        gpio_add_callback(irq_pin.port, &irq_cb_data);
        init_led();
        while (1)
        {
                k_sem_take(&led_sem, K_FOREVER);
                test_run();
        }
}
K_THREAD_DEFINE(test_thread_id, 1024, test_thread, NULL, NULL, NULL, 7, 0, 0);