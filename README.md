CommandParser example
======================

This is an example for an Arduino and Mbed OS library for parsing commands of the form `COMMAND_NAME ARG1 ARG2 ARG3 ...`.

Features:

* **No dynamic memory allocation**.
* Compile-time-**configurable resource limits**.
* Strongly typed arguments with **strict input validation** (including integer overflow detection!).
* Friendly **error messages** for invalid inputs (e.g., `parse error: invalid double for arg 2` if the command's second argument cannot be parsed as a double).
* Support for **escape sequences** in string arguments (e.g., `SOME_COMMAND "\"\x41\r\n\t\\"`).

This library works with all Arduino- and Mbed OS-compatible boards.

Please see [README of CommandParser](https://github.com/ATM-HSW/CommandParser/blob/master/README.md)
