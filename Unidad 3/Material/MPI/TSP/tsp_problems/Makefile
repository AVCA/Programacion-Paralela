bbseq: bbseq.o libbb.o
	mpicxx bbseq.o libbb.o -o bbseq
	
bbseq.o: bbseq.cc
	mpicxx -c bbseq.cc

libbb.o: libbb.cc libbb.h
	mpicxx -c  libbb.cc 

clean:
	/bin/rm -f *.o bbseq