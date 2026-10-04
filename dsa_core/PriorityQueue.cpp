#include "PriorityQueue.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <cctype>
using namespace std;

PriorityQueue::PriorityQueue(int first_capacity) {
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

// So sanh thu tu uu tien cua hai doc gia
bool PriorityQueue::uuTienHon(const DGChoMuon& dg1, const DGChoMuon& dg2) const {
    int loaiThe_dg1 = dg1.doUuTienThe();
    int loaiThe_dg2 = dg2.doUuTienThe();
    /*Xet do uu tien:
    + Xep theo loai the: VIP > Student > Normal
    + Neu cung loai the thi uu tien ngay dang ky som hon
    + Neu cung loai the va cung ngay thi uu tien doc gia
    co do uu tien nho hon*/
    if (loaiThe_dg1 != loaiThe_dg2) {
        return (loaiThe_dg1 < loaiThe_dg2);
    }
    else {
        string ngaydk_dg1 = dg1.thoiGian.substr(0, 10);
        string ngaydk_dg2 = dg2.thoiGian.substr(0, 10);
        if (ngaydk_dg1 == ngaydk_dg2) {
            return dg1.doUuTien < dg2.doUuTien;
        }
        return ngaydk_dg1 < ngaydk_dg2;
    }
}

// Di chuyen phan tu len tren neu uu tien hon cha
void PriorityQueue::heapifyUp(int idx) {
    while (idx > 0) {
        int cha = (idx - 1)/2;

        if (uuTienHon(hangCho[idx], hangCho[cha])) {
            swap(hangCho[idx], hangCho[cha]);
            idx = cha;
        }
        else break;
    }
}

// Di chuyen phan tu xuong duoi neu cha uu tien hon
void PriorityQueue::heapifyDown(int idx) {
    while (true) {
        int left = 2 * idx + 1;
        int right = 2 * idx + 2;
        int cha = idx;

        if (left < size && uuTienHon(hangCho[left], hangCho[cha])) {
            cha = left;
        }
        if (right < size && uuTienHon(hangCho[right], hangCho[cha])) {
            cha = right;
        }

        if (cha != idx) {
            swap(hangCho[idx], hangCho[cha]);
            idx = cha;
        }
        else break;
    }
}

// Them 1 doc gia vao hang doi
void PriorityQueue::EnQueue(const DGChoMuon &dg) {
    if (size == capacity) {
        resize();
    }
    hangCho[size] = dg;
    heapifyUp(size);
    size++;
}

// Lay phan tu goc (cha) va xoa phan tu do ra khoi hang doi
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

// In danh sach cho muon ra man hinh
void PriorityQueue::hienThi() const {
    if (isEmpty()) {
        cout << "Hang cho rong!\n";
        return;
    }

    PriorityQueue tmp = *this;
    cout << string(100, '=') << endl;

    // 1. IN TIÊU ĐỀ CÁC CỘT (Căn trái và cố định kích thước từng cột)
    cout << left 
         << setw(6)  << "STT"
         << setw(12) << "Ma Doc Gia"
         << setw(20) << "Ho Va Ten"
         << setw(15) << "So Dien Thoai"
         << setw(12) << "Loai The"
         << setw(20) << "Thoi Gian" << endl;
    cout << string(100, '=') << endl;

    int stt = 1;
    while (!tmp.isEmpty()) {
        DGChoMuon dg = tmp.DeQueue();

        cout << left 
             << setw(6)  << stt++               
             << setw(12) << dg.maDG
             << setw(30) << dg.hoTen
             << setw(15) << dg.sdt
             << setw(12) << dg.loaiThe
             << setw(20) << dg.thoiGian << endl;
    }
    cout << string(100, '=') << endl;
}