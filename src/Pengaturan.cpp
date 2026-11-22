#include "Pengaturan.h"

Pengaturan::Pengaturan()
    : warnaMeja("hijau"), volumeSfx(80), volumeBgm(60)
{
}

void Pengaturan::aturWarnaMeja(const std::string& warna)
{
    if (warna == "hijau" || warna == "biru" || warna == "merah")
    {
        warnaMeja = warna;
    }
}

std::string Pengaturan::getWarnaMeja() const
{
    return warnaMeja;
}

void Pengaturan::aturVolumeSfx(int v)
{
    if (v < 0) v = 0;
    if (v > 100) v = 100;
    volumeSfx = v;
}

int Pengaturan::getVolumeSfx() const
{
    return volumeSfx;
}

void Pengaturan::aturVolumeBgm(int v)
{
    if (v < 0) v = 0;
    if (v > 100) v = 100;
    volumeBgm = v;
}

int Pengaturan::getVolumeBgm() const
{
    return volumeBgm;
}
