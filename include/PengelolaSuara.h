#pragma once
#include "Pengaturan.h"
#include <SFML/Audio.hpp>

enum JenisEfekSuara
{
    SFX_PUKUL_STIK = 0,
    SFX_TABRAKAN_BOLA,
    SFX_PANTUL_MEJA,
    SFX_MASUK_LUBANG,
    SFX_MASUK_LUBANG_2,
    SFX_PELANGGARAN,
    SFX_MENANG,
    SFX_KLIK_MENU,
    JUMLAH_SFX
};

// Mengelola efek suara (SFX) dan musik latar (BGM) permainan biliar (WBS 5.2).
class PengelolaSuara
{
private:
    static const int MAX_SALURAN = 24;

    // Buffer dideklarasikan sebelum saluran agar sf::Sound dihancurkan lebih dulu.
    sf::SoundBuffer buffer[JUMLAH_SFX];
    bool bufferSiap[JUMLAH_SFX];
    sf::Sound saluran[MAX_SALURAN];
    int indeksSaluran;

    sf::Music musik;
    bool musikSiap;

    int volumeSfx;
    int volumeBgm;
    sf::Clock jam;
    float waktuTerakhir[JUMLAH_SFX];
    unsigned long benihPitch;

    float cooldownEfek(JenisEfekSuara jenis) const;
    float acakPitch();

public:
    PengelolaSuara();
    ~PengelolaSuara();

    bool muatSemua();
    void terapkanPengaturan(const Pengaturan& cfg);

    void aturVolumeSfx(int v);
    void aturVolumeBgm(int v);
    int getVolumeSfx() const;
    int getVolumeBgm() const;

    void mulaiMusik();
    void jedaMusik();
    void hentikanMusik();
    void hentikanSemua();

    void putarEfek(JenisEfekSuara jenis, float skalaIntensitas = 1.0f);

    bool apakahEfekSiap(JenisEfekSuara jenis) const;
    bool apakahMusikSiap() const;
    bool apakahMusikAktif() const;
};
