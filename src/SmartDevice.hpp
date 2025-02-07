/* SmartDevice.hpp */
#ifndef SMARTDEVICE
#define SMARTDEVICE

#include "Smart_Config.h"

#pragma pack(1)


#define MASTER_BIOSCODE   23
#define MASTER_VERSION    0
#define MASTER_SUBVERSION 4

#define SMARTDEVICE_VERSION 0 /* esp8266/esp32  */

#define U8   unsigned char
#define U16  unsigned short
#define U32  unsigned int
#define S8   char
#define S16  short
#define S32  int
#define Bool unsigned char;
#define TRUE  1
#define FALSE 0
#define PACKED  /* */

#define FLASH_MARK 0x1AA11BBf
#define FLASH_WRITESIZE (36+4*16)

//макрос для автоматического определения размера статической части класса для записи-чтения во флеш
#define FLASH_WRITESIZE0 ((int)(&this->sts) - (int)(&this->mark))


struct Msg1
{   short int cmd0; //команда
    short int cmd; //команда
    short int ind; //параметр
    unsigned char Buf[120];
};

class SmartDevice
{
 public:
  const  int mark;  /* пометка для определения наличия во флеше  */
  const  int size;  /* размер для чтения-записи-сравнения версий */
/* константы */
  const int BiosCode;     /* код биоса */
  const int Vers;         /* версия */
  const int SubVers;      /* подверсия */
#if defined(ARDUINO_ARCH_ESP8266)
  static char  BiosDate[12];     /* дата компиляции биоса */
#elif defined(ARDUINO_ARCH_ESP32)
  const char  BiosDate[12];     /* дата компиляции биоса */
#endif
  int IdNumber;            /* номер устройства */

  char server_ipaddr[16]; /* ip адрес сервера */
  int server_port;  /* порт сервера */
  int server_repot_period;  /* периодичность отправки данных серверу, сек */
	IPAddress remoteIP;  


  const int ReservParam[16];    /* резерв параметры */ 

/**** во флеш далее не пишется */     
  int status;
  int sts;                 /* состояние       */  
  int sts_next;            /* состояние на следующий такт  */
  int server_sts;  /* статус сервера */
  long int server_t; /* время последнего сообщения серверу, следующее через server_repot_period */
  float humidity;
  float temperature;
  int status_DHT; /* status DHT: -1, 0=Ok, 1 Timeout, 2 Cheksum */


#if defined(ARDUINO_ARCH_ESP8266)
  SmartDevice(void):mark(FLASH_MARK), size (FLASH_WRITESIZE0), 
               BiosCode(MASTER_BIOSCODE),Vers(MASTER_VERSION), SubVers(MASTER_SUBVERSION),
							 ReservParam()
#elif defined(ARDUINO_ARCH_ESP32)
  SmartDevice(void):mark(FLASH_MARK), size (FLASH_WRITESIZE0), 
               BiosCode(MASTER_BIOSCODE),Vers(MASTER_VERSION), SubVers(MASTER_SUBVERSION), BiosDate(__DATE__),
							 ReservParam()
#endif
			   
  {  // int i;  
	sts = sts_next = 0;
	server_ipaddr[0] = 0; 
        server_port = 0;  
        server_repot_period = 0;
        server_sts = 0;
        server_t = 0;
	humidity = 0.;
	temperature = 0.;
	status_DHT = -1; 
  status = -1;
  }
  void udp_callback_HandShake( U8 *bf, PACKED unsigned char * &MsgOut,int &Lsend, U8 *(*get_buf) (U16 size));
  void udp_callback_Echo( U16 len, U8 *bf, PACKED unsigned char * &MsgOut,int &Lsend, U8 *(*get_buf) (U16 size));
  void udp_callback_Identify( U8 *bf, PACKED unsigned char * &MsgOut,int &Lsend, U8 *(*get_buf) (U16 size));
  void udp_callback_gettime( U8 *bf, PACKED unsigned char * &MsgOut,int &Lsend, U8 *(*get_buf) (U16 size));
  void udp_callback_settime( U8 *bf, PACKED unsigned char * &MsgOut,int &Lsend, U8 *(*get_buf) (U16 size));
  void udp_callback_serverinfo( U8 *bf, PACKED unsigned char * &MsgOut,int &Lsend, U8 *(*get_buf) (U16 size));
  virtual void udp_OpenThermInfo( U8 *bf, PACKED unsigned char * &MsgOut,int &Lsend, U8 *(*get_buf) (U16 size))
  {};


  int uart_send(unsigned char *buf, int len);

};
#pragma pack()

 #endif //SMARTDEVICE
