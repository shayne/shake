all: shake

shake: args.o scripts.o keyvaluevec.o

clean:
	rm -f shake *.o *.plist gmon.out

scan:
	 scan-build -k -V make clean all

.PHONY: all clean scan
