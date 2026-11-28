#include "PopupPemenang.h"
#include <cmath>
#include <cstdio>

namespace
{
const float PI = 3.14159265f;
const sf::Vector2f PUSAT_LAYAR(512.0f, 320.0f);
const float SKALA_TINGGI_PAPAN = 1.2f;   // papan kayu menu jeda dipanjangkan ke bawah

// Urutan animasi, dalam detik sejak pertandingan selesai.
const float JEDA_MUNCUL = 0.6f;          // bola terakhir sempat terlihat masuk dulu
const float LAMA_REDUP = 0.3f;
const float LAMA_LONCAT = 0.45f;
const float LAMA_HUJAN = 2.5f;

const float GRAVITASI = 520.0f;

// Warna konfeti diambil dari warna bola biliar.
const sf::Color WARNA_KONFETI[9] = {
    sf::Color(250, 205, 30), sf::Color(40, 90, 220), sf::Color(220, 40, 40),
    sf::Color(120, 50, 170), sf::Color(245, 120, 30), sf::Color(30, 150, 70),
    sf::Color(140, 30, 40), sf::Color(255, 255, 255), sf::Color(255, 225, 120)
};

// Pergerakan "meloncat": membesar melewati ukuran akhir sedikit, lalu kembali.
float easeOutBack(float t)
{
    const float c1 = 1.70158f;
    const float c3 = c1 + 1.0f;
    float u = t - 1.0f;
    return 1.0f + c3 * u * u * u + c1 * u * u;
}
}

PopupPemenang::PopupPemenang()
    : fontSiap(false), aktif(false), menangKomputer(false), waktu(0.0f), waktuKonfeti(-1.0f),
      benih(20261123UL)
{
    fontSiap = font.loadFromFile("assets/fonts/LemonDays.ttf");

    if (texPapan.loadFromFile("assets/ui/wooden_board_pause.png")) texPapan.setSmooth(true);
    if (texAvatarManusia.loadFromFile("assets/ui/hud_avatar_p1_active.png")) texAvatarManusia.setSmooth(true);
    if (texAvatarKomputer.loadFromFile("assets/ui/hud_avatar_ai_active.png")) texAvatarKomputer.setSmooth(true);
    if (texMedali.loadFromFile("assets/ui/medal_1_gold.png")) texMedali.setSmooth(true);
    if (texBintang.loadFromFile("assets/ui/deco_star_cluster.png")) texBintang.setSmooth(true);

    // Semua posisi diukur dari pusat layar karena seluruh isi papan ikut
    // diperbesar dari titik itu saat animasi meloncat.
    sprPapan.setTexture(texPapan);
    sf::Vector2u up = texPapan.getSize();
    sprPapan.setOrigin(up.x / 2.0f, up.y / 2.0f);
    sprPapan.setScale(1.0f, SKALA_TINGGI_PAPAN);
    sprPapan.setPosition(PUSAT_LAYAR);

    sprAvatar.setScale(1.55f, 1.55f);
    sprAvatar.setPosition(512.0f, 236.0f);

    sprMedali.setTexture(texMedali);
    sprMedali.setOrigin(18.0f, 21.0f);
    sprMedali.setScale(1.15f, 1.15f);
    sprMedali.setPosition(552.0f, 268.0f);

    sprBintangKiri.setTexture(texBintang);
    sprBintangKiri.setOrigin(45.0f, 35.0f);
    sprBintangKiri.setPosition(390.0f, 236.0f);
    sprBintangKanan = sprBintangKiri;
    sprBintangKanan.setScale(-1.0f, 1.0f);
    sprBintangKanan.setPosition(634.0f, 236.0f);

    aturTeks(teksJudul, 46, sf::Color(255, 212, 0), sf::Color(45, 25, 8), 4.0f);
    aturTeks(teksNama, 34, sf::Color::White, sf::Color(45, 25, 8), 3.0f);
    aturTeks(teksAlasan, 15, sf::Color(255, 236, 205), sf::Color(45, 25, 8), 2.0f);
    aturTeks(teksSkor, 20, sf::Color(255, 212, 0), sf::Color(45, 25, 8), 2.5f);

    btnMainLagi.muat("assets/ui/btn_390x68_yellow_normal.png", "assets/ui/btn_390x68_yellow_hover.png",
                     font, "MAIN LAGI", 24, sf::Vector2f(317.0f, 398.0f));
    btnMenuUtama.muat("assets/ui/btn_390x68_dark_normal.png", "assets/ui/btn_390x68_dark_hover.png",
                      font, "MENU UTAMA", 24, sf::Vector2f(317.0f, 472.0f));
}

void PopupPemenang::aturTeks(sf::Text& t, unsigned int ukuran, sf::Color isi, sf::Color garis, float tebal)
{
    t.setFont(font);
    t.setCharacterSize(ukuran);
    t.setFillColor(isi);
    t.setOutlineColor(garis);
    t.setOutlineThickness(tebal);
}

// Menaruh teks di tengah titik (x, y) dan mengecilkannya bila lebih lebar dari batas.
void PopupPemenang::pusatkan(sf::Text& t, float x, float y, float lebarMaks)
{
    t.setScale(1.0f, 1.0f);
    sf::FloatRect b = t.getLocalBounds();
    t.setOrigin(b.left + b.width / 2.0f, b.top + b.height / 2.0f);
    t.setPosition(x, y);
    if (b.width > lebarMaks)
    {
        float s = lebarMaks / b.width;
        t.setScale(s, s);
    }
}

float PopupPemenang::acak(float bawah, float atas)
{
    benih = benih * 1103515245UL + 12345UL;
    float r = static_cast<float>((benih >> 16) & 0x7FFFUL) / 32767.0f;
    return bawah + (atas - bawah) * r;
}

void PopupPemenang::tampilkan(const Permainan& permainan, int poinPapanSkor)
{
    const Pemain* juara = permainan.getWasit().pemenang();
    if (juara == 0)
    {
        return;
    }
    const Pemain* kalah = (juara == &permainan.getPemain1()) ? &permainan.getPemain2() : &permainan.getPemain1();

    aktif = true;
    waktu = 0.0f;
    waktuKonfeti = -1.0f;
    konfeti.clear();
    menangKomputer = juara->apakahKomputer();

    teksJudul.setString(menangKomputer ? "KOMPUTER MENANG" : "PEMENANG!");
    teksJudul.setFillColor(menangKomputer ? sf::Color(130, 205, 255) : sf::Color(255, 212, 0));
    pusatkan(teksJudul, 512.0f, 128.0f, 440.0f);

    teksNama.setString(juara->getNama());
    pusatkan(teksNama, 512.0f, 314.0f, 420.0f);

    teksAlasan.setString(permainan.getWasit().getKeterangan());
    pusatkan(teksAlasan, 512.0f, 346.0f, 430.0f);

    char buf[64];
    if (!menangKomputer && poinPapanSkor > 0)
    {
        std::sprintf(buf, "Skor akhir   %d : %d   |   +%d poin",
                     juara->getSkor(), kalah->getSkor(), poinPapanSkor);
    }
    else
    {
        std::sprintf(buf, "Skor akhir   %d : %d", juara->getSkor(), kalah->getSkor());
    }
    teksSkor.setString(buf);
    pusatkan(teksSkor, 512.0f, 372.0f, 420.0f);

    sprAvatar.setTexture(menangKomputer ? texAvatarKomputer : texAvatarManusia, true);
    sf::Vector2u ua = sprAvatar.getTexture()->getSize();
    sprAvatar.setOrigin(ua.x / 2.0f, ua.y / 2.0f);

    btnMainLagi.aturLabel(menangKomputer ? "COBA LAGI" : "MAIN LAGI");
}

void PopupPemenang::sembunyikan()
{
    aktif = false;
    konfeti.clear();
}

bool PopupPemenang::apakahAktif() const
{
    return aktif;
}

// Tombol baru bisa dipakai setelah papan selesai meloncat masuk.
AksiPopup PopupPemenang::prosesEvent(const sf::Event& e)
{
    if (!aktif || waktu < JEDA_MUNCUL + LAMA_LONCAT)
    {
        return POPUP_TIDAK_ADA;
    }
    if (e.type == sf::Event::MouseMoved)
    {
        sf::Vector2f m(static_cast<float>(e.mouseMove.x), static_cast<float>(e.mouseMove.y));
        btnMainLagi.perbaruiHover(m);
        btnMenuUtama.perbaruiHover(m);
    }
    if (btnMainLagi.diklik(e))
    {
        return POPUP_MAIN_LAGI;
    }
    if (btnMenuUtama.diklik(e))
    {
        return POPUP_MENU_UTAMA;
    }
    if (e.type == sf::Event::KeyPressed &&
        (e.key.code == sf::Keyboard::Return || e.key.code == sf::Keyboard::R))
    {
        return POPUP_MAIN_LAGI;
    }
    return POPUP_TIDAK_ADA;
}

// Konfeti ditembakkan miring ke atas dari satu sudut; arahX +1 ke kanan, -1 ke kiri.
void PopupPemenang::tembakKonfeti(sf::Vector2f asal, float arahX, int jumlah)
{
    for (int i = 0; i < jumlah; i++)
    {
        Konfeti k;
        k.posisi = asal;
        k.kecepatan = sf::Vector2f(arahX * acak(120.0f, 470.0f), -acak(560.0f, 920.0f));
        k.sudut = acak(0.0f, 360.0f);
        k.putaran = acak(-420.0f, 420.0f);
        k.fase = acak(0.0f, 2.0f * PI);
        k.ukuran = sf::Vector2f(acak(6.0f, 10.0f), acak(10.0f, 16.0f));
        k.warna = WARNA_KONFETI[static_cast<int>(acak(0.0f, 8.99f))];
        konfeti.push_back(k);
    }
}

void PopupPemenang::hujanKonfeti(int jumlah)
{
    for (int i = 0; i < jumlah; i++)
    {
        Konfeti k;
        k.posisi = sf::Vector2f(acak(0.0f, 1024.0f), -20.0f);
        k.kecepatan = sf::Vector2f(acak(-40.0f, 40.0f), acak(60.0f, 160.0f));
        k.sudut = acak(0.0f, 360.0f);
        k.putaran = acak(-300.0f, 300.0f);
        k.fase = acak(0.0f, 2.0f * PI);
        k.ukuran = sf::Vector2f(acak(6.0f, 9.0f), acak(9.0f, 14.0f));
        k.warna = WARNA_KONFETI[static_cast<int>(acak(0.0f, 8.99f))];
        konfeti.push_back(k);
    }
}

void PopupPemenang::update(float dt)
{
    if (!aktif)
    {
        return;
    }
    waktu += dt;

    // Ledakan konfeti dari kedua sudut bawah saat papan mulai muncul, lalu
    // hujan konfeti tipis dari atas selama beberapa detik.
    if (!menangKomputer)
    {
        if (waktuKonfeti < 0.0f && waktu >= JEDA_MUNCUL + 0.15f)
        {
            tembakKonfeti(sf::Vector2f(0.0f, 650.0f), 1.0f, 90);
            tembakKonfeti(sf::Vector2f(1024.0f, 650.0f), -1.0f, 90);
            waktuKonfeti = LAMA_HUJAN;
        }
        else if (waktuKonfeti > 0.0f)
        {
            waktuKonfeti -= dt;
            if (acak(0.0f, 1.0f) < 0.6f)
            {
                hujanKonfeti(1);
            }
        }
    }

    for (size_t i = 0; i < konfeti.size(); )
    {
        Konfeti& k = konfeti[i];
        k.kecepatan.y += GRAVITASI * dt;
        k.kecepatan.x *= (1.0f - 1.2f * dt);              // hambatan udara
        if (k.kecepatan.y > 190.0f) k.kecepatan.y = 190.0f; // kertas jatuh pelan
        k.posisi += k.kecepatan * dt;
        k.posisi.x += std::sin(waktu * 4.0f + k.fase) * 35.0f * dt;
        k.sudut += k.putaran * dt;
        if (k.posisi.y > 680.0f)
        {
            konfeti[i] = konfeti.back();
            konfeti.pop_back();
        }
        else
        {
            i++;
        }
    }
}

float PopupPemenang::skalaMuncul() const
{
    float t = (waktu - JEDA_MUNCUL) / LAMA_LONCAT;
    if (t <= 0.0f) return 0.0f;
    if (t >= 1.0f) return 1.0f;
    return easeOutBack(t);
}

// Sinar berbentuk segitiga tipis yang memancar dari satu titik.
void PopupPemenang::gambarSinar(sf::RenderWindow& w, sf::Vector2f pusat, float panjang, int jumlah,
                                sf::Color warna, float sudutAwal) const
{
    float lebar = 360.0f / jumlah / 4.0f;
    for (int i = 0; i < jumlah; i++)
    {
        float a = (sudutAwal + i * 360.0f / jumlah) * PI / 180.0f;
        float b = lebar * PI / 180.0f;
        sf::ConvexShape sinar(3);
        sinar.setPoint(0, pusat);
        sinar.setPoint(1, pusat + sf::Vector2f(std::cos(a - b), std::sin(a - b)) * panjang);
        sinar.setPoint(2, pusat + sf::Vector2f(std::cos(a + b), std::sin(a + b)) * panjang);
        sinar.setFillColor(warna);
        w.draw(sinar);
    }
}

void PopupPemenang::gambar(sf::RenderWindow& w)
{
    if (!aktif || waktu < JEDA_MUNCUL)
    {
        return;
    }

    float tampak = (waktu - JEDA_MUNCUL) / LAMA_REDUP;
    if (tampak > 1.0f) tampak = 1.0f;

    sf::RectangleShape redup(sf::Vector2f(1024.0f, 640.0f));
    redup.setFillColor(sf::Color(8, 12, 22, static_cast<sf::Uint8>(175 * tampak)));
    w.draw(redup);

    float s = skalaMuncul();
    if (s <= 0.0f)
    {
        return;
    }

    // Sinar besar di belakang papan, berputar pelan.
    sf::Color warnaSinar = menangKomputer ? sf::Color(110, 180, 255, 32) : sf::Color(255, 215, 60, 38);
    gambarSinar(w, PUSAT_LAYAR, 760.0f * s, 16, warnaSinar, waktu * 12.0f);

    // Isi papan digambar dengan transformasi skala dari pusat layar.
    sf::Transform tf;
    tf.scale(s, s, PUSAT_LAYAR.x, PUSAT_LAYAR.y);
    sf::RenderStates rs(tf);

    w.draw(sprPapan, rs);

    sf::Color warnaCahaya = menangKomputer ? sf::Color(130, 200, 255, 70) : sf::Color(255, 230, 120, 80);
    sf::Transform tfSinar = tf;
    sf::RenderStates rsSinar(tfSinar);
    // Sinar kecil di belakang avatar berputar berlawanan arah.
    {
        float lebar = 360.0f / 12 / 4.0f;
        sf::Vector2f pusat(512.0f, 236.0f);
        for (int i = 0; i < 12; i++)
        {
            float a = (-waktu * 25.0f + i * 30.0f) * PI / 180.0f;
            float b = lebar * PI / 180.0f;
            sf::ConvexShape sinar(3);
            sinar.setPoint(0, pusat);
            sinar.setPoint(1, pusat + sf::Vector2f(std::cos(a - b), std::sin(a - b)) * 92.0f);
            sinar.setPoint(2, pusat + sf::Vector2f(std::cos(a + b), std::sin(a + b)) * 92.0f);
            sinar.setFillColor(warnaCahaya);
            w.draw(sinar, rsSinar);
        }
    }

    sf::CircleShape cincin(48.0f);
    cincin.setOrigin(48.0f, 48.0f);
    cincin.setPosition(512.0f, 236.0f);
    cincin.setFillColor(sf::Color(30, 43, 60));
    cincin.setOutlineThickness(5.0f);
    cincin.setOutlineColor(menangKomputer ? sf::Color(110, 180, 255) : sf::Color(255, 205, 40));
    w.draw(cincin, rs);
    w.draw(sprAvatar, rs);
    if (!menangKomputer)
    {
        w.draw(sprMedali, rs);
        w.draw(sprBintangKiri, rs);
        w.draw(sprBintangKanan, rs);
    }

    // Judul berdenyut pelan.
    sf::Transform tfJudul = tf;
    float denyut = 1.0f + 0.045f * std::sin(waktu * 4.0f);
    tfJudul.scale(denyut, denyut, 512.0f, 128.0f);
    w.draw(teksJudul, sf::RenderStates(tfJudul));

    w.draw(teksNama, rs);
    w.draw(teksAlasan, rs);
    w.draw(teksSkor, rs);

    if (s >= 1.0f && waktu >= JEDA_MUNCUL + LAMA_LONCAT)
    {
        btnMainLagi.gambar(w);
        btnMenuUtama.gambar(w);
    }

    // Konfeti paling depan. Lebarnya mengikuti cos agar tampak berbalik di udara.
    for (size_t i = 0; i < konfeti.size(); i++)
    {
        const Konfeti& k = konfeti[i];
        float balik = std::fabs(std::cos(waktu * 6.0f + k.fase));
        if (balik < 0.2f) balik = 0.2f;
        sf::RectangleShape r(sf::Vector2f(k.ukuran.x * balik, k.ukuran.y));
        r.setOrigin(r.getSize().x / 2.0f, r.getSize().y / 2.0f);
        r.setPosition(k.posisi);
        r.setRotation(k.sudut);
        r.setFillColor(k.warna);
        w.draw(r);
    }
}
