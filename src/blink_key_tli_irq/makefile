# makefile 
# yooaroma : 20230104 
# https://www.st.com/en/evaluation-tools/stm8s-discovery.html
# https://www.st.com/en/microcontrollers-microprocessors/stm8s105c6.html
# https://www.st.com/resource/en/datasheet/stm8s105c6.pdf
# https://www.st.com/resource/en/reference_manual/rm0016-stm8s-series-and-stm8af-series-8bit-microcontrollers-stmicroelectronics.pdf
# https://github.com/yooaroma/yoo_stm8s

CC = sdcc
PRJ = stm8s_blink_key_tli_irq
FLASH_SIZE = 8192

#do not edit
TOP_DIR = ./../..
OBJ_DIR = ./obj
EXE_DIR = ./exe

SRC = $(PRJ)_main.c
#SRC += stm8s_gpio.c
#SRC += stm8s_tim1.c
#SRC += stm8s_tim2.c
#SRC += stm8s_tim4.c
#SRC += stm8s_clk.c
#SRC += tm1628.c
#SRC += vfd.c

VPATH = $(TOP_DIR)/lib/src
VPATH += $(TOP_DIR)/mlib

OBJS = $(patsubst %.c,$(OBJ_DIR)/%.rel,$(SRC))

all: $(OBJS)
	$(CC) -mstm8 $(OBJS) -o $(EXE_DIR)/$(PRJ).ihx
	packihx $(EXE_DIR)/$(PRJ).ihx > $(EXE_DIR)/$(PRJ).hex
	cp $(EXE_DIR)/$(PRJ).hex $(TOP_DIR)/bin
#	makebin -s $(FLASH_SIZE) $(PRJ).ihx $(PRJ).bin

$(OBJ_DIR)/%.rel: %.c
	$(CC) -c -mstm8 -DSTM8S105 $< -I. -I$(TOP_DIR)/lib/inc -I$(TOP_DIR)/mlib -o $@
#	$(CC) -c -mstm8 -DSTM8S105 -DUSE_STDPERIPH_DRIVER $< -I. -Ilib/inc -Iusr/inc -o $@
#	$(CC) -c -mstm8 -D_SDCC_ -DSTM8S105 -DUSE_STDPERIPH_DRIVER $< -I. -Ilib/inc -Iusr/inc -o $@

clean:
	rm -f *.map *.rel *.ihx *.o *.sym *.lk *.lst *.rst *.cdb *.bin *.asm *.hex $(OBJ_DIR)/*.*

cleanall:
	rm -f *.map *.rel *.ihx *.o *.sym *.lk *.lst *.rst *.cdb *.bin *.asm *.hex $(OBJ_DIR)/*.* $(EXE_DIR)/*.* 

flash:
	stm8flash.exe -c stlink -p stm8s105?6 -w $(EXE_DIR)/$(PRJ).ihx
#	stm8flash -c <stlink|stlinkv2|espstlink> -p <partname> [-s flash|eeprom|0x8000] [-r|-w|-v] <filename>