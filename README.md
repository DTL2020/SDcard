# SDcard
Command line application to send SD card commands and display reply as 16bytes sequence (typical reply to CMD9, 10, 13).
For CMD9 and CMD13 known to work only with Realtek-based USB cardreaders. For reading CID with CMD10 the special patched driver required 
(for example replacing processing of CMD9 with CMD10).

Usage: sdcard _drive_letter_ command

Command as decimal number.
Example: sdcard E 10
