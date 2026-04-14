#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>

#define SLEEP_TIME_MS 1000

// Define o LED usando Device Tree
#define LED0_NODE DT_ALIAS(led0)

// Verifica se o LED está definido no Device Tree
#if DT_NODE_HAS_STATUS(LED0_NODE, okay)
static const struct gpio_dt_spec led_verde = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
#else
#error "Unsupported board: led0 devicetree alias is not defined"
#endif

void main(void)
{
    int ret;

    // Verifica se o device está pronto
    if (!gpio_is_ready_dt(&led_verde)) {
        printk("Error: LED device %s is not ready\n", led_verde.port->name);
        return;
    }


    // Configura o pino como saída
    ret = gpio_pin_configure_dt(&led_verde, GPIO_OUTPUT_ACTIVE);
    if (ret < 0) {
        printk("Error %d: failed to configure LED pin\n", ret);
        return;
    }

    printk("LED blinking on %s pin %d\n", led_verde.port->name, led_verde.pin);


    while (1) {
        // Toggle do LED usando a nova API
        
        gpio_pin_set_dt(&led_verde, 1);
		k_msleep(SLEEP_TIME_MS*2);

		gpio_pin_set_dt(&led_verde, 0);
		k_msleep(SLEEP_TIME_MS*2);

    }
}