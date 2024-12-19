#include "gpio_settings.h"
#include"adxl345.h"
#include"utils.h"
#include <stdio.h>

LOG_MODULE_REGISTER(gpio_settings, LOG_LEVEL_DBG);
K_SEM_DEFINE(motion_semaphore,0,1);


const struct gpio_dt_spec errled = GPIO_DT_SPEC_GET(ERROR_LED, gpios);
const struct gpio_dt_spec adxl345_interrupt_pin = GPIO_DT_SPEC_GET(ADXL_INT_PIN, gpios);

struct gpio_callback adxl345_interrupt_callback;
struct spi_dt_spec* spi_device;



typedef void(*gpio_callback_handler)(const struct device*,struct gpio_callback*,uint32_t);

private void adxl345_interrupt_handler(const struct device *dev, struct gpio_callback *cb, uint32_t pins);
private void init_spi_device(void);
private gpio_status_t configure_interrupt(  const struct gpio_dt_spec*      GPIOx , 
                                            gpio_flags_t                    interrupt_type , 
                                            struct gpio_callback*           callback_config ,
                                            gpio_callback_handler           handler_func        );

private gpio_status_t configure_gpio_pin(const struct gpio_dt_spec *GPIOx, gpio_flags_t extra_flags);




/**
 * @brief  Belirtilen GPIO pini icin konfigurasyon yapar.
 * 
 * Bu fonksiyon, bir GPIO pininin belirtilen ekstra bayraklarla 
 * (flags) yapılandırılmasını saglar. Ilgili GPIO portunun hazir
 * olup olmadigini kontrol eder ve uygun loglama mesajlari ile
 * kullaniciya bilgi verir.
 *
 * @param[in] GPIOx        Yapilandirilacak GPIO pinini temsil eden GPIO spesifikasyonu.
 * @param[in] extra_flags  GPIO pinine atanacak ekstra bayraklar (flags).
 *
 * @return gpio_status_t    İşlem sonucu:
 * 
 * @retval GPIO_SUCCESS               İşlem başarılı.
 * @retval GPIO_NOT_READY             GPIO portu hazır değil.
 * @retval GPIO_PIN_CONFIG_FAILED     GPIO pini konfigüre edilemedi.
 */
private gpio_status_t configure_gpio_pin(const struct gpio_dt_spec *GPIOx, gpio_flags_t extra_flags)
{

    if (!GPIOx || !GPIOx->port ) {
        LOG_ERROR("[%s] HATA: GPIO pointer'i veya port'u NULL!", __func__);
        return GPIO_PIN_CONFIG_FAILED;
    }

    gpio_status_t ret;
    LOG_INFO("[%s] GPIO konfigurasyonu baslatildi. Port: %s, Pin: %d", __func__, GPIOx->port->name, GPIOx->pin);
    if (!gpio_is_ready_dt(GPIOx)) {
        LOG_ERROR("[%s] HATA: GPIO cihazinin hazir olmadigi tespit edildi! Port: %s, Pin: %d. [Dosya: %s, Satir: %d]", 
                        __func__, GPIOx->port->name, GPIOx->pin, __FILE__, __LINE__);
        return GPIO_NOT_READY; 
    }

    LOG_INFO("[%s] GPIO Port: %s hazir durumda, konfigurasyon baslatiliyor...", __func__, GPIOx->port->name);
    ret = gpio_pin_configure_dt(GPIOx, extra_flags);
    if (ret != GPIO_SUCCESS) {
        LOG_ERROR("[%s] HATA: GPIO pini ayarlanamadi! Hata Kodu: %d. Port: %s, Pin: %d, Ekstra Bayraklar: 0x%x. [Dosya: %s, Satir: %d]",
                        __func__, ret, GPIOx->port->name, GPIOx->pin, extra_flags, __FILE__, __LINE__);
        return GPIO_PIN_CONFIG_FAILED;
    }
    LOG_INFO("[%s] GPIO konfigurasyonu basariyla tamamlandi. Port: %s, Pin: %d, Ekstra Bayraklar: 0x%x", 
                __func__, GPIOx->port->name, GPIOx->pin, extra_flags);

    return GPIO_SUCCESS; 
}

/**
 * @brief  Belirtilen GPIO pini icin interrupt (kesme) konfigurasyonu yapar.
 * 
 * Bu fonksiyon, verilen bir GPIO pini icin kesme tetikleyici turunu ayarlar
 * ve ilgili callback fonksiyonunu tanimlar. Ilgili GPIO portu hazir degilse
 * veya konfigurasyon sirasinda hata meydana gelirse uygun hata kodu ile geri donus yapar.
 *
 * @param[in]  GPIOx             Kesme konfigurasyonu yapilacak GPIO pinini temsil eden spesifikasyon.
 * @param[in]  trigger_type      Kesme tetikleme tipi (örnegin: yukselen kenar, dusuk seviye).
 * @param[out] callback_config   Kesme icin tanimlanacak callback yapisi.
 * @param[in]  handler_func      Kesme tetiklendiginde cagrilacak callback fonksiyonu.
 *
 * @return gpio_status_t       Fonksiyonun durumu ile ilgili geri dönüş değeri.
 *
 * @retval GPIO_SUCCESS                   Konfigürasyon başarıyla tamamlandı.
 * @retval GPIO_PIN_CONFIG_FAILED         GPIO spesifikasyonu veya port NULL olduğu için işlem başarısız.
 * @retval GPIO_CONFIG_INTERRUPT_FAILED   Kesme tetikleyici ayarları başarısız oldu.
 * @retval GPIO_CONFIG_CALLBACK_FAILED    Callback yapılandırması veya eklenmesi başarısız oldu.
 */
private gpio_status_t configure_interrupt(  const struct gpio_dt_spec*              GPIOx , 
                                            gpio_flags_t                            trigger_type , 
                                            struct gpio_callback*                   callback_config ,
                                            gpio_callback_handler                   handler_func)
{

    if (!GPIOx  || !GPIOx->port) {
        LOG_ERROR("[%s]: HATA! GPIOx veya GPIO portu NULL!", __func__);
        return GPIO_PIN_CONFIG_FAILED; 
    }

    if (!callback_config) {
        LOG_ERROR("[%s]: HATA! Callback yapilandirilmasi NULL!", __func__);
        return GPIO_CONFIG_CALLBACK_FAILED;
    }

    if (!handler_func) {
        LOG_ERROR("[%s]: HATA! Callback handler işaretçisi NULL!", __func__);
        return GPIO_CONFIG_CALLBACK_FAILED;
    }

    gpio_status_t err ;
    err = gpio_pin_interrupt_configure_dt(GPIOx ,trigger_type ) ; 

    if(err != GPIO_SUCCESS )
    {
        LOG_ERROR("[%s]: GPIO kesme tetikleyici ayarlanamadi! Port: %s, Pin: %d, Hata Kodu: %d", 
                __func__, 
                GPIOx->port->name, 
                GPIOx->pin, 
                err);
        return GPIO_CONFIG_INTERRUPT_FAILED;
    }

    LOG_INFO("[%s]: GPIO kesme tetikleyici basariyla ayarlandi. Port: %s, Pin: %d", 
            __func__, 
            GPIOx->port->name, 
            GPIOx->pin);

    gpio_init_callback( callback_config , handler_func , BIT(GPIOx->pin)) ; 

    err = gpio_add_callback(GPIOx->port,callback_config);

    if( err != GPIO_SUCCESS)
    {
        LOG_ERROR("[%s]: GPIO callback eklenemedi! Port: %s, Pin: %d, Hata Kodu: %d", 
                __func__, 
                GPIOx->port->name, 
                GPIOx->pin, 
                err);        
        return GPIO_CONFIG_CALLBACK_FAILED;
    }

    LOG_INFO("[%s]: GPIO callback basariyla eklendi. Port: %s, Pin: %d", 
                __func__, 
                GPIOx->port->name, 
                GPIOx->pin);

    return GPIO_SUCCESS;
}   

/**
 * @brief SPI cihazını başlatır ve global `spi_device` değişkenini yapılandırır.
 * 
 * Bu fonksiyon, global olarak tanımlanan `spi_device` değişkenini başlatmak için 
 * kullanılır. Global değişkenin doğrudan başlatılması sırasında derleyici, 
 * sabit olmayan tanımlamalar nedeniyle hata verebilir. Bu nedenle başlatma işlemi 
 * burada yapılır.
 *
 * @note  `get_spi_device()` fonksiyonu kullanılarak SPI cihazı alınır ve 
 *        `spi_device` işaretçisine atanır. SPI cihazı, ADXL345 gibi donanımlarla 
 *        iletişim kurmak için kullanılır.
 *
 * @return void Döndürülen bir değer yoktur.
 */
private void init_spi_device(void)
{
    spi_device = get_spi_device();
}


/**
 * @brief  GPIO modulu icin ilk konfigurasyon islemlerini yapar.
 * 
 * Bu fonksiyon, GPIO modulu icin temel konfigurasyonlari yapar.
 * - Belirtilen GPIO LED pini konfigure edilir.
 * - ADXL345 icin interrupt (kesme) yapılandırması yapılır.
 * - SPI cihaz baslatilir.
 *
 * GPIO pini ve kesme konfigürasyonu sirasinda herhangi bir hata
 * meydana gelirse uygun hata kodlari ile geri donus yapar.
 *
 * @param[in] dev   Sistemdeki cihaz bilgisi. (Su an icin kullanilmiyor.)
 *
 * @return gpio_status_t      Konfigürasyon sonucu:
 * 
 * @retval GPIO_SUCCESS              Konfigürasyon başarılı.
 * @retval GPIO_NOT_READY            GPIO portu hazır değil.
 * @retval GPIO_PIN_CONFIG_FAILED    GPIO pini konfigüre edilemedi.
 * @retval GPIO_CONFIG_FAILED        ADXL345 interrupt konfigürasyonu başarısız.
 */
private gpio_status_t init_gpio(const struct device *dev)
{
    ARG_UNUSED(dev); 
    gpio_status_t ret;
    LOG_INFO("[%s] GPIO  konfigurasyonu baslatiliyor...", __func__);
    const struct gpio_dt_spec* GPIOx = get_gpio_led();

    ret = configure_gpio_pin(GPIOx, GPIO_OUTPUT | GPIO_ACTIVE_LOW);
    if (ret != GPIO_SUCCESS) {
        LOG_ERROR("[%s] HATA: GPIO pini ayarlanamadi! Port: %s, Pin: %d, Hata Kodu: %d", 
                __func__, GPIOx->port->name, GPIOx->pin, ret);
                
        return ret; 
    }
    LOG_INFO("[%s] GPIO konfigurasyonu tamamlandi.", __func__);

    ret = configure_interrupt(  &adxl345_interrupt_pin, 
                                GPIO_INT_EDGE_TO_ACTIVE, 
                                &adxl345_interrupt_callback, 
                                adxl345_interrupt_handler);
    if(ret != GPIO_SUCCESS )
    {
        LOG_ERROR("[%s]: ADXL345 kesme konfigurasyonu basarisiz! Hata Kodu: %d", __func__, ret);
        return ret;
    }
    LOG_INFO("[%s]: ADXL345 kesme konfigurasyonu basariyla tamamlandi.", __func__);

    init_spi_device();
    LOG_INFO("[%s]: GPIO konfigurasyonu basariyla tamamlandi.", __func__);
    return GPIO_SUCCESS; 
}

SYS_INIT(init_gpio, APPLICATION, GPIO_INIT_PRIORITY);



/**
 * @brief  ADXL345 sensöründen gelen interrupt'u (kesmeyi) isler.
 * 
 * Bu fonksiyon, ADXL345'ten gelen bir interrupt (kesme) tetiklendikten sonra
 * çağrılır. SPI üzerinden ADXL345'in INT_SOURCE register'ini okuyarak
 * hangi kesmenin (aktivite/inaktivite) tetiklendiğini tespit eder. Aktivite
 * veya inaktivite durumuna göre belirli islemler yapilir (örnegin, LED yakma/söndürme).
 *
 * @param[in] dev   Interrupt'a sebep olan cihaz (cihaz bilgisi).
 * @param[in] cb    Interrupt callback yapilandirmasi.
 * @param[in] pins  Hangi pinin kesme olusturdugu bilgisi.
 *
 * @return void     Döndürülen bir deger yoktur.
 */
private void adxl345_interrupt_handler(const struct device *dev , struct gpio_callback *cb , uint32_t pins)
{

   
    static uint8_t data[1];
    
    volatile int ret;

    LOG_DEBUG("[%s]: ADXL345 interrupt algilandi! Pins: 0x%x", __func__, pins);
   
    
    
    ret = spi_read_reg(spi_device , ADXL345_INT_SOURCE , data , 1);

    LOG_INFO("[%s]: INT_SOURCE: 0x%x", __func__, data[0]);

    if( data[0] & ADXL345_ACT_INTERRUPT_MASK )   // aktivite algilandi ! 
    {
        /*
            Sensor hareket algiladiginda buradaki islemleri yapar
            ...

        */


       k_sem_give(&motion_semaphore);

    } 
    else if( data[0] & ADXL345_INACT_INTERRUPT_MASK ) //inaktivite algilandi ! 
    {

        /*
            Sensor hareketsizliği algiladiginda buradaki islemleri yapar
            ...

        */       
        gpio_pin_set_dt(&errled , 1 );
     

    }
    else{return ;}

    

}


                                         





/**
 * @brief  Sistem genelinde kullanılacak LED GPIO pinine güvenli bir sekilde erisim saglar.
 * 
 * Bu fonksiyon, `errled` isimli LED GPIO spesifikasyonuna güvenli erişim sağlar. 
 * Fonksiyonun kullanılması, global değişkenlere direkt erişim yerine 
 * kontrollü bir erişim noktası sunar. Bu yöntem, `extern` ile değişken kullanımını 
 * engelleyerek kodun daha güvenli ve düzenli olmasını sağlar.
 *
 * @note   Bu fonksiyon, başka dosyalardan `errled` degiskenine erisim saglamak icin 
 *         kullanilabilir. Ancak istem dışı kullanımları engellemek için direkt 
 *         global erişime izin verilmez.
 *
 * @return const struct gpio_dt_spec*  LED GPIO spesifikasyonu (`errled`) icin bir pointer.
 */
public const struct gpio_dt_spec* get_gpio_led(void)
{
    return &errled;
}
