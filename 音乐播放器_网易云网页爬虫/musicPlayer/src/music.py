import cv2
import os
import time
import random
import configparser
from PyQt5 import QtGui, QtCore, QtWidgets, QtMultimedia
from .hand import * # type: ignore

"""音乐播放器"""


class musicPlayer(QtWidgets.QWidget):
    def __init__(self):
        super().__init__()
        self.timer_camera = QtCore.QTimer()  # 定义定时器，用于控制显示视频的帧率
        self.handcheck = handDetector()  # 手势检测器
        self.handtype = ""  # 手势类型
        self.pix = QtGui.QPixmap("image/back.png")  # 蒙版+图片
        self.resize(self.pix.size())  # 设置窗口大小
        self.setMask(self.pix.mask())  # 设置蒙版
        self.setWindowTitle("手势识别播放器")  # 设置窗口标题
        self.cap = cv2.VideoCapture()  # 视频流
        self.caporder = 0  # 视频流序号
        self.player = QtMultimedia.QMediaPlayer()  # 创建一个QMediaPlayer对象用于播放音频
        self.songs_list = []  # 歌曲列表
        self.song_formats = ["mp3", "m4a", "flac", "wav", "ogg"]  # 支持的音频格式
        self.cur_playing_song = ""  # 当前正在播放的歌曲
        self.settingfile = "config/music_setting.ini"  # 配置文件
        self.cur_path = os.path.abspath(os.path.dirname(__file__))  # 获取当前文件的绝对路径
        self.is_switching = False  # 是否正在切换歌曲
        self.is_pause = True  # 是否暂停播放
        self.pTime = time.time()  # 时间1
        self.cTime = 0  # 时间2

        # 界面元素
        # --播放时间
        self.label1 = QtWidgets.QLabel("00:00", self)
        self.label1.setStyle(QtWidgets.QStyleFactory.create("Fusion"))
        self.label1.setGeometry(70, 1000, 100, 100)
        self.label2 = QtWidgets.QLabel("00:00", self)
        self.label2.setStyle(QtWidgets.QStyleFactory.create("Fusion"))
        self.label2.setGeometry(1100, 1000, 100, 100)

        # --音乐播放进度条
        self.slider = QtWidgets.QSlider(QtCore.Qt.Horizontal, self)
        self.slider.sliderMoved[int].connect(
            lambda: self.player.setPosition(self.slider.value())
        )
        self.slider.setStyle(QtWidgets.QStyleFactory.create("Fusion"))
        self.slider.setGeometry(140, 1000, 950, 100)

        # --音量图标
        self.vlabel = QtWidgets.QLabel(self)
        v_img = QtGui.QPixmap("image/vuluem.jpg")
        self.vlabel.setPixmap(v_img)
        self.vlabel.move(70, 955)

        # --音量控制滑动条
        self.vslider = QtWidgets.QSlider(QtCore.Qt.Horizontal, self)
        self.vslider.sliderMoved[int].connect(
            lambda: self.player.setVolume(self.vslider.value())
        )
        self.vslider.setStyle(QtWidgets.QStyleFactory.create("Fusion"))
        self.vslider.setValue(10)
        self.vslider.setGeometry(150, 930, 300, 100)

        # --上一首按钮
        self.preview_button = QtWidgets.QPushButton("上一首", self)
        self.preview_button.setGeometry(650, 680, 150, 60)
        self.preview_button.clicked.connect(self.previewMusic)
        self.preview_button.setStyle(QtWidgets.QStyleFactory.create("Fusion"))

        # --下一首按钮
        self.next_button = QtWidgets.QPushButton("下一首", self)
        self.next_button.setGeometry(1050, 680, 150, 60)
        self.next_button.clicked.connect(self.nextMusic)
        self.next_button.setStyle(QtWidgets.QStyleFactory.create("Fusion"))

        # --打开文件夹按钮
        self.open_button = QtWidgets.QPushButton("导入音乐", self)
        self.open_button.setGeometry(650, 900, 150, 60)
        self.open_button.setStyle(QtWidgets.QStyleFactory.create("Fusion"))
        self.open_button.clicked.connect(self.openDir)

        # --显示音乐列表
        self.qlist = QtWidgets.QListWidget(self)
        self.qlist.setGeometry(50, 50, 450, 850)
        self.qlist.itemDoubleClicked.connect(self.doubleClicked)
        self.qlist.setStyle(QtWidgets.QStyleFactory.create("windows"))

        # --播放按钮
        self.play_button = QtWidgets.QPushButton(self)
        self.play_button.setGeometry(685, 150, 500, 500)
        self.play_button.setStyleSheet(
            "QPushButton{border-image: url(image/circle1.jpg)}"
            "QPushButton:focus {outline: none;}"
        )
        self.play_button.clicked.connect(self.playMusic)

        # --手势按钮
        self.gesture_button = QtWidgets.QPushButton(self)
        self.gesture_button.setStyleSheet(
            "QPushButton{border-image: url(image/hand1.png)}"
            "QPushButton:focus {outline: none;}"
        )
        self.gesture_button.clicked.connect(self.button_open_camera_clicked)
        self.timer_camera.timeout.connect(self.show_camera)
        self.gesture_button.setGeometry(1555, 860, 120, 120)

        # --如果有初始化setting, 导入setting
        self.loadSetting()
        # --播放模式
        self.cmb = QtWidgets.QComboBox(self)
        self.cmb.setStyle(QtWidgets.QStyleFactory.create("Fusion"))
        self.cmb.addItem("顺序播放")
        self.cmb.addItem("单曲循环")
        self.cmb.addItem("随机播放")
        self.cmb.setGeometry(1050, 900, 150, 60)
        # --计时器
        self.timer = QtCore.QTimer(self)
        self.timer.start(1000)
        self.timer.timeout.connect(self.playByMode)

        # 定义显示视频的Label
        self.label_show_camera = QtWidgets.QLabel(self)
        self.label_show_camera.setFixedSize(500, 500)
        self.label_show_camera.setText("暂未打开摄像头或视频\n" + "林展羽 22人工智能c 202203082015")
        self.label_show_camera.setAlignment(QtCore.Qt.AlignCenter)
        self.label_show_camera.setStyleSheet(
            "background-color:white;" "font: bold 30px"
        )
        self.label_show_camera.move(1360, 245)

    def paintEvent(self, event):
        """绘制窗口"""
        paint = QtGui.QPainter(self)
        paint.drawPixmap(0, 0, self.pix.width(), self.pix.height(), self.pix)

    """根据播放模式播放音乐"""

    def playByMode(self):
        if (not self.is_pause) and (not self.is_switching):
            self.slider.setMinimum(0)
            self.slider.setMaximum(self.player.duration())
            self.slider.setValue(self.slider.value() + 1000)
        self.label1.setText(
            time.strftime("%M:%S", time.localtime(self.player.position() / 1000))
        )
        self.label2.setText(
            time.strftime("%M:%S", time.localtime(self.player.duration() / 1000))
        )
        # 顺序播放
        if (
            (self.cmb.currentIndex() == 0)
            and (not self.is_pause)
            and (not self.is_switching)
        ):
            if self.qlist.count() == 0:
                return
            if self.player.position() == self.player.duration():
                self.nextMusic()
        # 单曲循环
        elif (
            (self.cmb.currentIndex() == 1)
            and (not self.is_pause)
            and (not self.is_switching)
        ):
            if self.qlist.count() == 0:
                return
            if self.player.position() == self.player.duration():
                self.is_switching = True
                self.setCurPlaying()
                self.slider.setValue(0)
                self.playMusic()
                self.is_switching = False
        # 随机播放
        elif (
            (self.cmb.currentIndex() == 2)
            and (not self.is_pause)
            and (not self.is_switching)
        ):
            if self.qlist.count() == 0:
                return
            if self.player.position() == self.player.duration():
                self.is_switching = True
                self.qlist.setCurrentRow(random.randint(0, self.qlist.count() - 1))
                self.setCurPlaying()
                self.slider.setValue(0)
                self.playMusic()
                self.is_switching = False

    """打开文件夹"""

    def openDir(self):
        self.cur_path = QtWidgets.QFileDialog.getExistingDirectory(
            self, "选取文件夹", self.cur_path
        )
        if self.cur_path:
            self.showMusicList()
            self.cur_playing_song = ""
            self.setCurPlaying()
            self.label1.setText("00:00")
            self.label2.setText("00:00")
            self.slider.setSliderPosition(0)
            self.is_pause = True

    """导入setting"""

    def loadSetting(self):
        if os.path.isfile(self.settingfile):
            config = configparser.ConfigParser()
            config.read(self.settingfile)
            self.cur_path = config.get("MusicPlayer", "PATH")
            self.showMusicList()

    """更新setting"""

    def updateSetting(self):
        config = configparser.ConfigParser()
        config.read(self.settingfile)
        if not os.path.isfile(self.settingfile):
            config.add_section("MusicPlayer")
        config.set("MusicPlayer", "PATH", self.cur_path)
        config.write(open(self.settingfile, "w"))

    """显示文件夹中所有音乐"""

    def showMusicList(self):
        self.qlist.clear()
        self.updateSetting()
        if os.path.exists(self.cur_path):
            for song in os.listdir(self.cur_path):
                if song.split(".")[-1] in self.song_formats:
                    self.songs_list.append(
                        [song, os.path.join(self.cur_path, song).replace("\\", "/")]
                    )
                    self.qlist.addItem(song)
            self.qlist.setCurrentRow(0)
            if self.songs_list:
                self.cur_playing_song = self.songs_list[self.qlist.currentRow()][-1]

    """双击播放音乐"""

    def doubleClicked(self):
        self.slider.setValue(0)
        self.is_switching = True
        self.setCurPlaying()
        self.playMusic()
        self.is_switching = False

    """设置当前播放的音乐"""

    def setCurPlaying(self):
        self.cur_playing_song = self.songs_list[self.qlist.currentRow()][-1]
        self.player.setMedia(
            QtMultimedia.QMediaContent(QtCore.QUrl(self.cur_playing_song))
        )
        self.player.setVolume(50)

    """提示"""

    def Tips(self, message):
        QtWidgets.QMessageBox.about(self, "提示", message)

    """播放音乐"""

    def playMusic(self):
        if self.qlist.count() == 0:
            self.Tips("当前路径内无可播放的音乐文件")
            return
        if not self.player.isAudioAvailable():
            self.setCurPlaying()
        if self.is_pause or self.is_switching:
            self.player.play()
            self.is_pause = False
            self.handtype = ""
            self.play_button.setStyleSheet(
                "QPushButton{border-image: url(image/circle2.png)}"
                "QPushButton:focus {outline: none;}"
            )
        elif (not self.is_pause) and (not self.is_switching):
            self.player.pause()
            self.is_pause = True
            self.play_button.setStyleSheet(
                "QPushButton{border-image: url(image/circle1.jpg)}"
                "QPushButton:focus {outline: none;}"
            )

    """上一首"""

    def previewMusic(self):
        self.slider.setValue(0)
        if self.qlist.count() == 0:
            self.Tips("当前路径内无可播放的音乐文件")
            return
        pre_row = (
            self.qlist.currentRow() - 1
            if self.qlist.currentRow() != 0
            else self.qlist.count() - 1
        )
        self.qlist.setCurrentRow(pre_row)
        self.is_switching = True
        self.setCurPlaying()
        self.playMusic()
        self.is_switching = False

    """下一首"""

    def nextMusic(self):
        self.slider.setValue(0)
        if self.qlist.count() == 0:
            self.Tips("当前路径内无可播放的音乐文件")
            return
        next_row = (
            self.qlist.currentRow() + 1
            if self.qlist.currentRow() != self.qlist.count() - 1
            else 0
        )
        self.qlist.setCurrentRow(next_row)
        self.is_switching = True
        self.setCurPlaying()
        self.playMusic()
        self.is_switching = False

    def button_open_camera_clicked(self):
        if self.timer_camera.isActive() == False:  # 若定时器未启动
            flag = self.cap.open(
                self.caporder, cv2.CAP_DSHOW
            )  # 参数是0，表示打开笔记本的内置摄像头，参数是视频文件路径则打开视频
            if flag == False:  # flag表示open()成不成功
                QtWidgets.QMessageBox.warning(
                    self, "warning", "请检查相机于电脑是否连接正确", buttons=QtWidgets.QMessageBox.Ok
                )
            else:
                self.timer_camera.start(10)  # 定时器开始计时1ms，结果是每过30ms从摄像头中取一帧显示
            self.gesture_button.setStyleSheet(
                "QPushButton{border-image: url(image/hand2.png)}"
                "QPushButton:focus {outline: none;}"
            )
        else:
            self.timer_camera.stop()  # 关闭定时器
            self.cap.release()  # 释放视频流
            self.label_show_camera.clear()  # 清空视频显示区域
            self.label_show_camera.setText("暂未打开摄像头或视频\n" + "林展羽 22人工智能c 202203082015")
            self.gesture_button.setStyleSheet(
                "QPushButton{border-image: url(image/hand1.png)}"
                "QPushButton:focus {outline: none;}"
            )

    def show_camera(self):
        _, frame = self.cap.read()  # 从视频流中读取
        frame = cv2.flip(frame, 180)
        self.cTime = time.time()
        self.handtype = self.handcheck.findPostion(frame)
        if self.cTime - self.pTime > 1.5:
            self.pTime = self.cTime
            self.handtype_control()
        show = cv2.resize(frame, (500, 500))  # 把读到的帧的大小重新设置为 500*500
        show = cv2.cvtColor(show, cv2.COLOR_BGR2RGB)  # 视频色彩转换回RGB，这样才是现实的颜色
        showframe = QtGui.QImage(
            show.data, show.shape[1], show.shape[0], QtGui.QImage.Format_RGB888
        )  # 把读取到的视频数据变成QImage形式
        self.label_show_camera.setPixmap(
            QtGui.QPixmap.fromImage(showframe)
        )  # 往显示视频的Label里 显示QImage

    def handtype_control(self):
        if self.handtype == "action" and (self.is_pause or self.is_switching):
            self.playMusic()
        elif (
            self.handtype == "pause" and (not self.is_pause) and (not self.is_switching)
        ):
            self.playMusic()
        elif self.handtype == "preview":
            self.previewMusic()
        elif self.handtype == "next":
            self.nextMusic()
        elif self.handtype == "conversion":
            if self.cmb.currentIndex() == 0:
                self.cmb.setCurrentIndex(1)
            elif self.cmb.currentIndex() == 1:
                self.cmb.setCurrentIndex(2)
            elif self.cmb.currentIndex() == 2:
                self.cmb.setCurrentIndex(0)
        self.handtype = ""
