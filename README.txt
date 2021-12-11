READ THIS FIRST
————————————

This folder contains the files necessary to build the sweep generator Arduino code.
This is NOT to be placed directly into the Arduino folder or there will be conflicts and lost data!
Follow the steps below

1. Place the folder “compulign-v2.0” in the folder on your system marked Arduino
2. Go to the Arduino folder named “libraries” under your Arduino folder
3. Take the folders from this data under the folder named “libraries” and place under the folder “libraries” into your Arduino libraries folder. This includes the folder named - AD9850spi, MCP4801spi, rotary2, and TFT9341SPI
4. Open the Arduino development program.
5. Open the project file “compulign-v2.0”
6. From the tools menu, choose board to be Arduino Nano and Processor to be ATmega328
7. Plug the Nano into the mini-USB connector and then to a USB port on the PC. Note which port it is on, e.g. COM5
8. under the tolls menu again, choose port to be the same as the port where the Nano is plugged in.
9. Now compile and download the code to the Arduino Nano by clicking on the big right arrow icon on the Arduino top menu.


If you do not have a full Arduino development environment, you can use the file “compulign-v2.0.ino.eightanaloginputs.hex” and download it with the xloader program which can be dowloaded at http://russemotto.com/xloader/

However, I strongly recommend that you get the full Arduino development environment so you can read and modify code as needed and handle future updates.



