# Bleskomat DIY

The Bleskomat DIY is an offline Bitcoin Lightning Network ATM which accepts coins and pays-out bitcoin. It's a great educational tool to give others a practical first experience with Bitcoin's Lightning Network.

You can buy the [Bleskomat DIY kit](https://shop.bleskomat.com/product/bleskomat-diy-kit/) from the official Bleskomat shop - includes all the parts needed to build the project. Alternatively, you can find all the components and equipment needed from various other sources - see [Parts Suppliers](#parts-suppliers).

The Bleskomat DIY must be paired with a server to facilitate Lightning Network payments on its behalf; see the options below:
* [Bleskomat Platform](https://platform.bleskomat.com) - non-custodial, requires a monthly subscription
* [bleskomat-server](https://github.com/bleskomat/bleskomat-server) - non-custodial, open-source, self-hosted solution
* [lnbits](https://github.com/lnbits/lnbits-legend) via the Bleskomat extension - open-source, self-hosted and possible to use custodial instances hosted by others; public instances of lnbits:
	* [legend.lnbits.com](https://legend.lnbits.com) - unstable, don't leave funds on this instance for very long

The rest of this document details the hardware and software requirements, how to build the hardware yourself, and instructions for compiling and uploading the firmware from source.

* [Requirements](#requirements)
	* [Hardware Requirements](#hardware-requirements)
		* [Parts Suppliers](#parts-suppliers)
	* [Software Requirements](#software-requirements)
* [Building the Hardware Device](#building-the-hardware-device)
	* [Prepare the breadboards](#prepare-the-breadboards)
		* [ESP32 devkit pinout](#esp32-devkit-pinout)
	* [Wiring the Power Supply](#wiring-the-power-supply)
		* [Optionally power the ESP32 devkit via 5V pin](#optionally-power-the-esp32-devkit-via-5v-pin)
	* [Wiring the TFT Display](#wiring-the-tft-display)
		* [Wiring the SD card slot](#wiring-the-sd-card-slot)
	* [Wiring the button](#wiring-the-button)
	* [Wiring the Coin Acceptor](#wiring-the-coin-acceptor)
		* [Wiring the DG600F](#wiring-the-dg600f)
		* [Wiring the HX616](#wiring-the-hx616)
	* [Wiring the Bill Acceptor](#wiring-the-bill-acceptor)
* [Configure and Train Coin Acceptor](#configure-and-train-coin-acceptor)
	* [Configure and Train the DG600F](#configure-and-train-the-dg600f)
	* [Configure and Train the HX616](#configure-and-train-the-hx616)
* [Configure Bill Acceptor](#configure-bill-acceptor)
* [Installing Libraries and Dependencies](#installing-libraries-and-dependencies)
* [Configure Firmware Build Flags](#configure-firmware-build-flags)
* [Compiling and Uploading to Device](#compiling-and-uploading-to-device)
* [Configuring the Device](#configuring-the-device)
	* [List of Configuration Options](#list-of-configuration-options)
	* [Browser-Based Configuration Tool](#browser-based-configuration-tool)
	* [Command-Line Configuration Tool](#command-line-configuration-tool)
	* [Hard-Coded Configuration](#hard-coded-configuration)
	* [Configuration via SD Card](#configuration-via-sd-card)
* [Changelog](#changelog)
* [Support](#support)
* [License](#license)
* [Trademark](#trademark)


## Requirements

This section includes information about the software and hardware requirements needed to build this project.


### Hardware Requirements

Basic components/equipment needed to build your own Bleskomat DIY:
* 2 x breadboard (400-pin)
* Jumper wires (M-M)
* Jumper wires (M-F)
* ESP32 Devkit
* Coin Acceptor - DG600F or HX616
* 1.8" TFT display
* Button
* 10k ohm resistor
* DC 5.5/2.1 mm DIP adapter
* 12V DC power adaptor with \~1A
* Standard USB to micro USB cable
* Multimeter

Optional components/equipment:
* Soldering iron
* 2.54 mm pins
* To power the ESP32 devkit from the 12V DC power supply:
	* XL4005 step-down converter
	* USB (F) adapter
	* Alternatively, you could use a USB car charger which includes both a step-down converter and USB (F) output
* If you wish to configure your Bleskomat via SD card:
	* Micro SD card with macro adapter

See the [Parts Suppliers](#parts-suppliers) section below for links to suppliers for each component/equipment.


#### Parts Suppliers

Below are parts lists based on geographic location:

* [Czech Republic](docs/parts-lists/czech-republic.md)
* [United Kingdom](docs/parts-lists/uk.md)
* [USA](docs/parts-lists/usa.md)

If you'd like to add your own list, please create a pull-request using [this template](docs/parts-lists/TEMPLATE.md) as a guide. Links with referral codes will not be accepted.


### Software Requirements

* [make](https://www.gnu.org/software/make/)
* [PlatformIO Core (CLI)](https://docs.platformio.org/en/latest/core/)
	* Version 6 or newer
	* Only the CLI ("Core") is required


## Building the Hardware Device

Before proceeding, be sure that you have all the project's [hardware requirements](#hardware-requirements).

Step-by-step build process for the hardware device.

### Prepare the breadboards

One breadboard is not large enough to accommodate all the pins of the ESP32 devkit due to the width of the devkit. This is why we recommend to connect two breadboards together.

Remove one of the power rails from one of the breadboards. Use the notches on the sides of the breadboards to connect them together length-wise.

Insert the ESP32 devkit into the pin holes of the new, combined breadboard.

![](docs/bleskomat-diy-build-breadboard-and-esp32-devkit.png)

Familiarize yourself with the ESP32 devkit's pinout reference below.

#### ESP32 devkit pinout

![](docs/ESP32-devkit-v1-board-pinout-36-gpio-pins.jpg)


### Wiring the Power Supply

The first step to building the device is wiring the power supply. If already plugged in, __unplug__ the 12V DC power supply now. Connect the DC 5.5/2.1 end of the power supply to the DIP adapter. The positive (+) pins should be the power pins and the negative (-) pins should be the ground. You can use your multimeter to check in case you want to be extra safe:
* Plug-in the power supply to electricity
* Turn on your multimeter and set it to measure voltage in the appropriate range (probably 20V)
* Touch the __red__ lead of your multimeter to one of the positive pins
* Touch the __black__ lead of your multimeter to one of the negative pins
* If you see a negative voltage reading, swap the leads between the two wires
* The pin touched by the __black__ lead is the ground ("GND")
* The pin touched by the __red__ lead is the hot wire ("DC12V")
* Unplug the power supply again

Use a pair of M-F jumper wires to connect the DC 5.5/2.1 DIP adapter to one of the breadboard's power rails. Negative to negative, positive to positive. This rail will be your 12V DC power (positive) and the common ground (negative).

#### Optionally power the ESP32 devkit via 5V pin

If you'd like to make your Bleskomat build a little bit more portable, you can power the ESP32 devkit via its 5V pin. To do this you will need the XL4005 (or equivalent) step-down converter and the USB (F) adapter. Note that powering the ESP32 devkit via its micro USB port requires a regulated voltage of approximately 5V.

It's also possible to use a USB car charger in case you have an extra one lying around.

Connect the step-down converter's input pins to the 12V DC power rail and common ground using (M-F) jumper wires. Use a multimeter to measure the voltage at the out pins. In the case of the XL4005, use a small screwdriver to turn the screw on the little blue box. Turning the screw counter clockwise should lower the voltage, turning it the opposite direction should increase the voltage. Once you have the voltage set to 5V, connect the out pins to the center power rails of the breadboard. This will be your 5V DC power rail.

Use a soldering iron to solder four 2.54 mm pins to the USB (F) DIP adapter. Insert the pins directly into the breadboard wherever you have space available. Using (M-M) jumper wires, connect the negative and positive pins of the USB (F) DIP adapter to the 5V DC power rail.

Connect the negative pin of the 5V DC power rail to the negative of the 12V DC power rail to ensure that they share a common ground. This is important because without a common ground shared between the coin acceptor and ESP32 devkit, the ESP32 will not receive a clean signal from the coin acceptor.

Use a standard USB to micro USB cable to connect the USB (F) DIP adapter to the ESP32 devkit.

There are other options when powering the ESP32 - e.g via the 3.3V pin or the 5V/VIN pin. You should __never__ power the ESP32 via more than one of these options at the same time. For example, do not power the ESP32 via its 3.3V pin while also connecting the ESP32 via USB to your computer. This can damage the ESP32 and possibly also your computer.


### Wiring the TFT Display

Insert the pins of the TFT display module into the breadboard where you have space available.

![](docs/bleskomat-diy-build-tft-display.png)

Use the table below to connect the ESP32 devkit to the TFT display module.

|  ESP32       | TFT         |
|--------------|-------------|
| VIN (V5)     | VCC         |
| GND          | GND         |
| GPIO22       | CS          |
| GPIO4        | RESET (RST) |
| GPIO2        | AO (RS)     |
| GPIO23       | SDA         |
| GPIO18       | SCK (CLK)   |
| 3V3          | LED (NC)    |

Notes on pin naming:
* There are boards where `GPIXXX` are marked as `GXX` instead of `DXX`.
* The `G23` may be there **twice** - the correct one is next to `GND`.
* Some boards have typos so a bit of guess-and-check is necessary sometimes.

![](docs/bleskomat-diy-build-tft-display-with-wires.png)

Refer to the [ESP32 devkit pinout](#esp32-devkit-pinout) for help identifying the pins on your ESP32.


#### Wiring the SD card slot

This step is only needed if you would like to configure your Bleskomat via an SD card.

The TFT display includes an SD card slot. The pins are located above the screen and not soldered to the module by default but the pins can be added. The table of mappings is the following:

| ESP32  | TFT     |
|--------|---------|
| GPIO15 | SD_CS   |
| GPIO13 | SD_MOSI |
| GPIO12 | SD_MISO |
| GPIO14 | SD_SCK  |

Refer to the [ESP32 devkit pinout](#esp32-devkit-pinout) for help identifying the pins on your ESP32.


### Wiring the button

Insert button pins into the breadboard wherever you have space available. Typically, its best to place the button at the center of a breadboard (over the gap).

Connect the button using the following table as a guide:

| ESP32    | Button    |
|----------|-----------|
| VIN (V5) | left pin  |
| GPIO33   | right pin |

Connect the right pin of the button to GND with a 10k ohm resistor.

![](docs/bleskomat-diy-build-button-with-wires.png)

Refer to the [ESP32 devkit pinout](#esp32-devkit-pinout) for help identifying the pins on your ESP32.


### Wiring the Coin Acceptor

This project supports two possible coin acceptors - the DG600F and the HX616. The DG600F is more expensive, but is easier to configure and more reliable.


#### Wiring the DG600F

| ESP32       | DG600F   | Power Supply  |
|-------------|----------|---------------|
| GPIO1 (TX0) | INHIBIT  |               |
|             | COUNTER  |               |
|             | GND      | - Ground      |
| GPIO3 (RX0) | SIGNAL   |               |
|             | DC12V    | + 12V DC      |

![](docs/bleskomat-diy-build-coin-acceptor-dg600f.png)

Refer to the [ESP32 devkit pinout](#esp32-devkit-pinout) for help identifying the pins on your ESP32.


#### Wiring the HX616

| ESP32       | HX616   | Power Supply  |
|-------------|----------|---------------|
|             | DC12V    | + 12V DC      |
| GPIO3 (RX0) | COIN     |               |
|             | GND      | - Ground      |

Refer to the [ESP32 devkit pinout](#esp32-devkit-pinout) for help identifying the pins on your ESP32.


### Wiring the Bill Acceptor

This project supports the NV10 (USB+) and NV9 bill acceptors. The SIO protocol is used to communicate with the bill acceptor unit.

|  ESP32       | NV10/NV9 | Power Supply  |
|--------------|----------|---------------|
| GPIO16       | 1 (Tx)   |               |
| GPIO17       | 5 (Rx)   |               |
|              | 16       | - Ground      |
|              | 15       | + 12V DC      |

Refer to the [ESP32 devkit pinout](#esp32-devkit-pinout) for help identifying the pins on your ESP32.


## Configure and Train Coin Acceptor

This project supports two possible coin acceptors - the DG600F and the HX616.

### Configure and Train the DG600F

Physical switches on the DG600F should set as follows:

| Switch           | State         |
|------------------|---------------|
| 1 (Port Level)   | Down (NO)     |
| 2 (Security)     | Down (Normal) |
| 3 (Transmitting) | Up (RS232)    |
| 4 (Inhibiting)   | Down (> +3V)  |

![](docs/DG600F-DIP-switch-configuration.png)

Open the [DG600F manual](docs/DG600F-Coin-Acceptor-Technical-Manual.pdf) to "Coin Acceptor Parameters Setting" on page 18. Set the parameters as follows:

| Parameter | Description                      | Value | Meaning                                          |
|-----------|----------------------------------|-------|--------------------------------------------------|
| A1        | machine charge amount            | 01    | min. coin value before data sent                 |
| A2        | serial output signal pulse-width | 01    | 25ms / 9600 bps (RS232 baud rate)                |
| A3        | faulty alarm option              | 01    | (rings only one time)                            |
| A4        | serial port RS232 signal length  | 03    | 3 bytes: 0xAA, coin value, XOR of prev two bytes |
| A5        | serial port output               | 01    | output to serial pin                             |


To train the coin acceptor, have a look at "Coin Parameters Setting" on page 16 of the [DG600F manual](docs/DG600F-Coin-Acceptor-Technical-Manual.pdf). Be sure to set the "coin value" for each coin in series, incremented by 1. For example:
* 0.05 EUR = 1 coin value
* 0.10 EUR = 2 coin value
* 0.20 EUR = 3 coin value
* 0.50 EUR = 4 coin value
* 1.00 EUR = 5 coin value
* 2.00 EUR = 6 coin value

Then set the `coinValues` configuration equal to `0.05,0.10,0.20,0.50,1,2`. For example, in the bleskomat.conf:
```
coinValues=0.05,0.10,0.20,0.50,1,2
```


### Configure and Train the HX616

There are two switches on the back of the HX616 coin acceptor:
* "NO" (normally open) / "NC" (normally closed) - This should be set to "NO"
* "Fast" / "Medium" / "Slow" - This should be set to "Fast"

An instruction sheet is included with the coin acceptor that will guide you through the training process:

![](docs/coin-acceptor-hx616-instructions-side-1.jpg)

| PANEL  | Meaning                                     |
|--------|---------------------------------------------|
| E      | number of coin values                       |
| Hn     | sampling quantity of coin n                 |
| Pn     | signal output value of coin n               |
| Fn     | recognition accuracy of coin n (8 advised)  |
| A      | training the acceptor                       |
| An     | start inserting coins for coin n            |

Press ADD + MINUS to get to the setting menu for Hn, Pn, Fn. Then press SET to swap between possibilities. To change the option press ADD or MINUS to select the desired value and confirm with SET.

Press SET to enter the training menu (A appears on the panel). After pressing SET again you can start inserting the coins.

If training the HX616 to accept EUR coins, you can set the `coinValueIncrement` equal to `0.05`. For example, in bleskomat.conf:
```
coinValueIncrement=0.05
```


## Configure Bill Acceptor

Please refer to the following user manuals for detailed instructions regarding how to configure the NV10 (USB+) or NV9 bill acceptors:
* [NV10 User Manual](docs/NV10-User-Manual-v1.1.pdf)
* [NV9 User Manual](docs/NV9-User-Manual-v1.1.pdf)



## Installing Libraries and Dependencies

Before proceeding, be sure that you have all the project's [software requirements](#software-requirements).

Use make to install libraries and dependencies needed to build the firmware:
```bash
make install
```
* The firmware's dependencies are defined in its platformio.ini file located at `./platformio.ini`

If while developing you need to install a new library, use the following as a guide:
```bash
platformio lib install LIBRARY_NAME[@VERSION]
```
You can find PlatformIO's libraries repository [here](https://platformio.org/lib).

2025-01-25 - Seems you need to add TFT_eSPI by running
pio lib install bodmer/TFT_eSPI


## Configure Firmware Build Flags

There are several build flags that can be changed in file platformio.ini file located at `./platformio.ini` before the firmware is compiled. If you follow the wire mappings in this readme file, then you most likely do not need to change any flags.


## Compiling and Uploading to Device

To compile the firmware (without uploading to a device):
```bash
make compile
```

To compile and upload to your device:
```bash
make upload DEVICE=/dev/ttyUSB0
```
The device path for your operating system might be different. If you receive a "Permission denied" error about `/dev/ttyUSB0` then you will need to set permissions for that file on your system:
```bash
sudo chown $USER:$USER /dev/ttyUSB0
```

To open the serial monitor:
```bash
make monitor DEVICE=/dev/ttyUSB0
```
Again the device path here could be different for your operating system.


## Configuring the Device

It is possible to configure the device via the following methods:
* [Browser-Based Configuration Tool](#browser-based-configuration-tool)
* [Command-Line Configuration Tool](#command-line-configuration-tool)
* [Hard-Coded Configuration](#hard-coded-configuration)
* [Configuration via SD Card](#configuration-via-sd-card)


### List of Configuration Options

The following is a list of possible configuration options for the Bleskomat DIY:
* `apiKey.id` - The API key ID of the device. This is needed by the server to verify signatures created by the device.
* `apiKey.key` - The API key secret that is used to generate signatures.
* `apiKey.encoding` - The explicit encoding of the API key secret. This can be "hex", "base64", or empty-string (e.g "") to mean no encoding. When generating a new API key on the server, it will store the encoding along with the ID and secret.
* `callbackUrl` - The LNURL server base URL plus endpoint path. Example:
	* `https://p.bleskomat.com/u`
* `shorten` - Whether or not to shorten the LNURL; see the [lnurl-node module](https://github.com/chill117/lnurl-node#signed-lnurls) for more details.
* `uriSchemaPrefix` - The URI schema prefix for LNURLs generated by the device. It has been discovered that some wallet apps mistakenly only support lowercase URI schema prefixes. Uppercase is better because when encoded as a QR code, the generated image is less complex and so easier to scan. Set this config to empty-string (e.g `uriSchemaPrefix=`) to not prepend any URI schema prefix.
* `fiatCurrency` - The fiat currency symbol for which the device is configured; see [ISO 4217](https://en.wikipedia.org/wiki/ISO_4217).
* `fiatPrecision` - The number of digits to the right of the decimal point when rendering fiat currency amounts.
* `billValues` - The value of bills for which the bill acceptor has been configured. Each value separated by a comma. Integers and floating point (decimal) values are accepted. Examples:
	* CZK: `100,200,500,1000,2000,5000`
	* EUR: `5,10,20,50,100,200,500`
* `billTxPin` - The GPIO connected to the NV10/NV9's Tx pin.
* `billRxPin` - The GPIO connected to the NV10/NV9's Rx pin.
* `billBaudRate` - The baud rate of the SIO protocol communication with the NV10/NV9. This value will be `300` or `9600`.
* `coinValues` - For the DG600F coin acceptor. The value of coins for which the coin acceptor has been configured. Each value separated by a comma. Integers and floating point (decimal) values are accepted. For example, when trained with EUR coins this configuration could be set to `0.05,0.10,0.20,0.50,1,2`.
* `coinValueIncrement` - For the HX616 coin acceptor, which communicates via pulses to the ESP32. The value here will determine how much fiat value each pulse represents. For example, when trained with EUR coins this value could be `0.05` with the following number of pulses set for each coin denomination:
	* 0.05 EUR = 1 pulse
	* 0.10 EUR = 2 pulses
	* 0.20 EUR = 4 pulses
	* 0.50 EUR = 10 pulses
	* 1.00 EUR = 20 pulses
	* 2.00 EUR = 40 pulses
* `coinSignalPin` - The GPIO connected to the HX616's "COIN" pin or DG600F's "SIGNAL" pin.
* `coinInhibitPin` - The GPIO connected to the DG600F's "INHIBIT"
* `coinBaudRate` - The baud rate for serial communication with the DG600F
* `coinAcceptorType` - "hx616" or "dg600f"
* `buttonPin` - The GPIO connected to the button
* `buttonDelay` - The number of milliseconds to wait before pressing the button can clear the QR code
* `buttonDebounce` - The number of milliseconds of debounce to prevent flickering button press/release
* `tftRotation` - The orientation of the TFT display. This is useful to allow different positions of the display. The possible rotation values are:
	* 0 = 0 degrees
	* 1 = 90 degrees
	* 2 = 180 degrees
	* 3 = 270 degrees
* `logLevel` - Possible values:
	* `trace` - everything
	* `debug`
	* `info` - default
	* `warn`
	* `error`
	* `none` - nothing


### Browser-Based Configuration Tool

The Bleskomat Platform provides a [browser-based configuration tool](https://platform.bleskomat.com/serial) to upload pre-built device firmware, view real-time log output, update device configurations, run JSON-RPC serial commands, and more.


### Command-Line Configuration Tool

It is also possible to use the [bleskomat-cli](https://github.com/bleskomat/bleskomat-cli) command-line utility to communicate with a Bleskomat hardware device via JSON-RPC over serial API. Please refer to that project's readme for installation and usage information.


### Hard-Coded Configuration

Hard-coded configurations can be set by modifying the source file [config.cpp](https://github.com/bleskomat/bleskomat-diy/blob/master/src/config.cpp#L196).

Each time you make changes to the hard-coded configurations, you will need to re-compile and flash the ESP32's firmware.


### Configuration via SD Card

First you will need to format the SD card with the FAT32 filesystem.

Create a new file named `bleskomat.conf` at the root of the SD card's filesystem.

The following is an example `bleskomat.conf` file:
```
apiKey.id=6d830ddeb0
apiKey.key=b11cd6b002916691ccf3097eee3b49e51759225704dde88ecfced76ad95324c9
apiKey.encoding=hex
callbackUrl=https://p.bleskomat.com/u
shorten=true
uriSchemaPrefix=
fiatCurrency=EUR
fiatPrecision=2
coinValues=0.05,0.10,0.20,0.50,1,2
coinValueIncrement=0.05
coinSignalPin=3
coinInhibitPin=1
coinBaudRate=9600
coinAcceptorType=hx616
buttonPin=33
buttonDelay=5000
buttonDebounce=100
tftRotation=2
logLevel=info
```


## Changelog

See [CHANGELOG.md](https://github.com/bleskomat/bleskomat-diy/blob/master/CHANGELOG.md)


## Support

Need some help? Join us in the official [Telegram group](https://t.me/bleskomat) or send us an email at [support@bleskomat.com](mailto:support@bleskomat.com) and we will try our best to respond in a reasonable time. If you have a feature request or bug to report, please [open an issue](https://github.com/bleskomat/bleskomat-diy/issues) in this project repository.


## License

The project is licensed under the [GNU General Public License v3 (GPL-3)](https://tldrlegal.com/license/gnu-general-public-license-v3-(gpl-3)):
> You may copy, distribute and modify the software as long as you track changes/dates in source files. Any modifications to or software including (via compiler) GPL-licensed code must also be made available under the GPL along with build & install instructions.


## Trademark

"Bleskomat" is a registered trademark. You are welcome to hack, fork, build, and use the source code and instructions found in this repository. However, the right to use the name "Bleskomat" with any commercial products or services is withheld and reserved for the trademark owner.
