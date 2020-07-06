/** @file
  This is a sample to demostrate the usage of the Unit Test Library that
  supports the PEI, DXE, SMM, UEFI SHell, and host execution environments.

  Copyright (c) Microsoft Corporation.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "CheckNumberPlusOneUnitTest.h"

/**
Super Sample FUNCTION TO TEST
@param[in]  Number    Unsigned invalue  if < 100
@retval		2		Return Number+1 if Number < 100
@retval       0		if Number >= 100
**/
UINTN NumberPlusOne(
	IN UINTN Number
)
{
	UINTN	ReturnNumber = 0;
	if (Number < 100) {
		ReturnNumber = Number + 1;
	}
	return (ReturnNumber);  // 1 + 1
}
