@echo off
echo ===================================================
echo Mengompilasi Proyek Billiard SFML (Dev-C++ MinGW)
echo ===================================================

"C:\Program Files (x86)\Dev-Cpp\MinGW64\bin\g++.exe" -I"include" -I"lib/SFML/include" -L"lib/SFML/lib" src/main.cpp src/Permainan.cpp src/Bola.cpp src/Meja.cpp src/Stik.cpp src/Pemain.cpp src/Wasit.cpp src/Tombol.cpp src/Pengaturan.cpp src/PengelolaSuara.cpp src/PapanSkor.cpp src/Menu.cpp src/HUD.cpp src/PemainManusia.cpp src/PemainKomputer.cpp src/PopupPemenang.cpp -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system -o BilliardGame.exe

if %ERRORLEVEL% EQU 0 (
    echo [SUKSES] Kompilasi berhasil! Menjalankan BilliardGame.exe...
    start BilliardGame.exe
) else (
    echo [GAGAL] Terjadi kesalahan kompilasi.
    pause
)
