# define keywords
RM = rm -rf
KDIR ?= /lib/modules/$(shell uname -r)/build
BUILD_DIR ?= $(PWD)/build/ # Put all generated files in folder build
INCLUDE_DIRS = -I$(src)/include 
LKM_NAME ?= lkm-name

# Project, create object
obj-m += lkm-name.o
lkm-name-objs += src/main.o

# Specify the target executable name
TARGET := src/main

# Specify the source file names (without .c extension)
SOURCES := src/main

# C 
CC = gcc -Wall -Werror=implicit-function-declaration
ccflags-y := $(INCLUDE_DIRS)


default:
	@ $(MAKE) -C $(KDIR) M=$(CURDIR) SUBDIRS=$(PWD) modules
$(TARGET): $(addsuffix .o,$(SOURCES))
	$(CC) $(CFLAGS) -o $@ $^
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<
install:
	@ $(MAKE) -C $(KDIR) M=$(CURDIR) SUBDIRS=$(PWD) modules
	sudo insmod ${LKM_NAME}.ko
	@ $(MAKE) -C $(KDIR) M=$(CURDIR) clean
	sudo dmesg | tail -5
clean:
	@ $(MAKE) -C $(KDIR) M=$(CURDIR) clean
	@ $(SHELL) -c "rm -rf $(BUILD_DIR)"  
	$(RM) ./doc
	sudo rmmod ${LKM_NAME}
	sudo dmesg | tail -5

cleanc:
	rm -f $(TARGET) $(addsuffix .o,$(SOURCES))
