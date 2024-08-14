![code-formatting](https://github.com/DaniusKalv/dk_c_lib/actions/workflows/main.yml/badge.svg)

# dk_c_lib

Embedded C library that is common to most of my embedded projects.

## Prologue

Writing/testing/debugging code takes time, therefore rewriting the same code is a waste of effort and should be avoided. In my opinion only the code that is defining application level logic should reside in the project files. Everything else that is applicatioon non-specific should be turned into neat reusable code components and packaged into this library that can be shared across a lot of projects. So that the next time when you need that I²C or SPI driver you don't have to go through microcontroller user manual or google and write/copy/paste code, just grab a component (that has been already tested and verified) and concentrate on that application level logic that really makes your project what it is. Everything else is just tools to get to that point.

Reusability in embedded is not always easy especially when it comes to dealing with different microcontroller manufacturers. So far in this library I've only included components that are designed for Nordic microcontroller family. Makes it easier to start out on this reusability quest of mine. Although I have a lot of experience with a variety of microcontrollers from various manufacturers like Espressif, Microchip, NXP, SiliconLabs to name a few. Once you understand how through registers we are interacting with peripherals at digital logic level it becomes way easier to adapt to new microcontroller families quickly. So when it will come to adding support for more families I'll have some tricks that I learned during many years in the industry, some I came up with by myself. But so far my projects mainly revolve around Bluetooth low energy and in my opinion Nordic is the best microcontroller manufacturer in this space. The quality of their hardware, software and community is unlike any other I've seen so far. Learned a lot from them by exploring their low and high level embedded code.

So, to sum it all up this library is for myself to enforce the reusability mindset. Hopefuly one day when I start a new project I'll have so many components in this library that all will be left to do will be application level logic.

## Library features
### BLE
| Component          | Description                                                                        |
|--------------------|------------------------------------------------------------------------------------|
| dk_ble_advertising | Customized BLE advertising functions                                               |
| dk_ble_gap         | BLE GAP initialization functions according to DK standards                         |

| Service           | Description                                            |
|-------------------|--------------------------------------------------------|
| dk_ble_acc        | BLE service for interacting with accelerometer         |
| dk_ble_dis        | Device information service                             |
| dk_ble_gyro       | BLE service for interacting with gyro                  |
| dk_ble_mag        | BLE service for interacting with magnetometer          |
| dk_ble_mr_pickle  | BLE service for changing mr pickle mode                |
| dk_ble_phil_it_up | Phil It Up service, supports multiple peripheral links |

### Peripheral drivers
| Driver           | Description                                                              |
|------------------|--------------------------------------------------------------------------|
| dk_flash_storage | DK flash storage access functions                                        |
| dk_lpcomp        | Low power comparator driver                                              |
| dk_twi           | Helper functions for working with TWI peripheral with ERRATA workarounds |

### External chip drivers
| IC            | Description                                                                        |
|---------------|------------------------------------------------------------------------------------|
| is31fl3206    | LED Driver IC 12 Output Linear PWM Dimming I²C                                     |
| lp5024        | LED Driver IC 24 Output Linear Dimming I²C                                         |
| lsm9ds1       | Accelerometer, Gyroscope, Magnetometer, Temperature, 9 Axis Sensor I²C, SPI Output |
| mlx90615      | Temperature Sensor Digital, Infrared (IR) -40°C ~ 85°C                             |
| sh1106        | Non-Touch Graphic LCD Display Module White OLED SPI 1.3"                           |
| tca9548a      | Automotive 8-Channel I2C Switch with Reset                                         |
| tlv320aic3106 | Low-Power Stereo Audio CODEC for Portable Audio/Telephony                          |
| tlv320dac32   | Low-Power Stereo Audio DAC for Portable Audio/Telephony                            |

### Modules
| Module         | Description                                                       |
|----------------|-------------------------------------------------------------------|
| dk_battery_lvl | Battery level measurement module                                  |
| dk_twi_mngr    | TWI manager that implements a queue buffer on top of nrf_twi_mngr |

### Toolchain
I heavily modified the Makefile provided by Nordic to include a lot of aditional commands.

| Command | Description |
|---|---|
| $(FULL_PROJECT_NAME)_debug | Build debug binary |
| $(FULL_PROJECT_NAME)_release | Build release binary |
| flash_debug  | Flash debug binary |
| flash_release  | Flash release binary |
| flash_softdevice  | Flash softdevice |
| reset  | Reset chip |
| generate_boot_settings_debug  | Generate bootloader settings using debug app hex |
| generate_boot_settings_release  | Generate bootloader settings using release app hex |
| flash_boot_settings_debug  | Flash bootloader settings page debug |
| flash_boot_settings_release  | Flash bootloader settings page release |
| flash_bootloader  | Flash bootloader |
| flash_uicr  | Write hw_id and hw_v to UICR |
| erase_uicr  | Clear uicr memory |
| generate_hex  | Generate a release hex file containing everything \(softdevice, app, bootloader\) |
| flash_hex  | Flash a release hex file containing everything \(softdevice, app, bootloader\) |
| generate_dfu_package  | Generate release dfu zip file containing everything \(softdevice, app, bootloader\) |
| generate_dfu_app_package | Generate release dfu app zip file containing only app |
| erase  | Erase chip |
| sdk_config | Start external tool for editing sdk_config.h |
| dk_config | Start external tool for editing dk_config.h |
