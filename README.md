# Arduino_Timelogger
A small arduino with four buttons to track the time spent on various projects or working/learning.

Because the Nano V3 isn't capable of posing as a HID, it sends data over Serial instead. A python script on the computer is used to read the serial connection for state changes and logs the current time into an excel sheet accordingly.
