#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/i2c-dev.h>

static char i2cDevice[255];

static int rawRead(char address, char reg)
{
	int file;
	if((file = open(i2cDevice, O_RDWR)) < 0)
	{
		printf("Failed to open the i2c bus.\n");
    	return -1;
	}

	if (ioctl(file, I2C_SLAVE, address) < 0) 
	{
	    printf("Failed to acquire bus access and/or talk to slave.\n");
	    close(file);
	    return -1;
	}

	char buf1[1] = {reg};

    if (write(file,buf1,1) != 1) 
    {
        printf("Failed to write to the i2c bus.\n");
        close(file);
        return -1;
    }

	char buf[1] = {0};
	if (read(file,buf,1) != 1) 
	{
        /* ERROR HANDLING: i2c transaction failed */
        printf("Failed to read from the i2c bus.\n");
        close(file);
        return -1;
    }

    close(file);
    return (int)buf[0];

}

static int rawWrite(char address, char reg, char data)
{
	int file;
	if((file = open(i2cDevice, O_RDWR)) < 0)
	{
		printf("Failed to open the i2c bus.\n");
    	return -1;
	}

	if (ioctl(file, I2C_SLAVE, address) < 0) 
	{
	    printf("Failed to acquire bus access and/or talk to slave.\n");
	    close(file);
	    return -1;
	}

	char buf1[2] = {reg, data};

    if (write(file,buf1,2) != 2) 
    {
        printf("Failed to write to the i2c bus.\n");
        close(file);
        return -1;
    }


    close(file);
    return 0;

}

int initI2c(const char * device)
{
	strncpy(i2cDevice, device, 255);
	return 0;
}

int readInput(char address)
{
	return rawRead(address, 0);
}

int writeOutputs(char address, char data)
{
	return rawWrite(address, 1, data);
}

int readConfig(char address)
{
	return rawRead(address, 3);
}

int writeConfig(char address, char data)
{
	return rawWrite(address, 3, data);
}
