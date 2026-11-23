#pragma once
#include "Bola.h"
#include <string>

// Data satu pemain. Pada fase Lawan Komputer (WBS 4.1 dan 4.2) kelas ini menjadi
// induk PemainManusia dan PemainKomputer.
class Pemain
{
protected:
    std::string nama;
    JenisBola jenisBola;   // Putih berarti jenis bola belum ditentukan
    int skor;

public:
    Pemain(const std::string& namaAwal);
    virtual ~Pemain() {}
    void aturNama(const std::string& namaBaru);
    std::string getNama() const;
    JenisBola getJenisBola() const;
    void aturJenisBola(JenisBola j);
    int getSkor() const;
    void tambahSkor(int n);
    void aturUlang();
};
