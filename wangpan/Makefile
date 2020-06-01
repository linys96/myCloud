H_DIR:=/usr/include/mysql #额外增加的头文件目录
LIB:=-lmysqlclient -pthread -lcrypt#此处填写要加入的库
#开头两行可以自己改动，井号为注释
OBJS:=$(wildcard *.c)
ifneq ($(OBJS), )
	CMAIN:=$(shell grep -hlE ^\\w*\\smain\\W $(OBJS))
endif
ifeq ($(OUTPUT), )
	OUTPUT_PATH:=#../bin #可执行文件存放目录
else
	OUTPUT_PATH:=$(OUTPUT)/
endif
ELF:=$(patsubst %.c,%,$(CMAIN))
SUBELF:=$(filter-out $(CMAIN),$(OBJS))
#如果没有不带main函数的.c文件
ifeq ($(SUBELF), )
	SUBELF:=x
endif
H_DIR:=$(patsubst %,-I %,$(H_DIR))
CC:=gcc# 编译器 
ifeq ($(NODEBUG_SYMBOLS), TRUE)
	CFLAGS:=-Wall
else
	CFLAGS:=-g -Wall -D DEBUG
endif
TEMP_LOG:=$(patsubst %.c,%.tmp,$(CMAIN))
TEMP:=
all:$(ELF)

%:%.c
	@tmp=$(shell make tmp_log TEMP=$@.tmp)
	-$(CC) $(CFLAGS) -o $@ $@.c $(filter $(shell grep -lE "`cat $@.tmp`" $(SUBELF) 2>/dev/null),$(SUBELF)) $(LIB) $(H_DIR)
	@rm -rf $@.tmp
	@if [ x$(OUTPUT_PATH) != x ];then mv $@ $(OUTPUT_PATH);fi

%.tmp:%.c
	@grep -hE "^.include\\s[\"\']" $^|sed "s/.include [\"\']//g"|sed "s/[\"\']//g" > $@
	@echo -n > head_dir.tmp; var=`cat $@|head -n 1`; [ x"`echo $${var%/*}`" = x"$$var" ] || (echo $${var%/*}/ > head_dir.tmp; sed -i "s/.*\///g" $@)
	@head_dir=`cat head_dir.tmp`; if [ x"`cat $@`" != x ];then tag=1;while [ $$tag = 1 ];do grep -hE "^.include\\s[\"\']" `cat $@|xargs -i echo $$head_dir{}" "`|sed "s/.include [\"\']//g"|sed "s/[\"\']//g" > $@0; tag=0; for k in `cat $@0`; do cat $@|grep -wq $$k || (echo $$k >> $@ && tag=1);done;done;fi
	@head_dir=`cat head_dir.tmp`; if [ x"`cat $@`" != x ];then for i in `cat $@|xargs -i echo $$head_dir{}" "`;do grep -ohE "^\w+[\s\*]{1,2}[_a-zA-Z][_a-zA-Z0-9]*\(" $$i|sed 's/(/\\(/g'; done > $@0 ;fi
	@grep -ohE "^\w+[\s\*]{1,2}[_a-zA-Z][_a-zA-Z0-9]*\(" $^|sed 's/(/\\(/g' >> $@0
	@cat $@0|tr '\n' '|'|sed 's/.$$//' > $@
	@rm -rf $@0 head_dir.tmp

.PHONY:proj clean rebuild nodebug help tmp_log tmpp tmp
tmpp:
	echo "../include/" > ttt.t0 ; echo thread_pool.h > ttt.t
	head_dir=`cat ttt.t0`; if [ x"`cat ttt.t`" != x ];then tag=1;while [ $$tag = 1 ];do grep -hE "^.include\\s[\"\']" `cat ttt.t|xargs -i echo $$head_dir{}" "`|sed "s/.include [\"\']//g"|sed "s/[\"\']//g" > ttt.t0; tag=0; for k in `cat ttt.t0`; do cat ttt.t|grep -wq $$k || (echo $$k >> ttt.t && tag=1);done;done;fi

tmp_log:$(TEMP)

proj:
	-@[ x$(PROJ) != x ] && mkdir $(PROJ) || [ 1 ]
	-@mkdir ./$(PROJ)/src ./$(PROJ)/bin ./$(PROJ)/include ./$(PROJ)/conf
	@[ x$(PROJ) != x ] && tmp_dir="/.." || [ 1 ];ln -s ..$$tmp_dir/Makefile ./$(PROJ)/scr/Makefile

clean:
	@rm -rf $(OUTPUT_PATH)$(ELF) || [ 1 ]

rebuild:clean all

nodebug:clean
	@make --no-print-directory NODEBUG_SYMBOLS=TRUE #OUTPUT=$(OUTPUT_PATH)

help:
	@echo "================= Makefile for c programs ===================="
	@echo "注意：非标准头文件需要在同一个文件夹，不过可以与c文件分开"
	@echo
	@echo " all             -- (等同于make) 编译并链接, 默认加调试"
	@echo " OUTPUT=path     -- 指定输出至path目录, 可与其它参数一起使用"
	@echo " proj            -- 在(当前/项目)目录创建scr、bin、include、conf目录"
	@echo "                    proj PROJ=name 可指定项目名称为name"
	@echo "                    如：make proj PROJ=test"
	@echo "                    ."
	@echo "                    ├── Makefile"
	@echo "                    └── \e[94mtest\e[0m"
	@echo "                        ├── \e[94mbin\e[0m"
	@echo "                        ├── \e[94mconf\e[0m"
	@echo "                        ├── \e[94minclude\e[0m"
	@echo "                        └── \e[94mscr\e[0m"
	@echo "                            └── \e[96mMakefile\e[0m -> ../../Makefile"
	@echo " clean           -- 清除编译后的内容"
	@echo " rebuild         -- 重新编译"
	@echo " nodebug         -- 不带调试进行编译"
	@echo " help            -- 查看帮助"
	@echo
	@echo "======================== ZHUZHU 4.0 =========================="
