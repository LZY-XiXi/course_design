from PyQt5 import QtWidgets
import sys
from src import music

"""run"""
if __name__ == "__main__":
    app = QtWidgets.QApplication(sys.argv)
    gui = music.musicPlayer()
    gui.show()
    sys.exit(app.exec_())
