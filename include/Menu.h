#pragma once
#include "Tombol.h"
#include "Pengaturan.h"
#include <SFML/Graphics.hpp>
#include <string>

enum LayarAktif
{
    LAYAR_MENU_UTAMA,
    LAYAR_PILIH_MODE,
    LAYAR_SETUP_PEMAIN,
    LAYAR_PENGATURAN,
    LAYAR_BERMAIN,
    LAYAR_JEDA,
    LAYAR_PAPAN_SKOR
};

enum AksiMenu
{
    AKSI_TIDAK_ADA,
    AKSI_MULAI_PERTANDINGAN,
    AKSI_LANJUTKAN,
    AKSI_ULANG_PERTANDINGAN,
    AKSI_TERAPKAN_PENGATURAN,
    AKSI_KELUAR
};

// Mengelola seluruh layar antarmuka (WBS 5.1): Menu Utama, Setup Pemain,
// Pengaturan, Menu Jeda, dan kerangka Papan Skor.
class Menu
{
private:
    LayarAktif layar;
    LayarAktif layarSebelumPengaturan;
    sf::Font font;
    bool fontSiap;

    // Latar & dekorasi
    sf::Texture texBgArcade;
    sf::Sprite sprBgArcade;
    sf::Texture texBgMainMenu;
    sf::Sprite sprBgMainMenu;
    sf::Texture texPapanJeda;
    sf::Sprite sprPapanJeda;
    sf::Texture texInputBox;
    sf::Sprite sprInputBox;
    sf::Texture texPensil;
    sf::Sprite sprPensil;
    sf::Texture texGemCyan;
    sf::Sprite sprGemCyan;
    sf::Texture texGemPink;
    sf::Sprite sprGemPink;
    sf::Texture texStarCluster;
    sf::Sprite sprStarCluster;
    sf::Texture texChalkBlue;
    sf::Sprite sprChalkBlue;
    sf::Texture texCueDeco;
    sf::Sprite sprCueDeco;
    sf::Texture texStarBadge;
    sf::Sprite sprStarBadge;

    // Menu Utama
    Tombol btnMulaiMain;
    Tombol btnPengaturan;
    Tombol btnPapanSkor;
    Tombol btnKeluar;

    // Pilih Mode: 1 lawan 1 dengan manusia atau melawan komputer
    sf::Text teksJudulMode;
    sf::Text teksDescManusia;
    sf::Text teksDescKomputer;
    Tombol btnModeManusia;
    Tombol btnModeKomputer;
    Tombol btnModeKembali;

    // Setup Pemain
    std::string namaPemain;
    int tingkatKesulitan; // 0 = Mudah, 1 = Sedang, 2 = Sulit
    bool lawanKomputer;   // mode yang dipilih di layar Pilih Mode
    bool inputNamaAktif;
    std::string namaPemain2;  // dipakai pada mode 1 lawan 1 manusia
    bool inputNama2Aktif;
    sf::Text teksLabelNama2;
    sf::Text teksInputNama2;
    sf::Sprite sprInputBox2;
    sf::Sprite sprPensil2;
    sf::RectangleShape kursorInput2;
    sf::Text teksJudulSetup;
    sf::Text teksLabelNama;
    sf::Text teksInputNama;
    sf::Text teksLabelKesulitan;
    sf::RectangleShape kursorInput;
    float waktuKursor;
    Tombol btnDiff[3];
    sf::Texture texDiffAktif[3];
    sf::Texture texDiffNormal[3];
    Tombol btnSetupKembali;
    Tombol btnSetupMulai;

    // Pengaturan
    Pengaturan* pengaturan;
    std::string draftWarna;
    int draftSfx;
    int draftBgm;
    int sliderAktif; // -1 = tidak ada, 0 = SFX, 1 = BGM
    sf::Text teksJudulPengaturan;
    sf::Text teksLabelKain;
    sf::Text teksLabelSfx;
    sf::Text teksLabelBgm;
    sf::Text teksPersenSfx;
    sf::Text teksPersenBgm;
    Tombol btnKain[3];
    sf::Texture texKainAktif[3];
    sf::Texture texKainNormal[3];
    sf::Texture texSliderTrack;
    sf::Texture texSliderFill;
    sf::Texture texSliderThumb;
    sf::Sprite sprTrackSfx;
    sf::Sprite sprFillSfx;
    sf::Sprite sprThumbSfx;
    sf::Sprite sprTrackBgm;
    sf::Sprite sprFillBgm;
    sf::Sprite sprThumbBgm;
    Tombol btnPengaturanKembali;
    Tombol btnPengaturanSimpan;

    // Menu Jeda
    sf::RectangleShape overlayGelap;
    sf::Text teksJudulJeda;
    Tombol btnJedaLanjut;
    Tombol btnJedaPengaturan;
    Tombol btnJedaMenuUtama;

    // Papan Skor (kerangka UI; data file dikerjakan WBS 5.4)
    sf::Text teksJudulSkor;
    sf::Texture texHeaderTabel;
    sf::Sprite sprHeaderTabel;
    sf::Texture texBarisTop;
    sf::Texture texBarisNormal;
    sf::Texture texMedali[3];
    sf::Texture texAvatar[5];
    Tombol btnSkorKembali;

    void aturTeksJudul(sf::Text& t, const std::string& isi, unsigned int ukuran, sf::Vector2f tengah);
    void aturTeksLabel(sf::Text& t, const std::string& isi, unsigned int ukuran, sf::Vector2f posisi);
    void perbaruiTampilanKesulitan();
    void ketikNama(std::string& nama, sf::Text& teks, sf::Uint32 ch);
    void perbaruiTampilanKain();
    void perbaruiSliderDariMouse(float mouseX);
    void sinkronSliderVisual();

public:
    Menu(Pengaturan* cfg);

    LayarAktif getLayar() const;
    void aturLayar(LayarAktif l);
    const sf::Font& getFont() const;

    std::string getNamaPemain() const;
    std::string getNamaPemain2() const;
    int getTingkatKesulitan() const;
    bool apakahLawanKomputer() const;

    AksiMenu prosesEvent(const sf::Event& e);
    void update(float dt);
    void gambar(sf::RenderWindow& w) const;
};
