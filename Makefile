# 'make fs' to build 'fs.exe'

  
fs: fs.o str.o barnyard.o ansivt2.o error.o regex.o
	gcc -mconsole  barnyard.o str.o fs.o ansivt2.o error.o regex/regex.o  -lole32 -loleaut32  -o fs.exe

fs.o: fs.c fs.h barnyard.h ansivt2.h
	gcc -c -o fs.o fs.c 

str.o: str.c str.h barnyard.h ansivt2.h
	gcc -c -o str.o str.c

barnyard.o: barnyard.c barnyard.h
	gcc -c -o barnyard.o barnyard.c
	
ansivt2.o: ansivt2.c ansivt2.h
	gcc -c -o ansivt2.o ansivt2.c
	
error.o: error.c error.h
	gcc -c -o error.o error.c
	
regex.o:
	make -C regex lib

fsd: fsd.o strd.o barnyardd.o ansivt2d.o errord.o regexd.o
	gcc -mconsole -DDEBUG -g barnyardd.o strd.o fsd.o ansivt2d.o errord.o regex/regexd.o -lole32 -loleaut32 -o fsd.exe
	
fsd.o: fs.c fs.h barnyard.h ansivt2.h
	gcc -g -DDEBUG -c -o fsd.o fs.c 

strd.o: str.c str.h barnyard.h ansivt2.h
	gcc -g -DDEBUG -c -o strd.o str.c

barnyardd.o: barnyard.c barnyard.h
	gcc -g -DDEBUG -c -o barnyardd.o barnyard.c
	
ansivt2d.o: ansivt2.c ansivt2.h
	gcc -g -DDEBUG -c -o ansivt2d.o ansivt2.c
	
errord.o: error.c error.h
	gcc -g -DDEBUG -c -o errord.o error.c
	
regexd.o:
	make -C regex libd
	
clean:
	del *.o
	del *.exe
	
	