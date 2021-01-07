############################################################################################################################################################################
#FileName:			Makefile
#Author:			Liam.Nurx
#Date:				2020.11.14
#
#Description:
#	1.此 Makefile 基于 Keil 自带的编译工具链 ARMCC (armcc, armar, armasm, armlink, fromelf);
#	2.在安装 Keil 软件之后，将其编译工具链所在路径添加至系统环境变量即可正常使用。工具链路径: C:\Keil_v5\ARM\ARMCC\bin
#	3.受限于 Windows 的系统环境及编译工具链现有的功能，此 Makefile 的设计不支持 make 的嵌套调用；
#	4.此 Makefile 支持多层目录的文件编译，在各级目录新增内容时，需及时更新对应的路径及目标文件变量的内容；
#	5.此 Makefile 中所涉及的命令选项皆参考自工具链对应的用户指导手册(User Guide Manual)，手册路径：C:\Keil_v5\ARM\Hlp
#	6.此 Makefile 在工程维护的过程中，请务必遵守相关指令及变量定义部分的注释说明进行操作；
#	7.以下“功能应用层”，“内核访问层”，“片上外设层” 纯属个人杜撰以区别不同的功能层次，仅供参考。
#
#ModifyLog:
#	//	2020.11.15
#	1.新增头文件依赖自动推倒功能；				
#	Note:
#		armcc 指令选项新增 "CCFLAGS += --md"		
#		相比于 GCC 的 “-MD”, ARMCC 的 "--md" 不支持指定依赖文件的输出路径及文件名。每个依赖输出文件输出在 Makefile 所在目录，与对应的输入同名且以 ".d" 为后缀。
#		编译过程中产生的 ".d" 文件会被立即拷贝至依赖文件目录 "./incdef"，在整个编译过程结束后再将当前目录下的 ".d" 文件一并删除。
############################################################################################################################################################################

#	Note:
#	在使用 .bat 文件执行 Make 时，若出现类似如下的异常:
#	"make: Interrupt/Exception caught (code = 0xc00000fd, addr = 0x425073)"
#	则需要使用如下声明：
SHELL=C:/Windows/System32/cmd.exe
#	End of Note!

ARMCC = armcc
ARMAR = armar
ARMASM = armasm
ARMLINK = armlink
FROMELF = fromelf

#	定义目标文件前缀
TARGET = STM32F407

#	定义目标文件输出路径
TARPATH = ./objects

#	定义头文件依赖输出的路径
INCDEFPATH = ./incdef

#	库文件路径
PATHLIB += C:\Keil_v5\ARM\ARMCC\lib

#	定义工程头文件路径变量，当新增功能模块时，路径在尾部添加增长
PATHINC += -I ./app
PATHINC += -I ./cmsis
PATHINC += -I ./cmsis/inc
PATHINC += 

#########################################
####	非必要，请暂勿更改此区域	#####
############################################################################################################################################################################
INFOFLAGS += --info sizes 
INFOFLAGS += --info totals 
INFOFLAGS += --info unused 
INFOFLAGS += --info veneers
INFOFLAGS += --info summarysizes

MAPFLAGS += --autoat
MAPFLAGS += --summary_stderr
MAPFLAGS += --xref
MAPFLAGS += --callgraph
MAPFLAGS += --symbols
MAPFLAGS += --map

CCFLAGS += -c
CCFLAGS += --cpu Cortex-M4
CCFLAGS += -g
CCFLAGS += -O0
CCFLAGS += --apcs=interwork
CCFLAGS += --split_sections
CCFLAGS += --md

ASMFLAGS += --cpu Cortex-M4
ASMFLAGS += -g
ASMFLAGS += --apcs=interwork

LINKFLAGS += --cpu Cortex-M4
LINKFLAGS += --strict
LINKFLAGS += --libpath $(PATHLIB)
LINKFLAGS += --scatter $(TARPATH)/$(TARGET).sct
############################################################################################################################################################################
####	非必要，请暂勿更改此区域	#####
#########################################

#	“内核访问层” 目标文件变量。此部分无新增必要，需说明的是，汇编启动目标文件必须在任意时刻均位于目标文件变量的起首位置！！！！！！
CMSISOBJS += ./cmsis/startup_stm32f40_41xxx.o
CMSISOBJS += ./cmsis/system_stm32f4xx.o
CMSISOBJS += ./cmsis/stm32f4xx_it.o

#	"片上外设层" 目标文件变量。新增使用外设时，在尾部添加即可。
CMSISSRCOBJS += ./cmsis/src/misc.o
CMSISSRCOBJS += ./cmsis/src/stm32f4xx_exti.o
CMSISSRCOBJS += ./cmsis/src/stm32f4xx_gpio.o
CMSISSRCOBJS += ./cmsis/src/stm32f4xx_rcc.o
CMSISSRCOBJS += ./cmsis/src/stm32f4xx_usart.o
CMSISSRCOBJS += ./cmsis/src/stm32f4xx_syscfg.o
CMSISSRCOBJS += ./cmsis/src/stm32f4xx_iwdg.o
#CMSISSRCOBJS += ./cmsis/src/
CMSISSRCOBJS += 

#	“功能应用层” 目标文件变量。新增模块功能时，在尾部添加即可。
APPOBJS += ./app/main.o
APPOBJS += ./app/app_sysconfig.o
APPOBJS += ./app/app_usart.o
APPOBJS += ./app/app_exti.o
APPOBJS += ./app/app_timer.o

#	汇总目标依赖文件，有新增内容时，在尾部增加即可。
#	务必保持 $(CMSISOBJS) 在变量的起首位置，因为汇编启动目标文件必须在任意时刻均位于目标依赖文件变量的起首位置！！！！！！
ALLOBJS += $(CMSISOBJS)
ALLOBJS += $(CMSISSRCOBJS)
ALLOBJS += $(APPOBJS)
ALLOBJS += 

#	Keil 下的 armcc 能通过指令选项 "-md" 生成目标文件 “obj.o” 对应的依赖文件 "obj.d" ，但不支持指定输出文件路径及文件名
#	故而，整个工程中的 ".c" 所对应的依赖文件 ".d" 都会被输出在 Makefile 文件的当前目录下，在整个工程编译完成后，应直接删除；
DEFFILES = $(wildcard $(INCDEFPATH)/*.d)

$(TARGET): $(ALLOBJS)
	$(ARMLINK) $(LINKFLAGS) -o $(TARPATH)/$(TARGET).axf $^
	$(ARMLINK) $(LINKFLAGS) $(INFOFLAGS) $(MAPFLAGS) --list $(TARPATH)/$(TARGET).map $^
	$(FROMELF) --i32 -o $(TARPATH)/$(TARGET).hex $(TARPATH)/$(TARGET).axf
	$(FROMELF) --bin -o $(TARPATH)/$(TARGET).bin $(TARPATH)/$(TARGET).axf
	$(FROMELF) --text -a -c --output $(TARPATH)/$(TARGET).dis $(TARPATH)/$(TARGET).axf
	rm -rf ./*.axf 
	rm -rf ./*.htm
	rm -rf ./*.d

#	依赖文件的引入位置应该在链接处理和编译处理之间，若摆放在其他位置，可能会有问题；
ifneq ($(DEFFILES),)
-include $(DEFFILES)
endif

%.o : %.c
	$(ARMCC) $(CCFLAGS) $(PATHINC) $< -o $@
	cp ./*.d $(INCDEFPATH)/

%.o : %.s
	$(ARMASM) $(ASMFLAGS) $(PATHINC) $< -o $@

.PHONY : clean

clean:
	rm -rf ./*.d
	rm -rf $(ALLOBJS)
	rm -rf $(DEFFILES)
	rm -rf $(TARPATH)/*.axf 
	rm -rf $(TARPATH)/*.bin 
	rm -rf $(TARPATH)/*.hex 
	rm -rf $(TARPATH)/*.htm 
	rm -rf $(TARPATH)/*.map
	rm -rf $(TARPATH)/*.dis


