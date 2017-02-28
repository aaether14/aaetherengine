#LIBRARY BUILDING MACRO
#$1 is the name of the library
#$2 shold be the the gcc flag for cpu architecture (-m32 / -m64)


	printf "Building library $1...\n"
	mkdir -p build/lib
	rm -f build/lib/$1.so	
	online_cores_define="-DAAE_NUMBER_OF_CPUS=$(getconf _NPROCESSORS_ONLN)"
	gcc -shared -fpic -O3 $2 ${*:3} $online_cores_define -std=gnu99 lib/$1.c -o build/lib/lib$1.so
	strip build/lib/lib$1.so
