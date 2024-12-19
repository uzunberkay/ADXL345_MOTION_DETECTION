#ifndef     UTILS_H
#define     UTILS_H

#include <zephyr/logging/log.h>
#include <zephyr/init.h>

// Metin Renkleri
#define RESET_COLOR     "\033[0m"    
#define CYAN_COLOR      "\033[36m"   
#define BLUE_COLOR      "\033[34m"   
#define RED_COLOR       "\033[31m"   
#define YELLOW_COLOR    "\033[33m"   

// Arka Plan Renkleri
#define BG_COLOR_BLACK   "\033[40m"
#define BG_COLOR_RED     "\033[41m"
#define BG_COLOR_GREEN   "\033[42m"
#define BG_COLOR_YELLOW  "\033[43m"
#define BG_COLOR_BLUE    "\033[44m"
#define BG_COLOR_MAGENTA "\033[45m"
#define BG_COLOR_CYAN    "\033[46m"
#define BG_COLOR_WHITE   "\033[47m"

// Parlak Metin Renkleri
#define COLOR_BRIGHT_BLACK   "\033[90m"
#define COLOR_BRIGHT_RED     "\033[91m"
#define COLOR_BRIGHT_GREEN   "\033[92m"
#define COLOR_BRIGHT_YELLOW  "\033[93m"
#define COLOR_BRIGHT_BLUE    "\033[94m"
#define COLOR_BRIGHT_MAGENTA "\033[95m"
#define COLOR_BRIGHT_CYAN    "\033[96m"
#define COLOR_BRIGHT_WHITE   "\033[97m"

// Parlak Arka Plan Renkleri
#define BG_COLOR_BRIGHT_BLACK   "\033[100m"
#define BG_COLOR_BRIGHT_RED     "\033[101m"
#define BG_COLOR_BRIGHT_GREEN   "\033[102m"
#define BG_COLOR_BRIGHT_YELLOW  "\033[103m"
#define BG_COLOR_BRIGHT_BLUE    "\033[104m"
#define BG_COLOR_BRIGHT_MAGENTA "\033[105m"
#define BG_COLOR_BRIGHT_CYAN    "\033[106m"
#define BG_COLOR_BRIGHT_WHITE   "\033[107m"


#define COLOR_RESET     "\033[0m"    // Reset / Normal Metin
#define COLOR_BOLD      "\033[1m"    // Kalın Metin
#define COLOR_UNDERLINE "\033[4m"    // Altı Çizili Metin
#define COLOR_INVERT    "\033[7m"    // Ters Metin (Background ve Foreground renk değişir)



#define LOG_DEBUG(fmt, ...) LOG_DBG(COLOR_BRIGHT_MAGENTA fmt RESET_COLOR, ##__VA_ARGS__)    // DEBUG logu cyan
#define LOG_INFO(fmt, ...) LOG_INF(COLOR_BRIGHT_CYAN fmt RESET_COLOR, ##__VA_ARGS__)     // INFO logu mavi
#define LOG_ERROR(fmt, ...) LOG_ERR(RED_COLOR fmt COLOR_BOLD, ##__VA_ARGS__)      // ERROR logu kırmızı
#define LOG_WARNING(fmt, ...) LOG_WRN(YELLOW_COLOR fmt RESET_COLOR, ##__VA_ARGS__) // WARNING logu sarı

/**
 * @brief private: Fonksiyonun yalnızca bu dosyada kullanılacağını ifade eder.
 *
 * Bu makro, bir fonksiyonun yalnızca bulunduğu `.c` dosyası içinde görünür olmasını
 * sağlamak için kullanılan `static` anahtar kelimesini temsil eder.
 * Amaç, modüler yapıyı vurgulamak ve fonksiyonların erişim seviyelerini kodu okuyanlara
 * daha net bir şekilde anlatmaktır. Sadece fonksiyonlarda kullanılır.
 *
 * @note Bu makro yalnızca **fonksiyonlar** için kullanılır ve değişkenlerle ilgili bir anlam ifade etmez.
 */
#define private static

/**
 * @brief public: Fonksiyonun dış dosyalardan erişilebilir olduğunu belirtir.
 *
 * Bu makro, herhangi bir işlevsellik eklemez veya değiştirmez. Ancak okunabilirliği artırmak için 
 * dış dosyalardan erişilebilir (global) fonksiyonları ayırmaya yardımcı olur.
 * Fonksiyonun diğer dosyalar tarafından kullanılabilir olduğunu vurgulamak için yazılır.
 *
 * @note Bu makro yalnızca **fonksiyonlar** için kullanılır ve değişkenlerle ilgili bir anlam ifade etmez.
 */
#define public




#endif