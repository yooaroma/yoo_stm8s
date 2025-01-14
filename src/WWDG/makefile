#makefile 
#yooaroma : 20230104 

CC = sdcc
PRJ = stm8s_wwdg
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
#SRC += stm8s_clk.c
#SRC += stm8s_awu.c
#SRC += stm8s_iwdg.c
#SRC += stm8s_wwdg.c
SRC += stm8s_mib_wwdg.c
SRC += stm8s_mib_debug.c
SRC += stm8s_mib_it.c
SRC += stm8s_mib_vsprintf.c
SRC += stm8s_mib_debug_tim.c
SRC += stm8s_mib_debug_cmd.c
SRC += stm8s_mib_debug_mem.c
SRC += stm8s_mib_debug_top.c

VPATH = $(TOP_DIR)/lib/src
VPATH += $(TOP_DIR)/mib

OBJS = $(patsubst %.c,$(OBJ_DIR)/%.rel,$(SRC))
C_DEFINE = -DSTMS105 
C_DEFINE += -DUART_IRQ_RX_USE 
#C_DEFINE += -DUART_IRQ_TX_USE 
#C_DEFINE += -DUART_485_USE 
C_DEFINE += -D__MIB_DEBUG_TIM__
#C_DEFINE += -D__MIB_DEBUG_CMD__
#C_DEFINE += -D__MIB_DEBUG_MEM__
C_DEFINE += -D__MIB_DEBUG_TOP__
C_DEFINE += -D__MIB_DEBUG_TEST__
#C_DEFINE += -D__IRQ_AWU_CALLBACK_USE__


all: $(OBJS)
	$(CC) -mstm8 $(OBJS) -o $(EXE_DIR)/$(PRJ).ihx
	packihx $(EXE_DIR)/$(PRJ).ihx > $(EXE_DIR)/$(PRJ).hex
#	makebin -s $(FLASH_SIZE) $(PRJ).ihx $(PRJ).bin

$(OBJ_DIR)/%.rel: %.c
	$(CC) -c -mstm8 $(C_DEFINE) $< -I. -I$(TOP_DIR)/lib/inc -I$(TOP_DIR)/mib -o $@

clean:
	rm -f *.map *.rel *.ihx *.o *.sym *.lk *.lst *.rst *.cdb *.bin *.asm *.hex $(OBJ_DIR)/*.*

cleanall:
	rm -f *.map *.rel *.ihx *.o *.sym *.lk *.lst *.rst *.cdb *.bin *.asm *.hex $(OBJ_DIR)/*.* $(EXE_DIR)/*.* 
