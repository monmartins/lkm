#include "cr.h"

//CR0
u64 cr0_modify;
u64 cr0_original;
extern void init_cr0(void){
    cr0_original = native_read_cr0();
    printk(KERN_INFO "init_cr0 1 cr0 = 0x%8.8X\n", cr0_original);
    cr0_modify = cr0_original & (~0x10000);
    printk(KERN_INFO "cr0_modify = 0x%8.8X\n", cr0_modify);
    
    native_write_cr0(cr0_modify);    
    
    printk(KERN_INFO "init_cr0 2 cr0 = 0x%8.8X\n", native_read_cr0());
}
extern void revert_cr0(void){
    printk(KERN_INFO "cr0_original = 0x%8.8X\n", cr0_original);
    
    native_write_cr0(cr0_original);
    
    printk(KERN_INFO "Revert cr0 = 0x%8.8X\n", native_read_cr0());
}
//CR4
u64 cr4_modify;
u64 cr4_original;
extern void init_cr4(void){
    cr4_original = native_read_cr4();
    cr4_modify = native_read_cr4();
    printk(KERN_INFO "init_cr4 1 cr4 = 0x%8.8X\n", native_read_cr4());
    cr4_modify ^=(1 << 16);
    native_write_cr4(cr4_modify);
    printk(KERN_INFO "init_cr4 2 cr4 = 0x%8.8X\n", native_read_cr4());
}
extern void revert_cr4(void){
    native_write_cr4(cr4_original);
    printk(KERN_INFO "Revert cr4 = 0x%8.8X\n", native_read_cr4());
}

