#include <iostream>
#include <string>
#include <vector>
#include <algorithm> 

using namespace std;
struct TaiLieu {
    string maSach;
    int soLuotMuon;
};

class MaxHeap {
public:
    vector<TaiLieu> heap;
    // Vun đống từ dưới lên khi thêm/cập nhật
    void vunUp(int idx) {
        while (idx > 0) {
            int cha = (idx - 1) / 2;
            if (heap[idx].soLuotMuon > heap[cha].soLuotMuon) {
                swap(heap[idx], heap[cha]);
                idx = cha;
            }
            else {
                break;
            }
        }
    }