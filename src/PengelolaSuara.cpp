#include "PengelolaSuara.h"
#include <iostream>

namespace
{
const char* PATH_SFX[JUMLAH_SFX] = {
    "assets/audio/sfx/stick_kena_bola.wav",
    "assets/audio/sfx/bola_tabrakan.wav",
    "assets/audio/sfx/bola_mantul_meja.wav",
    "assets/audio/sfx/bola_masuk_lubang.wav",
    "assets/audio/sfx/bola_masuk_lubang_2.wav",
    "assets/audio/sfx/pelanggaran.wav",
    "assets/audio/sfx/menang.wav",
    "assets/audio/sfx/klik.ogg"
};

const char* PATH_BGM = "assets/audio/music/background_music.ogg";

int batasiPersen(int v)
{
    if (v < 0) return 0;
    if (v > 100) return 100;
    return v;
}
}

PengelolaSuara::PengelolaSuara()
    : indeksSaluran(0),
      musikSiap(false),
      volumeSfx(80),
      volumeBgm(60),
      benihPitch(20261128UL)
{
    for (int i = 0; i < JUMLAH_SFX; i++)
    {
        bufferSiap[i] = false;
        waktuTerakhir[i] = -10.0f;
    }
}

PengelolaSuara::~PengelolaSuara()
{
    hentikanSemua();
    for (int i = 0; i < MAX_SALURAN; i++)
    {
        saluran[i].resetBuffer();
    }
}

bool PengelolaSuara::muatSemua()
{
    bool semuaBerhasil = true;
    for (int i = 0; i < JUMLAH_SFX; i++)
    {
        bufferSiap[i] = buffer[i].loadFromFile(PATH_SFX[i]);
        if (!bufferSiap[i])
        {
            std::cerr << "[PERINGATAN] Gagal memuat SFX: " << PATH_SFX[i] << std::endl;
            semuaBerhasil = false;
        }
    }

    musikSiap = musik.openFromFile(PATH_BGM);
    if (musikSiap)
    {
        musik.setLoop(true);
        musik.setVolume(static_cast<float>(volumeBgm));
    }
    else
    {
        std::cerr << "[PERINGATAN] Gagal membuka musik latar: " << PATH_BGM << std::endl;
        semuaBerhasil = false;
    }

    return semuaBerhasil;
}

void PengelolaSuara::terapkanPengaturan(const Pengaturan& cfg)
{
    aturVolumeSfx(cfg.getVolumeSfx());
    aturVolumeBgm(cfg.getVolumeBgm());
}

void PengelolaSuara::aturVolumeSfx(int v)
{
    volumeSfx = batasiPersen(v);
    if (volumeSfx == 0)
    {
        for (int i = 0; i < MAX_SALURAN; i++)
        {
            if (saluran[i].getStatus() == sf::Sound::Playing)
            {
                saluran[i].setVolume(0.0f);
            }
        }
    }
}

void PengelolaSuara::aturVolumeBgm(int v)
{
    volumeBgm = batasiPersen(v);
    if (musikSiap)
    {
        musik.setVolume(static_cast<float>(volumeBgm));
    }
}

int PengelolaSuara::getVolumeSfx() const
{
    return volumeSfx;
}

int PengelolaSuara::getVolumeBgm() const
{
    return volumeBgm;
}

void PengelolaSuara::mulaiMusik()
{
    if (!musikSiap)
    {
        return;
    }
    musik.setVolume(static_cast<float>(volumeBgm));
    if (musik.getStatus() != sf::SoundSource::Playing)
    {
        musik.play();
    }
}

void PengelolaSuara::jedaMusik()
{
    if (musikSiap && musik.getStatus() == sf::SoundSource::Playing)
    {
        musik.pause();
    }
}

void PengelolaSuara::hentikanMusik()
{
    if (musikSiap)
    {
        musik.stop();
    }
}

void PengelolaSuara::hentikanSemua()
{
    hentikanMusik();
    for (int i = 0; i < MAX_SALURAN; i++)
    {
        saluran[i].stop();
    }
}

float PengelolaSuara::cooldownEfek(JenisEfekSuara jenis) const
{
    if (jenis == SFX_TABRAKAN_BOLA) return 0.035f;
    if (jenis == SFX_PANTUL_MEJA) return 0.045f;
    if (jenis == SFX_MASUK_LUBANG || jenis == SFX_MASUK_LUBANG_2) return 0.03f;
    if (jenis == SFX_KLIK_MENU) return 0.03f;
    return 0.0f;
}

float PengelolaSuara::acakPitch()
{
    benihPitch = benihPitch * 1103515245UL + 12345UL;
    float r = static_cast<float>((benihPitch >> 16) & 0x7FFFUL) / 32767.0f;
    return 0.95f + 0.10f * r;
}

void PengelolaSuara::putarEfek(JenisEfekSuara jenis, float skalaIntensitas)
{
    int idx = static_cast<int>(jenis);
    if (idx < 0 || idx >= JUMLAH_SFX)
    {
        return;
    }
    if (!bufferSiap[idx] || volumeSfx <= 0)
    {
        return;
    }

    float sekarang = jam.getElapsedTime().asSeconds();
    float jeda = cooldownEfek(jenis);
    if (jeda > 0.0f && (sekarang - waktuTerakhir[idx]) < jeda)
    {
        return;
    }

    if (skalaIntensitas < 0.15f) skalaIntensitas = 0.15f;
    if (skalaIntensitas > 1.0f) skalaIntensitas = 1.0f;

    int pilih = -1;
    for (int i = 0; i < MAX_SALURAN; i++)
    {
        if (saluran[i].getStatus() != sf::Sound::Playing)
        {
            pilih = i;
            break;
        }
    }
    if (pilih < 0)
    {
        pilih = indeksSaluran;
        indeksSaluran = (indeksSaluran + 1) % MAX_SALURAN;
    }

    saluran[pilih].setBuffer(buffer[idx]);
    saluran[pilih].setVolume(static_cast<float>(volumeSfx) * skalaIntensitas);
    if (jenis == SFX_TABRAKAN_BOLA || jenis == SFX_PANTUL_MEJA)
    {
        saluran[pilih].setPitch(acakPitch());
    }
    else
    {
        saluran[pilih].setPitch(1.0f);
    }
    saluran[pilih].play();
    waktuTerakhir[idx] = sekarang;
}

bool PengelolaSuara::apakahEfekSiap(JenisEfekSuara jenis) const
{
    int idx = static_cast<int>(jenis);
    if (idx < 0 || idx >= JUMLAH_SFX)
    {
        return false;
    }
    return bufferSiap[idx];
}

bool PengelolaSuara::apakahMusikSiap() const
{
    return musikSiap;
}

bool PengelolaSuara::apakahMusikAktif() const
{
    return musikSiap && musik.getStatus() == sf::SoundSource::Playing;
}
