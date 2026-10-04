#include <iostream>
#include <vector>
#include <string>

// Chỉ cần include Models và PriorityQueue
#include "../models/Models.h"
#include "PriorityQueue.h"
#include "LibraryService.h"
#include "../persistence/FileStore.h"

using namespace std;

int main() {
    cout << "=== TEST ===" << endl << endl;
    
    vector<TaiLieu> dsSach = {};
    
    vector<DocGia> dsDocGia = docDanhSachDocGia();

    vector<PhieuMuon> dsPhieu = {};

    vector<WaitlistEntry> dsChoMuon = docDanhSachHangCho();  

    LibraryService service;
    service.napDuLieu(dsSach, dsDocGia, dsPhieu, dsChoMuon);
    cout << "DA NAP XONG\n\n";
    string maTLTest = "TL000015";

    vector<DGChoMuon> h = service.locDuLieu(maTLTest, dsChoMuon, dsDocGia);
    PriorityQueue q = service.layDanhSachChoTheoMaTL(h);

    if (q.isEmpty()) {
        cout << "[!] Ma tai lieu "<< maTLTest << " chua co danh sach doc gia cho muon!" << endl;
    } else {
        q.hienThi();
    }

    return 0;
}