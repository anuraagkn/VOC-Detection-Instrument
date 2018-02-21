# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'deviceui.ui'
#
# Created by: PyQt5 UI code generator 5.6
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtGui, QtWidgets

class Ui_DeviceUI(object):
    def setupUi(self, DeviceUI):
        DeviceUI.setObjectName("DeviceUI")
        DeviceUI.resize(800, 480)
        self.centralWidget = QtWidgets.QWidget(DeviceUI)
        self.centralWidget.setObjectName("centralWidget")
        self.ioTabs = QtWidgets.QTabWidget(self.centralWidget)
        self.ioTabs.setGeometry(QtCore.QRect(0, 0, 800, 480))
        self.ioTabs.setObjectName("ioTabs")
        self.inputTab = QtWidgets.QWidget()
        self.inputTab.setAccessibleName("")
        self.inputTab.setObjectName("inputTab")
        self.waveformGroupBox = QtWidgets.QGroupBox(self.inputTab)
        self.waveformGroupBox.setGeometry(QtCore.QRect(20, 10, 301, 131))
        self.waveformGroupBox.setObjectName("waveformGroupBox")
        self.waveformSpinBox = QtWidgets.QDoubleSpinBox(self.waveformGroupBox)
        self.waveformSpinBox.setGeometry(QtCore.QRect(230, 20, 62, 31))
        self.waveformSpinBox.setObjectName("waveformSpinBox")
        self.waveformSlider = QtWidgets.QSlider(self.waveformGroupBox)
        self.waveformSlider.setGeometry(QtCore.QRect(10, 100, 281, 22))
        self.waveformSlider.setMaximum(99)
        self.waveformSlider.setOrientation(QtCore.Qt.Horizontal)
        self.waveformSlider.setObjectName("waveformSlider")
        self.waveformDial = QtWidgets.QDial(self.waveformGroupBox)
        self.waveformDial.setGeometry(QtCore.QRect(0, 20, 71, 71))
        self.waveformDial.setObjectName("waveformDial")
        self.sinePushButton = QtWidgets.QPushButton(self.waveformGroupBox)
        self.sinePushButton.setGeometry(QtCore.QRect(80, 20, 41, 31))
        self.sinePushButton.setObjectName("sinePushButton")
        self.squarePushButton = QtWidgets.QPushButton(self.waveformGroupBox)
        self.squarePushButton.setGeometry(QtCore.QRect(130, 20, 41, 31))
        self.squarePushButton.setObjectName("squarePushButton")
        self.trianglePushButton = QtWidgets.QPushButton(self.waveformGroupBox)
        self.trianglePushButton.setGeometry(QtCore.QRect(180, 20, 41, 31))
        self.trianglePushButton.setObjectName("trianglePushButton")
        self.waveformOutPushButton = QtWidgets.QPushButton(self.waveformGroupBox)
        self.waveformOutPushButton.setGeometry(QtCore.QRect(180, 60, 41, 31))
        self.waveformOutPushButton.setObjectName("waveformOutPushButton")
        self.frequencyPushButton = QtWidgets.QPushButton(self.waveformGroupBox)
        self.frequencyPushButton.setGeometry(QtCore.QRect(80, 60, 41, 31))
        self.frequencyPushButton.setObjectName("frequencyPushButton")
        self.amplitudePushButton = QtWidgets.QPushButton(self.waveformGroupBox)
        self.amplitudePushButton.setGeometry(QtCore.QRect(130, 60, 41, 31))
        self.amplitudePushButton.setObjectName("amplitudePushButton")
        self.waveformSpinBoxPushButton = QtWidgets.QPushButton(self.waveformGroupBox)
        self.waveformSpinBoxPushButton.setGeometry(QtCore.QRect(230, 60, 61, 31))
        self.waveformSpinBoxPushButton.setObjectName("waveformSpinBoxPushButton")
        self.ioTabs.addTab(self.inputTab, "")
        self.outputTab = QtWidgets.QWidget()
        self.outputTab.setObjectName("outputTab")
        self.ioTabs.addTab(self.outputTab, "")
        DeviceUI.setCentralWidget(self.centralWidget)

        self.retranslateUi(DeviceUI)
        self.ioTabs.setCurrentIndex(0)
        QtCore.QMetaObject.connectSlotsByName(DeviceUI)

    def retranslateUi(self, DeviceUI):
        _translate = QtCore.QCoreApplication.translate
        DeviceUI.setWindowTitle(_translate("DeviceUI", "DeviceUI"))
        self.waveformGroupBox.setTitle(_translate("DeviceUI", "Waveform"))
        self.sinePushButton.setText(_translate("DeviceUI", "SINE"))
        self.squarePushButton.setText(_translate("DeviceUI", "SQUA"))
        self.trianglePushButton.setText(_translate("DeviceUI", "TRIA"))
        self.waveformOutPushButton.setText(_translate("DeviceUI", "OUT"))
        self.frequencyPushButton.setText(_translate("DeviceUI", "FREQ"))
        self.amplitudePushButton.setText(_translate("DeviceUI", "AMPL"))
        self.waveformSpinBoxPushButton.setText(_translate("DeviceUI", "ENTER"))
        self.ioTabs.setTabText(self.ioTabs.indexOf(self.inputTab), _translate("DeviceUI", "Input"))
        self.ioTabs.setTabText(self.ioTabs.indexOf(self.outputTab), _translate("DeviceUI", "Output"))


if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    DeviceUI = QtWidgets.QMainWindow()
    ui = Ui_DeviceUI()
    ui.setupUi(DeviceUI)
    DeviceUI.show()
    sys.exit(app.exec_())

