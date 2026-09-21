# WindowsNUI

**WindowsNUI** is a small Windows internals research project that demonstrates how suspending the threads of a user-mode process can make its graphical interface unresponsive without necessarily terminating the process itself.

The project uses native Windows APIs to:

* Enumerate running processes
* Locate processes by executable name
* Enumerate threads belonging to a process
* Suspend individual process threads
* Launch Windows Security components
* Demonstrate the effect of suspending the relevant user-mode process

The name **WindowsNUI** is a play on *Windows No UI*, referring to the resulting behavior when the Windows Security interface becomes unresponsive.

## How it works

WindowsNUI uses the Windows Tool Help API to enumerate processes and threads. Once the target process is identified, its threads are opened with `THREAD_SUSPEND_RESUME` access and suspended using `SuspendThread()`.

The project also includes a `selfc` demonstration mode that locates the relevant Windows Security process and attempts to demonstrate what happens when its user interface process is suspended.

## Purpose

This project was created for learning and experimentation with:

* Windows process and thread management
* Win32 API programming
* Process enumeration
* Thread enumeration
* Process suspension
* Windows user-mode architecture
* Understanding the separation between a graphical interface and underlying system functionality

**WindowsNUI is a research/educational project and is not intended to disable or bypass Windows security protections.** Suspending a user-interface process should not be interpreted as disabling the underlying security mechanisms themselves.

## Requirements

* Windows
* C++ compiler
* Windows SDK
* Administrator privileges may be required for some target processes

## Disclaimer

This project is intended for educational purposes and controlled experimentation on systems you own or are authorized to test. Behavior may vary between Windows versions and configurations.
