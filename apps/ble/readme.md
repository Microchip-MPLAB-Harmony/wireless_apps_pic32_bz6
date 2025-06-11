# BLE Applications

Documentation describing steps to test and develop the precompiled BLE application examples

The BLE Application examples are divided into 3 categories There are both simple precompiled examples that can be tested by observing blinking LEDs on the board and more complex examples that can communicate with your smartphone using *Bluetooth* low energy. These precompiled examples do not require you to set up the full toolchain. Therefore, programming and testing them should not take more than a few minutes.

1.  Building Blocks - Building Blocks are training modules that teach users how to create a basic BLE application from scratch. Each block demonstrates a basic BLE function, such as "How to Enable Legacy Adv," "How to Enable Long Range Advertising," and so on. This is recommended for users who are new to the Microchip BLE stack and other tool chains like MPLAB Code Configurator.

2.  Advanced Applications - Advanced Applications are for users who are familiar with the Microchip BLE stack running on the PIC32WM_BZ6204 family of devices and also with the MPLAB Code Conigurator tool chain. Advanced applications demonstrate features like OTA and serial firmware upgrade, monitoring and control of sensor and light functionality, and automatic advertisement when a connection is lost.

3.  Peripheral Applications - Periperal Applications uses the Microchip BLE stack running on the PIC32WM_BZ6204 family of devices with seamlessly integrate Controller Area Network Flexible Data-Rate (CAN-FD) and Bluetooth Low Energy (BLE) technologies. This section shows how to set up a peripheral device that can efficiently communicate CAN-FD messages between a CAN bus and a smartphone using the Microchip Bluetooth Data (MBD) app.


-   **[Building Block Examples](building_blocks/readme.md)**  

-   **[Advanced Applications](advanced_applications/readme.md)**  

-   **[Peripheral Applications](peripheral_applications/readme.md)**  