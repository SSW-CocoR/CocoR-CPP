# run gcc compiler in freestanding mode
optim=-O2
g++ -g $optim -static -fno-pie -nostdlib -nostdinc \
  -fno-omit-frame-pointer -pg -mnop-mcount -mno-tls-direct-seg-refs \
  -fno-exceptions -fno-rtti -Wall -fno-strict-aliasing \
  -o cocor.com.dbg cocor-am.cpp \
  -DWITHOUT_WCHAR \
  -DWITH_COSMOPOLITAN \
  -Wl,--gc-sections -fuse-ld=bfd \
  -Wl,-T,ape.lds crt.o ape-no-modify-self.o cosmopolitan.a \
   -include cosmopolitan.h

objcopy -S -O binary cocor.com.dbg cocor.com

