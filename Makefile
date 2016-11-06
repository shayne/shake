CFLAGS = \
    -g -O1 -fgnu89-inline \
 	-fstack-protector -Wall -pedantic \
 	-Wstrict-prototypes -Wundef -fno-common \
 	-Werror-implicit-function-declaration \
 	-Wformat -Wformat-security -Werror=format-security \
 	-Wconversion -Wunused-variable -Wunreachable-code \
 	-Wall -W -D_FORTIFY_SOURCE=2 -std=c11

all: shake

shake: args.o scripts.o runner.o keyvaluevec.o

clean:
	rm -f shake *.o *.plist gmon.out

scan:
	 scan-build -k -V make clean all

.PHONY: all clean scan
