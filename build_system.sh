


	printf "Building "$1"...\n"
	printf "Creating build directory...\n"
	mkdir -p build	
	cd build
	printf "Running cmake...\n"
	cmake -G"Unix Makefiles" ../
	printf "Creating binaries...\n"
	make 
	./$1
