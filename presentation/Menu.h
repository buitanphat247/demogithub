#ifndef MENU_H
#define MENU_H

#include <vector>
#include <string>
#include "../models/Models.h"
#include "../dsa_core/LibraryService.h"

using namespace std;

void chayMenu(LibraryService& service);

void inKeNgang(int doDai = 85);
void inTieuDeMenu();
void inBangSach(const vector<TaiLieu>& ds);
void inBangPhieu(const vector<PhieuMuon>& ds, const string& tieuDe);
void dungManHinh();

#endif
