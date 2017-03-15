
# * Makefile	
# * Description: Makefile for compiling the SIC Assembler Passes
# * Author: Arjun Rao (1MS14IS018)
# * Course: System Software Laboratory



.PHONY: clean

clean:
	rm -rf build/

pass1: pass1.c hashmap.c
	 mkdir -p build;	gcc -std=gnu99 -ggdb  pass1.c hashmap.c -o build/pass1

pass2: pass2.c hashmap.c
	 mkdir -p build;	gcc -std=gnu99 -ggdb pass2.c hashmap.c -o build/pass2