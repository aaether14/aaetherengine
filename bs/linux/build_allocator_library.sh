#LIBRARY BUILDING MACRO


	mkdir -p build/lib
	rm -f build/lib/$1.so	
	gcc -shared -fpic -O3 $2 lib/$1.c -o build/lib/lib$1.so
