#include "PriorityQueue.h"
#include <iostream>
#include <string>
using namespace std;

PriorityQueue::PriorityQueue(int first_capacity = 300000) {
    capacity = first_capacity;
    hangCho = new DGChoMuon[capacity];
    size = 0;
    front = 0;
    rear = -1;
}

PriorityQueue::~PriorityQueue() {
    delete[] hangCho;
}

void PriorityQueue::resize() {
    capacity *= 2;
    DGChoMuon* tmp = new DGChoMuon[capacity];
    for (int i = 0; i < size; i++) {
        tmp[i] = hangCho[i];
    }
    delete[] hangCho;
    hangCho = tmp;
}

bool PriorityQueue::uuTienHon(const DGChoMuon& dg1, const DGChoMuon& dg2) const {
    int loaiThe_dg1 = dg1.doUuTienThe();
    int loaiThe_dg2 = dg2.doUuTienThe();
    /*Xét độ ưu tiên:
    + Xếp theo loại thẻ: VIP > Student > Normal
    + Nếu cùng loại thẻ thì ưu tiên ngày đăng ký sớm hơn
    + Nếu cùng loại thẻ và cùng ngày đăng ký thì ưu tiên
    đọc giả có độ ưu tiên nhỏ hơn*/
    if (loaiThe_dg1 != loaiThe_dg2) {
        return (loaiThe_dg1 < loaiThe_dg2);
    }
    else {
        string ngaydk_dg1 = dg1.thoiGian.substr(0, 10);
        string ngaydk_dg2 = dg2.thoiGian.substr(0, 10);
        if (ngaydk_dg1 == ngaydk_dg2) {
            return (dg1.doUuTien < dg2.doUuTien);
        }
        else {
            return (ngaydk_dg1 < ngaydk_dg2);
        }
    }
}

void PriorityQueue::EnQueue(DGChoMuon &dg) {
    if (size == capacity) {
        resize();
    }
    else {
        int i = size - 1;
        while (i >= 0 && uuTienHon(dg, hangCho[i])) {
            hangCho[i+1] = hangCho[i];
            i--;
        }
        hangCho[i+1] = dg;
        size++;
    }
}

void PriorityQueue::hienThi() const {
    cout << "=====================================================================\n";
    cout << "STT\tMa Doc Gia\tHo Va Ten\tSo Dien Thoai\tLoai The\tThoi Gian\n";
    cout << "=====================================================================\n";
    for (int i = size-1; i >= 0; i--) {
        cout << size-i << "\t"
             << hangCho[i].maDG << "\t"
             << hangCho[i].hoTen << "\t"
             << hangCho[i].sdt << "\t"
             << hangCho[i].loaiThe << "\t"
             << hangCho[i].thoiGian << "\n";
    }
    cout << "=====================================================================\n";
}