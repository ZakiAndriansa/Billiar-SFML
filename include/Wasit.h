#pragma once
#include "Bola.h"
#include "Pemain.h"
#include <string>
#include <vector>

// Menerapkan aturan 8-ball setelah setiap pukulan: memeriksa pelanggaran,
// menentukan jenis bola setiap pemain, giliran, skor, dan pemenang.
class Wasit
{
private:
    Pemain* pemain[2];
    int giliran;
    bool jenisDitentukan;
    bool pukulanPembuka;      // pukulan berikutnya adalah break
    bool pelanggaran;
    bool belakangGaris;       // bola di tangan hanya di belakang garis kepala (pelanggaran saat break)
    Pemain* juara;
    std::string keterangan;

    int sisaBola(JenisBola j, Bola* const bola[16]) const;
    void gantiGiliran();

public:
    Wasit(Pemain* p1, Pemain* p2);
    void aturPemain(Pemain* p1, Pemain* p2);
    void mulai(int giliranAwal = 0);
    void periksaPukulan(int sentuhanPertama, const std::vector<int>& bolaMasuk, Bola* const bola[16]);
    Pemain* getGiliran() const;
    bool adaPelanggaran() const;
    bool adaBolaDiTangan() const;
    bool hanyaBelakangGaris() const;
    Pemain* pemenang() const;
    std::string getKeterangan() const;
};
