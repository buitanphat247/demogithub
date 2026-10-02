#include "Menu.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>

using namespace std;

void inKeNgang(int doDai) {
    cout << "+" << string(doDai - 2, '-') << "+\n";
}

void inTieuDeMenu() {
    cout << "\n";
    inKeNgang(62);
    cout << "|        HE THONG QUAN LY THU VIEN (LIBRARY ENGINE)          |\n";
    inKeNgang(62);
    cout << "|  1. [MC1] Tra cuu tai lieu theo Ma                         |\n";
    cout << "|  2. [MC2] Xem Top K tai lieu muon nhieu nhat               |\n";
    cout << "|  3. [RQ1] Tra cuu tai lieu theo The loai                   |\n";
    cout << "|  4. [RQ2] Kiem tra tai lieu qua han theo ngay              |\n";
    cout << "|  5. [RQ3] Tim kiem tai lieu theo Ten / Tu khoa             |\n";
    cout << "|  6. [Xem] Hien thi toan bo kho sach trong thu vien         |\n";
    cout << "|  0. Thoat chuong trinh & Luu tru                           |\n";
    inKeNgang(62);
}

void inBangSach(const vector<TaiLieu>& ds) {
    if (ds.empty()) {
        cout << "\n  [!] Khong tim thay tai lieu nao phu hop.\n";
        return;
    }
    inKeNgang(85);
    cout << "| " << left << setw(8) << "Ma TL"
         << "| " << setw(35) << "Ten Tai Lieu"
         << "| " << setw(12) << "The Loai"
         << "| " << setw(10) << "Ton Kho"
         << "| " << setw(10) << "Luot Muon" << " |\n";
    inKeNgang(85);
    for (size_t i = 0; i < ds.size(); i++) {
        string tenHienThi = ds[i].tenTL;
        if (tenHienThi.length() > 33) {
            tenHienThi = tenHienThi.substr(0, 30) + "...";
        }
        cout << "| " << left << setw(8) << ds[i].maTL
             << "| " << setw(35) << tenHienThi
             << "| " << setw(12) << ds[i].theLoai
             << "| " << setw(10) << ds[i].soLuong
             << "| " << setw(10) << ds[i].luotMuon << " |\n";
    }
    inKeNgang(85);
}

void inBangPhieu(const vector<PhieuMuon>& ds, const string& tieuDe) {
    cout << "\n>> " << tieuDe << " (So luong: " << ds.size() << "):\n";
    if (ds.empty()) {
        cout << "   (Khong co phieu muon nao)\n";
        return;
    }
    inKeNgang(72);
    cout << "| " << left << setw(10) << "Ma Phieu"
         << "| " << setw(12) << "Ma Doc Gia"
         << "| " << setw(12) << "Ma Sach"
         << "| " << setw(14) << "Han Tra"
         << "| " << setw(14) << "Trang Thai" << " |\n";
    inKeNgang(72);
    for (size_t i = 0; i < ds.size(); i++) {
        cout << "| " << left << setw(10) << ds[i].maPhieu
             << "| " << setw(12) << ds[i].maDG
             << "| " << setw(12) << ds[i].maTL
             << "| " << setw(14) << ds[i].hanTra
             << "| " << setw(14) << ds[i].trangThai << " |\n";
    }
    inKeNgang(72);
}

void dungManHinh() {
    cout << "\n(Nhan Enter de tiep tuc...)";
    cin.ignore();
    cin.get();
}

void chayMenu(LibraryService& service) {
    int luaChon = -1;
    while (luaChon != 0) {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
        inTieuDeMenu();
        cout << ">> Nhap lua chon cua ban (0-6): ";
        if (!(cin >> luaChon)) {
            cout << "\n[!] Nhap sai dinh dang so, vui long thu lai!\n";
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }

        switch (luaChon) {
            case 1: {
                string ma;
                cout << "\n--- [MC1] TRA CUU TAI LIEU THEO MA ---\n";
                cout << "Nhap ma tai lieu (vd: B01, B04): ";
                cin >> ma;
                TaiLieu* s = service.timTheoMa(ma);
                if (s != nullptr) {
                    cout << "\n[+] Tim thay thong tin tai lieu:\n";
                    inBangSach({*s});
                } else {
                    cout << "\n[-] Khong tim thay tai lieu mang ma: " << ma << "\n";
                }
                break;
            }
            case 2: {
                int k;
                cout << "\n--- [MC2] TOP K TAI LIEU MUON NHIEU NHAT ---\n";
                cout << "Nhap so luong Top K can xem (vd: 3, 5): ";
                cin >> k;
                if (k <= 0) {
                    cout << "\n[!] So luong K phai lon hon 0!\n";
                    break;
                }
                vector<TaiLieu> topList = service.layTopMuonNhieuNhat(k);
                cout << "\n[+] Danh sach Top " << k << " tai lieu co luot muon cao nhat:\n";
                inBangSach(topList);
                break;
            }
            case 3: {
                string theLoai;
                cout << "\n--- [RQ1] TRA CUU THEO THE LOAI ---\n";
                cout << "Nhap the loai can tra (vd: CNTT, KinhTe, VanHoc, KyNang): ";
                cin >> theLoai;
                vector<TaiLieu> ds = service.timTheoTheLoai(theLoai);
                cout << "\n[+] Danh sach tai lieu thuoc the loai [" << theLoai << "]:\n";
                inBangSach(ds);
                break;
            }
            case 4: {
                string ngay;
                cout << "\n--- [RQ2] THEO DOI TAI LIEU DANG MUON / QUA HAN ---\n";
                cout << "Nhap ngay kiem tra (YYYY-MM-DD, vd: 2026-09-20): ";
                cin >> ngay;
                vector<PhieuMuon> dangMuon, quaHan;
                service.kiemTraQuaHan(ngay, dangMuon, quaHan);
                inBangPhieu(dangMuon, "DANH SACH PHIEU DANG TRONG HAN");
                inBangPhieu(quaHan, "CANH BAO: DANH SACH PHIEU QUA HAN");
                break;
            }
            case 5: {
    cout << "\n--- [RQ3] TIM KIEM TAI LIEU THEO TEN / TU KHOA ---\n";
    cout << "Nhap tu khoa ten sach: ";
    
    string tuKhoa;
    // Bắt buộc dùng cin.ignore() nếu trước đó có nhập số để xóa bộ đệm '\n'
    cin.ignore(1000, '\n'); 
    getline(cin, tuKhoa);

    // Gọi hàm tìm kiếm từ service
    vector<TaiLieu> ketQua = service.timTheoTen(tuKhoa);

    cout << "\n[+] Ket qua tim kiem theo tu khoa [" << tuKhoa << "]:\n\n";

    if (ketQua.empty()) {
        cout << "  [!] Khong tim thay tai lieu nao phu hop.\n";
    } else {
        // In danh sách tìm thấy ra màn hình
        for (const auto& sach : ketQua) {
            cout << "  - [" << sach.maTL << "] " << sach.tenTL 
                 << " | The loai: " << sach.theLoai 
                 << " | Luot muon: " << sach.luotMuon << "\n";
        }
    }
    
    cout << "\n(Nhan Enter de tiep tuc...)";
    cin.get();
    break;
}
            case 6: {
                cout << "\n--- TOAN BO DANH MUC TAI LIEU TRONG KHO ---\n";
                inBangSach(service.layTatCaSach());
                break;
            }
            case 0: {
                cout << "\n>> Da dong chuong trinh. Tam biet!\n";
                break;
            }
            default: {
                cout << "\n[!] Lua chon khong hop le (0-6), vui long chon lai!\n";
                break;
            }
        }

        if (luaChon != 0) {
            dungManHinh();
        }
    }
}
