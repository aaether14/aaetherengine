#BUILD SYSTEM by Aaether - linux
#$1 is the name of the executable 
#$2 is the build type (Debug / Release)
#$3 is the cpu architecture (aae_32bit / aae_64bit)
#$4 is (Yes / No) to set MT use


	#default values for arguments
	arg1=$1
	arg2=${2:-Debug}
	arg3=${3:-aae_32bit}
	arg4=${4:-No}


	#embed license into executable
	if [ "$arg3" == "aae_32bit" ]; then
	objcopy -I binary -O elf32-i386 -B i386 license build/license.o
	#./lib/build_malloc.sh aae_allocator_lib -m32 -DAAE_32BIT_BUILD -DAAE_LINUX_PLATFORM -DAAE_USE_DEFAULT_NAMES
	elif [ "$arg3" == "aae_64bit" ]; then
	objcopy -I binary -O elf64-x86-64 -B i386:x86-64 license build/license.o
	#./lib/build_malloc.sh aae_malloc -m64 -DAAE_64BIT_BUILD -DAAE_LINUX_PLATFORM -DAAE_USE_DEFAULT_NAMES
	else
	printf "Error! - invalid architecture provided!"
	exit 1
	fi


	printf "Building "$arg1"...\n"
	printf "Creating build directory...\n"
	cd build
	printf "Running cmake...\n"
	cmake -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=$arg2 -DAAE_CPU_ARCHITECTURE=$arg3 -DAAE_MULTITHREADED=$arg4 ../
	printf "Creating binaries...\n"
	make
	if [ "$arg2" == "Release" ]; then
	printf "Stripping binary!"
	strip $arg1
	fi
