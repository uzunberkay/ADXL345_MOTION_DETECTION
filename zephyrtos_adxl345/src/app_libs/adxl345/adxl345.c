#include"adxl345.h"

LOG_MODULE_REGISTER(adxl345, LOG_LEVEL_DBG);



struct spi_dt_spec spispec = SPI_DT_SPEC_GET(DT_NODELABEL(mysensor1), SPIOP,0);


/**
 * @brief  SPI cihazına güvenli bir şekilde erişim sağlayan yardımcı fonksiyon.
 * 
 * Bu fonksiyon, `spispec` isimli SPI cihaz spesifikasyonuna güvenli erişim sağlar.
 * Fonksiyonun kullanılması, global değişkenlere doğrudan erişim yerine kontrollü 
 * bir erişim noktası sunar. Bu yöntem, `extern` kullanılarak değişkenlere doğrudan 
 * erişimin neden olabileceği karmaşıklıkları ve güvenlik açıklarını ortadan kaldırır.
 *
 * @note   Bu fonksiyon, başka modüllerden `spispec` değişkenine erişim sağlamak 
 *         için kullanılabilir. Ancak istem dışı erişimleri ve değiştirmeleri engellemek 
 *         için global erişim kısıtlanmıştır.
 *
 * @return struct spi_dt_spec*  SPI cihaz spesifikasyonu (`spispec`) için bir pointer döndürür.
 */
public struct spi_dt_spec* get_spi_device(void)
{
    return &spispec;
}


/**
 * @brief Verilen değer ile ADX_SPI değerini bit düzeyinde işler ve sonucunu döndürür.
 *
 * Bu fonksiyon, verilen `value` değeri ile `ADX_SPI` değerini bit OR işlemi yaparak işler.
 * Bit düzeyinde yapılan işlem sonucu elde edilen değeri döndürür.
 *
 * @param value İşlenecek olan değer.
 * @param ADX_SPI Bit düzeyinde işlemek için kullanılacak değer.
 * @return Bit düzeyinde işlenmiş sonuç.
 */
private uint8_t byte_operation( uint8_t value , uint8_t ADX_SPI )
{
	return ( value | ADX_SPI) ;
}

/**
 * @brief SPI üzerinden belirtilen register'a veri yazma işlemi yapar.
 *
 * Bu fonksiyon, belirtilen SPI ayarlarına göre verilen register'a belirtilen değeri yazar.
 * İlgili register adresi ve yazılacak değer `reg` ve `value` parametreleri olarak alınır.
 * Yazma işlemi için SPI ile iletişim sağlanır ve hata durumunda hata kodu döner.
 *
 * @param spispec SPI ayarları yapısı işaretçisi.
 * @param reg Yazılacak register adresi.
 * @param value Yazılacak değer.
 * @return Yazma işlemi başarılıysa 0, aksi halde hata kodu.
 */
private int write_reg( const struct spi_dt_spec *spispec , uint8_t reg , uint8_t value )
{
    int err ;
    uint8_t tx_buf[2];

    tx_buf[0] = reg;
    tx_buf[1] = value;

    struct spi_buf 		tx_spi_buf 		= {.buf = tx_buf, .len = sizeof(tx_buf)};
	struct spi_buf_set 	tx_spi_buf_set	= {.buffers = &tx_spi_buf, .count = 1};

    err = spi_write_dt(spispec , &tx_spi_buf_set);
    if(err < 0 )
    {
        LOG_ERROR("SPI yazma basarisiz (reg=0x%02X, value=0x%02X), err=%d", reg, value, err);
        return err;
    }
    LOG_DEBUG("SPI yazma basarili (reg=0x%02X, value=0x%02X)", reg, value);
    return 0 ;    
}

/**
 * @brief SPI üzerinden belirtilen register'dan veri okur.
 *
 * Bu fonksiyon, belirtilen SPI ayarlarına göre verilen register'dan veri okuma işlemini gerçekleştirir.
 * Register adresine okuma işlemi için gereken SPI komutlarını ekler (örneğin, 0x80 okuma işareti).
 * Gerekirse, multi-byte (çoklu byte) okuma modunu destekler (0x40 multi-byte bitini ayarlar).
 * Okunan veriyi `data` dizisine yazar.
 *
 * @param spispec SPI ayarları yapısı işaretçisi.
 * @param reg Okunacak register adresi.
 * @param data Okunan verinin yazılacağı buffer.
 * @param size Okunacak veri miktarı.
 * @return Okuma işlemi başarılıysa 0, aksi halde hata kodu.
 */
public int spi_read_reg( const struct spi_dt_spec *spispec , uint8_t reg , uint8_t *data , uint8_t size ) {
    int err;
    uint8_t tx_buffer[1] = {0};

    
    tx_buffer[0] =   byte_operation(reg ,ADXL_SPI_READ );           /*!< Okuma işlemi için 0x80 ekle */
    if (size > 1) {
        tx_buffer[0] = byte_operation(tx_buffer[0] ,ADXL_SPI_MB );  /*!< Multibyte okuma için 0x40 ekle */
    }

    struct spi_buf tx_spi_buf = {.buf = tx_buffer, .len = sizeof(tx_buffer)};
    struct spi_buf_set tx_spi_buf_set = {.buffers = &tx_spi_buf, .count = 1};


    struct spi_buf rx_spi_bufs[2];
    rx_spi_bufs[0].buf = NULL;
    rx_spi_bufs[0].len = sizeof(tx_buffer); 
    rx_spi_bufs[1].buf = data;
    rx_spi_bufs[1].len = size;

    struct spi_buf_set rx_spi_buf_set = {.buffers = rx_spi_bufs, .count = 2};

   
    err = spi_transceive_dt(spispec, &tx_spi_buf_set, &rx_spi_buf_set);
    if (err < 0) {
        LOG_ERROR("spi_transceive_dt() failed, err: %d", err);
        return err;
    }

    return 0;
}




/**
 * @brief ADXL345 ivmeölçer kesme konfigürasyonunu başlatır.
 *
 * Bu fonksiyon, ADXL345 ivmeölçeri için kesme işlemlerini başlatır:
 * - Bant genişliği hızını düşük güç moduna ayarlar.
 * - Kesme konfigürasyonlarını yapmadan önce kesme etkinleştirmeyi devre dışı bırakır.
 * - Güç kontrol register'ında bağlantı ve otomatik uyku modlarını aktifleştirir.
 * - Aktivite algılama için eşik değerini belirler.
 * - İnaktivite algılama için eşik değerini belirler.
 * - Cihazın uyku süresini tanımlar (yaklaşık 10 saniye hareketsizlikte uyuma).
 * - X ve Y eksenlerinde aktivite ve inaktivite için DC modunu ayarlar.
 * - Aktivite ve inaktivite kesmelerini INT2 pinine eşler.
 * - Aktivite ve inaktivite kesmelerini etkinleştirir.
 * - Güç kontrol register'ında bağlantı, otomatik uyku ve ölçüm modlarını aktifleştirir.
 */
private int init_adxl_interrupt(void)
{
    
    int err; 

    /*!< ADXL345_BW_RATE Register: Low power modu ayari */
    err = write_reg(&spispec, ADXL345_BW_RATE,  ADXL_BW_RATE_0_10HZ);
    if (err) {
        LOG_ERROR("ADXL345_BW_RATE yazma hatasi: %d", err);
        return err;
    }

    /*!< ADXL345_INT_ENABLE Register: Interruptlari devre disi birakma */
    err = write_reg(&spispec, ADXL345_INT_ENABLE, ADXL_INT_DISABLE_ALL);
    if (err) {
        LOG_ERROR("ADXL345_INT_ENABLE devre disi birakma hatasi: %d", err);
        return err;
    }

    /*!< ADXL345_POWER_CTL Register: Link-bit ve auto-sleep ayari */
    err = write_reg(&spispec, ADXL345_POWER_CTL,    ADXL_POWER_CTL_LINK | 
                                                    ADXL_POWER_CTL_AUTO_SLEEP);
    if (err) {
        LOG_ERROR("ADXL345_POWER_CTL ayarlama hatasi: %d", err);
        return err;
    }

    /*!< ADXL345_THRESH_ACT Register: Aktivite threshold tanimlama */
    err = write_reg(&spispec, ADXL345_THRESH_ACT, ADXL_THRESH_ACT_500MG);
    if (err) {
        LOG_ERROR("ADXL345_THRESH_ACT threshold ayarlama hatasi: %d", err);
        return err;
    }

    /*!< ADXL345_THRESH_INT Register: Inaktivite threshold tanimlama */
    err = write_reg(&spispec, ADXL345_THRESH_INT, ADXL_THRESH_INACT_500MG);
    if (err) {
        LOG_ERROR("ADXL345_THRESH_INT threshold ayarlama hatasi: %d", err);
        return err;
    }

    /*!< ADXL345_TIME_INACT Register: Uyku suresi ayari (yaklasik 10 sn) */
    err = write_reg(&spispec, ADXL345_TIME_INACT, ADXL_TIME_INACT_10_SEC);
    if (err) {
        LOG_ERROR("ADXL345_TIME_INACT sure ayarlama hatasi: %d", err);
        return err;
    }

    /*!< ADXL345_ACT_INACT_CTL Register: X, Y eksenleri aktivite/inaktivite DC mod ayari */
    err = write_reg(&spispec,   ADXL345_ACT_INACT_CTL, 
                                ADXL_ACT_INACT_CTL_ACT_X_ENABLE     |   ADXL_ACT_INACT_CTL_ACT_Y_ENABLE | 
                                ADXL_ACT_INACT_CTL_INACT_X_ENABLE   |   ADXL_ACT_INACT_CTL_INACT_Y_ENABLE);
    if (err) {
        LOG_ERROR("ADXL345_ACT_INACT_CTL ayarlama hatasi: %d", err);
        return err;
    }

    /*!< ADXL345_INT_MAP Register: Interrupt pin ayari (int2 uzerinden interrupt) */
    err = write_reg(&spispec, ADXL345_INT_MAP,  ADXL_INT_MAP_ACTIVITY | 
                                                ADXL_INT_MAP_INACTIVITY );
    if (err) {
        LOG_ERROR("ADXL345_INT_MAP pin ayarlama hatasi: %d", err);
        return err;
    }

    /*!< ADXL345_INT_ENABLE Register: Interruptlari etkinlestirme */
    err = write_reg(&spispec, ADXL345_INT_ENABLE,   ADXL_INT_ENABLE_ACTIVITY | 
                                                    ADXL_INT_ENABLE_INACTIVITY );
    if (err) {
        LOG_ERROR("ADXL345_INT_ENABLE etkinlestirme hatasi: %d", err);
        return err;
    }

    /*!< ADXL345_POWER_CTL Register: Olcum modu, link-bit ve auto-sleep etkinlestirme */
    err = write_reg(&spispec, ADXL345_POWER_CTL,    ADXL_POWER_CTL_LINK         | 
                                                    ADXL_POWER_CTL_AUTO_SLEEP   | 
                                                    ADXL_POWER_CTL_MEASURE );
    if (err) {
        LOG_ERROR("ADXL345_POWER_CTL olcum modu etkinlestirme hatasi: %d", err);
        return err;
    }

    /*!< ADXL345 yapilandirma tamamlandi */
    LOG_INFO("ADXL345 yapilandirma basariyla tamamlandi.");

    uint8_t data[1];
    spi_read_reg(&spispec , ADXL345_DEVID_REG , data , 1);
    LOG_DEBUG("Spi device id : %d \n " , data[0]);
    return 0;

}




static int adxl345_init_func(const struct device *dev)
{
    ARG_UNUSED(dev); 
    init_adxl_interrupt();

    return 0 ; 
	 
}


SYS_INIT(adxl345_init_func, APPLICATION, ADXL345_INIT_PRIORITY);