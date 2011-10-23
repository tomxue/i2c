/*
 * FM24CL64: 64Kb Serial 3V FRAM Memory, 8KB
 * EMA SBC3730-B1-3990-LUAC0 kit, Angstrom
 * I2C Address: 0x50
 * */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/i2c.h>

#define I2C_ADD 0x50

main()
{
    int i;
    int r_num = 8192;
    int w_num = 8192;
    unsigned int fd;
    unsigned char w_buf[w_num];   // = {0x0, 0x0, 0x3, 0x4, 0x5, 0x6};
    unsigned char r_buf[r_num];

    fd = open("/dev/i2c-3", O_RDWR);
    printf("fd = %d\n", fd);

    int ret;
    ret = ioctl(fd, I2C_SLAVE, I2C_ADD);  //set the I2C slave device address
    printf("ioctl ret = %d\n", ret);
    
    for(i=0;i<w_num;i++)   //initialize 'write buffer' for test
        w_buf[i] = 0x95;
    w_buf[0] = 0;	   //memory address: 0:MSB, 1:LSB
    w_buf[1] = 0;
    ret = write(fd, w_buf, w_num);  //in fact, each time only 32 bytes are written, why?
    printf("write ret = %d\n", ret);
    sleep(1);	           //without sleep, next operation (either write or read) fails
    //w_buf[0] = 0;
    //w_buf[1] = 32;       //to write the next 32 bytes
    //ret = write(fd, w_buf, w_num);
    //printf("write ret = %d\n", ret);
   
    w_buf[0] = 0;    //selective address read
    w_buf[1] = 0;    //to set (by writing) the 'start address of reading' to the device
    ret = write(fd, w_buf, 2);
    printf("set the 'start address if reading', ret = %d\n", ret);    
    
    sleep(1);
    memset(r_buf, 0x0, r_num);   //clear the 'read buffer' first
    ret = read(fd, r_buf, r_num);
    printf("read ret = %d\n", ret);
    for(i=0;i<r_num;i++)
        {
                printf("%d=0x%x\t",i,r_buf[i]);
                if(i%10 == 0)
                        printf("\n");
        }
    return 1;
    close(fd);
}

