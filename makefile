all: build 

build: ./src/*
	ino build -f "-I src/util/SdFat"

upload: build
	ino upload

clean:
	ino clean

console: upload
	minicom
