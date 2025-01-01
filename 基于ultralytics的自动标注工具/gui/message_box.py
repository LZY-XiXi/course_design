from PyQt5.QtWidgets import QMessageBox


def finish_information(widget, box_text="已完成"):
    reply = QMessageBox(widget)
    reply.setIcon(QMessageBox.Information)
    reply.setWindowTitle("提示")
    reply.setText(box_text)
    reply.addButton("确定", QMessageBox.YesRole)
    reply.show()
    reply.exec_()


def dir_warning(widget, box_text="未选择文件目录"):
    reply = QMessageBox(widget)
    reply.setIcon(QMessageBox.Warning)
    reply.setWindowTitle("警告")
    reply.setText(box_text)
    reply.addButton("确定", QMessageBox.YesRole)
    reply.show()
    reply.exec_()


def unknow_error(widget, box_text="未知错误"):
    reply = QMessageBox(widget)
    reply.setIcon(QMessageBox.Critical)
    reply.setWindowTitle("错误")
    reply.setText(box_text)
    reply.addButton("确定", QMessageBox.YesRole)
    reply.show()
    reply.exec_()
