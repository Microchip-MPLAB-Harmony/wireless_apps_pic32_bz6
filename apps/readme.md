
[![MCHP](https://www.microchip.com/ResourcePackages/Microchip/assets/dist/images/logo.png)](https://www.microchip.com)
# Examples applications

The following applications are provided to demonstrate the typical use case of BLE, 15.4, Thread and Multiprotocol with PIC32WM_BZ6204 devices.

## BLE

### BLE Building Blocks

* [Getting Started](ble\building_blocks\readme.md)
    * [Peripheral](ble\building_blocks\peripheral\readme.md)
		* [BLE Deep Sleep Adv](ble\building_blocks\peripheral\deep_sleep_adv\readme.md)
        * [Extended Advertisement](ble\building_blocks\peripheral\ext_adv\readme.md)		
        * [Legacy Advertisement](ble\building_blocks\peripheral\legacy_adv\readme.md)
		* [BLE Peripheral PATH Loss Advertisement](ble\building_blocks\peripheral\legacy_adv_pathloss\readme.md)
        * [Connection](ble\building_blocks\peripheral\peripheral_conn\readme.md)
        * [Transparent UART](ble\building_blocks\peripheral\profiles_services\peripheral_trp_uart\readme.md)
		* [BLE Custom Service](ble\building_blocks\peripheral\profiles_services\custom_service\readme.md)
		* [BLE Peripheral BLE Multiple Advertising Sets](ble\building_blocks\peripheral\two_set_adv\readme.md)
		* [BLE peripheral Code PHY transparent UART](ble\building_blocks\peripheral\profiles_services\peripheral_trp_uart_codedPhy\readme.md)		
    * [Central](ble\building_blocks\central\readme.md)
        * [Connection](ble\building_blocks\central\central_conn\readme.md)
        * [Legacy Scan](ble\building_blocks\central\legacy_scan\readme.md)
        * [Transparent UART](ble\building_blocks\central\profiles_services\central_trp_uart\readme.md)
        * [Transparent UART Multilink](ble/building_blocks/central/profiles_services/multilink/readme.md)
        * [Scan Extended Advertisement](ble\building_blocks\central\scan_ext_adv\readme.md)
        * [BLE Central Code PHY transparent UART](ble\building_blocks\central\profiles_services\central_trp_uart_codedPhy\readme.md)
    * [Multirole](ble\building_blocks\multirole\readme.md)
	* [HCI Application](ble\building_blocks\hci_application\ble_HCI_interface_app\readme.md)

### BLE Advanced Applications
* [BLE Peripheral ANCS](ble/advanced_applications/ble_ancs_app/readme.md)
* [BLE Peripheral HOGPS or HID Profile](ble/advanced_applications/ble_hogps_app/readme.md)
* [BLE Sensor App](ble/advanced_applications/ble_sensor/readme.md)
* [BLE Throughput](ble/advanced_applications/ble_throughput/readme.md)
* [BLE Peripheral ANPC](ble/advanced_applications/ble_anpc_app/readme.md)
* [BLE Peripheral ANPS](ble/advanced_applications/ble_anps_app/readme.md)
* [BLE Proximity Monitor PXPM](ble/advanced_applications/ble_pxpm_app/readme.md)
* [BLE Proximity Reporter PXPR](ble/advanced_applications/ble_pxpr_app/readme.md)

### BLE Peripheral Applications
* [BLE CAN Peripheral TRP UART](ble/peripheral_applications/can_peripheral_trp_uart/readme.md)
* [BLE LCC Central TRP UART](ble/peripheral_applications/lcc_central_trp_uart/readme.md)
* [BLE QEI Peripheral TRP UART](ble/peripheral_applications/qei_peripheral_trp_uart/readme.md)
* [BLE TCPIP TCP Server TRP UART](ble/peripheral_applications/tcpip_tcp_server_trp_uart/readme.md)
* [BLE Touch Peripheral TRP UART](ble/peripheral_applications/touch_peripheral_trp_uart/readme.md)
* [BLE USB CDC Peripheral TRP UART](ble/peripheral_applications/usb_cdc_peripheral_trp_uart/readme.md)
* [BLE Parallel8080 Peripheral TRP UART](ble/peripheral_applications/parallel8080_peripheral_trp_uart/readme.md)

## ieee_802_15_4 Applications
* [ieee_802_15_4](ieee_802_15_4/readme.md)
	* [IEEE_802_15_4 demoapp_ffd](ieee_802_15_4/MAC/push_pull_mac_app/demoapp_ffd/readme.md)
	* [IEEE_802_15_4 demoapp_rfd](ieee_802_15_4/MAC/push_pull_mac_app/demoapp_rfd/readme.md)
	* [IEEE_802_15_4 p2p_phy](ieee_802_15_4/PHY/p2p_phy/readme.md)
	
## Thread Applications
* [Building Blocks](thread/building_blocks/readme.md)
* [Advanced Applications](thread/advanced_applications/readme.md)

## Multiprotocol Applications
* [BLE + IEEE 802.15.4 MAC Provisioning FFD](multiprotocol/ble_mac_prov_ffd/readme.md)
* [BLE + IEEE 802.15.4 MAC Provisioning RFD](multiprotocol/ble_mac_prov_rfd/readme.md)
* [BLE + Thread Provisioning FTD Light](multiprotocol/ble_thread_prov_ftd_light/readme.md)
* [BLE + Thread Provisioning MTD Light](multiprotocol/ble_thread_prov_mtd_light/readme.md)
* [BLE + Thread Provisioning SED Sensor](multiprotocol/ble_thread_prov_sed_sensor/readme.md)
