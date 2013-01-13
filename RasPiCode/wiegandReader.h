#ifndef wiegandReader_h
#define wiegandReader_h

typedef void (*cardHandler)(unsigned long cardCode);

void addCardCallback(cardHandler pCallback);

void startCardReaderThreads(void);

#endif