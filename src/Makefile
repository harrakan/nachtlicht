# Makefile to build light_ws2812 library examples
# This is not a very good example of a makefile - the dependencies do not work, therefore everything is rebuilt every time.

# Change these parameters for your device

F_CPU = 8000000
DEVICE = attiny85
AVRDUDEMCU=t85

# Tools:
CC = avr-gcc
AVRDUDE=/usr/local/bin/avrdude


#-----------------------------------------
IDIR =../include

CC=avr-gcc
CFLAGS =-I$(IDIR)
CFLAGS+= -g2 -I. -mmcu=$(DEVICE) -DF_CPU=$(F_CPU) 
CFLAGS+= -Os -ffunction-sections -fdata-sections -fpack-struct -fno-move-loop-invariants -fno-tree-scev-cprop -fno-inline-small-functions  
CFLAGS+= -Wall -Wno-pointer-to-int-cast -std=gnu99

ODIR=../obj
LDIR =../lib
BDIR =../bin

_DEPS = light_ws2812.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = light_ws2812.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

nachtlicht: $(DEPS)
	@echo Building $@
	@$(CC) $(CFLAGS) -o $(ODIR)/$@.o main.c  ../include/light_ws2812.c
	@avr-size $(ODIR)/$@.o
	@avr-objcopy -j .text  -j .data -O ihex $(ODIR)/$@.o $(BDIR)/$@.hex
	@avr-objdump -d -S $(ODIR)/$@.o > $(ODIR)/$@.lss
	

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o $(BDIR)/*.hex
	
program : nachtlicht
	sudo gpio -g mode 22 out
	sudo gpio -g write 22 0
	sudo $(AVRDUDE) -p $(AVRDUDEMCU) -P /dev/spidev0.0 -c linuxspi -b 20000 -U flash:w:$(BDIR)/nachtlicht.hex
	sudo gpio -g write 22 1




