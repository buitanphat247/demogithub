#ifndef QUEUE_H
#define QUEUE_H

#include <string>
#include "../models/Models.h"

/*
Struct phụ: chứa các dữ liệu hỗ trợ sắp xếp độ ưu tiên trong
hangCho và lưu các dữ liệu cần hiển thị trong một danh sách
hàng chờ ở đầu ra
*/
struct DSHangCho{
    string maDG;
    string hoTen;
    string sdt;
    string loaiThe;
    string thoiGian;
    int doUuTien;       // Độ ưu tiên theo thời gian trong một ngày

    int doUuTienThe() {
        if (loaiThe == "VIP") return 1;
        else if (loaiThe == "Student") return 2;
        else return 3;
    }
};

class PriorityQueue
{
private:
    WaitlistEntry* hangCho;
    int capacity;
    int size;
    int front;
    int rear;

    void resize();
    bool uuTienHon(const DSHangCho& a, const DSHangCho& b) const;

public:
    PriorityQueue(int first_capacity = 300000);
    ~PriorityQueue();

    void EnQueue(WaitlistEntry* &hangCho);
    void hienThi() const;
};

#endif