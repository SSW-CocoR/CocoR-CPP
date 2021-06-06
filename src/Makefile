all:
	g++ -g -Wall -fno-rtti -fno-exceptions  *.cpp -o Coco $(CFLAGS)
	#x86_64-w64-mingw32-g++ -g -Wall -fno-rtti -fno-exceptions  *.cpp -o Coco.exe $(CFLAGS)

clean:
	rm -f Coco

install:
	ln -s /usr/lib/coco-cpp/Coco $(DESTDIR)/usr/bin/cococpp
	install -m 0755 Coco $(DESTDIR)/usr/lib/coco-cpp
	install -m 0644 *frame $(DESTDIR)/usr/share/coco-cpp

