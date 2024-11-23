import time
import serial

def main():
    """
    Main function
    """

    com_port = "COM4"
    baud_rate = 9600

    ser = serial.Serial(com_port, baud_rate, timeout=1)

    time.sleep(2)

    command = ""

    with open("./boba_files/45.boba", "r", encoding="utf-8") as r_file:
        command = r_file.read().strip()

    print(f"Sending command: {command}")
    ser.write(command.encode())
    ser.close()

if __name__ == "__main__":
    main()
