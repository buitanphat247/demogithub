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

      // Thêm hoặc cập nhật số lượt mượn của tài liệu
    void capNhatLuotMuon(string ma, int luotMuonMoi) {
        int viTri = -1;
        for (int i = 0; i < (int)heap.size(); i++) {
            if (heap[i].maSach == ma) {
                viTri = i;
                break;
            }
        }
        if (viTri != -1) {
            // Nếu tài liệu đã có Cập nhật lượt mượn mới
            heap[viTri].soLuotMuon = luotMuonMoi;
            vunUp(viTri);
        }
        else {
            // Nếu chưa có Thêm mới vào cuối Heap
            heap.push_back({ ma, luotMuonMoi });
            vunUp((int)heap.size() - 1);
        }
    }
    // Lấy tài liệu được mượn nhiều nhất O(1)
    void inTop1() {
        if (!heap.empty()) {
            cout << "=> Sach duoc muon nhieu nhat: " << heap[0].maSach
                << " (" << heap[0].soLuotMuon << " luot muon)\n";
        }
        else {
            cout << "Chua co du lieu trong Heap.\n";
        }
    }
};
//RQ2 CÂY CÂN BẰNG AVL
struct PhieuMuon {
    string maPhieu;
    string maDocGia;
    string maTaiLieu;
    string hanTra; 
};

struct NodeAVL {
    PhieuMuon data;
    NodeAVL* trai = nullptr;
    NodeAVL* phai = nullptr;
    int chieuCao = 1;
    NodeAVL(PhieuMuon pm) : data(pm) {}
};

class CayAVL {
private:
    NodeAVL* goc = nullptr;
    int layChieuCao(NodeAVL* node) {
        return node ? node->chieuCao : 0;
    }
    int layDoChenhLech(NodeAVL* node) {
        return node ? layChieuCao(node->trai) - layChieuCao(node->phai) : 0;
    }