#ifndef MODELS_H
#define MODELS_H

#include <string>

using namespace std;

struct TaiLieu {
    string maTL;
    string tenTL;
    string tacGia;
    string theLoai;
    int namXB;
    int soLuong;
    int luotMuon;
};

struct DocGia {
    string maDG;
    string hoTen;
    string sdt;
    string loaiThe;
};

struct PhieuMuon {
    string maPhieu;
    string maDG;
    string maTL;
    string ngayMuon;
    string hanTra;
    string trangThai;
};

struct WaitlistEntry {
    string maTL;
    string maDG;
    int doUuTien;
    string thoiGian;
};

#endif
