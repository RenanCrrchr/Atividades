                                 #include <zephyr/kernel.h>             // Funções básicas do Zephyr (ex: k_msleep, k_thread, etc.)
#include <zephyr/device.h>             // API para obter e utilizar dispositivos do sistema
#include <zephyr/drivers/gpio.h>       // API para controle de pinos de entrada/saída (GPIO)
#include <pwm_z42.h>                // Biblioteca personalizada com funções de controle do TPM (Timer/PWM Module)




#define TPM_IRQ_LINE TPM1_IRQn  // relaciona a interrupção ao timer TPM1
#define TPM_IRQ_PRIORITY 1      // define a prioridade da interrupção

volatile uint16_t subida= 0;
volatile uint16_t descida= 0;
volatile bool     par_pronto = false; 

void tpm1_isr(void *arg)
{
    TPM1->STATUS |= TPM_STATUS_CH0F_MASK;
    uint16_t val = TPM1->CONTROLS[0].CnV;

    
    // Lê o nível atual do pino para identificar a borda
    if (GPIOE->PDIR & (1 << 20))  // pino em nível alto → acabou de subir
       {
          subida = val;
          par_pronto = false;
       }
    else                            // pino em nível baixo → acabou de descer
        {
          descida = val;
          par_pronto = true;
        }
}


void main(void)
{
    pwm_tpm_Init(TPM2, TPM_PLLFLL, 9600, TPM_CLK, PS_1, EDGE_PWM);


    // Inicializa o canal 0 do TPM2 para gerar sinal PWM na porta GPIOE_22
    // - modo TPM_PWM_H (nível alto durante o pulso)
    pwm_tpm_Ch_Init(TPM2, 0, TPM_PWM_H, GPIOE, 22);


    // Define o valor do duty cycle: nesse caso, 1 (trigger quase desligado)
    pwm_tpm_CnV(TPM2, 0, 480);




    // Inicializa TPM1 com módulo e prescaler desejado
    pwm_tpm_Init(TPM1, TPM_PLLFLL, 65535, TPM_CLK, PS_8, EDGE_PWM);


    // Configura TPM1_CH0 como input capture na borda de subida
    pwm_tpm_Ch_Init(TPM1, 0, TPM_INPUT_CAPTURE_BOTH| TPM_CHANNEL_INTERRUPT, GPIOE, 20);




    // Conecta a interrupção via Zephyr
    IRQ_CONNECT(TPM_IRQ_LINE, TPM_IRQ_PRIORITY, tpm1_isr, NULL, 0);
    irq_enable(TPM_IRQ_LINE);
 
  volatile double dist = 0;

    while (1)
    {

      while (!par_pronto) { k_msleep(1); }

        unsigned int key = irq_lock();
        uint16_t s = subida;
        uint16_t d = descida;
        par_pronto = false;
        irq_unlock(key);



        printk("Valor do TPM1: %u\n", s);

        printk("Valor do TPM2: %u\n", d);

        uint16_t t;

        if (s > d){
          t = 65535 - (s - d);
        }
        else t = d - s;

        printk("Tempo de ida e volta: %u\n", t);

        dist = t*0.002834; // Distancia em cm

        printk("Distancia em cm: %f\n", dist);

        k_msleep(1000);
    }


}
