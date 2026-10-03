#include <iostream>
#include <string>
#include <vector>
#include <algorithm> 

using namespace std;


// MC2 MAX-HEAP 

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


// RQ2 CÂY CÂN BẰNG AVL 

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
    NodeAVL* xoayPhai(NodeAVL* y) {
        NodeAVL* x = y->trai;
        NodeAVL* T2 = x->phai;
        x->phai = y;
        y->trai = T2;
        y->chieuCao = max(layChieuCao(y->trai), layChieuCao(y->phai)) + 1;
        x->chieuCao = max(layChieuCao(x->trai), layChieuCao(x->phai)) + 1;
        return x;
    }
    NodeAVL* xoayTrai(NodeAVL* x) {
        NodeAVL* y = x->phai;
        NodeAVL* T2 = y->trai;
        y->trai = x;
        x->phai = T2;
        x->chieuCao = max(layChieuCao(x->trai), layChieuCao(x->phai)) + 1;
        y->chieuCao = max(layChieuCao(y->trai), layChieuCao(y->phai)) + 1;
        return y;
    }
    NodeAVL* themPhieu(NodeAVL* node, PhieuMuon pm) {
        if (!node) return new NodeAVL(pm);

        if (pm.hanTra < node->data.hanTra) {
            node->trai = themPhieu(node->trai, pm);
        }
        else {
            node->phai = themPhieu(node->phai, pm);
        }
        node->chieuCao = 1 + max(layChieuCao(node->trai), layChieuCao(node->phai));
        int doChenh = layDoChenhLech(node);

        // Xoay cân bằng cây AVL
        if (doChenh > 1 && pm.hanTra < node->trai->data.hanTra)
            return xoayPhai(node);
        if (doChenh < -1 && pm.hanTra >= node->phai->data.hanTra)
            return xoayTrai(node);
        if (doChenh > 1 && pm.hanTra >= node->trai->data.hanTra) {
            node->trai = xoayTrai(node->trai);
            return xoayPhai(node);
        }
        if (doChenh < -1 && pm.hanTra < node->phai->data.hanTra) {
            node->phai = xoayTrai(node->phai);
            return xoayTrai(node);
        }

        return node;
    }
    void duyetInOrder(NodeAVL* node) {
        if (node) {
            duyetInOrder(node->trai);
            cout << " Ma PM: " << node->data.maPhieu
                << " | Doc gia: " << node->data.maDocGia
                << " | Ma sach: " << node->data.maTaiLieu
                << " | Han tra: " << node->data.hanTra << endl;
            duyetInOrder(node->phai);
        }
    }
    void inQuaHan(NodeAVL* node, string ngayHienTai) {
        if (!node) return;
        inQuaHan(node->trai, ngayHienTai);
        if (node->data.hanTra < ngayHienTai) {
            cout << " QUA HAN: " << node->data.maPhieu
                << " | Doc gia: " << node->data.maDocGia
                << " | Han tra: " << node->data.hanTra << endl;
            inQuaHan(node->phai, ngayHienTai);
        }
    }

public:
    void them(PhieuMuon pm) {
        goc = themPhieu(goc, pm);
    }

    void inDanhSachDangMuon() {
        cout << " DANH SACH PHIEU DANG MUON (XEP THEO HAN TRA) \n";
        duyetInOrder(goc);
    }

    void inDanhSachQuaHan(string ngayHienTai) {
        cout << " DANH SACH PHIEU QUA HAN TINH DEN NGAY " << ngayHienTai << " \n";
        inQuaHan(goc, ngayHienTai);
    }
};

// HÀM MAIN 

int main() {
    
    // 1. TEST MC2 
    
    cout << "\n";
    cout << "1. KET QUA TEST MC2 (MAX-HEAP)\n";
    cout << "\n";
    MaxHeap heap;

    // Nạp 8 sách với số lượt mượn ngẫu nhiên
    heap.capNhatLuotMuon("TL001", 12);
    heap.capNhatLuotMuon("TL002", 25);
    heap.capNhatLuotMuon("TL003", 8);
    heap.capNhatLuotMuon("TL004", 30); // Đang đứng Top 1 (30 lượt)
    heap.capNhatLuotMuon("TL005", 15);
    heap.capNhatLuotMuon("TL006", 5);
    heap.capNhatLuotMuon("TL007", 22);
    heap.capNhatLuotMuon("TL008", 18);

    cout << "-> Tim Top 1 ban dau:\n";
    heap.inTop1(); // Kết quả kỳ vọng: TL004 (30 lượt)

    cout << "\n[Cap nhat] Sach TL001 duoc muon nhieu dot ngot, tang len 50 luot...\n";
    heap.capNhatLuotMuon("TL001", 50);

    cout << "-> Tim Top 1 sau khi cap nhat:\n";
    heap.inTop1(); // Kết quả kỳ vọng: TL001 (50 lượt)


    
    // 2. TEST RQ2 
    
    cout << "\n=========================================\n";
    cout << "2. KET QUA TEST RQ2 (CAY CAN BANG AVL)\n";
    cout << "=========================================\n";
    CayAVL cay;

    // Nạp 8 phiếu mượn (được chèn theo thứ tự ngày lộn xộn để test tự cân bằng)
    cay.them({ "PM01", "DG01", "TL001", "2026-09-20" });
    cay.them({ "PM02", "DG02", "TL002", "2026-10-05" });
    cay.them({ "PM03", "DG03", "TL004", "2026-09-15" });
    cay.them({ "PM04", "DG04", "TL005", "2026-09-28" });
    cay.them({ "PM05", "DG01", "TL003", "2026-10-01" });
    cay.them({ "PM06", "DG05", "TL007", "2026-09-18" });
    cay.them({ "PM07", "DG06", "TL008", "2026-10-10" });
    cay.them({ "PM08", "DG07", "TL006", "2026-09-25" });

    // In toàn bộ phiếu mượn (Tự động in theo thứ tự ngày tăng dần)
    cay.inDanhSachDangMuon();

    // Lọc các phiếu quá hạn tính đến mốc ngày 2026-09-30
    cout << endl;
    cay.inDanhSachQuaHan("2026-09-30");

    return 0;
}