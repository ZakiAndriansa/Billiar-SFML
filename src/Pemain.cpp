#include "Pemain.h"

Pemain::Pemain(const std::string& namaAwal)
    : nama(namaAwal), jenisBola(Putih), skor(0)
{
}

void Pemain::aturNama(const std::string& namaBaru)
{
    if (!namaBaru.empty())
    {
        nama = namaBaru;
    }
}

std::string Pemain::getNama() const
{
    return nama;
}

JenisBola Pemain::getJenisBola() const
{
    return jenisBola;
}

void Pemain::aturJenisBola(JenisBola j)
{
    jenisBola = j;
}

int Pemain::getSkor() const
{
    return skor;
}

void Pemain::tambahSkor(int n)
{
    skor += n;
}

void Pemain::aturUlang()
{
    jenisBola = Putih;
    skor = 0;
}
