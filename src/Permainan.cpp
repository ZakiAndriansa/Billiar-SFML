#include "Permainan.h"
#include "PengelolaSuara.h"
#include <cmath>
#include <cstdlib>
#include <iostream>

namespace
{
const float DIAMETER_BOLA = 32.0f;
const float LANGKAH = 1.0f / 240.0f;      // langkah fisika tetap agar bola cepat tidak menembus bola lain
const sf::Vector2f TITIK_KEPALA(302.0f, 320.0f);   // titik kepala, juga letak garis kepala
const float ZONA_LUBANG_SUDUT = 40.0f;   // pusat bola putih tidak boleh ditaruh di mulut lubang
const float ZONA_LUBANG_TENGAH = 36.0f;

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
    : pemain1("Pemain 1", sf::Vector2f(SUSUNAN[1].x, SUSUNAN[1].y)),
      pemain2("Pemain 2", sf::Vector2f(SUSUNAN[1].x, SUSUNAN[1].y)),
      komputer("Komputer", meja, bola), lawan(&komputer), wasit(&pemain1, &komputer), selesai(false),
      bolaBergerak(false), menaruhBola(false), posisiTaruhSah(true), sisaWaktu(0.0f),
      sentuhanPertama(-1), suara(0), jumlahPelanggaranP1(0), hitunganMasukSuara(0)
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

// Pertandingan baru: bola disusun ulang dan giliran pertama ditentukan secara acak
// (UC-01 langkah 10).
void Permainan::mulaiPertandingan()
{
    susunBola();
    wasit.mulai(std::rand() % 2);
    selesai = false;
    bolaBergerak = false;
    menaruhBola = false;
    posisiTaruhSah = true;
    jumlahPelanggaranP1 = 0;
    hitunganMasukSuara = 0;
    pemain1.batalkan();
    pemain2.batalkan();
    komputer.batalkan();
    stik.aturKekuatan(0.0f);
    std::cout << std::endl << "Pertandingan baru. Giliran: " << wasit.getGiliran()->getNama() << std::endl;
}

// Membatalkan pengisian kekuatan, misalnya saat permainan dijeda di tengah
// bidikan, agar klik berikutnya tidak langsung memukul bola putih.
void Permainan::batalkanBidikan()
{
    pemain1.batalkan();
    pemain2.batalkan();
    stik.aturKekuatan(0.0f);
}

// Memilih lawan: komputer atau pemain manusia kedua. Panggil mulaiPertandingan()
// setelahnya agar wasit memulai dengan pasangan pemain yang baru.
void Permainan::aturLawanKomputer(bool status)
{
    lawan = status ? static_cast<Pemain*>(&komputer) : static_cast<Pemain*>(&pemain2);
    wasit.aturPemain(&pemain1, lawan);
}

bool Permainan::apakahLawanKomputer() const
{
    return lawan == &komputer;
}

void Permainan::aturTingkat(Tingkat t)
{
    komputer.aturTingkat(t);
}

Tingkat Permainan::getTingkat() const
{
    return komputer.getTingkat();
}

void Permainan::aturPengelolaSuara(PengelolaSuara* s)
{
    suara = s;
}

int Permainan::getJumlahPelanggaranP1() const
{
    return jumlahPelanggaranP1;
}

void Permainan::aturNamaPemain1(const std::string& nama)
{
    pemain1.aturNama(nama);
}

void Permainan::aturNamaPemain2(const std::string& nama)
{
    pemain2.aturNama(nama);
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
    return *lawan;
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
    return stik.getKekuatan();
}

bool Permainan::apakahSelesai() const
{
    return selesai;
}

bool Permainan::apakahMenaruhBola() const
{
    return menaruhBola;
}

// Gerakan mouse diteruskan ke semua pemain manusia, sedangkan klik hanya kepada
// pemain yang sedang mendapat giliran. Komputer mengabaikan masukan ini.
void Permainan::prosesEvent(const sf::Event& e)
{
    if (e.type == sf::Event::MouseMoved)
    {
        pemain1.bacaEvent(e);
        pemain2.bacaEvent(e);
    }
    else if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left)
    {
        if (!bolaBergerak && !selesai)
        {
            wasit.getGiliran()->bacaEvent(e);
        }
    }
    else if (e.type == sf::Event::MouseButtonReleased && e.mouseButton.button == sf::Mouse::Left)
    {
        wasit.getGiliran()->bacaEvent(e);
    }
    else if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::R && !bolaBergerak)
    {
        mulaiPertandingan();
    }
}

void Permainan::update(float dt)
{
    // Bola di tangan diselesaikan dulu sebelum pemain boleh membidik.
    if (menaruhBola && !bolaBergerak && !selesai)
    {
        prosesTaruhBola(dt);
        if (menaruhBola)
        {
            return;
        }
    }

    // Selama bola diam, pemain yang mendapat giliran mengendalikan stik. Pemanggilan
    // ini polimorfik: PemainManusia membaca mouse, PemainKomputer menghitung sendiri.
    if (!bolaBergerak && !selesai)
    {
        if (wasit.getGiliran()->ambilGiliran(stik, *bola[0], dt))
        {
            bolaBergerak = true;
            sisaWaktu = 0.0f;
            sentuhanPertama = -1;
            bolaMasuk.clear();
            if (suara)
            {
                sf::Vector2f vPukul = bola[0]->getKecepatan();
                float laju = std::sqrt(vPukul.x * vPukul.x + vPukul.y * vPukul.y);
                float skala = laju / 1800.0f;
                if (skala < 0.25f) skala = 0.25f;
                if (skala > 1.0f) skala = 1.0f;
                suara->putarEfek(SFX_PUKUL_STIK, skala);
            }
        }
    }

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
    }
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
            sf::Vector2f vSebelum = bola[i]->getKecepatan();
            if (bola[i]->tumbuk(*bola[j]))
            {
                if (i == 0 && sentuhanPertama < 0)
                {
                    sentuhanPertama = bola[j]->getNomor();
                }
                if (suara)
                {
                    sf::Vector2f dv = bola[i]->getKecepatan() - vSebelum;
                    float impuls = std::sqrt(dv.x * dv.x + dv.y * dv.y);
                    if (impuls > 25.0f)
                    {
                        float skala = impuls / 700.0f;
                        if (skala < 0.18f) skala = 0.18f;
                        if (skala > 1.0f) skala = 1.0f;
                        suara->putarEfek(SFX_TABRAKAN_BOLA, skala);
                    }
                }
            }
        }
    }

    for (int i = 0; i < 16; i++)
    {
        sf::Vector2f vSebelum = bola[i]->getKecepatan();
        meja.pantulkan(*bola[i]);
        if (suara)
        {
            sf::Vector2f dv = bola[i]->getKecepatan() - vSebelum;
            float impuls = std::sqrt(dv.x * dv.x + dv.y * dv.y);
            if (impuls > 30.0f)
            {
                float skala = impuls / 800.0f;
                if (skala < 0.20f) skala = 0.20f;
                if (skala > 1.0f) skala = 1.0f;
                suara->putarEfek(SFX_PANTUL_MEJA, skala);
            }
        }
        if (meja.cekLubang(*bola[i]))
        {
            bolaMasuk.push_back(bola[i]->getNomor());
            if (suara)
            {
                JenisEfekSuara efek = (bola[i]->getNomor() == 8 || (hitunganMasukSuara % 2 == 1))
                    ? SFX_MASUK_LUBANG_2 : SFX_MASUK_LUBANG;
                hitunganMasukSuara++;
                suara->putarEfek(efek, 1.0f);
            }
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

    const Pemain* penembak = wasit.getGiliran();
    wasit.periksaPukulan(sentuhanPertama, bolaMasuk, bola);
    if (wasit.adaPelanggaran() && penembak == &pemain1)
    {
        jumlahPelanggaranP1++;
    }
    if (suara)
    {
        if (wasit.pemenang())
        {
            suara->putarEfek(wasit.pemenang()->apakahKomputer() ? SFX_PELANGGARAN : SFX_MENANG, 1.0f);
        }
        else if (wasit.adaPelanggaran())
        {
            suara->putarEfek(SFX_PELANGGARAN, 1.0f);
        }
    }
    if (!wasit.getKeterangan().empty())
    {
        std::cout << "  " << wasit.getKeterangan() << std::endl;
    }

    if (bola[0]->statusBola())
    {
        kembalikanBolaPutih();
    }

    // Pelanggaran memberi lawan bola di tangan (UC-04). Bidikan yang tersisa
    // dibatalkan agar klik untuk menaruh bola tidak bercampur dengan pukulan.
    menaruhBola = wasit.adaBolaDiTangan();
    if (menaruhBola)
    {
        pemain1.batalkan();
        pemain2.batalkan();
        wasit.getGiliran()->batalkan();
        stik.aturKekuatan(0.0f);
        if (wasit.hanyaBelakangGaris() && bola[0]->getPosisi().x > TITIK_KEPALA.x)
        {
            kembalikanBolaPutih();
        }
        posisiTaruhSah = posisiSah(bola[0]->getPosisi());
    }

    if (wasit.pemenang())
    {
        selesai = true;
        std::cout << "Pertandingan selesai. Pemenang: " << wasit.pemenang()->getNama()
                  << ". Tekan R untuk bermain lagi." << std::endl;
        return;
    }

    std::cout << "  Skor " << pemain1.getNama() << ": " << pemain1.getSkor() << ", "
              << lawan->getNama() << ": " << lawan->getSkor()
              << ". Giliran: " << wasit.getGiliran()->getNama() << std::endl;
    if (menaruhBola)
    {
        std::cout << "  " << wasit.getGiliran()->getNama() << " mendapat bola di tangan"
                  << (wasit.hanyaBelakangGaris() ? " di belakang garis kepala." : ".") << std::endl;
    }
}

// Daerah yang boleh ditempati pusat bola putih: seluruh meja dikurangi jari-jari
// bola, atau hanya di belakang garis kepala setelah pelanggaran saat break.
sf::FloatRect Permainan::areaTaruh() const
{
    sf::FloatRect b = meja.getBatas();
    float r = DIAMETER_BOLA / 2.0f;
    sf::FloatRect a(b.left + r, b.top + r, b.width - 2.0f * r, b.height - 2.0f * r);
    if (wasit.hanyaBelakangGaris())
    {
        a.width = TITIK_KEPALA.x - a.left;
    }
    return a;
}

// UC-04 langkah 4: posisi sah jika berada di dalam area, tidak menimpa bola lain,
// dan tidak berada di mulut lubang.
bool Permainan::posisiSah(sf::Vector2f p) const
{
    sf::FloatRect a = areaTaruh();
    if (p.x < a.left || p.x > a.left + a.width || p.y < a.top || p.y > a.top + a.height)
    {
        return false;
    }
    for (int i = 1; i < 16; i++)
    {
        if (bola[i]->statusBola())
        {
            continue;
        }
        sf::Vector2f d = bola[i]->getPosisi() - p;
        if (d.x * d.x + d.y * d.y < DIAMETER_BOLA * DIAMETER_BOLA)
        {
            return false;
        }
    }
    for (int i = 0; i < 6; i++)
    {
        float zona = (i == 1 || i == 4) ? ZONA_LUBANG_TENGAH : ZONA_LUBANG_SUDUT;
        sf::Vector2f d = meja.posisiLubang(i) - p;
        if (d.x * d.x + d.y * d.y < zona * zona)
        {
            return false;
        }
    }
    return true;
}

// Bola putih mengikuti posisi pilihan pemain, dijepit agar tetap di dalam area.
// Pemain hanya bisa menaruhnya pada posisi yang sah (UC-04 langkah 5 dan 6).
void Permainan::prosesTaruhBola(float dt)
{
    sf::FloatRect a = areaTaruh();
    sf::Vector2f p = bola[0]->getPosisi();
    bool pilih = wasit.getGiliran()->taruhBolaPutih(p, a, dt);

    if (p.x < a.left) p.x = a.left;
    if (p.x > a.left + a.width) p.x = a.left + a.width;
    if (p.y < a.top) p.y = a.top;
    if (p.y > a.top + a.height) p.y = a.top + a.height;

    bola[0]->aturUlang(p);
    posisiTaruhSah = posisiSah(p);
    if (pilih && posisiTaruhSah)
    {
        menaruhBola = false;
        std::cout << "  " << wasit.getGiliran()->getNama() << " menaruh bola putih di ("
                  << static_cast<int>(p.x) << ", " << static_cast<int>(p.y) << ")." << std::endl;
    }
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

    if (menaruhBola && wasit.hanyaBelakangGaris())
    {
        // Garis kepala sebagai batas penempatan setelah pelanggaran saat break.
        sf::FloatRect b = meja.getBatas();
        sf::RectangleShape garis(sf::Vector2f(2.0f, b.height));
        garis.setPosition(TITIK_KEPALA.x - 1.0f, b.top);
        garis.setFillColor(sf::Color(255, 255, 255, 110));
        w.draw(garis);
    }

    for (int i = 0; i < 16; i++)
    {
        bola[i]->gambar(w);
    }

    if (menaruhBola && !selesai)
    {
        // UC-04 langkah 4: lingkaran hijau berarti posisi sah, merah berarti tidak.
        float r = DIAMETER_BOLA / 2.0f + 5.0f;
        sf::CircleShape tanda(r);
        tanda.setOrigin(r, r);
        tanda.setPosition(bola[0]->getPosisi());
        tanda.setFillColor(sf::Color::Transparent);
        tanda.setOutlineThickness(3.0f);
        tanda.setOutlineColor(posisiTaruhSah ? sf::Color(80, 230, 110) : sf::Color(235, 70, 60));
        w.draw(tanda);
    }
    else if (!bolaBergerak && !selesai)
    {
        stik.gambar(w);
    }
}
