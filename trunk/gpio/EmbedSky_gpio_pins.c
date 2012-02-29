/*************************************

NAME:EmbedSky_pins.c
This is a merge of the gpio and irq modules to control the other gpio pins

@author cal
EmbedSky_gpio.c - led controller

EmbedSky_irq.c - button controller 
 - make sure the CONFIG_SKY2440_IRQ_TEST menu option is not selected 
   or 2 buttons won't work

There are other gpio pins on the side of the device directly above the power switch:
EINT4,8,14,19,18 on the "omo" side (facing out it looks like)
pins 7,9,11,13,15 respectively
EINT3,11,13,15,9 on the gnd side
pins 8,10,12,14,16 respectively
Note that EINT4 is mapped to a button also as are EINT0,1,2

*************************************/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/poll.h>
#include <linux/irq.h>
#include <asm/irq.h>
#include <linux/interrupt.h>
#include <asm/uaccess.h>
#include <mach/regs-gpio.h>
#include <mach/hardware.h>
#include <linux/platform_device.h>
#include <linux/cdev.h>
#include <linux/miscdevice.h>

#define DEVICE_NAME "GPIO-other-pins"
#define IOCTL_GPIO_ON 1
#define IOCTL_GPIO_OFF 0
#define GPIO_WRITE 1
#define GPIO_READ 0
#define GPIO_UNKNOWN -1

static int writeable;
module_param(writeable,int,S_IRUGO);
MODULE_PARM_DESC(
	writeable,
	"make EINT_ pins writeable:\n\t\t(bit val=>pin) 1=>8, 2=>9, 4=>11, 8=>13, 16=>14, 32=>15, 64=>18, 128=>19\n"
	"\t\tpins cannot be readable and writeable"
);

static int readable;
module_param(readable,int,S_IRUGO);
MODULE_PARM_DESC(
	readable,
	"make EINT_ pins readable:\n\t\t(bit val=>pin) 1=>8, 2=>9, 4=>11, 8=>13, 16=>14, 32=>15, 64=>18, 128=>19\n"
	"\t\tpins cannot be readable and writeable"
);


struct gpio_irq_desc {
    int irq;
    int pin;
    int pin_setting_in;
    int number;
    char *name;	
    // tacked on to make output to a pin work
    int access;
    int pin_setting_out;
    int flag;
};

// these are all the pins on the gpio that look like they can be used as general io ?
static struct gpio_irq_desc gpio_irqs [] = {
	{IRQ_EINT0,	S3C2410_GPG0,	S3C2410_GPG0_EINT8,	0, 	"EINT8",	GPIO_UNKNOWN,	S3C2410_GPG0_OUTP, 1}, 
	{IRQ_EINT1,	S3C2410_GPG1,	S3C2410_GPG1_EINT9,	1, 	"EINT9",	GPIO_UNKNOWN,	S3C2410_GPG1_OUTP, 2}, 
	{IRQ_EINT11,	S3C2410_GPG3,	S3C2410_GPG3_EINT11,	3, 	"EINT11",	GPIO_UNKNOWN,	S3C2410_GPG3_OUTP, 4}, 
	{IRQ_EINT13,	S3C2410_GPG5,	S3C2410_GPG5_EINT13,	5, 	"EINT13",	GPIO_UNKNOWN,	S3C2410_GPG5_OUTP, 8}, 
	{IRQ_EINT14,	S3C2410_GPG6,	S3C2410_GPG6_EINT14,	6, 	"EINT14",	GPIO_UNKNOWN,	S3C2410_GPG6_OUTP, 16}, 
	{IRQ_EINT15,	S3C2410_GPG7,	S3C2410_GPG7_EINT15,	7, 	"EINT15",	GPIO_UNKNOWN,	S3C2410_GPG7_OUTP, 32}, 
	{IRQ_EINT18,	S3C2410_GPG10,	S3C2410_GPG10_EINT18,	10, 	"EINT18",	GPIO_UNKNOWN,	S3C2410_GPG10_OUTP, 64}, 
	{IRQ_EINT19,	S3C2410_GPG11,	S3C2410_GPG11_EINT19,	11, 	"EINT19",	GPIO_UNKNOWN,	S3C2410_GPG11_OUTP, 128}, 
};
#define GPIOLEN (sizeof(gpio_irqs)/sizeof(gpio_irqs[0]))

static volatile char key_values [] = {'0', '0', '0', '0', '0', '0', '0', '0'};

static DECLARE_WAIT_QUEUE_HEAD(button_waitq);

static volatile int ev_press = 0;


static irqreturn_t irq_interrupt(int irq, void *dev_id)
{
	struct gpio_irq_desc *gpio_irqs = (struct gpio_irq_desc *)dev_id;
	int down;

	down = !s3c2410_gpio_getpin(gpio_irqs->pin);

	if (down != (key_values[gpio_irqs->number] & 1))
	{
		key_values[gpio_irqs->number] = '0' + down;
		ev_press = 1;
		wake_up_interruptible(&button_waitq);
	}

	return IRQ_RETVAL(IRQ_HANDLED);
}


static int tq2440_irq_open(struct inode *inode, struct file *file)
{
	int i;
	int err = 0;

	for (i = 0; i < GPIOLEN; i++)
	{
		if (gpio_irqs[i].access == GPIO_WRITE) {
			s3c2410_gpio_cfgpin(
				gpio_irqs[i].pin, 
				gpio_irqs[i].pin_setting_out
			);
			s3c2410_gpio_setpin( gpio_irqs[i].pin, 0 );

		} else if (gpio_irqs[i].access == GPIO_READ) {
			if (gpio_irqs[i].irq < 0) continue;
			err = request_irq(
					gpio_irqs[i].irq, 
					irq_interrupt, 
					IRQ_TYPE_EDGE_BOTH, 
					gpio_irqs[i].name, 
					(void *)&gpio_irqs[i]
			);
		}
		if (err) break;
	}

	if (err)
	{
		i--;
		for (; i >= 0; i--)
		{
			// didn't appear there was anything to do for output mode so ...
			if (gpio_irqs[i].access == GPIO_READ) {
				if (gpio_irqs[i].irq < 0)
					continue;
				disable_irq(gpio_irqs[i].irq);
				free_irq(gpio_irqs[i].irq, (void *)&gpio_irqs[i]);
			}
		}
		return -EBUSY;
	}

	ev_press = 1;

	return 0;
}


static int tq2440_irq_close(struct inode *inode, struct file *file)
{
	int i;

	for (i = 0; i < GPIOLEN; i++)
	{
		if (gpio_irqs[i].access == GPIO_READ) {
			if (gpio_irqs[i].irq < 0) continue;
			free_irq(gpio_irqs[i].irq, (void *)&gpio_irqs[i]);
		}
	}

	return 0;
}


static int tq2440_irq_read(struct file *filp, char __user *buff, size_t count, loff_t *offp)
{
	unsigned long err;

	if (!ev_press)
	{
		if (filp->f_flags & O_NONBLOCK)
			return -EAGAIN;
		else
			wait_event_interruptible(button_waitq, ev_press);
	}

	ev_press = 0;

	err = copy_to_user(buff, (const void *)key_values, min(sizeof(key_values), count));

	return err ? -EFAULT : min(sizeof(key_values), count);
}

static unsigned int tq2440_irq_poll( struct file *file, struct poll_table_struct *wait)
{
	unsigned int mask = 0;
	poll_wait(file, &button_waitq, wait);
	if (ev_press)
		mask |= POLLIN | POLLRDNORM;
	return mask;
}


// from the gpio module (leds output example)

static int tq2440_gpio_ioctl (
	struct inode *inode, 
	struct file *file, 
	unsigned int cmd, 
	unsigned long arg)
{
	if (arg > 4)
	{
		return -EINVAL;
	}

	switch(cmd)
	{
		case IOCTL_GPIO_ON:
			s3c2410_gpio_setpin(gpio_irqs[arg].pin, 0);
			return 0;

		case IOCTL_GPIO_OFF:
			s3c2410_gpio_setpin(gpio_irqs[arg].pin, 1);
			return 0;

		default:
			return -EINVAL;
	}
}

static struct file_operations dev_fops = {
	.owner		=   THIS_MODULE,
	.open		=   tq2440_irq_open,
	.release	=   tq2440_irq_close, 
	.read		=   tq2440_irq_read,
	.poll		=   tq2440_irq_poll,
	.ioctl 		=   tq2440_gpio_ioctl,
};

static struct miscdevice misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEVICE_NAME,
	.fops = &dev_fops,
};

static int __init dev_init(void)
{
	int ret, i, enabled=0;

	for (i = 0; i < GPIOLEN; i++) {
		if (readable & gpio_irqs[i].flag) {
			gpio_irqs[i].access = GPIO_READ;
			printk(KERN_INFO "GPIO pin %s set to accept input\n", gpio_irqs[i].name);
			enabled = 1;

		} else if (writeable & gpio_irqs[i].flag) {
			gpio_irqs[i].access = GPIO_WRITE;
			printk(KERN_INFO "GPIO pin %s set to send output\n", gpio_irqs[i].name);
			enabled = 1;
		}
	}

	if (!enabled) {
		printk(KERN_ERR "No GPIO pins enabled - aborting! (use readable/writeable parameters to enable pins)\n");
		return -1;
	}
	ret = misc_register(&misc);

	printk (DEVICE_NAME" initialized\n");

	return ret;
}

static void __exit dev_exit(void)
{
	misc_deregister(&misc);
}

module_init(dev_init);
module_exit(dev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("cal/embedsky");
MODULE_DESCRIPTION("GPIO IRQ reader / writer for breakout pins on side of board");

