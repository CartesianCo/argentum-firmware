all: build 

build: ./src/*
	ino build -f "-I src/util/SdFat -O2"

upload: build
	ino upload

clean:
	ino clean

console: upload
	minicom
