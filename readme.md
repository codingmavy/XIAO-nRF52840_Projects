# BLE Analyzer (Zephyr, nRF52840)

A BLE advertisement scanner running entirely on hardware and firmware. It scans nearby devices and prints every captured packet over a USB serial (COM) port. No host software is needed; any serial terminal can read the output.

**Status:** in development.

**Responsible use:** passive scanning only, on your own devices or with permission.

## Features

- Passive scan of BLE advertisements (no connections, no transmission)
- Per packet: timestamp, device address, address type, advertisement type, RSSI, and raw advertising data
- One line of output per packet over USB serial
- Duplicates are not filtered, so repeated packets give RSSI samples

## Limitations

- Uses Zephyr's host scanner, so it cannot see CRC-failed packets or the channel a packet arrived on, and it cannot follow connections
- Cannot read encrypted traffic

## Hardware

- Seeed XIAO nRF52840 (SMD) mounted on a custom PCB, programmed and debugged over SWD
- 2x nRF24L01 modules on the same PCB (wired, not used by the current firmware)
- ST-Link probe for flashing

### nRF52840

The nRF52840 does all the work in this version: its BLE radio scans, the CPU timestamps and formats each packet, and the USB port carries the output to the PC.

### nRF24L01 modules

The two modules are wired but unused for now. The nRF24L01 uses its own packet format and cannot properly receive BLE traffic, so it takes no part in the BLE scanner. Planned uses are listed under Upcoming.

Both modules share one SPI bus. Each has its own CSN, CE, and IRQ line, which uses 9 GPIO pins in total.

#### Schematic

![Schematic: two nRF24L01 modules on a XIAO nRF52840](docs/images/schematic.png)

#### Pin map (XIAO nRF52840 SMD)

| XIAO pin | Signal | Goes to |
|---|---|---|
| D0 | CE_1 | Radio 1, CE |
| D1 | CSN_1 | Radio 1, CSN |
| D2 | CE_2 | Radio 2, CE |
| D3 | CSN_2 | Radio 2, CSN |
| D4 | IRQ_1 | Radio 1, IRQ |
| D5 | IRQ_2 | Radio 2, IRQ |
| D6 | not connected | Free |
| D7 | not connected | Free |
| D8 | SCK | Both radios, SCK |
| D9 | MISO | Both radios, MISO |
| D10 | MOSI | Both radios, MOSI |
| 3V3_OUT | +3.3 V | Both radios, VCC |
| GND | GND | Both radios, GND |
| VBUS | not connected | Unused |

Notes:
- 3.3 V only, never 5 V
- Each module has a 10 uF capacitor on VCC, placed close to the pins
- Keep SPI wires short, since the bus is shared

## What the firmware does

1. Initializes the Bluetooth stack and the USB serial port
2. Runs a continuous passive scan on the advertising channels
3. For each advertisement, reads the address, address type, advertisement type, RSSI, and raw advertising data
4. Attaches an uptime timestamp
5. Prints one line per packet to the USB serial port
6. Prints an error line if Bluetooth or scanning fails to start
7. Never connects to or transmits to other devices

## Output format

One comma-separated line per packet:

```
ADV,<uptime_ms>,<address>,<adv_type>,<rssi_dbm>,<data_length>,<data_hex>
```

Status and error lines start with `INFO` or `ERR`.

## Build and flash

Board target: `xiao_ble/nrf52840`. Flashing uses an ST-Link over SWD through OpenOCD.

Build:

```bash
west build -b xiao_ble/nrf52840
```

Pristine build (wipes the build folder first, use after changing the board, overlay, or Kconfig):

```bash
west build -b xiao_ble/nrf52840 -p always
```

Flash over SWD with ST-Link:

```bash
west flash --cmd-pre-init "source [find interface/stlink.cfg]" \
           --cmd-pre-init "transport select swd" \
           --cmd-pre-init "source [find target/nordic/nrf52.cfg]"
```

Wiring for flashing: connect the ST-Link's SWDIO, SWCLK, and GND to the board's SWD pads, and make sure the board is powered.

Troubleshooting:
- If `west` picks the wrong runner, add `-r openocd` to the flash command, and check the available runners with `west flash --context`
- If OpenOCD cannot connect, check the SWD wiring, the ST-Link connection, and that the board has power
- If the chip is locked, run a recover erase first. This wipes the whole chip, which also removes the UF2 bootloader, so the UF2 fallback below will stop working until you reinstall the bootloader
- Fallback without a probe: double-tap the reset button to enter the UF2 bootloader, then flash the UF2 file with `west flash -r uf2`

## Viewing the output

Plug the board into the PC over USB, open the COM port in any serial terminal, and the packet lines appear as the scan runs.

## Upcoming

Firmware:
- Shell for changing scan interval and window
- Per-device filtering by address or RSSI
- Record counter so lost packets can be detected
- On-device RSSI filtering
- Advertising data decoding (flags, name, TX power, manufacturer data, iBeacon, Eddystone)
- Private address rotation detection

nRF24L01 radios:
- Register read and write test on both modules
- Loopback test between the two radios
- 2.4 GHz occupancy scan, with each radio sweeping half the band
- Wireless data link between two boards

Host tools:
- Host reader with CSV logging
- Distance estimation from RSSI with calibration
- PCAP export for Wireshark
- Live dashboard

Project quality:
- Unit tests and CI
- Measured results table
- Raw-radio sniffer with connection following
- ESP32 port
- Angle of arrival

## License

Choose MIT or Apache-2.0 and credit any code you reuse.