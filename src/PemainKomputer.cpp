#include "PemainKomputer.h"
#include <cmath>

namespace
{
const float PI = 3.14159265f;

// Konstanta fisika yang sama dengan Bola.cpp dan Stik.cpp. Komputer memakainya
// untuk memperkirakan kekuatan pukulan yang dibutuhkan.
const float GESEKAN = 120.0f;
const float REDAMAN = 0.3f;
const float PANTULAN_BOLA = 0.95f;
const float LAJU_MAKS = 1800.0f;
const float LANGKAH_FISIKA = 1.0f / 240.0f;   // sama dengan LANGKAH di Permainan.cpp

// Mulut lubang tanpa tepi: radius tangkap (26 di sudut, 22 di tengah) ditambah 14.
const float ZONA_SUDUT = 40.0f;
const float ZONA_TENGAH = 36.0f;

const float SUDUT_POTONG_MAKS = 78.0f;   // potongan yang lebih tipis dianggap mustahil
const float SISA_JARAK = 70.0f;          // sasaran harus masih bisa melaju sejauh ini setelah lubang

const float JEDA_BERPIKIR = 0.8f;
const float LAMA_MEMBIDIK = 0.7f;
const float KECEPATAN_MENGISI = 90.0f;
const float LAMA_MENGGESER = 0.6f;       // animasi bola putih menuju posisi pilihan

// Banyak posisi yang dicoba saat bola di tangan. Sulit memeriksa seluruh meja
// dengan kisi berjarak 12 piksel, tingkat lain hanya mencoba posisi acak.
const int CONTOH_POSISI[3] = { 8, 60, 0 };
const float JARAK_KISI = 12.0f;

// Galat arah (derajat) dan galat kekuatan (bagian dari kekuatan) untuk setiap tingkat.
const float GALAT_ARAH[3] = { 3.0f, 0.9f, 0.2f };
const float GALAT_KEKUATAN[3] = { 0.15f, 0.08f, 0.03f };

float panjang(sf::Vector2f v)
{
    return std::sqrt(v.x * v.x + v.y * v.y);
}

float dot(sf::Vector2f a, sf::Vector2f b)
{
    return a.x * b.x + a.y * b.y;
}

sf::Vector2f arahSudut(float derajat)
{
    float r = derajat * PI / 180.0f;
    return sf::Vector2f(std::cos(r), std::sin(r));
}

float sudutArah(sf::Vector2f v)
{
    return std::atan2(v.y, v.x) * 180.0f / PI;
}

// Jarak tempuh bola berkecepatan awal v sampai berhenti, dari persamaan
// dv/dt = -(GESEKAN + REDAMAN * v) yang dipakai Bola::update().
float jarakHenti(float v)
{
    if (v <= 0.0f) return 0.0f;
    float a = GESEKAN, b = REDAMAN;
    return v / b - (a / (b * b)) * std::log(1.0f + b * v / a);
}

// Kebalikan jarakHenti(): kecepatan awal agar bola menempuh jarak s.
float lajuUntukJarak(float s)
{
    float bawah = 0.0f, atas = 6000.0f;
    for (int i = 0; i < 40; i++)
    {
        float tengah = (bawah + atas) / 2.0f;
        if (jarakHenti(tengah) < s) bawah = tengah;
        else atas = tengah;
    }
    return atas;
}

bool lubangTengah(int i)
{
    return i == 1 || i == 4;
}
}

PemainKomputer::PemainKomputer(const std::string& namaAwal, const Meja& m, Bola* const* b)
    : Pemain(namaAwal), tingkat(Sedang), meja(m), bola(b), tahap(BERPIKIR), waktu(0.0f),
      sudutAwal(0.0f), sudutTujuan(0.0f), kekuatanSaatIni(0.0f), kekuatanTujuan(0.0f),
      tanpaJeda(false), benihAcak(12345UL), tahapTaruh(TARUH_BERPIKIR)
{
    rencana.sasaran = 0;
    rencana.lubang = -1;
    rencana.sudutPotong = 0.0f;
    rencana.jarak = 0.0f;
    rencana.nilai = 0.0f;
}

bool PemainKomputer::apakahKomputer() const
{
    return true;
}

void PemainKomputer::aturTingkat(Tingkat t)
{
    tingkat = t;
}

Tingkat PemainKomputer::getTingkat() const
{
    return tingkat;
}

void PemainKomputer::aturTanpaJeda(bool status)
{
    tanpaJeda = status;
}

void PemainKomputer::aturBenihAcak(unsigned long benih)
{
    benihAcak = benih;
}

const Pukulan& PemainKomputer::getRencana() const
{
    return rencana;
}

void PemainKomputer::batalkan()
{
    tahap = BERPIKIR;
    tahapTaruh = TARUH_BERPIKIR;
    waktu = 0.0f;
}

float PemainKomputer::acak()
{
    benihAcak = benihAcak * 1103515245UL + 12345UL;
    unsigned long n = (benihAcak >> 16) & 0x7FFFUL;
    return static_cast<float>(n) / 16383.5f - 1.0f;
}

// Bola yang boleh disentuh lebih dulu: saat meja terbuka semua bola kecuali bola 8,
// setelah golongan ditentukan hanya bola golongan sendiri, dan bola 8 jika bola
// golongan sendiri sudah habis.
bool PemainKomputer::sasaranSah(const Bola& b) const
{
    if (b.statusBola() || b.getNomor() == 0)
    {
        return false;
    }
    if (jenisBola == Putih)
    {
        return b.getNomor() != 8;
    }

    int sisa = 0;
    for (int i = 0; i < 16; i++)
    {
        if (!bola[i]->statusBola() && bola[i]->getJenis() == jenisBola) sisa++;
    }
    if (sisa == 0)
    {
        return b.getNomor() == 8;
    }
    return b.getJenis() == jenisBola;
}

// Rak masih utuh jika kelima belas bola ada di meja dan berada di area segitiga.
bool PemainKomputer::sedangBreak() const
{
    for (int i = 0; i < 16; i++)
    {
        if (bola[i]->getNomor() == 0) continue;
        if (bola[i]->statusBola() || bola[i]->getPosisi().x < 700.0f) return false;
    }
    return true;
}

// Metode ghost ball: bola putih harus menyentuh sasaran di titik yang berada tepat
// di belakang sasaran, berlawanan dengan arah lubang, sejauh dua kali jari jari.
sf::Vector2f PemainKomputer::titikBidik(const Bola& sasaran, sf::Vector2f lubang) const
{
    sf::Vector2f arah = lubang - sasaran.getPosisi();
    float d = panjang(arah);
    if (d <= 0.0f) return sasaran.getPosisi();
    arah /= d;
    return sasaran.getPosisi() - arah * (sasaran.getJariJari() * 2.0f);
}

// Lintasan bola dari titik dari ke titik ke dianggap terhalang jika ada bola lain
// yang pusatnya lebih dekat dari satu diameter terhadap garis lintasan itu.
bool PemainKomputer::terhalang(sf::Vector2f dari, sf::Vector2f ke, const Bola* abaikan1, const Bola* abaikan2) const
{
    sf::Vector2f seg = ke - dari;
    float pj2 = dot(seg, seg);
    for (int i = 0; i < 16; i++)
    {
        const Bola* b = bola[i];
        if (b == abaikan1 || b == abaikan2 || b->statusBola()) continue;

        sf::Vector2f p = b->getPosisi();
        float t = (pj2 > 0.0f) ? dot(p - dari, seg) / pj2 : 0.0f;
        if (t < 0.0f) t = 0.0f;
        if (t > 1.0f) t = 1.0f;
        sf::Vector2f terdekat = dari + seg * t;
        float batasJarak = b->getJariJari() * 2.0f - 0.5f;
        sf::Vector2f d = p - terdekat;
        if (dot(d, d) < batasJarak * batasJarak) return true;
    }
    return false;
}

// WBS 4.1: setiap bola sasaran yang sah dipasangkan dengan keenam lubang, lalu
// pasangan yang mustahil atau terhalang dibuang.
std::vector<Pukulan> PemainKomputer::cariKandidat(const Bola& bolaPutih) const
{
    std::vector<Pukulan> hasil;
    sf::Vector2f c = bolaPutih.getPosisi();
    sf::FloatRect batas = meja.getBatas();
    float r = bolaPutih.getJariJari();

    for (int i = 0; i < 16; i++)
    {
        const Bola& s = *bola[i];
        if (!sasaranSah(s)) continue;

        for (int l = 0; l < 6; l++)
        {
            sf::Vector2f lub = meja.posisiLubang(l);
            sf::Vector2f g = titikBidik(s, lub);

            // Titik bidik harus berada di atas kain agar bola putih bisa sampai ke sana.
            if (g.x < batas.left + r || g.x > batas.left + batas.width - r ||
                g.y < batas.top + r || g.y > batas.top + batas.height - r) continue;

            sf::Vector2f bidik = g - c;
            float jarakBidik = panjang(bidik);
            if (jarakBidik < 1.0f) continue;

            sf::Vector2f keLubang = lub - s.getPosisi();
            float jarakLubang = panjang(keLubang);
            sf::Vector2f arahLubang = keLubang / jarakLubang;

            float cosPotong = dot(bidik / jarakBidik, arahLubang);
            if (cosPotong > 1.0f) cosPotong = 1.0f;
            float sudut = std::acos(cosPotong) * 180.0f / PI;
            if (sudut > SUDUT_POTONG_MAKS) continue;

            // Lubang tengah hanya bisa dimasuki dari arah yang cukup tegak.
            if (lubangTengah(l) && std::fabs(arahLubang.y) < 0.45f) continue;

            // Sasaran tidak boleh menabrak tepi sebelum sampai di mulut lubang.
            float zona = lubangTengah(l) ? ZONA_TENGAH : ZONA_SUDUT;
            bool kenaTepi = false;
            for (float t = 0.0f; t < jarakLubang - zona; t += 8.0f)
            {
                sf::Vector2f q = s.getPosisi() + arahLubang * t;
                if (q.x < batas.left + r - 2.0f || q.x > batas.left + batas.width - r + 2.0f ||
                    q.y < batas.top + r - 2.0f || q.y > batas.top + batas.height - r + 2.0f)
                {
                    kenaTepi = true;
                    break;
                }
            }
            if (kenaTepi) continue;

            if (terhalang(c, g, &bolaPutih, &s)) continue;
            if (terhalang(s.getPosisi(), lub, &s, 0)) continue;

            Pukulan p;
            p.sasaran = &s;
            p.lubang = l;
            p.titikBidik = g;
            p.sudutPotong = sudut;
            p.jarak = jarakBidik + jarakLubang;
            p.nilai = 0.0f;
            hasil.push_back(p);
        }
    }
    return hasil;
}

// WBS 4.2: cara menilai pukulan berbeda untuk setiap tingkat.
float PemainKomputer::nilaiPukulan(const Pukulan& p) const
{
    sf::Vector2f c = bola[0]->getPosisi();
    if (tingkat == Mudah)
    {
        // Mudah: bola yang paling dekat dengan bola putih.
        return -panjang(p.sasaran->getPosisi() - c);
    }
    if (tingkat == Sedang)
    {
        // Sedang: sudut potong paling kecil, artinya sasaran paling mudah dikenai.
        return -p.sudutPotong - 0.001f * p.jarak;
    }
    // Sulit: peluang masuk terbaik. Potongan lurus, jarak pendek, dan lubang sudut
    // memperbesar peluang.
    float cosPotong = std::cos(p.sudutPotong * PI / 180.0f);
    float peluang = cosPotong * cosPotong * std::exp(-p.jarak / 1400.0f);
    if (lubangTengah(p.lubang)) peluang *= 0.85f;
    return peluang;
}

Pukulan PemainKomputer::pilihPukulan(std::vector<Pukulan>& kandidat) const
{
    size_t terbaik = 0;
    for (size_t i = 0; i < kandidat.size(); i++)
    {
        kandidat[i].nilai = nilaiPukulan(kandidat[i]);
        if (kandidat[i].nilai > kandidat[terbaik].nilai) terbaik = i;
    }
    return kandidat[terbaik];
}

// Jika tidak ada lubang yang terbuka, komputer cukup menyentuh bola sasaran yang
// sah dan terdekat supaya tidak melanggar.
Pukulan PemainKomputer::pukulanCadangan(const Bola& bolaPutih) const
{
    sf::Vector2f c = bolaPutih.getPosisi();
    const Bola* pilih = 0;
    float jarakPilih = 0.0f;
    bool pilihBebas = false;

    for (int i = 0; i < 16; i++)
    {
        const Bola& s = *bola[i];
        if (!sasaranSah(s)) continue;
        float d = panjang(s.getPosisi() - c);
        bool bebas = !terhalang(c, s.getPosisi(), &bolaPutih, &s);
        if (pilih == 0 || (bebas && !pilihBebas) || (bebas == pilihBebas && d < jarakPilih))
        {
            pilih = &s;
            jarakPilih = d;
            pilihBebas = bebas;
        }
    }

    Pukulan p;
    p.sasaran = pilih;
    p.lubang = -1;
    p.titikBidik = pilih ? pilih->getPosisi() : c + sf::Vector2f(100.0f, 0.0f);
    p.sudutPotong = 0.0f;
    p.jarak = jarakPilih;
    p.nilai = 0.0f;
    return p;
}

// Kekuatan dihitung mundur dari lubang: kecepatan sasaran yang dibutuhkan, lalu
// kecepatan bola putih saat menyentuh sasaran, lalu kecepatan awal bola putih.
float PemainKomputer::hitungKekuatan(const Pukulan& p) const
{
    if (p.lubang < 0)
    {
        return 55.0f;
    }

    float d1 = panjang(p.titikBidik - bola[0]->getPosisi());
    float d2 = panjang(meja.posisiLubang(p.lubang) - p.sasaran->getPosisi());

    float lajuSasaran = lajuUntukJarak(d2 + SISA_JARAK);
    float cosPotong = std::cos(p.sudutPotong * PI / 180.0f);
    if (cosPotong < 0.2f) cosPotong = 0.2f;
    float lajuSentuh = lajuSasaran / (cosPotong * (1.0f + PANTULAN_BOLA) / 2.0f);
    float lajuAwal = lajuUntukJarak(d1 + jarakHenti(lajuSentuh));

    float k = lajuAwal / LAJU_MAKS * 100.0f;
    if (k < 18.0f) k = 18.0f;
    if (k > 100.0f) k = 100.0f;
    return k;
}

// Laju bola putih saat menyentuh sasaran jika dipukul dengan kekuatan tertentu.
float PemainKomputer::lajuSaatMenyentuh(const Pukulan& p, float kekuatan) const
{
    float lajuAwal = LAJU_MAKS * kekuatan / 100.0f;
    float d1 = panjang(p.titikBidik - bola[0]->getPosisi());
    float sisa = jarakHenti(lajuAwal) - d1;
    return (sisa > 0.0f) ? lajuUntukJarak(sisa) : 0.0f;
}

// Fisika berjalan per langkah, jadi tumbukan baru terdeteksi setelah bola putih
// sedikit menembus sasaran, rata rata setengah jarak tempuh satu langkah. Arah
// pantulan sasaran mengikuti garis pusat pada saat terdeteksi, sehingga bidikan
// diarahkan ke titik Q di garis sasaran ke lubang, yaitu posisi bola putih pada
// saat tumbukan terdeteksi. Garis yang melewati Q selalu mengenai sasaran.
float PemainKomputer::sudutBidik(const Pukulan& p, float kekuatan) const
{
    sf::Vector2f c = bola[0]->getPosisi();
    if (p.lubang < 0)
    {
        return sudutArah(p.titikBidik - c);
    }

    sf::Vector2f t = p.sasaran->getPosisi();
    sf::Vector2f n = meja.posisiLubang(p.lubang) - t;
    n /= panjang(n);
    float jarakSentuh = p.sasaran->getJariJari() * 2.0f;
    float tembus = lajuSaatMenyentuh(p, kekuatan) * LANGKAH_FISIKA * 0.5f;

    // f(k) = jarak C ke Q dikurangi (titik sentuh pertama + tembus). Akar f dicari
    // dengan bisection pada k, yaitu jarak Q dari pusat sasaran.
    float bawah = jarakSentuh - 4.0f * tembus - 2.0f;
    if (bawah < 1.0f) bawah = 1.0f;
    float atas = jarakSentuh;
    for (int i = 0; i < 30; i++)
    {
        float k = (bawah + atas) / 2.0f;
        sf::Vector2f q = t - n * k;
        sf::Vector2f u = q - c;
        float jarakQ = panjang(u);
        u /= jarakQ;
        sf::Vector2f tc = t - c;
        float sejajar = dot(tc, u);
        float tegak2 = dot(tc, tc) - sejajar * sejajar;
        float sentuh = sejajar - std::sqrt(jarakSentuh * jarakSentuh - tegak2);
        float f = jarakQ - (sentuh + tembus);
        if (f < 0.0f) atas = k;
        else bawah = k;
    }
    sf::Vector2f q = t - n * ((bawah + atas) / 2.0f);
    return sudutArah(q - c);
}

float PemainKomputer::hitungGalat()
{
    return acak() * GALAT_ARAH[tingkat];
}

void PemainKomputer::rencanakan(const Bola& bolaPutih)
{
    sf::Vector2f c = bolaPutih.getPosisi();

    if (sedangBreak())
    {
        // Pukulan pembuka: bola terdepan rak dipukul sekuat mungkin.
        const Bola* depan = 0;
        for (int i = 0; i < 16; i++)
        {
            if (bola[i]->getNomor() == 0) continue;
            if (depan == 0 || bola[i]->getPosisi().x < depan->getPosisi().x) depan = bola[i];
        }
        rencana.sasaran = depan;
        rencana.lubang = -1;
        rencana.titikBidik = depan->getPosisi();
        rencana.sudutPotong = 0.0f;
        rencana.jarak = panjang(depan->getPosisi() - c);
        rencana.nilai = 0.0f;
        sudutTujuan = sudutArah(rencana.titikBidik - c) + hitungGalat() * 0.5f;
        kekuatanTujuan = (tingkat == Mudah) ? 85.0f : 100.0f;
        return;
    }

    std::vector<Pukulan> kandidat = cariKandidat(bolaPutih);
    rencana = kandidat.empty() ? pukulanCadangan(bolaPutih) : pilihPukulan(kandidat);

    float kekuatanIdeal = hitungKekuatan(rencana);
    sudutTujuan = sudutBidik(rencana, kekuatanIdeal) + hitungGalat();
    kekuatanTujuan = kekuatanIdeal * (1.0f + acak() * GALAT_KEKUATAN[tingkat]);
    if (kekuatanTujuan < 10.0f) kekuatanTujuan = 10.0f;
    if (kekuatanTujuan > 100.0f) kekuatanTujuan = 100.0f;
}

// Komputer berpikir sebentar, memutar stik ke arah bidikan, mengisi kekuatan,
// lalu memukul. Semua tahap terlihat di layar seperti pemain manusia.
bool PemainKomputer::ambilGiliran(Stik& stik, Bola& bolaPutih, float dt)
{
    sf::Vector2f c = bolaPutih.getPosisi();

    if (tanpaJeda)
    {
        rencanakan(bolaPutih);
        stik.bidik(arahSudut(sudutTujuan));
        stik.aturKekuatan(kekuatanTujuan);
        stik.pukul(bolaPutih);
        stik.aturKekuatan(0.0f);
        return true;
    }

    if (tahap == BERPIKIR)
    {
        waktu += dt;
        stik.aturKekuatan(0.0f);
        stik.ikuti(c);
        if (waktu >= JEDA_BERPIKIR)
        {
            rencanakan(bolaPutih);
            sudutAwal = stik.getSudut();
            waktu = 0.0f;
            tahap = MEMBIDIK;
        }
        return false;
    }

    if (tahap == MEMBIDIK)
    {
        waktu += dt;
        float t = waktu / LAMA_MEMBIDIK;
        if (t > 1.0f) t = 1.0f;
        t = t * t * (3.0f - 2.0f * t);

        float selisih = sudutTujuan - sudutAwal;
        while (selisih > 180.0f) selisih -= 360.0f;
        while (selisih < -180.0f) selisih += 360.0f;

        stik.bidik(arahSudut(sudutAwal + selisih * t));
        stik.aturKekuatan(0.0f);
        stik.ikuti(c);
        if (waktu >= LAMA_MEMBIDIK)
        {
            kekuatanSaatIni = 0.0f;
            tahap = MENGISI;
        }
        return false;
    }

    // MENGISI
    kekuatanSaatIni += KECEPATAN_MENGISI * dt;
    bool penuh = kekuatanSaatIni >= kekuatanTujuan;
    if (penuh) kekuatanSaatIni = kekuatanTujuan;

    stik.bidik(arahSudut(sudutTujuan));
    stik.aturKekuatan(kekuatanSaatIni);
    stik.ikuti(c);
    if (!penuh)
    {
        return false;
    }

    stik.pukul(bolaPutih);
    stik.aturKekuatan(0.0f);
    tahap = BERPIKIR;
    waktu = 0.0f;
    return true;
}

// ---------------------------------------------------------------------------
// Bola di tangan (WBS 5.5)

// Posisi boleh ditempati bola putih jika tidak menimpa bola lain dan tidak berada
// di mulut lubang.
bool PemainKomputer::posisiKosong(sf::Vector2f p) const
{
    for (int i = 1; i < 16; i++)
    {
        if (bola[i]->statusBola()) continue;
        sf::Vector2f d = bola[i]->getPosisi() - p;
        float r = bola[i]->getJariJari() * 2.0f + 1.0f;
        if (dot(d, d) < r * r) return false;
    }
    for (int L = 0; L < 6; L++)
    {
        sf::Vector2f d = meja.posisiLubang(L) - p;
        float zona = lubangTengah(L) ? ZONA_TENGAH : ZONA_SUDUT;
        if (dot(d, d) < zona * zona) return false;
    }
    return true;
}

// Nilai sebuah posisi adalah peluang pukulan terbaik dari posisi itu, dihitung
// dengan rumus tingkat Sulit agar semua tingkat menilai dengan ukuran yang sama.
float PemainKomputer::nilaiPosisi(sf::Vector2f p)
{
    bola[0]->aturUlang(p);
    std::vector<Pukulan> kandidat = cariKandidat(*bola[0]);
    float terbaik = 0.0f;
    for (size_t i = 0; i < kandidat.size(); i++)
    {
        float cosPotong = std::cos(kandidat[i].sudutPotong * PI / 180.0f);
        float peluang = cosPotong * cosPotong * std::exp(-kandidat[i].jarak / 1400.0f);
        if (lubangTengah(kandidat[i].lubang)) peluang *= 0.85f;
        if (peluang > terbaik) terbaik = peluang;
    }
    return terbaik;
}

// Komputer mencoba banyak posisi di dalam area, lalu memilih yang memberi pukulan
// paling mudah. Bola putih dipindah sementara selama perhitungan, kemudian
// dikembalikan ke asal agar animasi bergeser dimulai dari tempat semula.
sf::Vector2f PemainKomputer::cariPosisiTaruh(sf::Vector2f asal, const sf::FloatRect& area)
{
    // Rak belum pecah (break gagal menyentuh bola): cukup tetap di garis kepala.
    if (sedangBreak())
    {
        return asal;
    }

    sf::Vector2f terbaik = asal;
    float nilaiTerbaik = posisiKosong(asal) ? nilaiPosisi(asal) : -1.0f;

    if (CONTOH_POSISI[tingkat] > 0)
    {
        for (int i = 0; i < CONTOH_POSISI[tingkat]; i++)
        {
            sf::Vector2f p(area.left + (acak() + 1.0f) * 0.5f * area.width,
                           area.top + (acak() + 1.0f) * 0.5f * area.height);
            if (!posisiKosong(p)) continue;
            float n = nilaiPosisi(p);
            if (n > nilaiTerbaik)
            {
                nilaiTerbaik = n;
                terbaik = p;
            }
        }
    }
    else
    {
        for (float x = area.left; x <= area.left + area.width; x += JARAK_KISI)
        {
            for (float y = area.top; y <= area.top + area.height; y += JARAK_KISI)
            {
                sf::Vector2f p(x, y);
                if (!posisiKosong(p)) continue;
                float n = nilaiPosisi(p);
                if (n > nilaiTerbaik)
                {
                    nilaiTerbaik = n;
                    terbaik = p;
                }
            }
        }
    }

    bola[0]->aturUlang(asal);
    return terbaik;
}

// Komputer berpikir sebentar, lalu menggeser bola putih ke posisi pilihannya.
bool PemainKomputer::taruhBolaPutih(sf::Vector2f& posisi, const sf::FloatRect& area, float dt)
{
    if (tanpaJeda)
    {
        posisi = cariPosisiTaruh(posisi, area);
        return true;
    }

    if (tahapTaruh == TARUH_BERPIKIR)
    {
        waktu += dt;
        if (waktu >= JEDA_BERPIKIR)
        {
            asalTaruh = posisi;
            tujuanTaruh = cariPosisiTaruh(posisi, area);
            waktu = 0.0f;
            tahapTaruh = TARUH_MENGGESER;
        }
        return false;
    }

    waktu += dt;
    float t = waktu / LAMA_MENGGESER;
    if (t > 1.0f) t = 1.0f;
    t = t * t * (3.0f - 2.0f * t);
    posisi = asalTaruh + (tujuanTaruh - asalTaruh) * t;
    if (waktu < LAMA_MENGGESER)
    {
        return false;
    }

    posisi = tujuanTaruh;
    tahapTaruh = TARUH_BERPIKIR;
    waktu = 0.0f;
    return true;
}
