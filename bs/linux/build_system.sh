#BUILD SYSTEM by Aaether - linux
#$1 is the name of the executable 
#$2 is the build type (Debug / Release)
#$3 is the cpu architecture (aae_32bit / aae_64bit)
#$4 should be set to --USE_VALGRIND for obvious reasons


	#bembed license into executable
	if [ "${3:-aae_32bit}" == "aae_32bit" ]; then
	objcopy -I binary -O elf32-i386 -B i386 license build/license.o
	else
	objcopy -I binary -O elf64-x86-64 -B i386:x86-64 license build/license.o
	fi


	printf "Building "$1"...\n"
	printf "Creating build directory...\n"
	cd build
	printf "Running cmake...\n"
	cmake -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=${2:-Release} -DAAE_CPU_ARCHITECTURE=${3:-aae_32bit} -DAAE_MULTITHREADED=${4:-No} ../
	printf "Creating binaries...\n"
	make
	if [ "$5" == "--USE_VALGRIND" ]; then
	valgrind ./$1
	else
	time ./$1
	fi
