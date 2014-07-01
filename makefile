all: build 

build: ./src/*
	ino build

upload: build
	ino upload

clean:
	ino clean

console: upload
	minicom
