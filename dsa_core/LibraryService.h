#ifndef LIBRARYSERVICE_H
#define LIBRARYSERVICE_H

#include <vector>
#include <string>
#include "../models/Models.h"

using namespace std;

class LibraryService {
private:
    vector<TaiLieu> dsSach;
    vector<DocGia> dsDocGia;
    vector<PhieuMuon> dsPhieuMuon;

public:
    LibraryService();

    void napDuLieu(const vector<TaiLieu>& s, const vector<DocGia>& dg, const vector<PhieuMuon>& pm);

    TaiLieu* timTheoMa(const string& ma);

    vector<TaiLieu> layTopMuonNhieuNhat(int k);

    vector<TaiLieu> timTheoTheLoai(const string& theLoai);

    void kiemTraQuaHan(const string& ngayHienTai, vector<PhieuMuon>& dsDangMuon, vector<PhieuMuon>& dsQuaHan);

    vector<TaiLieu> timTheoTen(const string& tuKhoa);

    vector<TaiLieu> layTatCaSach() const;
};

#endif
