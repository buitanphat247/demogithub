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

// Ghi danh sach sach vao file JSON
bool ghiDanhSachSach(const string& duongDan, const vector<TaiLieu>& ds) {
    ofstream file(duongDan);
    if (!file.is_open()) {
        cerr << "[!] Khong the mo file: " << duongDan << endl;
        return false;
    }

    try {
        json j = json::array();
        for (const auto& tl : ds) {
            json item;
            item["maTL"] = tl.maTL;
            item["tenTL"] = tl.tenTL;
            item["tacGia"] = tl.tacGia;
            item["theLoai"] = tl.theLoai;
            item["namXB"] = tl.namXB;
            item["soLuong"] = tl.soLuong;
            item["luotMuon"] = tl.luotMuon;
            j.push_back(item);
        }
        file << j.dump(4);
        
        file.close();
        return true;
    } catch (const exception& e) {
        cerr << "[!] Loi parse JSON file " << duongDan << ": " << e.what() << endl;
        return false;
    }
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

// Ghi danh sach doc gia vao file JSON
bool ghiDanhSachDocGia(const string& duongDan, const vector<DocGia>& ds) {
    ofstream file(duongDan);
    if (!file.is_open()) {
        cerr << "[!] Khong the mo file: " << duongDan << endl;
        return false;
    }

    try {
        json j = json::array();
        for (const auto& dg : ds) {
            json item;
            item["maDG"] = dg.maDG;
            item["hoTen"] = dg.hoTen;
            item["sdt"] = dg.sdt;
            item["loaiThe"] = dg.loaiThe;
            j.push_back(item);
        }
        file << j.dump(4);
        
        file.close();
        return true;
    } catch (const exception& e) {
        cerr << "[!] Loi parse JSON file " << duongDan << ": " << e.what() << endl;
        return false;
    }
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

// Ghi danh sach phieu muon vao file JSON
bool ghiDanhSachPhieu(const string& duongDan, const vector<PhieuMuon>& ds) {
    ofstream file(duongDan);
    if (!file.is_open()) {
        cerr << "[!] Khong the mo file: " << duongDan << endl;
        return false;
    }

    try {
        json j = json::array();
        for (const auto& pm : ds) {
            json item;
            item["maPhieu"] = pm.maPhieu;
            item["maDG"] = pm.maDG;
            item["maTL"] = pm.maTL;
            item["ngayMuon"] = pm.ngayMuon;
            item["hanTra"] = pm.hanTra;
            item["trangThai"] = pm.trangThai;
            j.push_back(item);
        }
        file << j.dump(4);
        
        file.close();
        return true;
    } catch (const exception& e) {
        cerr << "[!] Loi parse JSON file " << duongDan << ": " << e.what() << endl;
        return false;
    }
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

// Ghi danh sach hang cho vao file JSON
bool ghiDanhSachHangCho(const string& duongDan, const vector<WaitlistEntry>& ds) {
    ofstream file(duongDan);
    if (!file.is_open()) {
        cerr << "[!] Khong the mo file: " << duongDan << endl;
        return false;
    }

    try {
        json j = json::array();
        for (const auto& entry : ds) {
            json item;
            item["maTL"] = entry.maTL;
            item["maDG"] = entry.maDG;
            item["doUuTien"] = entry.doUuTien;
            item["thoiGian"] = entry.thoiGian;
            j.push_back(item);
        }
        file << j.dump(4);
        
        file.close();
        return true;
    } catch (const exception& e) {
        cerr << "[!] Loi parse JSON file " << duongDan << ": " << e.what() << endl;
        return false;
    }
}