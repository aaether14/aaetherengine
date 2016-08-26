#BUILD SYSTEM by Aaether



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
