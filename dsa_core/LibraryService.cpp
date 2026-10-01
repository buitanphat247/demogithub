#include "LibraryService.h"
#include <algorithm>

using namespace std;

LibraryService::LibraryService() {}

// Nap du lieu vao bo nho he thong
void LibraryService::napDuLieu(const vector<TaiLieu>& s, const vector<DocGia>& dg,
                    const vector<PhieuMuon>& pm, const vector<WaitlistEntry> dscm) {
    dsSach = s;
    dsDocGia = dg;
    dsPhieuMuon = pm;
}

// [MC1] Tra cuu tai lieu theo ma (Linear Search)
TaiLieu* LibraryService::timTheoMa(const string& ma) {
    for (size_t i = 0; i < dsSach.size(); i++) {
        if (dsSach[i].maTL == ma) {
            return &dsSach[i];
        }
    }
    return nullptr;
}

// [MC2] Lay Top K tai lieu muon nhieu nhat (Selection Sort giam dan)
vector<TaiLieu> LibraryService::layTopMuonNhieuNhat(int k) {
    vector<TaiLieu> copy = dsSach;
    
    // Sap xep giam dan theo luotMuon
    for (size_t i = 0; i < copy.size(); i++) {
        for (size_t j = i + 1; j < copy.size(); j++) {
            if (copy[j].luotMuon > copy[i].luotMuon) {
                swap(copy[i], copy[j]);
            }
        }
    }

    // Lay k phan tu dau tien
    vector<TaiLieu> ketQua;
    for (size_t i = 0; i < copy.size() && (int)i < k; i++) {
        ketQua.push_back(copy[i]);
    }
    return ketQua;
}

// Chuyen chuoi ve chu thuong de so sanh khong phan biet hoa/thuong
static string chuyenChuThuong(const string& s) {
    string res = s;
    for (char& c : res) {
        c = (char)tolower((unsigned char)c);
    }
    return res;
}

// [RQ1] Loc danh sach tai lieu theo the loai
vector<TaiLieu> LibraryService::timTheoTheLoai(const string& theLoai) {
    vector<TaiLieu> ketQua;
    string target = chuyenChuThuong(theLoai);
    for (size_t i = 0; i < dsSach.size(); i++) {
        if (chuyenChuThuong(dsSach[i].theLoai) == target) {
            ketQua.push_back(dsSach[i]);
        }
    }
    return ketQua;
}

// [RQ2] Phan loai phieu muon dang muon va qua han theo ngay moc YYYY-MM-DD
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

// [RQ3] Tim kiem tai lieu theo tu khoa (chuoi con trong ten sach)
vector<TaiLieu> LibraryService::timTheoTen(const string& tuKhoa) {
    vector<TaiLieu> ketQua;
    string tuKhoaThuong = chuyenChuThuong(tuKhoa);
    for (size_t i = 0; i < dsSach.size(); i++) {
        if (chuyenChuThuong(dsSach[i].tenTL).find(tuKhoaThuong) != string::npos) {
            ketQua.push_back(dsSach[i]);
        }
    }
    return ketQua;
}

// Lay toan bo danh sach sach hien co
vector<TaiLieu> LibraryService::layTatCaSach() const {
    return dsSach;
}
