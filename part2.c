#include <linux/module.h> 
#include <linux/kernel.h>
#include <linux/proc_fs.h> 
#include <linux/string.h>
#include <linux/vmalloc.h> 
#include <asm/uaccess.h>

#define MAX_LEN 4096
int read_info(char *page, char **start, off_t off, int count, int *eof, 
             void *data);
ssize_t write_info(struct file *filp, const char __user *buff, unsigned 
             long len, void *data);

static struct proc_dir_entry *proc_entry;
static char *info;
static int write_index; 
static int read_index;

int init_module(void) {
    int ret = 0;
    // allocated memory space for the proc entry
    info = (char *)vmalloc(MAX_LEN); 
    memset(info, 0, MAX_LEN);
    
    // create test proc entry
    proc_entry = create_proc_entry("procEntry123", 0644, NULL);
    
    //implement this: create the proc entry
    
    write_index = 0;
    read_index = 0;
    
    //register the write and read callback functions 
    proc_entry->read_proc = read_info; 
    proc_entry->write_proc = write_info;
    printk(KERN_INFO "procEntry123 created .\n");
    return ret; 
}

void cleanup_module(void){
    remove_proc_entry("procEntry123", proc_entry);
    printk(KERN_INFO "procEntry123 unloaded.\n");
    
    // free up allocated memory
    vfree(info);
}

ssize_t write_info(struct file *filp, const char __user *buff,  
    unsigned long len, void * data){
    //copy the written data from user space and save it in info
    int capacity = (MAX_LEN-write_index)+1;
    
    // make sure there is room to write to the proc
    if (len > capacity)
    {
        printk(KERN_INFO "No space to write in procEntry123!\n");
        return -1;
    }
    if (copy_from_user( &info[write_index], buff, len ))
    {
        return -2;
    }
    
    write_index += len;
    info[write_index-1] = 0;
    return len;
}

int read_info(char *buff, char **start, off_t offset, int count, 
    int *eof, void *data){
    //output the content of info to user's buffer pointed by buff
    int len;
    
    // check to make sure that there is something in the file
    if (off > 0)
    {
        *eof = 1;
        return 0;
    }
    
    // check to make sure that everything matches and
    // nothing is longer then the other
    if (read_index >= write_index)
        read_index = 0;
    
    // return the info wanted to read
    len = sprintf(page, "%s\n", &info[read_index]);
    read_index += len;
    return len;
}
