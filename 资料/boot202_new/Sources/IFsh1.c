/** ###################################################################
**     THIS BEAN MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename  : IFsh1.C
**     Project   : Project
**     Processor : MC9S12XET256MAL
**     Beantype  : IntFLASH
**     Version   : Bean 02.327, Driver 02.12, CPU db: 3.00.002
**     Compiler  : CodeWarrior HCS12X C Compiler
**     Date/Time : 2014-5-16, 16:03
**     Abstract  :
**         This bean "IntFLASH" implements an access to internal FLASH.
**         The bean support reading/writing data into FLASH, erasing of
**         selected sector.
**         The bean supports events if the write interrupt is supported.
**         The bean supports following modes of write operations:
**           - Write - writing without any test.
**           - Destructive write - sector is erased if necessary.
**           - Safe write - user event is invoked to save and resore data
**                          from the current sector.
**         The bean requires on-chip FLASH memory (not used/allocated by
**         other beans).
**     Settings  :
**         Total FLASH memory size       : 256KB
**         Number of FLASH memory blocks : 2
**         Interrupt service             : Disabled
**         Write method                  : Write
**         Wait in RAM                   : yes
**     Contents  :
**         EraseSector   - byte IFsh1_EraseSector(IFsh1_TAddress Addr);
**         SetProtection - byte IFsh1_SetProtection(bool ProtectType, byte ProtectHigh, byte ProtectLow,...
**         SetBlockFlash - byte IFsh1_SetBlockFlash(IFsh1_TDataAddress Source, IFsh1_TAddress Dest, word...
**         GetBlockFlash - byte IFsh1_GetBlockFlash(IFsh1_TAddress Source, IFsh1_TDataAddress Dest, word...
**         DataPtr2Addr  - IFsh1_TAddress IFsh1_DataPtr2Addr(void* Addr);
**         FuncPtr2Addr  - IFsh1_TAddress IFsh1_FuncPtr2Addr(void(*Addr)());
**
**     (c) Copyright UNIS, spol. s r.o. 1997-2007
**     UNIS, spol. s r.o.
**     Jundrovska 33
**     624 00 Brno
**     Czech Republic
**     http      : www.processorexpert.com
**     mail      : info@processorexpert.com
** ###################################################################*/


/* MODULE IFsh1. */

#include "IFsh1.h"

#pragma DATA_SEG IFsh1_DATA
#pragma CODE_SEG IFsh1_CODE

#pragma OPTION ADD "-Onf"

/* Types definitions */

typedef struct {
   unsigned char code[15];             /* Structure required to copy code to ram memory */
  /* Size of this structure needs to be at least (but best) the size of the FnCmdInRam_ */
} FnCmdInRamStruct;

typedef void (* near pFnCmdInRam)(void);

/* Global variables */



#pragma CODE_SEG __NEAR_SEG NON_BANKED

static void FnCmdInRam_(void)
{
  FSTAT = 0x80;                        /* Clear flag command buffer empty */
  while (FSTAT_CCIF == 0) {}           /* Wait to command complete */
  return;
}

#pragma MESSAGE DISABLE C1805
static void CallFnCmdInRam()
{
  FnCmdInRamStruct FnCmdInRam = *(FnCmdInRamStruct *)(FnCmdInRam_); /* Create a copy of Wait in RAM routine on stack */

  ((pFnCmdInRam)&FnCmdInRam)();        /* Call code in RAM */
  return;
}

#pragma CODE_SEG IFsh1_CODE

#pragma MESSAGE DISABLE C4001
/*
** ===================================================================
**     Method      :  BlockOutOfRange (bean IntFLASH)
**
**     Description :
**         The method checks if selected block is present in the flash 
**         module.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
#define BlockOutOfRange(Addr1, Addr2) ((Addr1 > Addr2) || ((Addr1 < 7864320L) || (Addr2 > 7995391L)) && ((Addr1 < 8257536L) || (Addr2 > 8388607L)))

/*
** ===================================================================
**     Method      :  ClearFlags (bean IntFLASH)
**
**     Description :
**         The method clear error flags of Flash module.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
#define ClearFlags()  (FSTAT = 48)


/*
** ===================================================================
**     Method      :  WriteArray (bean IntFLASH)
**
**     Description :
**         The method writes a variable number of bytes  to the Flash 
**         memory.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
static byte WriteArray(IFsh1_TAddress Addr, byte *Data, word Size)
{
  word i;
  byte err = 0;
  dword PhraseAddr;                    /* Address of a phrase (8-byte alligned) */
  byte diff;
  byte j;

  EnterCritical();                     /* Enter critical section */
  ClearFlags();                        /* Clear all flags */
  PhraseAddr = Addr & 4294967288L;     /* Phrase is aligned to 8 bytes */
  diff = (byte)(Addr & 7);             /* difference between address and phrase */
  i = 0;
  while (i < Size) {
    FCCOBIX = 0;                       /* Clear index register */
    FCCOBHI = 6;                       /* Program P-Flash command */
    FCCOBLO = (byte)(PhraseAddr >> 16); /* High address word */
    FCCOBIX++;                         /* Shift index register */
    FCCOB = (word)(PhraseAddr);        /* Low address word */
    for(j = 0; j < 8; j += 2) {
      FCCOBIX++;                       /* Shift index register */
      if((diff > 0) || (i >= Size)) {  /* Is address outside written range ? */
        FCCOB = *((word *far)(PhraseAddr + j)); /* Yes - Load previously stored data */
        diff-=2;
      } else {
        FCCOB = *((word *far)(Data + i)); /* Load new data */
        i += 2;
      }
    } /* for j */
    CallFnCmdInRam();                  /* Copy Wait in RAM routine to stack and launch the flash process */
    if ((FSTAT_FPVIOL == 1) || (FSTAT_ACCERR == 1)) { /* Return error code if previous operation finished not correctly */
      ExitCritical();                  /* Exit critical section */
      return ERR_NOTAVAIL;
    }
    if (FSTAT_MGSTAT) {                /* Was attempt to write data to the given address errorneous? */
      err = 1;
    }
    PhraseAddr += 8;
  }
  if(err) {
    ExitCritical();                    /* Exit critical section */
    return ERR_VALUE;                  /* If yes then error */
  }
  ExitCritical();                      /* Exit critical section */
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  IFsh1_SetProtection (bean IntFLASH)
**
**     Description :
**         Method allows write to protection register. Please see
**         Flash Block User Guide.
**     Parameters  :
**         NAME            - DESCRIPTION
**         ProtectType     - Possible values: 0 or 1.
**                           Depend on CPU type. Two sense are
**                           possible.
**                           First:
**                           - 0 = whole Flash array is protected. In
**                           this case other parameters are don't
**                           care.
**                           - 1 = protection size depend on
**                           ProtectHigh and ProtectLow parameters.
**                           Second:
**                           - 0 = ProtectHigh and ProtectLow
**                           parameters define range to by protected .
**                           - 1 = ProtectHigh and ProtectLow
**                           parameters define range to by
**                           unprotected .
**         ProtectHigh     - Flash higher address
**                           range protection. Possible values:
**                           - 0 - 3 - defines range (size of range
**                           depend on CPU type) , 4 - Protection
**                           disabled
**         ProtectLow      - Flash lower address
**                           range protection. Possible values:
**                           - 0 - 3 - defines range (size of range
**                           depend on CPU type) , 4 - Protection
**                           disabled
**         Block           - Flash block number. Number of
**                           blocks depend on CPU type. If CPU has
**                           only one block, the value is don't care.
**     Returns     :
**         ---             - Error code, possible codes:
**                           - ERR_OK - OK
**                           - ERR_NOTAVAIL - Requested value not
**                           operation is not available
**                           - ERR_RANGE - Value is out of range
**                           - ERR_VALUE - Parameter of incorrect
**                           value
**                           - ERR_SPEED - This device does not work
**                           in the active speed mode
** ===================================================================
*/
byte IFsh1_SetProtection_(bool ProtectType,byte ProtectHigh,byte ProtectLow)
{
  byte TempFPROT;

  if ((ProtectHigh > 4)||(ProtectLow > 4)||(ProtectType > 1)) { /* Range check */
    return ERR_RANGE;
  }
  TempFPROT = (ProtectType<<7) | (ProtectHigh<<3) | ProtectLow; /* Set temoraly variable */
  if((TempFPROT & 36) != 36) {         /* Is FPHDIS or FPLDIS cleared ? */
    FPROT = TempFPROT | 36 ;           /* Set protection with FPHDIS and FPLDIS set first */
  }
  FPROT = TempFPROT;                   /* Set new protection */
  if ((FPROT & 0xBF) != (TempFPROT & 0xBF)) {
    return ERR_VALUE;                  /* Was protection register set corectly? */
  }
  return ERR_OK;
}

/*
** ===================================================================
**     Method      :  IFsh1_SetBlockFlash (bean IntFLASH)
**
**     Description :
**         Writes data to FLASH.
**     Parameters  :
**         NAME            - DESCRIPTION
**         Source          - Source address of the data
**         Dest            - Destination address in FLASH
**         Count           - Count of the data fields in the
**                           smallest addressable units.
**     Returns     :
**         ---             - Error code
** ===================================================================
*/
byte IFsh1_SetBlockFlash(IFsh1_TDataAddress Source, IFsh1_TAddress Addr, word Count)
{
  byte err;

  if (BlockOutOfRange(Addr, (Addr + Count))) { /* Check range of address */
    return(ERR_RANGE);
  }
  if ((Addr & 1)||(Count & 1)) {       /* Word aligned address and count? */
    return ERR_NOTAVAIL;
  }
  if (FSTAT_CCIF == 0) {               /* Is previous command complete ? */
    return ERR_BUSY;                   /* If yes then error */
  }
  err = WriteArray(Addr, Source, Count); /* Write new page to Flash */
  return(err);                         /* Return error */
}
/*
** ===================================================================
**     Method      :  IFsh1_GetBlockFlash (bean IntFLASH)
**
**     Description :
**         Reads data from FLASH.
**     Parameters  :
**         NAME            - DESCRIPTION
**         Source          - Source address of the data.
**         Dest            - Destination address
**         Count           - Count of the data fields in the
**                           smallest addressable units.
**     Returns     :
**         ---             - Error code
** ===================================================================
*/
byte IFsh1_GetBlockFlash(IFsh1_TAddress Addr, IFsh1_TDataAddress Dest, word Count)
{
  if (BlockOutOfRange(Addr, (Addr + Count))) { /* Check range of address */
    return(ERR_RANGE);
  }
  if ((Addr & 1)||(Count & 1)) {       /* Word aligned address and count? */
    return ERR_NOTAVAIL;
  }
  if (FSTAT_CCIF == 0) {               /* Is previous command complete ? */
    return ERR_BUSY;                   /* If yes then error */
  }
  while(Count--) {
    *Dest = *(byte *far)(Addr);        /* Set virtual page element according to value of Flash given by the appropriate address */
    Addr++;
    Dest++;
  } /* while Count */
  return ERR_OK;
}

/*
** ===================================================================
**     Method      :  IFsh1_EraseSector (bean IntFLASH)
**
**     Description :
**         Erase sector to which address Addr belongs.
**     Parameters  :
**         NAME            - DESCRIPTION
**         Addr            - Address in FLASH.
**     Returns     :
**         ---             - Error code, possible codes:
**                           - ERR_OK - OK
**                           - ERR_NOTAVAIL - Desired program/erase
**                           operation is not available
**                           - ERR_RANGE - Address is out of range
**                           - ERR_SPEED - This device does not work
**                           in the active speed mode
** ===================================================================
*/
byte IFsh1_EraseSector(IFsh1_TAddress Addr)
{
  if (BlockOutOfRange(Addr, Addr)) {   /* Check range of address */
    return(ERR_RANGE);
  }
  EnterCritical();                     /* Enter critical section */
  ClearFlags();                        /* Clear all flags */
  if (FSTAT_CCIF == 0) {               /* Is command buffer full ? */
    ExitCritical();                    /* Exit critical section */
    return ERR_BUSY;                   /* If yes then error */
  }
  FCCOBIX = 0;                         /* Clear index register */
  FCCOBHI = 10;                        /* Erase P-Flash sector command */
  FCCOBLO = (byte)(Addr >> 16);        /* High address word */
  FCCOBIX++;                           /* Shift index register */
  FCCOB = (word)(Addr & 4294967288L);  /* Low address word aligned to 8 byte phrase*/
  CallFnCmdInRam();                    /* Copy Wait in RAM routine to stack and launch the flash process */
  if ((FSTAT_FPVIOL == 1)||(FSTAT_ACCERR == 1)) { /* Is protection violation or access error detected ? */
    ExitCritical();                    /* Exit critical section */
    return ERR_NOTAVAIL;               /* If yes then error */
  }
  if (FSTAT_MGSTAT) {                  /* Was attempt to erase the sector errorneous? */
    ExitCritical();                    /* Exit critical section */
    return ERR_VALUE;                  /* If yes then error */
  }
  ExitCritical();                      /* Exit critical section */
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  IFsh1_DataPtr2Addr (bean IntFLASH)
**
**     Description :
**         This method converts data pointer to format of a bean method
**         address parameter. Generally a data pointer format is
**         different from format of a method Addr parameter.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * Addr            - Data pointer.
**     Returns     :
**         ---             - Address in the format used in the bean
**                           methods.
** ===================================================================
*/
IFsh1_TAddress IFsh1_DataPtr2Addr(void *far Addr)
{
  return((IFsh1_TAddress)((void *far)Addr));
}

#pragma MESSAGE DISABLE C1860 /* WARNING C1860: Pointer conversion - possible loss of data is ignored */

/*
** ===================================================================
**     Method      :  IFsh1_FuncPtr2Addr (bean IntFLASH)
**
**     Description :
**         This method converts function pointer to format of a bean
**         method address parameter. Generally a function pointer
**         format is different from format of a method Addr parameter.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * Addr            - Function pointer.
**     Returns     :
**         ---             - Address in the format used in the bean
**                           methods.
** ===================================================================
*/
IFsh1_TAddress IFsh1_FuncPtr2Addr(void(*far Addr)())
{

  if((((dword)Addr) > 65535UL)&&(((dword)Addr & 255) != 0)){ /* Is address banked ? */
    return((((IFsh1_TAddress)Addr & 255) << 14) + (((IFsh1_TAddress)Addr & 4194048UL)>>8) + 4194304UL);
  } else {
    if(((dword)Addr) > 65535UL){       /* if bank==0 ? */
      Addr = (void(*far)())((dword)Addr >> 8); /* Discard bank info */
    }
    return((IFsh1_TAddress)Addr + 8323072UL);
  }
}

/*
** ===================================================================
**     Method      :  IFsh1_Init (bean IntFLASH)
**
**     Description :
**         Initializes the associated peripheral(s) and the bean internal 
**         variables. The method is called automatically as a part of the 
**         application initialization code.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/

//void IFsh1_Init(void)
//{
 // FCLKDIV = 15;                        /* Set up Clock Divider Register */
 // FERCNFG &= 252;                        /* Disable error interrupts */
//}


/* END IFsh1. */

/*
** ###################################################################
**
**     This file was created by UNIS Processor Expert 2.98 [03.98]
**     for the Freescale HCS12X series of microcontrollers.
**
** ###################################################################
*/
