bin/gps-test:
	mkdir -p bin
	gcc -g -I./include -I./test/include src/* test/src/gps-test.c -o bin/gps-test
clean:
	rm -rf bin/*
	rmdir bin
