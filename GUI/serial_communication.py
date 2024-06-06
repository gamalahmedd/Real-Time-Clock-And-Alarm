import serial
import time
from serial.serialutil import SerialException


class SerialCommunication:

    _instance = None

    @classmethod
    def get_instance(cls):
        if not cls._instance:
            cls._instance = cls()
        return cls._instance

    def __init__(self, port='COM2', baudrate=9600):
        super(SerialCommunication, self).__init__()

        if not SerialCommunication._instance:
            self.port = port
            self.baudrate = baudrate
            self.serial = None
            SerialCommunication._instance = self
        else:
            raise Exception("An instance of SerialCommunication already exists.")

    def open_connection(self):
        try:
            self.serial = serial.Serial(self.port, self.baudrate)
        except SerialException as e:
            print(f"Error opening serial connection: {e}")

    def close_connection(self):
        if self.serial and self.serial.is_open:
            self.serial.close()
            print("Serial connection closed.")

    def write_data(self, data):
        if self.serial and self.serial.is_open:
            try:
                for ch in data:
                    self.serial.write(ch.encode())
                    time.sleep(0.25)
                print(f"Sent: {data}")
            except SerialException as e:
                print(f"Error writing data: {e}")

    def read_data_by_bytes(self, num_bytes):
        if self.serial and self.serial.is_open:
            try:
                data = self.serial.read(num_bytes).decode()
                print(f"Received: {data}")
                return data
            except SerialException as e:
                print(f"Error reading data: {e}")
        return None

    def read_all_data(self):
        if self.serial and self.serial.is_open:
            try:
                data = self.serial.readlines().decode('utf-8')
                print(f"Received: {data}")
                return data
            except SerialException as e:
                print(f"Error reading data: {e}")
        return None
