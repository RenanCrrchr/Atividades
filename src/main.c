#include <zephyr/kernel.h>             // Funções básicas do Zephyr (ex: k_msleep, k_thread, etc.)
#include <zephyr/device.h>             // API para obter e utilizar dispositivos do sistema
#include <zephyr/drivers/gpio.h>       // API para controle de pinos de entrada/saída (GPIO)
#include <pwm_z42.h>                // Biblioteca personalizada com funções de controle do TPM (Timer/PWM Module)
#include <ultrassom.h>


int main(void)
{
  ultrassonico_init();
  while (1)
  {
    double d;
    d = ultrassonico_ler();
    printk("%fcm \n", d);
  }
  
}