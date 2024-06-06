import time

from PyQt5.QtWidgets import QWidget, QHBoxLayout
from qfluentwidgets import TimePicker, PrimaryPushButton

from serial_communication import SerialCommunication
from utility import Utility


class AddAlarmWidget(QWidget):

    def __init__(self, parent=None):
        super().__init__()

        self.parent = parent
        self.alarm_picker = TimePicker(self)
        self.set_alarm_button = PrimaryPushButton('Add Alarm', self)
        self.set_alarm_layout = QHBoxLayout(self)
        self.selected_alarm = None
        self.serial = SerialCommunication.get_instance()

        self.init_ui()
        self.start_communication()

    def init_ui(self):
        self.resize(500, 500)
        self.set_alarm_layout.addWidget(self.set_alarm_button)
        self.set_alarm_layout.addWidget(self.alarm_picker)

    def start_communication(self):
        self.set_alarm_button.clicked.connect(self.set_alarm)
        self.alarm_picker.timeChanged.connect(self.on_time_changed)

    def on_time_changed(self, t):
        self.selected_alarm = t
        print(t.toString())

    def set_alarm(self):
        if self.selected_alarm is not None:
            print("Selected Alarm:", self.selected_alarm.toString())
            formatted_alarm = Utility.remove_colons(self.selected_alarm.toString())
            formatted_alarm = formatted_alarm[:-2]
            print("Formatted Alarm without colons:", formatted_alarm)

            stream = '2' + formatted_alarm
            print("Stream to send:", stream)

            self.serial.open_connection()
            for i in stream:
                self.serial.write_data(i)
                time.sleep(0.2)
            ack = self.serial.read_data_by_bytes(1)
            self.serial.close_connection()
            if ack == '1':
                Utility.createSuccessInfoBar(self.parent, "Success", "Alarm Set Successfully")
            else:
                Utility.createWarningInfoBar(self.parent, "Warning", "Maximum Number of Alarms reached")
        else:
            Utility.createWarningInfoBar(self.parent, "Warning", "Please select an alarm to add.")
