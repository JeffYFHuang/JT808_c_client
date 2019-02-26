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

#include "jt808_param.h"

JT808_PARAM jt808_param =
{
	0x130060600,         /*0x0000 �汾*/
	5,                  /*0x0001 �������ͼ��*/
	5,                  /*0x0002 TCPӦ��ʱʱ��*/
	3,                  /*0x0003 TCP��ʱ�ش�����*/
	3,                  /*0x0004 UDPӦ��ʱʱ��*/
	5,                  /*0x0005 UDP��ʱ�ش�����*/
	3,                  /*0x0006 SMS��ϢӦ��ʱʱ��*/
	5,                  /*0x0007 SMS��Ϣ�ش�����*/
	"CMNET",            /*0x0010 ��������APN*/
	"",                 /*0x0011 �û���*/
	"",                 /*0x0012 ����*/
	"60.28.50.210",     /*0x0013 ����������ַ*/
	"CMNET",            /*0x0014 ����APN*/
	"",                 /*0x0015 �����û���*/
	"",                 /*0x0016 ��������*/
	"www.google.com",   /*0x0017 ���ݷ�������ַ��ip������*/
	9131,               /*0x0018 TCP�˿�*/
	5678,               /*0x0019 UDP�˿�*/
	"www.google.com",   /*0x001A ic������������ַ��ip������*/
	9901,               /*0x001B ic��������TCP�˿�*/
	8875,                /*0x001C ic��������UDP�˿�*/
	"www.google.com",   /*0x001D ic�����ݷ�������ַ��ip������*/
	0,                  /*0x0020 λ�û㱨����*/
	1,                  /*0x0021 λ�û㱨����*/
	30,                 /*0x0022 ��ʻԱδ��¼�㱨ʱ����*/
	120,                /*0x0027 ����ʱ�㱨ʱ����*/
	5,                  /*0x0028 ��������ʱ�㱨ʱ����*/
	30,                 /*0x0029 ȱʡʱ��㱨���*/
	500,                /*0x002c ȱʡ����㱨���*/
	1000,               /*0x002d ��ʻԱδ��¼�㱨������*/
	1000,               /*0x002e ����ʱ����㱨���*/
	100,                /*0x002f ����ʱ����㱨���*/
	270,                /*0x0030 �յ㲹���Ƕ�*/
	500,                /*0x0031 ����Χ���뾶���Ƿ�λ����ֵ������λΪ��*/
	"1008611",          /*0x0040 ���ƽ̨�绰����*/
	"",                 /*0x0041 ��λ�绰����*/
	"",                 /*0x0042 �ָ��������õ绰����*/
	"",                 /*0x0043 ���ƽ̨SMS����*/
	"",                 /*0x0044 �����ն�SMS�ı���������*/
	5,                  /*0x0045 �ն˽����绰����*/
	3,                  /*0x0046 ÿ��ͨ��ʱ��*/
	3,                  /*0x0047 ����ͨ��ʱ��*/
	"",                 /*0x0048 �����绰����*/
	"",                 /*0x0049 ���ƽ̨��Ȩ���ź���*/
	5,                  /*0x0050 ����������*/
	3,                  /*0x0051 ���������ı�SMS����*/
	5,                  /*0x0052 �������տ���*/
	3,                  /*0x0053 ��������洢��־*/
	5,                  /*0x0054 �ؼ���־*/
	3,                  /*0x0055 ����ٶ�kmh*/
	5,                  /*0x0056 ���ٳ���ʱ��*/
	3,                  /*0x0057 ������ʻʱ������*/
	5,                  /*0x0058 �����ۼƼ�ʻʱ������*/
	3,                  /*0x0059 ��С��Ϣʱ��*/
	5,                  /*0x005A �ͣ��ʱ��*/
	900,                /*0x0005B ���ٱ���Ԥ����ֵ����λΪ 1/10Km/h */
	90,                 /*0x005C ƣ�ͼ�ʻԤ����ֵ����λΪ�루s����>0*/
	0x200a,             /*0x005D ��ײ������������:*/
	30,                 /*0x005E �෭�����������ã� �෭�Ƕȣ���λ 1 �ȣ�Ĭ��Ϊ 30 ��*/
	0,                  /*0x0064 ��ʱ���տ���*/
	0,                  /*0x0065 �������տ���*/
	3,                  /*0x0070 ͼ����Ƶ����(1-10)*/
	5,                  /*0x0071 ����*/
	3,                  /*0x0072 �Աȶ�*/
	5,                  /*0x0073 ���Ͷ�*/
	3,                  /*0x0074 ɫ��*/
	5,                  /*0x0080 ������̱����0.1km*/
	3,                  /*0x0081 ʡ��ID*/
	5,                  /*0x0082 ����ID*/
	"��O-00001",        /*0x0083 ����������*/
	1,                  /*0x0084 ������ɫ	1��ɫ 2��ɫ 3��ɫ 4��ɫ 9����*/
	0x0f,               /*0x0090 GNSS ��λģʽ*/
	0x01,               /*0x0091 GNSS ������*/
	0x01,               /*0x0092 GNSS ģ����ϸ��λ�������Ƶ��*/
	0x01,               /*0x0093	 GNSS ģ����ϸ��λ���ݲɼ�Ƶ��*/
	0x01,               /*0x0094	 GNSS ģ����ϸ��λ�����ϴ���ʽ*/
	0x01,               /*0x0095 GNSS ģ����ϸ��λ�����ϴ�����*/
	0,                  /*0x0100 CAN ����ͨ�� 1 �ɼ�ʱ����(ms)��0 ��ʾ���ɼ�*/
	0,                  /*0x0101 CAN ����ͨ�� 1 �ϴ�ʱ����(s)��0 ��ʾ���ϴ�*/
	0,                  /*0x0102 CAN ����ͨ�� 2 �ɼ�ʱ����(ms)��0 ��ʾ���ɼ�*/
	0,                  /*0x0103 CAN ����ͨ�� 2 �ϴ�ʱ����(s)��0 ��ʾ���ϴ�*/
	{ 0, 0 },           /*0x0110 CAN ���� ID �����ɼ�����*/
	{ 0, 0 },           /*0x0111 ����CAN ���� ID �����ɼ�����*/
	{ 0, 0 },           /*0x0112 ����CAN ���� ID �����ɼ�����*/
	{ 0, 0 },           /*0x0113 ����CAN ���� ID �����ɼ�����*/
	{ 0, 0 },           /*0x0114 ����CAN ���� ID �����ɼ�����*/
	{ 0, 0 },           /*0x0115 ����CAN ���� ID �����ɼ�����*/
	{ 0, 0 },           /*0x0116 ����CAN ���� ID �����ɼ�����*/
	{ 0, 0 },           /*0x0117 ����CAN ���� ID �����ɼ�����*/
	{ 0, 0 },           /*0x0118 ����CAN ���� ID �����ɼ�����*/
	{ 0, 0 },           /*0x0119 ����CAN ���� ID �����ɼ�����*/
};

TERM_PARAM term_param =
{
	0x07,
	{ 0x11, 0x22, 0x33, 0x44, 0x55, 0x66 },
	{ "TCBBD" },
	{ "TW701-BD" },
	{ 0x00, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee},
};

#define FLAG_DISABLE_REPORT_INVALID 1       /*�豸�Ƿ�*/
#define FLAG_DISABLE_REPORT_AREA	2       /*�����ڽ�ֹ�ϱ�*/

static uint32_t flag_disable_report = 0;    /*��ֹ�ϱ��ı�־λ*/

/*���������serialflash*/
void param_save( void )
{
	printf( "parma_save size=%d\r\n", sizeof( jt808_param ) );
#if 0
	sst25_write_back( ADDR_PARAM, (uint8_t*)&jt808_param, sizeof( jt808_param ) );
#endif
}

/*���ز�����serialflash*/
void param_load( void )
{
#if 0
	/*Ԥ��һ��������*/
	uint8_t		ver8[4];
	uint32_t	ver32;
	sst25_read( ADDR_PARAM, ver8, 4 );
	ver32 = ( ver8[0] ) | ( ver8[1] << 8 ) | ( ver8[2] << 16 ) | ( ver8[3] << 24 );
	printf( "param_load ver=%08x\r\n", ver32 );
	if( jt808_param.id_0x0000 != ver32 ) /*�����ǲ���δ��ʼ��*/
	{
		printf( "%s(%d)param_save\r\n", __func__, __LINE__ );
		param_save( );
	}
	sst25_read( ADDR_PARAM, (uint8_t*)&jt808_param, sizeof( jt808_param ) );
#endif
}

#define TYPE_BYTE	0x01    /*�̶�Ϊ1�ֽ�,С�˶���*/
#define TYPE_WORD	0x02    /*�̶�Ϊ2�ֽ�,С�˶���*/
#define TYPE_DWORD	0x04    /*�̶�Ϊ4�ֽ�,С�˶���*/
#define TYPE_STR	0x80    /*�̶�Ϊ32�ֽ�,����˳��*/
#define TYPE_CAN_ID 0x48    /*�̶�Ϊ8�ֽ�,��ǰ�洢CAN_ID����*/

struct _tbl_id_lookup
{
	uint16_t	id;
	uint8_t		type;
	uint8_t		* val;
} tbl_id_lookup[] = {
	{ 0x0000, TYPE_DWORD,	 (uint8_t*)&( jt808_param.id_0x0000 ) },    //uint32_t  id_0x0000;   /*0x0000 �汾*/
	{ 0x0001, TYPE_DWORD,	 (uint8_t*)&( jt808_param.id_0x0001 ) },    //uint32_t  id_0x0001;   /*0x0001 �������ͼ��*/
	{ 0x0002, TYPE_DWORD,	 (uint8_t*)&( jt808_param.id_0x0002 ) },    //uint32_t  id_0x0002;   /*0x0002 TCPӦ��ʱʱ��*/
	{ 0x0003, TYPE_DWORD,	 (uint8_t*)&( jt808_param.id_0x0003 ) },    //uint32_t  id_0x0003;   /*0x0003 TCP��ʱ�ش�����*/
	{ 0x0004, TYPE_DWORD,	 (uint8_t*)&( jt808_param.id_0x0004 ) },    //uint32_t  id_0x0004;   /*0x0004 UDPӦ��ʱʱ��*/
	{ 0x0005, TYPE_DWORD,	 (uint8_t*)&( jt808_param.id_0x0005 ) },    //uint32_t  id_0x0005;   /*0x0005 UDP��ʱ�ش�����*/
	{ 0x0006, TYPE_DWORD,	 (uint8_t*)&( jt808_param.id_0x0006 ) },    //uint32_t  id_0x0006;   /*0x0006 SMS��ϢӦ��ʱʱ��*/
	{ 0x0007, TYPE_DWORD,	 (uint8_t*)&( jt808_param.id_0x0007 ) },    //uint32_t  id_0x0007;   /*0x0007 SMS��Ϣ�ش�����*/
	{ 0x0010, TYPE_STR,		 (uint8_t*)&( jt808_param.id_0x0010 ) },    //char   id_0x0010[32];  /*0x0010 ��������APN*/
	{ 0x0011, TYPE_STR,		 (uint8_t*)&( jt808_param.id_0x0011 ) },    //char   id_0x0011[32];  /*0x0011 �û���*/
	{ 0x0012, TYPE_STR,		 (uint8_t*)&( jt808_param.id_0x0012 ) },    //char   id_0x0012[32];  /*0x0012 ����*/
	{ 0x0013, TYPE_STR,		 (uint8_t*)&( jt808_param.id_0x0013 ) },    //char   id_0x0013[32];  /*0x0013 ����������ַ*/
	{ 0x0014, TYPE_STR,		 (uint8_t*)&( jt808_param.id_0x0014 ) },    //char   id_0x0014[32];  /*0x0014 ����APN*/
	{ 0x0015, TYPE_STR,		 (uint8_t*)&( jt808_param.id_0x0015 ) },    //char   id_0x0015[32];  /*0x0015 �����û���*/
	{ 0x0016, TYPE_STR,		 (uint8_t*)&( jt808_param.id_0x0016 ) },    //char   id_0x0016[32];  /*0x0016 ��������*/
	{ 0x0017, TYPE_STR,		 (uint8_t*)&( jt808_param.id_0x0017 ) },    //char   id_0x0017[32];  /*0x0017 ���ݷ�������ַ��ip������*/
	{ 0x0018, TYPE_DWORD,	 (uint8_t*)&( jt808_param.id_0x0018 ) },    //uint32_t  id_0x0018;   /*0x0018 TCP�˿�*/
	{ 0x0019, TYPE_DWORD,	 (uint8_t*)&( jt808_param.id_0x0019 ) },    //uint32_t  id_0x0019;   /*0x0019 UDP�˿�*/
	{ 0x001A, TYPE_STR,		 (uint8_t*)&( jt808_param.id_0x001A ) },    //char   id_0x001A[32];  /*0x001A ic������������ַ��ip������*/
	{ 0x001B, TYPE_DWORD,	 (uint8_t*)&( jt808_param.id_0x001B ) },    //uint32_t  id_0x001B;   /*0x001B ic��������TCP�˿�*/
	{ 0x001C, TYPE_DWORD,	 (uint8_t*)&( jt808_param.id_0x001C ) },    //uint32_t  id_0x001C;   /*0x001C ic��������UDP�˿�*/
	{ 0x001D, TYPE_STR,		 (uint8_t*)&( jt808_param.id_0x001D ) },    //char   id_0x001D[32];  /*0x001D ic�����ݷ�������ַ��ip������*/
	{ 0x0020, TYPE_DWORD,	 (uint8_t*)&( jt808_param.id_0x0020 ) },    //uint32_t  id_0x0020;   /*0x0020 λ�û㱨����*/
	{ 0x0021, TYPE_DWORD,	 (uint8_t*)&( jt808_param.id_0x0021 ) },    //uint32_t  id_0x0021;   /*0x0021 λ�û㱨����*/
	{ 0x0022, TYPE_DWORD,	 (uint8_t*)&( jt808_param.id_0x0022 ) },    //uint32_t  id_0x0022;   /*0x0022 ��ʻԱδ��¼�㱨ʱ����*/
	{ 0x0027, TYPE_DWORD,	 (uint8_t*)&( jt808_param.id_0x0027 ) },    //uint32_t  id_0x0027;   /*0x0027 ����ʱ�㱨ʱ����*/
	{ 0x0028, TYPE_DWORD,	 (uint8_t*)&( jt808_param.id_0x0028 ) },    //uint32_t  id_0x0028;   /*0x0028 ��������ʱ�㱨ʱ����*/
	{ 0x0029, TYPE_DWORD,	 (uint8_t*)&( jt808_param.id_0x0029 ) },    //uint32_t  id_0x0029;   /*0x0029 ȱʡʱ��㱨���*/
	{ 0x002C, TYPE_DWORD,	 (uint8_t*)&( jt808_param.id_0x002C ) },    //uint32_t  id_0x002C;   /*0x002c ȱʡ����㱨���*/
	{ 0x002D, TYPE_DWORD,	 (uint8_t*)&( jt808_param.id_0x002D ) },    //uint32_t  id_0x002D;   /*0x002d ��ʻԱδ��¼�㱨������*/
	{ 0x002E, TYPE_DWORD,	 (uint8_t*)&( jt808_param.id_0x002E ) },    //uint32_t  id_0x002E;   /*0x002e ����ʱ����㱨���*/
	{ 0x002F, TYPE_DWORD,	 (uint8_t*)&( jt808_param.id_0x002F ) },    //uint32_t  id_0x002F;   /*0x002f ����ʱ����㱨���*/
	{ 0x0030, TYPE_DWORD,	 (uint8_t*)&( jt808_param.id_0x0030 ) },    //uint32_t  id_0x0030;   /*0x0030 �յ㲹���Ƕ�*/
	{ 0x0031, TYPE_DWORD,	 (uint8_t*)&( jt808_param.id_0x0031 ) },    //uint16_t  id_0x0031;   /*0x0031 ����Χ���뾶���Ƿ�λ����ֵ������λΪ��*/
	{ 0x0040, TYPE_STR,		 (uint8_t*)&( jt808_param.id_0x0040 ) },    //char   id_0x0040[32];  /*0x0040 ���ƽ̨�绰����*/
	{ 0x0041, TYPE_STR,		 (uint8_t*)&( jt808_param.id_0x0041 ) },    //char   id_0x0041[32];  /*0x0041 ��λ�绰����*/
	{ 0x0042, TYPE_STR,		 (uint8_t*)&( jt808_param.id_0x0042 ) },    //char   id_0x0042[32];  /*0x0042 �ָ��������õ绰����*/
	{ 0x0043, TYPE_STR,		 (uint8_t*)&( jt808_param.id_0x0043 ) },    //char   id_0x0043[32];  /*0x0043 ���ƽ̨SMS����*/
	{ 0x0044, TYPE_STR,		 (uint8_t*)&( jt808_param.id_0x0044 ) },    //char   id_0x0044[32];  /*0x0044 �����ն�SMS�ı���������*/
	{ 0x0045, TYPE_DWORD,	 (uint8_t*)&( jt808_param.id_0x0045 ) },    //uint32_t  id_0x0045;   /*0x0045 �ն˽����绰����*/
	{ 0x0046, TYPE_DWORD,	 (uint8_t*)&( jt808_param.id_0x0046 ) },    //uint32_t  id_0x0046;   /*0x0046 ÿ��ͨ��ʱ��*/
	{ 0x0047, TYPE_DWORD,	 (uint8_t*)&( jt808_param.id_0x0047 ) },    //uint32_t  id_0x0047;   /*0x0047 ����ͨ��ʱ��*/
	{ 0x0048, TYPE_STR,		 (uint8_t*)&( jt808_param.id_0x0048 ) },    //char   id_0x0048[32];  /*0x0048 �����绰����*/
	{ 0x0049, TYPE_STR,		 (uint8_t*)&( jt808_param.id_0x0049 ) },    //char   id_0x0049[32];  /*0x0049 ���ƽ̨��Ȩ���ź���*/
	{ 0x0050, TYPE_DWORD,	 (uint8_t*)&( jt808_param.id_0x0050 ) },    //uint32_t  id_0x0050;   /*0x0050 ����������*/
	{ 0x0051, TYPE_DWORD,	 (uint8_t*)&( jt808_param.id_0x0051 ) },    //uint32_t  id_0x0051;   /*0x0051 ���������ı�SMS����*/
	{ 0x0052, TYPE_DWORD,	 (uint8_t*)&( jt808_param.id_0x0052 ) },    //uint32_t  id_0x0052;   /*0x0052 �������տ���*/
	{ 0x0053, TYPE_DWORD,	 (uint8_t*)&( jt808_param.id_0x0053 ) },    //uint32_t  id_0x0053;   /*0x0053 ��������洢��־*/
	{ 0x0054, TYPE_DWORD,	 (uint8_t*)&( jt808_param.id_0x0054 ) },    //uint32_t  id_0x0054;   /*0x0054 �ؼ���־*/
	{ 0x0055, TYPE_DWORD,	 (uint8_t*)&( jt808_param.id_0x0055 ) },    //uint32_t  id_0x0055;   /*0x0055 ����ٶ�kmh*/
	{ 0x0056, TYPE_DWORD,	 (uint8_t*)&( jt808_param.id_0x0056 ) },    //uint32_t  id_0x0056;   /*0x0056 ���ٳ���ʱ��*/
	{ 0x0057, TYPE_DWORD,	 (uint8_t*)&( jt808_param.id_0x0057 ) },    //uint32_t  id_0x0057;   /*0x0057 ������ʻʱ������*/
	{ 0x0058, TYPE_DWORD,	 (uint8_t*)&( jt808_param.id_0x0058 ) },    //uint32_t  id_0x0058;   /*0x0058 �����ۼƼ�ʻʱ������*/
	{ 0x0059, TYPE_DWORD,	 (uint8_t*)&( jt808_param.id_0x0059 ) },    //uint32_t  id_0x0059;   /*0x0059 ��С��Ϣʱ��*/
	{ 0x005A, TYPE_DWORD,	 (uint8_t*)&( jt808_param.id_0x005A ) },    //uint32_t  id_0x005A;   /*0x005A �ͣ��ʱ��*/
	{ 0x005B, TYPE_WORD,	 (uint8_t*)&( jt808_param.id_0x005B ) },    //uint16_t  id_0x005B;   /*0x005B ���ٱ���Ԥ����ֵ����λΪ 1/10Km/h */
	{ 0x005C, TYPE_WORD,	 (uint8_t*)&( jt808_param.id_0x005C ) },    //uint16_t  id_0x005C;   /*0x005C ƣ�ͼ�ʻԤ����ֵ����λΪ�루s����>0*/
	{ 0x005D, TYPE_WORD,	 (uint8_t*)&( jt808_param.id_0x005D ) },    //uint16_t  id_0x005D;   /*0x005D ��ײ������������:b7..0����ײʱ��(4ms) b15..8����ײ���ٶ�(0.1g) 0-79 ֮�䣬Ĭ��Ϊ10 */
	{ 0x005E, TYPE_WORD,	 (uint8_t*)&( jt808_param.id_0x005E ) },    //uint16_t  id_0x005E;   /*0x005E �෭�����������ã� �෭�Ƕȣ���λ 1 �ȣ�Ĭ��Ϊ 30 ��*/
	{ 0x0064, TYPE_DWORD,	 (uint8_t*)&( jt808_param.id_0x0064 ) },    //uint32_t  id_0x0064;   /*0x0064 ��ʱ���տ���*/
	{ 0x0065, TYPE_DWORD,	 (uint8_t*)&( jt808_param.id_0x0065 ) },    //uint32_t  id_0x0065;   /*0x0065 �������տ���*/
	{ 0x0070, TYPE_DWORD,	 (uint8_t*)&( jt808_param.id_0x0070 ) },    //uint32_t  id_0x0070;   /*0x0070 ͼ����Ƶ����(1-10)*/
	{ 0x0071, TYPE_DWORD,	 (uint8_t*)&( jt808_param.id_0x0071 ) },    //uint32_t  id_0x0071;   /*0x0071 ����*/
	{ 0x0072, TYPE_DWORD,	 (uint8_t*)&( jt808_param.id_0x0072 ) },    //uint32_t  id_0x0072;   /*0x0072 �Աȶ�*/
	{ 0x0073, TYPE_DWORD,	 (uint8_t*)&( jt808_param.id_0x0073 ) },    //uint32_t  id_0x0073;   /*0x0073 ���Ͷ�*/
	{ 0x0074, TYPE_DWORD,	 (uint8_t*)&( jt808_param.id_0x0074 ) },    //uint32_t  id_0x0074;   /*0x0074 ɫ��*/
	{ 0x0080, TYPE_DWORD,	 (uint8_t*)&( jt808_param.id_0x0080 ) },    //uint32_t  id_0x0080;   /*0x0080 ������̱����0.1km*/
	{ 0x0081, TYPE_WORD,	 (uint8_t*)&( jt808_param.id_0x0081 ) },    //uint16_t  id_0x0081;   /*0x0081 ʡ��ID*/
	{ 0x0082, TYPE_WORD,	 (uint8_t*)&( jt808_param.id_0x0082 ) },    //uint16_t  id_0x0082;   /*0x0082 ����ID*/
	{ 0x0083, TYPE_STR,		 (uint8_t*)&( jt808_param.id_0x0083 ) },    //char   id_0x0083[32];  /*0x0083 ����������*/
	{ 0x0084, TYPE_BYTE,	 (uint8_t*)&( jt808_param.id_0x0084 ) },    //uint8_t		 id_0x0084;      /*0x0084 ������ɫ	1��ɫ 2��ɫ 3��ɫ 4��ɫ 9����*/
	{ 0x0090, TYPE_BYTE,	 (uint8_t*)&( jt808_param.id_0x0090 ) },    //uint8_t		 id_0x0090;      /*0x0090 GNSS ��λģʽ*/
	{ 0x0091, TYPE_BYTE,	 (uint8_t*)&( jt808_param.id_0x0091 ) },    //uint8_t		 id_0x0091;      /*0x0091 GNSS ������*/
	{ 0x0092, TYPE_BYTE,	 (uint8_t*)&( jt808_param.id_0x0092 ) },    //uint8_t		 id_0x0092;      /*0x0092 GNSS ģ����ϸ��λ�������Ƶ��*/
	{ 0x0093, TYPE_DWORD,	 (uint8_t*)&( jt808_param.id_0x0093 ) },    //uint32_t  id_0x0093;   /*0x0093 GNSS ģ����ϸ��λ���ݲɼ�Ƶ��*/
	{ 0x0094, TYPE_BYTE,	 (uint8_t*)&( jt808_param.id_0x0094 ) },    //uint8_t		 id_0x0094;      /*0x0094 GNSS ģ����ϸ��λ�����ϴ���ʽ*/
	{ 0x0095, TYPE_DWORD,	 (uint8_t*)&( jt808_param.id_0x0095 ) },    //uint32_t  id_0x0095;   /*0x0095 GNSS ģ����ϸ��λ�����ϴ�����*/
	{ 0x0100, TYPE_DWORD,	 (uint8_t*)&( jt808_param.id_0x0100 ) },    //uint32_t  id_0x0100;   /*0x0100 CAN ����ͨ�� 1 �ɼ�ʱ����(ms)��0 ��ʾ���ɼ�*/
	{ 0x0101, TYPE_WORD,	 (uint8_t*)&( jt808_param.id_0x0101 ) },    //uint16_t  id_0x0101;   /*0x0101 CAN ����ͨ�� 1 �ϴ�ʱ����(s)��0 ��ʾ���ϴ�*/
	{ 0x0102, TYPE_DWORD,	 (uint8_t*)&( jt808_param.id_0x0102 ) },    //uint32_t  id_0x0102;   /*0x0102 CAN ����ͨ�� 2 �ɼ�ʱ����(ms)��0 ��ʾ���ɼ�*/
	{ 0x0103, TYPE_WORD,	 (uint8_t*)&( jt808_param.id_0x0103 ) },    //uint16_t  id_0x0103;   /*0x0103 CAN ����ͨ�� 2 �ϴ�ʱ����(s)��0 ��ʾ���ϴ�*/
	{ 0x0110, TYPE_BYTE | 8, (uint8_t*)&( jt808_param.id_0x0110 ) },    //uint8_t		 id_0x0110[8];	 /*0x0110 CAN ���� ID �����ɼ�����*/
	{ 0x0111, TYPE_BYTE | 8, (uint8_t*)&( jt808_param.id_0x0111 ) },    //uint8_t		 id_0x0111[8];	 /*0x0111 ����CAN ���� ID �����ɼ�����*/
	{ 0x0112, TYPE_BYTE | 8, (uint8_t*)&( jt808_param.id_0x0112 ) },    //uint8_t		 id_0x0112[8];	 /*0x0112 ����CAN ���� ID �����ɼ�����*/
	{ 0x0113, TYPE_BYTE | 8, (uint8_t*)&( jt808_param.id_0x0113 ) },    //uint8_t		 id_0x0113[8];	 /*0x0113 ����CAN ���� ID �����ɼ�����*/
	{ 0x0114, TYPE_BYTE | 8, (uint8_t*)&( jt808_param.id_0x0114 ) },    //uint8_t		 id_0x0114[8];	 /*0x0114 ����CAN ���� ID �����ɼ�����*/
	{ 0x0115, TYPE_BYTE | 8, (uint8_t*)&( jt808_param.id_0x0115 ) },    //uint8_t		 id_0x0115[8];	 /*0x0115 ����CAN ���� ID �����ɼ�����*/
	{ 0x0116, TYPE_BYTE | 8, (uint8_t*)&( jt808_param.id_0x0116 ) },    //uint8_t		 id_0x0116[8];	 /*0x0116 ����CAN ���� ID �����ɼ�����*/
	{ 0x0117, TYPE_BYTE | 8, (uint8_t*)&( jt808_param.id_0x0117 ) },    //uint8_t		 id_0x0117[8];	 /*0x0117 ����CAN ���� ID �����ɼ�����*/
	{ 0x0118, TYPE_BYTE | 8, (uint8_t*)&( jt808_param.id_0x0118 ) },    //uint8_t		 id_0x0118[8];	 /*0x0118 ����CAN ���� ID �����ɼ�����*/
	{ 0x0119, TYPE_BYTE | 8, (uint8_t*)&( jt808_param.id_0x0119 ) } //uint8_t		 id_0x0119[8];	 /*0x0119 ����CAN ���� ID �����ɼ�����*/
};


/***********************************************************
* Function:
* Description:
* Input:
* Input:
* Output:
* Return:
* Others:
***********************************************************/
uint8_t param_put( uint16_t id, uint8_t len, uint8_t* value )
{
	int		i, j;
	uint8_t *psrc, *pdst;

	for( i = 0; i < sizeof( tbl_id_lookup ) / sizeof( struct _tbl_id_lookup ); i++ )
	{
		if( id == tbl_id_lookup[i].id )
		{
			if( ( tbl_id_lookup[i].type == TYPE_DWORD ) && ( len == 4 ) )
			{
				psrc = value;
				//printf("psrc=%02x %02x %02x %02x \r\n",*(psrc+3),*(psrc+2),*(psrc+1),*(psrc+0));
				pdst	= tbl_id_lookup[i].val;
				*pdst++ = *( psrc + 3 );
				*pdst++ = *( psrc + 2 );
				*pdst++ = *( psrc + 1 );
				*pdst	= *( psrc + 0 );
				return 0;
			}
			if( ( tbl_id_lookup[i].type == TYPE_WORD ) && ( len == 2 ) )
			{
				psrc	= value;
				pdst	= tbl_id_lookup[i].val;
				*pdst++ = *( psrc + 1 );
				*pdst	= *psrc;
				return 0;
			}
			if( ( tbl_id_lookup[i].type == TYPE_BYTE ) && ( len == 1 ) )
			{
				psrc	= value;
				pdst	= tbl_id_lookup[i].val;
				*pdst	= *psrc;
				return 0;
			}
			if( tbl_id_lookup[i].type == TYPE_CAN_ID )
			{
				psrc	= value;
				pdst	= tbl_id_lookup[i].val;
				*pdst++ = *( psrc + 3 );
				*pdst++ = *( psrc + 2 );
				*pdst++ = *( psrc + 1 );
				*pdst++ = *psrc;
				*pdst++ = *( psrc + 7 );
				*pdst++ = *( psrc + 6 );
				*pdst++ = *( psrc + 5 );
				*pdst	= *( psrc + 4 );
				return 0;
			}
			if( tbl_id_lookup[i].type == TYPE_STR )
			{
				psrc = tbl_id_lookup[i].val;
				strncpy( psrc, value, 32 );
				*( psrc + 31 ) = 0;
				return 0;
			}
		}
	}
	return 1;
}

/*���ò���*/
void param_put_int( uint16_t id, uint32_t value )
{
	uint32_t	i, j;
	int			len;
	uint8_t		*p;

	for( i = 0; i < sizeof( tbl_id_lookup ) / sizeof( struct _tbl_id_lookup ); i++ )
	{
		if( id == tbl_id_lookup[i].id )
		{
			p		= tbl_id_lookup[i].val;
			j		= value;
			*p++	= ( j & 0xff );
			*p++	= j >> 8;
			*p++	= j >> 16;
			*p		= j >> 24;
			break;
		}
	}
}

/***********************************************************
* Function:
* Description:
* Input:
* Input:
* Output:
* Return:
* Others:
***********************************************************/
static void param_put_str( uint16_t id, uint8_t* value )
{
	int		i, j;
	int		len;
	uint8_t *p;

	for( i = 0; i < sizeof( tbl_id_lookup ) / sizeof( struct _tbl_id_lookup ); i++ )
	{
		if( id == tbl_id_lookup[i].id )
		{
			p = tbl_id_lookup[i].val;
			strncpy( p, (char*)value, 32 );
			break;
		}
	}
}

/*��ȡ����,���ز������Ͳ���*/
uint8_t param_get( uint16_t id, uint8_t* value )
{
	int			i;
	uint8_t		*p;
	uint32_t	val;
	for( i = 0; i < sizeof( tbl_id_lookup ) / sizeof( struct _tbl_id_lookup ); i++ )
	{
		if( id == tbl_id_lookup[i].id )
		{
			if( tbl_id_lookup[i].type == TYPE_DWORD )
			{
				p = tbl_id_lookup[i].val;
				memcpy( value, p, 4 );
				return 4;
			}

			if( tbl_id_lookup[i].type == TYPE_WORD )
			{
				p = tbl_id_lookup[i].val;
				memcpy( value, p, 2 );
				return 2;
			}

			if( tbl_id_lookup[i].type == TYPE_BYTE )
			{
				p		= tbl_id_lookup[i].val;
				*value	= *p;
				return 1;
			}
			if( tbl_id_lookup[i].type == TYPE_STR )
			{
				p = tbl_id_lookup[i].val;
				memcpy( value, p, strlen( p ) );
				return strlen( p );
			}
			if( tbl_id_lookup[i].type == TYPE_CAN_ID )
			{
				p = tbl_id_lookup[i].val;
				memcpy( value, p, 8 );
				return 8;
			}
		}
	}
	return 0;
}

/*��ȡ����*/
uint32_t param_get_int( uint16_t id )
{
	int			i;
	uint8_t		*p;
	uint32_t	val = 0;
	for( i = 0; i < sizeof( tbl_id_lookup ) / sizeof( struct _tbl_id_lookup ); i++ )
	{
		if( id == tbl_id_lookup[i].id )
		{
			p	= tbl_id_lookup[i].val;
			val |= ( *p++ );
			val |= ( ( *p++ ) << 8 );
			val |= ( ( *p++ ) << 16 );
			val |= ( ( *p ) << 24 );
			return val;
		}
	}
}

/***********************************************************
* Function:
* Description:
* Input:
* Input:
* Output:
* Return:
* Others:
***********************************************************/
void param_print( void )
{
	int			i, j, id;
	int			type;
	uint8_t		*p;
	uint32_t	val = 0;

	for( i = 0; i < sizeof( tbl_id_lookup ) / sizeof( struct _tbl_id_lookup ); i++ )
	{
		id	= tbl_id_lookup[i].id;
		p	= tbl_id_lookup[i].val;
		val = 0;
		switch( tbl_id_lookup[i].type )
		{
			case TYPE_DWORD: /*�ֽڶ��뷽ʽ little_endian*/
				val |= ( *p++ );
				val |= ( ( *p++ ) << 8 );
				val |= ( ( *p++ ) << 16 );
				val |= ( ( *p ) << 24 );
				printf( "\r\nid=%04x value=%08x\r\n", id, val );
				break;
			case TYPE_CAN_ID:
				val |= ( *p++ );
				val |= ( ( *p++ ) << 8 );
				val |= ( ( *p++ ) << 16 );
				val |= ( ( *p++ ) << 24 );
				printf( "\r\nid=%04x value=%08x", id, val );
				val = 0;
				val |= ( *p++ );
				val |= ( ( *p++ ) << 8 );
				val |= ( ( *p++ ) << 16 );
				val |= ( ( *p ) << 24 );
				printf( " %08x\r\n", val );
				break;

				break;
			case TYPE_STR:
				printf( "\r\nid=%04x value=%s\r\n", id, p );
				break;
		}
	}
}

/*��ӡ������Ϣ*/
void param_dump( void )
{
	uint8_t tbl[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
	int		i, count = 0;
	uint8_t c;
	uint8_t *p = (uint8_t*)&jt808_param;
	uint8_t printbuf[70];
	int32_t len = sizeof( jt808_param );

	while( len > 0 )
	{
		count = ( len < 16 ) ? len : 16;
		memset( printbuf, 0x20, 70 );
		for( i = 0; i < count; i++ )
		{
			c					= *p;
			printbuf[i * 3]		= tbl[c >> 4];
			printbuf[i * 3 + 1] = tbl[c & 0x0f];
			if( c < 0x20 )
			{
				c = '.';
			}
			if( c > 0x7f )
			{
				c = '.';
			}
			printbuf[50 + i] = c;
			p++;
		}
		printbuf[69] = 0;
		printf("%s\r\n", printbuf );
		len -= count;
	}
}

/************************************** The End Of File **************************************/
