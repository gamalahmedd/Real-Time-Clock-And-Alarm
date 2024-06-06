import time

from PyQt5.QtWidgets import QWidget, QHBoxLayout
from qfluentwidgets import TimePicker, PrimaryPushButton

from serial_communication import SerialCommunication
from utility import Utility


class SetClockWidget(QWidget):

    def __init__(self, parent=None):
        self.parent = parent
        super().__init__()

        self.clock_picker = TimePicker(self, showSeconds=True)
        self.set_clock_button = PrimaryPushButton('Set Clock', self)
        self.set_clock_layout = QHBoxLayout(self)
        self.selected_clock = None
        self.stateTooltip = None
        self.serial = SerialCommunication.get_instance()

        self.init_ui()
        self.start_communication()

    def init_ui(self):
        self.resize(500, 500)
        self.set_clock_layout.addWidget(self.set_clock_button)
        self.set_clock_layout.addWidget(self.clock_picker)

    def start_communication(self):
        self.set_clock_button.clicked.connect(self.set_clock)
        self.clock_picker.timeChanged.connect(self.on_time_changed)

    def on_time_changed(self, t):
        self.selected_clock = t
        print(t.toString())

    def set_clock(self):
        if self.selected_clock is not None:
            print("Selected time:", self.selected_clock.toString())
            formatted_time = Utility.remove_colons(self.selected_clock.toString())
            print("Formatted time without colons:", formatted_time)

            stream = '1' + formatted_time
            print("Stream to send:", stream)
            self.serial.open_connection()
            for i in stream:
                self.serial.write_data(i)
                time.sleep(0.2)
            self.serial.close_connection()
            Utility.createSuccessInfoBar(self.parent, "Success", "Clock Set Successfully")
        else:
            Utility.createWarningInfoBar(self.parent, "Warning", "Please select a clock to set.")
