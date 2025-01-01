from PyQt5.QtWidgets import (
    QMainWindow,
    QComboBox,
    QPushButton,
    QFileDialog,
    QListWidget,
    QLabel,
)
from PyQt5.QtGui import QImage, QPixmap, QIcon, QFont
from PyQt5.QtCore import QTimer

from ultralytics import YOLO

import configparser

import cv2

from config import *
from tools import *

from .message_box import *


class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()

        self.init_Gui()
        self.init_Config()
        self.init_Button()

    def init_Config(self):
        self.configIni = configparser.ConfigParser()
        self.configIni.read(file_config["configIni"], encoding="utf-8")

        # 模型路径(列表)初始化
        self.modelDir = self.configIni.get("PathDir", "modelDir")
        if not os.path.exists(self.modelDir):
            self.modelDir = DESKTOP_PATH
            self.modelList = []
        else:
            self.modelList = walkBaseDir(self.modelDir, file_type="model")
            self.update_ModelBox()

        self.cap = None  # 读取视频
        self.videoPath = None  # 视频路径
        self.saveDir = None  # 保存路径
        self.imagePath = None  # 图像路径
        self.modelIdx = -1  # 模型索引
        self.showListNum = 0  # 列表文件数量
        self.saveCount = 0  # 保存计数

        # 图像
        self.imageDir = DESKTOP_PATH
        self.imageList = []

        # 视频
        self.videoDir = DESKTOP_PATH
        self.videoList = []

    def init_Gui(self):
        # 图像显示计时器
        self.frameTimer = QTimer()
        self.frameTimer.timeout.connect(self.start_Predict)

        # 窗口大小检测计时器
        self.sizeTimer = QTimer()
        self.sizeTimer.timeout.connect(self.update_WindowSize)
        self.sizeTimer.start(50)

        self.setWindowTitle(window_config["WindowTitle"])
        self.setWindowIcon(QIcon(file_config["windowIcon"]))
        self.resize(window_config["WindowWidth"], window_config["WindowHeight"])
        self.windowWidth = window_config["WindowWidth"]
        self.windowHeight = window_config["WindowHeight"]
        self.showFrameSize = [int(self.width() * 0.65), int(self.height() * 0.6)]

        # 识别列表
        self.predictList = QListWidget(self)
        self.predictList.setGeometry(
            int(self.width() * 0.025),  # ax
            int(self.height() * 0.025),  # ay
            int(self.width() * 0.25),  # aw
            int(self.height() * 0.95),  # ah
        )

        # 检测画面
        self.showFrameLabel = QLabel(self)
        self.showFrameLabel.setGeometry(
            int(self.width() * 0.3),  # ax
            int(self.height() * 0.025),  # ay
            self.showFrameSize[0],  # aw
            self.showFrameSize[1],  # ah
        )
        self.showFrameLabel.setStyleSheet("QLabel{background-color:rgb(100,100,100)}")

        # 模型选择
        self.modelText = QLabel("模型选择", self)
        self.modelText.setFont(QFont("微软雅黑", 12))
        self.modelText.setGeometry(
            int(self.width() * 0.3),  # ax
            int(self.height() * 0.65),  # ay
            int(self.width() * 0.15),  # aw
            int(self.height() * 0.03),  # ah
        )
        self.modelBox = QComboBox(self)
        self.modelBox.setGeometry(
            int(self.width() * 0.3),  # ax
            int(self.height() * 0.7),  # ay
            int(self.width() * 0.2),  # aw
            int(self.height() * 0.05),  # ah
        )

    def init_Button(self):
        self.openImageDir = QPushButton("打开图片文件夹", self)
        self.openImageDir.setGeometry(
            int(self.width() * 0.3),  # ax
            int(self.height() * 0.77),  # ay
            int(self.width() * 0.2),  # aw
            int(self.height() * 0.05),  # ah
        )
        self.openImageDir.clicked.connect(lambda: self.open_Dir("image"))

        self.openVideoDir = QPushButton("打开视频文件夹", self)
        self.openVideoDir.setGeometry(
            int(self.width() * 0.3),  # ax
            int(self.height() * 0.83),  # ay
            int(self.width() * 0.2),  # aw
            int(self.height() * 0.05),  # ah
        )
        self.openVideoDir.clicked.connect(lambda: self.open_Dir("video"))

        self.openModelDir = QPushButton("打开模型文件夹", self)
        self.openModelDir.setGeometry(
            int(self.width() * 0.3),  # ax
            int(self.height() * 0.89),  # ay
            int(self.width() * 0.2),  # aw
            int(self.height() * 0.05),  # ah
        )
        self.openModelDir.clicked.connect(lambda: self.open_Dir("model"))

        self.importButton = QPushButton("一键导入", self)
        self.importButton.setGeometry(
            int(self.width() * 0.53),  # ax
            int(self.height() * 0.78),  # ay
            int(self.width() * 0.2),  # aw
            int(self.height() * 0.15),  # ah
        )
        self.importButton.clicked.connect(lambda: self.open_Dir("all"))

        self.predictButton = QPushButton("开始标注", self)
        self.predictButton.setGeometry(
            int(self.width() * 0.75),  # ax
            int(self.height() * 0.78),  # ay
            int(self.width() * 0.2),  # aw
            int(self.height() * 0.15),  # ah
        )
        self.predictButton.clicked.connect(self.control_Predict)

    def open_Dir(self, dir_type):
        if dir_type == "image":
            self.imageDir = QFileDialog.getExistingDirectory(
                self, "选择图片目录", self.imageDir
            )
            self.imageList = walkBaseDir(self.imageDir, file_type="image")
            self.update_PredictList()

        elif dir_type == "video":
            self.videoDir = QFileDialog.getExistingDirectory(
                self, "选择视频目录", self.videoDir
            )
            self.videoList = walkBaseDir(self.videoDir, file_type="video")
            self.update_PredictList()

        elif dir_type == "model":
            self.modelDir = QFileDialog.getExistingDirectory(
                self, "选择模型目录", self.modelDir
            )
            self.modelList = walkBaseDir(self.modelDir, file_type="model")
            self.update_ModelBox()

            if len(self.modelList) > 0:
                self.configIni.set("PathDir", "modelDir", self.modelDir)
                self.configIni.write(open(file_config["configIni"], "w"))

        elif dir_type == "all":
            fileDir = QFileDialog.getExistingDirectory(
                self, "选择目录(包含图片, 视频, 模型, 类别文件)", DESKTOP_PATH
            )
            self.imageList, self.videoList, self.modelList = walkBaseDir(
                fileDir, file_type="all"
            )
            self.update_ModelBox()
            self.update_PredictList()

            if len(self.modelList) > 0:
                self.modelDir = os.path.dirname(self.modelList[0])
                self.configIni.set("PathDir", "modelDir", self.modelDir)
                self.configIni.write(open(file_config["configIni"], "w"))

            if len(self.imageList) > 0:
                self.imageDir = os.path.dirname(self.imageList[0])
            if len(self.videoList) > 0:
                self.videoDir = os.path.dirname(self.videoList[0])

    # 更新模型选择
    def update_ModelBox(self):
        while self.modelBox.count() > 0:
            self.modelBox.removeItem(0)
        self.modelBox.addItems(
            [os.path.basename(modelName) for modelName in self.modelList]
        )
        self.modelBox.setCurrentIndex(0)

    # 更新检测列表
    def update_PredictList(self):
        self.predictList.clear()
        self.predictList.addItems([os.path.basename(_) for _ in self.imageList])
        self.predictList.addItems([os.path.basename(_) for _ in self.videoList])

        self.showListNum = len(self.imageList) + len(self.videoList)

        self.predictList.setCurrentRow(0)

        if len(self.imageList) > 0:
            self.imagePath = self.imageList[self.predictList.currentRow()]
            self.cap = None
            self.videoPath = None
            self.saveDir = None
        elif len(self.videoList) > 0:
            self.videoPath = self.videoList[self.predictList.currentRow()]
            self.cap = cv2.VideoCapture(self.videoPath)
            self.saveDir = os.path.join(
                os.path.dirname(self.videoPath),
                os.path.basename(self.videoPath).split(".")[0],
            )
            os.makedirs(self.saveDir, exist_ok=True)
            self.saveCount = 0
            self.imagePath = None

    # 选择下一个
    def next_File(self):
        fileIdx = self.predictList.currentRow() + 1
        if fileIdx != self.showListNum:
            self.predictList.setCurrentRow(fileIdx)
            if fileIdx >= len(self.imageList):
                if self.cap is not None:
                    self.cap.release()
                self.videoPath = self.videoList[fileIdx - len(self.imageList)]
                self.cap = cv2.VideoCapture(self.videoPath)
                self.saveDir = os.path.join(
                    os.path.dirname(self.videoPath),
                    os.path.basename(self.videoPath).split(".")[0],
                )
                os.makedirs(self.saveDir, exist_ok=True)
                self.saveCount = 0
                self.imagePath = None
            else:
                self.imagePath = self.imageList[self.predictList.currentRow()]
                self.videoPath = None
                self.cap = None
                self.saveDir = None

        else:
            finish_information(self, box_text=MESSAGE_BOX_MESSAGE[2])
            self.showFrameLabel.clear()
            if self.cap is not None:
                self.cap.release()
            self.imagePath = None
            self.videoPath = None
            self.cap = None
            self.saveDir = None
            self.frameTimer.stop()

    # 控制显示(检测)计时器
    def control_Predict(self):
        if self.showListNum == 0:
            dir_warning(self, box_text=MESSAGE_BOX_MESSAGE[0])
        elif self.modelBox.count() == 0:
            dir_warning(self, box_text=MESSAGE_BOX_MESSAGE[1])
        elif self.predictList.currentRow() + 1 == self.showListNum:
            finish_information(self, box_text=MESSAGE_BOX_MESSAGE[3])

        else:
            if self.frameTimer.isActive():
                self.frameTimer.stop()
            else:
                self.frameTimer.start(10)

    def start_Predict(self):
        # 加载模型
        if self.modelBox.currentIndex() != self.modelIdx:
            self.model = YOLO(self.modelList[self.modelBox.currentIndex()])
            self.modelIdx = self.modelBox.currentIndex()

        def predict_Image():
            src_frame = cv2.imread(self.imagePath)

            predictMessage, dst_frame = predictResult2jsonValue(
                result=self.model.predict(src_frame)[0],
            )
            toJson(imagePath=self.imagePath, predictMessage=predictMessage)

            showframe = cv2.resize(
                dst_frame, (self.showFrameSize[0], self.showFrameSize[1])
            )
            showframe = cv2.cvtColor(showframe, cv2.COLOR_BGR2RGB)

            # 转码
            labelFrame = QImage(
                showframe.data,
                self.showFrameSize[0],
                self.showFrameSize[1],
                3 * self.showFrameSize[0],
                QImage.Format_RGB888,
            )
            self.showFrameLabel.setPixmap(QPixmap.fromImage(labelFrame))

            self.next_File()

        def predict_Video():
            ret, src_frame = self.cap.read()
            if ret:
                predictMessage, dst_frame = predictResult2jsonValue(
                    result=self.model.predict(src_frame)[0],
                )
                cv2.imwrite(f"{self.saveDir}/{self.saveCount:08d}.jpg", src_frame)
                if toJson(
                    imagePath=f"{self.saveDir}/{self.saveCount:08d}.jpg",
                    predictMessage=predictMessage,
                ):
                    self.saveCount += 1
                else:
                    os.remove(f"{self.saveDir}/{self.saveCount:08d}.jpg")

                showFrame = cv2.resize(
                    dst_frame, (self.showFrameSize[0], self.showFrameSize[1])
                )
                showFrame = cv2.cvtColor(showFrame, cv2.COLOR_BGR2RGB)

                # 转码
                labelFrame = QImage(
                    showFrame.data,
                    self.showFrameSize[0],
                    self.showFrameSize[1],
                    3 * self.showFrameSize[0],
                    QImage.Format_RGB888,
                )
                self.showFrameLabel.setPixmap(QPixmap.fromImage(labelFrame))
            else:
                self.next_File()

        if self.cap is None:
            predict_Image()
        else:
            predict_Video()

    def update_WindowSize(self):

        if self.width() != self.windowWidth or self.height() != self.windowHeight:
            self.windowWidth = self.width()
            self.windowHeight = self.height()
            self.showFrameSize = [int(self.width() * 0.65), int(self.height() * 0.6)]

            # 识别列表
            self.predictList.setGeometry(
                int(self.width() * 0.025),  # ax
                int(self.height() * 0.025),  # ay
                int(self.width() * 0.25),  # aw
                int(self.height() * 0.95),  # ah
            )

            # 检测画面
            self.showFrameLabel.setGeometry(
                int(self.width() * 0.3),  # ax
                int(self.height() * 0.025),  # ay
                self.showFrameSize[0],  # aw
                self.showFrameSize[1],  # ah
            )

            # 模型选择
            self.modelText.setGeometry(
                int(self.width() * 0.3),  # ax
                int(self.height() * 0.65),  # ay
                int(self.width() * 0.15),  # aw
                int(self.height() * 0.03),  # ah
            )
            self.modelBox.setGeometry(
                int(self.width() * 0.3),  # ax
                int(self.height() * 0.7),  # ay
                int(self.width() * 0.2),  # aw
                int(self.height() * 0.05),  # ah
            )

            self.openImageDir.setGeometry(
                int(self.width() * 0.3),  # ax
                int(self.height() * 0.77),  # ay
                int(self.width() * 0.2),  # aw
                int(self.height() * 0.05),  # ah
            )

            self.openVideoDir.setGeometry(
                int(self.width() * 0.3),  # ax
                int(self.height() * 0.83),  # ay
                int(self.width() * 0.2),  # aw
                int(self.height() * 0.05),  # ah
            )

            self.openModelDir.setGeometry(
                int(self.width() * 0.3),  # ax
                int(self.height() * 0.89),  # ay
                int(self.width() * 0.2),  # aw
                int(self.height() * 0.05),  # ah
            )

            self.importButton.setGeometry(
                int(self.width() * 0.53),  # ax
                int(self.height() * 0.78),  # ay
                int(self.width() * 0.2),  # aw
                int(self.height() * 0.15),  # ah
            )

            self.predictButton.setGeometry(
                int(self.width() * 0.75),  # ax
                int(self.height() * 0.78),  # ay
                int(self.width() * 0.2),  # aw
                int(self.height() * 0.15),  # ah
            )
