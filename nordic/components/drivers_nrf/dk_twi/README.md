# nRF52832 TWI power consumption

In order to achieve low power consumption on nRF52832 when using TWI peripheral, it has to be disabled between TWI transactions (for some reason it is constantly consuming ~270uA even when not used).
The way to do this if everything worked as it is supposed to, would be to use nrfx_twi_enable and nrfx_twi_disable functions. Unfortunately, this approach only works when the GPIOTE peripheral is not used. And that is rarely the case with most of applications.
Apparently, there is an [anomaly](http://infocenter.nordicsemi.com/index.jsp?topic=%2Fcom.nordic.infocenter.nrf52832.Rev1.errata%2Fanomaly_832_89.html&cp=2_2_1_0_1_26) in the nRF52832. Basically, it states that the power consumption is way higher than expected when a combination of TWI or SPI and GPIOTE peripherals is enabled.
The workaround for it is to toggle the power register of TWI peripheral after disabling it.
For TWI0/SPI0:

```C

*(volatile uint32_t *)0x40003FFC = 0;
*(volatile uint32_t *)0x40003FFC;
*(volatile uint32_t *)0x40003FFC = 1;
```

For TWI1/SPI1:

```C
*(volatile uint32_t *)0x40004FFC = 0;
*(volatile uint32_t *)0x40004FFC;
*(volatile uint32_t *)0x40004FFC = 1;
```

For SPI2:

```C
*(volatile uint32_t *)0x40023FFC = 0;
*(volatile uint32_t *)0x40023FFC;
*(volatile uint32_t *)0x40023FFC = 1;
```

A consequence of toggling the power register is that the registers of the peripheral get reset, so the safest approach is to do nrfx_twi_uninit in order to reset the application variables to an uninitialized state. This also means that the next time before enabling the peripheral it has to be reinitialized again.
dk_twi_enable() and dk_twi_disable() functions implement the ERRATA 89 workaround and allow enabling and disabling the peripheral easily.
