#BUILD SYSTEM by Aaether - linux
#$1 is the name of the executable 
#$2 is the build type (Debug / Release)
#$3 is the cpu architecture (aae_32bit / aae_64bit)
#$4 should be set to --USE_VALGRIND for obvious reasons


	#embed license into executable
	if [ "${3:-aae_32bit}" == "aae_32bit" ]; then
	objcopy -I binary -O elf32-i386 -B i386 license build/license.o
	#./bs/linux/build_library.sh aae_allocator_lib -m32 -DAAE_32BIT_BUILD -DAAE_LINUX_PLATFORM -DAAE_USE_DEFAULT_NAMES
	else
	objcopy -I binary -O elf64-x86-64 -B i386:x86-64 license build/license.o
	#./bs/linux/build_library.sh aae_malloc -m64 -DAAE_64BIT_BUILD -DAAE_LINUX_PLATFORM -DAAE_USE_DEFAULT_NAMES
	fi


	printf "Building "$1"...\n"
	printf "Creating build directory...\n"
	cd build
	printf "Running cmake...\n"
	cmake -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=${2:-Release} -DAAE_CPU_ARCHITECTURE=${3:-aae_32bit} -DAAE_MULTITHREADED=${4:-No} ../
	printf "Creating binaries...\n"
	make
	strip $1
	time ./$1
