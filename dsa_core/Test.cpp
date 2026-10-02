#include <iostream>
#include <vector>
#include <string>

// Chỉ cần include Models và PriorityQueue
#include "../models/Models.h"
#include "PriorityQueue.h"

using namespace std;

// =================================================================
// 1. CÁC HÀM HỖ TRỢ VIẾT TRỰC TIẾP Ở ĐẦU FILE TEST
// =================================================================

// Hàm phụ trợ: Tìm độc giả theo maDG trong vector
const DocGia* timDocGiaTheoMa(const string& maDG, const vector<DocGia>& dsDocGia) {
    for (const auto& dg : dsDocGia) {
        if (dg.maDG == maDG) {
            return &dg;
        }
    }
    return nullptr;
}

// Hàm lọc và nạp dữ liệu vào PriorityQueue
PriorityQueue layDanhSachChoTheoMaTL(const string& maTL, 
                                     const vector<WaitlistEntry>& dsChoMuon, 
                                     const vector<DocGia>& dsDocGia) {
    PriorityQueue waitlistQueue;

    for (const auto& entry : dsChoMuon) {
        if (entry.maTL == maTL) {
            const DocGia* reader = timDocGiaTheoMa(entry.maDG, dsDocGia);

            if (reader != nullptr) {
                DGChoMuon node;
                node.maDG = reader->maDG;
                node.hoTen = reader->hoTen;
                node.sdt = reader->sdt;
                node.loaiThe = reader->loaiThe;

                node.thoiGian = entry.thoiGian;
                node.doUuTien = entry.doUuTien;

                waitlistQueue.EnQueue(node);
            }
        }
    }

    return waitlistQueue;
}

// =================================================================
// 2. HÀM MAIN CHẠY TEST
// =================================================================
int main() {
    cout << "=== TEST ===" << endl << endl;

    // Dữ liệu giả lập 1: Độc giả (tương đương readers.json)
    vector<DocGia> dsDocGia = {
        {"DG01", "Nguyen Van A", "0901234567", "Normal"},
        {"DG02", "Tran Thi B",   "0912345678", "VIP"},
        {"DG03", "Le Van C",     "0923456789", "Student"},
        {"DG04", "Pham Van D",   "0934567890", "VIP"}
    };

    // Dữ liệu giả lập 2: Hàng chờ (tương đương waitlist.json)
    vector<WaitlistEntry> dsChoMuon = {
        {"TL01", "DG01", 3, "2026-09-15 08:00:00"}, // Normal - đăng ký trước
        {"TL01", "DG02", 1, "2026-09-15 09:00:00"}, // VIP - đăng ký sau DG01
        {"TL02", "DG03", 2, "2026-09-15 08:30:00"}, // Sách TL02 khác
        {"TL01", "DG04", 1, "2026-09-15 10:00:00"}  // VIP - đăng ký sau DG02
    };

    // Thử nghiệm lọc tài liệu "TL01"
    string maTLTest = "TL51";
    cout << "--> Dang loc va sap xep cho ma sach: " << maTLTest << endl << endl;

    PriorityQueue q = layDanhSachChoTheoMaTL(maTLTest, dsChoMuon, dsDocGia);

    if (q.isEmpty()) {
        cout << "[!] Ma tai lieu "<< maTLTest << " chua co danh sach doc gia cho muon!" << endl;
    } else {
        q.hienThi(); // In danh sách đã vun đống Max-Heap
    }

    return 0;
}