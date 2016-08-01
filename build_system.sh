#BUILD SYSTEM by Aaether



	printf "Building "$1"...\n"
	printf "Creating build directory...\n"
	mkdir -p build
	cd build
	printf "Running cmake...\n"
	cmake -G"Unix Makefiles" ../
	printf "Creating binaries...\n"
	make
	if [ "$2" == "--USE_VALGRIND" ]; then
	valgrind ./$1
	else
	./$1
	fi
