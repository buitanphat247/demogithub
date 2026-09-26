#ifndef DOCGIA_H
#define DOCGIA_H

#include <string>

using namespace std;

// Cau truc thong tin doc gia (nguoi muon sach)
struct DocGia {
    string maDG;       // Ma doc gia (VD: DG01, DG02)
    string hoTen;      // Ho va ten
    string sdt;        // So dien thoai
    string loaiThe;    // Loai the ban doc (VIP, Normal, Student)
};

#endif // DOCGIA_H
