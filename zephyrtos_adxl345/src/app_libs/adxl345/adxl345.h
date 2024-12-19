/**
 * @file adxl345.h
 * @brief ADXL345 Sensörü için Register Tanımları ve Fonksiyonlar
 * 
 * @author uzunberkay
 * @date 19.12.2024
 */
#ifndef ADXL345_H
#define ADXL345_H

#ifdef __cplusplus
extern "C" {
#endif

#include "utils.h"
#include<zephyr/drivers/spi.h>

/**
 * @brief ADXL init fonksiyonu öncelik seviyesi.
 */
#define ADXL345_INIT_PRIORITY 71


/** 
 * @brief SPI Ayarları 
 * 8 bit veri, MSB (Most Significant Bit) ilk gönderilir, CPOL=1 ve CPHA=1.
 * Bu ayarlar, ADXL345 sensörünün SPI iletişimi için gereklidir.
 */
#define SPIOP                     (SPI_WORD_SET(8) | SPI_TRANSFER_MSB | SPI_MODE_CPOL | SPI_MODE_CPHA)

/** 
 * @brief ADXL345 Okuma ve Multi-Byte Bit Tanımları
 * ADXL345 cihazına SPI üzerinden okuma ve çoklu veri aktarımı için tanımlamalar.
 */
#define ADXL_SPI_READ             0x80 /*!< Register okuma işlemi için gerekli bit */
#define ADXL_SPI_MB               0x40 /*!< Çoklu (multi-byte) okuma/yazma işlemleri için bit */

/** 
 * @brief ADXL345 Register Adresleri 
 * ADXL345 sensöründe bulunan register (yazma/okuma yapılabilen adresler) tanımları.
 */
#define ADXL345_POWER_CTL         0x2D /*!< Güç kontrolü register adresi */ 
#define ADXL345_DEVID_REG         0x00 /*!< Başlangıç register adresi */
#define ADXL345_INT_ENABLE        0x2E /*!< Interrupt enable (açma/kapama) register adresi */
#define ADXL345_INT_MAP           0x2F /*!< Interrupt haritalama register adresi */ 
#define ADXL345_INT_SOURCE        0x30 /*!< Interrupt kaynak register adresi */ 
#define ADXL345_THRESH_INT        0x25 /*!< Aktivite/inaktivite eşik değeri register adresi */
#define ADXL345_THRESH_ACT        0x24 /*!< Aktivite algılama eşik değeri register adresi */
#define ADXL345_TIME_INACT        0x26 /*!< İnaktivite süresi register adresi */
#define ADXL345_ACT_INACT_CTL     0x27 /*!< Aktivite/inaktivite kontrol register adresi */ 
#define ADXL345_BW_RATE           0x2C /*!< Bant genişliği ve veri hızı register adresi */ 
#define ADXL345_DATA_FORMAT       0x31 /*!< Veri formatı ayarları register adresi */ 
#define ADXL345_ID_DEVID          0xE5 /*!< Cihaz kimliği (Device ID) register adresi */


/** @brief POWER_CTL Register Bit Tanımlamaları */
#define ADXL_POWER_CTL_LINK             0x20 /*!< Link bit: D5 */       
#define ADXL_POWER_CTL_AUTO_SLEEP       0x10 /*!< AUTO_SLEEP bit: D4 */
#define ADXL_POWER_CTL_MEASURE          0x08 /*!< Measure bit: D3 */
#define ADXL_POWER_CTL_SLEEP            0x04 /*!< Sleep bit: D2 */
#define ADXL_POWER_CTL_WAKEUP_8_HZ      0x00 /*!< Wakeup 8 Hz */
#define ADXL_POWER_CTL_WAKEUP_4_HZ      0x01 /*!< Wakeup 4 Hz */
#define ADXL_POWER_CTL_WAKEUP_2_HZ      0x02 /*!< Wakeup 2 Hz */
#define ADXL_POWER_CTL_WAKEUP_1_HZ      0x03 /*!< Wakeup 1 Hz */        

/** @brief INT_ENABLE Register Bit Tanımlamaları */
#define ADXL_INT_ENABLE_DATA_READY      0x80 /*!< DATA_READY interrupt */
#define ADXL_INT_ENABLE_SINGLE_TAP      0x40 /*!< SINGLE_TAP interrupt */
#define ADXL_INT_ENABLE_DOUBLE_TAP      0x20 /*!< DOUBLE_TAP interrupt */
#define ADXL_INT_ENABLE_ACTIVITY        0x10 /*!< Activity interrupt */
#define ADXL_INT_ENABLE_INACTIVITY      0x08 /*!< Inactivity interrupt */
#define ADXL_INT_ENABLE_FREE_FALL       0x04 /*!< Free fall interrupt */
#define ADXL_INT_ENABLE_WATERMARK       0x02 /*!< FIFO watermark interrupt */
#define ADXL_INT_ENABLE_OVERRUN         0x01 /*!< FIFO overrun interrupt */
#define ADXL_INT_DISABLE_ALL            0x00 /*!< Tüm interrupt'ları devre dışı bırak */


/** @brief INT_MAP Register Bit Tanımlamaları */
#define ADXL_INT_MAP_DATA_READY         0x80 /*!< DATA_READY    */
#define ADXL_INT_MAP_SINGLE_TAP         0x40 /*!< SINGLE_TAP    */
#define ADXL_INT_MAP_DOUBLE_TAP         0x20 /*!< DOUBLE_TAP    */
#define ADXL_INT_MAP_ACTIVITY           0x10 /*!< Activity      */
#define ADXL_INT_MAP_INACTIVITY         0x08 /*!< Inactivity    */
#define ADXL_INT_MAP_FREE_FALL          0x04 /*!< FREE_FALL     */
#define ADXL_INT_MAP_WATERMARK          0x02 /*!< Watermark     */
#define ADXL_INT_MAP_OVERRUN            0x01 /*!< Overrun       */

/** @brief INT_SOURCE Register Bit Tanımlamaları */
#define ADXL_INT_SOURCE_DATA_READY      0x80 /*!< DATA_READY aktif      */
#define ADXL_INT_SOURCE_SINGLE_TAP      0x40 /*!< SINGLE_TAP aktif      */
#define ADXL_INT_SOURCE_DOUBLE_TAP      0x20 /*!< DOUBLE_TAP aktif      */
#define ADXL_INT_SOURCE_ACTIVITY        0x10 /*!< Activity aktif        */
#define ADXL_INT_SOURCE_INACTIVITY      0x08 /*!< Inactivity aktif      */
#define ADXL_INT_SOURCE_FREE_FALL       0x04 /*!< FREE_FALL aktif       */
#define ADXL_INT_SOURCE_WATERMARK       0x02 /*!< Watermark aktif       */
#define ADXL_INT_SOURCE_OVERRUN         0x01 /*!< Overrun aktif         */

/** @brief THRESH_INACT Register (0x25) ölçek faktörü */
#define ADXL_THRESH_INACT_SCALE           62.5 /*!< Ölçek faktörü: 62.5 mg/LSB */

/** @brief THRESH_INACT için Eşik Değerleri (mg cinsinden hesaplanmış)          */
#define ADXL_THRESH_INACT_0MG             0x00 /*!< 0 mg (tavsiye edilmez)      */
#define ADXL_THRESH_INACT_125MG           0x02 /*!< 125 mg                      */
#define ADXL_THRESH_INACT_250MG           0x04 /*!< 250 mg                      */
#define ADXL_THRESH_INACT_500MG           0x08 /*!< 500 mg                      */
#define ADXL_THRESH_INACT_1000MG          0x10 /*!< 1000 mg                     */
#define ADXL_THRESH_INACT_2000MG          0x20 /*!< 2000 mg                     */
#define ADXL_THRESH_INACT_4000MG          0x40 /*!< 4000 mg                     */
#define ADXL_THRESH_INACT_8000MG          0x80 /*!< 8000 mg                     */
#define ADXL_THRESH_INACT_MAX             0xFF /*!< Maksimum değer (15.875 g)   */


/** @brief THRESH_ACT Register (0x24) ölçek faktörü */
#define ADXL_THRESH_ACT_SCALE           62.5 /*!< Ölçek faktörü: 62.5 mg/LSB */

/** @brief THRESH_ACT için Eşik Değerleri (mg cinsinden hesaplanmış) */
#define ADXL_THRESH_ACT_0MG             0x00 /*!< 0 mg (tavsiye edilmez, sorun yaratabilir) */
#define ADXL_THRESH_ACT_125MG           0x02 /*!< 125 mg                                    */
#define ADXL_THRESH_ACT_250MG           0x04 /*!< 250 mg                                    */
#define ADXL_THRESH_ACT_500MG           0x08 /*!< 500 mg                                    */
#define ADXL_THRESH_ACT_1000MG          0x10 /*!< 1000 mg                                   */
#define ADXL_THRESH_ACT_2000MG          0x20 /*!< 2000 mg                                   */
#define ADXL_THRESH_ACT_4000MG          0x40 /*!< 4000 mg                                   */
#define ADXL_THRESH_ACT_8000MG          0x80 /*!< 8000 mg                                   */
#define ADXL_THRESH_ACT_MAX             0xFF /*!< Maksimum değer: 15.875 g                  */


/** @brief TIME_INACT Register (0x26) ölçek faktörü */
#define ADXL_TIME_INACT_SCALE             1 /*!< Ölçek faktörü: 1 saniye/LSB */

/** @brief TIME_INACT için zaman eşik değerleri */
#define ADXL_TIME_INACT_0_SEC             0x00 /*!< 0 saniye (istenmeyen durum, anında tetikleme)   */
#define ADXL_TIME_INACT_1_SEC             0x01 /*!< 1 saniye hareketsizlik süresi                   */
#define ADXL_TIME_INACT_2_SEC             0x02 /*!< 2 saniye hareketsizlik süresi                   */
#define ADXL_TIME_INACT_5_SEC             0x05 /*!< 5 saniye hareketsizlik süresi                   */
#define ADXL_TIME_INACT_10_SEC            0x0A /*!< 10 saniye hareketsizlik süresi                  */
#define ADXL_TIME_INACT_30_SEC            0x1E /*!< 30 saniye hareketsizlik süresi                  */
#define ADXL_TIME_INACT_60_SEC            0x3C /*!< 60 saniye hareketsizlik süresi                  */
#define ADXL_TIME_INACT_120_SEC           0x78 /*!< 120 saniye hareketsizlik süresi                 */
#define ADXL_TIME_INACT_MAX               0xFF /*!< Maksimum değer: 255 saniye                      */




/** @brief ACT_INACT_CTL Register Bit Tanımlamaları */
#define ADXL_ACT_INACT_CTL_ACT_AC_DC        0x80 /*!< ACT AC/DC seçimi      */
#define ADXL_ACT_INACT_CTL_ACT_X_ENABLE     0x40 /*!< ACT X etkin           */
#define ADXL_ACT_INACT_CTL_ACT_Y_ENABLE     0x20 /*!< ACT Y etkin           */
#define ADXL_ACT_INACT_CTL_ACT_Z_ENABLE     0x10 /*!< ACT Z etkin           */
#define ADXL_ACT_INACT_CTL_INACT_AC_DC      0x08 /*!< INACT AC/DC seçimi    */
#define ADXL_ACT_INACT_CTL_INACT_X_ENABLE   0x04 /*!< INACT X etkin         */
#define ADXL_ACT_INACT_CTL_INACT_Y_ENABLE   0x02 /*!< INACT Y etkin         */
#define ADXL_ACT_INACT_CTL_INACT_Z_ENABLE   0x01 /*!< INACT Z etkin         */

/** @brief BW_RATE Register Bit Tanımlamaları */
#define ADXL_BW_RATE_LOW_POWER           0x10 /*!< LOW_POWER modu   */
#define ADXL_BW_RATE_3200HZ              0x0F /*!< 3200 Hz          */
#define ADXL_BW_RATE_1600HZ              0x0E /*!< 1600 Hz          */
#define ADXL_BW_RATE_800HZ               0x0D /*!< 800 Hz           */
#define ADXL_BW_RATE_400HZ               0x0C /*!< 400 Hz           */
#define ADXL_BW_RATE_200HZ               0x0B /*!< 200 Hz           */
#define ADXL_BW_RATE_100HZ               0x0A /*!< 100 Hz           */
#define ADXL_BW_RATE_50HZ                0x09 /*!< 50 Hz            */
#define ADXL_BW_RATE_25HZ                0x08 /*!< 25 Hz            */
#define ADXL_BW_RATE_12_5HZ              0x07 /*!< 12.5 Hz          */
#define ADXL_BW_RATE_6_25HZ              0x06 /*!< 6.25 Hz          */
#define ADXL_BW_RATE_3_13HZ              0x05 /*!< 3.13 Hz          */
#define ADXL_BW_RATE_1_56HZ              0x04 /*!< 1.56 Hz          */
#define ADXL_BW_RATE_0_78HZ              0x03 /*!< 0.78 Hz          */
#define ADXL_BW_RATE_0_39HZ              0x02 /*!< 0.39 Hz          */
#define ADXL_BW_RATE_0_20HZ              0x01 /*!< 0.20 Hz          */
#define ADXL_BW_RATE_0_10HZ              0x00 /*!< 0.10 Hz          */

/** @brief DATA_FORMAT Register Bit Tanımlamaları */
#define ADXL_DATA_FORMAT_RANGE_2G        0x00 /*!< ±2g */
#define ADXL_DATA_FORMAT_RANGE_4G        0x01 /*!< ±4g */
#define ADXL_DATA_FORMAT_RANGE_8G        0x02 /*!< ±8g */
#define ADXL_DATA_FORMAT_RANGE_16G       0x03 /*!< ±16g */



/** 
 * @brief ADXL345 Interrupt Tanımları 
 * Interrupt kaynakları için bit tanımları.
 */
#define ADXL345_ACT_INTERRUPT_MASK      0x10 /*!< Aktivite algılama interrupt biti */
#define ADXL345_INACT_INTERRUPT_MASK    0x08 /*!< İnaktivite algılama interrupt biti */


public int spi_read_reg( const struct spi_dt_spec *spispec , uint8_t reg , uint8_t *data , uint8_t size );
public struct spi_dt_spec* get_spi_device(void);


#ifdef __cplusplus
}
#endif

#endif // ADXL345_H
