#ifndef QUEUE_H
#define QUEUE_H

#include <string>
#include "../models/Models.h"

/*
Struct phụ: chứa các dữ liệu hỗ trợ sắp xếp độ ưu tiên trong
hangCho và lưu các dữ liệu cần hiển thị trong một danh sách
hàng chờ ở đầu ra
*/
struct DGChoMuon{
    string maDG;
    string hoTen;
    string sdt;
    string loaiThe;
    string thoiGian;
    int doUuTien;       // Độ ưu tiên theo thời gian trong một ngày

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
    int front;
    int rear;

    void resize();
    bool uuTienHon(const DGChoMuon& dg1, const DGChoMuon& dg2) const;

public:
    PriorityQueue(int first_capacity = 300000);
    ~PriorityQueue();

    void EnQueue(DGChoMuon &dg);
    void hienThi() const;
};

#endif