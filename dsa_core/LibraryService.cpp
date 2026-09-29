#include "LibraryService.h"
#include <algorithm>
#include <cctype>
#include "../persistence/FileStore.h"

using namespace std;

// ============================================================
// HÀM HỖ TRỢ: Chuyển Tiếng Việt UTF-8 về chữ thường không dấu & dọn ký tự xuống dòng
// ============================================================
static string BoDauVaVietThuong(string str) {
    // 1. Loại bỏ các ký tự xuống dòng dư thừa (\r, \n) ở cuối chuỗi nhập từ Console
    while (!str.empty() && (str.back() == '\r' || str.back() == '\n' || str.back() == ' ')) {
        str.pop_back();
    }

    string res = "";
    for (size_t i = 0; i < str.length(); ) {
        unsigned char c = str[i];

        // Ký tự ASCII tiêu chuẩn (a-z, A-Z, 0-9, khoảng trắng...)
        if (c < 128) {
            res += (char)tolower(c);
            i++;
        } 
        // Ký tự Tiếng Việt UTF-8 (2 bytes)
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
                res += (char)tolower(c);
            }
            i += 2;
        } 
        // Ký tự UTF-8 3 bytes
        else if ((c & 0xF0) == 0xE0 && i + 2 < str.length()) {
            i += 3;
        } 
        else {
            i++;
        }
    }
    return res;
}

// ============================================================
// PHƯƠNG THỨC LỚP LibraryService
// ============================================================ 
LibraryService::LibraryService() {}

void LibraryService::napDuLieu(const vector<TaiLieu>& s, const vector<DocGia>& dg, const vector<PhieuMuon>& pm) {
    dsSach = s;
    dsDocGia = dg;
    dsPhieuMuon = pm;

    // Nếu có dùng Bảng Băm bangBamTheLoai trong Core:
    // for (auto& sach : dsSach) {
    //     bangBamTheLoai.Them(sach.theLoai, &sach);
    // }
}

TaiLieu* LibraryService::timTheoMa(const string& ma) {
    for (size_t i = 0; i < dsSach.size(); i++) {
        if (dsSach[i].maTL == ma) {
            return &dsSach[i];
        }
    }
    return nullptr;
}

vector<TaiLieu> LibraryService::layTopMuonNhieuNhat(int k) {
    vector<TaiLieu> copy = dsSach;
    for (size_t i = 0; i < copy.size(); i++) {
        for (size_t j = i + 1; j < copy.size(); j++) {
            if (copy[j].luotMuon > copy[i].luotMuon) {
                swap(copy[i], copy[j]);
            }
        }
    }
    vector<TaiLieu> ketQua;
    for (size_t i = 0; i < copy.size() && (int)i < k; i++) {
        ketQua.push_back(copy[i]);
    }
    return ketQua;
}

vector<TaiLieu> LibraryService::timTheoTheLoai(const string& theLoai) {
    vector<TaiLieu> ketQua;
    for (size_t i = 0; i < dsSach.size(); i++) {
        if (dsSach[i].theLoai == theLoai) {
            ketQua.push_back(dsSach[i]);
        }
    }
    return ketQua;
}

void LibraryService::kiemTraQuaHan(const string& ngayHienTai, vector<PhieuMuon>& dsDangMuon, vector<PhieuMuon>& dsQuaHan) {
    dsDangMuon.clear();
    dsQuaHan.clear();
    for (size_t i = 0; i < dsPhieuMuon.size(); i++) {
        if (dsPhieuMuon[i].trangThai == "DangMuon") {
            if (dsPhieuMuon[i].hanTra < ngayHienTai) {
                dsQuaHan.push_back(dsPhieuMuon[i]);
            } else {
                dsDangMuon.push_back(dsPhieuMuon[i]);
            }
        }
    }
}

// RQ3: Tìm kiếm theo Tên / Từ khóa (Đã tối ưu Tiếng Việt Không Dấu & Xử lý ký tự đặc biệt)
vector<TaiLieu> LibraryService::timTheoTen(const string& tuKhoa) {
    vector<TaiLieu> ketQua;
    
    // Chuẩn hóa từ khóa người dùng nhập ("nhap mon" -> "nhap mon")
    string tuKhoaChuAn = BoDauVaVietThuong(tuKhoa);

    if (tuKhoaChuAn.empty()) return ketQua;

    for (size_t i = 0; i < dsSach.size(); i++) {
        // Chuẩn hóa tên sách trong CSDL ("Nhap mon Hoc May va Deep Learning" -> "nhap mon hoc may va deep learning")
        string tenChuAn = BoDauVaVietThuong(dsSach[i].tenTL);

        if (tenChuAn.find(tuKhoaChuAn) != string::npos) {
            ketQua.push_back(dsSach[i]);
        }
    }
    return ketQua;
}

vector<TaiLieu> LibraryService::layTatCaSach() const {
    return dsSach;
}