# Instrument Controller

This repo contains source code for the back-end (and soon front-end) of a laboratory instrument controller. The back-end controller takes SCPI commands as input, then decodes and implements them in the instrument circuit.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.

### Prerequisites

What things you need to install the software and how to install them

* Arduino IDE with SPI library. See https://www.arduino.cc/en/Main/Software

### Installing

A step by step series of examples that tell you have to get a development env running

Say what the step will be
1. Save 'Controller.ino' in a folder named 'Controller'.
2. Save the folders 'CommandHandler', 'Menu' and 'Source' in the library directory of your Arduino IDE installation.
3. Open 'Controller.ino' in the Arduino IDE and load the sketch onto your Arduino board.

```
Give the example
```

And repeat

```
until finished
```

End with an example of getting some data out of the system or using it for a little demo

## Built With

* [Arduino IDE 1.8.5](https://www.arduino.cc/en/Main/Software) - For writting Arduino running code
* [Microsoft Visual Studio 2015](https://www.arduino.cc/en/Main/Software) - For writting the classes

## Authors

* **Anuraag Narang** - [anuraagkn](https://github.com/anuraagkn)

See also the list of [contributors](https://github.com/anuraagkn/VOC-Detection-Instrument/blob/master/Contributors.md) who participated in this project.

## Acknowledgments

* Project Supervisor Dr Kai Hoettges for continuing support.
* Kaspars Tuna for initial digital potentiometer code.
