#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <limits>
#include <cctype>

using namespace std;
// ============================================================
//0. HÀM HỖ TRỢ CHUẨN HÓA CHUỖI (loại bỏ dấu tiếng Việt, chuyển về chữ thường)
// ============================================================
std::string BoDauVaVietThuong(const std::string& str) {
    std::string res = "";
    for (size_t i = 0; i < str.length(); ) {
        unsigned char c = str[i];

        // Ký tự ASCII chuẩn (a-z, A-Z, 0-9...)
        if (c < 128) {
            res += (char)std::tolower(c);
            i++;
        } 
        // Ký tự UTF-8 2 bytes (Các chữ cái Tiếng Việt có dấu phổ biến)
        else if ((c & 0xE0) == 0xC0 && i + 1 < str.length()) {
            unsigned char c2 = str[i + 1];
            
            // Nhóm chữ A (à á ả ã ạ â ấ ầ ẩ ẫ ậ ă ắ ằ ẳ ẵ ặ...)
            if ((c == 0xC3 && (c2 >= 0x80 && c2 <= 0x85)) || 
                (c == 0xC3 && (c2 >= 0xA0 && c2 <= 0xA5)) ||
                (c == 0xC4 && (c2 >= 0x80 && c2 <= 0x83))) {
                res += 'a';
            }
            // Nhóm chữ E (è é ẻ ẽ ẹ ê ế ề ể ễ ệ...)
            else if ((c == 0xC3 && (c2 >= 0x88 && c2 <= 0x8B)) || 
                     (c == 0xC3 && (c2 >= 0xA8 && c2 <= 0xAB)) ||
                     (c == 0xC3 && c2 == 0xAA)) {
                res += 'e';
            }
            // Nhóm chữ I (ì í ỉ ĩ ị...)
            else if ((c == 0xC3 && (c2 >= 0x8C && c2 <= 0x8D)) || 
                     (c == 0xC3 && (c2 >= 0xAC && c2 <= 0xAD))) {
                res += 'i';
            }
            // Nhóm chữ O (ò ó ỏ õ ọ ô ố ồ ổ ỗ ộ ơ ớ ờ ở ỡ ợ...)
            else if ((c == 0xC3 && (c2 >= 0x92 && c2 <= 0x96)) || 
                     (c == 0xC3 && (c2 >= 0xB2 && c2 <= 0xB6)) ||
                     (c == 0xC3 && c2 == 0xB4) || (c == 0xC6 && c2 == 0xA1)) {
                res += 'o';
            }
            // Nhóm chữ U (ù ú ủ ũ ụ ư ứ ừ ử ữ ự...)
            else if ((c == 0xC3 && (c2 >= 0x99 && c2 <= 0x9C)) || 
                     (c == 0xC3 && (c2 >= 0xB9 && c2 <= 0xBC)) ||
                     (c == 0xC6 && c2 == 0xB0)) {
                res += 'u';
            }
            // Nhóm chữ Y (ỳ ý ỷ ỹ ỵ...)
            else if ((c == 0xC3 && c2 == 0xBD) || (c == 0xC3 && c2 == 0xDD)) {
                res += 'y';
            }
            // Nhóm chữ Đ (Đ / đ)
            else if ((c == 0xC4 && (c2 == 0x90 || c2 == 0x91))) {
                res += 'd';
            }
            else {
                res += (char)std::tolower(c);
            }
            i += 2;
        } 
        // Ký tự UTF-8 3 bytes (Ví dụ các dấu ghép tổ hợp)
        else if ((c & 0xF0) == 0xE0 && i + 2 < str.length()) {
            i += 3;
        } else {
            i++;
        }
    }
    return res;
}

// ============================================================
// 1. CẤU TRÚC DỮ LIỆU BẢN GHI TÀI LIỆU
// ============================================================
struct TaiLieu {
    string maTaiLieu;   // khóa chính - dùng cho MC1
    string tenTaiLieu;  // dùng cho RQ3
    string tacGia;
    string theLoai;     // khóa nhóm - dùng cho RQ1
    int namXuatBan;
    int soLuong;
    string tinhTrang;   // ví dụ: "Còn sách" / "Hết sách"

    TaiLieu() {}
    TaiLieu(string ma, string ten, string tg, string tl, int nam, int sl, string tt)
        : maTaiLieu(ma), tenTaiLieu(ten), tacGia(tg), theLoai(tl),
          namXuatBan(nam), soLuong(sl), tinhTrang(tt) {}
};

// Hàm chuyển chuỗi về chữ thường để so khớp không phân biệt hoa/thường
string ChuoiThuong(const string& chuoi) {
    string ketQua = chuoi;
    transform(ketQua.begin(), ketQua.end(), ketQua.begin(),
              [](unsigned char c) { return tolower(c); });
    return ketQua;
}

// ============================================================
// 2. BẢNG BĂM CHÍNH (MC1) — khóa duy nhất -> 1 bản ghi
// ============================================================
class BangBamMaTaiLieu {
private:
    struct Nut {
        string khoa;
        TaiLieu* giaTri;
        Nut* tiepTheo;
        Nut(string k, TaiLieu* v) : khoa(k), giaTri(v), tiepTheo(nullptr) {}
    };

    vector<Nut*> mangNgan;
    int soNganBan;
    int soPhanTu;

    unsigned long BamChuoi(const string& khoa) const {
        unsigned long ma = 5381;
        for (char c : khoa) ma = ma * 33 + (unsigned char)c;
        return ma;
    }

    int ViTriNgan(const string& khoa) const {
        return (int)(BamChuoi(khoa) % soNganBan);
    }

    void BamLai() {
        int soNganMoi = soNganBan * 2;
        vector<Nut*> mangMoi(soNganMoi, nullptr);
        for (int i = 0; i < soNganBan; i++) {
            Nut* nut = mangNgan[i];
            while (nut != nullptr) {
                Nut* nutTiep = nut->tiepTheo;
                unsigned long viTriMoi = BamChuoi(nut->khoa) % soNganMoi;
                nut->tiepTheo = mangMoi[viTriMoi];
                mangMoi[viTriMoi] = nut;
                nut = nutTiep;
            }
        }
        mangNgan = mangMoi;
        soNganBan = soNganMoi;
    }

public:
    BangBamMaTaiLieu(int soNganBanDau = 101)
        : soNganBan(soNganBanDau), soPhanTu(0) {
        mangNgan.assign(soNganBan, nullptr);
    }

    ~BangBamMaTaiLieu() {
        for (int i = 0; i < soNganBan; i++) {
            Nut* nut = mangNgan[i];
            while (nut != nullptr) {
                Nut* nutXoa = nut;
                nut = nut->tiepTheo;
                delete nutXoa;
            }
        }
    }

    void ChenHoacCapNhat(const string& maTaiLieu, TaiLieu* banGhi) {
        double heSoTai = (double)(soPhanTu + 1) / soNganBan;
        if (heSoTai > 0.75) BamLai();

        int viTri = ViTriNgan(maTaiLieu);
        Nut* nut = mangNgan[viTri];
        while (nut != nullptr) {
            if (nut->khoa == maTaiLieu) {
                nut->giaTri = banGhi;
                return;
            }
            nut = nut->tiepTheo;
        }
        Nut* nutMoi = new Nut(maTaiLieu, banGhi);
        nutMoi->tiepTheo = mangNgan[viTri];
        mangNgan[viTri] = nutMoi;
        soPhanTu++;
    }

    TaiLieu* TimTheoMa(const string& maTaiLieu) const {
        int viTri = ViTriNgan(maTaiLieu);
        Nut* nut = mangNgan[viTri];
        while (nut != nullptr) {
            if (nut->khoa == maTaiLieu) return nut->giaTri;
            nut = nut->tiepTheo;
        }
        return nullptr;
    }

    bool Xoa(const string& maTaiLieu) {
        int viTri = ViTriNgan(maTaiLieu);
        Nut* nut = mangNgan[viTri];
        Nut* nutTruoc = nullptr;
        while (nut != nullptr) {
            if (nut->khoa == maTaiLieu) {
                if (nutTruoc == nullptr) mangNgan[viTri] = nut->tiepTheo;
                else nutTruoc->tiepTheo = nut->tiepTheo;
                delete nut;
                soPhanTu--;
                return true;
            }
            nutTruoc = nut;
            nut = nut->tiepTheo;
        }
        return false;
    }

    int SoPhanTu() const { return soPhanTu; }
};

// ============================================================
// 3. BẢNG BĂM PHỤ (RQ1) — khóa = thể loại -> danh sách con trỏ
// ============================================================
class BangBamTheLoai {
private:
    struct Nut {
        string khoa;
        vector<TaiLieu*> danhSach;
        Nut* tiepTheo;
        Nut(string k) : khoa(k), tiepTheo(nullptr) {}
    };

    vector<Nut*> mangNgan;
    int soNganBan;
    int soPhanTu;

    unsigned long BamChuoi(const string& khoa) const {
        unsigned long ma = 5381;
        for (char c : khoa) ma = ma * 33 + (unsigned char)c;
        return ma;
    }

    int ViTriNgan(const string& khoa) const {
        return (int)(BamChuoi(khoa) % soNganBan);
    }

    Nut* TimNut(const string& theLoai) const {
        int viTri = ViTriNgan(theLoai);
        Nut* nut = mangNgan[viTri];
        while (nut != nullptr) {
            if (nut->khoa == theLoai) return nut;
            nut = nut->tiepTheo;
        }
        return nullptr;
    }

public:
    BangBamTheLoai(int soNganBanDau = 31)
        : soNganBan(soNganBanDau), soPhanTu(0) {
        mangNgan.assign(soNganBan, nullptr);
    }

    ~BangBamTheLoai() {
        for (int i = 0; i < soNganBan; i++) {
            Nut* nut = mangNgan[i];
            while (nut != nullptr) {
                Nut* nutXoa = nut;
                nut = nut->tiepTheo;
                delete nutXoa;
            }
        }
    }

    void ThemVaoNhom(const string& theLoai, TaiLieu* banGhi) {
        Nut* nut = TimNut(theLoai);
        if (nut != nullptr) {
            nut->danhSach.push_back(banGhi);
            return;
        }
        int viTri = ViTriNgan(theLoai);
        Nut* nutMoi = new Nut(theLoai);
        nutMoi->danhSach.push_back(banGhi);
        nutMoi->tiepTheo = mangNgan[viTri];
        mangNgan[viTri] = nutMoi;
        soPhanTu++;
    }

    bool GoKhoiNhom(const string& theLoai, const string& maTaiLieu) {
        Nut* nut = TimNut(theLoai);
        if (nut == nullptr) return false;
        for (size_t i = 0; i < nut->danhSach.size(); i++) {
            if (nut->danhSach[i]->maTaiLieu == maTaiLieu) {
                nut->danhSach.erase(nut->danhSach.begin() + i);
                return true;
            }
        }
        return false;
    }

    vector<TaiLieu*> TimTheoTheLoai(const string& theLoai) const {
        Nut* nut = TimNut(theLoai);
        if (nut == nullptr) return {};
        return nut->danhSach;
    }

    vector<vector<TaiLieu*>*> LayTatCaNhom() {
        vector<vector<TaiLieu*>*> ketQua;
        for (int i = 0; i < soNganBan; i++) {
            Nut* nut = mangNgan[i];
            while (nut != nullptr) {
                ketQua.push_back(&nut->danhSach);
                nut = nut->tiepTheo;
            }
        }
        return ketQua;
    }
};

// ============================================================
// 4. LỚP QUẢN LÝ TÀI LIỆU
// ============================================================
class QuanLyTaiLieu {
private:
    BangBamMaTaiLieu bangBamMa;      // MC1
    BangBamTheLoai bangBamTheLoai;   // RQ1, RQ3
    vector<TaiLieu*> tatCaBanGhi;

public:
    ~QuanLyTaiLieu() {
        for (TaiLieu* bg : tatCaBanGhi) delete bg;
    }

    void ThemTaiLieu(const string& ma, const string& ten, const string& tacGia,
                      const string& theLoai, int nam, int soLuong, const string& tinhTrang) {
        TaiLieu* banGhi = new TaiLieu(ma, ten, tacGia, theLoai, nam, soLuong, tinhTrang);
        tatCaBanGhi.push_back(banGhi);
        bangBamMa.ChenHoacCapNhat(ma, banGhi);
        bangBamTheLoai.ThemVaoNhom(theLoai, banGhi);
    }

    // MC1
    TaiLieu* MC1_TraCuuTheoMa(const string& ma) const {
        return bangBamMa.TimTheoMa(ma);
    }

    // RQ1
    vector<TaiLieu*> RQ1_TraCuuTheoTheLoai(const string& theLoai) const {
        return bangBamTheLoai.TimTheoTheLoai(theLoai);
    }

    // RQ3
    vector<TaiLieu*> RQ3_TimTheoTen(const string& tuKhoa) const {
    vector<TaiLieu*> ketQua;

    // Chuẩn hóa từ khóa người dùng nhập (ví dụ: "Cơ sở" -> "co so")
    string tuKhoaChuAn = BoDauVaVietThuong(tuKhoa);

    vector<vector<TaiLieu*>*> tatCaNhom =
        const_cast<BangBamTheLoai&>(bangBamTheLoai).LayTatCaNhom();

    for (vector<TaiLieu*>* nhom : tatCaNhom) {
        if (!nhom) continue; // Kiểm tra con trỏ an toàn

        for (TaiLieu* tl : *nhom) {
            if (!tl) continue; // Kiểm tra con trỏ an toàn

            // Chuẩn hóa tên tài liệu từ CSDL (ví dụ: "Cơ sở dữ liệu" -> "co so du lieu")
            string tenTaiLieuChuAn = BoDauVaVietThuong(tl->tenTaiLieu);

            // Tìm kiếm từ khóa chuẩn hóa trong tên tài liệu chuẩn hóa
            if (tenTaiLieuChuAn.find(tuKhoaChuAn) != string::npos) {
                ketQua.push_back(tl);
            }
        }
    }
    return ketQua;
}

    int TongSoTaiLieu() const { return bangBamMa.SoPhanTu(); }
};

// ============================================================
// 5. HÀM HIỂN THỊ HỖ TRỢ
// ============================================================
void InTaiLieu(const TaiLieu* tl) {
    if (tl == nullptr) {
        cout << "  -> Khong tim thay tai lieu phu hop.\n";
        return;
    }
    cout << "  [" << tl->maTaiLieu << "] " << tl->tenTaiLieu
         << " - " << tl->tacGia << " (" << tl->namXuatBan << ")"
         << " | The loai: " << tl->theLoai
         << " | SL: " << tl->soLuong
         << " | Tinh trang: " << tl->tinhTrang << "\n";
}

void InDanhSach(const vector<TaiLieu*>& ds) {
    if (ds.empty()) {
        cout << "  -> Khong co ket qua phu hop.\n";
        return;
    }
    for (TaiLieu* tl : ds) InTaiLieu(tl);
}

// ============================================================
// 6. GIAO DIỆN NHẬP XUẤT TƯƠNG TÁC (MENU)
// ============================================================
int main() {
    QuanLyTaiLieu qlTaiLieu;

    // Khởi tạo trước một vài dữ liệu mẫu để thử nghiệm
    qlTaiLieu.ThemTaiLieu("TL001", "Cau Truc Du Lieu Va Giai Thuat", "Nguyen Van A", "Cong nghe thong tin", 2019, 5, "Con sach");
    qlTaiLieu.ThemTaiLieu("TL002", "Nhap Mon Co So Du Lieu", "Tran Thi B", "Cong nghe thong tin", 2020, 3, "Con sach");
    qlTaiLieu.ThemTaiLieu("TL003", "Kinh Te Vi Mo", "Le Van C", "Kinh te", 2018, 2, "Het sach");

    int luaChon = 0;
    do {
        cout << "\n=========================================\n";
        cout << "      HE THONG QUAN LY THU VIEN\n";
        cout << "=========================================\n";
        cout << "1. Them tai lieu moi\n";
        cout << "2. [MC1] Tra cuu chinh xac theo MA TAI LIEU\n";
        cout << "3. [RQ1] Tra cuu danh sach theo THE LOAI\n";
        cout << "4. [RQ3] Tim kiem theo TEN TAI LIEU (hoac tu khoa)\n";
        cout << "5. THONG KE tong so tai lieu\n";
        cout << "0. Thoat\n";
        cout << "-----------------------------------------\n";
        cout << "Nhap lua chon cua ban: ";
        cin >> luaChon;

        // Xóa bộ đệm dòng nhập sau khi dùng cin >>
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (luaChon == 1) {
            string ma, ten, tacGia, theLoai, tinhTrang;
            int nam, soLuong;

            cout << "\n--- THEM TAI LIEU MOI ---\n";
            cout << "Nhap ma tai lieu: "; getline(cin, ma);
            cout << "Nhap ten tai lieu: "; getline(cin, ten);
            cout << "Nhap tac gia: "; getline(cin, tacGia);
            cout << "Nhap the loai: "; getline(cin, theLoai);
            cout << "Nhap nam xuat ban: "; cin >> nam;
            cout << "Nhap so luong: "; cin >> soLuong;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Nhap tinh trang (Con sach/Het sach): "; getline(cin, tinhTrang);

            qlTaiLieu.ThemTaiLieu(ma, ten, tacGia, theLoai, nam, soLuong, tinhTrang);
            cout << "=> Da them tai lieu thanh cong!\n";

        } else if (luaChon == 2) {
            string ma;
            cout << "\n--- [MC1] TRA CUU THEO MA TAI LIEU ---\n";
            cout << "Nhap ma tai lieu can tim: ";
            getline(cin, ma);
            InTaiLieu(qlTaiLieu.MC1_TraCuuTheoMa(ma));

        } else if (luaChon == 3) {
            string theLoai;
            cout << "\n--- [RQ1] TRA CUU THEO THE LOAI ---\n";
            cout << "Nhap the loai can tra cuu: ";
            getline(cin, theLoai);
            InDanhSach(qlTaiLieu.RQ1_TraCuuTheoTheLoai(theLoai));

        } else if (luaChon == 4) {
            string tuKhoa;
            cout << "\n--- [RQ3] TIM KIEM THEO TEN SACH / TU KHOA ---\n";
            cout << "Nhap ten sach hoac tu khoa: ";
            getline(cin, tuKhoa);
            InDanhSach(qlTaiLieu.RQ3_TimTheoTen(tuKhoa));

        } else if (luaChon == 5) {
            cout << "\nTong so tai lieu hien co trong he thong: " << qlTaiLieu.TongSoTaiLieu() << "\n";
        }

    } while (luaChon != 0);

    cout << "\nCam on ban da su dung chuong trinh!\n";
    return 0;
}