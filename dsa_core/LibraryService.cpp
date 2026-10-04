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
void LibraryService::napDanhSachDocGia(const vector<DocGia>& dsDocGia) {
    dgMap.clear();
    for (const auto& dg : dsDocGia) {
        dgMap[dg.maDG] = dg;
    }
}
DocGia* LibraryService::timDocGiaTheoMa(const string& maDG) {
    if (dgMap.count(maDG) > 0) { 
        return &dgMap[maDG];
    }
    return nullptr;
}

// So sanh thu tu uu tien cua hai doc gia
static bool soSanhUuTien(const DGChoMuon& dg1, const DGChoMuon& dg2) {
    int loaiThe_dg1 = dg1.doUuTienThe();
    int loaiThe_dg2 = dg2.doUuTienThe();
    if (loaiThe_dg1 != loaiThe_dg2) {
        return loaiThe_dg1 < loaiThe_dg2;
    }

    string ngaydk_dg1 = dg1.thoiGian.substr(0, 10);
    string ngaydk_dg2 = dg2.thoiGian.substr(0, 10);
    if (ngaydk_dg1 == ngaydk_dg2) {
        return dg1.doUuTien < dg2.doUuTien;
    }
    return ngaydk_dg1 < ngaydk_dg2;
}

// Chuan hoa thoi gian ve dinh dang YYYY-MM-DD
static string chuanHoaThgian(const string& thgian) {

    if (thgian.empty()) {
        return "9999-99-99 23:59:59";
    }
    string s = thgian;
    // Sai dinh dang "-" ("/") 
    for (char& c : s) {
        if (c == '/') c = '-';
    }

    // Dinh dang nguoc DD-MM-YYYY
    if (s.length() >= 10 && s[2] == '-' && s[5] == '-') {
        string ngay = s.substr(0, 2);
        string thang = s.substr(3, 2);
        string nam = s.substr(6, 4);
        string timePart = "";
        if (s[10] == ' ') {
            timePart = " " + s.substr(11);
        } else {
            timePart = s.substr(10);
        }
        return nam + "-" + thang + "-" + ngay + timePart;
    }
    return s;
}

vector<DGChoMuon> LibraryService::locDuLieu(const string& maTL, const vector<WaitlistEntry>& dsChoMuon, 
                                     const vector<DocGia>& dsDocGia) {
    napDanhSachDocGia(dsDocGia);
    vector<DGChoMuon> ketQua;
    for (const auto& entry : dsChoMuon) {
        if (entry.maTL == maTL) {
            const DocGia* reader = timDocGiaTheoMa(entry.maDG);

            if (reader != nullptr) {
                DGChoMuon dg;
                dg.maDG = reader->maDG;
                dg.hoTen = reader->hoTen;
                dg.sdt = reader->sdt;
                dg.thoiGian = chuanHoaThgian(entry.thoiGian);
                if (dg.thoiGian == "9999-99-99 23:59:59") {
                    dg.doUuTien = 9999;
                    dg.loaiThe = "Normal";
                }
                else {
                    dg.doUuTien = entry.doUuTien;
                    dg.loaiThe = reader->loaiThe;
                }
                ketQua.push_back(dg);
            }
        }
    }
    return ketQua;
}

// Lay danh sach doc gia cho muon tai lieu theo ma tai lieu
vector<DGChoMuon> LibraryService::layDanhSachChoTheoMaTL_Thuong(const string& maTL, const vector<WaitlistEntry>& dsChoMuon, const vector<DocGia>& dsDocGia) {
    vector<DGChoMuon> ketQua = locDuLieu(maTL, dsChoMuon, dsDocGia);
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

PriorityQueue LibraryService::layDanhSachChoTheoMaTL(const string& maTL, const vector<WaitlistEntry>& dsChoMuon, const vector<DocGia>& dsDocGia) {
    vector<DGChoMuon> hangdoi = locDuLieu(maTL, dsChoMuon, dsDocGia);
    PriorityQueue hangCho(hangdoi.size() > 0 ? hangdoi.size() : 1);
    
    for (const auto& u : hangdoi) {
        hangCho.EnQueue(u);
    }

    return hangCho;
}