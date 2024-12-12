# pok_project_robot

![image](https://github.com/user-attachments/assets/b0f67556-abd3-45e7-b23a-7ea80a87edb0)


https://www.overleaf.com/1926856414zdhfzghrxpwq

### 1
algorithms: you can see our move algorythm you can translate them via UART to Arduino
or write on SD card and use esp-idf.

### 2
arduino_project: there located parser on Arduino(parser.ino) framework, app to write and launch these files
on our robot and basic walk (chaplawalk.ino). If you want to change some upload properties you need to change
in writer.py port and braud rate.

### 3
work_with_rob_file: there you can observe decoder for rob files. You can try to use them for robot walk (But be careful,
because your robot might have different properties);

### 4
esp32s3: there is firmware for esp32-s3, pca9685 and sd card. There is a parser which can process strings to move servo motors on pca.
This parser read file ('fl.ns') with names of files with algorithms. You need to create file and write its name in 'fl.ns'. Syntax of parser
is described in ou overleaf
