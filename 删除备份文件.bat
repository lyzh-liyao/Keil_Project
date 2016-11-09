for /d /r %%i in (*output*) do del /s /q "%%i\*.*"
for /d /r %%i in (*Project*) do del /s /q "%%i\*.o" del /s /q "%%i\*.bak" del /s /q "%%i\*.crf"
pause