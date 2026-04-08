#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/devicetree.h>

#define DELAY 500

#define LEDR_NODE DT_ALIAS(led2)
#define LEDG_NODE DT_ALIAS(led0)
#define LEDB_NODE DT_ALIAS(led1)

#if DT_NODE_HAS_STATUS(LEDR_NODE, okay) && \
    DT_NODE_HAS_STATUS(LEDG_NODE, okay) && \
    DT_NODE_HAS_STATUS(LEDB_NODE, okay)

static const struct gpio_dt_spec ledr = GPIO_DT_SPEC_GET(LEDR_NODE, gpios);
static const struct gpio_dt_spec ledg = GPIO_DT_SPEC_GET(LEDG_NODE, gpios);
static const struct gpio_dt_spec ledb = GPIO_DT_SPEC_GET(LEDB_NODE, gpios);

#else
#error "LEDs não encontrados"
#endif


typedef enum {VERDE, AMARELO, VERMELHO} estado_semaforo;


void main(void){

    estado_semaforo estado = VERDE;

    if (!gpio_is_ready_dt(&ledr) ||
        !gpio_is_ready_dt(&ledg) ||
        !gpio_is_ready_dt(&ledb)) {
        printk("Erro nos LEDs\n");
        return;
    }

    gpio_pin_configure_dt(&ledr, GPIO_OUTPUT_INACTIVE);
    gpio_pin_configure_dt(&ledg, GPIO_OUTPUT_INACTIVE);
    gpio_pin_configure_dt(&ledb, GPIO_OUTPUT_INACTIVE);

    while (1){

        switch (estado){

            case VERDE:

                gpio_pin_set_dt(&ledg, 1);  // liga verde
                gpio_pin_set_dt(&ledr, 0);  // apaga vermelho
                gpio_pin_set_dt(&ledb, 0);  // apaga azul

                printk("VERDE\n");

                k_msleep(DELAY);
                estado = AMARELO;
                break;


            case AMARELO:

                gpio_pin_set_dt(&ledg, 1);
                gpio_pin_set_dt(&ledr, 1);
                gpio_pin_set_dt(&ledb, 0);

                printk("AMARELO\n");

                k_msleep(DELAY);
                estado = VERMELHO;
                break;


            case VERMELHO:

                gpio_pin_set_dt(&ledg, 0);
                gpio_pin_set_dt(&ledr, 1);
                gpio_pin_set_dt(&ledb, 0);

                printk("VERMELHO\n");

                k_msleep(DELAY);
                estado = VERDE;
                break;
        }
    }
}