/*

mymedia co., ltd.
Ryu seung hyun

*/
/* Includes */
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "mm_hw.h"
#include "mm_time.h"
#include "mm_debug.h"

#ifdef __CMD_MAIN_DEBUG__

#define _DEBUG_CMD_A_	1

extern int GetCommand(char *command, int len, UINT8 timeout);
extern int GetArgs(char *s, char **args);
extern int DoPrintHelp(int argc, char **argv);


#define MAX_COMMANDS	5
#define MAX_ARGS		5
#define CMD_LINE_MAX 	2
#define CMD_CHAR_MAX 	64

struct _CMD_MY {
    char *cmd;			
    int(*run) (struct _CMD_MY * cptr, int argc, char **argv);	
    char *usage;		
};

typedef struct _CMD_MY CMD_MY;

extern CMD_MY cmdTbl[];

struct _CMD_ARRAY {
    unsigned char cur,next;
    char buf[CMD_LINE_MAX][CMD_CHAR_MAX];
};

typedef struct _CMD_ARRAY CMD_ARRAY;

extern CMD_ARRAY cmd_list;

// function prototype.
void cmd_main(void);
int sub_main(void);
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
#ifdef __CMD_DS1307_DEBUG__
extern void cmd_ds1307(void);
#endif
#ifdef __CMD_EEPROM_DEBUG__
extern void cmd_EEPROM(void);
#endif
#ifdef __CMD_PWM_DEBUG__
extern void cmd_PWM(void);
#endif
#ifdef __CMD_ADC128_DEBUG__
extern void cmd_ADC128(void);
#endif
#ifdef __CMD_SD_DEBUG__
extern void cmd_SD(void);
#endif
#ifdef __CMD_NAND_DEBUG__
extern void cmd_NAND(void);
#endif
#ifdef __CMD_HARDWARE_DEBUG__
extern void cmd_Hardware(void);
#endif
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
    
void cmd_main(void)
{
	char cmd[CMD_CHAR_MAX];
	char *cmd_str;
	int argc = 0;
	char *argv[MAX_ARGS];
	CMD_MY *cptr;
	// TEST_OP *optr;

	/* Copy kernel data to RAM & zero out BSS */
	
	yfprintf(_DEBUG_CMD_A_,"\r\n #########################\r\n");
	yfprintf(_DEBUG_CMD_A_," #########################\r\n");
	yfprintf(_DEBUG_CMD_A_," #########################\r\n");
	yfprintf(_DEBUG_CMD_A_,"\r\n mymedia apl start !!!\r\n");
	yfprintf(_DEBUG_CMD_A_,"file name :"__FILE__"\r\n");
	yfprintf(_DEBUG_CMD_A_,"date      :"__DATE__":"__TIME__"\r\n");
	
	cmd[0] = 0;
	cmd[1] = 0;
	yfprintf(_DEBUG_CMD_A_,"\r\n [help or ?] to get a list of commands\r\n\r");
    cmd_list.cur=0;
    cmd_list.next=0;
    memset((void *)(&cmd_list),0,sizeof(cmd_list));
    {
		#ifdef __CMD_DS1307_DEBUG__
		{
    		cmd_ds1307();
    	}
    	#endif
		#ifdef __CMD_EEPROM_DEBUG__
		{
    		cmd_EEPROM();
    	}
    	#endif
		#ifdef __CMD_PWM_DEBUG__
		{
    		cmd_PWM();
    	}
    	#endif
		#ifdef __CMD_ADC128_DEBUG__
		{
    		cmd_ADC128();
    	}
    	#endif
		#ifdef __CMD_SD_DEBUG__
		{
    		cmd_SD();
    	}
    	#endif
		#ifdef __CMD_NAND_DEBUG__
		{
    		cmd_NAND();
    	}
    	#endif
		#ifdef __CMD_HARDWARE_DEBUG__
		{
    		cmd_Hardware();
    	}
    	#endif
    }
	for (;;) {
		yprintf("My>");

		// wait an hour to get a command.
		GetCommand(cmd, CMD_CHAR_MAX-1, 60);
		if (!cmd || !cmd[0])
			continue;
		cmd_str = (char *) cmd;
		argc = GetArgs(cmd_str, argv);
		for (cptr = cmdTbl; cptr->cmd; cptr++) {
			if (!strcmp(argv[0], cptr->cmd)) {
				(cptr->run) (cptr, argc, argv);
				break;
			}
		}
		if (!strcmp(argv[0], "help") || !strcmp(argv[0], "?")) {
			DoPrintHelp(argc, argv);
		}
		if (!strcmp(argv[0], "q") || !strcmp(argv[0], "Q")) {
			yfprintf(_DEBUG_CMD_A_,"\r\nmonitor program end!!!\r\n");
			break;
		}
	}
}
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
#else
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
void cmd_main(void)
{
	return;
}
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
#endif // __CMD_MAIN_DEBUG__
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
