# ADXL345 Hareket Algılama Projesi

Bu proje, **Zephyr RTOS** kullanılarak **ADXL345 ivmeölçer sensörü** ile hareket (aktivite) ve hareketsizlik (inaktivite) algılamasını gerçekleştiren bir uygulamadır. **nRF52833 DK** ve **nRF52840 DK** kartları için yapılandırılmıştır. Proje, güç tüketimini optimize ederek otomatik uyku (Auto-Sleep) modunda çalışır ve 23 µA gibi düşük bir akım tüketir.

---

## **Özellikler**

- **Zephyr RTOS** kullanılarak geliştirilmiştir.
- **nRF Connect SDK v2.7.0** versiyonu kullanılmıştır.
- **nRF52833 DK** ve **nRF52840 DK** geliştirme kartlarına uygun **overlay dosyaları** eklenmiştir.
- **ADXL345** sensörü:
  - Düşük güç modunda çalışacak şekilde yapılandırılmıştır.
  - **Hareket algılama**: Aktivite algılandığında sistem uyanır.
  - **Hareketsizlik algılama**: 10 saniye hareketsizlik tespit edilirse güç tasarrufu moduna geçer.
  - **Auto-Sleep modu**: Hareketsizlik durumunda sensör 23 µA akım tüketir.
- **SPI iletişimi** kullanılarak sensörle haberleşme sağlanmıştır.
- **Interrupt yönetimi**: Aktivite ve inaktivite olayları interrupt'lar ile tetiklenir.

---


## **Kullanım Alanları**

Bu proje, güç tasarrufu ve hareket algılamanın kritik olduğu çeşitli uygulamalar için uygundur:

1. **Taşınabilir IoT Cihazları:**
   - Düşük güç tüketimi ile batarya ömrü uzatılır.
   - Hareket algılandığında sistem aktif hale gelir.

2. **Giyilebilir Teknolojiler:**
   - Akıllı bileklikler veya giyilebilir cihazlarda hareketsizlik durumunda güç tasarrufu sağlanır.

3. **Titreşim ve Hareket Dedektörleri:**
   - Endüstriyel ortamlarda titreşim algılamak veya hareketsiz sistemleri tespit etmek için kullanılabilir.

4. **Akıllı Ev Sistemleri:**
   - Kapı, pencere veya nesne hareketlerini algılamak için idealdir.

5. **Endüstriyel Sensörler:**
   - Makine durumu izleme ve enerji verimliliği uygulamaları için kullanılabilir.

---

## **Kurulum Adımları**

1. **Gereksinimler:**
   - **nRF Connect SDK v2.7.0**
   - **Zephyr RTOS**
   - **nRF52833 DK** veya **nRF52840 DK** geliştirme kartı.

2. **Donanım Yapılandırması:**
   - **nRF52833 DK** ve **nRF52840 DK** için overlay dosyaları eklenmiştir:
     - `nrf52833.overlay`
     - `nrf52840dk_nrf52840.overlay`

3. **Proje Derleme ve Yükleme:**
   - **nRF Connect for VSCode** kullanarak projeyi derleyin.
   - Aşağıdaki komut ile projeyi karta yükleyin:
     ```bash
     west build -b nrf52833dk_nrf52833
     west flash
     ```

4. **Çalıştırma:**
   - Proje yüklendikten sonra hareket algılama işlemini gözlemlemek için uygun sensör bağlantılarını sağlayın.

---

## **Dosya Yapısı**

```plaintext
src/
├── app_libs/                                # Kütüphane klasörleri
│   ├── adxl345/                             # ADXL345 sensör konfigürasyonu
│   ├── gpio_settings/                       # GPIO pin ayarları
│   ├── motion_detection/                    # Hareket algılama işlevleri
│   └── utils/                               # Yardımcı fonksiyonlar ve genel araçlar
├── prj.conf                                 # Zephyr RTOS proje yapılandırma dosyası
├── nrf52833.overlay                         # nRF52833  için donanım tanımı
├── nrf52840dk.overlay                       # nRF52840 DK için donanım tanımı
└── CMakeLists.txt                           # Proje derleme yapılandırma dosyası

