/**
 * \file
 * \brief Shell and console interface implementation.
 * \author Erich Styger
 *
 * This module implements the front to the console/shell functionality.
 */


#include "Shell.h"
#include "BLUETOOTH.h"
#include "Application.h"
#include "FRTOS1.h"
#include "BT1.h"
#include "PID.h"

#include "MotionController.h"

#define PL_CONFIG_HAS_BLUETOOTH (1)

/* forward declaration */
static uint8_t SHELL_ParseCommand(const unsigned char *cmd, bool *handled, const BLUETOOTH_StdIOType *io);

static const BLUETOOTH_ParseCommandCallback CmdParserTable[] =
{
  BLUETOOTH_ParseCommand, /* Processor Expert Shell component, is first in list */
  SHELL_ParseCommand, /* our own module parser */
  MOT_ParseCommand,
  PID_ParseCommand,
  NULL /* Sentinel */
};

static uint32_t SHELL_val; /* used as demo value for shell */

void SHELL_SendString(unsigned char *msg) {
#if PL_CONFIG_HAS_SHELL_QUEUE
  SQUEUE_SendString(msg);
#elif CLS1_DEFAULT_SERIAL
  CLS1_SendStr(msg, CLS1_GetStdio()->stdOut);
#else
#endif
}

/*!
 * \brief Prints the help text to the console
 * \param io StdIO handler
 * \return ERR_OK or failure code
 */
static uint8_t SHELL_PrintHelp(const BLUETOOTH_StdIOType *io) {
	BLUETOOTH_SendHelpStr("Shell", "Shell commands\r\n", io->stdOut);
	BLUETOOTH_SendHelpStr("  help|status", "Print help or status information\r\n", io->stdOut);
	BLUETOOTH_SendHelpStr("  val <num>", "Assign number value\r\n", io->stdOut);
	return ERR_OK;
}

/*!
 * \brief Prints the status text to the console
 * \param io StdIO handler
 * \return ERR_OK or failure code
 */
static uint8_t SHELL_PrintStatus(const BLUETOOTH_StdIOType *io) {
  uint8_t buf[16];

  BLUETOOTH_SendStatusStr("Shell", "\r\n", io->stdOut);
  UTIL1_Num32sToStr(buf, sizeof(buf), SHELL_val);
  UTIL1_strcat(buf, sizeof(buf), "\r\n");
  BLUETOOTH_SendStatusStr("  val", buf, io->stdOut);
  return ERR_OK;
}

static uint8_t SHELL_ParseCommand(const unsigned char *cmd, bool *handled, const BLUETOOTH_StdIOType *io) {
  uint32_t val;
  const unsigned char *p;

  if (UTIL1_strcmp((char*)cmd, BLUETOOTH_CMD_HELP)==0 || UTIL1_strcmp((char*)cmd, "Shell help")==0) {
    *handled = TRUE;
    return SHELL_PrintHelp(io);
  } else if (UTIL1_strcmp((char*)cmd, BLUETOOTH_CMD_STATUS)==0 || UTIL1_strcmp((char*)cmd, "Shell status")==0) {
    *handled = TRUE;
    return SHELL_PrintStatus(io);
  } else if (UTIL1_strncmp(cmd, "Shell val ", sizeof("Shell val ")-1)==0) {
    p = cmd+sizeof("Shell val ")-1;
    if (UTIL1_xatoi(&p, &val)==ERR_OK) {
      SHELL_val = val;
      *handled = TRUE;
    } else {
      return ERR_FAILED; /* wrong format of command? */
    }
  }
  return ERR_OK;
}

#if PL_CONFIG_HAS_BLUETOOTH
/* Bluetooth stdio */
static BLUETOOTH_ConstStdIOType BT_stdio = {
  (BLUETOOTH_StdIO_In_FctType)BT1_StdIOReadChar, /* stdin */
  (BLUETOOTH_StdIO_OutErr_FctType)BT1_StdIOSendChar, /* stdout */
  (BLUETOOTH_StdIO_OutErr_FctType)BT1_StdIOSendChar, /* stderr */
  BT1_StdIOKeyPressed /* if input is not empty */
};
#endif

#if PL_CONFIG_HAS_SEGGER_RTT
static CLS1_ConstStdIOType RTT_stdio = {
  (CLS1_StdIO_In_FctType)RTT1_StdIOReadChar, /* stdin */
  (CLS1_StdIO_OutErr_FctType)RTT1_StdIOSendChar, /* stdout */
  (CLS1_StdIO_OutErr_FctType)RTT1_StdIOSendChar, /* stderr */
  RTT1_StdIOKeyPressed /* if input is not empty */
};
#endif

#define SHELL_COMBINED_STDIO  0

#if SHELL_COMBINED_STDIO /* copy */
static void CombineStdOut(uint8_t ch) {
#if CLS1_DEFAULT_SERIAL
  CLS1_GetStdio()->stdOut(ch);
#endif
#if PL_CONFIG_HAS_SEGGER_RTT
  RTT_stdio.stdOut(ch);
#endif
#if PL_CONFIG_HAS_BLUETOOTH
  BT_stdio.stdOut(ch);
#endif
}

static void CombineStdErr(uint8_t ch) {
#if CLS1_DEFAULT_SERIAL
  CLS1_GetStdio()->stdErr(ch);
#endif
#if PL_CONFIG_HAS_SEGGER_RTT
  RTT_stdio.stdErr(ch);
#endif
#if PL_CONFIG_HAS_BLUETOOTH
  BT_stdio.stdErr(ch);
#endif
}

static void CombineStdIn(uint8_t *ch) {
#if CLS1_DEFAULT_SERIAL
  CLS1_GetStdio()->stdIn(ch);
  if (*ch!='\0') {
    return;
  }
#endif
#if PL_CONFIG_HAS_SEGGER_RTT
  RTT_stdio.stdIn(ch);
  if (*ch!='\0') {
    return;
  }
#endif
#if PL_CONFIG_HAS_BLUETOOTH
  BT_stdio.stdIn(ch);
  if (*ch!='\0') {
    return;
  }
#endif
  *ch = '\0'; /* nothing received */
}

static bool CombineKeyPressed(void) {
#if CLS1_DEFAULT_SERIAL
  if (CLS1_GetStdio()->keyPressed()) {
    return TRUE;
  }
#endif
#if PL_CONFIG_HAS_SEGGER_RTT
  if (RTT_stdio.keyPressed()) {
    return TRUE;
  }
#endif
#if PL_CONFIG_HAS_BLUETOOTH
  if (BT_stdio.keyPressed()) {
    return TRUE;
  }
#endif
  return FALSE;
}

static CLS1_ConstStdIOType CombineStdio = {
  (CLS1_StdIO_In_FctType)CombineStdIn, /* stdin */
  (CLS1_StdIO_OutErr_FctType)CombineStdOut, /* stdout */
  (CLS1_StdIO_OutErr_FctType)CombineStdErr, /* stderr */
  CombineKeyPressed /* if input is not empty */
};
#endif

void SHELL_ParseCmd(unsigned char *cmd) {
#if SHELL_COMBINED_STDIO
  (void)CLS1_ParseWithCommandTable(cmd, &CombineStdio, CmdParserTable);
#else
  #if CLS1_DEFAULT_SERIAL
    (void)CLS1_ParseWithCommandTable(cmd, CLS1_GetStdio(), CmdParserTable);
  #endif
  #if PL_CONFIG_HAS_BLUETOOTH
    (void)BLUETOOTH_ParseWithCommandTable(cmd, &BT_stdio, CmdParserTable);
  #endif
  #if PL_CONFIG_HAS_SEGGER_RTT
    (void)CLS1_ParseWithCommandTable(cmd, &RTT_stdio, CmdParserTable);
  #endif
#endif
}

static void ShellTask(void *pvParameters) {

#define DEFAULT_BUF_SIZE 48
#if BLUETOOTH_DEFAULT_SERIAL
  static unsigned char localConsole_buf[DEFAULT_BUF_SIZE];
#endif
#if PL_CONFIG_HAS_BLUETOOTH
  static unsigned char bluetooth_buf[DEFAULT_BUF_SIZE];
#endif
#if BLUETOOTH_DEFAULT_SERIAL
  BLUETOOTH_ConstStdIOTypePtr ioLocal = BLUETOOTH_GetStdio();
#endif

  (void)pvParameters; /* not used */

#if PL_CONFIG_HAS_BLUETOOTH
  bluetooth_buf[0] = '\0';
#endif
#if BLUETOOTH_DEFAULT_SERIAL
  localConsole_buf[0] = '\0';
#endif
#if BLUETOOTH_DEFAULT_SERIAL
  (void)BLUETOOTH_ParseWithCommandTable((unsigned char*)BLUETOOTH_CMD_HELP, ioLocal, CmdParserTable);
#endif
  for(;;) {
#if BLUETOOTH_DEFAULT_SERIAL
    (void)BLUETOOTH_ReadAndParseWithCommandTable(localConsole_buf, sizeof(localConsole_buf), ioLocal, CmdParserTable);
#endif
#if PL_CONFIG_HAS_BLUETOOTH
    (void)BLUETOOTH_ReadAndParseWithCommandTable(bluetooth_buf, sizeof(bluetooth_buf), &BT_stdio, CmdParserTable);
#endif

#if PL_CONFIG_HAS_SHELL_QUEUE
#if PL_CONFIG_SQUEUE_SINGLE_CHAR
    {
        /*! \todo Handle shell queue */
      unsigned char ch;

      while((ch=SQUEUE_ReceiveChar()) && ch!='\0') {
    #if CLS1_DEFAULT_SERIAL
       ioLocal->stdOut(ch);
    #endif
    #if PL_CONFIG_HAS_BLUETOOTH
        BT_stdio.stdOut(ch); /* copy on Bluetooth */
    #endif
    #if PL_CONFIG_HAS_SEGGER_RTT
        CDC_stdio.stdOut(ch); /* copy on USB CDC */
    #endif
      }
    }
#else /* PL_CONFIG_SQUEUE_SINGLE_CHAR */
    {
      const unsigned char *msg;

      msg = SQUEUE_ReceiveMessage();
      if (msg!=NULL) {
        CLS1_SendStr(msg, CLS1_GetStdio()->stdOut);
        FRTOS1_vPortFree((void*)msg);
      }
    }
#endif /* PL_CONFIG_SQUEUE_SINGLE_CHAR */
#endif /* PL_CONFIG_HAS_SHELL_QUEUE */
    FRTOS1_vTaskDelay(10/portTICK_PERIOD_MS);
  } /* for */
}

void SHELL_Init(void) {
  SHELL_val = 0;

  (void)BLUETOOTH_SetStdio(&BT_stdio); /* use the Bluetooth stdio as default */


  if (FRTOS1_xTaskCreate(ShellTask, "Shell", configMINIMAL_STACK_SIZE+100, NULL, tskIDLE_PRIORITY+1, NULL) != pdPASS) {
    for(;;){} /* error */
  }

}

void SHELL_Deinit(void) {
}

