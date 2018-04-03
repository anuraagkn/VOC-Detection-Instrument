# Instrument Controller

This repo contains source code for the back-end and front-end of a laboratory instrument controller (still in development). The back-end controller takes SCPI-style commands as input, then decodes and implements them in the instrument circuit.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.

### Prerequisites

What things you need to install the software and how to install them

* Arduino IDE with SPI library. See https://www.arduino.cc/en/Main/Software
* Anaconda3 (includes PyQt5) to run front-end Python GUI.

### Installing

#### Back-End
1. Download the Controller folder and save it in a suitable location.
2. Save the CommandHandler, Menu and Source libraries in the library directory of your Arduino IDE installation.
3. From the Controller folder open Controller.ino in the Arduino IDE, then compile the sketch and load it onto an Arduino board.

#### Front-End (Coming Soon)

## Usage

Command the instrument using the Serial Monitor. Parameters can be set or queried. Example:

```
:SOURce:FREQuency: 5000
```

sets the excitation waveform frequency to 5000Hz.

```
:SOURce:FREQuency?
```
Returns the source frequency.

A full list of commands is available at: (Coming Soon)
A document detailing the available commands is available [here](https://github.com/anuraagkn/VOC-Detection-Instrument/blob/master/COMMANDS.md).

## Built With

* [Arduino IDE 1.8.5](https://www.arduino.cc/en/Main/Software) - For compiling and running the Arduino program
* [Microsoft Visual Studio 2015](https://www.arduino.cc/en/Main/Software) - For writting the classes

## Authors

* **Anuraag Narang** - [anuraagkn](https://github.com/anuraagkn)

See also the list of [contributors](https://github.com/anuraagkn/VOC-Detection-Instrument/blob/master/Contributors.md) who participated in this project.

## Acknowledgments

* Project Supervisor Dr Kai Hoettges for continuing support.
* Kaspars Tuna for initial digital potentiometer code.
