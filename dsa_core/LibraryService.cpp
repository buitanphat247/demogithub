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
    dsChoMuon = dscm;
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

// Ham phu tim doc gia theo maDG
DocGia* LibraryService::timDocGiaTheoMa(const string& maDG) {
    for (size_t i = 0; i < dsDocGia.size(); i++) {
        if (dsDocGia[i].maDG == maDG) {
            return &dsDocGia[i];
        }
    }
    return nullptr;
}

// Hàm so sánh tiêu chí ưu tiên theo thuật toán thông thường
static bool soSanhUuTien(const DGChoMuon& dg1, const DGChoMuon& dg2) {
    int loaiThe1 = dg1.doUuTienThe();
    int loaiThe2 = dg2.doUuTienThe();
    if (loaiThe1 != loaiThe2) {
        return loaiThe1 < loaiThe2;
    }

    string ngay1 = dg1.thoiGian.substr(0, 10);
    string ngay2 = dg2.thoiGian.substr(0, 10);
    if (ngay1 == ngay2) {
        return dg1.doUuTien < dg2.doUuTien;
    }
    return ngay1 < ngay2;
}

// Lay danh sach doc gia cho muon tai lieu theo ma tai lieu
vector<DGChoMuon> LibraryService::layDanhSachChoTheoMaTL(const string& maTL) {
    vector<DGChoMuon> ketQua;

    // Duyet tuyen tinh loc danh sach va lay du lieu tu dsDocGia
    for (const auto& entry : dsChoMuon) {
        if (entry.maTL == maTL) {
            const DocGia* reader = timDocGiaTheoMa(entry.maDG);

            if (reader != nullptr) {
                DGChoMuon dg;
                dg.maDG = reader->maDG;
                dg.hoTen = reader->hoTen;
                dg.sdt = reader->sdt;
                dg.loaiThe = reader->loaiThe;
                dg.thoiGian = entry.thoiGian;
                dg.doUuTien = entry.doUuTien;
                ketQua.push_back(dg);
            }
        }
    }

    // Sap xep ket qua bang Selection Sort 
    for (size_t i = 0; i < ketQua.size(); i++) {
        size_t minIdx = i;
        for (size_t j = i + 1; j < ketQua.size(); j++) {
            if (soSanhUuTien(ketQua[j], ketQua[minIdx])) {
                minIdx = j;
            }
        }
        if (minIdx != i) {
            swap(ketQua[i], ketQua[minIdx]);
        }
    }

    return ketQua;
}