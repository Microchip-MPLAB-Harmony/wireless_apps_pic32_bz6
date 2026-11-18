/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It
    implements the logic of the application's state machine and it may call
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <string.h>
#include <stdio.h>
#include "app.h"
#include "definitions.h"
#include "peripheral/sercom/usart/plib_sercom0_usart.h"
#include "efuse_driver.h"
#include "app_version.h"


// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/

APP_DATA appData;
// A CR may be followed by a delayed LF from the terminal. Consume that LF once.
static bool s_consoleSkipLF = false;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
*/

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************


/* TODO:  Add any necessary local functions.
*/


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    appData.state = APP_STATE_INIT;

    s_consoleSkipLF = false;

    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
}


/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */
// Console Helper Functions
  static void Console_Print(const char *message)
  {
      size_t length = strlen(message);
      size_t written = 0U;
      // Ring-buffer writes can accept only part of a message. Keep the message
      // alive and enqueue every byte before returning to the next print call.
      while(written < length)
      {
          written += SERCOM0_USART_Write((uint8_t *)&message[written], length - written);
      }
      // TXC can still be set before the first queued byte reaches hardware.
      // Wait for the software queue to drain before checking the final stop bit.
      while(SERCOM0_USART_WriteCountGet() != 0U)
      {
          // UART interrupts drain the queue.
      }
      while(!SERCOM0_USART_TransmitComplete())
      {
          // Wait
      }
  }

  static char Console_ReadChar(void)
  {
      char c = 0;
      // Wait until at least 1 byte is available in the RX buffer
      while(SERCOM0_USART_ReadCountGet() == 0)
      {
          // Wait for input
      }
      SERCOM0_USART_Read((uint8_t *)&c, 1);
      // Note: Echo disabled - terminal handles local echo
      return c;
  }

  static void Console_PrintError(const char *operation, EFUSE_STATUS status)
  {
      Console_Print("FAILED! ");
      Console_Print(operation);
      Console_Print(": ");
      switch(status)
      {
          case EFUSE_STATUS_INVALID_PARAM:
              Console_Print("invalid parameter or unsupported eFuse transition.");
              break;
          case EFUSE_STATUS_ALREADY_PROGRAMMED:
              Console_Print("the requested field is already locked or programmed.");
              break;
          case EFUSE_STATUS_PROGRAM_TIMEOUT:
              Console_Print("programming timed out; read-back was not attempted. The OTP result is uncertain.");
              break;
          case EFUSE_STATUS_RELOAD_TIMEOUT:
              Console_Print("OTP reload timed out; the actual value and verification are unavailable.");
              break;
          case EFUSE_STATUS_VERIFY_ERROR:
              Console_Print("read-back does not match the expected value or configuration; verification failed.");
              break;
          case EFUSE_STATUS_CONTROLLER_BUSY:
              Console_Print("the eFuse controller is busy.");
              break;
          case EFUSE_STATUS_TIMEBASE_ERROR:
              Console_Print("the timeout clock is unavailable or stopped.");
              break;
          case EFUSE_STATUS_KEY_NOT_CONFIGURED:
              Console_Print("customer public key is not configured; replace the placeholder in boot_key_config.h and rebuild.");
              break;
          case EFUSE_STATUS_NO_KEY_DATA:
              Console_Print("no SBK eFuse key data was detected; program and verify the key before locking.");
              break;
          default:
              Console_Print("operation failed; no more specific reason was reported.");
              break;
      }
      Console_Print("\r\n");
      if(EFUSE_GetControllerStatus() != EFUSE_STATUS_SUCCESS)
      {
          Console_Print("Further eFuse operations are blocked in this session. The OTP outcome must be checked after device recovery.\r\n");
      }
  }

  static const char *Console_OperationName(char selection)
  {
      switch(selection)
      {
          case '1': return "Read All eFuse Values";
          case '2': return "Anti-Rollback Counter configuration";
          case '3': return "UUID configuration";
          case '4': return "Debug Lock configuration";
          case '5': return "Security Configuration read";
          case '6': return "Secure Boot Key configuration";
          case '7': return "SRK configuration";
          case '8': return "Patch Lock configuration";
          default: return "eFuse operation";
      }
  }

  static void Display_Startup(void)
  {
      Console_Print("\r\n========================================\r\n");
      Console_Print(" " APP_NAME "\r\n");
      Console_Print(" Target: " APP_TARGET "\r\n");
      Console_Print(" Version: " APP_VERSION "\r\n");
      Console_Print("========================================\r\n");
  }
  static void Display_Menu(void)
  {
      Console_Print("\r\n\r\nMAIN MENU:\r\n");
      Console_Print("  [1] Read All eFuse Values\r\n");
      Console_Print("  [2] Configure Anti-Rollback Counter\r\n");
      Console_Print("  [3] Configure UUID\r\n");
      Console_Print("  [4] Configure Debug Lock\r\n");
      Console_Print("  [5] Read Security Configuration\r\n");
      Console_Print("  [6] Configure Secure Boot Key\r\n");
      Console_Print("  [7] Configure Storage Root Key (SRK)\r\n");
      Console_Print("  [8] Configure Patch Lock\r\n");
      Console_Print("  [9] Help\r\n");
      Console_Print("  [0] Exit\r\n");
      Console_Print("\r\nEnter selection: ");
  }

  static void Display_Help(void)
  {
      Console_Print("\r\n========== HELP ==========\r\n");
      Console_Print("This tool configures eFuse parameters.\r\n\r\n");
      Console_Print("Terminal: local echo ON; firmware character echo OFF.\r\n");
      Console_Print("Press Enter after each entry (CR, LF, or CR+LF).\r\n");
      Console_Print("Backspace/DEL edits input; the terminal handles screen erasure.\r\n");
      Console_Print("UUID: four words, exactly 8 hexadecimal characters each.\r\n");
      Console_Print("Wait for each prompt before entering the next value.\r\n\r\n");
      Console_Print("WARNING: eFuse programming is IRREVERSIBLE!\r\n");
      Console_Print("Once programmed, values CANNOT be changed.\r\n\r\n");
      Console_Print("Options:\r\n");
      Console_Print("  1 - Read all current eFuse values\r\n");
      Console_Print("  2 - Set Anti-Rollback Counter (0-255)\r\n");
      Console_Print("  3 - Set 128-bit UUID\r\n");
      Console_Print("  4 - Set Debug Lock (0-3)\r\n");
      Console_Print("  5 - Read SECCFG register\r\n");
      Console_Print("  6 - Configure Secure Boot Key\r\n");
      Console_Print("  7 - Configure Storage Root Key (SRK)\r\n");
      Console_Print("  8 - Read Patch Lock, then optionally lock and verify\r\n");
      Console_Print("      Use 1 or 5 to read Patch Lock again at any time.\r\n");
      Console_Print("  9 - Help\r\n");
      Console_Print("==========================\r\n");
  }

  static bool Console_ReadLine(char *buffer, uint8_t maxLen)
  {
      uint8_t idx = 0;
      bool overflow = false;
      bool invalid = false;
      if(buffer == NULL || maxLen == 0U)
      {
          return false;
      }

      // Always consume through Enter, even after the buffer fills. Never accept
      // a truncated prefix or leave the rest of a long line for another prompt.
      while(true)
      {
          char c = Console_ReadChar();
          if(s_consoleSkipLF)
          {
              s_consoleSkipLF = false;
              if(c == '\n')
              {
                  continue;
              }
          }

          if(c == '\r' || c == '\n')
          {
              s_consoleSkipLF = (c == '\r');
              Console_Print("\r\n");
              break;
          }
          else if(c == '\b' || c == 127)  // Backspace or DEL
          {
              if(!overflow && idx > 0)
              {
                  idx--;
                  // Note: No erase - terminal handles local echo
              }
          }
          else if(c >= 32 && c < 127)  // Printable characters
          {
              if(!overflow && idx < (maxLen - 1U))
              {
                  buffer[idx++] = c;
              }
              else
              {
                  overflow = true;
              }
              // Note: No echo - terminal handles local echo
          }
          else
          {
              // Reject tabs, escape sequences, NUL, and other unsupported bytes.
              invalid = true;
          }
      }
      buffer[idx] = '\0';
      if(overflow || invalid)
      {
          buffer[0] = '\0';
          Console_Print(overflow ? "Input too long. Entry rejected.\r\n" :
                                   "Invalid input characters. Entry rejected.\r\n");
          return false;
      }
      return true;
  }

  static bool Console_ReadSelection(char *selection)
  {
      char buffer[4];
      if(!Console_ReadLine(buffer, sizeof(buffer)))
      {
          return false;
      }
      if(buffer[0] == '\0' || buffer[1] != '\0')
      {
          Console_Print("Invalid selection. Enter one character only.\r\n");
          return false;
      }
      *selection = buffer[0];
      return true;
  }

  static bool Console_ParseHex32(const char *text, uint32_t *value)
  {
      if(text == NULL || value == NULL || strlen(text) != 8U)
      {
          return false;
      }
      uint32_t parsed = 0U;
      for(uint8_t i = 0U; i < 8U; i++)
      {
          char c = text[i];
          uint32_t digit;
          if(c >= '0' && c <= '9') digit = (uint32_t)(c - '0');
          else if(c >= 'A' && c <= 'F') digit = (uint32_t)(c - 'A' + 10);
          else if(c >= 'a' && c <= 'f') digit = (uint32_t)(c - 'a' + 10);
          else return false;
          parsed = (parsed << 4) | digit;
      }
      *value = parsed;
      return true;
  }

  static bool Console_ReadUUIDWord(const char *prompt, uint32_t *value)
  {
      while(true)
      {
          char buffer[10];
          Console_Print(prompt);
          if(!Console_ReadLine(buffer, sizeof(buffer)))
          {
              continue;
          }
          if((buffer[0] == 'c' || buffer[0] == 'C') && buffer[1] == '\0')
          {
              return false;
          }
          if(Console_ParseHex32(buffer, value))
          {
              return true;
          }
          Console_Print("Invalid UUID word. Enter exactly 8 hexadecimal characters (0-9, A-F), or 'c' to cancel.\r\n");
      }
  }

  static bool Console_ReadUUIDInput(EFUSE_UUID_t *uuid)
  {
      EFUSE_UUID_t input;
      Console_Print("\r\nEnter UUID as four words. Type 'c' at any word to cancel.\r\n");
      if(!Console_ReadUUIDWord("Word0 (8 hex chars): ", &input.word0) ||
         !Console_ReadUUIDWord("Word1 (8 hex chars): ", &input.word1) ||
         !Console_ReadUUIDWord("Word2 (8 hex chars): ", &input.word2) ||
         !Console_ReadUUIDWord("Word3 (8 hex chars): ", &input.word3))
      {
          return false;
      }
      *uuid = input;
      return true;
  }

  static uint8_t Console_ReadUint8(bool *valid)
  {
      char buffer[8];
      *valid = false;
      if(!Console_ReadLine(buffer, sizeof(buffer)))
      {
          return 0U;
      }
      uint16_t val = 0;

      for(int i = 0; buffer[i] != '\0'; i++)
      {
          if(buffer[i] >= '0' && buffer[i] <= '9')
          {
              val = val * 10 + (buffer[i] - '0');
              if(val > 255)
              {
                  return 0;  // Overflow
              }
          }
          else
          {
              return 0;  // Invalid character
          }
      }

      if(buffer[0] != '\0')
      {
          *valid = true;
      }
      return (uint8_t)val;
  }

  static void Console_FlushInput(void)
  {
      // Discard any pending characters in RX buffer
      while(SERCOM0_USART_ReadCountGet() > 0)
      {
          char discard;
          SERCOM0_USART_Read((uint8_t *)&discard, 1);
          // Keep CR/LF state aligned even when queued input is discarded.
          s_consoleSkipLF = (discard == '\r');
      }
  }

  static bool Console_ConfirmAction(void)
  {
      char buffer[8];
      Console_FlushInput();  // Clear any leftover chars
      Console_Print("Type YES to confirm; any other input cancels: ");
      if(!Console_ReadLine(buffer, sizeof(buffer)))
      {
          return false;
      }

      return (buffer[0] == 'Y' && buffer[1] == 'E' && buffer[2] == 'S' && buffer[3] == '\0');
  }

  static void Console_PrintHex8(uint8_t val)
  {
      char buf[4];
      sprintf(buf, "%02X", val);
      Console_Print(buf);
  }

  static void Console_PrintHex16(uint16_t val)
  {
      char buf[6];
      sprintf(buf, "%04X", val);
      Console_Print(buf);
  }

  static void Console_PrintHex32(uint32_t val)
  {
      char buf[10];
      sprintf(buf, "%08lX", (unsigned long)val);
      Console_Print(buf);
  }

  static void Console_PrintLockField(const char *label, uint32_t raw, uint32_t mask, uint8_t position)
  {
      uint8_t field = (uint8_t)((raw & mask) >> position);
      Console_Print(label);
      Console_Print("0x");
      Console_PrintHex8(field);
      switch(field)
      {
          case 0U: Console_Print(" (Unlocked)\r\n"); break;
          case 1U:
          case 2U: Console_Print(" (Locked; one bit set)\r\n"); break;
          case 3U: Console_Print(" (Locked; both bits set)\r\n"); break;
      }
  }

  static void Console_PrintSecureBootKeyStatus(const EFUSE_SecureBootKey_t *bootKey, const EFUSE_SECCFG_t *seccfg)
  {
      bool present = EFUSE_IsSecureBootKeyValid(bootKey);
      bool configured = EFUSE_IsConfiguredSecureBootKeyReady();
      Console_Print(present ? "SBK eFuse: KEY DATA PRESENT\r\n" :
                              "SBK eFuse: NO KEY DATA DETECTED\r\n");
      Console_Print("Configured public key: ");
      Console_Print(configured ? EFUSE_ConfiguredSecureBootCurve() : "NOT CONFIGURED");
      Console_Print("\r\n");
      if(!configured)
      {
          Console_Print("Verification: UNAVAILABLE - configure the customer public key\r\n");
      }
      else if(!present)
      {
          Console_Print("Verification: NOT PROGRAMMED\r\n");
      }
      else if(EFUSE_SecureBootKeyMatchesConfigured(bootKey, seccfg))
      {
          Console_Print("Verification: MATCHES configured key\r\n");
      }
      else
      {
          Console_Print("Verification: DOES NOT MATCH configured key\r\n");
      }
      Console_PrintLockField("Boot Key Lock: ", seccfg->rawValue,
                             ROT_SECCFG_BOOT_KEY_LCK_Msk, ROT_SECCFG_BOOT_KEY_LCK_Pos);
  }

  static void EFUSE_DisplayAll(void)
  {
      uint8_t antiRollback;
      EFUSE_UUID_t uuid;
      EFUSE_SECCFG_t seccfg;

      Console_Print("\r\n--- eFuse Values ---\r\n\r\n");
      EFUSE_STATUS readStatus = EFUSE_ReadSECCFG(&seccfg);
      if(readStatus != EFUSE_STATUS_SUCCESS)
      {
          Console_PrintError("Security Configuration read", readStatus);
          return;
      }

      // Read Anti-Rollback Counter
      readStatus = EFUSE_ReadAntiRollback(&antiRollback);
      if(readStatus == EFUSE_STATUS_SUCCESS)
      {
          Console_Print("Anti-Rollback Counter: 0x");
          Console_PrintHex8(antiRollback);
          Console_Print("\r\n");
      }
      else
      {
          Console_PrintError("Anti-Rollback Counter read", readStatus);
      }

      // Read UUID
      readStatus = EFUSE_ReadUUID(&uuid);
      if(readStatus == EFUSE_STATUS_SUCCESS)
      {
          Console_Print("UUID: ");
          Console_PrintHex32(uuid.word3);
          Console_Print("-");
          Console_PrintHex32(uuid.word2);
          Console_Print("-");
          Console_PrintHex32(uuid.word1);
          Console_Print("-");
          Console_PrintHex32(uuid.word0);
          Console_Print("\r\n");
      }
      else
      {
          Console_PrintError("UUID read", readStatus);
      }

      // Display the successfully reloaded SECCFG.
      {
          Console_Print("SECCFG: 0x");
          Console_PrintHex32(seccfg.rawValue);
          Console_Print("\r\n");
          Console_Print("  Debug Lock: ");
          Console_PrintHex8(seccfg.debugLock);
          Console_Print("\r\n");
          Console_PrintLockField("  UUID Lock: ", seccfg.rawValue,
                                 ROT_SECCFG_UUID_LCK_Msk, ROT_SECCFG_UUID_LCK_Pos);
          Console_PrintLockField("  Root Key Lock: ", seccfg.rawValue,
                                 ROT_SECCFG_ROOT_KEY_LCK_Msk, ROT_SECCFG_ROOT_KEY_LCK_Pos);
          Console_PrintLockField("  Boot Key Lock: ", seccfg.rawValue,
                                 ROT_SECCFG_BOOT_KEY_LCK_Msk, ROT_SECCFG_BOOT_KEY_LCK_Pos);
          Console_PrintLockField("  Patch Lock: ", seccfg.rawValue,
                                 EFUSE_SECCFG_PATCH_LOCK_Msk, EFUSE_SECCFG_PATCH_LOCK_Pos);
      }

      // Read Secure Boot Key status
      EFUSE_SecureBootKey_t bootKey;
      readStatus = EFUSE_ReadSecureBootKey(&bootKey);
      if(readStatus == EFUSE_STATUS_SUCCESS)
      {
          Console_PrintSecureBootKeyStatus(&bootKey, &seccfg);

      }
      else
      {
          Console_PrintError("Secure Boot Key read", readStatus);
      }

      Console_Print("\r\n--------------------\r\n");
  }

  static void EFUSE_DisplaySECCFG(void)
  {
      EFUSE_SECCFG_t seccfg;

      EFUSE_STATUS readStatus = EFUSE_ReadSECCFG(&seccfg);
      if(readStatus == EFUSE_STATUS_SUCCESS)
      {
          Console_Print("\r\nSECCFG Register: 0x");
          Console_PrintHex32(seccfg.rawValue);
          Console_Print("\r\n\r\n");

          Console_Print("Bit Fields:\r\n");
          Console_Print("  [7:6] DEBUG_LCK: ");
          Console_PrintHex8(seccfg.debugLock);
          Console_Print(" (");
          switch(seccfg.debugLock)
          {
              case 0: Console_Print("Unlocked"); break;
              case 1: Console_Print("Level 1"); break;
              case 2: Console_Print("Level 2"); break;
              case 3: Console_Print("Level 3 - Full Lock"); break;
          }
          Console_Print(")\r\n");

          Console_PrintLockField("  [5:4] UUID_LCK: ", seccfg.rawValue,
                                 ROT_SECCFG_UUID_LCK_Msk, ROT_SECCFG_UUID_LCK_Pos);
          Console_PrintLockField("  [9:8] ROOT_KEY_LCK: ", seccfg.rawValue,
                                 ROT_SECCFG_ROOT_KEY_LCK_Msk, ROT_SECCFG_ROOT_KEY_LCK_Pos);
          Console_PrintLockField("  [11:10] BOOT_KEY_LCK: ", seccfg.rawValue,
                                 ROT_SECCFG_BOOT_KEY_LCK_Msk, ROT_SECCFG_BOOT_KEY_LCK_Pos);
          Console_PrintLockField("  [15:14] PATCH_LCK: ", seccfg.rawValue,
                                 EFUSE_SECCFG_PATCH_LOCK_Msk, EFUSE_SECCFG_PATCH_LOCK_Pos);

          Console_Print("  [16] ADD_BOOT_KEY: ");
          Console_Print(seccfg.addBootKey ? "1 (Set)" : "0 (Clear)");
          Console_Print("\r\n");
      }
      else
      {
          Console_PrintError("Security Configuration read", readStatus);
      }
  }

  static void EFUSE_DisplaySecureBootKey(void)
  {
      EFUSE_SecureBootKey_t bootKey;
      EFUSE_SECCFG_t seccfg;

      Console_Print("\r\n--- Secure Boot Key ---\r\n\r\n");

      EFUSE_STATUS readStatus = EFUSE_ReadSECCFG(&seccfg);
      if(readStatus != EFUSE_STATUS_SUCCESS)
      {
          Console_PrintError("Secure Boot Key configuration read", readStatus);
          return;
      }

      readStatus = EFUSE_ReadSecureBootKey(&bootKey);
      if(readStatus == EFUSE_STATUS_SUCCESS)
      {
          Console_PrintSecureBootKeyStatus(&bootKey, &seccfg);

      }
      else
      {
          Console_PrintError("Secure Boot Key read", readStatus);
      }
      Console_Print("\r\n---------------------------------\r\n");
  }

void APP_Tasks ( void )
{

    /* Check the application's current state. */
    switch ( appData.state )
    {
        /* Application's initial state. */
        case APP_STATE_INIT:
        {
            bool appInitialized = true;

            Display_Startup();
            if (appInitialized)
            {

                appData.state = APP_STATE_SERVICE_TASKS;
            }
            break;
        }

        case APP_STATE_SERVICE_TASKS:
        {
            Console_FlushInput();  // Clear any leftover chars before menu input
            Display_Menu();
            char choice;
            if(!Console_ReadSelection(&choice))
            {
                break;
            }

            // Keep Help and Exit usable after a controller fault.
            if(choice >= '1' && choice <= '8' &&
               EFUSE_GetControllerStatus() != EFUSE_STATUS_SUCCESS)
            {
                Console_PrintError(Console_OperationName(choice), EFUSE_GetControllerStatus());
                break;
            }

            switch(choice)
            {
                case '1':
                    Console_Print("\r\n--- Reading All eFuse Values ---\r\n");
                    EFUSE_DisplayAll();
                    break;

                case '2':
                    Console_Print("\r\n--- Anti-Rollback Counter ---\r\n");
                    {
                        uint8_t currentVal;
                        EFUSE_STATUS readStatus = EFUSE_ReadAntiRollback(&currentVal);
                        if(readStatus == EFUSE_STATUS_SUCCESS)
                        {
                            Console_Print("Current value: ");
                            char buf[8];
                            sprintf(buf, "%d", currentVal);
                            Console_Print(buf);
                            Console_Print(" (0x");
                            Console_PrintHex8(currentVal);
                            Console_Print(")\r\n\r\n");

                            Console_Print("Enter value (0-255) or 'c'=cancel: ");

                            // Read the input line
                            char inputBuf[8];
                            if(!Console_ReadLine(inputBuf, sizeof(inputBuf)))
                            {
                                break;
                            }

                            if((inputBuf[0] == 'c' || inputBuf[0] == 'C') && inputBuf[1] == '\0')
                            {
                                Console_Print("Cancelled.\r\n");
                            }
                            else if(inputBuf[0] >= '0' && inputBuf[0] <= '9')
                            {
                                // Parse the number from input buffer
                                bool valid = true;
                                uint16_t val = 0;
                                for(int i = 0; inputBuf[i] != '\0'; i++)
                                {
                                    if(inputBuf[i] >= '0' && inputBuf[i] <= '9')
                                    {
                                        val = val * 10 + (inputBuf[i] - '0');
                                        if(val > 255) { valid = false; break; }
                                    }
                                    else { valid = false; break; }
                                }
                                uint8_t newVal = (uint8_t)val;

                                if(!valid || inputBuf[0] == '\0')
                                {
                                    Console_Print("Invalid input!\r\n");
                                }
                                else if(newVal < currentVal)
                                {
                                    Console_Print("ERROR: New value must be >= current value!\r\n");
                                    Console_Print("(Anti-rollback can only increase)\r\n");
                                }
                                else if(newVal == currentVal)
                                {
                                    Console_Print("Value unchanged.\r\n");
                                }
                                else
                                {
                                    Console_Print("\r\n*** WARNING: eFuse programming is IRREVERSIBLE! ***\r\n");
                                    Console_Print("You are about to set Anti-Rollback Counter to: ");
                                    sprintf(buf, "%d", newVal);
                                    Console_Print(buf);
                                    Console_Print("\r\n\r\n");

                                    if(Console_ConfirmAction())
                                    {
                                        Console_Print("Programming...\r\n");
                                        EFUSE_STATUS status = EFUSE_ProgramAntiRollback(newVal);
                                        if(status == EFUSE_STATUS_SUCCESS)
                                        {
                                            // Read back to verify
                                            uint8_t verifyVal;
                                            EFUSE_STATUS verifyStatus = EFUSE_ReadAntiRollback(&verifyVal);
                                            if(verifyStatus == EFUSE_STATUS_SUCCESS && verifyVal == newVal)
                                            {
                                                Console_Print("SUCCESS! Anti-Rollback Counter programmed.\r\n");
                                                Console_Print("Verified value: ");
                                                sprintf(buf, "%d", verifyVal);
                                                Console_Print(buf);
                                                Console_Print("\r\n");
                                            }
                                            else
                                            {
                                                Console_PrintError("Anti-Rollback Counter read-back",
                                                                   verifyStatus == EFUSE_STATUS_SUCCESS ?
                                                                   EFUSE_STATUS_VERIFY_ERROR : verifyStatus);
                                            }
                                        }
                                        else
                                        {
                                            Console_PrintError("Anti-Rollback Counter programming", status);
                                            // Additional read-back is allowed only while the controller is usable.
                                            uint8_t debugVal;
                                            if(EFUSE_GetControllerStatus() == EFUSE_STATUS_SUCCESS)
                                            {
                                                EFUSE_STATUS debugReadStatus = EFUSE_ReadAntiRollback(&debugVal);
                                                if(debugReadStatus == EFUSE_STATUS_SUCCESS)
                                                {
                                                    Console_Print("Read-back value: ");
                                                    sprintf(buf, "%d", debugVal);
                                                    Console_Print(buf);
                                                    Console_Print(" (expected ");
                                                    sprintf(buf, "%d", newVal);
                                                    Console_Print(buf);
                                                    Console_Print(")\r\n");
                                                }
                                                else
                                                {
                                                    Console_PrintError("Anti-Rollback Counter read-back", debugReadStatus);
                                                }
                                            }
                                        }
                                    }
                                    else
                                    {
                                        Console_Print("Aborted.\r\n");
                                    }
                                }
                            }
                            else
                            {
                                Console_Print("Invalid input.\r\n");
                            }
                        }
                        else
                        {
                            Console_PrintError("Anti-Rollback Counter read", readStatus);
                        }
                    }
                    break;

                case '3':
                    Console_Print("\r\n--- UUID Configuration ---\r\n");
                    {
                        EFUSE_UUID_t uuid;
                        EFUSE_SECCFG_t seccfg;
                        EFUSE_STATUS readStatus = EFUSE_ReadSECCFG(&seccfg);
                        if(readStatus != EFUSE_STATUS_SUCCESS)
                        {
                            Console_PrintError("UUID configuration read", readStatus);
                            break;
                        }

                        readStatus = EFUSE_ReadUUID(&uuid);
                        if(readStatus == EFUSE_STATUS_SUCCESS)
                        {
                            Console_Print("Current UUID: ");
                            Console_PrintHex32(uuid.word3);
                            Console_Print("-");
                            Console_PrintHex32(uuid.word2);
                            Console_Print("-");
                            Console_PrintHex32(uuid.word1);
                            Console_Print("-");
                            Console_PrintHex32(uuid.word0);
                            Console_Print("\r\n");
                            Console_PrintLockField("UUID Lock Status: ", seccfg.rawValue,
                                                   ROT_SECCFG_UUID_LCK_Msk, ROT_SECCFG_UUID_LCK_Pos);
                        }
                        else
                        {
                            Console_PrintError("UUID read", readStatus);
                            break;
                        }

                        if((seccfg.rawValue & ROT_SECCFG_UUID_LCK_Msk) == ROT_SECCFG_UUID_LCK_Msk)
                        {
                            Console_Print("\r\nUUID is LOCKED with both lock bits set - cannot modify.\r\n");
                        }
                        else
                        {
                            if(seccfg.uuidLocked)
                            {
                                Console_Print("UUID is already locked; option 2 completes the remaining lock bit.\r\n");
                            }
                            Console_Print("\r\nOptions:\r\n");
                            if(!seccfg.uuidLocked)
                            {
                                Console_Print("  [1] Program UUID (write only, no lock)\r\n");
                            }
                            Console_Print("  [2] Lock UUID (set both lock bits)\r\n");
                            Console_Print("  [c] Cancel\r\n");
                            Console_Print("Select: ");

                            char uuidChoice;
                            if(!Console_ReadSelection(&uuidChoice))
                            {
                                break;
                            }

                            if(uuidChoice == '1')
                            {
                                if(seccfg.uuidLocked)
                                {
                                    Console_Print("UUID is LOCKED - cannot modify.\r\n");
                                    break;
                                }
                                EFUSE_UUID_t newUuid;
                                if(!Console_ReadUUIDInput(&newUuid))
                                {
                                    Console_Print("Cancelled. No UUID changes made.\r\n");
                                    break;
                                }

                                Console_Print("\r\nYou entered: ");
                                Console_PrintHex32(newUuid.word3);
                                Console_Print("-");
                                Console_PrintHex32(newUuid.word2);
                                Console_Print("-");
                                Console_PrintHex32(newUuid.word1);
                                Console_Print("-");
                                Console_PrintHex32(newUuid.word0);
                                Console_Print("\r\n\r\n");

                                Console_Print("*** WARNING: UUID programming is IRREVERSIBLE! ***\r\n");
                                Console_Print("*** UUID will NOT be locked - you can verify then lock separately ***\r\n\r\n");

                                if(Console_ConfirmAction())
                                {
                                    Console_Print("Programming UUID...\r\n");
                                    EFUSE_STATUS status = EFUSE_ProgramUUID(&newUuid);
                                    if(status == EFUSE_STATUS_SUCCESS)
                                    {
                                        Console_Print("SUCCESS! UUID programmed.\r\n");
                                        Console_Print("NOTE: UUID is NOT locked yet.\r\n");
                                        Console_Print("Verify with Option 1, then use Option 3 -> [2] to lock.\r\n");
                                    }
                                    else
                                    {
                                        Console_PrintError("UUID programming", status);
                                    }
                                }
                                else
                                {
                                    Console_Print("Aborted.\r\n");
                                }
                            }
                            else if(uuidChoice == '2')
                            {
                                Console_Print("\r\n*** WARNING: UUID Lock is IRREVERSIBLE! ***\r\n");
                                Console_Print("Current UUID value will be permanently locked.\r\n\r\n");

                                if(Console_ConfirmAction())
                                {
                                    Console_Print("Locking UUID...\r\n");
                                    EFUSE_STATUS status = EFUSE_LockUUID();
                                    if(status == EFUSE_STATUS_SUCCESS)
                                    {
                                        Console_Print("SUCCESS! UUID is locked with both bits set (0x03). Read-back verified.\r\n");
                                    }
                                    else if(status == EFUSE_STATUS_ALREADY_PROGRAMMED)
                                    {
                                        Console_Print("UUID is already locked with both bits set (0x03).\r\n");
                                    }
                                    else
                                    {
                                        Console_PrintError("UUID locking", status);
                                    }
                                }
                                else
                                {
                                    Console_Print("Aborted.\r\n");
                                }
                            }
                            else if(uuidChoice == 'c' || uuidChoice == 'C')
                            {
                                Console_Print("Cancelled.\r\n");
                            }
                            else
                            {
                                Console_Print("Invalid selection.\r\n");
                            }
                        }
                    }
                    break;

                case '4':
                    Console_Print("\r\n--- Debug Lock Configuration ---\r\n");
                    {
                        EFUSE_SECCFG_t seccfg;
                        EFUSE_STATUS readStatus = EFUSE_ReadSECCFG(&seccfg);
                        if(readStatus == EFUSE_STATUS_SUCCESS)
                        {
                            Console_Print("Current Debug Lock: Level ");
                            char buf[8];
                            sprintf(buf, "%d", seccfg.debugLock);
                            Console_Print(buf);
                            Console_Print(" (");
                            switch(seccfg.debugLock)
                            {
                                case 0: Console_Print("Unlocked"); break;
                                case 1: Console_Print("Level 1"); break;
                                case 2: Console_Print("Level 2"); break;
                                case 3: Console_Print("Level 3 - FULL LOCK"); break;
                            }
                            Console_Print(")\r\n");

                            if(seccfg.debugLock == 3)
                            {
                                Console_Print("\r\nDebug is FULLY LOCKED - cannot modify.\r\n");
                            }
                            else
                            {
                                Console_Print("\r\nAvailable new Debug Lock levels:\r\n");
                                if(seccfg.debugLock == 0U)
                                {
                                    Console_Print("  1 = Level 1 lock (01)\r\n");
                                    Console_Print("  2 = Level 2 lock (10)\r\n");
                                }
                                Console_Print("  3 = FULL LOCK (no debug/programming!)\r\n");
                                Console_Print("\r\n*** WARNING: Programmed eFuse bits cannot be cleared. ***\r\n");
                                Console_Print("*** Level 3 blocks ALL debug access FOREVER! ***\r\n");
                                Console_Print("Selecting the current level leaves it unchanged.\r\n");
                                Console_Print(seccfg.debugLock == 0U ?
                                    "\r\nEnter 1, 2, 3, or 'c' to cancel: " :
                                    "\r\nEnter 3, or 'c' to cancel: ");

                                char inputBuf[4];
                                if(!Console_ReadLine(inputBuf, sizeof(inputBuf)))
                                {
                                    break;
                                }

                                if((inputBuf[0] == 'c' || inputBuf[0] == 'C') && inputBuf[1] == '\0')
                                {
                                    Console_Print("Cancelled.\r\n");
                                }
                                else if(inputBuf[0] >= '0' && inputBuf[0] <= '3' && inputBuf[1] == '\0')
                                {
                                    uint8_t newLevel = inputBuf[0] - '0';

                                    if(((uint32_t)seccfg.debugLock & ~(uint32_t)newLevel) != 0U)
                                    {
                                        Console_Print("ERROR: This transition requires clearing a programmed eFuse bit. No changes made.\r\n");
                                        if(seccfg.debugLock == 1U && newLevel == 2U)
                                        {
                                            Console_Print("Level 1 (01) cannot change to Level 2 (10). Only Level 3 (11) is available.\r\n");
                                        }
                                    }
                                    else if(newLevel == seccfg.debugLock)
                                    {
                                        Console_Print("Level unchanged.\r\n");
                                    }
                                    else
                                    {
                                        Console_Print("\r\n*** WARNING: Debug Lock is IRREVERSIBLE! ***\r\n");
                                        if(newLevel == 3)
                                        {
                                            Console_Print("**************************************************\r\n");
                                            Console_Print("*     !!! EXTREME WARNING - LEVEL 3 !!!         *\r\n");
                                            Console_Print("**************************************************\r\n");
                                            Console_Print("* Level 3 PERMANENTLY blocks debug access!      *\r\n");
                                            Console_Print("* You will NOT be able to:                      *\r\n");
                                            Console_Print("*   - Program firmware via debugger             *\r\n");
                                            Console_Print("*   - Read/write any eFuse via debugger         *\r\n");
                                            Console_Print("*   - Debug the device                          *\r\n");
                                            Console_Print("**************************************************\r\n");
                                        }
                                        Console_Print("\r\nSet Debug Lock to Level ");
                                        sprintf(buf, "%d", newLevel);
                                        Console_Print(buf);
                                        Console_Print("?\r\n\r\n");

                                        if(Console_ConfirmAction())
                                        {
                                            Console_Print("Programming Debug Lock...\r\n");
                                            EFUSE_STATUS status = EFUSE_ProgramDebugLock(newLevel);
                                            if(status == EFUSE_STATUS_SUCCESS)
                                            {
                                                Console_Print("SUCCESS! Debug Lock set to Level ");
                                                sprintf(buf, "%d", newLevel);
                                                Console_Print(buf);
                                                Console_Print(". Read-back verified.\r\n");
                                            }
                                            else
                                            {
                                                Console_PrintError("Debug Lock programming", status);
                                            }
                                        }
                                        else
                                        {
                                            Console_Print("Aborted.\r\n");
                                        }
                                    }
                                }
                                else
                                {
                                    Console_Print("Invalid input.\r\n");
                                }
                            }
                        }
                        else
                        {
                            Console_PrintError("Debug Lock configuration read", readStatus);
                        }
                    }
                    break;

                case '5':
                    EFUSE_DisplaySECCFG();
                    break;

                case '6':
                    Console_Print("\r\n--- Secure Boot Key Configuration ---\r\n");
                    {
                        EFUSE_SecureBootKey_t bootKey;
                        EFUSE_SECCFG_t seccfg;
                        EFUSE_STATUS readStatus = EFUSE_ReadSECCFG(&seccfg);
                        if(readStatus != EFUSE_STATUS_SUCCESS)
                        {
                            Console_PrintError("Secure Boot Key configuration read", readStatus);
                            break;
                        }
                        readStatus = EFUSE_ReadSecureBootKey(&bootKey);
                        if(readStatus != EFUSE_STATUS_SUCCESS)
                        {
                            Console_PrintError("Secure Boot Key read", readStatus);
                            break;
                        }

                        Console_PrintSecureBootKeyStatus(&bootKey, &seccfg);
                        if(!EFUSE_IsConfiguredSecureBootKeyReady())
                        {
                            Console_Print("SBK programming and locking are unavailable. Configure the customer public key in boot_key_config.h and rebuild.\r\n");
                            break;
                        }

                        if((seccfg.rawValue & ROT_SECCFG_BOOT_KEY_LCK_Msk) == ROT_SECCFG_BOOT_KEY_LCK_Msk)
                        {
                            Console_Print("\r\nBoot Key is LOCKED - cannot modify.\r\n");
                        }
                        else
                        {
                            Console_Print("\r\nOptions:\r\n");
                            if(!seccfg.bootKeyLocked)
                            {
                                Console_Print("  [1] Program Secure Boot Key (hardcoded)\r\n");
                            }
                            else
                            {
                                Console_Print("Boot Key is locked with one bit set; option 2 verifies the key and completes the remaining lock bit.\r\n");
                            }
                            Console_Print("  [2] Lock Secure Boot Key\r\n");
                            Console_Print("  [c] Cancel\r\n");
                            Console_Print("Select: ");

                            char keyChoice;
                            if(!Console_ReadSelection(&keyChoice))
                            {
                                break;
                            }

                            if(keyChoice == '1')
                            {
                                if(seccfg.bootKeyLocked)
                                {
                                    Console_Print("Boot Key is LOCKED - cannot modify.\r\n");
                                    break;
                                }
                                Console_Print("\r\n*** HARDCODED KEY PROGRAMMING ***\r\n");
                                Console_Print("Key source: compiled-in public key (boot_key_config.h)\r\n");
                                Console_Print("\r\n*** WARNING: EFUSE programming is IRREVERSIBLE! ***\r\n\r\n");

                                if(Console_ConfirmAction())
                                {
                                    Console_Print("Programming and verifying hardcoded Secure Boot Key...\r\n");
                                    EFUSE_STATUS status = EFUSE_ProgramHardcodedKey();
                                    if(status == EFUSE_STATUS_SUCCESS)
                                    {
                                        Console_Print("SUCCESS! Secure Boot Key matches the configured key.\r\n");
                                        Console_Print("ADD_BOOT_KEY parity verification: PASSED\r\n");
                                        Console_Print("NOTE: Key is NOT locked yet.\r\n");
                                        Console_Print("Use Option 6 -> [2] to lock the Secure Boot Key.\r\n");
                                    }
                                    else if(status == EFUSE_STATUS_ALREADY_PROGRAMMED)
                                    {
                                        Console_Print("Boot Key is already locked!\r\n");
                                    }
                                    else
                                    {
                                        Console_PrintError("Secure Boot Key programming", status);
                                        Console_Print("Secure Boot Key was not locked.\r\n");
                                    }
                                }
                                else
                                {
                                    Console_Print("Aborted.\r\n");
                                }
                            }
                            else if(keyChoice == '2')
                            {
                                if(!EFUSE_IsSecureBootKeyValid(&bootKey))
                                {
                                    Console_PrintError("Secure Boot Key locking", EFUSE_STATUS_NO_KEY_DATA);
                                    break;
                                }
                                Console_Print("\r\n*** WARNING: Boot Key Lock is IRREVERSIBLE! ***\r\n");
                                Console_Print("Current key value will be permanently locked.\r\n\r\n");

                                if(Console_ConfirmAction())
                                {
                                    Console_Print("Verifying Secure Boot Key before locking...\r\n");
                                    EFUSE_STATUS verifyStatus = EFUSE_VerifyHardcodedKey();
                                    if(verifyStatus != EFUSE_STATUS_SUCCESS)
                                    {
                                        Console_PrintError("Secure Boot Key verification before locking", verifyStatus);
                                        Console_Print("Secure Boot Key was not locked.\r\n");
                                        break;
                                    }
                                    Console_Print("Verification: PASSED\r\n");
                                    Console_Print("Locking Secure Boot Key...\r\n");
                                    EFUSE_STATUS status = EFUSE_LockSecureBootKey();
                                    if(status == EFUSE_STATUS_SUCCESS)
                                    {
                                        Console_Print("SUCCESS! Boot Key Lock is 0x03. Read-back verified.\r\n");
                                    }
                                    else if(status == EFUSE_STATUS_ALREADY_PROGRAMMED)
                                    {
                                        Console_Print("Boot Key already locked.\r\n");
                                    }
                                    else
                                    {
                                        Console_PrintError("Secure Boot Key locking", status);
                                        Console_Print("Boot Key Lock was not confirmed.\r\n");
                                    }
                                }
                                else
                                {
                                    Console_Print("Aborted.\r\n");
                                }
                            }
                            else if(keyChoice == 'c' || keyChoice == 'C')
                            {
                                Console_Print("Cancelled.\r\n");
                            }
                            else
                            {
                                Console_Print("Invalid selection.\r\n");
                            }
                        }
                    }
                    break;

                case '7':
                    Console_Print("\r\n--- Storage Root Key (SRK) Configuration ---\r\n");
                    {
                        EFUSE_SECCFG_t seccfg;
                        EFUSE_STATUS readStatus = EFUSE_ReadSECCFG(&seccfg);
                        if(readStatus != EFUSE_STATUS_SUCCESS)
                        {
                            Console_PrintError("SRK configuration read", readStatus);
                            break;
                        }

                        Console_Print("ROOT_KEY_LCK Status: ");
                        if(seccfg.rootKeyLocked)
                        {
                            Console_Print("LOCKED (SRK already programmed)\r\n");
                            Console_Print("\r\nNo changes possible - SRK is locked.\r\n");
                        }
                        else
                        {
                            Console_Print("NOT LOCKED (SRK can be programmed)\r\n\r\n");
                            Console_Print("**************************************************\r\n");
                            Console_Print("*     Storage Root Key Programming               *\r\n");
                            Console_Print("**************************************************\r\n");
                            Console_Print("* This will:                                    *\r\n");
                            Console_Print("*   1. Generate SRK via TRNG                    *\r\n");
                            Console_Print("*   2. Program SRK to eFuse                     *\r\n");
                            Console_Print("*   3. IMMEDIATELY LOCK                         *\r\n");
                            Console_Print("*                                               *\r\n");
                            Console_Print("* This operation is IRREVERSIBLE!               *\r\n");
                            Console_Print("**************************************************\r\n\r\n");

                            if(Console_ConfirmAction())
                            {
                                Console_Print("Step 1: Generating SRK via TRNG...\r\n");
                                EFUSE_STATUS status = EFUSE_GenerateStorageRootKey();
                                if(status == EFUSE_STATUS_SUCCESS)
                                {
                                    Console_Print("Step 2+3: Programming and locking...\r\n");
                                    status = EFUSE_ProgramGeneratedSRK();
                                    if(status == EFUSE_STATUS_SUCCESS)
                                    {
                                        Console_Print("\r\nSUCCESS! SRK generated, programmed, and LOCKED.\r\n");
                                    }
                                    else if(status == EFUSE_STATUS_ALREADY_PROGRAMMED)
                                    {
                                        Console_Print("SRK already locked!\r\n");
                                    }
                                    else
                                    {
                                        Console_PrintError("SRK programming and locking", status);
                                    }
                                }
                                else
                                {
                                    Console_Print("FAILED! SRK generation: TRNG generation failed. Operation aborted.\r\n");
                                }
                            }
                            else
                            {
                                Console_Print("Aborted.\r\n");
                            }
                        }
                    }
                    break;

                case '8':
                    Console_Print("\r\n--- Patch Lock Configuration ---\r\n");
                    {
                        EFUSE_SECCFG_t seccfg;
                        EFUSE_STATUS status = EFUSE_ReadSECCFG(&seccfg);
                        if(status != EFUSE_STATUS_SUCCESS)
                        {
                            Console_PrintError("Patch Lock", status);
                            break;
                        }

                        Console_Print("Current Patch Lock: ");
                        Console_Print(seccfg.patchLocked ? "LOCKED" : "Unlocked");
                        Console_Print(" (value 0x");
                        Console_PrintHex8((uint8_t)((seccfg.rawValue & EFUSE_SECCFG_PATCH_LOCK_Msk) >>
                                                   EFUSE_SECCFG_PATCH_LOCK_Pos));
                        Console_Print(")\r\nCurrent SECCFG:  0x");
                        Console_PrintHex32(seccfg.rawValue);
                        Console_Print("\r\n");
                        if((seccfg.rawValue & EFUSE_SECCFG_PATCH_LOCK_Msk) == EFUSE_SECCFG_PATCH_LOCK_Msk)
                        {
                            Console_Print("Patch Code and Patch Configuration are already permanently locked.\r\n");
                            break;
                        }

                        uint32_t expectedValue = seccfg.rawValue | EFUSE_SECCFG_PATCH_LOCK_Msk;
                        Console_Print("Patch Lock will be programmed to 0x03 (Locked).\r\n");
                        if(seccfg.patchLocked)
                        {
                            Console_Print("Completing the remaining Patch Lock bit.\r\n");
                        }
                        Console_Print("\r\n*** WARNING: Patch Lock is IRREVERSIBLE! ***\r\n");
                        Console_Print("Patch Code and Patch Configuration will be permanently locked.\r\n\r\n");
                        if(Console_ConfirmAction())
                        {
                            Console_Print("Locking Patch Code and Patch Configuration...\r\n");
                            status = EFUSE_LockPatch();
                            if(status != EFUSE_STATUS_SUCCESS && status != EFUSE_STATUS_ALREADY_PROGRAMMED)
                            {
                                Console_PrintError("Patch Lock", status);
                            }
                            else
                            {
                                status = EFUSE_ReadSECCFG(&seccfg);
                                if(status != EFUSE_STATUS_SUCCESS)
                                {
                                    Console_PrintError("Patch Lock", status);
                                }
                                else
                                {
                                    Console_Print("Read-back SECCFG: 0x");
                                    Console_PrintHex32(seccfg.rawValue);
                                    Console_Print("\r\nRead-back Patch Lock: 0x");
                                    Console_PrintHex8((uint8_t)((seccfg.rawValue & EFUSE_SECCFG_PATCH_LOCK_Msk) >>
                                                               EFUSE_SECCFG_PATCH_LOCK_Pos));
                                    Console_Print(seccfg.patchLocked ? " (Locked)\r\n" : " (Unlocked)\r\n");
                                    if(seccfg.rawValue == expectedValue)
                                    {
                                        Console_Print("SUCCESS! Patch Lock is LOCKED. Read-back matches the expected configuration.\r\n");
                                    }
                                    else
                                    {
                                        Console_PrintError("Patch Lock", EFUSE_STATUS_VERIFY_ERROR);
                                    }
                                }
                            }
                        }
                        else
                        {
                            Console_Print("Aborted.\r\n");
                        }
                    }
                    break;

                case '9':
                    Display_Help();
                    break;

                case '0':
                    Console_Print("\r\nExiting... Goodbye!\r\n");
                    appData.state = APP_STATE_IDLE;
                    break;

                default:
                    Console_Print("Invalid selection! Please try again.\r\n");
                    break;
            }
                break;
 
        }
        case APP_STATE_IDLE:
        {
             // Application stopped - do nothing
            break;
         }


        /* TODO: implement your application state machine.*/


        /* The default state should never be executed. */
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}

 
/*******************************************************************************
 End of File
 */
