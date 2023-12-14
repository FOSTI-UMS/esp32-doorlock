#include <Arduino.h>              // Library dasar Arduino
#include <WiFi.h>                 // Library WiFi untuk koneksi nirkabel
#include <SoftwareSerial.h>       // Library untuk komunikasi serial perangkat tambahan
#include <WiFiClient.h>           // Library klien WiFi
#include <WebServer.h>            // Library server web untuk mengelola permintaan HTTP
#include <Adafruit_Fingerprint.h> // Library sidik jari dari Adafruit
#include <SPI.h>                  // Library komunikasi SPI
#include <ArduinoJson.h>          // Library untuk mengolah data JSON
#include <TFT_eSPI.h>             // Library untuk mengendalikan layar TFT
#include "logowaiting.h"          // Gambar tampilan menunggu
#include "logofalse.h"            // Gambar tampilan gagal
#include "logotrue.h"             // Gambar tampilan berhasil
#include "logofosti.h"            // Gambar logo FOSTI

int relayPin = 12;                // Pin untuk mengendalikan relay
int buzzer = 4;                   // Pin untuk mengendalikan buzzer

TFT_eSPI tft = TFT_eSPI();        // Objek TFT_eSPI untuk mengendalikan layar TFT

const char* ssid = "fosti-sekre";     // SSID untuk koneksi WiFi
const char* password = "";            // Kata sandi WiFi
bool ENROLL_MODE = false;             // Mode pendaftaran sidik jari (true/false)
String ADMIN_PASSWORD = "";           // Kata sandi admin
bool IS_LOCKED = true;                // Status kunci (terkunci/terbuka)
int fpBlink = 500;                    // Interval berkedip untuk indikator sidik jari
int id_baru = 0;                      // ID baru untuk pendaftaran sidik jari
String nama_baru = "";                // Nama baru untuk pendaftaran sidik jari


// Objek WebServer untuk mengelola permintaan HTTP
WebServer server(80);

// Objek Serial untuk komunikasi dengan sensor sidik jari
#define mySerial Serial2
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
int FingerID = 0;
uint8_t id;

// Fungsi untuk memainkan nada pada buzzer dengan frekuensi dan durasi tertentu
void playTone(int frequency, int duration) {
  tone(buzzer, frequency);   // Memainkan nada dengan frekuensi tertentu
  delay(duration);           // Durasi
  noTone(buzzer);            // Mematikan nada
  delay(100);                // Menambahkan jeda
}

#define NOTE_D6  1175
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_A6  1760
#define NOTE_AS6 1865


void toneUp() {
  tone(buzzer, NOTE_A6, 200);
  delay(350);
  tone(buzzer, NOTE_AS6, 500);
  delay(650);
  tone(buzzer, NOTE_D6, 500);
  delay(650);
  tone(buzzer, NOTE_F6, 200);
  delay(350);
  tone(buzzer, NOTE_E6, 200);
  delay(350);
  tone(buzzer, NOTE_D6, 200);
  delay(350);
  tone(buzzer, NOTE_E6, 200);
  delay(350);
  tone(buzzer, NOTE_F6, 600);
  delay(750);
}

void toneDown() {
  playTone(2000, 100);
  playTone(1500, 100);
  playTone(1000, 100);
}

void toneRegistered() {
  playTone(2000, 100);
  playTone(1500, 100);
  playTone(1000, 2000);
}

void toneError() {
  playTone(2000, 2000);
}


uint8_t getFingerprintEnroll(int id, String nama) {
  int p = -1;
  Serial.println(id);

  // Memulai pengambilan gambar sidik jari
  while (p != FINGERPRINT_OK) {
    delay(50);    
    tone(buzzer, 2000);                 
    finger.LEDcontrol(false);
    delay(50);  
    noTone(buzzer);
    finger.LEDcontrol(true);
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Gambar diambil");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println(".");
      tft.fillScreen(TFT_WHITE);
      tft.setTextSize(2);
      tft.setTextColor(TFT_BLACK);
      tft.setCursor(25, 150);
      tft.println("Place Your Finger");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Koneksi Error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Penggambaran Error");
      break;
    default:
      Serial.println("Error");
      break;
    }
  }

  // Mengonversi gambar menjadi template sidik jari
  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Gambar dikonversi");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Gambar tidak jelas");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Koneksi error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Tidak ditemukan sidik");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Tidak ditemukan sidik");
      return p;
    default:
      Serial.println("Error");
      return p;
  }

  // Meminta pengguna untuk mengangkat jari
  Serial.println("Angkat Jari");
  tft.fillScreen(TFT_GREEN);
  tft.setTextSize(2);
  tft.setTextColor(TFT_BLACK);
  tft.setCursor(20, 150);
  tft.println("Remove Your Finger");
  playTone(2000, 200);
  playTone(1500, 200);
  playTone(2000, 500);
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println("Letakan jari yang sama");
  tft.fillScreen(TFT_WHITE);
  tft.setTextSize(2);
  tft.setTextColor(TFT_BLACK);
  tft.setCursor(25, 150);
  tft.println("Place Your Finger");
  tft.setCursor(90, 180);  
  tft.println("Again");

  // Mengambil gambar sidik jari kedua
  while (p != FINGERPRINT_OK) {
    delay(50);    
    tone(buzzer, 2000);                 
    finger.LEDcontrol(false);
    delay(50);  
    noTone(buzzer);
    finger.LEDcontrol(true);
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Gambar diambil");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("koneksi error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Penggambaran Error");
      break;
    default:
      Serial.println("Error");
      break;
    }
  }

  // Mengonversi gambar kedua menjadi template sidik jari
  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Gambar dikonversi");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Gambar tidak jelas");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("koneksi error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("tidak ditemukan sidik");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("tidak ditemukan sidik");
      return p;
    default:
      Serial.println("Error");
      return p;
  }

  // Membuat model sidik jari
  Serial.print("Membuat model FP");  Serial.println(id);
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Cocok !!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Koneksi error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Repeat tidak cocok");
    return p;
  } else {
    Serial.println("Error");
    return p;
  }

  // Menyimpan model sidik jari
  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Disimpan sebagai " + nama);
    tft.fillScreen(TFT_WHITE);
    tft.setTextSize(2);
    tft.setTextColor(TFT_BLACK);
    tft.setCursor(25, 150);
    tft.println("Template saved as");
    int textSize = 2;
    int textWidth = nama.length() * 6 * textSize;  // Lebar teks
    int x = (tft.width() - textWidth) / 2;
    tft.setCursor(x, 180);                         // Set posisi x ke tengah
    tft.println(nama);
    toneRegistered();
    //SET MODE ENROLL FALSE
    ENROLL_MODE = false;
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Koneksi error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("TIdak bisa menyimpan");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Memori Flash Error");
    return p;
  } else {
    Serial.println("Error");
    return p;
  }
  return true;
}

int scanFinger()
{
 uint8_t p = finger.getImage();
 if (p != FINGERPRINT_OK)  
 {
   tft.pushImage(0, 0, 240, 320, logowaiting);
   Serial.println("Waiting For Valid Finger");
   return -1;
 }

 // Mengkonversi gambar menjadi template sidik jari
 p = finger.image2Tz();
 if (p != FINGERPRINT_OK)  
 {
   tft.fillScreen(TFT_BLACK);
   tft.setTextSize(2);
   tft.setTextColor(TFT_WHITE);
   tft.setCursor(0, 0);
   tft.println("Messy  Image");
   tft.setCursor(0, 110);
   tft.println("Try Again");
   
   Serial.println("Messy Image Try Again");
   delay(3000);
   tft.fillScreen(TFT_BLACK);
   return -1;
 }
 
 // Melakukan pencarian sidik jari pada database
 p = finger.fingerFastSearch();
 if (p != FINGERPRINT_OK)  {
   tft.fillScreen(TFT_BLACK);
   tft.pushImage(0, 0, 240, 320, logofalse);
   toneError();
   delay(3000);
   tft.fillScreen(TFT_BLACK);
   return -1;
 }
   openLock();
   return finger.fingerID;
}

String str_isTerkunci() {
  return IS_LOCKED ? "TERKUNCI" : "TERBUKA";
}

// Deklarasi CSS sebagai string di luar fungsi
const char* globalCSS = R"(
<style>
  body {
    display: flex;
    justify-content: center;
    align-items: center;
    font-family: Arial, sans-serif;
    height: 100vh;
    background-color: whitesmoke;
  }
  .box {
    width: 330px;
    padding: 20px;
    background-color: rgba(255, 255, 255, 0.5);
    backdrop-filter: blur(10px);
    border-radius: 10px;
    box-shadow: 0 0 15px rgba(0, 0, 0, 0.3);
  }
  h1 {
    font-size: 24px;
    margin-bottom: 20px;
  }
  .checkbox-label {
    display: flex;
    align-items: center;
    min-width: 250px;
  }
  .checkbox-label input[type="checkbox"] {
    margin-right: 10px;
  }
  .button-hijau {
    background-color: green;
    border: 2px solid green;
    color: white;
    padding: 10px;
    width: 150px;
    border-radius: 5px;
    text-align: center;
    text-decoration: none;
    display: inline-block;
    font-size: 16px;
    margin: 4px 2px;
    transition-duration: 0.5s;
    cursor: pointer;
  }
  .button-hijau:hover {
    background-color: white;
    border: 2px solid green;
    color: black;
  }
  .form-input {
    width: 70%;
    padding: 10px;
    font-size: 16px;
    border: 1px solid #ccc;
    border-radius: 4px;
    box-sizing: border-box;
  }
</style>
)";

//Mengembalikan string HTML yang berisi tautan untuk membuka atau menutup kunci
String str_lock_link() {
  return IS_LOCKED ? "<a class=\"btn button-hijau\" href=\"/unlock\" type=\"button\">BUKA KUNCI</a>" : "<a class=\"btn button-biru\" href=\"/lock\" type=\"button\">TUTUP KUNCI</a>";
}

//Fungsi yang menghasilkan halaman beranda HTML
String homepage() {
  return "<!doctype html>\n"
        "<html lang=\"en\">\n"
        "  <head>\n"
        "    <!-- Required meta tags -->\n"
        "    <meta charset=\"utf-8\">\n"
        "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n"
        "\n"
        "   <style>\n"
        "     body{\n"
        "      background-color: whitesmoke;\n"
        "      font-family: Arial, sans-serif;\n"
        "      }\n"
        "\n"
        "    .box {\n"
        "      position: absolute;\n"
        "      top: 50%;\n"
        "      left: 50%;\n"
        "      transform: translate(-50%, -50%);\n"
        "      display: flex;\n"
        "      justify-content: center;\n"
        "      align-items: center;\n"
        "      width: 400px;\n"
        "      height: 320px;\n"
        "      background-color: rgba(255, 255, 255, 0.5);\n"
        "      backdrop-filter: blur(10px);\n"
        "      border-radius: 10px;\n"
        "      box-shadow: 0 0 15px rgba(0, 0, 0, 0.3);\n"
        "     }\n"
        "\n"
        "    .button-hijau {\n"
        "      background-color: green;\n"
        "      border: 2px solid green;\n"
        "      color: white;\n"
        "      padding: 10px;\n"
        "      width: 130px;\n"
        "      border-radius: 5px;\n"
        "      text-align: center;\n"
        "      text-decoration: none;\n"
        "      display: inline-block;\n"
        "      font-size: 16px;\n"
        "      margin: 4px 2px;\n"
        "      transition-duration: 0.5s;\n"
        "      cursor: pointer;\n"
        "     }\n"
        "\n"
        "    .button-hijau:hover{\n"
        "      background-color: white;\n"
        "      border: 2px solid green;\n"
        "      color: black;\n"
        "    }\n"
        "   </style>\n"
        "   <title>DoorLock Admin Page</title>\n"
        "   </head>\n"
        "<body>\n"
        "  <div class=\"box\">\n"
        "    <div>\n"
        "      <h1 align=\"center\">Dashboard Doorlock</h1>\n"
        "      <div class=\"d-grid gap-2\" align=\"center\">\n"
        "        <a href=\"/listfingerprint\" class=\"btn button-hijau\" type=\"button\">DAFTAR USER</a>\n"
        "        <br>\n"
        "        <a href=\"/enrollpage\" class=\"btn button-hijau\" type=\"button\">ENROLL BARU</a>\n"
        "        <br>\n"      
        "        <a href=\"/deleteDatabase\" class=\"btn button-hijau\" type=\"button\">EMPTY DATA</a>\n"   
        "        <br>\n"   
                  +str_lock_link()+
        "      </div>\n"
        "    </div>\n"
        "  </div>\n"
        "  </body>\n"
        "</html>";
}

//Menangani permintaan halaman yang menampilkan daftar sidik jari yang terdaftar
String handleListFingerprint() {
  // Tambahkan logika untuk mengambil dan menampilkan daftar ID yang terdaftar
  String fingerprintList = "<form action=\"/hapusfingerprint\" method=\"post\">";
  for (int id = 1; id < 150; id++) {
    uint8_t p = finger.loadModel(id);
    if (p == FINGERPRINT_OK) {
      // Menampilkan ID yang terdaftar dengan checkbox
      fingerprintList += "<label class=\"checkbox-label\">";
      fingerprintList += "<input type=\"checkbox\" name=\"id\" value=\"" + String(id) + "\">";
      fingerprintList += " User ID " + String(id);
      fingerprintList += "</label>";
    }
  }
  // Input password admin
  fingerprintList += "<br><input type=\"password\" class=\"form-input\" name=\"password\" placeholder=\"Insert Admin Password to Delete\"><br>";
  // Tombol untuk menghapus ID yang dipilih
  fingerprintList += "<input type=\"submit\" value=\"Delete Selected\" class=\"btn button-hijau\"></form>";

  String html = "<!doctype html>\n"
                "<html lang=\"en\">\n"
                "<head>\n"
                "  <meta charset=\"utf-8\">\n"
                "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n"
                "  <title>Hasil Penghapusan Fingerprint</title>\n"
                "  " + String(globalCSS) + // Menggunakan CSS global di sini
                "</head>\n"
                "<body>\n";
                "  <div class=\"box\">\n"
                "    <h1 align=\"center\">Daftar Fingerprint</h1>\n"
                "    <div align=\"center\">\n"
                "      " + fingerprintList + "\n"              
                "     <form action=\"/\" method=\"post\">\n"
                "       <input type=\"submit\" value=\"HOME\" class=\"btn button-hijau\">\n"
                "     </form>";         
                "    </div>\n"
                "  </div>\n"
                "</body>\n"
                "</html>";

  return html;
}

//Menangani permintaan penghapusan sidik jari
String handleDeleteFingerprint() {
  String html = "<!doctype html>\n"
                "<html lang=\"en\">\n"
                "<head>\n"
                "  <meta charset=\"utf-8\">\n"
                "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n"
                "  <title>Hasil Penghapusan Fingerprint</title>\n"
                "  <style>\n"
                "    body {\n"
                "      display: flex;\n"
                "      justify-content: center;\n"
                "      align-items: center;\n"
                "      font-family: Arial, sans-serif;\n"
                "      height: 100vh;\n"
                "      background-color: whitesmoke;\n"
                "    }\n"
                "    .container {\n"
                "      text-align: center;\n"
                "      padding: 20px;\n"
                "      background-color: rgba(255, 255, 255, 0.5);\n"
                "      backdrop-filter: blur(10px);\n"
                "      border-radius: 10px;\n"
                "      box-shadow: 0 0 15px rgba(0, 0, 0, 0.3);\n"
                "    }\n"
                "    h1 {\n"
                "      font-size: 24px;\n"
                "      margin-bottom: 20px;\n"
                "    }\n"
                "    .button-hijau {\n"
                "      background-color: green;\n"
                "      border: 2px solid green;\n"
                "      color: white;\n"
                "      padding: 10px;\n"
                "      width: 130px;\n"
                "      border-radius: 5px;\n"
                "      text-align: center;\n"
                "      text-decoration: none;\n"
                "      display: inline-block;\n"
                "      font-size: 16px;\n"
                "      margin: 4px 2px;\n"
                "      transition-duration: 0.5s;\n"
                "      cursor: pointer;\n"
                "     }\n"
                "    .button-hijau:hover{\n"
                "      background-color: white;\n"
                "      border: 2px solid green;\n"
                "      color: black;\n"
                "    }\n"
                "  </style>\n"
                "</head>\n"
                "<body>\n";

  // Tambahkan logika untuk memeriksa password admin yang dimasukkan
  if (server.hasArg("password")) {
    String adminPassword = server.arg("password");
    if (adminPassword == ADMIN_PASSWORD) {
      html += "  <div class=\"container\">\n"
              "    <h1>Hasil Penghapusan Fingerprint</h1>\n";

      // Tambahkan logika untuk menghapus sidik jari berdasarkan ID yang dipilih
      for (int i = 0; i < server.args(); i++) {
        if (server.argName(i) == "id") {
          int idToDelete = server.arg(i).toInt();
          uint8_t deleteStatus = finger.deleteModel(idToDelete);

          if (deleteStatus == FINGERPRINT_OK) {
            html += "Fingerprint dengan ID " + String(idToDelete) + " telah dihapus.<br>";
          } else {
            html += "Gagal menghapus fingerprint dengan ID " + String(idToDelete) + ".<br>";
          }
        }
      }

      html += "<br><a href=\"/listfingerprint\" class=\"btn button-hijau\" type=\"button\">BACK</a>\n";
      html += "</div>\n";
    } else {
      // Password admin salah
      html += "  <h1>Password Admin Salah</h1>\n";
    }
  }

  html += "</body>\n"
          "</html>";

  return html;
}

//Menampilkan halaman untuk menambahkan pengguna baru dengan sidik jari
String enrollpage() {
    int sisaSlot = 0;
    String pilihanSlot = "";
    for (int id = 1; id < 150; id++) {
        uint8_t p = finger.loadModel(id);
        if (p == FINGERPRINT_OK) {

        } else {
            pilihanSlot += "  \t\t<option value=\""+String(id)+"\">Slot "+String(id)+"</option>\n";
            sisaSlot++;
        }
    }
  return
    "<!doctype html>\n"
    "<html lang=\"en\">\n"
    "<head>\n"
    "  <!-- Required meta tags -->\n"
    "  <meta charset=\"utf-8\">\n"
    "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n"
    "  <style>\n"
    "    " + String(globalCSS) + // Menggunakan CSS global di sini
    "  </style>\n"
    "  <title>Enroll Fingerprint</title>\n"
    "</head>\n"
    "<body>\n"
    "  <form action=\"/tambahuser\">\n"
    "    <table class=\"form-table\">\n"
    "      <tr>\n"
    "        <td>SISA SLOT</td>\n"
    "        <td>"+String(sisaSlot)+"</td>\n"
    "      </tr>\n"
    "      <tr>\n"
    "        <td>ID</td>\n"
    "        <td>\n"
    "          <select class=\"form-select\" name=\"id\">\n"
    "            "+pilihanSlot+"\n"
    "          </select>\n"
    "        </td>\n"
    "      </tr>\n"
    "      <tr>\n"
    "        <td>NAMA</td>\n"
    "        <td><input type=\"text\" class=\"form-input\" name=\"nama\"></td>\n"
    "      </tr>\n"
    "      <tr>\n"
    "        <td>PASSWORD ADMIN</td>\n"
    "        <td><input type=\"password\" class=\"form-input\" name=\"password\"></td>\n"
    "      </tr>\n"
    "    </table>\n"
    "    <div class=\"form-group\">\n"
    "      <input type=\"submit\" class=\"form-button\" name=\"submit\" value=\"Submit\">\n"
    "    </div>\n"
    "  </form>\n"
    "</body>\n"
    "</html>";
}

//Menangani permintaan untuk menambahkan pengguna baru
String tambahuser(){
  if (server.arg("id") != "" && server.arg("nama") != "" && server.arg("password") != "") {
    int id = server.arg("id").toInt();
    String nama = server.arg("nama");
    String password = server.arg("password");
    if (password == ADMIN_PASSWORD) {
      id_baru = id;
      nama_baru = nama;
      ENROLL_MODE = true;
    } else {
      playTone(500, 5000);
      ENROLL_MODE = false;
    }
  } else {
    playTone(5000, 5000);
    ENROLL_MODE = false;
  }
  return 
  "<!doctype html>\n"
  "<html lang=\"en\">\n"
  "<head>\n"
  "  <!-- Required meta tags -->\n"
  "  <meta charset=\"utf-8\">\n"
  "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n"
  "  <style>\n"
  "    " + String(globalCSS) + // Menggunakan CSS global di sini
  "  </style>\n"
  "  <title>Enroll Fingerprint</title>\n"
  "</head>\n"
  "<body>\n"
  "  <h1>IKUTI INSTRUKSI PADA LCD DISPLAY </h1>\n"
  "  <br><br>\n"
  "  <a href=\"/\" class=\"btn button-hijau\" type=\"button\">HOME</a>\n"
  "</body>\n"
  "</html>\n";
}

//Menangani permintaan untuk mengosongkan database sidik jari
String clearDatabase(){
    finger.getTemplateCount();
    finger.emptyDatabase();
    playTone(2000, 1000);
    tft.fillScreen(TFT_RED);
    tft.setTextSize(2);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(25, 150);
    tft.println("Database Cleared");
    delay(3000);
  return "<!doctype html>\n"
      "<html lang=\"en\">\n"
      "<head>\n"
      "  <meta charset=\"utf-8\">\n"
      "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n"
      "  <style>\n"
      "    " + String(globalCSS) + // Menggunakan CSS global di sini
      "  </style>\n"
      "  <title>Empty Database</title>\n"
      "</head>\n"
      "<body>\n"
      "  <h1>Fingerprint Database Cleared</h1>\n"
      "  <br><br>\n"
      "  <a href=\"/\" class=\"btn button-hijau\" type=\"button\">HOME</a>\n"
      "</body>\n"
      "</html>\n";
}

String deleteDatabase(){
  if (IS_LOCKED) {
  return "<!doctype html>\n"
      "<html lang=\"en\">\n"
      "<head>\n"
      "  <meta charset=\"utf-8\">\n"
      "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n"
      "  <style>\n"
      "    " + String(globalCSS) + // Menggunakan CSS global di sini
      "  </style>\n"
      "  <title>Delete Database</title>\n"
      "</head>\n"
      "<body>\n"
      "  <form action=\"/clearDatabase\" method=\"post\">\n"
      "    <table class=\"form-table\">\n"
      "      <h1 align=\"center\">Masukkan Password</h1>\n"
      "      </tr>\n"
      "      <tr>\n"
      "        <td>PASSWORD ADMIN</td>\n"
      "        <td><input type=\"password\" class=\"form-input\" name=\"password\"></td>\n"
      "      </tr>\n"
      "    </table>\n"
      "    <div class=\"form-group\">\n"
      "      <input type=\"submit\" class=\"form-button\" name=\"submit\" value=\"Submit\">\n"
      "    </div>\n"
      "  </form>\n"
      "</body>\n"
      "</html>";
    } else {
      server.send(403, "text/plain", "Lock is locked. Please unlock before clearing the database");
    }
}


// SETUP WEB SERVER ENDPOINTS
void setupEndpoints() {
    // Mengatur endpoint untuk halaman utama "/"
    server.on("/", [](){
        server.send(200, "text/html",
        homepage()
      );
    });

    // Mengatur endpoint untuk halaman daftar sidik jari "/listfingerprint" dengan metode HTTP GET
    server.on("/listfingerprint", HTTP_GET, []() {
      server.send(200, "text/html", handleListFingerprint());
    });

    // Mengatur endpoint untuk menghapus sidik jari "/hapusfingerprint" dengan metode HTTP POST
    server.on("/hapusfingerprint", HTTP_POST, []() {
      // Periksa apakah password yang dimasukkan benar
      if (server.arg("password") != ""){
        // Password benar, lanjutkan dengan proses penghapusan sidik jari
        server.send(200, "text/html", handleDeleteFingerprint());
      } else {
        // Password salah, tampilkan pesan kesalahan
        server.send(200, "text/html", "Password Admin Salah");
      }
    });

    // Mengatur endpoint untuk halaman enroll "/enrollpage"
    server.on("/enrollpage", [](){
        server.send(200, "text/html", enrollpage());
    });

    // Mengatur endpoint untuk menambahkan pengguna baru "/tambahuser"
    server.on("/tambahuser", [](){
        if (server.arg("password") != "") {
          String password = server.arg("password");
          if (password == ADMIN_PASSWORD) {
            server.send(200, "text/html", tambahuser());
          }else{
            server.send(200, "text/html","Password Admin Salah");
          }
        }    
    });
    
    // Mengatur endpoint untuk menghapus database "/deleteDatabase"
    server.on("/deleteDatabase", [](){
      server.send(200, "text/html", deleteDatabase());
    });

    // Mengatur endpoint untuk menghapus database setelah memasukkan password "/clearDatabase"
    server.on("/clearDatabase", [](){
        if (server.arg("password") != "") {
          String password = server.arg("password");
          if (password == ADMIN_PASSWORD) {
            server.send(200, "text/html", clearDatabase());
          }else{
            server.send(200, "text/html","Password Admin Salah");
          }
        }    
    });

    // Mengatur endpoint untuk membuka kunci "/unlock"
    server.on("/unlock", [](){
    IS_LOCKED = false;
    openLock();
    server.sendHeader("Location", String("/"), true);
    server.send(302, "text/plain",""); 
    });

    // Mengatur endpoint untuk mengunci "/lock"
    server.on("/lock", [](){
      IS_LOCKED = true;
      server.sendHeader("Location", String("/"), true);
      server.send(302, "text/plain",""); 
    });

    // Memulai server
    server.begin();
}

// Fungsi untuk membuka kunci dan memberikan umpan balik visual (LCD TFT) dan audio buzzer
void openLock() {
  digitalWrite(relayPin, HIGH);
  toneUp();
  IS_LOCKED = false;
  tft.fillScreen(TFT_BLACK);
  tft.pushImage(0, 0, 240, 320, logotrue);
  digitalWrite(relayPin, HIGH);
  for (int i = 0; i < 5; i++)
  {
    finger.LEDcontrol(true);
    delay(500);
    finger.LEDcontrol(false);
    delay(500);
  }
  IS_LOCKED = true;
  tft.fillScreen(TFT_BLACK);
  toneDown();
  digitalWrite(relayPin, LOW);  
  
}

// Fungsi untuk mengaktifkan mode Access Point WiFi
void connectToWiFiAP(){
  WiFi.mode(WIFI_AP); 
  WiFi.softAP(ssid, password);
  delay(100);
  Serial.println("Access Point mode activated");
  Serial.print("Access Point IP address: ");
  Serial.println(WiFi.softAPIP());
  IPAddress myIP = WiFi.softAPIP();
  Serial.print(myIP);
}

void setup() {
  // Persiapan awal
  pinMode(relayPin, OUTPUT); 
  digitalWrite(relayPin, LOW);
  
  // Inisialisasi komunikasi serial dan layar TFT
  Serial.begin(57600);
  finger.begin(57600);
  tft.init();
  tft.setRotation(0);
  
  // Tampilan awal dengan logo Fosti
  tft.fillScreen(TFT_WHITE);
  tft.pushImage(70, 110, 100, 100, logofosti);
  delay(3000);
  playTone(500, 100);

  // Koneksi ke WiFi AP dan setup server web
  connectToWiFiAP();
  setupEndpoints();
  server.begin();
  Serial.println("Server Start");

  // Verifikasi sensor sidik jari
  if (finger.verifyPassword()) {
    Serial.println("Fingerprint Sensor Connected");
    tft.fillScreen(TFT_GREEN);
    tft.setTextSize(2);
    tft.setTextColor(TFT_BLACK);
    tft.setCursor(85, 150);
    tft.println("Sensor");
    tft.setCursor(70, 180);
    tft.println("Connected");
    delay(3000);
    tft.fillScreen(TFT_BLACK);
  } else {
    // Tampilan jika sensor tidak terdeteksi
    tft.fillScreen(TFT_RED);
    tft.setTextSize(2);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(85, 150);
    tft.println("Sensor");
    tft.setCursor(70, 180);
    tft.println("Not Found");
    Serial.println("Unable to find Sensor");
    delay(3000);
    Serial.println("Check Connections");
  
    // Loop tak terbatas
    while (1) {
      delay(1);
    }
  }
}

// void jsontoOBJ() {
//   char json[] = "{'data':[{'finger':'54','nama':Husni Thamrin,'nim':'L200190002'},{'finger':'54','nama':Dimas,'nim':'L200190002'}]}";

//   DynamicJsonDocument doc(1024);
//   deserializeJson(doc, json);

//   for (int i = 0; i < doc["data"].size(); i++)
//   {
//     String nama = doc["data"][i]["nama"];
//     Serial.println(nama);
//   }
// }

void loop() {
  server.handleClient();    // Handle permintaan klien pada server
  Serial.flush();           // Membersihkan buffer serial
  scanFinger();
  finger.LEDcontrol(true);

  if (ENROLL_MODE) {
    getFingerprintEnroll(id_baru, nama_baru); // SCAN UNTUK ENROLL
  } else {
    scanFinger(); // SCAN UNTUK MEMBUKA
  }
  delay(30);  
}