#include "PapanSkor.h"
#include <algorithm>
#include <cstdio>
#include <ctime>
#include <fstream>

namespace
{
struct UrutSkorTurun
{
    bool operator()(const EntriSkor& a, const EntriSkor& b) const
    {
        return a.skor > b.skor;
    }
};

std::string potongSpasi(const std::string& s)
{
    size_t awal = 0;
    while (awal < s.size() && (s[awal] == ' ' || s[awal] == '\t' || s[awal] == '\r' || s[awal] == '\n'))
    {
        awal++;
    }
    size_t akhir = s.size();
    while (akhir > awal && (s[akhir - 1] == ' ' || s[akhir - 1] == '\t' || s[akhir - 1] == '\r' || s[akhir - 1] == '\n'))
    {
        akhir--;
    }
    return s.substr(awal, akhir - awal);
}
}

PapanSkor::PapanSkor(const std::string& path)
    : pathBerkas(path.empty() ? std::string("papan_skor.txt") : path)
{
    muat();
}

std::string PapanSkor::bersihkanTeks(const std::string& s, const std::string& fallback)
{
    std::string hasil;
    hasil.reserve(s.size());
    for (size_t i = 0; i < s.size(); i++)
    {
        char c = s[i];
        if (c == '|' || c == '\r' || c == '\n')
        {
            hasil += ' ';
        }
        else
        {
            hasil += c;
        }
    }
    hasil = potongSpasi(hasil);
    if (hasil.empty())
    {
        return fallback;
    }
    return hasil;
}

bool PapanSkor::parseSkor(const std::string& s, int& hasil)
{
    std::string bersih = potongSpasi(s);
    if (bersih.empty())
    {
        return false;
    }
    long nilai = 0;
    for (size_t i = 0; i < bersih.size(); i++)
    {
        if (bersih[i] < '0' || bersih[i] > '9')
        {
            return false;
        }
        nilai = nilai * 10 + (bersih[i] - '0');
        if (nilai > 1000000L)
        {
            nilai = 1000000L;
        }
    }
    hasil = static_cast<int>(nilai);
    return true;
}

bool PapanSkor::tingkatSah(const std::string& t)
{
    return t == "Mudah" || t == "Sedang" || t == "Sulit";
}

void PapanSkor::urutkan()
{
    std::stable_sort(daftar.begin(), daftar.end(), UrutSkorTurun());
    if (daftar.size() > MAKS_ENTRI)
    {
        daftar.resize(MAKS_ENTRI);
    }
}

bool PapanSkor::muat()
{
    daftar.clear();
    std::ifstream in(pathBerkas.c_str());
    if (!in.is_open())
    {
        return false;
    }

    std::string baris;
    while (std::getline(in, baris))
    {
        if (!baris.empty() && baris[baris.size() - 1] == '\r')
        {
            baris.erase(baris.size() - 1);
        }
        std::string trim = potongSpasi(baris);
        if (trim.empty() || trim[0] == '#')
        {
            continue;
        }

        size_t p1 = baris.find('|');
        if (p1 == std::string::npos) continue;
        size_t p2 = baris.find('|', p1 + 1);
        if (p2 == std::string::npos) continue;
        size_t p3 = baris.find('|', p2 + 1);
        if (p3 == std::string::npos) continue;
        if (baris.find('|', p3 + 1) != std::string::npos) continue;

        std::string nama = potongSpasi(baris.substr(0, p1));
        std::string strSkor = baris.substr(p1 + 1, p2 - p1 - 1);
        std::string tingkat = potongSpasi(baris.substr(p2 + 1, p3 - p2 - 1));
        std::string tanggal = potongSpasi(baris.substr(p3 + 1));

        int nilaiSkor = 0;
        if (nama.empty() || !parseSkor(strSkor, nilaiSkor) || !tingkatSah(tingkat) || tanggal.empty())
        {
            continue;
        }

        EntriSkor entri;
        entri.nama = nama;
        entri.skor = nilaiSkor;
        entri.tingkat = tingkat;
        entri.tanggal = tanggal;
        daftar.push_back(entri);
    }

    urutkan();
    return true;
}

bool PapanSkor::simpan() const
{
    std::ofstream out(pathBerkas.c_str(), std::ios::trunc);
    if (!out.is_open())
    {
        return false;
    }

    out << "# Format: nama|skor|tingkat|tanggal\n";
    for (size_t i = 0; i < daftar.size(); i++)
    {
        out << daftar[i].nama << "|"
            << daftar[i].skor << "|"
            << daftar[i].tingkat << "|"
            << daftar[i].tanggal << "\n";
    }
    return out.good();
}

bool PapanSkor::tambahSkor(const std::string& nama,
                           int skor,
                           const std::string& tingkat,
                           const std::string& tanggal)
{
    EntriSkor entri;
    entri.nama = bersihkanTeks(nama, "Pemain 1");
    entri.skor = (skor < 0) ? 0 : skor;
    entri.tingkat = tingkatSah(tingkat) ? tingkat : std::string("Sedang");
    entri.tanggal = tanggal.empty() ? tanggalHariIni() : bersihkanTeks(tanggal, tanggalHariIni());

    daftar.push_back(entri);
    urutkan();
    return simpan();
}

const std::vector<EntriSkor>& PapanSkor::getDaftar() const
{
    return daftar;
}

std::string PapanSkor::tanggalHariIni()
{
    static const char* BULAN[12] = {
        "Jan", "Feb", "Mar", "Apr", "Mei", "Jun",
        "Jul", "Agu", "Sep", "Okt", "Nov", "Des"
    };
    std::time_t sekarang = std::time(0);
    std::tm* lokal = std::localtime(&sekarang);
    if (!lokal)
    {
        return "01 Jan 2026";
    }
    int bln = lokal->tm_mon;
    if (bln < 0 || bln > 11) bln = 0;
    char buf[32];
    std::sprintf(buf, "%02d %s %04d", lokal->tm_mday, BULAN[bln], lokal->tm_year + 1900);
    return std::string(buf);
}

std::string PapanSkor::namaTingkat(Tingkat t)
{
    if (t == Mudah) return "Mudah";
    if (t == Sulit) return "Sulit";
    return "Sedang";
}

int PapanSkor::hitungSkorAkhir(int bolaPemain,
                               int bolaLawan,
                               Tingkat tingkat,
                               int pelanggaranPemain)
{
    if (bolaPemain < 0) bolaPemain = 0;
    if (bolaPemain > 7) bolaPemain = 7;
    if (bolaLawan < 0) bolaLawan = 0;
    if (bolaLawan > 7) bolaLawan = 7;
    if (pelanggaranPemain < 0) pelanggaranPemain = 0;

    int poinMenang = 400;
    int poinBola = bolaPemain * 70;
    int bonusSelisih = (bolaPemain > bolaLawan) ? (bolaPemain - bolaLawan) * 30 : 0;
    int bonusBersih = (pelanggaranPemain == 0 && bolaPemain >= 5) ? 100 : 0;
    int penalti = pelanggaranPemain * 25;

    int dasar = poinMenang + poinBola + bonusSelisih + bonusBersih - penalti;
    if (dasar < 300)
    {
        dasar = 300;
    }

    float pengali = 1.2f;
    if (tingkat == Mudah) pengali = 1.0f;
    else if (tingkat == Sulit) pengali = 1.4f;

    return static_cast<int>(dasar * pengali + 0.5f);
}
