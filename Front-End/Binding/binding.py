import sys
import deviceui as dui  # import Qt form
from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtWidgets import*
import serial
from time import sleep


class MainWindow(QMainWindow, dui.Ui_DeviceUI):

    #ser = serial.Serial('COM5', 2000000, timeout=0)  # serial port instance

    def __init__(self):
        super(self.__class__, self).__init__()
        self.setupUi(self)  # defined in UI file

        self.minFrequency = 0
        self.maxFrequency = 10000
        self.frequency = 1000   # default. will be used to store updated value
        self.minAmplitude = 16
        self.maxAmplitude = 30
        self.amplitude = 20     # default. will be used to store updated value

        # set waveform enter button colour  to grey
        self.waveformSpinBoxPushButton.setStyleSheet("background-color: LightGrey")

        # set default waveform output to off
        self.waveformOutPushButton.setStyleSheet("background-color: LightGrey")

        # set default wavetype to sine
        self.sinePushButton.setStyleSheet("background-color: GreenYellow")
        self.squarePushButton.setStyleSheet("background-color: LightGrey")
        self.trianglePushButton.setStyleSheet("background-color: LightGrey")

        # initialise waveform parameter to frequency
        self.frequencyPushButton.setStyleSheet("background-color: GreenYellow")
        self.amplitudePushButton.setStyleSheet("background-color: LightGrey")
        self.waveformParam = 0b0    # flag for waveform panel parameter. 0 = freq, 1 = ampl.
        self.setParamFreq()

        # sync waveform widgets and update parameter
        self.waveformSlider.valueChanged.connect(self.waveformDial.setValue)
        self.waveformSlider.valueChanged.connect(self.waveformSpinBox.setValue)
        self.waveformDial.valueChanged.connect(self.waveformSlider.setValue)
        self.waveformSpinBoxPushButton.clicked.connect(self.setWaveformSpinBoxValue)
        # update param
        self.waveformSlider.valueChanged.connect(self.setWaveformParam)

        # signal to configure waveform panel upon button toggle
        self.frequencyPushButton.clicked.connect(self.setParamFreq)
        self.amplitudePushButton.clicked.connect(self.setParamAmpl)

    def setWaveformSpinBoxValue(self):
        self.waveformSlider.setValue(self.waveformSpinBox.value())

    # configure waveform panel to set frequency
    def setParamFreq(self):
        self.waveformParam = 0b0    # flag: setting frequency
        self.setWaveformRange()     # set slider and spin box ranges to frequency range

    # configure waveform panel to set amplitude
    def setParamAmpl(self):
        self.waveformParam = 0b1    # flag: setting amplitude
        self.setWaveformRange()     # set slider and spin box ranges to amplitude range

    def setWaveformParam(self, value):
        if self.waveformParam == 0b0:
            self.frequency = value
        elif self.waveformParam == 0b1:
            self.amplitude = value

    def setWaveformRange(self):
        if self.waveformParam == 0b0:   # frequency push button selected
            # set button colours
            self.frequencyPushButton.setStyleSheet("background-color: GreenYellow")
            self.amplitudePushButton.setStyleSheet("background-color: LightGrey")
            # set spin box range
            self.waveformSpinBox.setRange(self.minFrequency, self.maxFrequency)
            # set slider minimums
            self.waveformSlider.setMinimum(self.minFrequency)
            self.waveformDial.setMinimum(self.minFrequency)
            # set slider maximums
            self.waveformSlider.setMaximum(self.maxFrequency)
            self.waveformDial.setMaximum(self.maxFrequency)
            # set slider values using the last set parameter value
            self.waveformSpinBox.setValue(self.frequency)
            self.waveformSlider.setValue(self.frequency)
            self.waveformDial.setValue(self.frequency)
        elif self.waveformParam == 0b1:     # amplitude push button selected
            # see above for comments.
            self.frequencyPushButton.setStyleSheet("background-color: LightGrey")
            self.amplitudePushButton.setStyleSheet("background-color: GreenYellow")
            # set slider values using the last set parameter value
            # this is done first due to smaller range than frequency
            self.waveformSpinBox.setValue(self.amplitude)
            self.waveformSlider.setValue(self.amplitude)
            self.waveformDial.setValue(self.amplitude)
            # set spin box and slider amplitude ranges (as with frequency)
            self.waveformSpinBox.setRange(self.minAmplitude, self.maxAmplitude)
            self.waveformSlider.setMinimum(self.minAmplitude)
            self.waveformDial.setMinimum(self.minAmplitude)
            self.waveformSlider.setMaximum(self.maxAmplitude)
            self.waveformDial.setMaximum(self.maxAmplitude)


# main method
def main():
    # new app instance
    app = QApplication(sys.argv)
    form = MainWindow()
    form.show()

    sys.exit(app.exec_())   # w/o this, script exits immediately


# run main on startup
if __name__ == "__main__":
    main()
