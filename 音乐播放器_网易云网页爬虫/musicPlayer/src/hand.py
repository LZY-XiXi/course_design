import cv2
import mediapipe as mp
import math


class handDetector:
    def __init__(
        self, mode=False, maxHands=1, complexity=1, detectionCon=0.5, trackCon=0.5
    ):
        # 初始化类的参数
        self.mode = mode  # 手检测模式
        self.maxHands = maxHands  # 最大手数
        self.complexity = complexity  # 模型复杂度
        self.detectionCon = detectionCon  # 检测置信度阈值
        self.trackCon = trackCon  # 跟踪置信度阈值

        # 初始化手跟踪模块
        self.mpHands = mp.solutions.hands
        self.hands = self.mpHands.Hands(
            self.mode, self.maxHands, self.complexity, self.detectionCon, self.trackCon
        )
        self.mpDraw = mp.solutions.drawing_utils

    def vector_2d_angle(self, v1, v2):  # 求解二维向量的角度
        v1_x = v1[0]
        v1_y = v1[1]
        v2_x = v2[0]
        v2_y = v2[1]
        try:
            angle = math.degrees(
                math.acos(
                    (v1_x * v2_x + v1_y * v2_y)
                    / (
                        ((v1_x**2 + v1_y**2) ** 0.5)
                        * ((v2_x**2 + v2_y**2) ** 0.5)
                    )
                )
            )
        except:
            angle = 114514.0
        if angle > 180.0:
            angle = 114514.0
        return angle

    def hand_angle(self, hand_):  # 计算手指角度
        angle_list = []
        # ---------------------------- thumb 大拇指角度
        angle = self.vector_2d_angle(
            (
                (int(hand_[0][0]) - int(hand_[2][0])),
                (int(hand_[0][1]) - int(hand_[2][1])),
            ),
            (
                (int(hand_[3][0]) - int(hand_[4][0])),
                (int(hand_[3][1]) - int(hand_[4][1])),
            ),
        )
        angle_list.append(angle)
        # ---------------------------- index 食指角度
        angle = self.vector_2d_angle(
            (
                (int(hand_[0][0]) - int(hand_[6][0])),
                (int(hand_[0][1]) - int(hand_[6][1])),
            ),
            (
                (int(hand_[7][0]) - int(hand_[8][0])),
                (int(hand_[7][1]) - int(hand_[8][1])),
            ),
        )
        angle_list.append(angle)
        # ---------------------------- middle 中指角度
        angle = self.vector_2d_angle(
            (
                (int(hand_[0][0]) - int(hand_[10][0])),
                (int(hand_[0][1]) - int(hand_[10][1])),
            ),
            (
                (int(hand_[11][0]) - int(hand_[12][0])),
                (int(hand_[11][1]) - int(hand_[12][1])),
            ),
        )
        angle_list.append(angle)
        # ---------------------------- ring 无名指角度
        angle = self.vector_2d_angle(
            (
                (int(hand_[0][0]) - int(hand_[14][0])),
                (int(hand_[0][1]) - int(hand_[14][1])),
            ),
            (
                (int(hand_[15][0]) - int(hand_[16][0])),
                (int(hand_[15][1]) - int(hand_[16][1])),
            ),
        )
        angle_list.append(angle)
        # ---------------------------- pink 小拇指角度
        angle = self.vector_2d_angle(
            (
                (int(hand_[0][0]) - int(hand_[18][0])),
                (int(hand_[0][1]) - int(hand_[18][1])),
            ),
            (
                (int(hand_[19][0]) - int(hand_[20][0])),
                (int(hand_[19][1]) - int(hand_[20][1])),
            ),
        )
        angle_list.append(angle)
        return angle_list

    def h_gesture(self, angle_list):  # 根据手指角度确定手势
        """
        action       播放   点赞
        pause        暂停    5
        preview     上一首   1
        next        下一首  拳头
        conversion  换模式  love
        """
        thr_angle = 65.0
        thr_angle_thumb = 53.0
        thr_angle_s = 49.0
        gesture_str = None
        if 114514.0 not in angle_list:
            if (
                (angle_list[0] < thr_angle_s)
                and (angle_list[1] > thr_angle)
                and (angle_list[2] > thr_angle)
                and (angle_list[3] > thr_angle)
                and (angle_list[4] > thr_angle)
            ):
                gesture_str = "action"
            elif (
                (angle_list[0] < thr_angle_s)
                and (angle_list[1] < thr_angle_s)
                and (angle_list[2] < thr_angle_s)
                and (angle_list[3] < thr_angle_s)
                and (angle_list[4] < thr_angle_s)
            ):
                gesture_str = "pause"

            elif (
                (angle_list[0] > 5)
                and (angle_list[1] < thr_angle_s)
                and (angle_list[2] > thr_angle)
                and (angle_list[3] > thr_angle)
                and (angle_list[4] > thr_angle)
            ):
                gesture_str = "preview"
            elif (
                (angle_list[0] > thr_angle_thumb)
                and (angle_list[1] > thr_angle)
                and (angle_list[2] > thr_angle)
                and (angle_list[3] > thr_angle)
                and (angle_list[4] > thr_angle)
            ):
                gesture_str = "next"

            elif (
                (angle_list[0] < thr_angle_s)
                and (angle_list[1] < thr_angle_s)
                and (angle_list[2] > thr_angle)
                and (angle_list[3] > thr_angle)
                and (angle_list[4] < thr_angle_s)
            ):
                gesture_str = "conversion"

        return gesture_str

    def findPostion(self, img, handNo=0, draw=True):
        gesture_str = None
        imgRGB = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
        self.results = self.hands.process(imgRGB)
        if self.results.multi_hand_landmarks:
            for myHand in self.results.multi_hand_landmarks:
                self.mpDraw.draw_landmarks(img, myHand, self.mpHands.HAND_CONNECTIONS)
                hand_local = []
                for i in range(21):
                    x = myHand.landmark[i].x * img.shape[1]
                    y = myHand.landmark[i].y * img.shape[0]
                    hand_local.append((x, y))
                if hand_local:
                    angle_list = self.hand_angle(hand_local)
                    gesture_str = self.h_gesture(angle_list)
                    cv2.putText(img, gesture_str, (0, 100), 0, 1.3, (0, 0, 255), 3)
        return gesture_str
