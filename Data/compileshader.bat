@REM #change current directory to this file
@%~d0
@cd %~dp0

set comp=..\build\SimpleGameEngine-x64-windows\src\shadercompiler\Debug\sge_shadercompiler.exe


%comp%

@rem pause