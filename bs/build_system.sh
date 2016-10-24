#BUILD SYSTEM by Aaether - linux

	#embed license into executable
	objcopy -I binary -O elf64-x86-64 -B i386:x86-64 license build/license.o
	printf "Building "$1"...\n"
	printf "Creating build directory...\n"
	mkdir -p build
	cd build
	printf "Running cmake...\n"
	cmake -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=$2  ../
	printf "Creating binaries...\n"
	make
	if [ "$3" == "--USE_VALGRIND" ]; then
	valgrind ./$1
	else
	time ./$1
	fi
