@echo off

IF %1.==. GOTO ParameterError
IF %2.==. GOTO ParameterError

set PROJECT_LC=project
set PROJECT_CAPS=PROJECT

echo Generating %PROJECT_LC%_build_info_%2.h...

set build_file_name=%1\%PROJECT_LC%_build_info_%2.h

for /f "tokens=2 delims==" %%I in ('wmic os get localdatetime /format:list') do set datetime=%%I

echo //============================================================= >%build_file_name%
echo //===== Auto Generated Project Build File - Do NOT Modify ===== >>%build_file_name%
echo //============================================================= >>%build_file_name%
echo. >>%build_file_name%

IF "%2" == "COIDE" (
    echo #ifdef __GNUC__    // GNU C Compiler >>%build_file_name%
) 
IF "%2" == "IAR" (
    echo #ifdef __ICCARM__  // IAR C Compiler >>%build_file_name%
)
IF "%2" == "KEIL" (
    echo #ifdef __CC_ARM    // Keil RealView C Compiler >>%build_file_name%
)

echo. >>%build_file_name%
echo // %2 %PROJECT_CAPS% Library Build-%datetime:~0,8%.%datetime:~8,6% >>%build_file_name%
echo. >>%build_file_name%
echo #ifndef %PROJECT_CAPS%_BUILD_INFO_H >>%build_file_name%
echo #define %PROJECT_CAPS%_BUILD_INFO_H >>%build_file_name%
echo. >>%build_file_name%
echo #define %PROJECT_CAPS%_BUILD_STR   		"%datetime:~0,8%.%datetime:~8,6%" >>%build_file_name%
echo #define %PROJECT_CAPS%_BUILD_DATE_STR		"%datetime:~0,8%" >>%build_file_name%
echo #define %PROJECT_CAPS%_BUILD_DATE_NUM		%datetime:~0,8% >>%build_file_name%
echo #define %PROJECT_CAPS%_BUILD_TIME_STR		"%datetime:~8,6%" >>%build_file_name%
echo #define %PROJECT_CAPS%_BUILD_TIME_NUM		%datetime:~8,6% >>%build_file_name%

echo. >>%build_file_name%
echo #endif // %PROJECT_CAPS%_BUILD_INFO_H >>%build_file_name%
echo. >>%build_file_name%

IF "%2" == "COIDE" (
    echo #endif __GNUC__    // GNU C Compiler >>%build_file_name%
 )
IF "%2" == "IAR" (
    echo #endif __ICCARM__  // IAR C Compiler >>%build_file_name%
)
IF "%2" == "KEIL" (
    echo #endif __CC_ARM    // Keil RealView C Compiler >>%build_file_name%
)

rem Normal Exit
exit

:ParameterError
echo !!!!! ERROR: 1st parameter must be the path to the project directory
echo !!!!! ERROR: 2nd parameter must be one of {COIDE, IAR, KEIL}
exit 1
