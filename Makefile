MCU = attiny13
F_CPU = 9600000ul

TARGET=main
OBJS=$(TARGET).o

OPTIMIZE = Os
CC = avr-gcc
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
OBJSIZE = avr-size

CFLAGS  = -mmcu=$(MCU)
CFLAGS += -$(OPTIMIZE)
CFLAGS += -g
CFLAGS += -std=c17
CFLAGS += -funsigned-char
CFLAGS += -funsigned-bitfields
CFLAGS += -fpack-struct
CFLAGS += -ffreestanding
CFLAGS += -fshort-enums
CFLAGS += -fverbose-asm
CFLAGS += -Wall
CFLAGS += -I/usr/avr/include
CFLAGS += -Wstrict-prototypes
CFLAGS += -DF_CPU=$(F_CPU)


all: $(OBJS)
	$(CC) $(CFLAGS) -Wl,-Map,$(TARGET).map -o $(TARGET).elf $(OBJS)
	$(OBJCOPY) -j .text -j .data -O ihex $(TARGET).elf $(TARGET).hex
	$(OBJSIZE) $(TARGET).elf

clean:
	rm -f *.o *.hex *.elf *.map *.lst
	rm -f $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.lst: %.elf
	$(OBJDUMP) -d $< > $@

program: all install
debug: all $(TARGET).lst
pprogram: clean all program
aall: clean all

install:
	# micronucleus $(TARGET).hex
	avrdude -p ${MCU} -c usbasp -U flash:w:$(TARGET).hex
	
fuse:
	avrdude -p ${MCU} -c usbasp -U lfuse:w:0x5a:m -U hfuse:w:0xf9:m 
