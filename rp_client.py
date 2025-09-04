from PyQt5.QtWidgets import QApplication, QWidget, QLabel, QPushButton, QCheckBox, QBoxLayout
import sys
import struct
import socket
from QtPyNetwork.client import TCPClient

def setPinout(pinout):
    pass

class Window(QWidget):
    def __init__(self):
        QWidget.__init__(self)
        self.setWindowTitle('Magnetometer Control Client')

        layout = QBoxLayout(QBoxLayout.TopToBottom)
        self.setLayout(layout)

        label_layout = QBoxLayout(QBoxLayout.LeftToRight)
        self.label = QLabel('Magnetometer Controller')
        label_layout.addWidget(self.label, 0)
        layout.addLayout(label_layout)

        button_layout = QBoxLayout(QBoxLayout.LeftToRight)
        self.check1 = QCheckBox('PIN1')
        self.check1.setEnabled(False)
        self.check1.toggled.connect(self.toggleChecks)
        button_layout.addWidget(self.check1)
        self.check2 = QCheckBox('PIN2')
        self.check2.setEnabled(False)
        self.check2.toggled.connect(self.toggleChecks)
        button_layout.addWidget(self.check2)
        self.check3 = QCheckBox('PIN3')
        self.check3.setEnabled(False)
        self.check3.toggled.connect(self.toggleChecks)
        button_layout.addWidget(self.check3)
        self.check4 = QCheckBox('PIN4')
        self.check4.setEnabled(False)
        self.check4.toggled.connect(self.toggleChecks)
        button_layout.addWidget(self.check4)
        layout.addLayout(button_layout)

        control_layout = QBoxLayout(QBoxLayout.LeftToRight)
        self.connected = False
        self.buttonCon = QPushButton('Connect')
        self.buttonCon.clicked.connect(self.connectPressed)
        control_layout.addWidget(self.buttonCon)
        layout.addLayout(control_layout)

        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    
    @Slot()
    def toggleChecks(self):
        pinout = 0b00000000

        if(self.check1.isChecked()):
            pinout = pinout | 0b00000001

        if(self.check2.isChecked()):
            pinout = pinout | 0b00000010

        if(self.check3.isChecked()):
            pinout = pinout | 0b00000100
        
        if(self.check4.isChecked()):
            pinout = pinout | 0b00001000

        data = struct.pack('<BB', 0x02, pinout)
        self.socket.sendall(data)
        data = self.sock.recv(1024)

    @Slot()
    def connectPressed(self):
        if(self.connected):
            self.check1.setEnabled(False)
            self.check2.setEnabled(False)
            self.check3.setEnabled(False)
            self.check4.setEnabled(False)
            self.sock.close()
            self.buttonCon.setText('Connect')
        else:
            self.check1.setEnabled(True)
            self.check2.setEnabled(True)
            self.check3.setEnabled(True)
            self.check4.setEnabled(True)
            self.sock.connect(('rp-f082cf.local', 5005))
            self.buttonCon.setText('Disconnect')
        


app = QApplication(sys.argv)
win = Window()
win.show()

sys.exit(app.exec())