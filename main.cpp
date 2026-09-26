#include <iostream>
#include <vector>
#include <cstdlib>
#include "models/Models.h"
#include "persistence/FileStore.h"
#include "dsa_core/LibraryService.h"
#include "presentation/Menu.h"

using namespace std;

int main() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    cout << "=========================================================\n";
    cout << " [KHOI DONG] Nap du lieu tu tang Persistence (data/*.json)...\n";
    
    vector<TaiLieu> dsSach = docDanhSachSach();
    vector<DocGia> dsDocGia = docDanhSachDocGia();
    vector<PhieuMuon> dsPhieu = docDanhSachPhieu();

    LibraryService service;
    service.napDuLieu(dsSach, dsDocGia, dsPhieu);

    cout << " [SAN SANG] Da nap " << dsSach.size() << " sach, " 
         << dsDocGia.size() << " doc gia, " 
         << dsPhieu.size() << " phieu muon vao Core.\n";
    cout << "=========================================================\n";

    chayMenu(service);

    return 0;
}
