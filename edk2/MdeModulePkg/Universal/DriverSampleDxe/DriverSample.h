/** @file

Copyright (c) 2007, Intel Corporation
All rights reserved. This program and the accompanying materials
are licensed and made available under the terms and conditions of the BSD License
which accompanies this distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

Module Name:

  DriverSample.h

Abstract:


Revision History


**/

#ifndef _DRIVER_SAMPLE_H_
#define _DRIVER_SAMPLE_H_

#include <Uefi.h>

#include <Protocol/HiiConfigRouting.h>
#include <Protocol/FormBrowser2.h>
#include <Protocol/HiiConfigAccess.h>
#include <Protocol/HiiDatabase.h>
#include <Protocol/HiiString.h>

#include <Guid/MdeModuleHii.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/HiiLib.h>
#include <Library/DevicePathLib.h>

#include "NVDataStruc.h"

//
// This is the generated <AltResp> for defaults defined in VFR
//
extern UINT8 VfrMyIfrNVDataDefault0000[];
extern UINT8 VfrMyIfrNVDataDefault0001[];
extern UINT8 VfrMyIfrNVDataBlockName[];

//
// This is the generated IFR binary data for each formset defined in VFR.
// This data array is ready to be used as input of HiiAddPackages() to
// create a packagelist (which contains Form packages, String packages, etc).
//
extern UINT8  VfrBin[];
extern UINT8  InventoryBin[];

//
// This is the generated String package data for all .UNI files.
// This data array is ready to be used as input of HiiAddPackages() to
// create a packagelist (which contains Form packages, String packages, etc).
//
extern UINT8  DriverSampleStrings[];

#define DYNAMIC_ONE_OF_VAR_OFFSET        OFFSET_OF (DRIVER_SAMPLE_CONFIGURATION, DynamicOneof)
#define DYNAMIC_ORDERED_LIST_VAR_OFFSET  OFFSET_OF (DRIVER_SAMPLE_CONFIGURATION, DynamicOrderedList)

#define DRIVER_SAMPLE_PRIVATE_SIGNATURE SIGNATURE_32 ('D', 'S', 'p', 's')

typedef struct {
  UINTN                            Signature;

  EFI_HANDLE                       DriverHandle[2];
  EFI_HII_HANDLE                   HiiHandle[2];
  DRIVER_SAMPLE_CONFIGURATION      Configuration;
  UINT8                            PasswordState;

  //
  // Consumed protocol
  //
  EFI_HII_DATABASE_PROTOCOL        *HiiDatabase;
  EFI_HII_STRING_PROTOCOL          *HiiString;
  EFI_HII_CONFIG_ROUTING_PROTOCOL  *HiiConfigRouting;
  EFI_FORM_BROWSER2_PROTOCOL       *FormBrowser2;

  //
  // Produced protocol
  //
  EFI_HII_CONFIG_ACCESS_PROTOCOL   ConfigAccess;
} DRIVER_SAMPLE_PRIVATE_DATA;

#define DRIVER_SAMPLE_PRIVATE_FROM_THIS(a)  CR (a, DRIVER_SAMPLE_PRIVATE_DATA, ConfigAccess, DRIVER_SAMPLE_PRIVATE_SIGNATURE)

#pragma pack(1)

///
/// HII specific Vendor Device Path definition.
///
typedef struct {
  VENDOR_DEVICE_PATH             VendorDevicePath;
  EFI_DEVICE_PATH_PROTOCOL       End;
} HII_VENDOR_DEVICE_PATH;

#pragma pack()

#endif
