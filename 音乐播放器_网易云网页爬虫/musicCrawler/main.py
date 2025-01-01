# 爬取网易云网页版连接上的歌曲, 仅支持非会员歌曲
import sys
from PyQt5 import QtWidgets

from utils import Cloud_By_Singer

if __name__ == "__main__":
    app = QtWidgets.QApplication(sys.argv)
    window = Cloud_By_Singer.MainWindow()
    window.show()
    sys.exit(app.exec_())