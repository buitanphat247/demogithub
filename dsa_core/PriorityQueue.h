#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
#include <string>
#include "../models/Models.h"

/*
Struct phụ: chứa các dữ liệu hỗ trợ sắp xếp độ ưu tiên trong
hangCho và lưu các dữ liệu cần hiển thị trong một danh sách
hàng chờ ở đầu ra
*/
struct DGChoMuon{
    string maDG;        // Truy xuất từ readers.json
    string hoTen;       // Truy xuất từ readers.json
    string sdt;         // Truy xuất từ readers.json
    string loaiThe;     // Truy xuất từ readers.json
    string thoiGian;    // Truy xuất từ waitlist.json
    int doUuTien;       // Truy xuất từ waitlist.json_Độ ưu tiên theo thời gian trong một ngày

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

    void EnQueue(DGChoMuon &dg);
    DGChoMuon DeQueue();
    bool isEmpty() const;
    int getSize();
    void hienThi() const;
};

#endif