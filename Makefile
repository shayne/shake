CFLAGS = \
    -g -O1 -fgnu89-inline \
 	-fstack-protector -Wall -pedantic \
 	-Wstrict-prototypes -Wundef -fno-common \
 	-Werror-implicit-function-declaration \
 	-Wformat -Wformat-security -Werror=format-security \
 	-Wconversion -Wunused-variable -Wunreachable-code \
 	-Wall -W -D_FORTIFY_SOURCE=2 -std=c11

TARGET=shake

SHAKE_SRC=$(wildcard src/*.c)
SHAKE_OBJ=$(patsubst %.c,%.o,$(SHAKE_SRC))

LIBS_SRC=$(wildcard lib/*.c)
LIBS_OBJ=$(patsubst %.c,%.o,$(LIBS_SRC))
LIBS_STA=$(patsubst %.c,%.a,$(LIBS_SRC))

all: $(TARGET)

$(TARGET): CFLAGS += -I.
$(TARGET): $(LIBS_STA) $(SHAKE_OBJ)
	$(CC) -o $@ $(SHAKE_OBJ) $(LIBS_STA)

$(LIBS_STA): CFLAGS += -fPIC
$(LIBS_STA): $(LIBS_OBJ)
	ar rcs $@ $^

clean:
	rm -f $(TARGET) $(SHAKE_OBJ) *.plist gmon.out

clean-all: clean
	rm -f $(LIBS_OBJ) $(LIBS_STA)

scan:
	 scan-build -k -V make clean all

.PHONY: all libs clean clean-all scan
