#pragma once
#include <linux/module.h>
#include <linux/kernel.h>

void init_cr0(void);
void init_cr4(void);
void revert_cr0(void);
void revert_cr4(void);
