#ifndef _JT808_GPS_H_
#define _JT808_GPS_H_

/*����λ����Ϣ,��Ϊ�ֽڶ���ķ�ʽ������ʹ�����鷽��*/
typedef struct _gps_baseinfo
{
	uint32_t alarm;
	uint32_t status;
	uint32_t latitude; /*γ��*/
	uint32_t longitude;/*����*/
	uint16_t altitude;
	uint16_t speed;
	uint16_t direction;
	uint8_t datetime[6];
	uint32_t odb_odometer;
	uint16_t fuellevel;
	uint16_t odb_speed;
} GPS_BASEINFO;


enum BDGPS_MODE 
{
	MODE_GET=0,	/*��ѯ*/
	MODE_BD=1,
	MODE_GPS,
	MODE_BDGPS,
};


typedef  struct  _gps_status
{
   enum BDGPS_MODE   Position_Moule_Status;  /* 1: BD   2:  GPS   3: BD+GPS    ��λģ���״̬*/
   uint8_t  Antenna_Flag;//��ʾ��ʾ��· 
   uint8_t  Raw_Output;   //  ԭʼ�������  
}GPS_STATUS;

#define NMEA_END_CHAR_1		'\r'
#define NMEA_END_CHAR_2		'\n'

extern GPS_STATUS	gps_status;
extern GPS_BASEINFO gps_baseinfo;



void gps_rx( uint8_t * pinfo, uint16_t length );




#endif
