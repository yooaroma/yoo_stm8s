/**
	******************************************************************************
	* @file    : stm8s_flash_main.c
	* @brief   : 
	* @author  : MYMEDIA Co., Ltd.
	* @version : V1.0.0
	* @date    : 2023 : 02 : 09
	******************************************************************************
  */
/*
  compile : make
  write   : ST Visual Programmer 
  exe     : ./exe/stm8s_flash.hex
  src : ./mib/stm8s_mib_it.c
        ./mib/stm8s_mib_debug.c
        ./mib/stm8s_mib_vsprintf.c
        ./mib/stm8s_mib_debug_tim.c
        ./mib/stm8s_mib_debug_cmd.c
        ./mib/stm8s_mib_debug_mem.c
        ./mib/stm8s_mib_debug_top.c
        
  info :  
    How to program STM8S and STM8A Flash program memory and data EEPROM (PM0051)
    STM8S Reference Manuals : <https://www.st.com/resource/en/reference_manual/rm0016-stm8s-series-and-stm8af-series-8bit-microcontrollers-stmicroelectronics.pdf>
    page 34, 참고한다.
    stm8s 가 지니고 있는 flash에 데이터를 써넣기 방법에 대한 예제이다.

    STM8S microcontrollers offer low density (8 Kbytes), medium density (from 16 to 32 Kbytes) 
    and high density (from 32 to 128 Kbytes) Flash program memory, plus data EEPROM.
    STM8A microcontrollers feature medium density (from 8 to 32 Kbytes) and high density
    (from 32 to 128 Kbytes) Flash memory, plus data EEPROM.

                    BLOCK      -> PAGE 
      8K  (low)     64 bytes      0 (1 page = 1 block) to n 
      32K (medium)  128 bytes     0 (1 page = 4 block) to n
      128K(high)    128 bytes     0 (1 page = 4 block) to n

    Data EEPROM and option bytes
        Unlock : Write 0xAE then 56h in FLASH_DUKR (0x00 5064)(1)(2)
        Lock   : Reset bit 3 (DUL) in FLASH_IAPSR (0x00 505F)
    Program memory
        Unlock : Write 0x56 then 0xAE in FLASH_PUKR (0x00 5062)(3)
        Lock   : Reset bit 1 (PUL) in FLASH_IAPSR (0x00 505F)

      1. The OPT and NOPT bits of FLASH_CR2 and FLASH_NCR2 registers must be set/cleared to enable access to the option bytes.
      2. If wrong keys have been entered, another key programming sequence can be issued without resetting the device.
      3. If wrong keys have been entered, the device must be reset, and a key program sequence issued.

  pdf 사양서 : <https://www.st.com/resource/en/datasheet/stm8s105c6.pdf> page 11

  STM8S105C6T6 microcontroller, 32 KB Flash, 2 KB RAM, 1 KB EEPROM

  FLASH_PROG_START_PHYSICAL_ADDRESS  [0x00008000]
  FLASH_PROG_END_PHYSICAL_ADDRESS    [0x0000ffff]
  FLASH_PROG_BLOCKS_NUMBER           [256]
  FLASH_DATA_START_PHYSICAL_ADDRESS  [0x00004000]
  FLASH_DATA_END_PHYSICAL_ADDRESS    [0x000043ff]
  FLASH_DATA_BLOCKS_NUMBER           [8]
  FLASH_BLOCK_SIZE                   [128]
  OPTION_BYTE_START_PHYSICAL_ADDRESS [0x00004800]
  OPTION_BYTE_END_PHYSICAL_ADDRESS   [0x0000487f]

  STM8S105C6T6 칩이 제공한 플래시 영역 : 008000 에서 00FFFF까지에 데이터를 쓰는 것에 대한 방법이다.
  블락으로 써 넣기 위해서는 램에서 동작하여 저장하여야 하나 
  1byte를 써 넣는 것은 가능한 것으로 보인다.

  flash help 
  flash init
  flash write f000 aa
  flash read f000
  flash unlock
*/

// #if defined(__MIB_DEBUG_FLASH__) || defined(__MIB_DEBUG_TEST__)
#if 1
/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "stm8s_mib.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static int DoFLASH(CMD_MY * cptr, int argc, char **argv);
/* Private macro -------------------------------------------------------------*/
#define CMD_MY_FLASH_ALL	\
{   "flash", DoFLASH,		\
	"  flash {init} \r\n"\
	"  flash {lock} \r\n"\
	"  flash {write} [addr] [data] : (0x8000) <= addr <= (0x8000 + size) \r\n"\
	"  flash {read} [addr] \r\n"\
	"  flash {help} flash help. : flash control reg display\r\n",\
},
/* Private variables ---------------------------------------------------------*/
static CMD_MY cmdTbl_only[] = {
		CMD_MY_FLASH_ALL
		CMD_MY_END
};
/* Public functions ----------------------------------------------------------*/
extern CMD_ARRAY cmd_list;
extern CMD_MY cmdTbl[];
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
/**
 * @brief
 * @par Parameters: None
 * @retval None
 */ 
void cmd_flash(void)
{
	CMD_MY *cptr;
	uint32_t index;

	index = 0;
	for (cptr = cmdTbl;; cptr++) 
	{
		if(cptr->cmd==0)
		{
			ccprintf(1,("INFO:+cmd_flash...\r\n"));
			memcpy((void *)&(cptr->cmd),(void *)&(cmdTbl_only[0].cmd),sizeof(CMD_MY));
			break;
		}
		index++;
		if(index>MAX_COMMANDS)
		{ 
			ccprintf(1,("INFO:-cmd_flash...\r\n"));
			return;
		}
	}
	return;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
#if defined(__MIB_DEBUG_TEST__)
void cmd_test(void)
{
  cmd_flash();
}
#endif
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
#if defined(__MIB_DEBUG_TEST__)
void cmd_test_init_before_irq_enable(void)
{
  // 인터럽트 인에이블 하기 전 초기화 시킬 것을 실행하는 곳..
}
#endif
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
/**
 * @brief
 * @par Parameters: None
 * @retval None
 */ 
#define _DEBUG_CMD_A_  1
static int DoFLASH(CMD_MY *cptr, int argc, char **argv)
{
	uint32_t vdwAddr = 0;
	uint8_t vbData = 0;

	if (argc < 2)
	{
		MibWriteDebugString(cptr->usage);
		return FALSE;
	}	
  // argc=4, argv
  // 0       1       2      3
  // flash {write} [addr] [data]  
	if (!strcmp(argv[1], "write")) // _CMD_MY_WRITE_
	{
		if (argc < 4)
		{
			MibWriteDebugString(cptr->usage);
			return FALSE;
		}
		if (!HexToInt(argv[2], &vdwAddr, 32))
		{
			ccprintf(_DEBUG_CMD_A_, ("Illugal character is used.\r\n"));
			return FALSE;
		}
		if (!HexToInt(argv[3], &vbData, 8))
		{
			ccprintf(_DEBUG_CMD_A_, ("Illugal character is used.\r\n"));
			return FALSE;
		}    
		// wr_ADDR8(vbAddr, vbData);
    if(mmFlashWrite(vdwAddr, vbData)==0)
		{
			// false..
			ccprintf(_DEBUG_CMD_A_,("mmFlashWrite ERROR!!!: 0x%08lx[0x%02x]\r\n", vdwAddr,vbData));
		}
		else
		{
			ccprintf(_DEBUG_CMD_A_,("mmFlashWrite OK!!!: 0x%08lx[0x%02x]\r\n", vdwAddr,vbData));
		}
	}
  // argc=3, argv
  // 0       1       2      
  // flash {read} [addr]
	else if (!strcmp(argv[1], "read")) // _CMD_MY_READ_
	{
		if (argc < 3)
		{
			MibWriteDebugString(cptr->usage);
			return FALSE;
		}
		if (!HexToInt(argv[2], &vdwAddr, 32))
		{
			ccprintf(_DEBUG_CMD_A_, ("Illugal character is used.\r\n"));
			return FALSE;
		}
    {      
			// vbData = rd_ADDR8(vbAddr);
      vbData = mmFlashRead(vdwAddr);
    }   
		ccprintf(_DEBUG_CMD_A_,("mmFlashRead : 0x%08lx[0x%02x]\r\n", vdwAddr,vbData));
	}	  
  // argc, argv
  // 0       1           
  // flash {init}
	else if (!strcmp(argv[1], "init")) // _CMD_MY_READ_
	{
    if(mmFlashUnlock()==0)
		{
			// error...
			ccprintf(_DEBUG_CMD_A_, ("flash init ERROR!!!....mmFlashUnlock()\r\n"));		
		}
		else
		{
			ccprintf(_DEBUG_CMD_A_, ("flash init OK!!!....mmFlashUnlock()\r\n"));		
		}
	}	  
  // argc, argv
  // 0       1           
  // flash {lock}
	else if (!strcmp(argv[1], "lock")) // _CMD_MY_READ_
	{
		ccprintf(_DEBUG_CMD_A_, ("flash lock....mmFlashLock()\r\n"));		
    mmFlashLock();
	}	  
  // argc, argv
  // 0       1           
  // flash {help}
	else if (!strcmp(argv[1], "help")) // _CMD_MY_READ_
	{
		ccprintf(_DEBUG_CMD_A_, ("FLASH_PROG_START_PHYSICAL_ADDRESS  [0x%08lx]\r\n",FLASH_PROG_START_PHYSICAL_ADDRESS  ));		
		ccprintf(_DEBUG_CMD_A_, ("FLASH_PROG_END_PHYSICAL_ADDRESS    [0x%08lx]\r\n",FLASH_PROG_END_PHYSICAL_ADDRESS    ));		
		ccprintf(_DEBUG_CMD_A_, ("FLASH_PROG_BLOCKS_NUMBER           [%d]\r\n" ,FLASH_PROG_BLOCKS_NUMBER             ));		
		ccprintf(_DEBUG_CMD_A_, ("FLASH_DATA_START_PHYSICAL_ADDRESS  [0x%08lx]\r\n",FLASH_DATA_START_PHYSICAL_ADDRESS));		
		ccprintf(_DEBUG_CMD_A_, ("FLASH_DATA_END_PHYSICAL_ADDRESS    [0x%08lx]\r\n",FLASH_DATA_END_PHYSICAL_ADDRESS  ));		
		ccprintf(_DEBUG_CMD_A_, ("FLASH_DATA_BLOCKS_NUMBER           [%d]\r\n" ,FLASH_DATA_BLOCKS_NUMBER      ));		
		ccprintf(_DEBUG_CMD_A_, ("FLASH_BLOCK_SIZE                   [%d]\r\n" ,(uint16_t)FLASH_BLOCK_SIZE       ));		
		ccprintf(_DEBUG_CMD_A_, ("OPTION_BYTE_START_PHYSICAL_ADDRESS [0x%08lx]\r\n",(uint32_t)OPTION_BYTE_START_PHYSICAL_ADDRESS));		
		ccprintf(_DEBUG_CMD_A_, ("OPTION_BYTE_END_PHYSICAL_ADDRESS   [0x%08lx]\r\n",(uint32_t)OPTION_BYTE_END_PHYSICAL_ADDRESS  ));		
		ccprintf(_DEBUG_CMD_A_, (" flash init \r\n flash write f000 aa \r\n flash read f000 \r\n flash unlock \r\n"));		    
		ccprintf(_DEBUG_CMD_A_, (" https://www.st.com/resource/en/reference_manual/rm0016-stm8s-series-and-stm8af-series-8bit-microcontrollers-stmicroelectronics.pdf  page 34\r\n"));		    
    // https://www.st.com/resource/en/reference_manual/rm0016-stm8s-series-and-stm8af-series-8bit-microcontrollers-stmicroelectronics.pdf  page 34
	}	 
	else
	{
		MibWriteDebugString(cptr->usage);
		return FALSE;
	}
	return TRUE;
}
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
#endif // __MIB_DEBUG_FLASH__
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
