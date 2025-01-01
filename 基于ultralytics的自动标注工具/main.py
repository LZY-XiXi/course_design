import sys

from PyQt5.QtWidgets import QApplication

from config import *
from tools import *
from gui import *

if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = MainWindow()
    window.show()
    sys.exit(app.exec_())
