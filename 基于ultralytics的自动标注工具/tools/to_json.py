import os
from PIL import Image
import base64
import pkg_resources
import subprocess
import json

# 获取labelme版本号
try:
    LABELME_VERSION = pkg_resources.get_distribution("labelme").version
except:
    output = subprocess.run(
        ["pip", "index", "versions", "labelme"], capture_output=True
    ).stdout.decode("utf-8")

    if output:
        output = list(filter(lambda x: len(x) > 0, output.split("\n")))
        LABELME_VERSION = output[-1].split(":")[1].strip()
    else:
        LABELME_VERSION = "unknown"


class Json_Shape_Keys:
    def __init__(
        self,
        label: str,
        points: list,
        group_id: int,
    ) -> None:

        self.label = label
        self.points = points
        self.group_id = group_id
        self.description = ""
        self.flags = {}
        self.mask = None

        self.check_value()

    def check_value(self):
        for idx1, point in enumerate(self.points):
            if type(point) != list:
                if "polygon" in point:
                    self.shape_type = "polygon"
                else:
                    self.shape_type = point
                self.points.pop(idx1)
                break
            else:
                for idx2, value in enumerate(point):
                    if type(value) == float:
                        continue
                    else:
                        try:
                            point[idx2] = float(value)
                        except Exception as e:
                            print(e)
                            return False
                self.points[idx1] = point

        return True

    def to_json(self):
        return {
            "label": self.label,
            "points": self.points,
            "group_id": self.group_id,
            "description": self.description,
            "shape_type": self.shape_type,
            "flags": self.flags,
            "mask": self.mask,
        }


class Json_Keys:
    def __init__(
        self,
        shapes: list[Json_Shape_Keys],
        imagePath: str,
    ) -> None:

        self.version = LABELME_VERSION
        self.flags = {}
        self.shapes = shapes
        self.imagePath = os.path.basename(imagePath)
        self.imageData = self.image2base64(imagePath)
        self.imageWidth, self.imageHeight = Image.open(imagePath).size

    def image2base64(self, imagePath):
        with open(imagePath, "rb") as image:
            image_data = image.read()
            base64_data = base64.b64encode(image_data).decode("utf-8")
            return base64_data

    def to_json(self):
        return {
            "version": self.version,
            "flags": self.flags,
            "shapes": [shape.to_json() for shape in self.shapes],
            "imagePath": self.imagePath,
            "imageData": self.imageData,
            "imageWidth": self.imageWidth,
            "imageHeight": self.imageHeight,
        }


# 检测结果解析
def predictResult2jsonValue(result):
    has_boxes = type(result.boxes).__name__ != "NoneType"
    has_keypoints = type(result.keypoints).__name__ != "NoneType"
    has_obb = type(result.obb).__name__ != "NoneType"
    has_masks = type(result.masks).__name__ != "NoneType"

    if has_boxes and not (has_keypoints or has_masks):
        classes = result.names
        labeles = [classes[int(label)] for label in result.boxes.cls.tolist()]
        points = [
            [resultRectPoints[0:2], resultRectPoints[2:4], "rectangle"]
            for resultRectPoints in result.boxes.xyxy.tolist()
        ]
        group_ids = [idx for idx in range(len(labeles))]
        dst_frame = result.plot()
        return ((labeles, points, group_ids), dst_frame)

    elif has_keypoints :
        classes = result.names
        labeles = [classes[int(label)] for label in result.boxes.cls.tolist()]
        points = [
            [
                [
                    resultRectPoints[0:2],
                    resultRectPoints[2:4],
                    "rectangle",
                ]
            ]
            for resultRectPoints in result.boxes.xyxy.tolist()
        ]
        try:
            for idx in range(len(points)):
                for resultKeyPoint in result.keypoints.xy.tolist()[idx]:
                    points[idx].append([resultKeyPoint, "point"])
        except Exception as e:
            print(e)
            pass

        group_ids = [idx for idx in range(len(labeles))]
        dst_frame = result.plot()
        return ((labeles, points, group_ids), dst_frame)

    elif has_masks:
        classes = result.names
        labeles = [classes[int(label)] for label in result.boxes.cls.tolist()]
        points = [
            [resultMaskPoints.tolist() + ["polygon_seg"]]
            for resultMaskPoints in result.masks.xy
        ]
        group_ids = [idx for idx in range(len(labeles))]
        dst_frame = result.plot()
        return ((labeles, points, group_ids), dst_frame)

    elif has_obb:
        classes = result.names
        labeles = [classes[int(label)] for label in result.obb.cls.tolist()]
        points = [
            [resultObbPoints + ["polygon_obb"]]
            for resultObbPoints in result.obb.xyxyxyxy.tolist()
        ]

        group_ids = [idx for idx in range(len(labeles))]
        dst_frame = result.plot()
        return ((labeles, points, group_ids), dst_frame)


def toJson(imagePath, predictMessage):

    jsk_list = []
    jsonPath = f"{imagePath.split('.')[0]}.json"
    for label, points, group_id in zip(*predictMessage):

        # [[p1], [p2], "rectangle"]
        if len(points) == 3:
            jsk_list.append(
                Json_Shape_Keys(
                    label=label,
                    points=points,
                    group_id=group_id,
                )
            )

        # [[[rp1], [rp2], "rectangle"], [[p1], "point"], [[p2], "point"], [[p3] "point"] ...... [[pn], "point"]]
        elif len(points[0]) == 3 and len(points[1]) == 2:
            for idx in range(len(points)):
                jsk_list.append(
                    Json_Shape_Keys(
                        label=label if idx - 1 < 0 else f"p{idx-1}",
                        points=points[idx],
                        group_id=group_id,
                    )
                )

        # [[[p1], [p2], [p3], ...... , [pn], "polygon_seg"]]
        elif points[0][-1] == "polygon_seg":
            jsk_list.append(
                Json_Shape_Keys(
                    label=label,
                    points=points[0],
                    group_id=group_id,
                )
            )

        # [[[p1], [p2], [p3], [p4], "polygon_obb"]]
        elif points[0][-1] == "polygon_obb":
            jsk_list.append(
                Json_Shape_Keys(
                    label=label,
                    points=points[0],
                    group_id=group_id,
                )
            )

    if len(jsk_list) > 0:
        with open(jsonPath, "w") as json_file:
            json.dump(
                Json_Keys(
                    shapes=jsk_list,
                    imagePath=imagePath,
                ).to_json(),
                json_file,
            )
        return True
    
    else:
        return False
