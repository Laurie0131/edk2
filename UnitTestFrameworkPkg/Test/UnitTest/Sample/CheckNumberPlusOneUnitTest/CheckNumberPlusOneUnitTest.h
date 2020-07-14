/** @file
  This is a sample to demostrate the usage of the Unit Test Library that
  supports the PEI, DXE, SMM, UEFI SHell, and host execution environments.

   Copyright (c) Microsoft Corporation.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef _CHECK_NUMBER_PLUS_ONE_UNIT_TEST_H_
#define _CHECK_NUMBER_PLUS_ONE_UNIT_TEST_H_
#include <PiPei.h>
#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/UnitTestLib.h>
#include <Library/PrintLib.h>


/**
  Super Sample FUNCTION TO TEST Prototype
  @param[in]  Number    Unsigned invalue  if < 100
  @retval		2		Return Number+1 if Number < 100
  @retval       0		if Number >= 100
  **/
UINTN NumberPlusOne(
	IN UINTN Number
);





#endif