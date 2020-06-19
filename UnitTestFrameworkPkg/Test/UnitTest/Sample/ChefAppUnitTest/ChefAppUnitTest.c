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

#define UNIT_TEST_NAME     "Sample Waiter Process Unit Test"
#define UNIT_TEST_VERSION  "0.1"

// Set a limit on the size of the chef dish string
#define MAX_DISH_SIZE 16


/**
*A chef object would return:
*0 - cooking dish went fine
* -1 - unknown dish
* -2 - ran out of ingredients for the dish
* any other error code -- unexpected error while cooking
*
* The return codes should be consistent between the real and mocked objects.
*/

INTN __wrap_ChefCook(
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
		//DEBUG((DEBUG_INFO, ">>> Chef test >> Need to get memory for string \n"));
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
INTN ChefCook(
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
STATIC INTN WaiterProcess(
	IN CONST CHAR16 *order, 
	OUT CHAR16 **dish)
{
	INTN rv;

	rv = __wrap_ChefCook(order, dish);
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
TestOrderHotdog(
	IN UNIT_TEST_CONTEXT  Context
)
{
	INTN rv;
	CHAR16 *dish = NULL;

	//dish = mChefDish;
	//ZeroMem(&dish, sizeof(dish));
	//(void)state; /* unused */

				 /* We expect the chef to receive an order for a hotdog */
	expect_string(__wrap_ChefCook, order, L"hotdog");
	/* And we tell the test chef that ke knows how to cook a hotdog
	* and has the ingredients
	*/
	will_return(__wrap_ChefCook, TRUE);
	will_return(__wrap_ChefCook, TRUE);
	/* The result will be a hotdog and the cooking process will succeed */
	will_return(__wrap_ChefCook, cast_ptr_to_largest_integral_type(L"hotdog"));
	will_return(__wrap_ChefCook, 0);

	/* Test the waiter */
	rv = WaiterProcess(L"hotdog", &dish);

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
TestBadDish(
	IN UNIT_TEST_CONTEXT  Context
	)
{
	INTN rv;
	CHAR16 *dish = NULL;


//	dish = mChefDish;
//	ZeroMem(&dish, sizeof(dish));
    //	(void)state; /* unused */

				 /* We expect the chef to receive an order for a hotdog */
	expect_string(__wrap_ChefCook, order, L"hotdog");
	/* And we tell the test chef that ke knows how to cook a hotdog
	* and has the ingredients
	*/
	will_return(__wrap_ChefCook, TRUE);
	will_return(__wrap_ChefCook, TRUE);
	/* The result will be a burger and the cooking process will succeed.
	* We expect the waiter to handle the bad dish and return an error
	* code
	*/
	will_return(__wrap_ChefCook, cast_ptr_to_largest_integral_type(L"burger"));
	will_return(__wrap_ChefCook, 0);

	/* Test the waiter */
	rv = WaiterProcess(L"hotdog", &dish);

	/* According to the documentation the waiter should return -2 now */
	UT_ASSERT_EQUAL(rv, -2);
	/* And do not give the bad dish to the customer */
	UT_ASSERT_EQUAL((UINTN)dish, (UINTN)NULL);
	//assert_null(dish);
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
  UNIT_TEST_SUITE_HANDLE      ChefAppTests;
  
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
  Status = CreateUnitTestSuite (&ChefAppTests, Framework, "Waiter calls ChefCook App Tests", "Waiter.cook", NULL, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed in CreateUnitTestSuite for ChefAppTests\n"));
    Status = EFI_OUT_OF_RESOURCES;
    goto EXIT;
  }

  AddTestCase (ChefAppTests, "Chef cook test order hotdog", "Hotdog0", TestOrderHotdog, NULL, NULL, NULL);
  AddTestCase (ChefAppTests, "Chef cook test order burger not hotdog", "Burger0", TestBadDish, NULL, NULL, NULL);
  
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
