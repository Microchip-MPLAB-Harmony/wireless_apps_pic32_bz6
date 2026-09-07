![Microchip logo](https://raw.githubusercontent.com/wiki/Microchip-MPLAB-Harmony/Microchip-MPLAB-Harmony.github.io/images/microchip_logo.png)
![Harmony logo small](https://raw.githubusercontent.com/wiki/Microchip-MPLAB-Harmony/Microchip-MPLAB-Harmony.github.io/images/microchip_mplab_harmony_logo_small.png)

# Microchip MPLAB® Harmony 3 Release Notes

## Harmony 3 Wireless Multiprotocol application examples for PIC32WM_BZ6204 family  **v1.1.1**

### Development kit and demo application support

Following table provides number of Wireless examples available for different development kits.

| Development Kits           | MPLABx applications |
|:--------------------------:|:-------------------:|
| PIC32WM_BZ6204 Curiosity Board 	 |       18             |

## New Features
### Multiprotocol applications

- BLE + IEEE 802.15.4 MAC Provisioning FFD (`ble_mac_prov_ffd`)
- BLE + IEEE 802.15.4 MAC Provisioning RFD (`ble_mac_prov_rfd`)
- BLE + Thread Provisioning FTD Light (`ble_thread_prov_ftd_light`)
- BLE + Thread Provisioning MTD Light (`ble_thread_prov_mtd_light`)
- BLE + Thread Provisioning SED Sensor (`ble_thread_prov_sed_sensor`)

### Known Issues
- Ongoing improvements for stress and extended validation tests in multi-node environments.
- In ble_mac_prov_rfd applications, the device IEEE address may get corrupted
  while storing to persistent variable before deep sleep, which can cause communication failures between the RFD
  and FFD. A fix has been applied directly in the driver source file (`mac.c`) inside the
  `MAC_ReadyToDeepSleep()` function. If the project is regenerated using MCC, this
  fix will be lost and must be re-applied manually.
  - **Fix:** In `mac.c`, within the `MAC_ReadyToDeepSleep()` function, replace the
    calls to `memcpy4ByteAligned()` with the standard library `memcpy()` (from `<string.h>`).

### Development Tools

- [MPLAB® X IDE v6.35]
- MPLAB® X IDE plug-ins:
  - MPLAB® Code Configurator (MCC) `v5.6.4`
- [MPLAB® XC32 C/C++ Compiler v5.10]
- PIC32CX-BZ6 Family - DFP 1.3.38

## Harmony 3 Wireless BLE, Thread, IEEE_802 15.4 MAC and PHY application examples for PIC32WM_BZ6204 family  **v1.1.0**

### Development kit and demo application support

Following table provides number of Wireless examples available for different development kits.

| Development Kits           | MPLABx applications |
|:--------------------------:|:-------------------:|
| PIC32WM_BZ6204 Curiosity Board 	 |       13    |

## New Features
### BLE Applications
- BLE Parallel8080 Peripheral Transparent UART

### IEEE_802_15_4
- IEEE_802_15_4 MAC demoapp_ffd
- IEEE_802_15_4 MAC demoapp_rfd
- IEEE_802_15_4 p2p_phy

### Thread
- Thread cli_ftd
- Thread cli_mtd
- Thread demo_app_udp_ftd
- Thread demo_app_udp_mtd
- Thread rcp_uart
- Thread CoAP coap_ftd_client
- Thread CoAP coap_ftd_server
- Thread CoAP coap_sed_client
- Thread CoAP coap_sed_server

### Bug fixes and Improvements
- New applications release.

### Known Issues
- Ongoing improvements for stress and extended validation tests in multi-node environments. 

### Development Tools

- [MPLAB® X IDE v6.25]
- MPLAB® X IDE plug-ins:
  - MPLAB® Code Configurator (MCC) `v5.6.2`
- [MPLAB® XC32 C/C++ Compiler v4.60]
- PIC32CX-BZ6 Family - DFP 1.3.38

## Harmony 3 Wireless BLE application examples for PIC32WM_BZ6204 family  **v1.0.0**

### Development kit and demo application support

Following table provides number of Wireless examples available for different development kits.

| Development Kits           | MPLABx applications |
|:--------------------------:|:-------------------:|
| PIC32WM_BZ6204 Curiosity Board 	 |       31             |

## New Features
### BLE Applications
- BLE peripheral Legacy Adv 
- BLE peripheral Ext Adv 
- BLE Peripheral Connection 
- BLE Deep Sleep Adv
- BLE Custom Service 
- BLE peripheral transparent UART
- BLE Central Legacy Scan
- BLE Central Scan Ext Adv
- BLE Central Connection
- BLE Central transparent UART
- BLE Central Multilink 
- BLE Sensor
- BLE Throughput
- BLE Multirole
- BLE Central Code PHY transparent UART
- BLE Peripheral ANCS
- BLE Peripheral HOGPS or HID Profile
- BLE Peripheral PATH Loss Advertisement
- BLE peripheral Code PHY transparent UART
- BLE Peripheral BLE Multiple Advertising Sets
- BLE ANPS
- BLE ANPC
- BLE PXPM
- BLE PXPR
- BLE HCI
- BLE Peripheral CAN transparent UART
- BLE Central LCC transparent UART
- BLE Peripheral QEI transparent UART
- BLE Server TCPIP TCP transprant UART
- BLE Peripheral Touch transparent UART
- BLE Peripheral USB CDC transparent UART

### Bug fixes and Improvements
- New applications release.


### Known Issues
- None 

### Development Tools

- [MPLAB® X IDE v6.25]
- MPLAB® X IDE plug-ins:
  - MPLAB® Code Configurator (MCC) `v5.5.1`
- [MPLAB® XC32 C/C++ Compiler v4.60]
- PIC32CX-BZ6 Family - DFP 1.2.17