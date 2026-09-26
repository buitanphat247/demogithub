#ifndef FILESTORE_H
#define FILESTORE_H

#include <vector>
#include <string>
#include "../models/Models.h"

using namespace std;

// Doc danh sach sach tu file JSON
vector<TaiLieu> docDanhSachSach(const string& duongDan = "data/books.json");

// Doc danh sach doc gia tu file JSON
vector<DocGia> docDanhSachDocGia(const string& duongDan = "data/readers.json");

// Doc danh sach phieu muon tu file JSON
vector<PhieuMuon> docDanhSachPhieu(const string& duongDan = "data/tickets.json");

// Doc danh sach hang cho tu file JSON
vector<WaitlistEntry> docDanhSachHangCho(const string& duongDan = "data/waitlist.json");

#endif // FILESTORE_H
