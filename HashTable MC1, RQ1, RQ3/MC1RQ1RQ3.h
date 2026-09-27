#ifndef HASH_TABLE_H
#define HASH_TABLE_H

/*
    PHỤ TRÁCH: BẢNG BĂM (MC1, RQ1, RQ3)
    ------------------------------------
    - MC1: Tra cứu chính xác tài liệu theo MÃ TÀI LIỆU (định danh duy nhất)
           -> Bảng băm chính: khóa = mã tài liệu, giá trị = con trỏ TaiLieu*
    - RQ1: Tra cứu tài liệu theo THỂ LOẠI
           -> Bảng băm phụ: khóa = thể loại, giá trị = danh sách con trỏ TaiLieu*
    - RQ3: Tra cứu / tìm kiếm tài liệu theo TÊN SÁCH (hoặc từ khóa gần đúng)
           -> Duyệt qua các nhóm trong bảng băm phụ (theo thể loại) rồi lọc
              chuỗi cục bộ (string matching) theo tên tài liệu.

    Cả hai bảng băm đều được TỰ CÀI ĐẶT bằng phương pháp separate chaining
    (mỗi ngăn/bucket là một danh sách liên kết các nút).

    File này chỉ chứa KHAI BÁO + ĐỊNH NGHĨA (header-only) để include vào
    dsa_core/ của dự án (ví dụ: hash_table.h trong cấu trúc thư mục đã đề xuất).
*/

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

// ============================================================
// 1. CẤU TRÚC DỮ LIỆU BẢN GHI TÀI LIỆU
// ============================================================
struct TaiLieu {
    string maTaiLieu;   // khóa chính - dùng cho MC1
    string tenTaiLieu;  // dùng cho RQ3
    string tacGia;
    string theLoai;     // khóa nhóm - dùng cho RQ1
    int namXuatBan;
    int soLuong;
    string tinhTrang;   // ví dụ: "Còn sách" / "Hết sách"

    TaiLieu() {}
    TaiLieu(string ma, string ten, string tg, string tl, int nam, int sl, string tt)
        : maTaiLieu(ma), tenTaiLieu(ten), tacGia(tg), theLoai(tl),
          namXuatBan(nam), soLuong(sl), tinhTrang(tt) {}
};

// Hàm chuyển chuỗi về chữ thường để so khớp không phân biệt hoa/thường
// (khai báo "inline" vì được định nghĩa trong header, tránh lỗi multiple definition)
inline string ChuoiThuong(const string& chuoi) {
    string ketQua = chuoi;
    transform(ketQua.begin(), ketQua.end(), ketQua.begin(),
              [](unsigned char c) { return tolower(c); });
    return ketQua;
}

// ============================================================
// 2. BẢNG BĂM CHÍNH (MC1) — khóa duy nhất -> 1 bản ghi
//    Tự cài đặt bằng separate chaining
// ============================================================
class BangBamMaTaiLieu {
private:
    struct Nut {
        string khoa;        // mã tài liệu
        TaiLieu* giaTri;    // con trỏ tới bản ghi tài liệu
        Nut* tiepTheo;
        Nut(string k, TaiLieu* v) : khoa(k), giaTri(v), tiepTheo(nullptr) {}
    };

    vector<Nut*> mangNgan;   // mảng các bucket
    int soNganBan;           // số lượng bucket
    int soPhanTu;            // số phần tử hiện có

    // Hàm băm chuỗi kiểu polynomial (djb2)
    unsigned long BamChuoi(const string& khoa) const {
        unsigned long ma = 5381;
        for (char c : khoa) ma = ma * 33 + (unsigned char)c;
        return ma;
    }

    int ViTriNgan(const string& khoa) const {
        return (int)(BamChuoi(khoa) % soNganBan);
    }

    // Băm lại khi hệ số tải quá cao (giữ average-case O(1))
    void BamLai() {
        int soNganMoi = soNganBan * 2;
        vector<Nut*> mangMoi(soNganMoi, nullptr);
        for (int i = 0; i < soNganBan; i++) {
            Nut* nut = mangNgan[i];
            while (nut != nullptr) {
                Nut* nutTiep = nut->tiepTheo;
                unsigned long viTriMoi = BamChuoi(nut->khoa) % soNganMoi;
                nut->tiepTheo = mangMoi[viTriMoi];
                mangMoi[viTriMoi] = nut;
                nut = nutTiep;
            }
        }
        mangNgan = mangMoi;
        soNganBan = soNganMoi;
    }

public:
    BangBamMaTaiLieu(int soNganBanDau = 101)
        : soNganBan(soNganBanDau), soPhanTu(0) {
        mangNgan.assign(soNganBan, nullptr);
    }

    ~BangBamMaTaiLieu() {
        for (int i = 0; i < soNganBan; i++) {
            Nut* nut = mangNgan[i];
            while (nut != nullptr) {
                Nut* nutXoa = nut;
                nut = nut->tiepTheo;
                delete nutXoa;
            }
        }
    }

    // Thêm mới hoặc cập nhật con trỏ ứng với 1 mã tài liệu — dùng cho nghiệp vụ Thêm/Sửa
    void ChenHoacCapNhat(const string& maTaiLieu, TaiLieu* banGhi) {
        double heSoTai = (double)(soPhanTu + 1) / soNganBan;
        if (heSoTai > 0.75) BamLai();

        int viTri = ViTriNgan(maTaiLieu);
        Nut* nut = mangNgan[viTri];
        while (nut != nullptr) {
            if (nut->khoa == maTaiLieu) { // đã tồn tại -> cập nhật
                nut->giaTri = banGhi;
                return;
            }
            nut = nut->tiepTheo;
        }
        // chưa có -> chèn đầu danh sách liên kết của ngăn
        Nut* nutMoi = new Nut(maTaiLieu, banGhi);
        nutMoi->tiepTheo = mangNgan[viTri];
        mangNgan[viTri] = nutMoi;
        soPhanTu++;
    }

    // MC1: Tra cứu chính xác theo mã tài liệu — average-case O(1)
    TaiLieu* TimTheoMa(const string& maTaiLieu) const {
        int viTri = ViTriNgan(maTaiLieu);
        Nut* nut = mangNgan[viTri];
        while (nut != nullptr) {
            if (nut->khoa == maTaiLieu) return nut->giaTri;
            nut = nut->tiepTheo;
        }
        return nullptr; // không tìm thấy
    }

    // Xóa 1 mã tài liệu khỏi bảng băm chính
    bool Xoa(const string& maTaiLieu) {
        int viTri = ViTriNgan(maTaiLieu);
        Nut* nut = mangNgan[viTri];
        Nut* nutTruoc = nullptr;
        while (nut != nullptr) {
            if (nut->khoa == maTaiLieu) {
                if (nutTruoc == nullptr) mangNgan[viTri] = nut->tiepTheo;
                else nutTruoc->tiepTheo = nut->tiepTheo;
                delete nut;
                soPhanTu--;
                return true;
            }
            nutTruoc = nut;
            nut = nut->tiepTheo;
        }
        return false;
    }

    // Lấy toàn bộ bản ghi hiện có (dùng để phục vụ RQ3 khi cần duyệt toàn tập)
    vector<TaiLieu*> LayTatCa() const {
        vector<TaiLieu*> ketQua;
        for (int i = 0; i < soNganBan; i++) {
            Nut* nut = mangNgan[i];
            while (nut != nullptr) {
                ketQua.push_back(nut->giaTri);
                nut = nut->tiepTheo;
            }
        }
        return ketQua;
    }

    int SoPhanTu() const { return soPhanTu; }
};

// ============================================================
// 3. BẢNG BĂM PHỤ (RQ1) — khóa = thể loại -> danh sách con trỏ
//    Tự cài đặt bằng separate chaining, mỗi nút chứa 1 nhóm (vector)
// ============================================================
class BangBamTheLoai {
private:
    struct Nut {
        string khoa;                    // thể loại
        vector<TaiLieu*> danhSach;      // nhóm các con trỏ tài liệu cùng thể loại
        Nut* tiepTheo;
        Nut(string k) : khoa(k), tiepTheo(nullptr) {}
    };

    vector<Nut*> mangNgan;
    int soNganBan;
    int soPhanTu; // số thể loại (khóa) khác nhau

    unsigned long BamChuoi(const string& khoa) const {
        unsigned long ma = 5381;
        for (char c : khoa) ma = ma * 33 + (unsigned char)c;
        return ma;
    }

    int ViTriNgan(const string& khoa) const {
        return (int)(BamChuoi(khoa) % soNganBan);
    }

    Nut* TimNut(const string& theLoai) const {
        int viTri = ViTriNgan(theLoai);
        Nut* nut = mangNgan[viTri];
        while (nut != nullptr) {
            if (nut->khoa == theLoai) return nut;
            nut = nut->tiepTheo;
        }
        return nullptr;
    }

public:
    BangBamTheLoai(int soNganBanDau = 31)
        : soNganBan(soNganBanDau), soPhanTu(0) {
        mangNgan.assign(soNganBan, nullptr);
    }

    ~BangBamTheLoai() {
        for (int i = 0; i < soNganBan; i++) {
            Nut* nut = mangNgan[i];
            while (nut != nullptr) {
                Nut* nutXoa = nut;
                nut = nut->tiepTheo;
                delete nutXoa;
            }
        }
    }

    // Thêm 1 con trỏ tài liệu vào nhóm thể loại tương ứng
    void ThemVaoNhom(const string& theLoai, TaiLieu* banGhi) {
        Nut* nut = TimNut(theLoai);
        if (nut != nullptr) {
            nut->danhSach.push_back(banGhi);
            return;
        }
        int viTri = ViTriNgan(theLoai);
        Nut* nutMoi = new Nut(theLoai);
        nutMoi->danhSach.push_back(banGhi);
        nutMoi->tiepTheo = mangNgan[viTri];
        mangNgan[viTri] = nutMoi;
        soPhanTu++;
    }

    // Gỡ 1 con trỏ tài liệu khỏi nhóm thể loại (dùng khi Xóa/Đổi thể loại)
    bool GoKhoiNhom(const string& theLoai, const string& maTaiLieu) {
        Nut* nut = TimNut(theLoai);
        if (nut == nullptr) return false;
        for (size_t i = 0; i < nut->danhSach.size(); i++) {
            if (nut->danhSach[i]->maTaiLieu == maTaiLieu) {
                nut->danhSach.erase(nut->danhSach.begin() + i);
                return true;
            }
        }
        return false;
    }

    // RQ1: Tra cứu toàn bộ tài liệu theo thể loại — average-case O(1) để tìm nhóm
    vector<TaiLieu*> TimTheoTheLoai(const string& theLoai) const {
        Nut* nut = TimNut(theLoai);
        if (nut == nullptr) return {};
        return nut->danhSach; // liệt kê cả nhóm
    }

    // Lấy danh sách tất cả các thể loại hiện có (phục vụ hiển thị menu, v.v.)
    vector<string> LayDanhSachTheLoai() const {
        vector<string> ketQua;
        for (int i = 0; i < soNganBan; i++) {
            Nut* nut = mangNgan[i];
            while (nut != nullptr) {
                ketQua.push_back(nut->khoa);
                nut = nut->tiepTheo;
            }
        }
        return ketQua;
    }

    // Duyệt toàn bộ các nhóm — nền tảng phục vụ RQ3 (tìm theo tên/từ khóa)
    vector<vector<TaiLieu*>*> LayTatCaNhom() {
        vector<vector<TaiLieu*>*> ketQua;
        for (int i = 0; i < soNganBan; i++) {
            Nut* nut = mangNgan[i];
            while (nut != nullptr) {
                ketQua.push_back(&nut->danhSach);
                nut = nut->tiepTheo;
            }
        }
        return ketQua;
    }
};

// ============================================================
// 4. LỚP QUẢN LÝ TÀI LIỆU — gộp 2 bảng băm, đồng bộ dữ liệu,
//    và cung cấp MC1, RQ1, RQ3 cho tầng trên (Presentation) gọi vào
// ============================================================
class QuanLyTaiLieu {
private:
    BangBamMaTaiLieu bangBamMa;      // phục vụ MC1
    BangBamTheLoai bangBamTheLoai;   // phục vụ RQ1, làm nền cho RQ3
    vector<TaiLieu*> tatCaBanGhi;    // giữ quyền sở hữu bộ nhớ các bản ghi

public:
    ~QuanLyTaiLieu() {
        for (TaiLieu* bg : tatCaBanGhi) delete bg;
    }

    // Thêm tài liệu mới -> cập nhật ĐỒNG THỜI cả 2 bảng băm (giữ đồng bộ)
    void ThemTaiLieu(const string& ma, const string& ten, const string& tacGia,
                      const string& theLoai, int nam, int soLuong, const string& tinhTrang) {
        TaiLieu* banGhi = new TaiLieu(ma, ten, tacGia, theLoai, nam, soLuong, tinhTrang);
        tatCaBanGhi.push_back(banGhi);
        bangBamMa.ChenHoacCapNhat(ma, banGhi);       // cho MC1
        bangBamTheLoai.ThemVaoNhom(theLoai, banGhi); // cho RQ1 / RQ3
    }

    // Xóa tài liệu theo mã -> phải xóa đồng thời ở cả 2 bảng để tránh lệch dữ liệu
    bool XoaTaiLieu(const string& ma) {
        TaiLieu* banGhi = bangBamMa.TimTheoMa(ma);
        if (banGhi == nullptr) return false;
        bangBamTheLoai.GoKhoiNhom(banGhi->theLoai, ma);
        bangBamMa.Xoa(ma);
        // gỡ khỏi danh sách sở hữu và giải phóng bộ nhớ
        for (size_t i = 0; i < tatCaBanGhi.size(); i++) {
            if (tatCaBanGhi[i] == banGhi) {
                tatCaBanGhi.erase(tatCaBanGhi.begin() + i);
                break;
            }
        }
        delete banGhi;
        return true;
    }

    // ---------------- MC1 ----------------
    // Tra cứu chính xác tài liệu theo mã định danh, O(1) trung bình
    TaiLieu* MC1_TraCuuTheoMa(const string& ma) const {
        return bangBamMa.TimTheoMa(ma);
    }

    // ---------------- RQ1 ----------------
    // Tra cứu tài liệu theo thể loại, trả về toàn bộ danh sách cùng nhóm
    vector<TaiLieu*> RQ1_TraCuuTheoTheLoai(const string& theLoai) const {
        return bangBamTheLoai.TimTheoTheLoai(theLoai);
    }

    // ---------------- RQ3 ----------------
    // Tìm kiếm tài liệu theo tên sách / từ khóa, không cần biết mã hay thể loại.
    // Cách làm: duyệt lần lượt các NHÓM đã có sẵn trong bảng băm phụ (RQ1),
    // rồi lọc chuỗi cục bộ theo tên trong từng nhóm — tận dụng lại cấu trúc
    // đã có, không cần dựng thêm bảng băm thứ ba cho tên sách.
    vector<TaiLieu*> RQ3_TimTheoTen(const string& tuKhoa) const {
        vector<TaiLieu*> ketQua;
        string tuKhoaThuong = ChuoiThuong(tuKhoa);

        vector<vector<TaiLieu*>*> tatCaNhom =
            const_cast<BangBamTheLoai&>(bangBamTheLoai).LayTatCaNhom();

        for (vector<TaiLieu*>* nhom : tatCaNhom) {
            for (TaiLieu* tl : *nhom) {
                string tenThuong = ChuoiThuong(tl->tenTaiLieu);
                if (tenThuong.find(tuKhoaThuong) != string::npos) {
                    ketQua.push_back(tl);
                }
            }
        }
        return ketQua;
    }

    int TongSoTaiLieu() const { return bangBamMa.SoPhanTu(); }
};

// ============================================================
// 5. HÀM PHỤ TRỢ HIỂN THỊ (chỉ để demo, KHÔNG chứa logic cấu trúc dữ liệu)
// ============================================================
inline void InTaiLieu(const TaiLieu* tl) {
    if (tl == nullptr) {
        cout << "  -> Khong tim thay tai lieu.\n";
        return;
    }
    cout << "  [" << tl->maTaiLieu << "] " << tl->tenTaiLieu
         << " - " << tl->tacGia << " (" << tl->namXuatBan << ")"
         << " | The loai: " << tl->theLoai
         << " | SL: " << tl->soLuong
         << " | Tinh trang: " << tl->tinhTrang << "\n";
}

inline void InDanhSach(const vector<TaiLieu*>& ds) {
    if (ds.empty()) {
        cout << "  -> Khong co ket qua.\n";
        return;
    }
    for (TaiLieu* tl : ds) InTaiLieu(tl);
}

#endif // HASH_TABLE_H