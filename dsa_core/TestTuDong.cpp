#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <fstream>
#include <iomanip>

#include "../models/Models.h"
#include "LibraryService.h"
#include "PriorityQueue.h"

using namespace std;

int main() {
    // 1. Du lieu gia lap
    vector<TaiLieu> dsSach = {};
    
    vector<DocGia> dsDocGia = {
        {"DG01", "Nguyen Van A", "0901234567", "Student"},
        {"DG02", "Tran Thi B",   "0912345678", "VIP"},
        {"DG03", "Le Van C",     "0923456789", "Student"},
        {"DG04", "Pham Van D",   "0934567890", "VIP"},
        {"DG05", "Huynh Thi E",  "0900567894", "Normal"},
        {"DG06", "Phan Le Vu",   "0999967892", "Normal"}
    };

    vector<PhieuMuon> dsPhieu = {};

    vector<WaitlistEntry> dsChoMuon = {
        {"TL01", "DG01", 1, ""}, // Student
        {"TL01", "DG02", 3, "2026-09-15 09:00:00"}, // VIP
        {"TL01", "DG03", 2, "2026-09-15 08:30:00"}, // Student
        {"TL01", "DG04", 1, "2026-09-15 7:40:00"},  // VIP
        {"TL01", "DG05", 1, "2026-09-16 8:50:00"},   // Normal
        {"TL01", "DG06", 1, "2026-09-14 13:50:00"}   // Normal
    };
    // Output ky vong: DG04, DG02, DG03, DG06, DG05

    LibraryService service;
    service.napDuLieu(dsSach, dsDocGia, dsPhieu, dsChoMuon);
    
    PriorityQueue q = service.layDanhSachChoTheoMaTL("TL01", dsChoMuon,dsDocGia);
    PriorityQueue tmp = q;
    string kq[6];
    int i = 0;
    while (!tmp.isEmpty()) {
        DGChoMuon r = tmp.DeQueue();
        kq[i] = r.maDG;
        i++;
    }
    int tongPass = 0;

    cout << "== TEST TU DONG ==\n\n";
    cout << "TH1: Cung loai the, cung ngay\n";  // DG02, DG04
    if (kq[0] == "DG04") {
        cout << "[PASS]\n";
        tongPass++;
    }
    else cout << "[FAIL]\n";

    cout << "TH2: Khac loai the, cung ngay\n";  // DG02, DG03, DG04
    if (kq[2] == "DG03") {
        cout << "[PASS]\n";
        tongPass++;
    }
    else cout << "[FAIL]\n";

    cout << "TH3: Cung loai the, cung do uu tien, khac ngay\n"; // DG05, DG06
    if (kq[3] == "DG06") {
        cout << "[PASS]\n";
        tongPass++;
    }
    else cout << "[FAIL]\n";

    cout << "TH4: Loi thoi gian rong\n";    // DG01
    if (kq[5] == "DG01") {
        cout << "[PASS]\n";
        tongPass++;
    }
    else cout << "[FAIL]\n";

    cout << "TH5: Khong tim thay tai lieu\n";
    PriorityQueue r = service.layDanhSachChoTheoMaTL("TL02", dsChoMuon,dsDocGia);
    if (r.isEmpty()) {
        cout << "[PASS]\n";
        tongPass++;
    }
    else cout << "[FAIL]\n";

    cout << "KET QUA: Code da pass " << tongPass << " /5 truong hop\n\n"; 

    return 0;
}