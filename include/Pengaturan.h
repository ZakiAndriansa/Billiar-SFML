#pragma once
#include <string>

// Menyimpan konfigurasi permainan (warna kain meja dan volume audio).
class Pengaturan
{
private:
    std::string warnaMeja;   // "hijau", "biru", atau "merah"
    int volumeSfx;           // 0..100
    int volumeBgm;           // 0..100

public:
    Pengaturan();

    void aturWarnaMeja(const std::string& warna);
    std::string getWarnaMeja() const;

    void aturVolumeSfx(int v);
    int getVolumeSfx() const;

    void aturVolumeBgm(int v);
    int getVolumeBgm() const;
};
