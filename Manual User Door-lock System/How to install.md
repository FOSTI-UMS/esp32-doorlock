# How to Install Door-Lock System

Powered By FOSTI UMS

✨ Door-lock system with fingerprint✨  

## Installation
### Langkah 1: Menyiapkan Alat dan Bahan
Bahan :
1. Modul NodeMCU ESP32
2. PCB Board
3. Modul Fingerprint
4. LCD TFT 7889
5. Antenna
6. Battery 12V
7. Battery Management System (BMS)
8. Solenoid Lock
9. Elco, Resistor, LED, Dioda, Transistor, Terminal (Sesuai desain PCB Board)
10. Saklar
11. Buzzer
12. Adaptor 12-14 V
13. Kabel Jumper

Alat :
1. PC/Laptop
2. Micro-USB to USB cable
3. Breadboard

### Langkah 2: Menginstall Software
1. Instal Arduino IDE pada perangkat komputer Anda.

### Langkah 3: Menghubungkan Perangkat
Ikuti skema koneksi pada gambar [](https://github.com/FOSTI-UMS/esp32-doorlock/blob/main/assets/assembling/assembling.png). Pastikan semua komponen telah terhubung dengan benar.

### Langkah 4: Menghubungkan ESP32 ke Komputer
Sambungkan ESP32 ke komputer menggunakan kabel USB. Lihat gambar [](https://github.com/FOSTI-UMS/esp32-doorlock/blob/main/assets/assembling/assembling1.jpeg).

### Langkah 5: Membuka Arduino IDE
Buka Arduino IDE di komputer Anda.

### Langkah 6: Menginstall Modul-modul
Instal modul-modul berikut pada Arduino IDE:
1. Arduino
2. WiFi
3. SoftwareSerial
4. WiFiClient
5. WebServer
6. Adafruit_Fingerprint
7. SPI
8. ArduinoJson
9. TFT_eSPI

### Langkah 7: Salin Kode
Salin kode dari file [src/main.ino](https://github.com/FOSTI-UMS/esp32-doorlock/blob/main/src/main.ino) dan pastekan ke dalam Arduino IDE.

### Langkah 8: Memilih Board dan Port
Pilih board dan port yang sesuai dengan ESP32 Anda pada Arduino IDE. Lihat gambar [](https://github.com/FOSTI-UMS/esp32-doorlock/blob/main/assets/install/port.png).

### Langkah 9: Kompilasi dan Unggah
Klik tombol kompilasi (ikon ceklis) untuk memastikan tidak ada kesalahan dalam kode. Setelah itu, klik tombol unggah (ikon panah menghadap ke kanan) untuk mengunggah program ke ESP32. Tunggu hingga proses kompilasi selesai. Lihat gambar [](https://github.com/FOSTI-UMS/esp32-doorlock/blob/main/assets/install/compile.png), [](https://github.com/FOSTI-UMS/esp32-doorlock/blob/main/assets/install/waiting.png).

### Catatan Penting:
Pastikan bahwa ESP32 telah terhubung dengan komputer melalui kabel USB sebelum mengompilasi atau mengunggah program.

Setelah langkah-langkah di atas selesai, Door-Lock System dengan fingerprint seharusnya dapat berfungsi dengan baik.

## License
MIT

**FOSTI UMS 2023**

