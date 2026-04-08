#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>

#define SLEEP_TIME_MS 1000

// Define o LED usando Device Tree
#define LED0_NODE DT_ALIAS(led0)
#define LED1_NODE DT_ALIAS(led1)
#define LED2_NODE DT_ALIAS(led2)

// Verifica se o LED está definido no Device Tree
#if DT_NODE_HAS_STATUS(LED0_NODE, okay)
static const struct gpio_dt_spec led_verde = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
#else
#error "Unsupported board: led0 devicetree alias is not defined"
#endif

#if DT_NODE_HAS_STATUS(LED1_NODE, okay)
static const struct gpio_dt_spec led_azul = GPIO_DT_SPEC_GET(LED1_NODE, gpios);
#else
#error "Unsupported board: led1 devicetree alias is not defined"
#endif

#if DT_NODE_HAS_STATUS(LED2_NODE, okay)
static const struct gpio_dt_spec led_vermelho = GPIO_DT_SPEC_GET(LED2_NODE, gpios);
#else
#error "Unsupported board: led2 devicetree alias is not defined"
#endif

void main(void)
{
    int ret;

    // Verifica se o device está pronto
    if (!gpio_is_ready_dt(&led_verde)) {
        printk("Error: LED device %s is not ready\n", led_verde.port->name);
        return;
    }

if (!gpio_is_ready_dt(&led_azul)) {
        printk("Error: LED device %s is not ready\n", led_azul.port->name);
        return;
    }

    if (!gpio_is_ready_dt(&led_vermelho)) {
        printk("Error: LED device %s is not ready\n", led_vermelho.port->name);
        return;
    }

    // Configura o pino como saída
    ret = gpio_pin_configure_dt(&led_verde, GPIO_OUTPUT_ACTIVE);
    if (ret < 0) {
        printk("Error %d: failed to configure LED pin\n", ret);
        return;
    }

    printk("LED blinking on %s pin %d\n", led_verde.port->name, led_verde.pin);


    ret = gpio_pin_configure_dt(&led_azul, GPIO_OUTPUT_ACTIVE);
    if (ret < 0) {
        printk("Error %d: failed to configure LED pin\n", ret);
        return;
    }

    printk("LED blinking on %s pin %d\n", led_azul.port->name, led_azul.pin);

    
    ret = gpio_pin_configure_dt(&led_vermelho, GPIO_OUTPUT_ACTIVE);
    if (ret < 0) {
        printk("Error %d: failed to configure LED pin\n", ret);
        return;
    }

    printk("LED blinking on %s pin %d\n", led_vermelho.port->name, led_vermelho.pin);
    int estado = 0;

    while (1) {
        // Toggle do LED usando a nova API
        if ( estado == 0){
        gpio_pin_set_dt(&led_verde, 1);
        gpio_pin_set_dt(&led_azul, 0);
        gpio_pin_set_dt(&led_vermelho, 0);
        
        k_msleep(5*SLEEP_TIME_MS);

        estado = 1;

        continue;
        }
        if (estado == 1){
        gpio_pin_set_dt(&led_verde, 1);
        gpio_pin_set_dt(&led_azul, 0);
        gpio_pin_set_dt(&led_vermelho, 1);
        
        k_msleep(2*SLEEP_TIME_MS);

        estado = 2;

        continue;
        }
        
        if(estado == 2){
        gpio_pin_set_dt(&led_verde, 0);
        gpio_pin_set_dt(&led_azul, 0);
        gpio_pin_set_dt(&led_vermelho, 1);
        
        k_msleep(6*SLEEP_TIME_MS);

        estado = 0;

        continue;
        }
    }
}