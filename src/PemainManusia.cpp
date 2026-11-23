#include "PemainManusia.h"

namespace
{
const float KEKUATAN_PER_DETIK = 70.0f;
const float KEKUATAN_MIN = 3.0f;   // pukulan di bawah nilai ini dianggap batal
}

PemainManusia::PemainManusia(const std::string& namaAwal, sf::Vector2f arahAwal)
    : Pemain(namaAwal), arahMouse(arahAwal), menekan(false), siapMemukul(false), kekuatan(0.0f)
{
}

// Permainan hanya meneruskan klik kepada pemain yang sedang mendapat giliran,
// sedangkan gerakan mouse diteruskan kepada semua pemain manusia.
void PemainManusia::bacaEvent(const sf::Event& e)
{
    if (e.type == sf::Event::MouseMoved)
    {
        arahMouse = sf::Vector2f(static_cast<float>(e.mouseMove.x), static_cast<float>(e.mouseMove.y));
    }
    else if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left)
    {
        menekan = true;
    }
    else if (e.type == sf::Event::MouseButtonReleased && e.mouseButton.button == sf::Mouse::Left)
    {
        if (menekan && kekuatan >= KEKUATAN_MIN)
        {
            siapMemukul = true;
        }
        else
        {
            kekuatan = 0.0f;
        }
        menekan = false;
    }
}

bool PemainManusia::ambilGiliran(Stik& stik, Bola& bolaPutih, float dt)
{
    if (siapMemukul)
    {
        // Stik masih memegang kekuatan dari frame sebelumnya, lalu dikosongkan.
        stik.pukul(bolaPutih);
        siapMemukul = false;
        kekuatan = 0.0f;
        stik.aturKekuatan(kekuatan);
        return true;
    }

    if (menekan)
    {
        kekuatan += KEKUATAN_PER_DETIK * dt;
        if (kekuatan > 100.0f)
        {
            kekuatan = 100.0f;
        }
    }

    stik.bidik(arahMouse - bolaPutih.getPosisi());
    stik.aturKekuatan(kekuatan);
    stik.ikuti(bolaPutih.getPosisi());
    return false;
}

// Bola putih mengikuti kursor. Klik kiri memilih posisi; klik itu langsung dipakai
// habis agar tidak ikut mengisi kekuatan pukulan berikutnya.
bool PemainManusia::taruhBolaPutih(sf::Vector2f& posisi, const sf::FloatRect& area, float dt)
{
    (void)area;
    (void)dt;
    posisi = arahMouse;
    if (menekan)
    {
        menekan = false;
        kekuatan = 0.0f;
        return true;
    }
    return false;
}

void PemainManusia::batalkan()
{
    menekan = false;
    siapMemukul = false;
    kekuatan = 0.0f;
}
