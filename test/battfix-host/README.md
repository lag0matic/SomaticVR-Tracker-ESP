# Host regression tests

Compile the production latch helper and PD driver with mocked GPIO/I2C interfaces. From a Visual Studio x64 developer command prompt:

```bat
cl /nologo /EHsc /std:c++17 /Itest\battfix-host test\battfix-host\battfix_test.cpp /Fe:.pio\battfix-host-tests.exe /Fo:.pio\battfix-host-tests.obj
.pio\battfix-host-tests.exe
```

Keep assertions enabled. Covers cold start, button start, software/watchdog/USB resets, deliberate shutdown, latch transition order, unstable CC readings, I2C failures and subsequent PD recovery. These checks cannot validate physical power continuity or actual USB/OTA reset behavior.
