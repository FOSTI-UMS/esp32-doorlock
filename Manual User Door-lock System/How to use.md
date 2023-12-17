# How to Use Door-Lock System

**Powered By FOSTI UMS**

✨ Door-lock system with fingerprint✨  

### Langkah 1 :
Membuka server ESP32
1. Menghubungkan Wi-Fi ESP32 dengan device
2. Masukkan alamat IP pada browser anda dengan 192.168.4.1
3. Selanjutnya akan muncul tampilan seperti dibawah ini

![](https://github.com/FOSTI-UMS/esp32-doorlock/blob/main/assets/use/dashboard.png)

### Langkah 2 :
Pada halaman Dashboard terdapat 4 fitur, yaitu:
1. DAFTAR USER
    Fitur ini menampilkan daftar sidik jari yang sudah terdaftar pada modul fingerprint. Sekaligus dapat dilakukan penghapusan data sidik jari sesuai ID yang terdaftar dengan memasukkan password admin. Tampilan pada fitur Daftar User sebagai berikut:
    ![](https://github.com/FOSTI-UMS/esp32-doorlock/blob/main/assets/use/list.png)
    
    Langkah menghapus data sidik jari:
    - Pilih ID yang akan dihapus dengan ceklist checkbox.
    - ID yang dipilih bisa lebih dari satu, disesuaikan dengan data yang akan dihapus.
    - Masukkan password admin.
    - Klik button `Delete Selected`. 
    - Apabila data sidik jari terhapus, akan ditampilan sebagai berikut:
    ![](https://github.com/FOSTI-UMS/esp32-doorlock/blob/main/assets/use/delete.png)
    - Klik button `HOME` untuk kembali ke Dashboard.

2. ENROLL BARU
    Fitur ini digunakan untuk mendaftarkan sidik jari serta menampilkan kapasitas yang masih dapat ditampung oleh modul fingerprint. Tampilan pada fitrur Enroll Baru sebagai berikut:
    ![](https://github.com/FOSTI-UMS/esp32-doorlock/blob/main/assets/use/enroll.png)

    Langkah mendaftarkan sidik jari:
    - Pilih ID yang tersedia.
    - Masukkan nama untuk disimpan dalam database modul fingerprint.
    - Masukkan password admin.
    - Klik button `Submit` untuk melakukan pendaftaran.
    - Berikut tampilan device setalah klik tombol `Submit` 
    ![](https://github.com/FOSTI-UMS/esp32-doorlock/blob/main/assets/use/prosesEnroll.png)
    - Ikuti langkah yang diinstruksikan pada layar LCD hingga selesai.
    - Data sidik jari akan tersimpan dalam modul fingerprint.

3. EMPTY DATA
    Fitur ini digunakan untuk menghapus seluruh data sidik jari yang sudah terdaftar tanpa terkecuali. Perlu diingat, data sidik jari yang sudah terhapus tidak dapat dikembalikan sehingga hati-hati dalam menggunakan fitur ini. Tampilan pada fitur Empty Data sebagai berikut:
    ![](https://github.com/FOSTI-UMS/esp32-doorlock/blob/main/assets/use/deleteDatabase.png)

    Langkah mengosongkan data sidik jari:
    - Masukkan password admin.
    - Apabila password admin salah, akan ditampilkan sebagai berikut:
    ![](https://github.com/FOSTI-UMS/esp32-doorlock/blob/main/assets/use/wrong.png)
    - Apabila password admin yang dimasukkan benar, seluruh data sidik jari akan terhapus.

4. BUKA KUNCI
    Fitur ini digunakan untuk membuka pintu dari Dashboard tanpa menggunakan sidik jari.

## License

MIT

**FOSTI UMS 2023s**

