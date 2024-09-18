
Berikut adalah contoh README yang dapat Anda gunakan untuk dokumentasi proyek pemindaian file menggunakan ClamAV dan Boost. Anda dapat menyesuaikan informasi ini sesuai dengan kebutuhan Anda.

### README.md

# Aplikasi Pemindaian File dengan ClamAV

Aplikasi ini adalah server HTTP sederhana yang menggunakan ClamAV untuk memindai file yang diunggah melalui permintaan POST. Aplikasi dibangun menggunakan C++ dan pustaka Boost, dan dikemas dalam kontainer Docker berbasis Alpine.

## Fitur

- Memindai file menggunakan ClamAV.
- Mengembalikan hasil pemindaian dalam format teks.
- Menggunakan server HTTP untuk menerima file melalui permintaan POST.

## Prasyarat

Sebelum menggunakan aplikasi ini, pastikan Anda telah menginstal [Docker](https://www.docker.com/) di sistem Anda.

## Struktur Proyek

```
.
├── Dockerfile
└── avscan.cpp
```

## Instalasi dan Penggunaan

### 1. Menyimpan File Sumber

Simpan kode sumber C++ Anda di dalam file bernama `avscan.cpp` di direktori proyek yang sama dengan Dockerfile.

### 2. Membangun Image Docker

Buka terminal dan navigasikan ke direktori tempat Anda menyimpan Dockerfile. Jalankan perintah berikut untuk membangun image Docker:

```bash
docker build -t avscan .
```

### 3. Menjalankan Kontainer

Setelah image berhasil dibangun, jalankan kontainer dengan perintah berikut:

```bash
docker run --rm -p 8080:8080 avscan
```

- Port 8080 di kontainer akan dipetakan ke port 8080 di host Anda. Anda dapat mengakses server melalui `http://localhost:8080`.

### 4. Mengunggah File untuk Dipindai

Anda dapat menggunakan [Postman](https://www.postman.com/) untuk mengunggah file ke server. Berikut adalah langkah-langkahnya:

1. Buka Postman.
2. Pilih metode HTTP **POST**.
3. Masukkan URL: `http://localhost:8080`.
4. Di tab **Body**, pilih opsi **form-data**.
5. Tambahkan kunci dengan nama `file` dan pilih file yang ingin dipindai dari sistem Anda.
6. Klik tombol **Send**.

![contoh penggunaan pada POSTMAN](https://raw.githubusercontent.com/rahadiana/av-scan/refs/heads/main/Capture.PNG)

### 5. Memeriksa Hasil Pemindaian

Setelah mengunggah file, Anda akan menerima respons yang menunjukkan hasil pemindaian dari ClamAV. Hasil ini akan menampilkan apakah file tersebut terdeteksi sebagai malware atau tidak.

## Catatan

- Pastikan ClamAV terinstal dan diperbarui di dalam kontainer.
- Anda dapat menyesuaikan logika pemindaian dan respons sesuai kebutuhan proyek Anda.

## Lisensi

Proyek ini dilisensikan di bawah lisensi MIT. Lihat file LICENSE untuk detail lebih lanjut.

---

Dengan README ini, pengguna akan mendapatkan panduan lengkap tentang cara menggunakan aplikasi pemindaian file. Anda dapat menambahkan lebih banyak informasi atau detail jika diperlukan!
