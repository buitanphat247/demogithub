#ifndef LIBRARYSERVICE_H
#define LIBRARYSERVICE_H

#include <vector>
#include <string>
#include <unordered_map>
#include "../models/Models.h"
#include "PriorityQueue.h"

using namespace std;

// Class xu ly nghiep vu va thuat toan quan ly thu vien
class LibraryService {
private:
    vector<TaiLieu> dsSach;         // Danh sach tai lieu
    vector<DocGia> dsDocGia;        // Danh sach doc gia
    vector<PhieuMuon> dsPhieuMuon;  // Danh sach phieu muon
    vector<WaitlistEntry> dsChoMuon;// Danh sach cho muon
    unordered_map<std::string, DocGia> dgMap;
public:
    LibraryService();

    // Nap du lieu ban dau vao he thong
    void napDuLieu(const vector<TaiLieu>& s, const vector<DocGia>& dg,
        const vector<PhieuMuon>& pm, const vector<WaitlistEntry> dscm);

    // [MC1] Tim kiem tai lieu theo ma (tim kiem tuyen tinh)
    TaiLieu* timTheoMa(const string& ma);

    // [MC2] Lay top K tai lieu co luot muon cao nhat (sap xep giam dan theo luot muon)
    vector<TaiLieu> layTopMuonNhieuNhat(int k);

    // [RQ1] Loc danh sach tai lieu theo the loai (khong phan biet hoa/thuong)
    vector<TaiLieu> timTheoTheLoai(const string& theLoai);

    // [RQ2] Kiem tra va phan loai phieu muon theo han tra (dang muon / qua han)
    void kiemTraQuaHan(const string& ngayHienTai, vector<PhieuMuon>& dsDangMuon, vector<PhieuMuon>& dsQuaHan);

    // [RQ3] Tim kiem tai lieu theo tu khoa ten sach (khong phan biet hoa/thuong)
    vector<TaiLieu> timTheoTen(const string& tuKhoa);

    // Lay toan bo danh sach sach hien co
    vector<TaiLieu> layTatCaSach() const;

    //Ham phu: tim doc gia theo maDG
    void napDanhSachDocGia(const vector<DocGia>& dsDocGia);
    DocGia* timDocGiaTheoMa(const string& maDG);
    vector<DGChoMuon> locDuLieu(const string& maTL, const vector<WaitlistEntry>& dsChoMuon, const vector<DocGia>& dsDocGia);
    
    // Lay danh sach doc gia cho muon tai lieu theo ma tai lieu (Thuat toan thuong_toi uu)
    vector<DGChoMuon> layDanhSachChoTheoMaTL_Thuong(const string& maTL, const vector<WaitlistEntry>& dsChoMuon, const vector<DocGia>& dsDocGia);
    PriorityQueue layDanhSachChoTheoMaTL(const string& maTL, const vector<WaitlistEntry>& dsChoMuon, const vector<DocGia>& dsDocGia);
};

#endif // LIBRARYSERVICE_H
