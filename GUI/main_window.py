import sys

from PyQt5.QtCore import Qt
from PyQt5.QtWidgets import QApplication, QMainWindow, QWidget, QVBoxLayout

from add_alarm_widget import AddAlarmWidget
from set_clock_widget import SetClockWidget
from update_delete_alarms_widget import UpdateDeleteAlarmsWidget
from utility import Utility


class MainWindow(QMainWindow):
    def __init__(self):
        super(MainWindow, self).__init__()

        self.set_clock_widget = SetClockWidget(self)
        self.add_alarm_widget = AddAlarmWidget(self)
        self.update_delete_alarms_widget = UpdateDeleteAlarmsWidget(self)
        Utility.send_current_time()
        self.initUI()

    def initUI(self):
        self.setWindowTitle('Real Time Clock and Alarm')
        self.setGeometry(350, 150, 600, 500)

        main_widget = QWidget(self)
        self.setCentralWidget(main_widget)
        layout = QVBoxLayout(main_widget)
        layout.addWidget(self.set_clock_widget)
        layout.addWidget(self.add_alarm_widget)
        layout.addWidget(self.update_delete_alarms_widget)


if __name__ == '__main__':
    QApplication.setHighDpiScaleFactorRoundingPolicy(
        Qt.HighDpiScaleFactorRoundingPolicy.PassThrough)
    QApplication.setAttribute(Qt.AA_EnableHighDpiScaling)
    QApplication.setAttribute(Qt.AA_UseHighDpiPixmaps)
    app = QApplication(sys.argv)
    timer_app = MainWindow()
    timer_app.show()
    sys.exit(app.exec_())
