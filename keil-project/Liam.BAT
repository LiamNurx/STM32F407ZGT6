SET PATH=C:\Keil_v5\ARM\ARMCC\Bin;C:\Program Files (x86)\Common Files\Oracle\Java\javapath;C:\MentorGraphics\9.5PADS\SDD_HOME\common\win32\bin;C:\MentorGraphics\9.5PADS\SDD_HOME\common\win32\lib;C:\MentorGraphics\9.5PADS\MGC_HOME.ixn\bin;C:\MentorGraphics\9.5PADS\MGC_HOME.ixn\lib;C:\WINDOWS\system32;C:\WINDOWS;C:\WINDOWS\System32\Wbem;C:\WINDOWS\System32\WindowsPowerShell\v1.0\;C:\WINDOWS\System32\OpenSSH\;C:\Program Files\Intel\WiFi\bin\;C:\Program Files\Common Files\Intel\WirelessCommon\;C:\Program Files\Git\cmd;C:\Program Files (x86)\openocd\0.6.0\bin;C:\Qt\Qt5.9.8\5.9.8\mingw53_32\bin;C:\Program Files (x86)\Microsoft SQL Server\100\Tools\Binn\;C:\Program Files\Microsoft SQL Server\100\Tools\Binn\;C:\Program Files\Microsoft SQL Server\100\DTS\Binn\;C:\Program Files\Java\jdk1.8.0_231\bin;C:\Program Files\Java\jdk1.8.0_231\jre\bin;C:\Program Files (x86)\yagarto-tools-20100703\bin;C:\Program Files (x86)\yagarto\bin;C:\MinGW\bin;C:\Keil_v5\ARM\ARMCC\bin;C:\Users\zhengchaozhu\AppData\Local\Microsoft\WindowsApps;C:\MentorGraphics\9.5PADS\SDD_HOME\common\win32\lib;C:\MentorGraphics\9.5PADS\SDD_HOME\CAMCAD;C:\Users\zhengchaozhu\AppData\Local\GitHubDesktop\bin;C:\Program Files (x86)\openocd\0.6.0\bin;C:\Users\zhengchaozhu\AppData\Local\Microsoft\WindowsApps;C:\Users\zhengchaozhu\AppData\Local\Pandoc\;
SET CPU_TYPE=STM32F407ZGTx
SET CPU_VENDOR=STMicroelectronics
SET UV2_TARGET=Liam
SET CPU_CLOCK=0x00B71B00
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\main.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\sysconfig.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\usart.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\exti.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\timer.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\system_stm32f4xx.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\stm32f4xx_it.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmAsm" --Via ".\objects\startup_stm32f40_41xxx._ia"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\misc.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\stm32f4xx_exti.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\stm32f4xx_gpio.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\stm32f4xx_rcc.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\stm32f4xx_usart.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\stm32f4xx_syscfg.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmLink" --Via ".\Objects\STM32F407.lnp"
"C:\Keil_v5\ARM\ARMCC\Bin\fromelf.exe" ".\Objects\STM32F407.axf" --i32combined --output ".\Objects\STM32F407.hex"
C:\Keil_v5\ARM\ARMCC\bin\fromelf.exe --bin --output .\Objects\STM32F407.bin .\Objects\STM32F407.axf
.\CleanObjFiles.bat
