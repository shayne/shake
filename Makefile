CFLAGS = \
  -g -O1 -fgnu89-inline \
 	-fstack-protector -Wall -pedantic \
 	-Wstrict-prototypes -Wundef -fno-common \
 	-Werror-implicit-function-declaration \
 	-Wformat -Wformat-security -Werror=format-security \
 	-Wconversion -Wunused-variable -Wunreachable-code \
 	-Wall -W -D_GNU_SOURCE -D_FORTIFY_SOURCE=2 -std=c11

TARGET=shake

SHAKE_SRC=$(wildcard src/*.c)
SHAKE_SRC+=$(wildcard src/**/*.c)
SHAKE_OBJ=$(patsubst %.c,%.o,$(SHAKE_SRC))

VENDOR_SRC=$(wildcard vendor/*.c)
VENDOR_OBJ=$(patsubst %.c,%.o,$(VENDOR_SRC))
VENDOR_STA=$(patsubst %.c,%.a,$(VENDOR_SRC))

all: $(TARGET)

$(TARGET): CFLAGS += -Ivendor/ -Isrc/ -Isrc/shakefile -Isrc/runscripts
$(TARGET): $(VENDOR_STA) $(SHAKE_OBJ)
	$(CC) -o $@ $(SHAKE_OBJ) $(VENDOR_STA)

$(VENDOR_STA): CFLAGS += -fPIC
$(VENDOR_STA): $(VENDOR_OBJ)
	ar rcs $@ $^

clean:
	rm -f $(TARGET) $(SHAKE_OBJ) *.plist gmon.out

clean-all: clean all

clean-everything: clean
	rm -f $(VENDOR_OBJ) $(VENDOR_STA)

scan:
	 scan-build -k -V make clean all

.PHONY: all clean clean-all scan
