@if not defined INCLUDE goto :FAIL

@setlocal

@set LUA_INCLUDE=D:\develpop\lua\lua-5.2.4\src
@set LUA_LIB=D:\develop\lua\lua-5.2.4\build\lua52_MT.lib
@set LUA_EXE=D:\develop\lua\lua52.exe


:RELEASE
@set DO_CL=cl.exe /nologo /c /MT /O2 /Oi /Gy /GL /W3 /Zi /WX /D_CRT_SECURE_NO_DEPRECATE /DLUNUM_API_NOCOMPLEX  /TP
@set DO_LINK=link.exe /nologo /debug /LTCG /DEF:lunum.def
@set DO_MT=mt.exe /nologo
@goto :COMPILE


:COMPILE
"%LUA_EXE%" bin2c.lua array_class.lua > array_class.lc
%DO_CL% /I"." /I"%LUA_INCLUDE%" numarray.c lunum_capi.c array_class.c lunum.c  
%DO_LINK% /DLL /OUT:lunum.dll "%LUA_LIB%" *.obj


:CLEAN_OBJ
del *.obj *.manifest
@goto :END

:FAIL
@echo You must open a "Visual Studio Command Prompt" to run this script
:END

