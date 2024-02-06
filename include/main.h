#pragma once
// https://elixir.bootlin.com/linux/v4.4.186/source

#include <linux/moduleparam.h>

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/fs.h> // For intercept read of files
#include <linux/uaccess.h>
#include <asm/paravirt.h> // Inclua esse cabeçalho para a função read_cr0()

#include <linux/mm.h>
#include <linux/mman.h>
#include <linux/syscalls.h>
