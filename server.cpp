#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
 
std::mutex mtx; // Untuk sinkronisasi thread saat mengakses data
 
// ==========================================
// PILAR 1 & 2 OOP: ABSTRAKSI & ENKAPSULASI
// ==========================================
class ItemKatalog {
protected:
  int id;
  std::string judul;
public:
  ItemKatalog(int id, std::string judul) : id(id), judul(judul) {}
  virtual ~ItemKatalog() {}
 
  // Getter & Setter (Enkapsulasi)
  int getId() const { return id; }
  std::string getJudul() const { return judul; }
 
  // Pure Virtual Function (Abstraksi)
  virtual std::string toJSON() = 0;
};
 
// ==========================================
// PILAR 3 OOP: PEWARISAN (INHERITANCE)
// ==========================================
class Buku : public ItemKatalog {
private:
  std::string penulis;
public:
  Buku(int id, std::string judul, std::string penulis)
      : ItemKatalog(id, judul), penulis(penulis) {}
 
  std::string getPenulis() const { return penulis; }
 
  // PILAR 4 OOP: POLIMORFISME (Method Overriding)
  std::string toJSON() override {
      return "{\"id\":" + std::to_string(id) +
             ",\"judul\":\"" + judul +
             "\",\"penulis\":\"" + penulis + "\"}";
  }
};
 
// ==========================================
// LINKED LIST MANUAL 
// ==========================================
struct Node {
  Buku* data;
  Node* next;
  Node(Buku* b) : data(b), next(nullptr) {}
};
 
class BukuLinkedList {
private:
  Node* head;
public:
  BukuLinkedList() : head(nullptr) {}
 
  void insert(Buku* b) {
      Node* newNode = new Node(b);
      if (!head) {
          head = newNode;
      } else {
          Node* temp = head;
          while (temp->next) temp = temp->next;
          temp->next = newNode;
      }
  }
 
  void sortByJudul() {
      if (!head) return;
      bool swapped;
      Node* ptr1;
      Node* lptr = nullptr;
 
      do {
          swapped = false;
          ptr1 = head;
 
          while (ptr1->next != lptr) {
              if (ptr1->data->getJudul() > ptr1->next->data->getJudul()) {
                  Buku* temp = ptr1->data;
                  ptr1->data = ptr1->next->data;
                  ptr1->next->data = temp;
                  swapped = true;
              }
              ptr1 = ptr1->next;
          }
          lptr = ptr1;
      } while (swapped);
  }
 
  Buku* searchLinear(std::string keyword) {
      Node* temp = head;
      while (temp != nullptr) {
          if (temp->data->getJudul().find(keyword) != std::string::npos) {
              return temp->data;
          }
          temp = temp->next;
      }
      return nullptr;
  }
 
  std::string getAllAsJSON() {
      std::string json = "[";
      Node* temp = head;
      while (temp) {
          json += temp->data->toJSON();
          if (temp->next) json += ",";
          temp = temp->next;
      }
      json += "]";
      return json;
  }
};
 
BukuLinkedList perpustakaan; 
 
void handleClient(int clientSocket) {
  char buffer[1024] = {0};
  read(clientSocket, buffer, 1024);
  std::string request(buffer);
  std::string response;
 
  std::lock_guard<std::mutex> lock(mtx); 
 
  if (request.find("\"action\":\"list\"") != std::string::npos) {
      perpustakaan.sortByJudul(); 
      response = "{\"status\":\"success\",\"data\":" + perpustakaan.getAllAsJSON() + "}";
  }
  else if (request.find("\"action\":\"search\"") != std::string::npos) {
      size_t keyPos = request.find("\"keyword\":\"");
      if (keyPos != std::string::npos) {
          size_t start = keyPos + 11;
          size_t end = request.find("\"", start);
          std::string keyword = request.substr(start, end - start);
 
          Buku* hasil = perpustakaan.searchLinear(keyword);
          if (hasil) {
              response = "{\"status\":\"found\",\"data\":" + hasil->toJSON() + "}";
          } else {
              response = "{\"status\":\"not_found\",\"message\":\"Buku tidak ditemukan!\"}";
          }
      }
  } else {
      response = "{\"status\":\"error\",\"message\":\"Aksi tidak dikenali.\"}";
  }
 
  send(clientSocket, response.c_str(), response.length(), 0);
  close(clientSocket);
}
 
int main() {
  perpustakaan.insert(new Buku(101, "Cara Coding Ala Hacker Solo", "Ribran Gabakuming"));
  perpustakaan.insert(new Buku(103, "Berenang Di Lautan Susu", "Stefani Skuy"));
  perpustakaan.insert(new Buku(102, "Cerita Horror Kuda Laut", "Widodo"));
 
  int serverFd = socket(AF_INET, SOCK_STREAM, 0);
  sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(8080);
 
  bind(serverFd, (struct sockaddr*)&address, sizeof(address));
  listen(serverFd, 3);
 
  std::cout << "[SERVER] Berjalan di port 8080. Menunggu koneksi...\n";
 
  while (true) {
      int addrlen = sizeof(address);
      int clientSocket = accept(serverFd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
      std::cout << "[SERVER] Klien terhubung!\n";
     
      std::thread(handleClient, clientSocket).detach();
  }
 
  close(serverFd);
  return 0;
}
