#pragma once
#include "PemainKomputer.h"
#include <cstddef>
#include <string>
#include <vector>

struct EntriSkor
{
    std::string nama;
    int skor;
    std::string tingkat;
    std::string tanggal;
};

// Mengelola penyimpanan otomatis dan pembacaan rekor papan skor ke berkas lokal (WBS 5.4).
class PapanSkor
{
public:
    static const size_t MAKS_ENTRI = 10;

private:
    std::string pathBerkas;
    std::vector<EntriSkor> daftar;

    static std::string bersihkanTeks(const std::string& s, const std::string& fallback);
    static bool parseSkor(const std::string& s, int& hasil);
    static bool tingkatSah(const std::string& t);
    void urutkan();

public:
    explicit PapanSkor(const std::string& path = "papan_skor.txt");

    bool muat();
    bool simpan() const;
    bool tambahSkor(const std::string& nama,
                    int skor,
                    const std::string& tingkat,
                    const std::string& tanggal = "");

    const std::vector<EntriSkor>& getDaftar() const;

    static std::string tanggalHariIni();
    static std::string namaTingkat(Tingkat t);
    static int hitungSkorAkhir(int bolaPemain,
                               int bolaLawan,
                               Tingkat tingkat,
                               int pelanggaranPemain);
};
