# Proyek Real Time Clock (RTC) Arduino NANO
Proyek kecil-kecilan untuk kesenangan pribadi. Sebuah RTC dengan fitur:.
1. Menampilkan Hari, Tanggal, Bulan, Tahun.
2. Menampilkan Jam, Menit, Detik.
3. Data waktu dapat diubah dengan tombol.
4. Alarm, yang dapat diatur menggunakan tombol, dan tersimpan di EEPROM.
5. TOmbol dengan debounce.

# Hardware utama
Dalam proyek ini dibutuhkan:
1. Arduino Nano
2. LCD 16x2 I2C
3. RTC DS3231
4. Push Button

# Software dan Lib
Proyek ini memanfaatkan software berikut:
1. Arduino IDE 1.8.4
2. Library LiquidCrytsal_I2C: LiquidCrystal Arduino library for the DFRobot I2C LCD displays, by Marco Schwartz.
3. Library RBD _Button: Read and debounce buttons and switches without delay or interrupts, by Alex Taujenis.
4. Library RBD_Timer: he Arduino real-time loop stops advancing when you write delay() or use interrupts in your sketch, by Alex Taujenis.
5. Library RTCLib_by_Neiron: A library that makes interfacing DS1302, DS1307, DS3231, PCF8583, PCF8563, RTC_Millis Real Time Clock modules easy.


