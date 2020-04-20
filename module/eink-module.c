
#ifdef __KERNEL__
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/cdev.h>
#include <linux/gpio.h>                 // Required for the GPIO functions
#include <linux/interrupt.h>            // Required for the IRQ code
#include <linux/spi/spi.h>
#include <linux/printk.h>
// #define printf PDEBUG


#define AESD_DEBUG 1  //Remove comment on this line to enable debug

#undef PDEBUG             /* undef it, just in case */
#ifdef AESD_DEBUG
#  ifdef __KERNEL__
     /* This one if debugging is on, and kernel space */
#    define PDEBUG(fmt, args...) printk( KERN_DEBUG "aesdchar: " fmt, ## args)
#  else
     /* This one for user space */
#    define PDEBUG(fmt, args...) fprintf(stderr, fmt, ## args)
#  endif
#else
#  define PDEBUG(fmt, args...) /* not debugging: nothing */
#endif

#endif

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Prayag Desai");
MODULE_DESCRIPTION("A module to work with the e-ink display");
MODULE_VERSION("0.1");

#include "epd1in54.h"
#include "epdif.h"
#include "Lucida_Console_8pts.h"
#include "drawFunctions.h"


int einkReset = RST_PIN;
int einkDC = DC_PIN;
int einkBusy = BUSY_PIN;

bool einkResetState = false;
bool einkDCState = false;
bool einkBusyState = false;



struct spi_device *spi_device;

int aesd_major =   0; // use dynamic major
int aesd_minor =   0;

struct file_operations aesd_fops = {
	.owner =    THIS_MODULE,
	// .read =     aesd_read,
	// .write =    aesd_write,
	// .open =     aesd_open,
	// .llseek  = 	aesd_llseek,
	// .unlocked_ioctl = aesd_ioctl,
	// .release =  aesd_release,
};

int __init eink_init(void)
{

    struct cdev cdev;
    int err, devno = MKDEV(aesd_major, aesd_minor);
    cdev_init(&cdev, &aesd_fops);
	cdev.owner = THIS_MODULE;
	cdev.ops = &aesd_fops;
	err = cdev_add (&cdev, devno, 1);
	if (err) {
		printk(KERN_ERR "Error %d adding aesd cdev", err);
	}


     //NO PULL UPS

     printk(KERN_INFO "GPIO INIT: Initializing the GPIO\n");

     if (!gpio_is_valid(einkReset)){
      PDEBUG(KERN_INFO "GPIO_TEST: invalid LED GPIO\n");
      return -ENODEV;
     }
     if (!gpio_is_valid(einkDC)){
      PDEBUG(KERN_INFO "GPIO_TEST: invalid LED GPIO\n");
      return -ENODEV;
     }
     if (!gpio_is_valid(einkBusy)){
      PDEBUG(KERN_INFO "GPIO_TEST: invalid LED GPIO\n");
      return -ENODEV;
     }

     gpio_request(einkReset, "sysfs"); 
     gpio_request(einkDC, "sysfs"); 

     gpio_direction_output(einkReset, einkResetState);
     gpio_direction_output(einkDC, einkDCState);

     gpio_export(einkReset, false);
     gpio_export(einkDC, false);

     gpio_request(einkBusy, "sysfs");
     gpio_direction_input(einkBusy);
     gpio_set_debounce(einkBusy, 50);
     gpio_export(einkBusy, false);


     int ret;
     
           printk("beddsvfs to setup slave.\n");

    struct spi_master *master;

      //Register information about your slave device:
struct spi_board_info spi_device_info = {
     .modalias = "eink-spi-driver",
     .max_speed_hz = 10000000, //speed your device (slave) can handle
     .bus_num = 0,
     .chip_select = 0,
     .mode = 0
     // .bits_per_word = 8
};

     
     /*To send data we have to know what spi port/pins should be used. This information 
       can be found in the device-tree. */
     master = spi_busnum_to_master( spi_device_info.bus_num );
     if( !master ){
         printk("MASTER not found.\n");
             return -ENODEV;
      }
      printk("PADD to setup slave.\n");
     
    //  // create a new slave device, given the master and device info
     spi_device = spi_new_device( master, &spi_device_info );
 
      if( !spi_device ) {
         printk("FAILED to create slave.\n");
         return -ENODEV;
     }
     
     spi_device->bits_per_word = 8;
 
     ret = spi_setup( spi_device );

     printk("MAGGG to setup slave.\n");
     
     if( ret ){
         printk("FAILED to setup slave.\n");
         spi_unregister_device( spi_device );
         return -ENODEV;
     }
     printk("jrfskm to setup slave.\n");
 
     // spi_write(spi_device, &ch, sizeof(ch));
     Init(lut_full_update);
     ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
     DisplayFrame();

     int i, j;

     for(i = 0; i < 200; i++)
    {
        for(j = 0; j < 200 / 8; j++)
        {
            imageBuffer[i][j] = 0xFF;
        }
    }

    for(i = 0; i < 200; i++)
    {
        drawPixel(i, i, DISP_BLACK);
        // printf("Draw for: %d\n", i);
    }

    drawLineX(100, 50, 10, DISP_BLACK);
    updateDisplay();

    drawLineY(10, 100, 100, DISP_BLACK);
    updateDisplay();

    writeString(20, 20, DISP_BLACK, "Prayag - module");
    updateDisplay();
     
     return 0;
}

void __exit eink_exit(void)
{
     gpio_unexport(einkReset);
     gpio_unexport(einkDC);
     gpio_unexport(einkBusy);

     gpio_free(einkReset);
     gpio_free(einkDC);
     gpio_free(einkBusy);

    if(spi_device)
     spi_unregister_device( spi_device );
}


module_init(eink_init);
module_exit(eink_exit);
