![Microchip logo](https://raw.githubusercontent.com/wiki/Microchip-MPLAB-Harmony/Microchip-MPLAB-Harmony.github.io/images/microchip_logo.png)
![Harmony logo small](https://raw.githubusercontent.com/wiki/Microchip-MPLAB-Harmony/Microchip-MPLAB-Harmony.github.io/images/microchip_mplab_harmony_logo_small.png)

# MPLAB® Harmony 3 Wireless BLE Application Examples

MPLAB® Harmony 3 is an extension of the MPLAB® ecosystem for creating embedded firmware 
solutions for Microchip 32-bit SAM and PIC® microcontroller and microprocessor devices.  
Refer to the following links for more information.

- [Microchip 32-bit MCUs](https://www.microchip.com/design-centers/32-bit)
- [Microchip 32-bit MPUs](https://www.microchip.com/design-centers/32-bit-mpus)
- [Microchip MPLAB X IDE](https://www.microchip.com/mplab/mplab-x-ide)
- [Microchip MPLAB® Harmony](https://www.microchip.com/mplab/mplab-harmony)
- [Microchip MPLAB® Harmony Pages](https://microchip-mplab-harmony.github.io/)

This repository contains the MPLAB® Harmony 3 Wireless BLE Application Examples solutions and example applications. 
Quickly incorporate connectivity to your designs with Wireless BLE Application Examples ICs, modules, software and 
development kits that make connecting effortless for your customers. 
Our comprehensive Wireless BLE Application Examples portfolio has the technology to meet your range, data rate, interoperability, frequency and 
topology needs. Refer to the following links for release notes, training materials, and interface reference information.

- [Release Notes](./release_notes.md)
- [MPLAB® Harmony License](mplab_harmony_license.md)

# Contents Summary

| Folder     | Description                                               |
| ---        | ---                                                       |
| apps       | [Example applications for PIC32WM_BZ6204 devices](./apps/readme.md)         |
| docs       | [Wireless Application Example help documentation](https://onlinedocs.microchip.com/oxy/GUID-657D3893-6C33-47F8-978B-86DB297AC33D-en-US-1/index.html)

## Code Examples

The following applications are provided to demonstrate the typical use case of one or more Peripheral libraries.

| BLE Examples | Families |
| --- | :---: |
| [BLE peripheral Legacy Adv](apps/ble/building_blocks/peripheral/legacy_adv/readme.md) | PIC32WM_BZ6204 |
| [BLE HCI](apps/ble/building_blocks/hci_application/readme.md) | PIC32WM_BZ6204 |
| [BLE peripheral Ext Adv](apps/ble/building_blocks/peripheral/ext_adv/readme.md) | PIC32WM_BZ6204 |
| [BLE Peripheral Connection](apps/ble/building_blocks/peripheral/peripheral_conn/readme.md) | PIC32WM_BZ6204 |
| [BLE Deep Sleep Adv](apps/ble/building_blocks/peripheral/deep_sleep_adv/readme.md) | PIC32WM_BZ6204 |
| [BLE Custom Service](apps/ble/building_blocks/peripheral/profiles_services/custom_service/readme.md) | PIC32WM_BZ6204 |
| [BLE peripheral transparent UART](apps/ble/building_blocks/peripheral/profiles_services/peripheral_trp_uart/readme.md) | PIC32WM_BZ6204 |
| [BLE Central Legacy Scan](apps/ble/building_blocks/central/legacy_scan/readme.md) | PIC32WM_BZ6204 |
| [BLE Central Scan Ext Adv](apps/ble/building_blocks/central/scan_ext_adv/readme.md) | PIC32WM_BZ6204 |
| [BLE Central Connection](apps/ble/building_blocks/central/central_conn/readme.md) | PIC32WM_BZ6204 |
| [BLE Central transparent UART](apps/ble/building_blocks/central/profiles_services/central_trp_uart/readme.md) | PIC32WM_BZ6204 |
| [BLE Central Multilink](apps/ble/building_blocks/central/profiles_services/multilink/readme.md) | PIC32WM_BZ6204 |
| [BLE Multirole](apps/ble/building_blocks/multirole/readme.md) | PIC32WM_BZ6204 |
| [BLE Central Code PHY transparent UART](apps/ble/building_blocks/central/profiles_services/central_trp_uart_codedPhy/readme.md) | PIC32WM_BZ6204 |
| [BLE Peripheral PATH Loss Advertisement](apps/ble/building_blocks/peripheral/legacy_adv_pathloss/readme.md) | PIC32WM_BZ6204 |
| [BLE peripheral Code PHY transparent UART](apps/ble/building_blocks/peripheral/profiles_services/peripheral_trp_uart_codedPhy/readme.md) | PIC32WM_BZ6204 |
| [BLE Peripheral BLE Multiple Advertising Sets](apps/ble/building_blocks/peripheral/two_set_adv/readme.md) | PIC32WM_BZ6204 |
| [BLE Sensor](apps/ble/advanced_applications/ble_sensor/readme.md) | PIC32WM_BZ6204 |
| [BLE Throughput](apps/ble/advanced_applications/ble_throughput/readme.md) | PIC32WM_BZ6204 |
| [BLE Peripheral ANCS](apps/ble/advanced_applications/ble_ancs_app/readme.md) | PIC32WM_BZ6204 |
| [BLE Peripheral HOGPS or HID Profile](apps/ble/advanced_applications/ble_hogps_app/readme.md) | PIC32WM_BZ6204 |
| [BLE Peripheral ANPC](apps/ble/advanced_applications/ble_anpc_app/readme.md) | PIC32WM_BZ6204 |
| [BLE Peripheral ANPS](apps/ble/advanced_applications/ble_anps_app/readme.md) | PIC32WM_BZ6204 |
| [BLE Proximity Monitor PXPM](apps/ble/advanced_applications/ble_pxpm_app/readme.md) | PIC32WM_BZ6204 |
| [BLE Proximity Reporter PXPR](apps/ble/advanced_applications/ble_pxpr_app/readme.md) | PIC32WM_BZ6204 |
| [BLE Peripheral CAN TRP UART](apps/ble/peripheral_applications/can_peripheral_trp_uart/readme.md) | PIC32WM_BZ6204 |
| [BLE Central LCC TRP UART](apps/ble/peripheral_applications/lcc_central_trp_uart/readme.md) | PIC32WM_BZ6204 |
| [BLE Peripheral QEI TRP UART](apps/ble/peripheral_applications/qei_peripheral_trp_uart/readme.md) | PIC32WM_BZ6204 |
| [BLE TCPIP TCP SERVER TRP UART](apps/ble/peripheral_applications/tcpip_tcp_server_trp_uart/readme.md) | PIC32WM_BZ6204 |
| [BLE Peripheral Touch TRP UART](apps/ble/peripheral_applications/touch_peripheral_trp_uart/readme.md) | PIC32WM_BZ6204 |
| [BLE Peripheral USB CDC TRP UART](apps/ble/peripheral_applications/usb_cdc_peripheral_trp_uart/readme.md) | PIC32WM_BZ6204 |
____


[![License](https://img.shields.io/badge/license-Harmony%20license-orange.svg)](https://github.com/Microchip-MPLAB-Harmony/wireless_apps_pic32cxbz3_wbz35/blob/master/mplab_harmony_license.md)
[![Latest release](https://img.shields.io/github/release/Microchip-MPLAB-Harmony/wireless_apps_pic32cxbz3_wbz35.svg)](https://github.com/Microchip-MPLAB-Harmony/wireless_apps_pic32cxbz3_wbz35/releases/latest)
[![Latest release date](https://img.shields.io/github/release-date/Microchip-MPLAB-Harmony/wireless_apps_pic32cxbz3_wbz35.svg)](https://github.com/Microchip-MPLAB-Harmony/wireless_apps_pic32cxbz3_wbz35/releases/latest)
[![Commit activity](https://img.shields.io/github/commit-activity/y/Microchip-MPLAB-Harmony/wireless_apps_pic32cxbz3_wbz35.svg)](https://github.com/Microchip-MPLAB-Harmony/wireless_apps_pic32cxbz3_wbz35/graphs/commit-activity)
[![Contributors](https://img.shields.io/github/contributors-anon/Microchip-MPLAB-Harmony/wireless_apps_pic32cxbz3_wbz35.svg)]()

____

[![Developer Help](https://img.shields.io/badge/Youtube-Developer%20Help-red.svg)](https://www.youtube.com/MicrochipDeveloperHelp)
[![Developer Help](https://img.shields.io/badge/XWiki-Developer%20Help-torquiose.svg)](https://developerhelp.microchip.com/xwiki/bin/view/software-tools/harmony/)
[![Follow us on Youtube](https://img.shields.io/badge/Youtube-Follow%20us%20on%20Youtube-red.svg)](https://www.youtube.com/user/MicrochipTechnology)
[![Follow us on LinkedIn](https://img.shields.io/badge/LinkedIn-Follow%20us%20on%20LinkedIn-blue.svg)](https://www.linkedin.com/company/microchip-technology)
[![Follow us on Facebook](https://img.shields.io/badge/Facebook-Follow%20us%20on%20Facebook-blue.svg)](https://www.facebook.com/microchiptechnology/)
[![Follow us on Twitter](https://img.shields.io/twitter/follow/MicrochipTech.svg?style=social)](https://twitter.com/MicrochipTech)


