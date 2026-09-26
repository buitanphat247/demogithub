#include "LibraryService.h"
#include <algorithm>

using namespace std;

LibraryService::LibraryService() {}

void LibraryService::napDuLieu(const vector<TaiLieu>& s, const vector<DocGia>& dg, const vector<PhieuMuon>& pm) {
    dsSach = s;
    dsDocGia = dg;
    dsPhieuMuon = pm;
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

vector<TaiLieu> LibraryService::timTheoTen(const string& tuKhoa) {
    vector<TaiLieu> ketQua;
    for (size_t i = 0; i < dsSach.size(); i++) {
        if (dsSach[i].tenTL.find(tuKhoa) != string::npos) {
            ketQua.push_back(dsSach[i]);
        }
    }
    return ketQua;
}

vector<TaiLieu> LibraryService::layTatCaSach() const {
    return dsSach;
}
