# 📚 DigiLib: Sistem Perpustakaan Digital Berbasis Client-Server

Proyek akhir mata kuliah Algoritma dan Pemrograman (Semester 2).
Sistem manajemen perpustakaan yang mengimplementasikan 4 pilar OOP, struktur data dinamis (Linked List manual), algoritma pencarian & pengurutan manual, serta komunikasi jaringan menggunakan protokol TCP dan format data JSON.

## 👥 Anggota Kelompok 
1. Kemal Ananda Syafaat - 2506621075
2. Aristawidya Annisa Berliana - 2506621970
3. Khodijah Sofia - 2506534604

## 🛠️ Persyaratan Sistem
* Sistem Operasi: Linux, macOS, atau Windows (menggunakan WSL).
* Compiler: GCC/G++ dengan dukungan C++11.
* Library bawaan: `<sys/socket.h>`, `<thread>`, `<mutex>`.

## 🚀 Instruksi Kompilasi dan Cara Menjalankan

### 1. Kompilasi Server
Buka terminal, masuk ke folder `src`, lalu jalankan perintah berikut untuk mengompilasi server beserta dukungan multithreading:
```bash
cd src
g++ -std=c++11 server.cpp -o server -pthread
