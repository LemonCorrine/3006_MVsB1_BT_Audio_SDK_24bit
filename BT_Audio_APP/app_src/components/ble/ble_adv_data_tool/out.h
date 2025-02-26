
//BLE configuration generator for use with MV_BT_Stack
//Copyright 2019 MVSilicon
// out.h generated from example.gatt for MV_BT_Stack
// att db format version 1

// binary attribute representation:
// - size in bytes (16), flags(16), handle (16), uuid (16/128), value(...)

// 
#include <stdint.h>

const uint8_t profile_data[] =
{
    // 0x0001 PRIMARY_SERVICE-GAP_SERVICE
    0x0a, 0x00, 0x02, 0xf0, 0x01, 0x00, 0x00, 0x28, 0x00, 0x18, 
    // 0x0002 CHARACTERISTIC-GAP_DEVICE_NAME-READ | WRITE | DYNAMIC
    0x0d, 0x00, 0x02, 0xf0, 0x02, 0x00, 0x03, 0x28, 0x0a, 0x03, 0x00, 0x00, 0x2a, 
    // 0x0003 VALUE-GAP_DEVICE_NAME-READ | WRITE | DYNAMIC-'BP10-BLE'
    // READ_ANYBODY, WRITE_ANYBODY
    0x10, 0x00, 0x0a, 0xf1, 0x03, 0x00, 0x00, 0x2a, 0x42, 0x50, 0x31, 0x30, 0x2d, 0x42, 0x4c, 0x45, 

    // 0x0004 PRIMARY_SERVICE-AB00
    0x0a, 0x00, 0x02, 0xf0, 0x04, 0x00, 0x00, 0x28, 0x00, 0xab, 
    // 0x0005 CHARACTERISTIC-AB01-READ | WRITE | WRITE_WITHOUT_RESPONSE | DYNAMIC
    0x0d, 0x00, 0x02, 0xf0, 0x05, 0x00, 0x03, 0x28, 0x0e, 0x06, 0x00, 0x01, 0xab, 
    // 0x0006 VALUE-AB01-READ | WRITE | WRITE_WITHOUT_RESPONSE | DYNAMIC-''
    // READ_ANYBODY, WRITE_ANYBODY
    0x08, 0x00, 0x0e, 0xf1, 0x06, 0x00, 0x01, 0xab, 
    // 0x0007 CHARACTERISTIC-AB02-NOTIFY | DYNAMIC
    0x0d, 0x00, 0x02, 0xf0, 0x07, 0x00, 0x03, 0x28, 0x10, 0x08, 0x00, 0x02, 0xab, 
    // 0x0008 VALUE-AB02-NOTIFY | DYNAMIC-''
    // 
    0x08, 0x00, 0x00, 0xf1, 0x08, 0x00, 0x02, 0xab, 
    // 0x0009 CLIENT_CHARACTERISTIC_CONFIGURATION
    // READ_ANYBODY, WRITE_ANYBODY
    0x0a, 0x00, 0x0e, 0xf1, 0x09, 0x00, 0x02, 0x29, 0x00, 0x00, 
    // 0x000a CHARACTERISTIC-AB03-NOTIFY | DYNAMIC
    0x0d, 0x00, 0x02, 0xf0, 0x0a, 0x00, 0x03, 0x28, 0x10, 0x0b, 0x00, 0x03, 0xab, 
    // 0x000b VALUE-AB03-NOTIFY | DYNAMIC-''
    // 
    0x08, 0x00, 0x00, 0xf1, 0x0b, 0x00, 0x03, 0xab, 
    // 0x000c CLIENT_CHARACTERISTIC_CONFIGURATION
    // READ_ANYBODY, WRITE_ANYBODY
    0x0a, 0x00, 0x0e, 0xf1, 0x0c, 0x00, 0x02, 0x29, 0x00, 0x00, 

    // END
    0x00, 0x00, 
}; // total size 74 bytes 


//
// list service handle ranges
//
#define ATT_SERVICE_GAP_SERVICE_START_HANDLE 0x0001
#define ATT_SERVICE_GAP_SERVICE_END_HANDLE 0x0003
#define ATT_SERVICE_AB00_START_HANDLE 0x0004
#define ATT_SERVICE_AB00_END_HANDLE 0x000c

//
// list mapping between characteristics and handles
//
#define ATT_CHARACTERISTIC_GAP_DEVICE_NAME_01_VALUE_HANDLE 0x0003
#define ATT_CHARACTERISTIC_AB01_01_VALUE_HANDLE 0x0006
#define ATT_CHARACTERISTIC_AB02_01_VALUE_HANDLE 0x0008
#define ATT_CHARACTERISTIC_AB02_01_CLIENT_CONFIGURATION_HANDLE 0x0009
#define ATT_CHARACTERISTIC_AB03_01_VALUE_HANDLE 0x000b
#define ATT_CHARACTERISTIC_AB03_01_CLIENT_CONFIGURATION_HANDLE 0x000c
