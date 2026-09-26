#ifndef PHIEUMUON_H
#define PHIEUMUON_H

#include <string>

using namespace std;

// Cau truc thong tin phieu muon sach
struct PhieuMuon {
    string maPhieu;    // Ma phieu muon (VD: PM01, PM02)
    string maDG;       // Ma doc gia muon sach
    string maTL;       // Ma tai lieu duoc muon
    string ngayMuon;   // Ngay muon (YYYY-MM-DD)
    string hanTra;     // Han chot tra sach (YYYY-MM-DD)
    string trangThai;  // Trang thai ("DangMuon", "DaTra")
};

#endif // PHIEUMUON_H
