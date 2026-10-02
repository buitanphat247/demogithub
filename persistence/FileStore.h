#ifndef FILESTORE_H
#define FILESTORE_H

#include <vector>
#include <string>
#include "../models/Models.h"

using namespace std;

// Doc (ghi) danh sach sach tu (vao) file JSON
vector<TaiLieu> docDanhSachSach(const string& duongDan = "data/books.json");
bool ghiDanhSachSach(const vector<TaiLieu>& ds, const string& duongDan = "data/books.json");

// Doc (ghi) danh sach doc gia tu (vao) file JSON
vector<DocGia> docDanhSachDocGia(const string& duongDan = "data/readers.json");
bool ghiDanhSachDocGia(const vector<DocGia>& ds, const string& duongDan = "data/readers.json");

// Doc (ghi) danh sach phieu muon tu (vao) file JSON
vector<PhieuMuon> docDanhSachPhieu(const string& duongDan = "data/tickets.json");
bool ghiDanhSachPhieu(const vector<PhieuMuon>& ds, const string& duongDan = "data/tickets.json");

// Doc (ghi) danh sach hang cho tu (vao) file JSON
vector<WaitlistEntry> docDanhSachHangCho(const string& duongDan = "data/waitlist.json");
bool ghiDanhSachHangCho(const vector<WaitlistEntry>& ds, const string& duongDan = "data/waitlist.json");

#endif // FILESTORE_H
