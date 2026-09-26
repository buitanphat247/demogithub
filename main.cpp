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

    // Bước 2: Nạp dữ liệu vào Service
    LibraryService service;
    service.napDuLieu(dsSach, dsDocGia, dsPhieu);

    cout << " [SAN SANG] Da nap " << dsSach.size() << " sach, "
         << dsDocGia.size() << " doc gia, "
         << dsPhieu.size() << " phieu muon vao Core.\n";
    cout << "=========================================================\n";

    // Bước 3: Chạy vòng lặp Menu tương tác
    chayMenu(service);

    return 0;
}
