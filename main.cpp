#include <iostream>
#include <vector>
#include "persistence/FileStore.h"
#include "dsa_core/LibraryService.h"
#include "presentation/Menu.h"

using namespace std;

// Chuong trinh chinh - Quan ly thu vien
int main() {
    // Xóa sạch màn hình khi khởi động
    xoaManHinh();

    cout << "=========================================================\n";
    cout << " [KHOI DONG] Nap du lieu tu tang Persistence (data/*.json)...\n";

    // Bước 1: Đọc dữ liệu từ các tệp JSON
    vector<TaiLieu> dsSach    = docDanhSachSach();
    vector<DocGia> dsDocGia   = docDanhSachDocGia();
    vector<PhieuMuon> dsPhieu = docDanhSachPhieu();
    vector<WaitlistEntry> dsChoMuon = docDanhSachHangCho();

    // Bước 2: Nạp dữ liệu vào Service
    LibraryService service;
    service.napDuLieu(dsSach, dsDocGia, dsPhieu, dsChoMuon);

    cout << " [SAN SANG] Da nap " << dsSach.size() << " sach, "
         << dsDocGia.size() << " doc gia, "
         << dsPhieu.size() << " phieu muon, "
         << dsChoMuon.size() << " hang cho vao Core.\n";
    cout << "=========================================================\n";

    // Bước 3: Chạy vòng lặp Menu tương tác
    chayMenu(service);

    // Bước 4: Lưu dữ liệu vào các tệp JSON
    cout << " [LUU DU LIEU] Luu du lieu vao tang Persistence (data/*.json)...\n";
    cout << "=========================================================\n";

    bool g1 = ghiDanhSachSach(dsSach);
    bool g2 = ghiDanhSachDocGia(dsDocGia);
    bool g3 = ghiDanhSachPhieu(dsPhieu);
    bool g4 = ghiDanhSachHangCho(dsChoMuon);

    if (g1 && g2 && g3 && g4) {
        cout << " [THANH CONG] Da luu toan bo du lieu an toan!\n";
    } else {
        cout << " [THAT BAI] Co loi xay ra trong qua trinh ghi file!\n";
    }
    cout << "=========================================================\n";

    return 0;
}
