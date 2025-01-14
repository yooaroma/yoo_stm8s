#makefile 
#yooaroma : 20230104 
# 컴퓨터\HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Enum\USB\VID_0483&PID_3744

CC = sdcc
PRJ = stm8s_blink
FLASH_SIZE = 8192
PRJ_MCU = STM8S105
PRJ_MCU_FLASH = stm8s105?6
PRJ_MCU_FLASHX = stm8s105x6

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
#	makebin -s $(FLASH_SIZE) $(PRJ).ihx $(PRJ).bin

$(OBJ_DIR)/%.rel: %.c
	$(CC) -c -mstm8 -D$(PRJ_MCU) $< -I. -I$(TOP_DIR)/lib/inc -I$(TOP_DIR)/mlib -o $@
#	$(CC) -c -mstm8 -D$(PRJ_MCU) -DUSE_STDPERIPH_DRIVER $< -I. -Ilib/inc -Iusr/inc -o $@
#	$(CC) -c -mstm8 -D_SDCC_ -D$(PRJ_MCU) -DUSE_STDPERIPH_DRIVER $< -I. -Ilib/inc -Iusr/inc -o $@

clean:
	rm -f *.map *.rel *.ihx *.o *.sym *.lk *.lst *.rst *.cdb *.bin *.asm *.hex $(OBJ_DIR)/*.*

cleanall:
	rm -f *.map *.rel *.ihx *.o *.sym *.lk *.lst *.rst *.cdb *.bin *.asm *.hex $(OBJ_DIR)/*.* $(EXE_DIR)/*.* 

flash:
	stm8flash.exe -c stlink -p $(PRJ_MCU_FLASH) -s flash -w $(EXE_DIR)/$(PRJ).ihx
#	stm8flash -c <stlink|stlinkv2|espstlink> -p <partname> [-s flash|eeprom|0x8000] [-r|-w|-v] <filename>

flasha:
	STVP_CmdLine -BoardName=ST-LINK -Port=USB -ProgMode=SWIM -Device=$(PRJ_MCU_FLASHX) -verbose -no_log -no_progress -no_loop -verif -FileProg=$(PRJ).ihx