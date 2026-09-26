#include "FileStore.h"
#include <fstream>
#include <iostream>
#include "../nlohmann/json.hpp"

using namespace std;
using json = nlohmann::json;

// Doc danh sach sach tu file JSON
vector<TaiLieu> docDanhSachSach(const string& duongDan) {
    vector<TaiLieu> ds;
    ifstream file(duongDan);
    if (!file.is_open()) {
        cerr << "[!] Khong the mo file: " << duongDan << endl;
        return ds;
    }

    try {
        json j;
        file >> j;
        for (const auto& item : j) {
            TaiLieu tl;
            tl.maTL     = item.value("maTL", "");
            tl.tenTL    = item.value("tenTL", "");
            tl.tacGia   = item.value("tacGia", "");
            tl.theLoai  = item.value("theLoai", "");
            tl.namXB    = item.value("namXB", 0);
            tl.soLuong  = item.value("soLuong", 0);
            tl.luotMuon = item.value("luotMuon", 0);
            ds.push_back(tl);
        }
    } catch (const exception& e) {
        cerr << "[!] Loi parse JSON file " << duongDan << ": " << e.what() << endl;
    }

    return ds;
}

// Doc danh sach doc gia tu file JSON
vector<DocGia> docDanhSachDocGia(const string& duongDan) {
    vector<DocGia> ds;
    ifstream file(duongDan);
    if (!file.is_open()) {
        cerr << "[!] Khong the mo file: " << duongDan << endl;
        return ds;
    }

    try {
        json j;
        file >> j;
        for (const auto& item : j) {
            DocGia dg;
            dg.maDG    = item.value("maDG", "");
            dg.hoTen   = item.value("hoTen", "");
            dg.sdt     = item.value("sdt", "");
            dg.loaiThe = item.value("loaiThe", "Normal");
            ds.push_back(dg);
        }
    } catch (const exception& e) {
        cerr << "[!] Loi parse JSON file " << duongDan << ": " << e.what() << endl;
    }

    return ds;
}

// Doc danh sach phieu muon tu file JSON
vector<PhieuMuon> docDanhSachPhieu(const string& duongDan) {
    vector<PhieuMuon> ds;
    ifstream file(duongDan);
    if (!file.is_open()) {
        cerr << "[!] Khong the mo file: " << duongDan << endl;
        return ds;
    }

    try {
        json j;
        file >> j;
        for (const auto& item : j) {
            PhieuMuon pm;
            pm.maPhieu   = item.value("maPhieu", "");
            pm.maDG      = item.value("maDG", "");
            pm.maTL      = item.value("maTL", "");
            pm.ngayMuon  = item.value("ngayMuon", "");
            pm.hanTra    = item.value("hanTra", "");
            pm.trangThai = item.value("trangThai", "DangMuon");
            ds.push_back(pm);
        }
    } catch (const exception& e) {
        cerr << "[!] Loi parse JSON file " << duongDan << ": " << e.what() << endl;
    }

    return ds;
}

// Doc danh sach hang cho tu file JSON
vector<WaitlistEntry> docDanhSachHangCho(const string& duongDan) {
    vector<WaitlistEntry> ds;
    ifstream file(duongDan);
    if (!file.is_open()) {
        cerr << "[!] Khong the mo file: " << duongDan << endl;
        return ds;
    }

    try {
        json j;
        file >> j;
        for (const auto& item : j) {
            WaitlistEntry entry;
            entry.maTL     = item.value("maTL", "");
            entry.maDG     = item.value("maDG", "");
            entry.doUuTien = item.value("doUuTien", 1);
            entry.thoiGian = item.value("thoiGian", "");
            ds.push_back(entry);
        }
    } catch (const exception& e) {
        cerr << "[!] Loi parse JSON file " << duongDan << ": " << e.what() << endl;
    }

    return ds;
}
