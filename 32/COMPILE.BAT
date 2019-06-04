call setvars.bat
cd wolf4gw
del *.obj
del *.exe
del *.sym
del *.map
wmake -f wolf.mk
