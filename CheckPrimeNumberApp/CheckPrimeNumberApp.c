/** @file
  This is a simple shell application

  Copyright (c) 2020, Intel Corporation                                                         
  All rights reserved. This program and the accompanying materials                          
  are licensed and made available under the terms and conditions of the BSD License         
  which accompanies this distribution.  The full text of the license may be found at        
  http://opensource.org/licenses/bsd-license.php                                            

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,                     
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.             

**/
#include <Uefi.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>

#include <Library/BaseMemoryLib.h>


//EFI_STATUS
//EFIAPI
//WaitForKeyPress(
//)
//{
//	UINTN                      EventIndex;
//	gBS->WaitForEvent(1, &gST->ConIn->WaitForKey, &EventIndex);
//	return EFI_SUCCESS;
//}

EFI_STATUS
EFIAPI
ReadKeyPress(
 OUT	EFI_INPUT_KEY  *TheKey
)
{
	UINTN                      EventIndex;
	// Wait for Key event then read the key from the keyboard buffer in to "TheKey" from caller
	gBS->WaitForEvent(1, &gST->ConIn->WaitForKey, &EventIndex);
	gST->ConIn->ReadKeyStroke(gST->ConIn, TheKey);
	return EFI_SUCCESS;
}

BOOLEAN IsPrimeCheck(
	IN UINTN Number
)
{
	BOOLEAN Result = TRUE;
	UINTN i;
	
	if (Number <= 1) Result = FALSE;
	for (i = 2; (i * i) <= Number; i++) {
		if (Number % i == 0 ) Result = FALSE;
	}

	return Result;  // Prime number
}

/**
  as the real entry point for the application.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.  
  @param[in] SystemTable    A pointer to the EFI System Table.
  
  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

**/
EFI_STATUS
EFIAPI
UefiMain (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
	UINTN Number;
	EFI_INPUT_KEY  Key;
	EFI_STATUS      Status;
	Status = EFI_UNSUPPORTED;  // or any other EFI Error 

	DEBUG((EFI_D_INFO, "\r\n>>>>>[UefiMain] Entry point: 0x%p <<<<<<\r\n", UefiMain));
	ZeroMem(&Key, sizeof(EFI_INPUT_KEY));

	Print(L"\nPress any Key to continue : \n");
	//Status = WaitForKeyPress();
	Status = ReadKeyPress(&Key);
	Print(L"The Key you pressed was:\n "); //

	Number = (UINTN)Key.UnicodeChar;
	Print(L"%c the Hex is: %x the Dec is: %d\n", Key.UnicodeChar, Number, Number);

	
	// check if the Number is Prime
	if (IsPrimeCheck(Number) ) {
		Print(L"The number %d is prime", Number);
	}
	else {
		Print(L"The number %d is Not prime", Number);
	}

	Print(L"\n");


	return EFI_SUCCESS;
}