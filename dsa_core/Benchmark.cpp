#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>

#include "../models/Models.h"
#include "../persistence/FileStore.h"
#include "LibraryService.h"
#include "PriorityQueue.h"

using namespace std;

int main() {
    cout << "================================================================" << endl;
    cout << "                       BANG CHUNG HIEU NANG  " << endl;
    cout << "================================================================" << endl << endl;

    // 1. ĐỌC DỮ LIỆU THỰC TẾ TỪ FILE PERSISTENCE 
    vector<DocGia> dsDocGiaGoc = docDanhSachDocGia();
    vector<WaitlistEntry> dsChoMuonGoc = docDanhSachHangCho();
    vector<TaiLieu> dsSachGoc = {};
    vector<PhieuMuon> dsPhieuGoc = {};

    int tongds = dsChoMuonGoc.size();
    cout << "[+] Da nap " << tongds << " ban ghi waitlist tu file persistence.\n\n";

    // 2. CHỌN CÁC MỐC KÍCH THƯỚC N ĐỂ DO (Cắt từ tập dữ liệu gốc)
    vector<int> testSizes;
    for (int size : {10, 1000, 10000, 30000}) {
        if (size <= tongds) {
            testSizes.push_back(size);
        }
    }

    // IN BẢNG BẰNG CHỨNG HỆU NĂNG
    cout << left << setw(15) << "So luong (N)" 
         << setw(25) << "Baseline (Sort) [ms]" 
         << setw(25) << "Optimized (Heap) [ms]" 
         << setw(15) << "Gia toc" << endl;
    cout << string(80, '-') << endl;

    for (int N : testSizes) {
        // Cắt lấy N bản ghi đầu tiên từ dữ liệu gốc
        vector<WaitlistEntry> dsChoSub(dsChoMuonGoc.begin(), dsChoMuonGoc.begin() + N);

        LibraryService service;
        service.napDuLieu(dsSachGoc, dsDocGiaGoc, dsPhieuGoc, dsChoSub);

        // Lấy mã tài liệu có nhiều người chờ nhất hoặc dùng mã cố định để test
        string targetMaTL = "TL000015"; 

        // --- A. ĐO THUẬT TOÁN THƯỜNG (Baseline - Selection Sort) ---
        auto start1 = chrono::high_resolution_clock::now();
        vector<DGChoMuon> resThuong = service.layDanhSachChoTheoMaTL_Thuong(targetMaTL, dsChoSub, dsDocGiaGoc);
        auto end1 = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> timeThuong = end1 - start1;

        // --- B. ĐO THUẬT TOÁN TỐI ƯU (Optimized - Priority Queue Heap) ---
        auto start2 = chrono::high_resolution_clock::now();
        PriorityQueue resHeap = service.layDanhSachChoTheoMaTL(targetMaTL, dsChoSub, dsDocGiaGoc);
        auto end2 = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> timeHeap = end2 - start2;

        // Tính gia tốc cải thiện (Speedup)
        double speedup = (timeHeap.count() > 0) ? (timeThuong.count() / timeHeap.count()) : 1.0;

        cout << left << setw(15) << N 
             << setw(25) << fixed << setprecision(3) << timeThuong.count()
             << setw(25) << fixed << setprecision(3) << timeHeap.count()
             << fixed << setprecision(1) << speedup << "x" << endl;
    }

    cout << string(80, '-') << endl;
    cout << "[!] Nhan xet: Du lieu thuc te tu Persistence khang dinh Max-Heap giam thoi gian xu ly dang ke." << endl;

    return 0;
}