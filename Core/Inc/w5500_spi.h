#ifndef INC_W5500_SPI_H_
#define INC_W5500_SPI_H_

#ifdef __cplusplus
extern "C" {
#endif


void W5500Init();
void startServer();
void sendData(uint8_t* pBuf, uint16_t len);

#ifdef __cplusplus
}
#endif
#endif
