echo off
rem +++++++++ config ++++++++++
rem +++++++++++++++++++++++++++

set mbedos=mbed-os-6.9.0

set platform=NUCLEO_F446RE
rem set platform=AUTO

set ide=uvision6
rem set ide=eclipse_gcc_arm
rem set ide=sw4stm32
rem set ide=gcc_arm

rem +++++++++++++++++++++++++++
rem +++++++ config ende +++++++

for %%I in (.) do (
   set projectpath=%%~dpI
   set prj=%%~nxI
)

cd ..

echo:"Mbed OS:   "%mbedos%
echo:"Plattform: "%platform%
echo:"IDE:       "%ide%

mbed config -G MBED_OS_DIR %projectpath%\%mbedos%
mbed export -v -m %platform% -i %ide% --source .\%prj% --source .\%mbedos%

cd .\%prj%
pause 