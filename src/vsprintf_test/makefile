#makefile 
#yooaroma : 20230104 

CC = sdcc
PRJ = stm8s_vsprintf_test
FLASH_SIZE = 8192

#do not edit
TOP_DIR = ./../..
OBJ_DIR = ./obj
EXE_DIR = ./exe

SRC = $(PRJ)_main.c
#SRC += stm8s_gpio.c
#SRC += stm8s_tim1.c
#SRC += stm8s_tim2.c
#SRC += stm8s_tim3.c
#SRC += stm8s_tim4.c
SRC += stm8s_clk.c
SRC += stm8s_mib_it.c
SRC += stm8s_mib_debug.c
SRC += stm8s_mib_vsprintf.c

VPATH = $(TOP_DIR)/lib/src
VPATH += $(TOP_DIR)/mib

OBJS = $(patsubst %.c,$(OBJ_DIR)/%.rel,$(SRC))
C_DEFINE = -DSTMS105 
C_DEFINE += -DUART_IRQ_RX_USE 
#C_DEFINE += -DUART_IRQ_TX_USE 
#C_DEFINE += -DUART_485_USE 

all: $(OBJS)
	$(CC) -mstm8 $(OBJS) -o $(EXE_DIR)/$(PRJ).ihx
	packihx $(EXE_DIR)/$(PRJ).ihx > $(EXE_DIR)/$(PRJ).hex
#	makebin -s $(FLASH_SIZE) $(PRJ).ihx $(PRJ).bin

$(OBJ_DIR)/%.rel: %.c
	$(CC) -c -mstm8 $(C_DEFINE) $< -I. -I$(TOP_DIR)/lib/inc -I$(TOP_DIR)/mib -o $@
#	$(CC) -c -mstm8 -DSTM8S105 $< -I. -I$(TOP_DIR)/lib/inc -I$(TOP_DIR)/mib -o $@
#	$(CC) -c -mstm8 -DSTM8S105 -DUART_IRQ_USE $< -I. -I$(TOP_DIR)/lib/inc -I$(TOP_DIR)/mib -o $@
#	$(CC) -c -mstm8 -DSTM8S105 -DUART_IRQ_USE -DUCOM_485_USE $< -I. -I$(TOP_DIR)/lib/inc -I$(TOP_DIR)/mib -o $@
#	$(CC) -c -mstm8 -DSTM8S105 $< -I. -I$(TOP_DIR)/lib/inc -I$(TOP_DIR)/mib -o $@

clean:
	rm -f *.map *.rel *.ihx *.o *.sym *.lk *.lst *.rst *.cdb *.bin *.asm *.hex $(OBJ_DIR)/*.*

cleanall:
	rm -f *.map *.rel *.ihx *.o *.sym *.lk *.lst *.rst *.cdb *.bin *.asm *.hex $(OBJ_DIR)/*.* $(EXE_DIR)/*.* 

flash:
	stm8flash.exe -c stlink -p stm8s105?6 -w $(EXE_DIR)/$(PRJ).ihx
#	stm8flash -c <stlink|stlinkv2|espstlink> -p <partname> [-s flash|eeprom|0x8000] [-r|-w|-v] <filename>