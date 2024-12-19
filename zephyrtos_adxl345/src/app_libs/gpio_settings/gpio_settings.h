/**
 * @file gpio_settings.h
 * @brief GPIO Pin Yapılandırmaları ve İşlevleri
 * 
 * @author uzunberkay
 * @date 19.12.2024
 */

#ifndef GPIO_SETTINGS_H
#define GPIO_SETTINGS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include"utils.h"


typedef enum {
    GPIO_SUCCESS                    =    0,             
    GPIO_NOT_READY                  =    1,           
    GPIO_PIN_CONFIG_FAILED          =    2,           
    GPIO_ERROR                      =    3,           
    GPIO_CONFIG_INTERRUPT_FAILED    =    4,           
    GPIO_CONFIG_CALLBACK_FAILED     =    5                          
} gpio_status_t;


/**
 * @brief GPIO init fonksiyonu öncelik seviyesi.
 */
#define GPIO_INIT_PRIORITY 41


#define ADXL_INT_PIN         DT_ALIAS(adxl_select)
#define ERROR_LED            DT_ALIAS(error_led)

public const struct gpio_dt_spec* get_gpio_led(void);






#ifdef __cplusplus
}
#endif


#endif // GPIO_SETTINGS_H



