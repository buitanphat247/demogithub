#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
#include <string>
#include "../models/Models.h"

/*
Struct phu: chua cac du lieu ho tro sap xep do uu tien trong
hangCho va cac du lieu can hien thi trong danh sach hang cho
o dau ra
*/
struct DGChoMuon{
    string maDG;        // Truy xuat tu readers.json
    string hoTen;       // Truy xuat tu readers.json
    string sdt;         // Truy xuat tu readers.json
    string loaiThe;     // Truy xuat tu readers.json
    string thoiGian;    // Truy xuat tu waitlist.json
    int doUuTien;       // Truy xuat tu waitlist.json_Do uu tien theo thoi gian trong mot ngay

    int doUuTienThe() const {
        if (loaiThe == "VIP") return 1;
        else if (loaiThe == "Student") return 2;
        else return 3;
    }
};

class PriorityQueue
{
private:
    DGChoMuon* hangCho;
    int capacity;
    int size;

    void resize();
    bool uuTienHon(const DGChoMuon& dg1, const DGChoMuon& dg2) const;
    void heapifyUp(int idx);
    void heapifyDown(int idx);
public:
    PriorityQueue(int first_capacity = 300000);
    PriorityQueue(const PriorityQueue& tmp);
    ~PriorityQueue();

    void EnQueue(const DGChoMuon &dg);
    DGChoMuon DeQueue();
    bool isEmpty() const;
    int getSize();
    void hienThi() const;
};

#endif