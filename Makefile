# Makefile for 'FS', filesearch.

compiler=gcc
analysis=-g -DDEBUG -O0
noexe=-c
name=filesearch


fs: ./fs.c ./fs.h stringy colour wregex

	$(compiler) $(noexe) $(analysis) -o ./fs.o
	$(compiler) $(analysis) ./../stringy/stringyd.o ./../colour/colour_d.o ./fs.o ./../regex_w/libd.o -std=gnu++11 -lole32 -loleaut32  -o $(name).exe

stringy:
	gnumake -C ./../stringy/ stringyd

colour:
	gnumake -C ./../colour/ colour_d

wregex:
	gnumake -C ./../regex_w/ libd
	
clean:
	rm *.o
	rm *.exe
	
wipe: clean
	make -C ./../stringy/ clean
	make -C ./../coloiur/ clean
	make -C ./../regex_w/ clean

