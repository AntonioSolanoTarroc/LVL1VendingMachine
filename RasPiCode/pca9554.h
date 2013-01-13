#ifndef pca9554_h
#define pca9554_h

int initI2c(const char * device);

int readInput(char address);
int writeOutputs(char address, char data);
int readConfig(char address);
int writeConfig(char address, char data);


#endif