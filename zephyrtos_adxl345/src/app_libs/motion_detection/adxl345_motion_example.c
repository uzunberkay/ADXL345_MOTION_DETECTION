#include"utils.h"
#include"adxl345.h"
#include"gpio_settings.h"
#include<zephyr/kernel.h>

LOG_MODULE_REGISTER(motion_sample, LOG_LEVEL_INF);


extern struct k_sem motion_semaphore;

#define MOTION_THREAD_STACK_SIZE   512  
#define MOTION_THREAD_PRIORITY     5     

/**
 * @brief Hareket algilama semaforunu bekleyen ve LED tetikleyen thread.
 *
 * Bu thread, hareket algilandiğinda bir semafor sinyali alarak tetiklenir.
 * Semafor sinyali alindiğinda, bir LED'i yakar ve ardından belirli bir süre bekler.
 *
 * @param vp1 Thread'in birinci argümanıdır. Kullanılmamaktadır ve ARG_UNUSED ile işaretlenmiştir.
 * @param vp2 Thread'in ikinci argümanıdır. Kullanılmamaktadır ve ARG_UNUSED ile işaretlenmiştir.
 * @param vp3 Thread'in üçüncü argümanıdır. Kullanılmamaktadır ve ARG_UNUSED ile işaretlenmiştir.
 *
 * @note Bu thread, `motion_semaphore` semaforunun sinyal almasını bekler.
 */
void motion_thread(void* vp1 , void* vp2 , void* vp3)
{
    ARG_UNUSED(vp1);
    ARG_UNUSED(vp2);
    ARG_UNUSED(vp3);

    const struct gpio_dt_spec* led = get_gpio_led();
    while (1) {

        k_sem_take(&motion_semaphore, K_FOREVER);

        LOG_INFO("Hareket algilandi! Thread tetiklendi!");

        gpio_pin_set_dt(led , 0);
        k_msleep(ADXL_TIME_INACT_10_SEC); 
    }
}


K_THREAD_DEFINE(
    motion_thread_id,            
    MOTION_THREAD_STACK_SIZE,    
    motion_thread,               
    NULL,                        // vp1 
    NULL,                        // vp2 
    NULL,                        // vp3 
    MOTION_THREAD_PRIORITY,      
    0,                           
    0                            
);