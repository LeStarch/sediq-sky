testing:
	mkdir -p bin
	gcc -g -I./include src/gps.c src/testing.c src/serror.c -o bin/testing
clean:
	rm -rf bin/*
	rmdir bin
