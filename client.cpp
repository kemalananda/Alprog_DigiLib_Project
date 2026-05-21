#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
 
void kirimPermintaan(std::string jsonRequest) {
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  sockaddr_in serv_addr;
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(8080);
  inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
 
  if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
      std::cout << "Gagal terhubung ke server perpustakaan.\n";
      return;
  }
 
  send(sock, jsonRequest.c_str(), jsonRequest.length(), 0);
 
  char buffer[4096] = {0};
  read(sock, buffer, 4096);
 
  std::cout << "\n[Respon JSON dari Server]:\n" << buffer << "\n\n";
  close(sock);
}
 
int main() {
  int pilihan;
  std::string keyword;
 
  while (true) {
      std::cout << "=== APLIKASI PERPUSTAKAAN DIGITAL (DIGILIB) ===\n";
      std::cout << "1. Tampilkan Semua Buku (Terurut A-Z)\n";
      std::cout << "2. Cari Buku berdasarkan Judul\n";
      std::cout << "3. Keluar\n";
      std::cout << "Pilih menu (1-3): ";
      std::cin >> pilihan;
      std::cin.ignore();
 
      if (pilihan == 1) {
          std::string req = "{\"action\":\"list\"}";
          kirimPermintaan(req);
      }
      else if (pilihan == 2) {
          std::cout << "Masukkan kata kunci judul: ";
          std::getline(std::cin, keyword);
          std::string req = "{\"action\":\"search\",\"keyword\":\"" + keyword + "\"}";
          kirimPermintaan(req);
      }
      else if (pilihan == 3) {
          std::cout << "Keluar dari aplikasi.\n";
          break;
      }
      else {
          std::cout << "Pilihan tidak valid!\n";
      }
  }
  return 0;
}