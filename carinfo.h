/************************************************************
 * Copyright (C), 2008-2012,
 * FileName:		// �ļ���
 * Author:			// ����
 * Date:			// ����
 * Description:		// ģ������
 * Version:			// �汾��Ϣ
 * Function List:	// ��Ҫ�������书��
 *     1. -------
 * History:			// ��ʷ�޸ļ�¼
 *     <author>  <time>   <version >   <desc>
 *     David    96/10/12     1.0     build this moudle
 ***********************************************************/



/*
xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx

bit16 ������Ϣ�仯



*/


#define EVT_GPS 0x00000001
#define EVT_GPS 0x00000001


#define EVT_vechinfo 0x00010000
#define EVT_deiverinfo 0x00020000





__packed struct
{
/*bdgps*/
	__packed struct {
		uint32_t	alarm;
		uint32_t	status;
		uint32_t	lat;
		uint32_t 	long;
		uint16_t	alt;
		uint16_t	speed;
		uint16_t	direction;
		uint8_t		datetime[6];
	} jt808_position_info;

	struct _DriveInfo
	{
		u8	DriveCode[3];
		u8	DriverCard_ID[19];      // ��ʻԱ��ʻ֤���� 18λ
		u8	DriveName[22];          // ��ʻԱ ����21
		u8	Driver_ID[21];          // ��ʻԱ���֤����20
		u8	Drv_CareerID[41];       // ��ʻԱ��ҵ�ʸ�֤40
		u8	Comfirm_agentID[41];    // ��֤�������� 40
	} DRV_INFO;

	struct _VechInfo
	{
		u8	Vech_VIN[18];           // ����VIN��17
		u8	Vech_Num[13];           // ���ƺ�12
		u8	Vech_Type[13];          // �������� 12
		u16 Dev_ProvinceID;         // ��������ʡID
		u16 Dev_CityID;             // ����������ID
		u8	Dev_Color;              // ������ɫ
	} VechINFO;

	struct  _SYSConfig              //  name:  config
	{
		u16 Version_ID;             //  ϵͳ�汾IDE
		u8	APN_str[40];            //   ���������
		u8	IP_Main[4];             //   ��IP ��ַ
		u16 Port_main;              //   ��socket  �˿�
		u8	IP_Aux[4];              //   ��IP ��ַ
		u16 Port_Aux;               //   ��socket  �˿�
		u8	DNSR[50];               //  DNSR  ��������
		u16 AccOn_Dur;              //  ACC ��  �ϱ����
		u16 AccOff_Dur;             //   ACC ��  �ϱ����
		u8	TriggerSDsatus;         //  �����������ϱ�״̬
	} SYS_CONF;


	char *����������Ϣ��ַ;
} global_info;

/************************************** The End Of File **************************************/
