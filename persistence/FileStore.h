#ifndef FILESTORE_H
#define FILESTORE_H

#include <vector>
#include <string>
#include "../models/Models.h"

using namespace std;

vector<TaiLieu> docDanhSachSach(const string& duongDan = "data/books.json");
vector<DocGia> docDanhSachDocGia(const string& duongDan = "data/readers.json");
vector<PhieuMuon> docDanhSachPhieu(const string& duongDan = "data/tickets.json");
vector<WaitlistEntry> docDanhSachHangCho(const string& duongDan = "data/waitlist.json");

#endif
