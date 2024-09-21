fs: fs.o str.o barnyard.o ansivt2.o error.o regex.o searchfile.o
	gcc -Wall   -mconsole  barnyard.o str.o fs.o ansivt2.o searchfile.o error.o regex/regex.o  -lole32 -loleaut32  -o fs.exe

searchfile.o: searchfile.c
	gcc -Wall   -c -o searchfile.o searchfile.c

fs.o: fs.c fs.h barnyard.h ansivt2.h
	gcc -Wall   -c -o fs.o fs.c 

str.o: str.c str.h barnyard.h ansivt2.h
	gcc -Wall   -c -o str.o str.c

barnyard.o: barnyard.c barnyard.h
	gcc -Wall   -c -o barnyard.o barnyard.c
	
ansivt2.o: ansivt2.c ansivt2.h
	gcc -Wall   -c -o ansivt2.o ansivt2.c
	
error.o: error.c error.h
	gcc -Wall   -c -o error.o error.c

regex.o:
	make -C regex lib

fsd: fsd.o strd.o barnyardd.o ansivt2d.o errord.o regexd.o searchfiled.o
	gcc -Wall   -mconsole -Wall    -std=gnu++11 -DDEBUG -g barnyardd.o ansivt2d.o strd.o fsd.o  searchfiled.o errord.o regex/regexd.o  -lole32 -loleaut32 -o fsd.exe

searchfiled.o: searchfile.c searchfile.h regexd.o

	gcc -Wall   -g -DDEBUG -c -o searchfiled.o searchfile.c 

fsd.o: fs.c fs.h
	gcc -Wall   -g -DDEBUG -c -o fsd.o fs.c 

strd.o: str.c str.h
	gcc -Wall   -g -DDEBUG -c -o strd.o str.c

barnyardd.o: barnyard.c barnyard.h
	gcc -Wall   -g -DDEBUG -c -o barnyardd.o barnyard.c
	
ansivt2d.o: ansivt2.c ansivt2.h
	gcc -Wall   -g -DDEBUG -c -o ansivt2d.o ansivt2.c
	
errord.o: error.c error.h
	gcc -Wall   -g -DDEBUG -c -o errord.o error.c
	
regexd.o:
	make -C regex libd
	
clean:
	rm -f *.o
	rm -f *.exe
	
	