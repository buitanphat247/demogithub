#include "PriorityQueue.h"
#include <iostream>
#include <string>
using namespace std;

PriorityQueue::PriorityQueue(int first_capacity = 300000) {
    capacity = first_capacity;
    hangCho = new DGChoMuon[capacity];
    size = 0;
}

// Sao chep hang doi
PriorityQueue::PriorityQueue(const PriorityQueue& tmp) {
    this->capacity = tmp.capacity;
    this->size = tmp.size;

    this->hangCho = new DGChoMuon[this->capacity];
    for (int i = 0; i < this->size; i++) {
        this->hangCho[i] = tmp.hangCho[i];
    }
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

// So sánh thứ tự ưu tiên của hai độc giả 
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

// Di chuyen phan tu len tren neu uu tien hon cha
void PriorityQueue::heapifyUp(int idx) {
    while (idx > 0) {
        int cha = (idx - 1)/2;

        if (uuTienHon(hangCho[idx], hangCho[cha])) {
            DGChoMuon tmp = hangCho[idx];
            hangCho[idx] = hangCho[cha];
            hangCho[cha] = tmp;

            idx = cha;
        }
        else break;
    }
}

void PriorityQueue::heapifyDown(int idx) {
    while (true) {
        int left = 2 * idx + 1;
        int right = 2 * idx + 1;
        int cha = idx;

        if (left < size && uuTienHon(hangCho[left], hangCho[cha])) {
            cha = left;
        }
        if (right < size && uuTienHon(hangCho[right], hangCho[cha])) {
            cha = right;
        }

        if (cha != idx) {
            DGChoMuon tmp = hangCho[idx];
            hangCho[idx] = hangCho[cha];
            hangCho[cha] = tmp;

            idx = cha;
        }
        else break;
    }
}

// Thêm 1 độc giả vào hàng đợi
void PriorityQueue::EnQueue(DGChoMuon &dg) {
    if (size == capacity) {
        resize();
    }
    hangCho[size] = dg;
    heapifyUp(size);
    size++;
}

DGChoMuon PriorityQueue::DeQueue() {
    if (isEmpty()) {
        throw runtime_error("Hang cho dang rong!");
    }

    DGChoMuon goc = hangCho[0];
    hangCho[0] = hangCho[size-1];
    size--;

    if (size > 0) {
        heapifyDown(0);
    }
    return goc;
}

bool PriorityQueue::isEmpty() const{
    return (size == 0);
}

int PriorityQueue::getSize() {
    return size;
}

// In danh sách chờ mượn ra màn hình
void PriorityQueue::hienThi() const {
    if (isEmpty()) {
        cout << "Hang cho rong!\n";
        return;
    }

    PriorityQueue tmp = *this;
    cout << "=====================================================================\n";
    cout << "STT\tMa Doc Gia\tHo Va Ten\tSo Dien Thoai\tLoai The\tThoi Gian\n";
    cout << "=====================================================================\n";
    int stt = 1;
    while (!tmp.isEmpty()) {
        DGChoMuon dg = tmp.DeQueue();
        cout << stt << "\t"
             << dg.maDG << "\t"
             << dg.hoTen << "\t"
             << dg.sdt << "\t"
             << dg.loaiThe << "\t"
             << dg.thoiGian << "\n";
    }
    cout << "=====================================================================\n";
}