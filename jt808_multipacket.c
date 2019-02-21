/*
   jt808������ͺͲ�����ʵ��

 */

#include "jt808_multipacket.h"
#include "jt808.h"

typedef struct _jt808_tx_msg_multipacket_nodedata
{
/*���ͻ������*/
	uint8_t			linkno;     /*����ʹ�õ�link,������Э���Զ��socket*/
	JT808_MSG_TYPE	type;
	JT808_MSG_STATE state;      /*����״̬*/
	uint32_t		retry;      /*�ش�����,�������ݼ��Ҳ���*/
	uint32_t		max_retry;  /*����ش�����*/
	uint32_t		timeout;    /*��ʱʱ��*/
	uint32_t		tick;       /*����ʱ��*/
/*���յĴ����ж����*/
	uint16_t	head_id;        /*��ϢID*/
	uint16_t	head_sn;        /*��Ϣ��ˮ��*/
/*��ʵ�ķ�������*/
	uint16_t	msg_len;        /*��Ϣ����*/
	void	(*get_data)(void *pmsgout);  /*��ȡҪ���͵���Ϣ*/
	
}JT808_TX_MSG_MULTIPACKET_NODEDATA;




typedef  struct
{
	uint16_t id;
	uint16_t attr;
	uint8_t  mobile[6];
	uint16_t seq;
}MSG_HEAD;


typedef  struct
{
	uint16_t id;
	uint16_t attr;
	uint8_t  mobile[6];
	uint16_t seq;
	uint16_t packet_count;	/*�ܰ���*/
	uint16_t packet_no;		/*��ǰ�����*/
}MSG_HEAD_EX;




typedef  struct
{
	uint8_t chn;	/*ͨ��*/
	uint16_t cmd;
	uint16_t interval;
	uint8_t flag_save;
	uint8_t	not_used[6];
}CMD_0x8801;















/*
�����Ϣ�Ľ��մ���ֻҪ������Ϣ���������зְ���

 0x8700 �г���¼�����ݲɼ�

 0x8800 ��ý�������ϴ�Ӧ��-�������

 0x8801 ����ͷ�������գ�

 0x8804 ¼��

*/
int handle_rx_multipacket( uint8_t linkno, uint8_t *pmsg )
{
	uint16_t id;
	uint8_t *psrc;

	psrc=pmsg;
	id=(*psrc<<8)|(*(psrc+1));

	switch(id)
	{
		case 0x8801:		/*��������*/
			
			break;


	}




}





void jt808_tx_response_multipacket( uint8_t linkno, uint8_t *pmsg )
{


}


void jt808_tx_timeout_multipacket( uint8_t linkno, uint8_t *pmsg )
{


}




/***********************************************************
* Function: jt808������������͹���
* Description:
* Input:
* Input:
* Output:
* Return:
* Others:
***********************************************************/
void jt808_multipacket_process( void )
{




}



/*
�����г���¼������0x0700
*/

void multipacket_send_0x0700(uint16_t center_sq,uint8_t *pinfo,uint16_t len)
{



}





/************************************** The End Of File **************************************/
