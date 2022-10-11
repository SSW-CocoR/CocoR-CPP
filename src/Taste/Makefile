all: Parser.cpp
	g++ -g *.cpp -o Taste $(CFLAGS)

Parser.cpp: Taste.atg
	../Coco -frames .. Taste.atg

clean:
	rm -f Taste

