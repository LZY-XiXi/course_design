import os
from typing import Literal

from config import IMAGE_TYPES, VIDEO_TYPES, MODEL_TYPES


# 遍历目录下文件
def walkBaseDir(dir, file_type: Literal["image", "video", "model", "all"]):

    # 搜索图片类文件
    if file_type == "image":
        pathList = []
        if os.path.exists(dir):
            dir = os.path.abspath(dir)
            for root, _, files in os.walk(dir):
                for file in files:
                    if file.split(".")[-1] in IMAGE_TYPES:
                        pathList.append(os.path.join(root, file))
        return pathList

    # 搜索视频类文件
    elif file_type == "video":
        pathList = []
        if os.path.exists(dir):
            dir = os.path.abspath(dir)
            for root, _, files in os.walk(dir):
                for file in files:
                    if file.split(".")[-1] in VIDEO_TYPES:
                        pathList.append(os.path.join(root, file))

        return pathList

    # 搜索模型类文件
    elif file_type == "model":
        pathList = []
        if os.path.exists(dir):
            dir = os.path.abspath(dir)
            for root, _, files in os.walk(dir):
                for file in files:
                    if file.split(".")[-1] in MODEL_TYPES:
                        pathList.append(os.path.join(root, file))

        return pathList

    # 搜索图片类, 视频类, 模型类, 类名文件
    elif file_type == "all":
        imageList = []
        videoList = []
        modelList = []
        if os.path.exists(dir):
            dir = os.path.abspath(dir)
            for root, _, files in os.walk(dir):
                for file in files:
                    if file.split(".")[-1] in IMAGE_TYPES:
                        imageList.append(os.path.join(root, file))
                    elif file.split(".")[-1] in VIDEO_TYPES:
                        videoList.append(os.path.join(root, file))
                    elif file.split(".")[-1] in MODEL_TYPES:
                        modelList.append(os.path.join(root, file))

        return (imageList, videoList, modelList)
