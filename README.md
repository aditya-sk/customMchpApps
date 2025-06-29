# Out of tree SAME54 applications using Zephyr

## [Overview](#overview)
This repository contains PWM with LED, I2C with BNO055 sensor, SMF, and Multithread applications are developed on Microchip SAME54-XPRO developement board.

    ├── zephyrWrkSpace/
    │   ├── .west/
    │   ├── bootloader
    │   ├── customMchpApps/
    │   │   ├── pwmMachine/
    │   │   │── sensorMachineApp/
    │   │   └── multiThreadApp/
    │   ├── modules/
    │   ├── tools/
    │   ├── zephyr/
    │   └── zephyr-sdk-0.17.0/
  

## [Features](#features)
1. pwmMachine (PWM + SMF)
2. sensorMachineApp (NDOF + PWM + SMF)
3. multiThreadApp (PWM + NDOF + Threads)
