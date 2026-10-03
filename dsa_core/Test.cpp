#include <iostream>
#include <vector>
#include <string>

// Chỉ cần include Models và PriorityQueue
#include "../models/Models.h"
#include "PriorityQueue.h"
#include "LibraryService.h"

using namespace std;

int main() {
    cout << "=== TEST ===" << endl << endl;
    
    vector<TaiLieu> dsSach = {{"TL01", "Giai Tich 1", "Nguyen Van A", "Toan", 2023, 0, 50}};
    
    vector<DocGia> dsDocGia = {
        {"DG01", "Nguyen Van A", "0901234567", "VIP"},
        {"DG02", "Tran Thi B",   "0912345678", "VIP"},
        {"DG03", "Le Van C",     "0923456789", "VIP"},
        {"DG04", "Pham Van D",   "0934567890", "VIP"}
    };

    vector<PhieuMuon> dsPhieu = {};

    vector<WaitlistEntry> dsChoMuon = {
        {"TL01", "DG01", 1, "2026-09-08 08:00:00"}, 
        {"TL01", "DG02", 1, "2026-09-07 09:00:00"},
        {"TL01", "DG03", 1, "2026-09-06 08:30:00"},
        {"TL01", "DG04", 1, "2026-09-05 10:00:00"}
    };  

    LibraryService service;
    service.napDuLieu(dsSach, dsDocGia, dsPhieu, dsChoMuon);
    string maTLTest = "TL01";

    PriorityQueue q = service.layDanhSachChoTheoMaTL(maTLTest, dsChoMuon, dsDocGia);

    if (q.isEmpty()) {
        cout << "[!] Ma tai lieu "<< maTLTest << " chua co danh sach doc gia cho muon!" << endl;
    } else {
        q.hienThi();
    }

    return 0;
}