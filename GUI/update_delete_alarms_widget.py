import time

from PyQt5.QtWidgets import QTableWidgetItem, QWidget, QHeaderView, QVBoxLayout, QHBoxLayout
from qfluentwidgets import TableWidget, PrimaryPushButton

from serial_communication import SerialCommunication
from utility import Utility


class UpdateDeleteAlarmsWidget(QWidget):

    def __init__(self, parent=None):
        super().__init__()

        self.parent = parent
        self.selected_alarm = None
        self.alarms_table = TableWidget(self)
        self.update_alarms_button = PrimaryPushButton("Update Alarms", self)
        self.delete_alarm_button = PrimaryPushButton("Delete Selected Alarm", self)
        self.show_alarms_layout = QVBoxLayout(self)
        self.buttons_layout = QHBoxLayout(self)

        # self.dummy_data = '2244FFFF05500909'
        self.alarms_text = None
        self.serial = SerialCommunication.get_instance()

        self.init_ui()
        self.start_communication()

    def start_communication(self):
        self.update_alarms_button.clicked.connect(self.update_table_with_alarms)
        self.delete_alarm_button.clicked.connect(self.delete_selected_alarm)

    def init_ui(self):
        self.alarms_table.setBorderVisible(True)
        self.alarms_table.setBorderRadius(8)
        self.alarms_table.setWordWrap(False)
        self.alarms_table.setRowCount(4)
        self.alarms_table.setColumnCount(2)
        self.alarms_table.verticalHeader().hide()
        self.alarms_table.setHorizontalHeaderLabels(['Number', 'Alarm'])
        self.alarms_table.resizeColumnsToContents()
        self.alarms_table.horizontalHeader().setSectionResizeMode(QHeaderView.Stretch)
        self.alarms_table.setSortingEnabled(True)
        self.show_alarms_layout.setContentsMargins(50, 30, 50, 30)
        self.buttons_layout.addWidget(self.update_alarms_button)
        self.buttons_layout.addWidget(self.delete_alarm_button)
        self.show_alarms_layout.addWidget(self.alarms_table)
        self.show_alarms_layout.addLayout(self.buttons_layout)

        self.resize(500, 500)

    def update_table_with_alarms(self):
        self.serial.open_connection()
        self.serial.write_data('4')
        self.alarms_text = self.serial.read_data_by_bytes(16)
        self.serial.close_connection()

        self.alarms_table.setRowCount(0)

        if self.alarms_text:
            alarm_chunks = [self.alarms_text[i:i + 4] for i in range(0, min(16, len(self.alarms_text)), 4)]

            row_count = min(len(alarm_chunks), 4)
            self.alarms_table.setRowCount(row_count)

            for i, alarm in enumerate(alarm_chunks[:4]):
                if len(alarm) >= 4:
                    hours = alarm[0:2]
                    minutes = alarm[2:4]

                    if len(alarm) > 4 and alarm[4] == 'F':
                        alarm_time = 'null alarm'
                    else:
                        alarm_time = f"{hours}:{minutes}"

                    self.alarms_table.setItem(i, 0, QTableWidgetItem(str(i + 1)))
                    self.alarms_table.setItem(i, 1, QTableWidgetItem(alarm_time))
            Utility.createSuccessInfoBar(self.parent, "Success", "Alarms Table Updated Successfully")
        else:
            Utility.createWarningInfoBar(self.parent, "Warning", "There is No Alarms")

    def delete_selected_alarm(self):
        selected_rows = self.alarms_table.selectionModel().selectedRows()

        if selected_rows:
            for row in selected_rows:
                row_index = row.row() + 1
                alarm_item = self.alarms_table.item(row_index, 1)
                if alarm_item and alarm_item.text() and alarm_item.text() != "null alarm":
                    print(f"Selected Row Index: {row_index}, Alarm Data: {alarm_item.text()}")
                    stream = '3' + str(row_index)
                    print('Stream To send:', stream)
                    self.serial.open_connection()
                    self.serial.write_data(stream)
                    self.serial.close_connection()
                    Utility.createSuccessInfoBar(self.parent, "Success", "Selected alarm is deleted successfully")
                else:
                    Utility.createWarningInfoBar(self.parent, "Warning", "Selected alarm is empty")
        else:
            Utility.createWarningInfoBar(self.parent, "Warning", "Please select an alarm to delete")
