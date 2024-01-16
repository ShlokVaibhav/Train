from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
from PyQt5.QtCore import *
import requests
from requests import ConnectionError
import time
import threading
from threading import Thread
import re

import openpyxl

import train_utils as TU

connection_timeout = 30 # seconds
    
app = QApplication([])
qtgui_obj = TU.SliderExample()
TU.est_connection(qtgui_obj)
print(qtgui_obj.pwm)

qtgui_obj.show()
app.exec_()


