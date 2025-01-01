# 爬取网易云网页版(歌单或歌手页面)连接上的歌曲, 仅支持非会员歌曲
# 会员歌曲会下载试听版或无音质
import configparser
import os
import sys
from PyQt5 import QtCore, QtGui, QtWidgets
import requests
from bs4 import BeautifulSoup


class DownloadThread(QtCore.QThread):
    output_signal = QtCore.pyqtSignal(str)

    def __init__(self, singer_url, save_path):
        super().__init__()
        self.singer_url = singer_url
        self.save_path = save_path

    def run(self):
        """
        运行函数，用于下载歌曲
        """
        try:
            headers = {
                "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/118.0.0.0 Safari/537.36"
            }  # 构造请求头
            url = self.singer_url.replace("/#", "")  # 去除URL中的尾部斜杠
            response = requests.get(url=url, headers=headers)  # 发送请求，获取响应
            soup = BeautifulSoup(response.text, "lxml")  # 解析响应内容
            song_list = soup.select("ul.f-hide li a")  # 选择所有带有指定选择器的元素
        except Exception as e:
            message = f"网址输入错误"
            self.output_signal.emit(message)
            self.output_signal.emit(str(e))
            return
        try:
            for song in song_list:
                song_name = song.text  # 获取歌曲名称
                message = f"《{song_name}》 下载中"
                self.output_signal.emit(message)

                song_id = song["href"].split("=")[1]  # 获取歌曲ID
                music_open_api = (
                    "http://music.163.com/song/media/outer/url?id=" + song_id
                )  # 构造音乐接口URL
                music = requests.get(url=music_open_api)  # 发送请求，获取音乐响应

                try:
                    with open(f"{self.save_path}/{song_name}.mp3", "wb") as file:
                        file.write(music.content)  # 将音乐内容写入文件

                    message = f"《{song_name}》下载成功\n"
                except Exception as e:
                    message = f"{song_name}下载异常: {str(e)}\n"

                self.output_signal.emit(message)
        except Exception as e:
            return
        


class MainWindow(QtWidgets.QMainWindow):
    def __init__(self):
        super().__init__()
        self.settingfile = "configs/config.ini"  # 配置文件
        self.save_path = os.path.abspath(os.path.dirname(__file__))  # 获取当前文件所在目录的绝对路径
        self.loadSetting()  # 加载设置
        self.setWindowTitle("网易云音乐下载(歌单批量下载)")  # 设置窗口标题为
        self.setGeometry(800, 300, 1000, 800)  # 设置窗口位置和大小

        self.url_label = QtWidgets.QLabel("请输入您要下载网易云音乐页面链接:")
        self.url_input = QtWidgets.QLineEdit()
        self.download_button = QtWidgets.QPushButton("下载歌曲")  # 下载按钮
        self.download_button.clicked.connect(self.download_songs)

        self.message_area = QtWidgets.QPlainTextEdit()  # 创建一个文本框，用于显示消息
        self.message_area.setReadOnly(True)  # 设置文本框只读

        self.link_label = QtWidgets.QLabel()
        self.link_label.setText(
            '<a href="https://music.163.com/#">点击此处跳转到网易云音乐首页</a>'
        )  # 设置标签的文本为超链接，点击时跳转到网易云音乐首页
        self.link_label.setOpenExternalLinks(True)  # 允许点击外部链接
        self.link_label.setAlignment(QtCore.Qt.AlignCenter)  # 设置标签文本居中对齐
        self.link_label.linkActivated.connect(self.open_link)

        self.select_folder_button = QtWidgets.QPushButton("选择下载位置")  # 创建下载位置按钮
        self.select_folder_button.clicked.connect(self.select_folder)

        # 配置窗口布局
        layout = QtWidgets.QVBoxLayout()
        layout.addWidget(self.url_label)
        layout.addWidget(self.url_input)
        layout.addWidget(self.select_folder_button)
        layout.addWidget(self.download_button)
        layout.addWidget(self.message_area)
        layout.addWidget(self.link_label)

        widget = QtWidgets.QWidget()
        widget.setLayout(layout)
        self.setCentralWidget(widget)

    def select_folder(self):
        self.save_path = QtWidgets.QFileDialog.getExistingDirectory(
            self, "选择下载位置", self.save_path
        )
        if self.save_path:
            self.updateSetting()
        else:
            self.loadSetting()

    def download_songs(self):
        self.message_area.clear()
        if self.save_path is None:
            self.message_area.appendPlainText("请先选择下载位置")
            return
        self.updateSetting()
        singer_url = self.url_input.text()
        self.thread = DownloadThread(singer_url, self.save_path)
        self.thread.output_signal.connect(self.display_output)
        self.thread.start()

    def display_output(self, message):
        self.message_area.appendPlainText(message)

    def open_link(self, url):
        QtGui.QDesktopServices.openUrl(QtCore.QUrl(url))

    def loadSetting(self):
        if os.path.isfile(self.settingfile):
            config = configparser.ConfigParser()
            config.read(self.settingfile)
            self.save_path = config.get("path", "save_path")
        else:
            self.save_path = os.path.abspath(os.path.dirname(__file__))

    def updateSetting(self):
        config = configparser.ConfigParser()
        config.read(self.settingfile)
        if not os.path.isfile(self.settingfile):
            config.add_section("DownLoad")
        config.set("path", "save_path", self.save_path)
        config.write(open(self.settingfile, "w"))



