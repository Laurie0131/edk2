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
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#define UNIT_TEST_NAME     "Sample Chef cook Unit Test"
#define UNIT_TEST_VERSION  "0.1"
#define A_PRIME_NUMBER  197
#define NOT_PRIME_NUMBER  24
#define MAX_DISH_SIZE 16
///
/// Global variables used in unit tests
///
BOOLEAN  mSampleGlobalTestBoolean  = FALSE;
VOID     *mSampleGlobalTestPointer = NULL;


/**
*A chef object would return:
*0 - cooking dish went fine
* -1 - unknown dish
* -2 - ran out of ingredients for the dish
* any other error code -- unexpected error while cooking
*
* The return codes should be consistent between the real and mocked objects.
*/

INTN __wrap_chef_cook(
	IN CONST CHAR16 *order, 
	OUT CHAR16 **dish_out)
{
	BOOLEAN  has_ingredients;
	BOOLEAN  knows_dish;
	CHAR16 *dish;
	EFI_STATUS  Status;

	check_expected_ptr(order);

	knows_dish = mock_type(BOOLEAN);
	if (knows_dish == FALSE) {
		return -1;
	}

	has_ingredients = mock_type(BOOLEAN);
	if (has_ingredients == FALSE) {
		return -2;
	}

	dish = mock_ptr_type(CHAR16 *);
	DEBUG((DEBUG_INFO, ">>> Chef test >> The Dish to get is: %s \n", dish ));
	if (*dish_out == NULL) {
		*dish_out = (CHAR16*)AllocateZeroPool(MAX_DISH_SIZE * sizeof(CHAR16));
		DEBUG((DEBUG_INFO, ">>> Chef test >> Need to get memory for string \n"));
	}
	Status = StrCpyS(*dish_out, (MAX_DISH_SIZE * sizeof(CHAR16)) , dish);
	if (EFI_ERROR(Status)) {
		DEBUG((DEBUG_INFO, ">>> Chef test >> Failed to copy dish to dish_out. Status = %r\n", Status));
		return -1;
	}
	//*dish_out = strdup(dish);
	//if (*dish_out == NULL) return ENOMEM;

	return mock_type(INTN);
}


/* This is the real chef, just not implemented yet, currently it always
* returns ENOSYS
*/
INTN chef_cook(
	CONST CHAR16 *order,
	CHAR16 **dish_out
)
{
	DEBUG((DEBUG_INFO, ">>> Chef test >> Failed to call the mock version of chef_cook\n"));
		return -1;
}


/* Waiter return codes:
* This is the function to test 
* it calls chef_cook 
*  0  - success
* -1  - kitchen failed
* -2  - kitchen succeeded, but cooked a different food
*/
STATIC INTN waiter_process(
	IN CONST CHAR16 *order, 
	OUT CHAR16 **dish)
{
	INTN rv;

	rv = __wrap_chef_cook(order, dish);
	if (rv != 0) {
		DEBUG((DEBUG_INFO, ">>> Chef test >> Chef couldn't cook %s: because code: %d \n", order, rv));
		return -1;
	}

	/* Check if we received the dish we wanted from the kitchen */
	if (StrCmp(order, *dish) != 0) {
		DEBUG((DEBUG_INFO, ">>> Chef test >> Did not receive the correct dish\n"));
		FreePool(*dish);
		*dish = NULL;
		return -2;
	}

	return 0;
}

UNIT_TEST_STATUS
EFIAPI
test_order_hotdog(
	IN UNIT_TEST_CONTEXT  Context
)
{
	INTN rv;
	CHAR16 *dish = NULL;

	//dish = mChefDish;
	//ZeroMem(&dish, sizeof(dish));
	//(void)state; /* unused */

				 /* We expect the chef to receive an order for a hotdog */
	expect_string(__wrap_chef_cook, order, L"hotdog");
	/* And we tell the test chef that ke knows how to cook a hotdog
	* and has the ingredients
	*/
	will_return(__wrap_chef_cook, TRUE);
	will_return(__wrap_chef_cook, TRUE);
	/* The result will be a hotdog and the cooking process will succeed */
	will_return(__wrap_chef_cook, cast_ptr_to_largest_integral_type(L"hotdog"));
	will_return(__wrap_chef_cook, 0);

	/* Test the waiter */
	rv = waiter_process(L"hotdog", &dish);

	/* We expect the cook to succeed cooking the hotdog */
	UT_ASSERT_EQUAL(rv, 0);
	/* And actually receive one */
	UT_ASSERT_MEM_EQUAL(dish, L"hotdog", sizeof(L"hotdog"));
	if (dish != NULL) {
		FreePool(dish);
	}

	return UNIT_TEST_PASSED;
	
}

UNIT_TEST_STATUS
EFIAPI
test_bad_dish(
	IN UNIT_TEST_CONTEXT  Context
	)
{
	INTN rv;
	CHAR16 *dish = NULL;


//	dish = mChefDish;
//	ZeroMem(&dish, sizeof(dish));
    //	(void)state; /* unused */

				 /* We expect the chef to receive an order for a hotdog */
	expect_string(__wrap_chef_cook, order, L"hotdog");
	/* And we tell the test chef that ke knows how to cook a hotdog
	* and has the ingredients
	*/
	will_return(__wrap_chef_cook, TRUE);
	will_return(__wrap_chef_cook, TRUE);
	/* The result will be a burger and the cooking process will succeed.
	* We expect the waiter to handle the bad dish and return an error
	* code
	*/
	will_return(__wrap_chef_cook, cast_ptr_to_largest_integral_type(L"burger"));
	will_return(__wrap_chef_cook, 0);

	/* Test the waiter */
	rv = waiter_process(L"hotdog", &dish);

	/* According to the documentation the waiter should return -2 now */
	UT_ASSERT_EQUAL(rv, -2);
	/* And do not give the bad dish to the customer */
	UT_ASSERT_EQUAL((UINTN)dish, (UINTN)NULL);
	//assert_null(dish);
	return UNIT_TEST_PASSED;
}


/**
  Sample Unit-Test Prerequisite Function that checks to make sure the global
  pointer used in the test is already set to NULL.

  Functions with this prototype are registered to be dispatched by the unit test
  framework prior to a given test case. If this prereq function returns
  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET, the test case will be skipped.

  @param[in]  Context    [Optional] An optional parameter that enables:
                         1) test-case reuse with varied parameters and
                         2) test-case re-entry for Target tests that need a
                         reboot.  This parameter is a VOID* and it is the
                         responsibility of the test author to ensure that the
                         contents are well understood by all test cases that may
                         consume it.

  @retval  UNIT_TEST_PASSED                      Unit test case prerequisites
                                                 are met.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
MakeSureThatPointerIsNull (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  UT_ASSERT_EQUAL ((UINTN)mSampleGlobalTestPointer, (UINTN)NULL);
  return UNIT_TEST_PASSED;
}

/**
  Sample Unit-Test Cleanup (after) function that resets the global pointer to
  NULL.

  Functions with this prototype are registered to be dispatched by the
  unit test framework after a given test case. This will be called even if the
  test case returns an error, but not if the prerequisite fails and the test is
  skipped.  The purpose of this function is to clean up any global state or
  test data.

  @param[in]  Context    [Optional] An optional parameter that enables:
                         1) test-case reuse with varied parameters and
                         2) test-case re-entry for Target tests that need a
                         reboot.  This parameter is a VOID* and it is the
                         responsibility of the test author to ensure that the
                         contents are well understood by all test cases that may
                         consume it.

  @retval  UNIT_TEST_PASSED                Test case cleanup succeeded.
  @retval  UNIT_TEST_ERROR_CLEANUP_FAILED  Test case cleanup failed.

**/
VOID
EFIAPI
ClearThePointer (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  mSampleGlobalTestPointer = NULL;
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
OnePlusOneShouldEqualTwo (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  UINTN  A;
  UINTN  B;
  UINTN  C;

  A = 1;
  B = 1;
  C = A + B;

  UT_ASSERT_EQUAL (C, 2);

  return UNIT_TEST_PASSED;
}

/**
Function to test will return Boolean True if number is Prime or False if Number is Not Prime
@param[in] Number	Integer to check if prime number
@retval  TRUE		Number passed was prime
@retval  FALSE		Number passed was Not prime
*/
BOOLEAN IsNumberPrime(
	IN INTN Number
)
{
	INTN i;
	BOOLEAN NumberisPrime = TRUE;  // True if the number passed is prime
	if (Number <= 1) NumberisPrime = FALSE; // not prime
	for (i = 2; (i * i) <= Number; i++) {
		if (Number % i == 0) 
			NumberisPrime = FALSE; // not prime
	}

	return (NumberisPrime);  // Prime number
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
IsNumberPrimeTestTrue(
	IN UNIT_TEST_CONTEXT  Context
)
{
	UT_ASSERT_TRUE (IsNumberPrime(A_PRIME_NUMBER));
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
IsNumberPrimeTestFalse(
	IN UNIT_TEST_CONTEXT  Context
)
{
	UT_ASSERT_FALSE (IsNumberPrime(NOT_PRIME_NUMBER));
	return UNIT_TEST_PASSED;
}


/**
  Sample unit test that verifies that a global BOOLEAN is updatable.

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
GlobalBooleanShouldBeChangeable (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  mSampleGlobalTestBoolean = TRUE;
  UT_ASSERT_TRUE (mSampleGlobalTestBoolean);

  mSampleGlobalTestBoolean = FALSE;
  UT_ASSERT_FALSE (mSampleGlobalTestBoolean);

  return UNIT_TEST_PASSED;
}

/**
  Sample unit test that logs a warning message and verifies that a global
  pointer is updatable.

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
GlobalPointerShouldBeChangeable (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  //
  // Example of logging.
  //
  UT_LOG_WARNING ("About to change a global pointer! Current value is 0x%X\n", mSampleGlobalTestPointer);

  mSampleGlobalTestPointer = (VOID *)-1;
  UT_ASSERT_EQUAL ((UINTN)mSampleGlobalTestPointer, (UINTN)((VOID *)-1));
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
  UNIT_TEST_SUITE_HANDLE      SimpleMathTests;
  UNIT_TEST_SUITE_HANDLE      GlobalVarTests;

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
  Status = CreateUnitTestSuite (&SimpleMathTests, Framework, "Simple Math Tests", "Sample.Math", NULL, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed in CreateUnitTestSuite for SimpleMathTests\n"));
    Status = EFI_OUT_OF_RESOURCES;
    goto EXIT;
  }

  AddTestCase (SimpleMathTests, "Adding 1 to 1 should produce 2", "Addition", OnePlusOneShouldEqualTwo, NULL, NULL, NULL);
  AddTestCase (SimpleMathTests, "Number should be Prime", "Prime1", IsNumberPrimeTestTrue, NULL, NULL, NULL);
  AddTestCase (SimpleMathTests, "Number Should not be Prime", "Prime2", IsNumberPrimeTestFalse, NULL, NULL, NULL);
  AddTestCase (SimpleMathTests, "Chef cook test order hotdog", "Hotdog", test_order_hotdog, NULL, NULL, NULL);
  AddTestCase (SimpleMathTests, "Chef cook test order burger not hotdog", "Burger", test_bad_dish, NULL, NULL, NULL);
  
  //
  // Populate the GlobalVarTests Unit Test Suite.
  //
  Status = CreateUnitTestSuite (&GlobalVarTests, Framework, "Global Variable Tests", "Sample.Globals", NULL, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed in CreateUnitTestSuite for GlobalVarTests\n"));
    Status = EFI_OUT_OF_RESOURCES;
    goto EXIT;
  }
  AddTestCase (GlobalVarTests, "You should be able to change a global BOOLEAN", "Boolean", GlobalBooleanShouldBeChangeable, NULL, NULL, NULL);
  AddTestCase (GlobalVarTests, "You should be able to change a global pointer", "Pointer", GlobalPointerShouldBeChangeable, MakeSureThatPointerIsNull, ClearThePointer, NULL);

  //
  // Execute the tests.
  //
  Status = RunAllTestSuites (Framework);

EXIT:
  if (Framework) {
    FreeUnitTestFramework (Framework);
  }

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
