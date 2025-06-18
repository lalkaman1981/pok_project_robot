# Humanoid Robot

![image](https://github.com/user-attachments/assets/b0f67556-abd3-45e7-b23a-7ea80a87edb0)

The detailed report for this project is available in the `Report.pdf` file and can also be accessed online via the following Overleaf link:
[https://www.overleaf.com/project/6706b8493410a3e9d164b8d3](https://www.overleaf.com/project/6706b8493410a3e9d164b8d3)

### 1. Algorithms

This directory contains the movement algorithms used by the robot. These algorithms can be transmitted to the Arduino via UART or written to an SD card and executed using the ESP-IDF framework.

### 2. Arduino Project

This folder contains:

* `parser.ino`: An Arduino-based parser for executing movement sequences.
* `chaplawalk.ino`: A basic walking routine for the robot.
* `writer.py`: A Python utility for uploading movement files to the robot.

> **Note:** To modify upload settings such as the port or baud rate, edit the corresponding parameters in `writer.py`.

### 3. Work with .GS Files

This directory provides tools for decoding `.gs` files. These files can be used to command the robot’s movements.

> **Caution:** Ensure that the robot's physical configuration matches the expected properties in the `.gs` files, as discrepancies may lead to incorrect or unsafe behavior.

### 4. ESP32-S3

This folder contains firmware for the ESP32-S3, including support for the PCA9685 servo driver and SD card functionality. It includes:

* A parser that processes command strings to control servo motors via the PCA9685.
* Support for reading a file named `fl.ns`, which contains the filenames of the movement algorithms to execute.

> The syntax and structure of this parser are described in the project report available on Overleaf.

---

**Final Presentation:**
[https://www.canva.com/design/DAGY-ikruao/bg3jPskFeTuVDdUXPB4FtA/edit](https://www.canva.com/design/DAGY-ikruao/bg3jPskFeTuVDdUXPB4FtA/edit)

---
