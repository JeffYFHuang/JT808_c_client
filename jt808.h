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
#ifndef _H_JT808_H_
#define _H_JT808_H_

#include <stdint.h>
#include "msglist.h"

#define   MsgQ_Timeout 3


/*
   �洢�������,���þ��Ե�ַ,��4K(0x1000)Ϊһ������
 */

#define ADDR_PARAM 0x000000000

//------- �ı���Ϣ --------
typedef struct _TEXT_INFO
{
	uint8_t TEXT_FLAG;          //  �ı���־
	uint8_t TEXT_SD_FLAG;       // ���ͱ�־λ
	uint8_t TEXT_Content[100];  // �ı�����
}TEXT_INFO;

//----- ��Ϣ ----
typedef struct _MSG_TEXT
{
	uint8_t TEXT_mOld;          //  ���µ�һ����Ϣ  дΪ1���������µ�һ����Ϣ
	uint8_t TEXT_TYPE;          //  ��Ϣ����   1-8  �еڼ���
	uint8_t TEXT_LEN;           //  ��Ϣ����
	uint8_t TEXT_STR[150];      //  ��Ϣ����
}MSG_TEXT;

//-----  ���� ------
typedef struct _CENTER_ASK
{
	uint8_t		ASK_SdFlag;     //  ��־λ           ���� TTS  1  ��   TTS ����  2
	uint16_t	ASK_floatID;    // ������ˮ��
	uint8_t		ASK_infolen;    // ��Ϣ����
	uint8_t		ASK_answerID;   // �ظ�ID
	uint8_t		ASK_info[30];   //  ��Ϣ����
	uint8_t		ASK_answer[30]; // ��ѡ��
}CENTRE_ASK;

//------ �¼�  -------
typedef struct _EVENT           //  name: event
{
	uint8_t Event_ID;           //  �¼�ID
	uint8_t Event_Len;          //  �¼�����
	uint8_t Event_Effective;    //  �¼��Ƿ���Ч��   1 ΪҪ��ʾ  0
	uint8_t Event_Str[20];      //  �¼�����
}EVENT;

//----- ��Ϣ ----
typedef struct _MSG_BROADCAST   // name: msg_broadcast
{
	uint8_t		INFO_TYPE;      //  ��Ϣ����
	uint16_t	INFO_LEN;       //  ��Ϣ����
	uint8_t		INFO_PlyCancel; // �㲥/ȡ����־      0 ȡ��  1  �㲥
	uint8_t		INFO_SDFlag;    //  ���ͱ�־λ
	uint8_t		INFO_Effective; //  ��ʾ�Ƿ���Ч   1 ��ʾ��Ч    0  ��ʾ��Ч
	uint8_t		INFO_STR[30];   //  ��Ϣ����
}MSG_BRODCAST;

//------ �绰�� -----
typedef struct _PHONE_BOOK      // name: phonebook
{
	uint8_t CALL_TYPE;          // ��������  1 ���� 2 ���� 3 ����/����
	uint8_t NumLen;             // ���볤��
	uint8_t UserLen;            // ��ϵ�˳���
	uint8_t Effective_Flag;     // ��Ч��־λ   ��Ч 0 ����Ч  1
	uint8_t NumberStr[20];      // �绰����
	uint8_t UserStr[10];        // ��ϵ������  GBK ����
}PHONE_BOOK;

typedef struct _MULTIMEDIA
{
	uint32_t Media_ID;               //   ��ý������ID
	uint8_t	Media_Type;             //   0:   ͼ��    1 : ��Ƶ    2:  ��Ƶ
	uint8_t	Media_CodeType;         //   �����ʽ  0 : JPEG  1:TIF  2:MP3  3:WAV  4: WMV
	uint8_t	Event_Code;             //   �¼�����  0: ƽ̨�·�ָ��  1: ��ʱ����  2 : ���ٱ������� 3: ��ײ�෭�������� ��������
	uint8_t	Media_Channel;          //   ͨ��ID
	//----------------------
	uint8_t	SD_Eventstate;          // �����¼���Ϣ�ϴ�״̬    0 ��ʾ����   1  ��ʾ���ڷ���״̬
	uint8_t	SD_media_Flag;          // ����û���¼���Ϣ��־λ
	uint8_t	SD_Data_Flag;           // �������ݱ�־λ
	uint8_t	SD_timer;               // ���Ͷ�ʱ��
	uint8_t	MaxSd_counter;          // ����ʹ���
	uint8_t	Media_transmittingFlag; // ��ý�崫������״̬  1: ��ý�崫��ǰ����1����λ��Ϣ    2 :��ý�����ݴ�����  0:  δ���ж�ý�����ݴ���
	uint16_t Media_totalPacketNum;   // ��ý���ܰ���
	uint16_t Media_currentPacketNum; // ��ý�嵱ǰ����
	//----------------------
	uint8_t	RSD_State;              //  �ش�״̬   0 : �ش�û������   1 :  �ش���ʼ    2  : ��ʾ˳���굫�ǻ�û�յ����ĵ��ش�����
	uint8_t	RSD_Timer;              //  ��״̬�µļ�����
	uint8_t	RSD_Reader;             //  �ش���������ǰ��ֵ
	uint8_t	RSD_total;              //  �ش�ѡ����Ŀ

	uint8_t Media_ReSdList[10];      //  ��ý���ش���Ϣ�б�
}MULTIMEDIA;

//--------------ÿ���ӵ�ƽ���ٶ�
typedef  struct AvrgMintSpeed
{
	uint8_t datetime[6];        //current
	uint8_t datetime_Bak[6];    //Bak
	uint8_t avgrspd[60];
	uint8_t saveFlag;
}Avrg_MintSpeed;

extern TEXT_INFO	TextInfo;
//-------�ı���Ϣ-------
extern MSG_TEXT		TEXT_Obj;
extern MSG_TEXT		TEXT_Obj_8[8], TEXT_Obj_8bak[8];

//------ ����  --------
extern CENTRE_ASK ASK_Centre;                       // ��������

//------- �¼� ----
extern EVENT	EventObj;                           // �¼�
extern EVENT	EventObj_8[8];                      // �¼�

//------  ��Ϣ�㲥  ---
extern MSG_BRODCAST MSG_BroadCast_Obj;              // ��Ϣ�㲥
extern MSG_BRODCAST MSG_Obj_8[8];                   // ��Ϣ�㲥

//------  �绰��  -----
extern PHONE_BOOK		PhoneBook, Rx_PhoneBOOK;    //  �绰��
extern PHONE_BOOK		PhoneBook_8[8];

extern MULTIMEDIA		MediaObj;                   // ��ý����Ϣ

extern uint8_t			CarLoadState_Flag;          //ѡ�г���״̬�ı�־   1:�ճ�   2:���   3:�س�
extern uint8_t			Warn_Status[4];

extern uint16_t				ISP_total_packnum;          // ISP  �ܰ���
extern uint16_t				ISP_current_packnum;        // ISP  ��ǰ����

extern uint8_t				APN_String[30];

extern uint8_t				Camera_Number;
extern uint8_t				Duomeiti_sdFlag;

extern Avrg_MintSpeed	Avrgspd_Mint;
extern uint8_t				avgspd_Mint_Wr; // ��дÿ����ƽ���ٶȼ�¼�±�

/*for new use*/

typedef struct
{
	int		id;
	short	attr;
	int		latitute;   /*�Զ�λ��λ��γ��ֵ����10��6�η�����ȷ�������֮һ��*/
	int		longitute;
	int		radius;     /*��λΪ��m��·��Ϊ�ùյ㵽��һ�յ�*/
	char	start[6];
	char	end[6];
	short	speed;
	char	interval;   /*����ʱ��,��*/
}GPS_AREA_CIRCLE;

typedef enum
{
	T_NODEF = 1,
	T_BYTE,
	T_WORD,
	T_DWORD,
	T_STRING,
}PARAM_TYPE;

/*�ն˲�������*/
typedef  struct
{
	uint8_t		id;
	PARAM_TYPE	type;
	void		* pvalue;
}PARAM;

/*�ն˲�������*/
typedef struct
{
	PARAM_TYPE	type;
	void		* pvalue;
}PARAM_BODY;

#if 0
typedef struct
{
	uint32_t ver; /*�汾��Ϣ�ĸ��ֽ�yy_mm_dd_build,�Ƚϴ�С*/
/*����ע����Ϣ*/
	uint16_t	id0100_1_w;
	uint16_t	id0100_2_w;
	uint8_t		id0100_3_s[5];
	uint8_t		id0100_4_s[8];
	uint8_t		id0100_5_s[7];
	uint8_t		id0100_6_b;
	uint8_t		id0100_7_s[12];

/*�����й�*/
	char	apn[32];
	char	user[32];
	char	psw[32];
	char	mobile[6];
/*�������*/
	uint32_t	timeout_udp;    /*udp���䳬ʱʱ��*/
	uint32_t	retry_udp;      /*udp�����ش�����*/
	uint32_t	timeout_tcp;    /*udp���䳬ʱʱ��*/
	uint32_t	retry_tcp;      /*udp�����ش�����*/
}JT808_PARAM;

#endif

typedef enum
{
	GET_DATA=1,					/*��ȡ����*/
	IDLE,						/*���еȴ�����*/
	WAIT_ACK,                   /*�ȴ�ACK��*/
	ACK_OK,                     /*���յ�ACKӦ��*/
} JT808_MSG_STATE;

typedef enum
{
	TERMINAL_CMD = 1,
	TERMINAL_ACK,
	CENTER_CMD,
	CENTER_ACK
}JT808_MSG_TYPE;

typedef struct JT808_MSG_HEAD
{
	uint16_t	id;
	uint16_t	attr;
	uint8_t		mobile[6];
	uint16_t	seq;
}JT808_MSG_HEAD;


/*
   �洢jt808���͵������Ϣ
 */

typedef struct _jt808_tx_msg_nodedata_old
{
/*���ͻ������*/
	uint8_t			linkno;                             /*����ʹ�õ�link,������Э���Զ��socket*/
	JT808_MSG_TYPE	type;                               /*������Ϣ������*/
	JT808_MSG_STATE state;                              /*����״̬*/
	uint32_t		retry;                              /*�ش�����,�������ݼ��Ҳ���*/
	uint32_t		max_retry;                          /*����ش�����*/
	uint32_t		timeout;                            /*��ʱʱ��*/
	uint32_t		tick;                               /*����ʱ��*/
/*���յĴ����ж����*/
	void ( *cb_tx_timeout )( struct _jt808_tx_msg_nodedata *pnodedata );
	void ( *cb_tx_response )( uint8_t linkno, uint8_t *pmsg );
	uint16_t	head_id;                                /*��ϢID*/
	uint16_t	head_sn;                                /*��Ϣ��ˮ��*/
/*��ʵ�ķ�������*/
	uint16_t	msg_len;                                /*��Ϣ����*/
	uint8_t		*pmsg;                                  /*������Ϣ��,��ʵ��Ҫ���͵����ݸ�ʽ������ת���FCS���<7e>Ϊ��־*/
}JT808_TX_MSG_NODEDATA_OLD;

typedef struct _jt808_tx_msg_nodedata
{
/*���ͻ������*/
	uint8_t			linkno;                             /*����ʹ�õ�link,������Э���Զ��socket*/
	uint8_t			multipacket;                        /*�ǲ��Ƕ������*/
	JT808_MSG_TYPE	type;                               /*������Ϣ������*/
	JT808_MSG_STATE state;                              /*����״̬*/
	uint32_t		retry;                              /*�ش�����,�������ݼ��Ҳ���*/
	uint32_t		max_retry;                          /*����ش�����*/
	uint32_t		timeout;                            /*��ʱʱ��*/
	uint64_t		tick;                               /*����ʱ��*/
/*���յĴ����ж����*/
	void ( *cb_tx_timeout )( struct _jt808_tx_msg_nodedata *pnodedata );
	void ( *cb_tx_response )( struct _jt808_tx_msg_nodedata *pnodedata ,uint8_t linkno, uint8_t *pmsg );
	uint16_t	head_id;                                /*��ϢID*/
	uint16_t	head_sn;                                /*��Ϣ��ˮ��*/
	
/*������ʵ�ķ�������-��Ϣ��*/
	uint16_t	msg_len;                                /*��Ϣ����*/
	uint8_t		*pmsg;                                  /*ԭʼ��Ϣ,��Ҫ�ڷ���ʱת��,��Ϊ�������ʱ�õ�����ԭʼ��Ϣ��
                                                       ������808ת���M66��HEXת�壬����������RAMʹ��*/
/*������͵Ĵ���*/
	//uint8_t		stage;                                  /*�׶�*/
	uint16_t	packet_num;		/*�ܰ���*/
	uint16_t	packet_no;		/*��ǰ����*/
	uint32_t	size;                                   /*�ܵ����ݴ�С*/
	void 		*user_para;								/*get_data������Ҫ�Ĺؼ�ԭʼ���ݲ�����ͨ���ò�����ȡ��ȷ������*/
	//void		*user_data;								/*user_data,�����������ʾ��Ҫ���͵����ݰ��б���һ���ֽڵ�bit0Ϊ1��ʾ��Ҫ���͸ð����ݣ��Դ�����*/
	void (*init)(uint8_t id,...);
	uint16_t ( *get_data )( struct _jt808_tx_msg_nodedata *pnodedata ); /*��ȡҪ���͵���Ϣ*/
}JT808_TX_MSG_NODEDATA;

uint8_t gprs_rx( uint8_t linkno, uint8_t *pinfo, uint16_t length );

uint8_t jt808_add_tx_data( uint8_t linkno, JT808_MSG_TYPE type, uint16_t id, uint8_t *pinfo, uint16_t len );
static uint8_t auth_jt808_tx( void );

typedef enum
{
	SOCKET_STATE=0,		/*��ѯsocket״̬*/
	SOCKET_IDLE = 1,            /*��������*/
	SOCKET_ERR,
	SOCKET_START,				/*��������Զ��*/
	SOCKET_DNS,                 /*DNS��ѯ��*/
	SOCKET_DNS_ERR,
	SOCKET_CONNECT,             /*������*/
	SOCKET_CONNECT_ERR,         /*���Ӵ��󣬶Է���Ӧ��*/
	SOCKET_READY,               /*�����ɣ����Խ�������*/
	SOCKET_CLOSE,
}T_SOCKET_STATE;

typedef enum
{
	GSM_STATE_GET=0,		/*��ѯGSM״̬*/
	GSM_IDLE=1,			/*����*/
	GSM_POWERON,		/*�ϵ����̲�����ģ����AT������ʼ������*/
	GSM_AT,				/*����AT�������У���������socket�������շ�����*/
	GSM_AT_SEND,		/*����AT�շ�״̬*/
	GSM_GPRS,		/*��¼GPRS��*/
	GSM_TCPIP,			/*�Ѿ����������Խ���socket����*/
	GSM_SOCKET_PROC,	/*���ڽ���socket����*/
	GSM_ERR_POWERON,
	GSM_ERR_GPRS,
	GSM_ERR_TCPIP,
	GSM_POWEROFF,		/*�Ѿ��ϵ�*/
}T_GSM_STATE;

#endif

/************************************** The End Of File **************************************/
