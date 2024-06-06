import datetime
import time

from PyQt5.QtCore import Qt
from qfluentwidgets import InfoBar, InfoBarPosition

from serial_communication import SerialCommunication


class Utility:

    @staticmethod
    def remove_colons(time_str):
        return time_str.replace(':', '')

    @staticmethod
    def send_current_time():
        current_time = str(datetime.datetime.now().time()).split('.')[0]
        stream = Utility.remove_colons(current_time)
        serial = SerialCommunication.get_instance()

        serial.open_connection()
        serial.write_data('1')
        for i in stream:
            serial.write_data(i)
            time.sleep(0.2)
        serial.close_connection()

    @staticmethod
    def createSuccessInfoBar(parent, title, content):
        InfoBar.success(
            title=title,
            content=content,
            orient=Qt.Horizontal,
            isClosable=True,
            position=InfoBarPosition.BOTTOM_LEFT,
            duration=3000,
            parent=parent
        )

    @staticmethod
    def createWarningInfoBar(parent, title, content):
        InfoBar.warning(
            title=title,
            content=content,
            orient=Qt.Horizontal,
            isClosable=False,
            position=InfoBarPosition.BOTTOM_LEFT,
            duration=3000,
            parent=parent
        )

    @staticmethod
    def createErrorInfoBar(parent, title, content):
        InfoBar.error(
            title=title,
            content=content,
            orient=Qt.Horizontal,
            isClosable=True,
            position=InfoBarPosition.BOTTOM_LEFT,
            duration=3000,
            parent=parent
        )
