#ifndef MENU_H
#define MENU_H

#include <vector>
#include <string>
#include "../models/Models.h"
#include "../dsa_core/LibraryService.h"

using namespace std;

// Vong lap dieu khien menu chinh
void chayMenu(LibraryService& service);

// Xoa man hinh terminal
void xoaManHinh();

// Doc mot phim tu ban phim (mui ten, Enter, ESC)
int docPhim();

// In duong ke ngang trang tri khung
void inKeNgang(int doDai = 87);

// In menu chinh voi con tro lua chon
void inTieuDeMenu(int viTriHienTai = 0);

// [MC1] Tra cuu va chon tai lieu bang phim mui ten
void traCuuTaiLieuTuongTac(const vector<TaiLieu>& dsGoc);

// In bang danh sach sach
void inBangSach(const vector<TaiLieu>& ds);

// In bang danh sach phieu muon
void inBangPhieu(const vector<PhieuMuon>& ds, const string& tieuDe);

// Tam dung man hinh cho nguoi dung xem ket qua
void dungManHinh();

#endif // MENU_H
