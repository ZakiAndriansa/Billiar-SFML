#include "Wasit.h"

namespace
{
JenisBola jenisNomor(int nomor)
{
    if (nomor == 0) return Putih;
    if (nomor == 8) return Hitam;
    return (nomor < 8) ? Polos : Belang;
}

std::string namaJenis(JenisBola j)
{
    if (j == Polos) return "polos (1-7)";
    if (j == Belang) return "belang (9-15)";
    return "bola 8";
}
}

Wasit::Wasit(Pemain* p1, Pemain* p2)
{
    pemain[0] = p1;
    pemain[1] = p2;
    mulai();
}

void Wasit::mulai()
{
    giliran = 0;
    jenisDitentukan = false;
    pelanggaran = false;
    juara = 0;
    keterangan = "";
    pemain[0]->aturUlang();
    pemain[1]->aturUlang();
}

int Wasit::sisaBola(JenisBola j, Bola* const bola[16]) const
{
    int n = 0;
    for (int i = 0; i < 16; i++)
    {
        if (bola[i]->getJenis() == j && !bola[i]->statusBola())
        {
            n++;
        }
    }
    return n;
}

void Wasit::gantiGiliran()
{
    giliran = 1 - giliran;
}

void Wasit::periksaPukulan(int sentuhanPertama, const std::vector<int>& bolaMasuk, Bola* const bola[16])
{
    Pemain* p = pemain[giliran];
    Pemain* lawan = pemain[1 - giliran];
    pelanggaran = false;
    keterangan = "";

    bool putihMasuk = false;
    bool delapanMasuk = false;
    for (size_t i = 0; i < bolaMasuk.size(); i++)
    {
        if (bolaMasuk[i] == 0) putihMasuk = true;
        if (bolaMasuk[i] == 8) delapanMasuk = true;
    }

    // Jumlah bola milik pemain yang masih di meja sebelum pukulan ini.
    int sisaSebelum = -1;
    if (jenisDitentukan)
    {
        sisaSebelum = sisaBola(p->getJenisBola(), bola);
        for (size_t i = 0; i < bolaMasuk.size(); i++)
        {
            if (jenisNomor(bolaMasuk[i]) == p->getJenisBola()) sisaSebelum++;
        }
    }

    // Pelanggaran.
    if (sentuhanPertama < 0)
    {
        pelanggaran = true;
        keterangan = "Pelanggaran: bola putih tidak menyentuh bola lain.";
    }
    else
    {
        JenisBola pertama = jenisNomor(sentuhanPertama);
        if (!jenisDitentukan && pertama == Hitam)
        {
            pelanggaran = true;
            keterangan = "Pelanggaran: bola 8 disentuh lebih dulu.";
        }
        else if (jenisDitentukan)
        {
            JenisBola sasaran = (sisaSebelum == 0) ? Hitam : p->getJenisBola();
            if (pertama != sasaran)
            {
                pelanggaran = true;
                keterangan = "Pelanggaran: bola pertama yang disentuh bukan " + namaJenis(sasaran) + ".";
            }
        }
    }
    if (putihMasuk)
    {
        pelanggaran = true;
        keterangan = "Pelanggaran: bola putih masuk lubang.";
    }

    // Bola 8 menentukan akhir pertandingan.
    if (delapanMasuk)
    {
        if (!pelanggaran && jenisDitentukan && sisaSebelum == 0)
        {
            juara = p;
            keterangan = p->getNama() + " memasukkan bola 8 dan menang.";
        }
        else
        {
            juara = lawan;
            keterangan = "Bola 8 masuk sebelum waktunya. " + lawan->getNama() + " menang.";
        }
        return;
    }

    // Pemasukan sah pertama menentukan jenis bola kedua pemain.
    if (!jenisDitentukan && !pelanggaran)
    {
        for (size_t i = 0; i < bolaMasuk.size(); i++)
        {
            JenisBola j = jenisNomor(bolaMasuk[i]);
            if (j == Polos || j == Belang)
            {
                p->aturJenisBola(j);
                lawan->aturJenisBola(j == Polos ? Belang : Polos);
                jenisDitentukan = true;
                keterangan = p->getNama() + " mendapat bola " + namaJenis(j) + ".";
                break;
            }
        }
    }

    // Skor adalah jumlah bola milik sendiri yang sudah masuk.
    if (jenisDitentukan)
    {
        for (int k = 0; k < 2; k++)
        {
            int target = 7 - sisaBola(pemain[k]->getJenisBola(), bola);
            pemain[k]->tambahSkor(target - pemain[k]->getSkor());
        }
    }

    // Pemain tetap bergiliran jika memasukkan bola miliknya tanpa pelanggaran.
    bool lanjut = false;
    if (!pelanggaran && jenisDitentukan)
    {
        for (size_t i = 0; i < bolaMasuk.size(); i++)
        {
            if (jenisNomor(bolaMasuk[i]) == p->getJenisBola()) lanjut = true;
        }
    }
    if (!lanjut)
    {
        gantiGiliran();
    }
}

Pemain* Wasit::getGiliran() const
{
    return pemain[giliran];
}

bool Wasit::adaPelanggaran() const
{
    return pelanggaran;
}

Pemain* Wasit::pemenang() const
{
    return juara;
}

std::string Wasit::getKeterangan() const
{
    return keterangan;
}
