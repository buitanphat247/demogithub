#include "Menu.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

using namespace std;

// Ma phim dieu huong
enum MaPhim {
    PHIM_LEN   = 1001,
    PHIM_XUONG = 1002,
    PHIM_TRAI  = 1003,
    PHIM_PHAI  = 1004,
    PHIM_ENTER = 13,
    PHIM_ESC   = 27
};

// Danh sach menu chinh
static const vector<string> danhSachMenu = {
    "[MC1] Tra cuu tai lieu theo Ma",
    "[MC2] Xem Top K tai lieu muon nhieu nhat",
    "[RQ1] Tra cuu tai lieu theo The loai",
    "[RQ2] Kiem tra tai lieu qua han theo ngay",
    "[RQ3] Tim kiem tai lieu theo Ten / Tu khoa",
    "[Xem] Hien thi toan bo kho sach trong thu vien",
    "Thoat chuong trinh & Luu tru"
};

// Doc phim bam tu ban phim (khong can Enter)
int docPhim() {
#ifdef _WIN32
    int c = _getch();
    if (c == 0 || c == 224) {
        c = _getch();
        if (c == 72) return PHIM_LEN;
        if (c == 80) return PHIM_XUONG;
        if (c == 75) return PHIM_TRAI;
        if (c == 77) return PHIM_PHAI;
    }
    return c;
#else
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    int c = getchar();
    if (c == 27) {
        int c2 = getchar();
        if (c2 == '[') {
            int c3 = getchar();
            if (c3 == 'A') c = PHIM_LEN;
            else if (c3 == 'B') c = PHIM_XUONG;
            else if (c3 == 'D') c = PHIM_TRAI;
            else if (c3 == 'C') c = PHIM_PHAI;
        }
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return c;
#endif
}

// Xoa toan bo man hinh terminal
void xoaManHinh() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    cout << "\033[H\033[2J\033[3J" << flush;
}

// In duong ke ngang trang tri
void inKeNgang(int doDai) {
    cout << "+" << string(doDai - 2, '-') << "+\n";
}

// In dong noi dung nam trong khung 74 ky tu (canh le chuan)
static void inDongKhung(const string& noiDung, const string& ansiMau = "") {
    int doRong = 70;
    string text = noiDung;
    if ((int)text.length() > doRong) {
        text = text.substr(0, doRong);
    }
    int padPhai = doRong - (int)text.length();
    if (!ansiMau.empty()) {
        cout << "| " << ansiMau << text << string(padPhai, ' ') << "\033[0m |\n";
    } else {
        cout << "| " << text << string(padPhai, ' ') << " |\n";
    }
}

// In dong tieu de can giua trong khung 74 ky tu
static void inDongTieuDeKhung(const string& tieuDe) {
    int doRong = 72;
    string text = tieuDe;
    if ((int)text.length() > doRong) {
        text = text.substr(0, doRong);
    }
    int padTrai = (doRong - (int)text.length()) / 2;
    int padPhai = doRong - (int)text.length() - padTrai;
    cout << "|" << string(padTrai, ' ') << text << string(padPhai, ' ') << "|\n";
}

// In menu chinh
void inTieuDeMenu(int viTriHienTai) {
    cout << "\n";
    inKeNgang(74);
    inDongTieuDeKhung("HE THONG QUAN LY THU VIEN (LIBRARY ENGINE)");
    inKeNgang(74);
    inDongKhung("(Dung mui ten LEN / XUONG de chon, ENTER thuc hien, ESC thoat)");
    inKeNgang(74);

    for (int i = 0; i < (int)danhSachMenu.size(); i++) {
        string soThuTu = (i == (int)danhSachMenu.size() - 1) ? "0" : to_string(i + 1);
        string noiDung = "[" + soThuTu + "] " + danhSachMenu[i];

        if (i == viTriHienTai) {
            inDongKhung(">> " + noiDung, "\033[1;36m");
        } else {
            inDongKhung("   " + noiDung);
        }
    }
    inKeNgang(74);
}

// In bang danh sach sach (kho 87 ky tu)
void inBangSach(const vector<TaiLieu>& ds) {
    if (ds.empty()) {
        cout << "\n  [!] Khong tim thay tai lieu nao phu hop.\n";
        return;
    }
    inKeNgang(87);
    cout << "| " << left << setw(8) << "Ma TL"
         << "| " << setw(35) << "Ten Tai Lieu"
         << "| " << setw(12) << "The Loai"
         << "| " << setw(10) << "Ton Kho"
         << "| " << setw(10) << "Luot Muon" << " |\n";
    inKeNgang(87);
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
    inKeNgang(87);
}

// Doc chuoi ky tu co ho tro ESC de quay lai
bool docChuoiHoacESC(string& ketQua) {
    ketQua = "";
    while (true) {
        int c = docPhim();
        if (c == PHIM_ESC) {
            return false;
        }
        if (c == PHIM_ENTER || c == '\n') {
            cout << "\n";
            return true;
        }
        if (c == 8 || c == 127) { // Phim Backspace
            if (!ketQua.empty()) {
                ketQua.pop_back();
                cout << "\b \b" << flush;
            }
        } else if (c >= 32 && c <= 126) {
            ketQua.push_back((char)c);
            cout << (char)c << flush;
        }
    }
}

// Lay ngay hien tai tu dong ho he thong YYYY-MM-DD
static string layNgayHienTai() {
    time_t t = time(nullptr);
    tm* now = localtime(&t);
    char buf[32];
    snprintf(buf, sizeof(buf), "%04d-%02d-%02d", now->tm_year + 1900, now->tm_mon + 1, now->tm_mday);
    return string(buf);
}

// [MC1] Tra cuu tai lieu tuong tac bang phim mui ten
void traCuuTaiLieuTuongTac(const vector<TaiLieu>& dsGoc) {
    if (dsGoc.empty()) {
        cout << "\n  [!] Khong co tai lieu nao trong he thong.\n";
        dungManHinh();
        return;
    }

    vector<TaiLieu> ds = dsGoc;
    sort(ds.begin(), ds.end(), [](const TaiLieu& a, const TaiLieu& b) {
        return a.maTL < b.maTL;
    });

    int viTri = 0;
    const int soLuong = (int)ds.size();

    while (true) {
        xoaManHinh();
        cout << "\n";
        inKeNgang(92);
        cout << "|                     [MC1] TRA CUU TAI LIEU - DIEU HUONG CHON                       |\n";
        inKeNgang(92);
        cout << "|  (Dung mui ten LEN / XUONG de chon tai lieu, ENTER de xem chi tiet, ESC de quay lai)  |\n";
        inKeNgang(92);

        cout << "|   | " << left << setw(8) << "Ma TL"
             << "| " << setw(35) << "Ten Tai Lieu"
             << "| " << setw(12) << "The Loai"
             << "| " << setw(10) << "Ton Kho"
             << "| " << setw(10) << "Luot Muon" << " |\n";
        inKeNgang(92);

        for (int i = 0; i < soLuong; i++) {
            string tenHienThi = ds[i].tenTL;
            if (tenHienThi.length() > 33) {
                tenHienThi = tenHienThi.substr(0, 30) + "...";
            }

            if (i == viTri) {
                cout << "|\033[1;36m>> | " << left << setw(8) << ds[i].maTL
                     << "| " << setw(35) << tenHienThi
                     << "| " << setw(12) << ds[i].theLoai
                     << "| " << setw(10) << ds[i].soLuong
                     << "| " << setw(10) << ds[i].luotMuon << " \033[0m|\n";
            } else {
                cout << "|   | " << left << setw(8) << ds[i].maTL
                     << "| " << setw(35) << tenHienThi
                     << "| " << setw(12) << ds[i].theLoai
                     << "| " << setw(10) << ds[i].soLuong
                     << "| " << setw(10) << ds[i].luotMuon << " |\n";
            }
        }
        inKeNgang(92);

        int phim = docPhim();

        if (phim == PHIM_LEN || phim == 'w' || phim == 'W') {
            viTri = (viTri - 1 + soLuong) % soLuong;
        } else if (phim == PHIM_XUONG || phim == 's' || phim == 'S') {
            viTri = (viTri + 1) % soLuong;
        } else if (phim == PHIM_ESC || phim == 'q' || phim == 'Q') {
            break;
        } else if (phim == PHIM_ENTER || phim == '\n') {
            // Xem chi tiet tai lieu
            xoaManHinh();
            cout << "\n";
            inKeNgang(74);
            inDongTieuDeKhung("THONG TIN CHI TIET TAI LIEU DA CHON");
            inKeNgang(74);
            inDongKhung("  Ma tai lieu : " + ds[viTri].maTL);
            inDongKhung("  Ten tai lieu: " + ds[viTri].tenTL);
            inDongKhung("  Tac gia     : " + ds[viTri].tacGia);
            inDongKhung("  The loai    : " + ds[viTri].theLoai);
            inDongKhung("  Nam xuat ban: " + to_string(ds[viTri].namXB));
            inDongKhung("  Ton kho     : " + to_string(ds[viTri].soLuong) + " ban");
            inDongKhung("  Luot muon   : " + to_string(ds[viTri].luotMuon) + " luot");
            inKeNgang(74);
            dungManHinh();
        }
    }
}

// In bang danh sach phieu muon (kho 74 ky tu)
void inBangPhieu(const vector<PhieuMuon>& ds, const string& tieuDe) {
    cout << "\n>> " << tieuDe << " (So luong: " << ds.size() << "):\n";
    if (ds.empty()) {
        cout << "   (Khong co phieu muon nao)\n";
        return;
    }
    inKeNgang(74);
    cout << "| " << left << setw(10) << "Ma Phieu"
         << "| " << setw(12) << "Ma Doc Gia"
         << "| " << setw(12) << "Ma Sach"
         << "| " << setw(15) << "Han Tra"
         << "| " << setw(15) << "Trang Thai" << " |\n";
    inKeNgang(74);
    for (size_t i = 0; i < ds.size(); i++) {
        cout << "| " << left << setw(10) << ds[i].maPhieu
             << "| " << setw(12) << ds[i].maDG
             << "| " << setw(12) << ds[i].maTL
             << "| " << setw(15) << ds[i].hanTra
             << "| " << setw(15) << ds[i].trangThai << " |\n";
    }
    inKeNgang(74);
}

// Tam dung man hinh
void dungManHinh() {
    cout << "\n(Nhan phim bat ky de tiep tuc...)";
    docPhim();
}

// Vong lap dieu khien menu chinh
void chayMenu(LibraryService& service) {
    int viTri = 0;
    const int soMuc = (int)danhSachMenu.size();

    while (true) {
        xoaManHinh();
        inTieuDeMenu(viTri);

        int phim = docPhim();

        // Dieu huong mui ten
        if (phim == PHIM_LEN || phim == 'w' || phim == 'W') {
            viTri = (viTri - 1 + soMuc) % soMuc;
        } else if (phim == PHIM_XUONG || phim == 's' || phim == 'S') {
            viTri = (viTri + 1) % soMuc;
        } else if (phim == PHIM_ESC || phim == '0') {
            xoaManHinh();
            cout << "\n>> Dang dong chuong trinh. Tam biet!\n";
            break;
        } else if (phim >= '1' && phim <= '6') {
            viTri = phim - '1';
            phim = PHIM_ENTER;
        }

        // Xu ly lua chon khi nhan ENTER
        if (phim == PHIM_ENTER || phim == '\n') {
            int luaChon = (viTri == soMuc - 1) ? 0 : (viTri + 1);

            if (luaChon == 0) {
                xoaManHinh();
                cout << "\n>> Dang dong chuong trinh. Tam biet!\n";
                break;
            }

            xoaManHinh();

            switch (luaChon) {
                case 1: { // [MC1] Tra cuu tai lieu tuong tac
                    traCuuTaiLieuTuongTac(service.layTatCaSach());
                    break;
                }
                case 2: { // [MC2] Xem Top K tai lieu muon nhieu nhat
                    int tongSoTL = (int)service.layTatCaSach().size();

                    cout << "\n";
                    inKeNgang(74);
                    inDongTieuDeKhung("[MC2] TOP K TAI LIEU MUON NHIEU NHAT");
                    inKeNgang(74);
                    inDongKhung("  - He thong tu dong loc va trich xuat Top K sach pho bien nhat.");
                    inDongKhung("  - Tong so tai lieu hien co trong kho : " + to_string(tongSoTL) + " tai lieu");
                    inDongKhung("  - Pham vi so luong K hop le          : Tu 1 den " + to_string(tongSoTL));
                    inKeNgang(74);

                    cout << "\n>> Nhap so luong Top K can xem (1 - " << tongSoTL << ") [nhan ESC de quay lai]: ";

                    string inputStr;
                    if (!docChuoiHoacESC(inputStr) || inputStr.empty()) {
                        break;
                    }

                    bool laSo = true;
                    for (char ch : inputStr) {
                        if (!isdigit(ch)) {
                            laSo = false;
                            break;
                        }
                    }

                    if (!laSo) {
                        cout << "  [!] Loi: Vui long nhap mot so nguyen hop le!\n";
                        dungManHinh();
                        break;
                    }

                    int k = stoi(inputStr);
                    if (k <= 0 || k > tongSoTL) {
                        cout << "  [!] So luong K khong hop le! K phai lon hon 0 va nho hon hoac bang " << tongSoTL << ".\n";
                        dungManHinh();
                        break;
                    }

                    vector<TaiLieu> topList = service.layTopMuonNhieuNhat(k);
                    xoaManHinh();
                    cout << "\n";
                    inKeNgang(87);
                    string tieuDeKQ = "KET QUA: DANH SACH TOP " + to_string(k) + " TAI LIEU MUON NHIEU NHAT";
                    int padTraiKQ = (85 - (int)tieuDeKQ.length()) / 2;
                    int padPhaiKQ = 85 - (int)tieuDeKQ.length() - padTraiKQ;
                    cout << "|" << string(padTraiKQ, ' ') << tieuDeKQ << string(padPhaiKQ, ' ') << "|\n";
                    inBangSach(topList);
                    dungManHinh();
                    break;
                }
                case 3: { // [RQ1] Tra cuu tai lieu theo the loai
                    while (true) {
                        xoaManHinh();
                        cout << "--- [RQ1] TRA CUU TAI LIEU THEO THE LOAI ---\n";
                        cout << "Nhap the loai can tra cuu (vd: CNTT, cntt, KinhTe...) [ESC de ve Menu chinh]: ";

                        string inputTheLoai;
                        if (!docChuoiHoacESC(inputTheLoai) || inputTheLoai.empty()) {
                            break;
                        }

                        vector<TaiLieu> ds = service.timTheoTheLoai(inputTheLoai);
                        xoaManHinh();

                        if (ds.empty()) {
                            cout << "\n[!] Khong tim thay tai lieu nao thuoc the loai [" << inputTheLoai << "].\n";
                        } else {
                            cout << "\n";
                            inKeNgang(87);
                            string tieuDeKQ = "DANH SACH TAI LIEU THUOC THE LOAI [" + inputTheLoai + "]";
                            int padTraiKQ = (85 - (int)tieuDeKQ.length()) / 2;
                            int padPhaiKQ = 85 - (int)tieuDeKQ.length() - padTraiKQ;
                            cout << "|" << string(padTraiKQ, ' ') << tieuDeKQ << string(padPhaiKQ, ' ') << "|\n";
                            inBangSach(ds);
                        }
                        cout << "\n>> Nhan ESC hoac phim bat ky de quay lai tim kiem...";
                        docPhim();
                    }
                    break;
                }
                case 4: { // [RQ2] Kiem tra phieu muon theo ngay (in-place picker)
                    int buoc = 1; // 1: Chon Nam, 2: Chon Thang, 3: Chon Ngay
                    int namChon = 2026;
                    int thangChon = 9;
                    int ngayChon = 20;

                    int namBatDau = 1980;
                    int namKetThuc = 2026;

                    while (true) {
                        xoaManHinh();
                        cout << "\n";
                        inKeNgang(74);
                        inDongTieuDeKhung("[RQ2] THIET LAP NGAY KIEM TRA PHIEU MUON");
                        inKeNgang(74);
                        inDongKhung("(Dung mui ten LEN/XUONG hoac TRAI/PHAI de thay doi, ENTER xac nhan)");
                        inDongKhung("(Nhan ESC de quay lai buoc truoc hoac thoat ra Menu)");
                        inKeNgang(74);

                        // Dong 1: Nam
                        if (buoc == 1) {
                            inDongKhung("  Vui long chon nam   : <  " + to_string(namChon) + "  >", "\033[1;36m");
                        } else {
                            inDongKhung("  Nam da chon         : " + to_string(namChon));
                        }

                        // Dong 2: Thang
                        if (buoc == 2) {
                            string sThang = (thangChon < 10 ? "0" : "") + to_string(thangChon);
                            inDongKhung("  Vui long chon thang : <  Thang " + sThang + "  >", "\033[1;36m");
                        } else if (buoc > 2) {
                            string sThang = (thangChon < 10 ? "0" : "") + to_string(thangChon);
                            inDongKhung("  Thang da chon       : Thang " + sThang);
                        }

                        // Tinh so ngay hop le cua thang
                        bool laNamNhuan = (namChon % 4 == 0 && namChon % 100 != 0) || (namChon % 400 == 0);
                        int soNgay = 31;
                        if (thangChon == 4 || thangChon == 6 || thangChon == 9 || thangChon == 11) {
                            soNgay = 30;
                        } else if (thangChon == 2) {
                            soNgay = laNamNhuan ? 29 : 28;
                        }
                        if (ngayChon > soNgay) ngayChon = soNgay;

                        // Dong 3: Ngay
                        if (buoc == 3) {
                            string sNgay = (ngayChon < 10 ? "0" : "") + to_string(ngayChon);
                            inDongKhung("  Vui long chon ngay  : <  Ngay " + sNgay + "  >", "\033[1;36m");
                        }

                        inKeNgang(74);

                        int phim = docPhim();

                        // Xu ly phim ESC
                        if (phim == PHIM_ESC || phim == 'q' || phim == 'Q') {
                            if (buoc == 1) {
                                break;
                            } else {
                                buoc--;
                                continue;
                            }
                        }

                        // Dieu chinh gia tri tai cho
                        if (buoc == 1) {
                            if (phim == PHIM_LEN || phim == PHIM_PHAI || phim == 'w' || phim == 'W' || phim == 'd' || phim == 'D') {
                                if (namChon < namKetThuc) namChon++;
                                else namChon = namBatDau;
                            } else if (phim == PHIM_XUONG || phim == PHIM_TRAI || phim == 's' || phim == 'S' || phim == 'a' || phim == 'A') {
                                if (namChon > namBatDau) namChon--;
                                else namChon = namKetThuc;
                            } else if (phim == PHIM_ENTER || phim == '\n') {
                                buoc = 2;
                            }
                        } else if (buoc == 2) {
                            if (phim == PHIM_LEN || phim == PHIM_PHAI || phim == 'w' || phim == 'W' || phim == 'd' || phim == 'D') {
                                if (thangChon < 12) thangChon++;
                                else thangChon = 1;
                            } else if (phim == PHIM_XUONG || phim == PHIM_TRAI || phim == 's' || phim == 'S' || phim == 'a' || phim == 'A') {
                                if (thangChon > 1) thangChon--;
                                else thangChon = 12;
                            } else if (phim == PHIM_ENTER || phim == '\n') {
                                buoc = 3;
                            }
                        } else if (buoc == 3) {
                            if (phim == PHIM_LEN || phim == PHIM_PHAI || phim == 'w' || phim == 'W' || phim == 'd' || phim == 'D') {
                                if (ngayChon < soNgay) ngayChon++;
                                else ngayChon = 1;
                            } else if (phim == PHIM_XUONG || phim == PHIM_TRAI || phim == 's' || phim == 'S' || phim == 'a' || phim == 'A') {
                                if (ngayChon > 1) ngayChon--;
                                else ngayChon = soNgay;
                            } else if (phim == PHIM_ENTER || phim == '\n') {
                                string chuoiThang = (thangChon < 10 ? "0" : "") + to_string(thangChon);
                                string chuoiNgay = (ngayChon < 10 ? "0" : "") + to_string(ngayChon);
                                string chuoiNam = to_string(namChon);
                                string ngayKiemTra = chuoiNam + "-" + chuoiThang + "-" + chuoiNgay;

                                string ngayHienTai = layNgayHienTai();

                                if (ngayKiemTra > ngayHienTai) {
                                    xoaManHinh();
                                    cout << "\n";
                                    inKeNgang(74);
                                    inDongTieuDeKhung("[!] CANH BAO: NGAY KIEM TRA KHONG HOP LE");
                                    inKeNgang(74);
                                    inDongKhung("Ngay ban da chon : " + ngayKiemTra + " (Vuot qua thoi gian hien tai)");
                                    inDongKhung("Ngay hop le      : Tu 1980-01-01 den ngay hien tai (" + ngayHienTai + ")");
                                    inDongKhung("Goi y            : Vui long chon ngay trong qua khu hoac ngay hom nay.");
                                    inKeNgang(74);
                                    cout << "\n>> Nhan ESC hoac phim bat ky de quay lai dieu chinh...";
                                    docPhim();
                                    buoc = 1;
                                    continue;
                                }

                                vector<PhieuMuon> dangMuon, quaHan;
                                service.kiemTraQuaHan(ngayKiemTra, dangMuon, quaHan);

                                xoaManHinh();
                                cout << "\n";
                                inKeNgang(74);
                                inDongTieuDeKhung("[RQ2] KET QUA KIEM TRA PHIEU MUON NGAY " + ngayKiemTra);
                                inKeNgang(74);

                                inBangPhieu(dangMuon, "DANH SACH PHIEU DANG TRONG HAN");
                                inBangPhieu(quaHan, "CANH BAO: DANH SACH PHIEU QUA HAN");
                                dungManHinh();
                                break;
                            }
                        }
                    }
                    break;
                }
                case 5: { // [RQ3] Tim kiem tai lieu theo ten/tu khoa
                    while (true) {
                        xoaManHinh();
                        cout << "--- [RQ3] TIM KIEM TAI LIEU THEO TEN / TU KHOA ---\n";
                        cout << "Nhap tu khoa ten sach (khong phan biet chu hoa/thuong) [ESC de ve Menu chinh]: ";

                        string tuKhoa;
                        if (!docChuoiHoacESC(tuKhoa) || tuKhoa.empty()) {
                            break;
                        }

                        vector<TaiLieu> ds = service.timTheoTen(tuKhoa);
                        xoaManHinh();

                        if (ds.empty()) {
                            cout << "\n[!] Khong tim thay tai lieu nao chua tu khoa [" << tuKhoa << "].\n";
                        } else {
                            cout << "\n";
                            inKeNgang(87);
                            string tieuDeKQ = "KET QUA TIM KIEM THEO TU KHOA [" + tuKhoa + "]";
                            int padTraiKQ = (85 - (int)tieuDeKQ.length()) / 2;
                            int padPhaiKQ = 85 - (int)tieuDeKQ.length() - padTraiKQ;
                            cout << "|" << string(padTraiKQ, ' ') << tieuDeKQ << string(padPhaiKQ, ' ') << "|\n";
                            inBangSach(ds);
                        }
                        cout << "\n>> Nhan ESC hoac phim bat ky de tiep tuc tim kiem...";
                        docPhim();
                    }
                    break;
                }
                case 6: { // Hien thi toan bo kho sach
                    cout << "--- TOAN BO DANH MUC TAI LIEU TRONG KHO ---\n";
                    inBangSach(service.layTatCaSach());
                    break;
                }
            }

            if (luaChon != 1 && luaChon != 2 && luaChon != 3 && luaChon != 4 && luaChon != 5) {
                dungManHinh();
            }
        }
    }
}
