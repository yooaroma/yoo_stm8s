/**
	******************************************************************************
	* @file    : stm8s_eeprom_main.c
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

    메모리의 범위는 0x4000 to 0x483F 까지이다.
      
    How to program STM8S and STM8A Flash program memory and data EEPROM (PM0051)
    <https://www.st.com/resource/en/programming_manual/pm0051-how-to-program-stm8s-and-stm8a-flash-program-memory-and-data-eeprom-stmicroelectronics.pdf>

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

  STM8S105C6T6 칩이 제공한 플래시 영역 : 004000 에서 0043FF까지에 데이터를 쓰는 것에 대한 방법이다.
  여기서 제공하는 것은 4000을 베이스로 하여 옵셋 어드레스를 가지고서 데이터를 쓰거나 읽는다.
  옵셋의 최대는 각 칩이 제공하는 크기이다. 여기서는 3FF이며 1K byte이다.

  eeprom help 
  eeprom init
  eeprom write 100 aa
  eeprom read 100
  eeprom lock

  또한 옵션 바이트를 써 넣는 프로그램도 테스트 할 수 있으며 4800에서 487F 즉 128byte이다.
  위와 같은 방법으로 옵셋 어드레스를 주어서 저장하거나 읽을 수 있다.

  eeprom oinit
  eeprom owrite 100 aa
  eeprom oread 100
  eeprom oerase 100
  eeprom olock

*/

// #if defined(__MIB_DEBUG_EEPROM__) || defined(__MIB_DEBUG_TEST__)
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
static int DoEEPROM(CMD_MY * cptr, int argc, char **argv);
/* Private macro -------------------------------------------------------------*/
#define CMD_MY_EEPROM_ALL	\
{   "eeprom", DoEEPROM,		\
	"  eeprom {init} : EEPROM init\r\n"\
	"  eeprom {lock} \r\n"\
	"  eeprom {write} [offset] [data] : (0x4000) <= addr <= (0x4000 + 0x03FF(1K))\r\n"\
	"  eeprom {read} [offset] : offset 1K(0x3FF)\r\n"\
	"  eeprom {oinit} : EEPROM OPTION BYTE init(0x4800 to 0x483F : 64bytes) \r\n"\
	"  eeprom {olock} \r\n"\
	"  eeprom {owrite} [addr] [data] : addr(4800h to 487Fh) \r\n"\
	"  eeprom {oread} [addr] \r\n"\
	"  eeprom {oerase} [addr] \r\n"\
	"  eeprom {help}  eeprom help.\r\n",\
},
/* Private variables ---------------------------------------------------------*/
static CMD_MY cmdTbl_only[] = {
		CMD_MY_EEPROM_ALL
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
void cmd_eeprom(void)
{
	CMD_MY *cptr;
	uint32_t index;

	index = 0;
	for (cptr = cmdTbl;; cptr++) 
	{
		if(cptr->cmd==0)
		{
			ccprintf(1,("INFO:+cmd_eeprom...\r\n"));
			memcpy((void *)&(cptr->cmd),(void *)&(cmdTbl_only[0].cmd),sizeof(CMD_MY));
			break;
		}
		index++;
		if(index>MAX_COMMANDS)
		{ 
			ccprintf(1,("INFO:-cmd_eeprom...\r\n"));
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
  cmd_eeprom();
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
static int DoEEPROM(CMD_MY *cptr, int argc, char **argv)
{
	uint16_t vwAddr = 0;
	uint8_t vbData = 0;

	if (argc < 2)
	{
		MibWriteDebugString(cptr->usage);
		return FALSE;
	}	
  // argc=4, argv
  // 0       1       2      3
  // eeprom {write} [addr] [data]  
	if (!strcmp(argv[1], "write")) // _CMD_MY_WRITE_
	{
		if (argc < 4)
		{
			MibWriteDebugString(cptr->usage);
			return FALSE;
		}
		if (!HexToInt(argv[2], &vwAddr, 16))
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
    if(mmEepromWrite(vwAddr, vbData)==0)
		{
			// false..
			ccprintf(_DEBUG_CMD_A_,("mmEepromWrite ERROR!!!: 0x%04x[0x%02x]\r\n", vwAddr,vbData));
			return 0; // false...
		}
		else
		{
			ccprintf(_DEBUG_CMD_A_,("mmEepromWrite OK!!!: 0x%04x[0x%02x]\r\n", vwAddr,vbData));
		}
	}
  // argc=3, argv
  // 0       1       2      
  // eeprom {read} [addr]
	else if (!strcmp(argv[1], "read")) // _CMD_MY_READ_
	{
		if (argc < 3)
		{
			MibWriteDebugString(cptr->usage);
			return FALSE;
		}
		if (!HexToInt(argv[2], &vwAddr, 16))
		{
			ccprintf(_DEBUG_CMD_A_, ("Illugal character is used.\r\n"));
			return FALSE;
		}
    {      
			// vbData = rd_ADDR8(vbAddr);
      vbData = mmEepromRead(vwAddr);
    }   
		ccprintf(_DEBUG_CMD_A_,("mmEepromRead : 0x%04x[0x%02x]\r\n", vwAddr,vbData));
	}	  
  // argc, argv
  // 0       1           
  // eeprom {init}
	else if (!strcmp(argv[1], "init")) // _CMD_MY_READ_
	{
    if(mmEepromUnlock()==0)
		{
			// error...
			ccprintf(_DEBUG_CMD_A_, ("eeprom init ERROR!!!....mmEepromUnlock()\r\n"));		
		  return FALSE;
		}
		else
		{
			ccprintf(_DEBUG_CMD_A_, ("eeprom init OK!!!....mmEepromUnlock()\r\n"));		
		}
	}	  
  // argc, argv
  // 0       1           
  // eeprom {lock}
	else if (!strcmp(argv[1], "lock")) // _CMD_MY_READ_
	{
		ccprintf(_DEBUG_CMD_A_, ("eeprom lock....mmEepromLock()\r\n"));		
    mmEepromLock();
	}	  
  // argc, argv
  // 0       1           
  // eeprom {help}
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
		ccprintf(_DEBUG_CMD_A_, (" eeprom help\r\n eeprom init\r\n eeprom write 100 aa\r\n eeprom read 100\r\n eeprom lock\r\n"));	
		ccprintf(_DEBUG_CMD_A_, (" eeprom oinit\r\n eeprom oread 4803\r\n eeprom owrite 4803 80\r\n eeprom oerase 4803\r\n eeprom olock\r\n"));	
		ccprintf(_DEBUG_CMD_A_, (" https://www.st.com/resource/en/datasheet/stm8s105c6.pdf page 46, AFR7 (beep) / page 29 PD4 (BEEP output, AFR7 bit)\r\n"));		
    // https://www.st.com/resource/en/datasheet/stm8s105c6.pdf page 46, AFR7 (beep) / page 29 PD4 (BEEP output, AFR7 bit) 
	}	 
  // argc=4, argv
  // 0       1       2      3
  // eeprom {owrite} [addr] [data]  
	else if (!strcmp(argv[1], "owrite")) // _CMD_MY_WRITE_
	{
		if (argc < 4)
		{
			MibWriteDebugString(cptr->usage);
			return FALSE;
		}
		if (!HexToInt(argv[2], &vwAddr, 16))
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
    if(mmFlashOptionWrite(vwAddr, vbData)==0)
		{
			// false..
			ccprintf(_DEBUG_CMD_A_,("mmFlashOptionWrite ERROR!!!: 0x%04x[0x%02x]\r\n", vwAddr,vbData));
			return 0; // false...
		}
		else
		{
			ccprintf(_DEBUG_CMD_A_,("mmFlashOptionWrite OK!!!: 0x%04x[0x%02x]\r\n", vwAddr,vbData));
		}
	}
  // argc=3, argv
  // 0       1       2      
  // eeprom {oread} [addr]
	else if (!strcmp(argv[1], "oread")) // _CMD_MY_READ_
	{
		uint16_t vwData=0;
		if (argc < 3)
		{
			MibWriteDebugString(cptr->usage);
			return FALSE;
		}
		if (!HexToInt(argv[2], &vwAddr, 16))
		{
			ccprintf(_DEBUG_CMD_A_, ("Illugal character is used.\r\n"));
			return FALSE;
		}
    {      
			// vbData = rd_ADDR8(vbAddr);
      vwData = mmFlashOptionRead(vwAddr);
    }   
		ccprintf(_DEBUG_CMD_A_,("mmFlashOptionRead : 0x%04x[0x%04x]\r\n", vwAddr,vwData));
	}	  
  // argc=3, argv
  // 0       1       2      
  // eeprom {oerase} [addr]
	else if (!strcmp(argv[1], "oerase")) // _CMD_MY_READ_
	{
		uint16_t vwData=0;
		if (argc < 3)
		{
			MibWriteDebugString(cptr->usage);
			return FALSE;
		}
		if (!HexToInt(argv[2], &vwAddr, 16))
		{
			ccprintf(_DEBUG_CMD_A_, ("Illugal character is used.\r\n"));
			return FALSE;
		}
    {      
			// vbData = rd_ADDR8(vbAddr);
      if(mmFlashOptionErase(vwAddr)==0)
			{
				// false..
				ccprintf(_DEBUG_CMD_A_,("mmFlashOptionErase ERROR!!!: 0x%04x\r\n", vwAddr));
				return 0; // false...
			}
			else
			{
				ccprintf(_DEBUG_CMD_A_,("mmFlashOptionErase OK!!!: 0x%04x\r\n", vwAddr));
			}
      vwData = mmFlashOptionRead(vwAddr);
    }   
		ccprintf(_DEBUG_CMD_A_,("mmFlashOptionErase : 0x%04x[0x%04x]\r\n", vwAddr,vwData));
	}	  
  // argc, argv
  // 0       1           
  // eeprom {oinit}
	else if (!strcmp(argv[1], "oinit")) // _CMD_MY_READ_
	{
    if(mmFlashOptionUnlock()==0)
		{
			// error...
			ccprintf(_DEBUG_CMD_A_, ("flash Option bytes init ERROR!!!....mmFlashOptionUnlock()\r\n"));		
		  return FALSE;
		}
		else
		{
			ccprintf(_DEBUG_CMD_A_, ("flash Option bytes init OK!!!....mmFlashOptionUnlock()\r\n"));		
		}
	}	  
  // argc, argv
  // 0       1           
  // eeprom {olock}
	else if (!strcmp(argv[1], "olock")) // _CMD_MY_READ_
	{
		ccprintf(_DEBUG_CMD_A_, ("flash Option bytes lock....mmFlashOptionLock()\r\n"));		
    mmFlashOptionLock();
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
#endif // __MIB_DEBUG_EEPROM__
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
