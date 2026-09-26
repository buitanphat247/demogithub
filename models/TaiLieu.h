#ifndef TAILIEU_H
#define TAILIEU_H

#include <string>

using namespace std;

// Cau truc thong tin sach / tai lieu trong thu vien
struct TaiLieu {
    string maTL;       // Ma tai lieu (VD: B01, B02)
    string tenTL;      // Ten sach
    string tacGia;     // Tac gia
    string theLoai;    // The loai (CNTT, KinhTe, VanHoc, KyNang...)
    int namXB;         // Nam xuat ban
    int soLuong;       // So luong con trong kho
    int luotMuon;      // Tong luot da muon
};

#endif // TAILIEU_H
