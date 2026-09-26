#ifndef WAITLISTENTRY_H
#define WAITLISTENTRY_H

#include <string>

using namespace std;

// Cau truc thong tin hang cho (khi sach het trong kho)
struct WaitlistEntry {
    string maTL;       // Ma tai lieu dang cho
    string maDG;       // Ma doc gia dang cho
    int doUuTien;      // Muc do uu tien (so cang lon uu tien cang cao)
    string thoiGian;   // Thoi diem vao hang cho (YYYY-MM-DD HH:MM:SS)
};

#endif // WAITLISTENTRY_H
