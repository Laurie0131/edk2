/** @file
  This is a sample to demostrate the usage of the Unit Test Library that
  supports the PEI, DXE, SMM, UEFI SHell, and host execution environments.

  Copyright (c) Microsoft Corporation.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include <PiPei.h>
#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/UnitTestLib.h>
#include <Library/PrintLib.h>

#define UNIT_TEST_NAME     "Sample Check Number Plus One Unit Test"
#define UNIT_TEST_VERSION  "0.1"



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



/**OnePlusOne
Sample unit test that verifies the expected result of an unsigned integer
addition operation.

@param[in]  Context    [Optional] An optional parameter that enables:
1) test-case reuse with varied parameters and
2) test-case re-entry for Target tests that need a
reboot.  This parameter is a VOID* and it is the
responsibility of the test author to ensure that the
contents are well understood by all test cases that may
consume it.

@retval  UNIT_TEST_PASSED             The Unit test has completed and the test
case was successful.
@retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.
**/
UNIT_TEST_STATUS
EFIAPI
IsNumberPlus1(
	IN UNIT_TEST_CONTEXT  Context
)
{
	UINTN  RetNumber;

	RetNumber = NumberPlusOne(1);
	UT_ASSERT_EQUAL(RetNumber, 2);
	return UNIT_TEST_PASSED;
}


/**
Sample unit test that verifies the expected result of an unsigned integer
addition operation.

@param[in]  Context    [Optional] An optional parameter that enables:
1) test-case reuse with varied parameters and
2) test-case re-entry for Target tests that need a
reboot.  This parameter is a VOID* and it is the
responsibility of the test author to ensure that the
contents are well understood by all test cases that may
consume it.

@retval  UNIT_TEST_PASSED             The Unit test has completed and the test
case was successful.
@retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.
**/
UNIT_TEST_STATUS
EFIAPI
IsNumberNotPlus1(
	IN UNIT_TEST_CONTEXT  Context
)
{
	UINTN  RetNumber;

	RetNumber = NumberPlusOne(100);
	UT_ASSERT_NOT_EQUAL(RetNumber, 101);
	return UNIT_TEST_PASSED;
}

/**
Sample unit test that verifies the expected result of an unsigned integer
addition operation.

@param[in]  Context    [Optional] An optional parameter that enables:
1) test-case reuse with varied parameters and
2) test-case re-entry for Target tests that need a
reboot.  This parameter is a VOID* and it is the
responsibility of the test author to ensure that the
contents are well understood by all test cases that may
consume it.

@retval  UNIT_TEST_PASSED             The Unit test has completed and the test
case was successful.
@retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.
**/
UNIT_TEST_STATUS
EFIAPI
IsNumberCorrect(
	IN UNIT_TEST_CONTEXT  Context
)
{
	UINTN  RetNumber;
	// Check for !2 condition 
	RetNumber = NumberPlusOne(100);
	UT_LOG_INFO("Number is: 100 returned was (%d)\n", RetNumber);
	UT_ASSERT_NOT_EQUAL(RetNumber, 101);

	// Check for is 2 condition 
	RetNumber = NumberPlusOne(1);
	UT_LOG_INFO("Number is:   1 returned was (%d)\n", RetNumber);
	UT_ASSERT_EQUAL(RetNumber, 2);

	return UNIT_TEST_PASSED;
}




/**
  Initialize the unit test framework, suite, and unit tests for the
  sample unit tests and run the unit tests.

  @retval  EFI_SUCCESS           All test cases were dispatched.
  @retval  EFI_OUT_OF_RESOURCES  There are not enough resources available to
                                 initialize the unit tests.
**/
EFI_STATUS
EFIAPI
UefiTestMain (
  VOID
  )
{
  EFI_STATUS                  Status;
  UNIT_TEST_FRAMEWORK_HANDLE  Framework;
  UNIT_TEST_SUITE_HANDLE      SimpleNumberTests;
//  UNIT_TEST_SUITE_HANDLE      GlobalVarTests;

  Framework = NULL;

  DEBUG(( DEBUG_INFO, "%a v%a\n", UNIT_TEST_NAME, UNIT_TEST_VERSION ));

  //
  // Start setting up the test framework for running the tests.
  //
  Status = InitUnitTestFramework (&Framework, UNIT_TEST_NAME, gEfiCallerBaseName, UNIT_TEST_VERSION);
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_ERROR, "Failed in InitUnitTestFramework. Status = %r\n", Status));
    goto EXIT;
  }

  //
  // Populate the SimpleMathTests Unit Test Suite.
  //
  Status = CreateUnitTestSuite (&SimpleNumberTests, Framework, "Simple Number Tests", "Sample.Test", NULL, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed in CreateUnitTestSuite for SimpleMathTests\n"));
    Status = EFI_OUT_OF_RESOURCES;
    goto EXIT;
  }

  AddTestCase (SimpleNumberTests, "Test IsNumberPlus1 -  produce 1+1=2", "Test-1", IsNumberPlus1, NULL, NULL, NULL);
  AddTestCase (SimpleNumberTests, "Test IsNumberNotPlus1 - Not produce number +1", "Test-2", IsNumberNotPlus1, NULL, NULL, NULL);
  AddTestCase(SimpleNumberTests, "Test IsNumberCorrect w/ results in one test", "Test-3", IsNumberCorrect, NULL, NULL, NULL);

  //
  // Execute the tests.
  //
  Status = RunAllTestSuites (Framework);

EXIT:
  if (Framework) {
    FreeUnitTestFramework (Framework);
  }
  DEBUG((DEBUG_INFO, "Status is %x\n", Status));
  return Status;
}

/**
  Standard PEIM entry point for target based unit test execution from PEI.
**/
EFI_STATUS
EFIAPI
PeiEntryPoint (
  IN EFI_PEI_FILE_HANDLE     FileHandle,
  IN CONST EFI_PEI_SERVICES  **PeiServices
  )
{
  return UefiTestMain ();
}

/**
  Standard UEFI entry point for target based unit test execution from DXE, SMM,
  UEFI Shell.
**/
EFI_STATUS
EFIAPI
DxeEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  return UefiTestMain ();
}

/**
  Standard POSIX C entry point for host based unit test execution.
**/
int
main (
  int argc,
  char *argv[]
  )
{
  return UefiTestMain ();
}
