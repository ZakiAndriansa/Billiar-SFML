#include "Permainan.h"
#include <iostream>

namespace
{
const float DIAMETER_BOLA = 32.0f;
const float KEKUATAN_PER_DETIK = 70.0f;
const float KEKUATAN_MIN = 3.0f;          // pukulan di bawah nilai ini dianggap batal
const float LANGKAH = 1.0f / 240.0f;      // langkah fisika tetap agar bola cepat tidak menembus bola lain
const sf::Vector2f TITIK_KEPALA(302.0f, 320.0f);

struct DataBola
{
    int nomor;
    float x;
    float y;
};

// Posisi 16 bola: 1 bola putih di garis kepala, 15 bola di rak segitiga.
const DataBola SUSUNAN[16] = {
    {  0, 302.0f, 320.0f },
    {  1, 722.0f, 320.0f },
    {  9, 749.7f, 304.0f }, {  2, 749.7f, 336.0f },
    { 10, 777.4f, 288.0f }, {  8, 777.4f, 320.0f }, {  3, 777.4f, 352.0f },
    { 11, 805.1f, 272.0f }, {  7, 805.1f, 304.0f }, { 14, 805.1f, 336.0f }, {  4, 805.1f, 368.0f },
    {  5, 832.8f, 256.0f }, { 13, 832.8f, 288.0f }, { 15, 832.8f, 320.0f }, {  6, 832.8f, 352.0f }, { 12, 832.8f, 384.0f }
};
}

Permainan::Permainan()
    : pemain1("Pemain 1"), pemain2("Pemain 2"), wasit(&pemain1, &pemain2), selesai(false),
      posisiMouse(SUSUNAN[1].x, SUSUNAN[1].y), kekuatan(0.0f), menahan(false),
      bolaBergerak(false), sisaWaktu(0.0f), sentuhanPertama(-1)
{
    for (int i = 0; i < 16; i++)
    {
        bola[i] = new Bola(SUSUNAN[i].nomor, sf::Vector2f(SUSUNAN[i].x, SUSUNAN[i].y), DIAMETER_BOLA);
    }
    std::cout << "Pertandingan dimulai. Giliran: " << wasit.getGiliran()->getNama() << std::endl;
}

Permainan::~Permainan()
{
    for (int i = 0; i < 16; i++)
    {
        delete bola[i];
    }
}

void Permainan::susunBola()
{
    for (int i = 0; i < 16; i++)
    {
        bola[i]->aturUlang(sf::Vector2f(SUSUNAN[i].x, SUSUNAN[i].y));
    }
}

void Permainan::mulaiPertandingan()
{
    susunBola();
    wasit.mulai();
    selesai = false;
    bolaBergerak = false;
    menahan = false;
    kekuatan = 0.0f;
    stik.aturKekuatan(0.0f);
    std::cout << std::endl << "Pertandingan baru. Giliran: " << wasit.getGiliran()->getNama() << std::endl;
}

void Permainan::aturNamaPemain1(const std::string& nama)
{
    pemain1.aturNama(nama);
}

void Permainan::aturWarnaMeja(const std::string& warna)
{
    meja.aturWarna(warna);
}

const Pemain& Permainan::getPemain1() const
{
    return pemain1;
}

const Pemain& Permainan::getPemain2() const
{
    return pemain2;
}

const Wasit& Permainan::getWasit() const
{
    return wasit;
}

const Bola* Permainan::getBola(int indeks) const
{
    if (indeks < 0 || indeks >= 16) return 0;
    return bola[indeks];
}

float Permainan::getKekuatan() const
{
    return kekuatan;
}

bool Permainan::apakahSelesai() const
{
    return selesai;
}

void Permainan::prosesEvent(const sf::Event& e)
{
    if (e.type == sf::Event::MouseMoved)
    {
        posisiMouse = sf::Vector2f(static_cast<float>(e.mouseMove.x), static_cast<float>(e.mouseMove.y));
    }
    else if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left)
    {
        if (!bolaBergerak && !selesai)
        {
            menahan = true;
        }
    }
    else if (e.type == sf::Event::MouseButtonReleased && e.mouseButton.button == sf::Mouse::Left)
    {
        if (menahan && !bolaBergerak && kekuatan >= KEKUATAN_MIN)
        {
            stik.pukul(*bola[0]);
            bolaBergerak = true;
            sisaWaktu = 0.0f;
            sentuhanPertama = -1;
            bolaMasuk.clear();
        }
        menahan = false;
        kekuatan = 0.0f;
        stik.aturKekuatan(kekuatan);
    }
    else if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::R && !bolaBergerak)
    {
        mulaiPertandingan();
    }
}

void Permainan::update(float dt)
{
    if (bolaBergerak)
    {
        // Batas 0,25 detik mencegah lonjakan saat jendela sempat tertahan.
        sisaWaktu += (dt > 0.25f) ? 0.25f : dt;
        while (sisaWaktu >= LANGKAH)
        {
            langkahFisika(LANGKAH);
            sisaWaktu -= LANGKAH;
        }
        if (semuaBerhenti())
        {
            bolaBergerak = false;
            selesaiPukulan();
        }
        return;
    }

    if (menahan)
    {
        kekuatan += KEKUATAN_PER_DETIK * dt;
        if (kekuatan > 100.0f)
        {
            kekuatan = 100.0f;
        }
    }

    stik.bidik(posisiMouse - bola[0]->getPosisi());
    stik.aturKekuatan(kekuatan);
    stik.ikuti(bola[0]->getPosisi());
}

void Permainan::langkahFisika(float h)
{
    for (int i = 0; i < 16; i++)
    {
        bola[i]->update(h);
    }

    for (int i = 0; i < 16; i++)
    {
        for (int j = i + 1; j < 16; j++)
        {
            if (bola[i]->tumbuk(*bola[j]) && i == 0 && sentuhanPertama < 0)
            {
                sentuhanPertama = bola[j]->getNomor();
            }
        }
    }

    for (int i = 0; i < 16; i++)
    {
        meja.pantulkan(*bola[i]);
        if (meja.cekLubang(*bola[i]))
        {
            bolaMasuk.push_back(bola[i]->getNomor());
        }
    }
}

bool Permainan::semuaBerhenti() const
{
    for (int i = 0; i < 16; i++)
    {
        if (!bola[i]->berhenti())
        {
            return false;
        }
    }
    return true;
}

// Tampilan skor dan giliran di layar dikerjakan pada WBS 5.3, jadi untuk
// sementara hasil setiap pukulan dicetak ke konsol.
void Permainan::selesaiPukulan()
{
    std::cout << wasit.getGiliran()->getNama() << " memukul.";
    for (size_t i = 0; i < bolaMasuk.size(); i++)
    {
        std::cout << " Bola " << bolaMasuk[i] << " masuk.";
    }
    std::cout << std::endl;

    wasit.periksaPukulan(sentuhanPertama, bolaMasuk, bola);
    if (!wasit.getKeterangan().empty())
    {
        std::cout << "  " << wasit.getKeterangan() << std::endl;
    }

    if (bola[0]->statusBola())
    {
        kembalikanBolaPutih();
    }

    if (wasit.pemenang())
    {
        selesai = true;
        std::cout << "Pertandingan selesai. Pemenang: " << wasit.pemenang()->getNama()
                  << ". Tekan R untuk bermain lagi." << std::endl;
        return;
    }

    std::cout << "  Skor " << pemain1.getNama() << ": " << pemain1.getSkor() << ", "
              << pemain2.getNama() << ": " << pemain2.getSkor()
              << ". Giliran: " << wasit.getGiliran()->getNama() << std::endl;
}

// Bola putih yang masuk dikembalikan ke titik kepala. Jika titik itu terhalang,
// bola digeser ke kiri sampai menemukan tempat kosong.
void Permainan::kembalikanBolaPutih()
{
    sf::FloatRect batas = meja.getBatas();
    for (float x = TITIK_KEPALA.x; x > batas.left + DIAMETER_BOLA; x -= 4.0f)
    {
        sf::Vector2f p(x, TITIK_KEPALA.y);
        bool kosong = true;
        for (int i = 1; i < 16 && kosong; i++)
        {
            if (bola[i]->statusBola())
            {
                continue;
            }
            sf::Vector2f d = bola[i]->getPosisi() - p;
            if (d.x * d.x + d.y * d.y < DIAMETER_BOLA * DIAMETER_BOLA)
            {
                kosong = false;
            }
        }
        if (kosong)
        {
            bola[0]->aturUlang(p);
            return;
        }
    }
    bola[0]->aturUlang(TITIK_KEPALA);
}

void Permainan::gambar(sf::RenderWindow& w)
{
    meja.gambar(w);
    for (int i = 0; i < 16; i++)
    {
        bola[i]->gambar(w);
    }
    if (!bolaBergerak && !selesai)
    {
        stik.gambar(w);
    }
}
