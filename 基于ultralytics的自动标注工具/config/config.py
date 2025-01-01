import os

WS_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
DESKTOP_PATH = os.path.join(os.path.expanduser("~"), "Desktop")

IMAGE_TYPES = ["jpg", "jpeg", "png", "bmp"]
VIDEO_TYPES = ["mp4", "avi", "mov"]
MODEL_TYPES = ["pt", "pth", "onnx"]
MESSAGE_BOX_MESSAGE = [
    "未选择检测文件目录",
    "未选择模型目录",
    "选中文件已全部完成自动标注",
    "选择的文件均已完成标注, 请重新选择文件路径",
]

window_config = {
    "WindowTitle": "Autolabelme-Lin",
    "WindowWidth": 1600,
    "WindowHeight": 1200,
}
file_config = {
    "configIni": "config/config.ini",
    "windowIcon": "images/favicon.ico",
}
window_qss = {}
