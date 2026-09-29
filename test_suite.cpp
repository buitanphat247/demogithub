#include <iostream>
#include <vector>
#include <string>
#include "models/Models.h"
#include "dsa_core/LibraryService.h"

using namespace std;

// 1. Hàm tạo TaiLieu chuẩn
TaiLieu TaoTaiLieu(string ma, string ten, string tl, int soBan, int muon) {
    TaiLieu x;
    x.maTL = ma;
    x.tenTL = ten;
    x.theLoai = tl;
    x.soBanSao = soBan;
    x.luotMuon = muon;
    return x;
}

// 2. Hàm tạo DocGia chuẩn (Sửa doiTuong -> loaiDocGia)
DocGia TaoDocGia(string ma, string ten, string loai) {
    DocGia d;
    d.maDG = ma;
    d.hoTen = ten;
    d.loaiDocGia = loai; // Đã sửa chuẩn theo Models.h
    return d;
}

// 3. Hàm tạo PhieuMuon chuẩn
PhieuMuon TaoPhieuMuon(string maP, string maDG, string maTL, string ngayM, string hanT, string trangThai) {
    PhieuMuon p;
    p.maPhieu = maP;
    p.maDG = maDG;
    p.maTL = maTL;
    p.ngayMuon = ngayM;
    p.hanTra = hanT;
    p.trangThai = trangThai;
    return p;
}

int main(int argc, char* argv[]) {
    cout << "=======================================================\n";
    cout << "   KICK OFF: CHAY BO TEST TU DONG (MC1, MC2, RQ1, RQ2, RQ3)\n";
    cout << "=======================================================\n\n";

    // 1. DỮ LIỆU GIẢ LẬP
    vector<TaiLieu> dsSachTest;
    dsSachTest.push_back(TaoTaiLieu("B01", "Cau truc du lieu va Giai thuat", "CNTT", 8, 145));
    dsSachTest.push_back(TaoTaiLieu("B02", "Lap trinh C++ Nang cao", "CNTT", 3, 210));
    dsSachTest.push_back(TaoTaiLieu("B03", "Kinh te Vi mo ung dung", "KinhTe", 10, 48));
    dsSachTest.push_back(TaoTaiLieu("B04", "Dac Nhan Tam", "KyNang", 15, 320));
    dsSachTest.push_back(TaoTaiLieu("B12", "Nhap mon Hoc May va Deep Learning", "CNTT", 2, 175));
    dsSachTest.push_back(TaoTaiLieu("B99", "Co so du lieu", "CNTT", 5, 100));

    vector<DocGia> dsDocGiaTest;
    dsDocGiaTest.push_back(TaoDocGia("DG01", "Nguyen Van A", "SV"));
    dsDocGiaTest.push_back(TaoDocGia("DG02", "Tran Thi B", "GV"));

    vector<PhieuMuon> dsPhieuTest;
    dsPhieuTest.push_back(TaoPhieuMuon("PM01", "DG01", "B01", "2026-09-01", "2026-09-15", "DangMuon"));
    dsPhieuTest.push_back(TaoPhieuMuon("PM02", "DG02", "B04", "2026-09-20", "2026-10-05", "DangMuon"));

    LibraryService service;
    service.napDuLieu(dsSachTest, dsDocGiaTest, dsPhieuTest);

    int tongPass = 0;

    // TEST MC1: Tra cứu theo Mã tài liệu
    if (service.timTheoMa("B02") != nullptr) {
        cout << "  [PASS] MC1: Tim theo ma B02 thanh cong!\n";
        tongPass++;
    } else {
        cout << "  [FAIL] MC1: Tim theo ma B02 that bai!\n";
    }

    // TEST MC2: Top K lượt mượn
    if (service.layTopMuonNhieuNhat(2).size() == 2) {
        cout << "  [PASS] MC2: Top 2 luot muon thanh cong!\n";
        tongPass++;
    } else {
        cout << "  [FAIL] MC2: Top 2 luot muon that bai!\n";
    }

    // TEST RQ1: Tìm theo thể loại
    if (service.timTheoTheLoai("CNTT").size() == 4) {
        cout << "  [PASS] RQ1: Tim theo the loai CNTT thanh cong!\n";
        tongPass++;
    } else {
        cout << "  [FAIL] RQ1: Tim theo the loai CNTT that bai!\n";
    }

    // TEST RQ2: Kiểm tra quá hạn
    vector<PhieuMuon> dm, qh;
    service.kiemTraQuaHan("2026-09-20", dm, qh);
    if (qh.size() == 1) {
        cout << "  [PASS] RQ2: Kiem tra phieu qua han thanh cong!\n";
        tongPass++;
    } else {
        cout << "  [FAIL] RQ2: Kiem tra phieu qua han that bai!\n";
    }

    // TEST RQ3: Tìm theo từ khóa
    if (!service.timTheoTen("nhap mon").empty()) {
        cout << "  [PASS] RQ3: Tim tu khoa 'nhap mon' thanh cong!\n";
        tongPass++;
    } else {
        cout << "  [FAIL] RQ3: Tim tu khoa 'nhap mon' that bai!\n";
    }

    cout << "\n=======================================================\n";
    cout << "  KET QUA: PASSED " << tongPass << "/5 TEST CASES\n";
    cout << "=======================================================\n";

    return 0;
}