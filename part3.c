#include<linux/module.h>
#include<linux/list.h> 
#include<linux/init.h>
#include<linux/kernel.h> 
#include<linux/types.h>
#include<linux/kthread.h> 
#include<linux/proc_fs.h>
#include<linux/sched.h>
#include<linux/mm.h> 
#include<linux/fs.h> 
#include<linux/slab.h> 
#include <asm/io.h>
 
static struct proc_dir_entry *tempdir, *tempinfo;
static unsigned char * buffer;
static unsigned char array [12]={0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

static void allocate_memory(void);
static void clear_memory(void);
int my_map(struct file *filp, struct vm_area_struct *vma);

static const struct file_operations myproc_fops = {
    .mmap = my_map,
};

static int my_map(struct file *flip, struct vm_area_struct *vma){
    // map vma of user space to a continuous physical space
    return 0;
}

static int init_myproc_module(void){
    //create a directory in /proc
    //create a new entry under the new directory 
    printk("init myproc module successfully\n");
    
    allocate_memory();
    //initialize the buffer
    for(i=0; i<12; i++){ 
        buffer[i] = array[i];
    }
    
    return 0;
}

static void allocate_memory(void){
    //allocation memory
    //set the memory as reserved
}

static void clear_memory(void){
    //clear reserved memory
    //free memory
}

static void exit_myproc_module(void){
    clear_memory();
    remove_proc_entry("myinfo", tempdir); 
    remove_proc_entry("mydir", NULL); 
    printk("remove myproc module successfully\n");
}

module_init(init_myproc_module);
module_exit(exit_myproc_module); 
MODULE_LICENSE("GPL");
