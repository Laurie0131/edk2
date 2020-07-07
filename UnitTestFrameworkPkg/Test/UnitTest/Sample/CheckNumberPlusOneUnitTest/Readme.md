# Check Number Plus One Unit Test

## About

This is an example of a unit test for a simple function to test.  This can be used as a template for other test cases for both target and host unit testing of stand-alone functions.

## How to build Windows

### Prerequistes
* Windows 10
* Visual Studio VS2015, VS2017 or VS2019
* Python 3.7.x or Newer
* git 


### Download the Repo
Check out the example code from the following Repo

```shell
$ git clone -b LJ_UnitTest2 https://github.com/Laurie0131/edk2.git 
$ cd edk2 
$ git submodule update --init
```

Next Build the `BaseTools`

```shell
$ edksetup.bat Rebuild
```
Build the `UnitTestFrameworkPkg` Unit Test Host, Below builds using Visual Stuido 2015, 
```shell
$ build -b NOOPT -t VS2015x86 -a X64 -p UnitTestFrameworkPkg\Test\UnitTestFrameworkPkgHostTest.dsc 
```
For other Visual Studio versions:
* 2017 use `-t VS2017` 
* 2019 use `-t VS2019`

### Run the Host Unit test
Run the following Windows Batch file
```
$ RunHostTestCheckNumber.bat
```
Result from running Unit test
```
Sample Check Number Plus One Unit Test v0.1
---------------------------------------------------------
------------     RUNNING ALL TEST SUITES   --------------
---------------------------------------------------------
---------------------------------------------------------
RUNNING TEST SUITE: Simple Number Tests
---------------------------------------------------------
[==========] Running 3 test(s).
[ RUN      ] Test IsNumberPlus1 -  produce 1+1=2
[       OK ] Test IsNumberPlus1 -  produce 1+1=2
[ RUN      ] Test IsNumberNotPlus1 - Not produce number +1
[       OK ] Test IsNumberNotPlus1 - Not produce number +1
[ RUN      ] Test IsNumberCorrect w/ results in one test
UnitTest: Test-3 - Test IsNumberCorrect w/ results in one test
Log Output Start
[INFO]        Number is: 100 returned was (0)
[INFO]        Number is:   1 returned was (2)
Log Output End
[       OK ] Test IsNumberCorrect w/ results in one test
[==========] 3 test(s) run.
[  PASSED  ] 3 test(s).
```
## How to add a function to test and test case

1. Use the example .c file [`NumberPlusOne.c`](https://github.com/Laurie0131/edk2/blob/LJ_UnitTest2/UnitTestFrameworkPkg/Test/UnitTest/Sample/CheckNumberPlusOneUnitTest/NumberPlusOne.c) for where to place function to test


2. Use the example .c file [`CheckNumberPlusOneUnitTest.c`](https://github.com/Laurie0131/edk2/blob/LJ_UnitTest2/UnitTestFrameworkPkg/Test/UnitTest/Sample/CheckNumberPlusOneUnitTest/CheckNumberPlusOneUnitTest.c) to add test cases to test your functions
 * Write a test function that will call the function to test
    * Within the test function use the `UT_ASSERT*()` Unit Test Library calls to determine if each test returns or produces the desired outcome.
    * Within the test function if all values returned and prduced are correct, return the value of `UNIT_TEST_PASSED`  
    * Example:
```
UNIT_TEST_STATUS
EFIAPI
MyTestCase1(
	IN UNIT_TEST_CONTEXT  Context
)
{
	UINTN  RetValue;

	RetValue = MyFunction2Test(1);
	UT_ASSERT_EQUAL(RetValue, 2); // Expects RetValue == 2 otherwise the test will fail
	return UNIT_TEST_PASSED;
}
```    
  * Add the `AddTestCase` function call after the call to  `CreateUnitTestSuite`
```
AddTestCase (SuiteHandle, “Test Description", “short.name", MyTestCase1, NULL, NULL, NULL);

Where:
 SuiteHandle		UNIT_TEST_SUITE_HANDLE (same as Create for Suite) 
 “Test Description”	User friendly ASCII String for Test Case Description
 “Test.Name”		User friendly ASCII String for Test Case Name (no spaces)
 MyTestCase1	    Unit Test Function
 NULL				Optional Prerequisite Function runs before test function
 NULL				Optional Clean-up Function runs after test function
 NULL				UNIT_TEST_CONTEXT - Optional Pointer to Context 
```
3. Use the [`CheckNumberPlusOneUnitTest.h`](https://github.com/Laurie0131/edk2/blob/LJ_UnitTest2/UnitTestFrameworkPkg/Test/UnitTest/Sample/CheckNumberPlusOneUnitTest/CheckNumberPlusOneUnitTest.h) file to add your prototype for your function to test and any other `#include` statements required for your function.








