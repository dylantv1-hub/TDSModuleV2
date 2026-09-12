# TDSModuleV2
IEEE x AlChE Project Kit Collab

The following code will allow a ESP32-C3 microcontroller to read data from a temperature and total-dissolved-solids proble and display the data on a liquid glass display.

Important to note:
1. TDSModuleV2.ino is the main code that will run on Arduino IDE
2. KeyestudioTDS.cpp and KeyestudioTDS.h are custom library files that need to be downloaded when the user calls "#include Keyestudio.h". This custom library abstracts the mathematical computations so the main code is easier to work with.
