# Real-Time Clock and Alarm System

This project implements a real-time clock and alarm system using embedded programming and a graphical user interface (GUI). It provides a reliable timekeeping mechanism with the capability to set and manage alarms.

## Features

- **Real-Time Clock:** Accurate timekeeping functionality.
- **Alarm Management:** Set, update, and delete alarms.
- **User Interface:** Interactive GUI for ease of use.

## Technologies
- **ATMega32**: interfacing between modules.
- **LCD & Keypad**: To set the clock and alarm.
- **7-Segment**: To show the clock time.
- **PyQT**: making GUI Application.
- **PySerial**: communicate between GUI application and microcontroller through UART Protocol.

## Project Structure

- **embedded/**: Contains the code for the embedded system implementation.
- **gui/**: Contains the Python code for the graphical user interface.


## Prerequisites
- `Atmel Studio 6.0`
- `VSPE Software Program`
- `Python 3.x`
- `Proteus Design Sutie 8.13`

## Installation
1. Run VSPE Software and create virtual com port at COM2.
2. Run real time clock proteus design
3. Run gui application
```sh
python main_window.py
```
## Usage
- **Setting Time**: Use the GUI to set the current time.
- **Managing Alarms**: Add, update, or delete alarms using the GUI. The embedded system will trigger alarms as scheduled.

## Screenshoots

**Proteus**

![alt text](https://serving.photos.photobox.com/027308578f9c425881ae4b4d9787ed389534a5de35f716ebe6f16826aaa9a28dea44910f.jpg)

**GUI**

![alt text](https://serving.photos.photobox.com/15733709eca601d0f611a261547124f4b514f518ee5ec087665674b942df6186334a7d5c.jpg)

## Contributors
- Gamal Ahmed Saeed
- Ahmed Sherif Sayed
- Ahmed Hamdi Mohamed

## License
This project is licensed under the Mozilla Public License 2.0. See the [LICENSE]() file for details.