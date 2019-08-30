#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/proc_fs.h>

#include "driver.h"


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Douglas Santos e Erich Morais");
MODULE_DESCRIPTION("Fundamentos SO");

#define DEVICE 60
#define DEVICE_NAME "/dev/damas"

#define BUF_MSG 80
#define BUF_LEN 80

int init_device(void);
void cleanup_device(void);
static int device_open(struct inode *inode, struct file *file);
static int device_release(struct inode *inode, struct file *file);
static ssize_t device_read(struct file *file, char __user *buffer, size_t length,loff_t * offset);
static ssize_t device_write(struct file *file, const char __user * buffer, size_t length, loff_t * offset);
static long device_ioctl(struct file *file, unsigned int ioctl_num, unsigned long ioctl_param);


module_init(init_device);
module_exit(cleanup_device)


static int aberto = 0;
static char mensagem[BUF_MSG];
static char *ptr;

struct file_operations fops = {
	.read = device_read,
	.write = device_write,
	.open = device_open,
	.release = device_release,
	.unlocked_ioctl = device_ioctl	
};


int init_device(){

	int ret;

	ret = register_chrdev(DEVICE, DEVICE_NAME, &fops);
	if (ret < 0) {
		printk("Erro ao carregar o dispositivo %d\n.",DEVICE);	
		return ret;
	}

	printk("O dispositivo %d foi carregado.\n", DEVICE);
	return 0;
}

void cleanup_device(){
	unregister_chrdev(DEVICE, DEVICE_NAME);
	printk("O dispositivo %d foi descarregado.\n", DEVICE);
}

static int device_open(struct inode *inode, struct file *file){
	if (aberto) return -EBUSY;
	aberto++;
	ptr = mensagem;
	try_module_get(THIS_MODULE);
	return 0;
}


static int device_release(struct inode *inode, struct file *file){
	aberto--;
	module_put(THIS_MODULE);
	return 0;
}


static ssize_t device_read(struct file *file, char __user * buffer, size_t length, loff_t * offset){
	int bytes_read = 0;

	if (*ptr == 0) return 0;
	while (length && *ptr) {

		put_user(*(ptr++), buffer++);
		length--;
		bytes_read++;
	}
	return bytes_read;
}


static ssize_t device_write(struct file *file, const char __user * buffer, size_t length, loff_t * offset){
	
	int i;

	for (i = 0; i < length && i < BUF_MSG; i++) get_user(mensagem[i], buffer + i);
	printk("\nRecebeu: %s\n",mensagem);
	ptr = mensagem;
	return i;
}

//Verificar e testar estrutura, Ultima modificação 11/07 14:30
static long device_ioctl(struct file *file, unsigned int ioctl_num, unsigned long ioctl_param){
	
	int i;
	char *temp;
	char ch;
	//printk("\nTeste1: Comunicou device_ioctl\n");
	switch (ioctl_num) {
	case IOCTL_SET_MSG:
		printk("\nIOCTL_SET_MSG\n");

		temp = (char *)ioctl_param;

		get_user(ch, temp);
		for (i = 0; ch && i < BUF_LEN; i++, temp++)
			get_user(ch, temp);

		device_write(file, (char *)ioctl_param, i, 0);
		break;

	case IOCTL_GET_MSG:
		printk("\nIOCTL_GET_MSG\n");
		
		//Passa a mensagem pro processo de chamada
		i = device_read(file, (char *)ioctl_param, 99, 0);
		

		//Termian a string com \0 
		put_user('\0', (char *)ioctl_param + i);
		break;
	}
	return 0;
}
