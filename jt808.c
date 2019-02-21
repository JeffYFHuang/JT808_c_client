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
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/stat.h>

#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include "minmea.h"

#define INDENT_SPACES "  "

pthread_t tid[4];
pthread_mutex_t tx_lock;
pthread_mutex_t rx_lock;

#include "Err.h"
#include "jt808.h"
#include "msglist.h"
#include "jt808_sprintf.h"

#include "jt808_param.h"
#include "jt808_sms.h"
#include "jt808_gps.h"

#pragma diag_error 223

FILE *gps_fd;
#define gps_demo
#ifdef gps_demo
#define GPSPATH "./test.gps"
#else
#define GPSPATH "/dev/smd7"
#endif
int socketfd;

GPS_BASEINFO gps_baseinfo;

#define ByteSwap2( val )    \
    ( ( ( val & 0xff ) << 8 ) |   \
      ( ( val & 0xff00 ) >> 8 ) )

#define ByteSwap4( val )    \
    ( ( ( val & 0xff ) << 24 ) |   \
      ( ( val & 0xff00 ) << 8 ) |  \
      ( ( val & 0xff0000 ) >> 8 ) |  \
      ( ( val & 0xff000000 ) >> 24 ) )

typedef struct
{
	uint16_t id;
	int ( *func )( uint8_t linkno, uint8_t *pmsg );
}HANDLE_JT808_RX_MSG;
#if 0
static struct rt_mailbox	mb_gprsrx;
#define MB_GPRSDATA_POOL_SIZE 32
static uint8_t				mb_gprsrx_pool[MB_GPRSDATA_POOL_SIZE];

#define NO_COMMON_ACK

#define MB_TTS_POOL_SIZE 32
static struct rt_mailbox	mb_tts;
static uint8_t				mb_tts_pool[MB_TTS_POOL_SIZE];

#define MB_AT_TX_POOL_SIZE 32
static struct rt_mailbox	mb_at_tx;
static uint8_t				mb_at_tx_pool[MB_AT_TX_POOL_SIZE];

#define MB_VOICE_POOL_SIZE 32
static struct rt_mailbox	mb_voice;
static uint8_t				mb_voice_pool[MB_VOICE_POOL_SIZE];

#define MB_SMS_POOL_SIZE 32
static struct rt_mailbox	mb_sms;
static uint8_t				mb_sms_pool[MB_SMS_POOL_SIZE];
#endif
static uint16_t				tx_seq = 0;             /*�������*/

static uint16_t				total_send_error = 0;   /*�ܵķ��ͳ����������ﵽһ���Ĵ���Ҫ����M66*/

/*������Ϣ�б�*/
MsgList* list_jt808_tx;

/*������Ϣ�б�*/
MsgList				* list_jt808_rx;

typedef enum
{
	CONNECT_IDLE = 0,
	CONNECT_PEER,
	CONNECTED,
	CONNECT_ERROR,
}CONN_STATE;

struct
{
	uint32_t	disable_connect; /*��ֹ���ӱ�־��Э����� 0:��������*/
	CONN_STATE	server_state;
	uint8_t		server_index;
	CONN_STATE	auth_state;
	uint8_t		auth_index;
} connect_state =
{ 0, CONNECT_IDLE, 0, CONNECT_IDLE, 0 };

#if 0

T_SOCKET_STATE	socket_jt808_state	= SOCKET_IDLE;
uint16_t		socket_jt808_index	= 0;

T_SOCKET_STATE	socket_iccard_state = SOCKET_IDLE;
uint16_t		socket_iccard_index = 0;


/*
   ͬʱ׼���ÿ��õ��ĸ����ӣ�����Ҫ��ѡ����,����Ϊ
   ʵ���в�����ͬʱ�Զ�����ӽ�����ֻ�����η���������
   ��808������
   ����808������
   ��IC����Ȩ������
   ����IC����Ȩ������

 */
GSM_SOCKET gsm_socket[MAX_GSM_SOCKET];
#endif


/*
   jt808��ʽ���ݽ����ж�
   <��ʶ0x7e><��Ϣͷ><��Ϣ��><У����><��ʶ0x7e>

   ������Ч�����ݳ���,Ϊ0 �����д�

 */
static uint16_t jt808_decode_fcs( uint8_t * pinfo, uint16_t length )
{
	uint8_t		* psrc, * pdst;
	uint16_t	count, len;
	uint8_t		fstuff	= 0;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      /*�Ƿ��ֽ����*/
	uint8_t		fcs		= 0;

	if( length < 5 )
	{
		return 0;
	}
	if( *pinfo != 0x7e )
	{
		return 0;
	}
	if( *( pinfo + length - 1 ) != 0x7e )
	{
		return 0;
	}
	psrc	= pinfo + 1;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    /*1byte��ʶ��Ϊ��ʽ��Ϣ*/
	pdst	= pinfo;
	count	= 0;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            /*ת���ĳ���*/
	len		= length - 2;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   /*ȥ����ʶλ�����ݳ���*/

	while( len )
	{
		if( fstuff )
		{
			*pdst	= *psrc + 0x7c;
			fstuff	= 0;
			count++;
			fcs ^= *pdst;
		} else
		{
			if( *psrc == 0x7d )
			{
				fstuff = 1;
			} else
			{
				*pdst	= *psrc;
				fcs		^= *pdst;
				count++;
			}
		}
		psrc++;
		pdst++;
		len--;
	}
	if( fcs != 0 )
	{
		printf( "%s>fcs error\r\n", __func__ );
		return 0;
	}
	printf( "count=%d\r\n", count );
	return count;
}

/**���һ���ֽ�**/
static uint16_t jt808_pack_byte( uint8_t * buf, uint8_t * fcs, uint8_t data )
{
	uint8_t * p = buf;
	*fcs ^= data;
	if( ( data == 0x7d ) || ( data == 0x7e ) )
	{
		*p++	= 0x7d;
		*p		= ( data - 0x7c );
		return 2;
	} else
	{
		*p = data;
		return 1;
	}
}

/*���ݽ����ȣ����ڼ���*/
static uint16_t jt808_pack_int( uint8_t * buf, uint8_t * fcs, uint32_t data, uint8_t width )
{
	uint16_t count = 0;
	switch( width )
	{
		case 1:
			count += jt808_pack_byte( buf + count, fcs, ( data & 0xff ) );
			break;
		case 2:
			count	+= jt808_pack_byte( buf + count, fcs, ( data >> 8 ) );
			count	+= jt808_pack_byte( buf + count, fcs, ( data & 0xff ) );
			break;
		case 4:
			count	+= jt808_pack_byte( buf + count, fcs, ( data >> 24 ) );
			count	+= jt808_pack_byte( buf + count, fcs, ( data >> 16 ) );
			count	+= jt808_pack_byte( buf + count, fcs, ( data >> 8 ) );
			count	+= jt808_pack_byte( buf + count, fcs, ( data & 0xff ) );
			break;
	}
	return count;
}

/*����ַ���***/
static uint16_t jt808_pack_string( uint8_t * buf, uint8_t * fcs, char * str )
{
	uint16_t	count	= 0;
	char		* p		= str;
	while( *p )
	{
		count += jt808_pack_byte( buf + count, fcs, *p++ );
	}
	return count;
}

/**�������**/
static uint16_t jt808_pack_array( uint8_t *buf, uint8_t *fcs, uint8_t *src, uint16_t len )
{
	uint16_t	count = 0;
	int			i = 0;
	uint8_t		*p = src;
	for( i = 0; i < len; i++ )
	{
		count += jt808_pack_byte( buf + count, fcs, *p++ );
	}
	return count;
}

/*
   jt808�ն˷�����Ϣ
   ���������Ϣע�ᵽ������Ϣ�Ĵ����߳���
   ��Ҫ������ϢID,����Ϣ�壬��jt808_send�߳����
    ��Ϣ�����
    ���ͺ��ط�����
    ��ˮ��
    �ѷ���Ϣ�Ļ���free
   ���ݽ����ĸ�ʽ
   <msgid 2bytes><msg_len 2bytes><msgbody nbytes>

 */
static void jt808_send( void * parameter )
{
}

/*���ͺ��յ�Ӧ����*/
void jt808_tx_response( JT808_TX_MSG_NODEDATA * nodedata, uint8_t linkno, uint8_t *pmsg )
{
	uint8_t		* msg = pmsg + 12;
	uint16_t	id;
	uint16_t	seq;
	uint8_t		res;

	seq = ( *msg << 8 ) | *( msg + 1 );
	id	= ( *( msg + 2 ) << 8 ) | *( msg + 3 );
	res = *( msg + 4 );

	switch( id )                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        // �ж϶�Ӧ�ն���Ϣ��ID�����ִ���
	{
		case 0x0200:                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    //	��Ӧλ����Ϣ��Ӧ��
			printf( "\r\nCentre ACK!\r\n" );
			break;
		case 0x0002:                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    //	��������Ӧ��
			printf( "\r\n  Centre  Heart ACK!\r\n" );
			break;
		case 0x0101:
			printf("\r\n  test ACK!\r\n");
			break;
		case 0x0102:
		    switch (res) {
		        case 0://RESULT_OK:
		        	printf("auth resp ok\r\n");
		        	start_gps_send();
		        	break;
		        case 1://RESULT_FAIL:
		        	printf("auth resp fail\r\n");
		        	break;
		        case 2://RESULT_BAD_REQUEST:
		        	printf("auth resp bad request\r\n");
		        	break;
		        case 3://RESULT_UNSUPPORTED:
		        	printf("auth resp unsupported\r\n");
		        	break;
		        case 4://RESULT_CONFIRM:
		            printf("auth resp confirm\r\n");
		            break;
		        default:
		          printf("Unknown result.");
		    }
			break;
		case 0x0800:                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    // ��ý���¼���Ϣ�ϴ�
			break;
		case 0x0702:
			printf( "\r\n  ��ʻԱ��Ϣ�ϱ�---����Ӧ��!  \r\n" );
			break;
		case 0x0701:
			printf( "\r\n	�����˵��ϱ�---����Ӧ��!  \r\n");
			break;
		default:
			printf( "\r\nunknown id=%04x\r\n", id );
			break;
	}
}

/*
   ��Ϣ���ͳ�ʱ
 */
static uint8_t jt808_tx_timeout( JT808_TX_MSG_NODEDATA * nodedata )
{
	printf( "tx timeout\r\n" );
}

/*
   ���һ����Ϣ�������б���
 */
uint8_t jt808_add_tx_data( uint8_t linkno, JT808_MSG_TYPE type, uint16_t id, uint8_t *pinfo, uint16_t len )
{
	uint8_t					* pdata;
	JT808_TX_MSG_NODEDATA	* pnodedata;

	pnodedata = malloc( sizeof( JT808_TX_MSG_NODEDATA ) );
	if( pnodedata == NULL )
	{
		return -RT_ERROR;
	}
	memset(pnodedata,0,sizeof(JT808_TX_MSG_NODEDATA));
	pnodedata->linkno           = linkno;
	pnodedata->type				= type;
	pnodedata->state			= IDLE;
	pnodedata->retry			= 0;
	pnodedata->cb_tx_timeout	= jt808_tx_timeout;
	pnodedata->cb_tx_response	= jt808_tx_response;

	pdata = malloc( len );
	if( pdata == NULL )
	{
		free( pnodedata );
		return -RT_ERROR;
	}
	memcpy( pdata, pinfo, len );
	pnodedata->msg_len	= len;
	pnodedata->pmsg		= pdata;
	pnodedata->head_sn	= tx_seq;
	pnodedata->head_id	= id;
	msglist_append( list_jt808_tx, pnodedata );
	tx_seq++;
    return RT_OK;
}

/*
   �ն�ͨ��Ӧ��
 */
static uint8_t jt808_tx_0x0001( uint8_t linkno, uint16_t seq, uint16_t id, uint8_t res )
{
	uint8_t					* pdata;
	JT808_TX_MSG_NODEDATA	* pnodedata;
	uint8_t					buf [256];
	uint8_t					* p;
	uint16_t				len;

	pnodedata = malloc( sizeof( JT808_TX_MSG_NODEDATA ) );
	if( pnodedata == NULL )
	{
		return -RT_ERROR;
	}
	memset(pnodedata,0,sizeof(JT808_TX_MSG_NODEDATA));
	pnodedata->type				= TERMINAL_ACK;
	pnodedata->state			= IDLE;
	pnodedata->retry			= 0;
	pnodedata->cb_tx_timeout	= jt808_tx_timeout;
	pnodedata->cb_tx_response	= jt808_tx_response;

	len		= jt808_pack( buf, "%w%6s%w%w%w%b", 0x0001, term_param.mobile, tx_seq, seq, id, res );
	pdata	= malloc( len );
	if( pdata == NULL )
	{
		free( pnodedata );
		return -RT_ERROR;
	}
	memcpy( pdata, buf, len );
	pnodedata->msg_len	= len;
	pnodedata->pmsg		= pdata;
	msglist_append( list_jt808_tx, pnodedata );
	tx_seq++;
	return RT_OK;
}

/*
ƽ̨ͨ��Ӧ��,�յ���Ϣ��ֹͣ����
*/
static int handle_rx_0x8001( uint8_t linkno, uint8_t *pmsg )
{
	MsgListNode				* iter;
	JT808_TX_MSG_NODEDATA	* iterdata;

	uint16_t				id;
	uint16_t				seq;
	uint8_t					res;
/*������Ϣͷ12byte*/
	seq = ( *( pmsg + 12 ) << 8 ) | *( pmsg + 13 );
	id	= ( *( pmsg + 14 ) << 8 ) | *( pmsg + 15 );
	res = *( pmsg + 16 );

	/*��������*/
	iter		= list_jt808_tx->first;
	iterdata	= (JT808_TX_MSG_NODEDATA*)iter->data;
	if(iterdata->multipacket==0)
	{
		if( ( iterdata->head_id == id ) && ( iterdata->head_sn == seq ) )
		{
			iterdata->cb_tx_response(iterdata, linkno, pmsg ); /*Ӧ������*/
			iterdata->state = ACK_OK;
		}
	}
	else
	{
		iterdata->state = GET_DATA;
	}
	return RT_OK;
}

int handle_8001(void)
{
	MsgListNode				* iter;
	JT808_TX_MSG_NODEDATA	* iterdata;

	/*��������*/
	iter		= list_jt808_tx->first;
	iterdata	= (JT808_TX_MSG_NODEDATA*)iter->data;
	if(iterdata->multipacket)
	{
		iterdata->state = GET_DATA;
	}
	return RT_OK;
}

FINSH_FUNCTION_EXPORT( handle_8001, handle_8001 );


/*�����ְ�����*/
static int handle_rx_0x8003( uint8_t linkno, uint8_t *pmsg )
{
}

/* ������Ķ��ն�ע����Ϣ��Ӧ��*/
static int handle_rx_0x8100( uint8_t linkno, uint8_t *pmsg )
{
	MsgListNode				* iter;
	JT808_TX_MSG_NODEDATA	* iterdata;

	uint16_t				body_len; /*��Ϣ�峤��*/
	uint16_t				ack_seq;
	uint8_t					res;
	uint8_t					* msg;

	body_len	= ( ( *( pmsg + 2 ) << 8 ) | ( *( pmsg + 3 ) ) ) & 0x3FF;
	msg			= pmsg + 12;

	ack_seq = ( *msg << 8 ) | *( msg + 1 );
	res		= *( msg + 2 );

	iter		= list_jt808_tx->first;
	iterdata	= iter->data;
	if( ( iterdata->head_id == 0x0100 ) && ( iterdata->head_sn == ack_seq ) )
	{
		if( res == 0 )
		{
			strncpy( term_param.register_code, msg + 3, body_len - 3 );
			iterdata->state = ACK_OK;
		}
	}
	return 1;
}

/*�����ն˲���*/
static int handle_rx_0x8103( uint8_t linkno, uint8_t *pmsg )
{
	uint8_t		* p;
	uint8_t		res = 0;

	uint16_t	msg_len, count = 0;
	uint32_t	param_id;
	uint8_t		param_len;

	uint16_t	seq, id;

	if( *( pmsg + 2 ) >= 0x20 ) /*����Ƕ�������ò���*/
	{
		printf( "\r\n>%s multi packet no support!", __func__ );
		return 1;
	}

	id	= ( pmsg[0] << 8 ) | pmsg[1];
	seq = ( pmsg[10] << 8 ) | pmsg[11];

	msg_len = ( ( pmsg[2] << 8 ) | pmsg[3] ) & 0x3FF - 1;
	p		= pmsg + 13;

	/*ʹ�����ݳ���,�ж������Ƿ������û��ʹ�ò�������*/
	while( count < msg_len )
	{
		param_id	= ( ( *p++ ) << 24 ) | ( ( *p++ ) << 16 ) | ( ( *p++ ) << 8 ) | ( *p++ );
		param_len	= *p++;
		count		+= ( 5 + param_len );
		res			|= param_put( param_id, param_len, p );
		if( res )
		{
			printf( "\r\n%s>res=%d\r\n", __func__, __LINE__ );
			break;
		}
	}
	/*����ͨ��Ӧ��*/
	jt808_tx_0x0001( linkno, seq, id, res );
	return 1;
}

/*��ѯȫ���ն˲������п��ܻᳬ����������ֽ�*/
static int handle_rx_0x8104( uint8_t linkno, uint8_t *pmsg )
{
	return 1;
}

/*�ն˿���*/
static int handle_rx_0x8105( uint8_t linkno, uint8_t *pmsg )
{
	uint8_t cmd;
	uint8_t * cmd_arg;

	cmd = *( pmsg + 12 );
	switch( cmd )
	{
		case 1: /*��������*/
			break;
		case 2: /*�ն˿�������ָ��������*/
			break;
		case 3: /*�ն˹ػ�*/
			break;
		case 4: /*�ն˸�λ*/
			break;
		case 5: /*�ָ���������*/
			break;
		case 6: /*�ر�����ͨѶ*/
			break;
		case 7: /*�ر���������ͨѶ*/
			break;
	}
	return 1;
}

/*��ѯָ���ն˲���,����Ӧ��0x0104*/
static int handle_rx_0x8106( uint8_t linkno, uint8_t *pmsg )
{
	int			i;
	uint8_t		*p;
	uint8_t		fcs = 0;
	uint8_t		value[8];
	uint32_t	id;
	uint16_t	len;
	uint16_t	pos;
	uint16_t	info_len	= 0;
	uint16_t	head_len	= 0;
	uint8_t		param_count, return_param_count;

	uint8_t		buf[1500];

	pos					= 100;              /*�ȿճ�100byte*/
	param_count			= *( pmsg + 12 );   /*�ܵĲ�������*/
	return_param_count	= 0;
	p					= pmsg + 13;
	/*���Ҫ������Ϣ�����ݣ�����¼����*/
	for( i = 0; i < param_count; i++ )      /*�����δ֪��id��ô�죬����,�������������͸ı���*/
	{
		id	= *p++;
		id	|= ( *p++ ) << 8;
		id	|= ( *p++ ) << 16;
		id	|= ( *p++ ) << 24;
		len = param_get( id, value );       /*�õ������ĳ��ȣ�δת��*/
		if( len )
		{
			return_param_count++;           /*�ҵ���Ч��id*/
			pos += jt808_pack_int( buf + pos, &fcs, id, 2 );
			pos += jt808_pack_int( buf + pos, &fcs, len, 1 );
			pos			+= jt808_pack_array( buf + pos, &fcs, value, len );
			info_len	+= ( len + 3 );     /*id+����+����*/
		}
	}

	head_len	= 1;                        /*�ճ���ʼ��0x7e*/
	head_len	+= jt808_pack_int( buf + head_len, &fcs, 0x0104, 2 );
	head_len	+= jt808_pack_int( buf + head_len, &fcs, info_len + 3, 2 );
	head_len	+= jt808_pack_array( buf + head_len, &fcs, pmsg + 4, 6 );
	head_len	+= jt808_pack_int( buf + head_len, &fcs, tx_seq, 2 );

	head_len	+= jt808_pack_array( buf + head_len, &fcs, pmsg + 10, 2 );
	head_len	+= jt808_pack_int( buf + head_len, &fcs, return_param_count, 1 );

	memcpy( buf + head_len, buf + 100, pos - 100 ); /*ƴ������*/
	len = head_len + pos - 100;                     /*��ǰ����0x7e,<head><msg>*/

	len			+= jt808_pack_byte( buf + len, &fcs, fcs );
	buf [0]		= 0x7e;
	buf [len]	= 0x7e;

	jt808_add_tx_data( linkno, TERMINAL_ACK, 0x0104, buf, len + 1 );
	return 1;
}

/*��ѯ�ն�����,Ӧ�� 0x0107*/
static int handle_rx_0x8107( uint8_t linkno, uint8_t *pmsg )
{
	uint8_t		buf[100];
	uint8_t		fcs			= 0;
	uint16_t	len			= 1;
	uint16_t	info_len	= 0;
	uint16_t	head_len	= 1;

	len += jt808_pack_int( buf + len, &fcs, 0x0107, 2 );
	len += jt808_pack_int( buf + len, &fcs, 0x0107, 2 );
	len += jt808_pack_int( buf + len, &fcs, 0x0107, 2 );

	jt808_add_tx_data( linkno, TERMINAL_ACK, 0x0107, buf, len + 1 );
	return 1;
}

/**/
static int handle_rx_0x8201( uint8_t linkno, uint8_t *pmsg )
{
	printf("8201\r\n");
	return 1;
}

/**/
static int handle_rx_0x8202( uint8_t linkno, uint8_t *pmsg )
{
	printf("8202\r\n");
	return 1;
}

/**/
static int handle_rx_0x8300( uint8_t linkno, uint8_t *pmsg )
{
	return 1;
}

/**/
static int handle_rx_0x8301( uint8_t linkno, uint8_t *pmsg )
{
	return 1;
}

/**/
static int handle_rx_0x8302( uint8_t linkno, uint8_t *pmsg )
{
	return 1;
}

/**/
static int handle_rx_0x8303( uint8_t linkno, uint8_t *pmsg )
{
	return 1;
}

/**/
static int handle_rx_0x8304( uint8_t linkno, uint8_t *pmsg )
{
	return 1;
}

/**/
static int handle_rx_0x8400( uint8_t linkno, uint8_t *pmsg )
{
	return 1;
}

/**/
static int handle_rx_0x8401( uint8_t linkno, uint8_t *pmsg )
{
	return 1;
}

/**/
static int handle_rx_0x8500( uint8_t linkno, uint8_t *pmsg )
{
	return 1;
}

/**/
static int handle_rx_0x8600( uint8_t linkno, uint8_t *pmsg )
{
	return 1;
}

/**/
static int handle_rx_0x8601( uint8_t linkno, uint8_t *pmsg )
{
	return 1;
}

/**/
static int handle_rx_0x8602( uint8_t linkno, uint8_t *pmsg )
{
	return 1;
}

/**/
static int handle_rx_0x8603( uint8_t linkno, uint8_t *pmsg )
{
	return 1;
}

/**/
static int handle_rx_0x8604( uint8_t linkno, uint8_t *pmsg )
{
	return 1;
}

/**/
static int handle_rx_0x8605( uint8_t linkno, uint8_t *pmsg )
{
	return 1;
}

/**/
static int handle_rx_0x8606( uint8_t linkno, uint8_t *pmsg )
{
	return 1;
}

/**/
static int handle_rx_0x8607( uint8_t linkno, uint8_t *pmsg )
{
	return 1;
}

/*��ʻ��¼�����ݲɼ�*/
static int handle_rx_0x8700( uint8_t linkno, uint8_t *pmsg )
{
	return 1;
}

/*��ʻ��¼�ǲ����´�*/
static int handle_rx_0x8701( uint8_t linkno, uint8_t *pmsg )
{
	return 1;
}

/**/
static int handle_rx_0x8800( uint8_t linkno, uint8_t *pmsg )
{
	MsgListNode				* iter;
	JT808_TX_MSG_NODEDATA	* iterdata;

	uint16_t				body_len; /*��Ϣ�峤��*/
	uint16_t				ack_seq;
	uint8_t					res;
	uint8_t					* msg;

	body_len	= ( ( *( pmsg + 2 ) << 8 ) | ( *( pmsg + 3 ) ) ) & 0x3FF;
	msg			= pmsg + 12;

	ack_seq = ( *msg << 8 ) | *( msg + 1 );
	res		= *( msg + 2 );

	iter		= list_jt808_tx->first;
	iterdata	= iter->data;
	if(( iterdata->head_id == 0x0801 )&&(iterdata->multipacket))	///�ж��Ƿ���ûص�����
	{
		if(iterdata->cb_tx_response!=NULL)
			{
			iterdata->cb_tx_response(iterdata,linkno,pmsg);
			}
	}
	return 1;
}


/**/
static int handle_8800( uint32_t media_id ,char* pmsg)
{
	MsgListNode				* iter;
	JT808_TX_MSG_NODEDATA	* iterdata;
	uint8_t 				tempbuf[128];

	uint16_t				body_len; /*��Ϣ�峤��*/
	uint16_t				ack_seq;
	uint8_t					res;
	uint8_t					* msg;
	uint16_t				i,j;
	
	printf("\r\n handle_8800_1");
	memset(tempbuf,0,sizeof(tempbuf));
	tempbuf[3]=4;
	
	body_len=12;
	tempbuf[body_len++]	= media_id>>24;
	tempbuf[body_len++]	= media_id>>16;
	tempbuf[body_len++]	= media_id>>8;
	tempbuf[body_len++]	= media_id;
	if(strlen(pmsg))
		{
		j=tempbuf[body_len++]=strlen(pmsg);
		tempbuf[3]++;
		for(i=0;i<j;i++)
			{
			tempbuf[body_len++] = 0;
			tempbuf[body_len++] = *pmsg-'0';
			pmsg++;
			tempbuf[3]+=2;
			}
		}
	iter		= list_jt808_tx->first;
	iterdata	= iter->data;
	if(( iterdata->head_id == 0x0801 )&&(iterdata->multipacket))	///�ж��Ƿ���ûص�����
	{
		if(iterdata->cb_tx_response!=NULL)
			{
			iterdata->cb_tx_response(iterdata,1,tempbuf);
			printf("\r\n handle_8800_2");
			}
	}
	return 1;
}
/*����ͷ������������*/
static int handle_rx_0x8801( uint8_t linkno, uint8_t *pmsg )
{
	return 1;
}

/**/
static int handle_rx_0x8802( uint8_t linkno, uint8_t *pmsg )
{
	return 1;
}

/**/
static int handle_rx_0x8803( uint8_t linkno, uint8_t *pmsg )
{
	return 1;
}

/**/
static int handle_rx_0x8804( uint8_t linkno, uint8_t *pmsg )
{
	return 1;
}

/**/
static int handle_rx_0x8805( uint8_t linkno, uint8_t *pmsg )
{
	return 1;
}

/**/
static int handle_rx_0x8900( uint8_t linkno, uint8_t *pmsg )
{
	return 1;
}

/**/
static int handle_rx_0x8A00( uint8_t linkno, uint8_t *pmsg )
{
	return 1;
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
static int handle_rx_default( uint8_t linkno, uint8_t *pmsg )
{
	printf( "\r\nunknown!\r\n" );
	return 1;
}

#define DECL_JT808_RX_HANDLE( a )	{ a, handle_rx_ ## a }
#define DECL_JT808_TX_HANDLE( a )	{ a, handle_jt808_tx_ ## a }

HANDLE_JT808_RX_MSG handle_rx_msg[] =
{
	DECL_JT808_RX_HANDLE( 0x8001 ), //	ͨ��Ӧ��
	DECL_JT808_RX_HANDLE( 0x8003 ), //	�����ְ�����
	DECL_JT808_RX_HANDLE( 0x8100 ), //  ������Ķ��ն�ע����Ϣ��Ӧ��
	DECL_JT808_RX_HANDLE( 0x8103 ), //	�����ն˲���
	DECL_JT808_RX_HANDLE( 0x8104 ), //	��ѯ�ն˲���
	DECL_JT808_RX_HANDLE( 0x8105 ), // �ն˿���
	DECL_JT808_RX_HANDLE( 0x8106 ), // ��ѯָ���ն˲���
	DECL_JT808_RX_HANDLE( 0x8201 ), // λ����Ϣ��ѯ    λ����Ϣ��ѯ��Ϣ��Ϊ��
	DECL_JT808_RX_HANDLE( 0x8202 ), // ��ʱλ�ø��ٿ���
	DECL_JT808_RX_HANDLE( 0x8300 ), //	�ı���Ϣ�·�
	DECL_JT808_RX_HANDLE( 0x8301 ), //	�¼�����
	DECL_JT808_RX_HANDLE( 0x8302 ), // �����·�
	DECL_JT808_RX_HANDLE( 0x8303 ), //	��Ϣ�㲥�˵�����
	DECL_JT808_RX_HANDLE( 0x8304 ), //	��Ϣ����
	DECL_JT808_RX_HANDLE( 0x8400 ), //	�绰�ز�
	DECL_JT808_RX_HANDLE( 0x8401 ), //	���õ绰��
	DECL_JT808_RX_HANDLE( 0x8500 ), //	��������
	DECL_JT808_RX_HANDLE( 0x8600 ), //	����Բ������
	DECL_JT808_RX_HANDLE( 0x8601 ), //	ɾ��Բ������
	DECL_JT808_RX_HANDLE( 0x8602 ), //	���þ�������
	DECL_JT808_RX_HANDLE( 0x8603 ), //	ɾ����������
	DECL_JT808_RX_HANDLE( 0x8604 ), //	���������
	DECL_JT808_RX_HANDLE( 0x8605 ), //	ɾ���������
	DECL_JT808_RX_HANDLE( 0x8606 ), //	����·��
	DECL_JT808_RX_HANDLE( 0x8607 ), //	ɾ��·��
	DECL_JT808_RX_HANDLE( 0x8700 ), //	�г���¼�����ݲɼ�����
	DECL_JT808_RX_HANDLE( 0x8701 ), //	��ʻ��¼�ǲ����´�����
	DECL_JT808_RX_HANDLE( 0x8800 ), //	��ý�������ϴ�Ӧ��
	DECL_JT808_RX_HANDLE( 0x8801 ), //	����ͷ��������
	DECL_JT808_RX_HANDLE( 0x8802 ), //	�洢��ý�����ݼ���
	DECL_JT808_RX_HANDLE( 0x8803 ), //	�洢��ý�������ϴ�����
	DECL_JT808_RX_HANDLE( 0x8804 ), //	¼����ʼ����
	DECL_JT808_RX_HANDLE( 0x8805 ), //	�����洢��ý�����ݼ����ϴ����� ---- ����Э��Ҫ��
	DECL_JT808_RX_HANDLE( 0x8900 ), //	��������͸��
	DECL_JT808_RX_HANDLE( 0x8A00 ), //	ƽ̨RSA��Կ
};


/*
   ���մ���
   ����jt808��ʽ������
   <linkno><����2byte><��ʶ0x7e><��Ϣͷ><��Ϣ��><У����><��ʶ0x7e>

 */
uint16_t jt808_rx_proc( uint8_t * pinfo, uint16_t len)
{
	uint8_t		* psrc;
//	uint16_t	len;
	uint8_t		linkno;
	uint16_t	i, id;
	uint8_t		flag_find	= 0;
	uint8_t		fcs			= 0;
	uint16_t	ret;

	linkno	= socketfd; //pinfo [0];
//	len		=  len ;//( pinfo [1] << 8 ) | pinfo [2];
	printf( ">dump start len=%d\r\n", len );

	len = jt808_decode_fcs( pinfo, len );
	if( len == 0 )
	{
		printf( ">len=0\r\n" );
		return 1;
	}

	printf( "\r\n>dump start" );
	psrc = pinfo;
	for( i = 0; i < len; i++ )
	{
		if( i % 16 == 0 )
		{
			printf( "\r\n" );
		}
		printf( "%02x ", *psrc++ );
	}
	printf( "\r\n>dump end\r\n" );
/*checksum*/
	psrc = pinfo;
	for( i = 0; i < len - 1; i++ )
	{
		fcs ^= *psrc++;
	}
	printf("%02x, %02x, %d\r\n", fcs, psrc[0], fcs==psrc[0]);
	if(fcs!=psrc[0])
	{
		printf( "\r\n checksum error!");
		return 1;
	}

	printf("---\r\n");

	psrc	= pinfo;
	id		= ( *psrc << 8 ) | *( psrc + 1 );

	for( i = 0; i < sizeof( handle_rx_msg ) / sizeof( HANDLE_JT808_RX_MSG ); i++ )
	{
		if( id == handle_rx_msg [i].id )
		{
			printf("%2x\r\n", id);
			handle_rx_msg [i].func( linkno, psrc );
			flag_find = 1;
		}
	}
	if( !flag_find )
	{
		handle_rx_default( linkno, psrc );
	}
	return RT_OK;
}

/*
   ����ÿ��Ҫ������Ϣ��״̬
   ���������д�����?

   2013.06.08���Ӷ�����ʹ���
 */

long long current_timestamp() {
    struct timeval te;
    gettimeofday(&te, NULL); // get current time
    long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; // calculate milliseconds
    //printf("milliseconds: %lld\n", milliseconds);
    return milliseconds;
}

static MsgListRet jt808_tx_proc( MsgListNode * node )
{
	//if(pnodedata->multipacket == 0)
	//	return MSGLIST_RET_DELETE_NODE; //jeff remove
	if( node == NULL )
	{
		return MSGLIST_RET_OK;
	}

	MsgListNode				* pnode		= ( MsgListNode* )node;
	JT808_TX_MSG_NODEDATA	* pnodedata = ( JT808_TX_MSG_NODEDATA* )( pnode->data );
	int                               i = 0;
	uint8_t				    ret;
	uint16_t				ret_getdata;

    //printf("pnode data state: %d\r\d", pnodedata->state);

	if( pnodedata->state == GET_DATA)                   /*��ȡ�������ʱ������*/
    {
		if( pnodedata->multipacket == 0 )   /*�ж��Ƿ�Ϊ�������,Ϊ0��ʾΪ��������*/
		{
			pnodedata->state = IDLE;
		}
		else								/*�������*/
		{
			ret_getdata=pnodedata->get_data(pnodedata);
			if(ret_getdata==0xFFFF)
			{
				return MSGLIST_RET_DELETE_NODE;
			}
			else if(ret_getdata==0)
			{
				pnodedata->state	= WAIT_ACK;
			}
			else
			{
				pnodedata->retry	= 0;
				pnodedata->state	= IDLE;
			}
		}
	}

	if( pnodedata->state == IDLE )                      /*���У�������Ϣ��ʱ��û������*/
	{
		if( pnodedata->retry >= jt808_param.id_0x0003 ) /*����������ش�����*/                                                                     /*�Ѿ��ﵽ���Դ���*/
		{
			/*��ʾ����ʧ��*/
			pnodedata->cb_tx_timeout( pnodedata );      /*���÷���ʧ�ܴ�����*/
			return MSGLIST_RET_DELETE_NODE;
		}
		#if 0
		/*Ҫ�ж��ǲ��ǳ���GSM_TCPIP״̬,��ǰsocket�Ƿ����*/
		if( gsmstate( GSM_STATE_GET ) != GSM_TCPIP )
		{
			return MSGLIST_RET_OK;
		}
		if( connect_state.server_state != SOCKET_READY )
		{
			return MSGLIST_RET_OK;
		}
		#endif
		
		if( pnodedata->multipacket == 0 )   /*�ж��Ƿ�Ϊ�������*/
		{
#if 0
			printer_data_hex(pnodedata->pmsg,pnodedata->msg_len);
#endif
			printf("send head id: %d\r\n", pnodedata->head_id);
			ret = send( pnodedata->linkno, pnodedata->pmsg, pnodedata->msg_len, 0);
            //printf("send ret: %d\r\n", ret);
			if( ret > 0 )             /*���ͳɹ��ȴ�����Ӧ����*/
			{
				pnodedata->tick = current_timestamp();//0;//rt_tick_get( );
				pnodedata->retry++;
				//pnodedata->timeout	= pnodedata->retry * jt808_param.id_0x0002; //Seconds  * RT_TICK_PER_SECOND;
				pnodedata->timeout	= 30 * 1000; //Seconds, RT_TICK_PER_SECOND*3;
				pnodedata->state	= WAIT_ACK;
				if (pnodedata->head_id == 0x0200)
					pnodedata->state	= ACK_OK;
				#ifdef NO_COMMON_ACK			///���û��ͨ��Ӧ
				if( pnodedata->multipacket )
					{
					pnodedata->state	= GET_DATA;
					}
				#endif
				printf( "send retry=%d,timeout=%d\r\n", pnodedata->retry, pnodedata->timeout );
			}else /*��������û�еȵ�ģ�鷵�ص�OK�������ط������ǵ�һ��ʱ���ٷ�*/
			{
				pnodedata->retry++; /*��λ�ٴη���*/
				total_send_error++;
				printf( "total_send_error=%d\r\n", total_send_error );
			}
		}//else
		{
		}

		return MSGLIST_RET_OK;
	}

	if( pnodedata->state == WAIT_ACK ) /*�������Ӧ���Ƿ�ʱ*/
	{
#if 1
		if( (current_timestamp() - pnodedata->tick) > pnodedata->timeout )
#endif
		{
			pnodedata->state = IDLE;
		}
		return MSGLIST_RET_OK;
	}

	if( pnodedata->state == ACK_OK )
	{
		return MSGLIST_RET_DELETE_NODE;
	}

	return MSGLIST_RET_OK;
}

/*jt808��socket����

   ά����·�����в�ͬ��ԭ��
   �ϱ�״̬��ά��
   1.��δ����
   2.�������ӣ�DNS,��ʱ��Ӧ��
   3.��ֹ�ϱ����ر�ģ�������
   4.��ǰ���ڽ��п��и��£���ý���ϱ��Ȳ���Ҫ��ϵĹ���

 */


/*
   ����ص���Ҫ��M66����·�ض�ʱ��֪ͨ��socket����һ���������߳�
 */
void cb_socket_state( uint8_t linkno, T_SOCKET_STATE new_state )
{
	if( linkno == 1 )
	{
		connect_state.server_state = new_state;
	}

	if( linkno == 2 )
	{
		connect_state.auth_state = new_state;
	}
}

/*808���Ӵ���*/
static void jt808_socket_proc( void )
{
#if 0
	T_GSM_STATE state;

/*����Ƿ�����gsm����*/
	if( connect_state.disable_connect )
	{
		return;
	}

/*���GSM״̬*/
	state = gsmstate( GSM_STATE_GET );
	if( state == GSM_IDLE )
	{
		gsmstate( GSM_POWERON );                /*��������*/
		return;
	}
/*���Ƶ���*/
	if( state == GSM_AT )                       /*����Ҫ�ж����Ǹ�apn user psw ����*/
	{
		if( connect_state.server_index % 2 )    /*�ñ��÷�����*/
		{
			ctl_gprs( jt808_param.id_0x0014, \
			          jt808_param.id_0x0015, \
			          jt808_param.id_0x0016, \
			          1 );
		}else /*����������*/
		{
			ctl_gprs( jt808_param.id_0x0010, \
			          jt808_param.id_0x0011, \
			          jt808_param.id_0x0012, \
			          1 );
		}
		return;
	}
/*���ƽ�������*/
	if( state == GSM_TCPIP )                        /*�Ѿ�������*/
	{
		if( connect_state.server_state == CONNECT_IDLE )
		{
			if( connect_state.server_index % 2 )    /*�����÷�����*/
			{
				ctl_socket( 1, 't', jt808_param.id_0x0017, jt808_param.id_0x0018, 1 );
			}else /*����������*/
			{
				ctl_socket( 1, 't', jt808_param.id_0x0013, jt808_param.id_0x0018, 1 );
			}
			connect_state.server_state = CONNECT_PEER;
			return;
		}

		if( connect_state.server_state == CONNECT_PEER ) /*�������ӵ�������*/
		{
			if( socketstate( SOCKET_STATE ) == SOCKET_READY )
			{
				connect_state.server_state = CONNECTED;
			}else /*û�����ӳɹ�,�л�������*/
			{
				connect_state.server_index++;
				connect_state.server_state = CONNECT_IDLE;
			}
		}

		/*808������û�����ӳɹ����Ͳ���IC��������*/
		if( connect_state.server_state != CONNECTED )
		{
			return;
		}

		/*����IC��������*/

		if( connect_state.auth_state == CONNECT_IDLE )  /*û������*/
		{
			if( connect_state.auth_index % 2 )          /*�����÷�����*/
			{
				ctl_socket( 2, 't', jt808_param.id_0x001A, jt808_param.id_0x001B, 1 );
			}else /*����������*/
			{
				ctl_socket( 2, 't', jt808_param.id_0x001D, jt808_param.id_0x001B, 1 );
			}
			connect_state.auth_state = CONNECT_PEER;
			return;
		}

		if( connect_state.auth_state == CONNECT_PEER ) /*�������ӵ�������*/
		{
			if( socketstate( 0 ) == SOCKET_READY )
			{
				connect_state.auth_state = CONNECTED;
			}else /*û�����ӳɹ�,�л�������*/
			{
				//connect_state.auth_index++;
				//connect_state.auth_state=CONNECT_IDLE;
				//if(connect_state.auth_index>6)
				//{

				//}
			}
		}
	}
}

/*
   tts���������Ĵ���

   ��ͨ��
   %TTS: 0 �ж�tts״̬(���ɲ�����ÿ�ζ������)
   ����AT%TTS? ��ѯ״̬
 */
void jt808_tts_proc( void )
{
	uint8_t	ret;
	size_t	len;
	uint8_t		*pinfo, *p;
	uint8_t		c;
	T_GSM_STATE oldstate;

	char		buf[20];
	char		tbl[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
/*gsm�ڴ�����������*/
	oldstate = gsmstate( GSM_STATE_GET );
	if( oldstate != GSM_TCPIP )
	{
		if( oldstate != GSM_AT )
		{
			return;
		}
	}

/*�Ƿ�����ϢҪ����*/
	ret = rt_mb_recv( &mb_tts, (uint32_t*)&pinfo, 0 );
	if( ret != RT_EOK )
	{
		return;
	}

	gsmstate( GSM_AT_SEND );

	GPIO_ResetBits( GPIOD, GPIO_Pin_9 ); /*������*/

	sprintf( buf, "AT%%TTS=2,3,5,\"" );

	rt_device_write( dev_gsm, 0, buf, strlen( buf ) );

	printf( "%s", buf );

	len = ( *pinfo << 8 ) | ( *( pinfo + 1 ) );
	p	= pinfo + 2;
	while( len-- )
	{
		c		= *p++;
		buf[0]	= tbl[c >> 4];
		buf[1]	= tbl[c & 0x0f];
		rt_device_write( dev_gsm, 0, buf, 2 );
		printf( "%c%c", buf[0], buf[1] );
	}
	buf[0]	= '"';
	buf[1]	= 0x0d;
	buf[2]	= 0x0a;
	buf[3]	= 0;
	rt_device_write( dev_gsm, 0, buf, 3 );
	printf( "%s", buf );
/*���жϣ���gsmrx_cb�д���*/
	free( pinfo );
	ret = gsm_send( "", NULL, "%TTS: 0", RESP_TYPE_STR, RT_TICK_PER_SECOND * 35, 1 );
	GPIO_SetBits( GPIOD, GPIO_Pin_9 ); /*�ع���*/
	gsmstate( oldstate );
#endif
}

/*
   at������յ�OK��ʱ�˳�
 */
void jt808_at_tx_proc( void )
{
#if 0
    rt_err_t        ret;
    rt_size_t       len;
    uint8_t         *pinfo, *p;
    uint8_t         c;
    T_GSM_STATE oldstate;

/*gsmÔÚ´¦ÀíÆäËûÃüÁî*/
    oldstate = gsmstate( GSM_STATE_GET );
    if( oldstate != GSM_TCPIP )
    {
            if( oldstate != GSM_AT )
            {
                    return;
            }
    }

/*ÊÇ·ñÓÐÐÅÏ¢Òª·¢ËÍ*/
    ret = rt_mb_recv( &mb_at_tx, (rt_uint32_t*)&pinfo, 0 );
    if( ret != RT_EOK )
    {
            return;
    }

    gsmstate( GSM_AT_SEND );

    len = ( *pinfo << 8 ) | ( *( pinfo + 1 ) );
    p       = pinfo + 2;
    ret = gsm_send( p, RT_NULL, "OK", RESP_TYPE_STR, RT_TICK_PER_SECOND * 5, 1 );
    rt_kprintf( "at_tx=%d\r\n", ret );
    free( pinfo );
    gsmstate( oldstate );
#endif
}

/*
   ����״̬ά��
   jt808Э�鴦��

 */
#if 0
static char thread_jt808_stack [4096];
struct rt_thread thread_jt808;
/***/
static void rt_thread_entry_jt808( void * parameter )
{
	uint8_t				ret;
	int						i;
	uint8_t					* pstr;

	MsgListNode				* iter;
	JT808_TX_MSG_NODEDATA	* pnodedata;

	int						j = 0xaabbccdd;

	printf( "\r\n1.id0=%08x\r\n", param_get_int( 0x0000 ) );

	param_put_int( 0x000, j );
	printf( "\r\nwrite j=%08x read=%08x\r\n", j, param_get_int( 0x0000 ) );

	param_put( 0x000, 4, (uint8_t*)&j );
	printf( "\r\nid0=%08x\r\n", param_get_int( 0x0000 ) );

/*��ȡ������������*/
	//param_load( );

	list_jt808_tx	= msglist_create( );
	list_jt808_rx	= msglist_create( );

	while( 1 )
	{
/*����gprs��Ϣ*/
		ret = rt_mb_recv( &mb_gprsrx, ( uint32_t* )&pstr, 5 );
		if( ret == RT_EOK )
		{
			jt808_rx_proc( pstr );
			free( pstr );
		}

		jt808_socket_proc( );   /*jt808 socket����*/

		jt808_tts_proc( );      /*tts����*/

		jt808_at_tx_proc( );    /*at�����*/

/*���Ŵ���*/
		SMS_Process( );

/*������Ϣ��������*/
		iter = list_jt808_tx->first;

		if( jt808_tx_proc( iter ) == MSGLIST_RET_DELETE_NODE )  /*ɾ���ýڵ�*/
		{
			pnodedata = ( JT808_TX_MSG_NODEDATA* )( iter->data );

			printf("\r\n ɾ���ڵ�,head_id=%X",pnodedata->head_id);

			free(pnodedata->user_para);						/*ɾ���û�����*/
			free( pnodedata->pmsg );                         /*ɾ���û�����*/
			free( pnodedata );                               /*ɾ���ڵ�����*/
			list_jt808_tx->first = iter->next;                  /*ָ����һ��*/
			free( iter );
		}
		//rt_thread_delay( RT_TICK_PER_SECOND / 20 );
		sleep(1);
	}

	msglist_destroy( list_jt808_tx );
}
#endif
/*jt808�����̳߳�ʼ��*/
void jt808_init( void )
{
#if 0
	sms_init( );

	dev_gsm = rt_device_find( "gsm" );
	rt_mb_init( &mb_gprsrx, "mb_gprs", &mb_gprsrx_pool, MB_GPRSDATA_POOL_SIZE / 4, RT_IPC_FLAG_FIFO );

	rt_mb_init( &mb_tts, "mb_tts", &mb_tts_pool, MB_TTS_POOL_SIZE / 4, RT_IPC_FLAG_FIFO );

	rt_mb_init( &mb_at_tx, "mb_at_tx", &mb_at_tx_pool, MB_AT_TX_POOL_SIZE / 4, RT_IPC_FLAG_FIFO );

	rt_thread_init( &thread_jt808,
	                "jt808",
	                rt_thread_entry_jt808,
	                NULL,
	                &thread_jt808_stack [0],
	                sizeof( thread_jt808_stack ), 10, 5 );
	rt_thread_startup( &thread_jt808 );
#endif
}

/*gprs���մ���,�յ�����Ҫ���촦��*/
uint8_t gprs_rx( uint8_t linkno, uint8_t * pinfo, uint16_t length )
{
#if 0
	uint8_t * pmsg;
	pmsg = malloc( length + 3 );                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 /*����������Ϣ*/
	if( pmsg != NULL )
	{
		pmsg [0]	= linkno;
		pmsg [1]	= length >> 8;
		pmsg [2]	= length & 0xff;
		memcpy( pmsg + 3, pinfo, length );
		rt_mb_send( &mb_gprsrx, ( uint32_t )pmsg );
		return 0;
	}
	return 1;
#endif
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
static uint8_t register_jt808_tx( void )
{
	uint8_t					* pdata;
	JT808_TX_MSG_NODEDATA	* pnodedata;
	uint8_t					buf[256];
	uint16_t				len;
	uint8_t					fcs = 0;
	int						i;

	pnodedata = malloc( sizeof( JT808_TX_MSG_NODEDATA ) );
	if( pnodedata == NULL )
	{
		return -1;
	}
	memset(pnodedata,0,sizeof(JT808_TX_MSG_NODEDATA));
	pnodedata->linkno	        = socketfd; //jeff add
	pnodedata->type				= TERMINAL_CMD;
	pnodedata->state			= IDLE;
	pnodedata->retry			= 0;
	pnodedata->cb_tx_timeout	= jt808_tx_timeout;
	pnodedata->cb_tx_response	= jt808_tx_response;
#if 0
	len = jt808_pack( buf, "%w%w%6s%w%w%w%5s%20s%7s%b%s",
	                  0x0100,
	                  37 + strlen( jt808_param.id_0x0083 ),
	                  term_param.mobile,
	                  tx_seq,
	                  jt808_param.id_0x0081,
	                  jt808_param.id_0x0082,
	                  term_param.producer_id,
	                  term_param.model,
	                  term_param.terminal_id,
	                  jt808_param.id_0x0084,
	                  jt808_param.id_0x0083 );

	printf( "\r\n*********************\r\n" );
	for( i = 0; i < len; i++ )
	{
		printf( "%02x ", buf [i] );
	}
	printf( "\r\n*********************\r\n" );
#endif
	len = 1;
	len += jt808_pack_int( buf + len, &fcs, 0x0100, 2 );
	len += jt808_pack_int( buf + len, &fcs, 37 + strlen( jt808_param.id_0x0083 ), 2 );
	len += jt808_pack_array( buf + len, &fcs, term_param.mobile, 6 );
	len += jt808_pack_int( buf + len, &fcs, tx_seq, 2 );

	len += jt808_pack_int( buf + len, &fcs, jt808_param.id_0x0081, 2 );
	len	+= jt808_pack_int( buf + len, &fcs, jt808_param.id_0x0082, 2 );
	len	+= jt808_pack_array( buf + len, &fcs, term_param.producer_id, 5 );
	len	+= jt808_pack_array( buf + len, &fcs, term_param.model, 20 );
	len	+= jt808_pack_array( buf + len, &fcs, term_param.terminal_id, 7 );
	len	+= jt808_pack_int( buf + len, &fcs, jt808_param.id_0x0084, 1 );
	len	+= jt808_pack_string( buf + len, &fcs, jt808_param.id_0x0083 );
	len	+= jt808_pack_byte( buf + len, &fcs, fcs );
	buf [0]		= 0x7e;
	buf [len]	= 0x7e;
	pdata		= malloc( len + 1 );
	if( pdata == NULL )
	{
		free( pnodedata );
		return RT_ERROR;
	}
	printf( "\r\n--------------------\r\n" );
	for( i = 0; i < len + 1; i++ )
	{
		printf( "%02x ", buf [i] );
	}
	printf( "\r\n--------------------\r\n" );
	memcpy( pdata, buf, len + 1 );
	pnodedata->msg_len	= len + 1;
	pnodedata->pmsg		= pdata;
	pnodedata->head_sn	= tx_seq;
	pnodedata->head_id	= 0x0100;
	msglist_append( list_jt808_tx, pnodedata );
	tx_seq++;
    return 0;
}

static uint8_t auth_jt808_tx( void )
{
	uint8_t					* pdata;
	JT808_TX_MSG_NODEDATA	* pnodedata;
	uint8_t					buf[256];
	uint16_t				len;
	uint8_t					fcs = 0;
	int						i;

	pnodedata = malloc( sizeof( JT808_TX_MSG_NODEDATA ) );
	if( pnodedata == NULL )
	{
		return -1;
	}
	memset(pnodedata,0,sizeof(JT808_TX_MSG_NODEDATA));
	pnodedata->linkno	        = socketfd; //jeff add
	pnodedata->type				= TERMINAL_CMD;
	pnodedata->state			= IDLE;
	pnodedata->retry			= 0;
	pnodedata->cb_tx_timeout	= jt808_tx_timeout;
	pnodedata->cb_tx_response	= jt808_tx_response;

	len = 1;
	len += jt808_pack_int( buf + len, &fcs, 0x0102, 2 );
	len += jt808_pack_int( buf + len, &fcs, 37 + strlen( jt808_param.id_0x0083 ), 2 );
	len += jt808_pack_array( buf + len, &fcs, term_param.mobile, 6 );
	len += jt808_pack_int( buf + len, &fcs, tx_seq, 2 );

	len	+= jt808_pack_array( buf + len, &fcs, term_param.register_code,  16);
	len	+= jt808_pack_byte( buf + len, &fcs, fcs );
	buf [0]		= 0x7e;
	buf [len]	= 0x7e;
	pdata		= malloc( len + 1 );
	if( pdata == NULL )
	{
		free( pnodedata );
		return RT_ERROR;
	}
	printf( "\r\n-------auth--------\r\n" );
	for( i = 0; i < len + 1; i++ )
	{
		printf( "%02x ", buf [i] );
	}
	printf( "\r\n--------------------\r\n" );
	memcpy( pdata, buf, len + 1 );
	pnodedata->msg_len	= len + 1;
	pnodedata->pmsg		= pdata;
	pnodedata->head_sn	= tx_seq;
	pnodedata->head_id	= 0x0102;
	msglist_append( list_jt808_tx, pnodedata );
	tx_seq++;
    return 0;
}

static uint8_t gps_jt808_tx( GPS_BASEINFO gps_baseinfo )
{
	uint8_t					* pdata;
	JT808_TX_MSG_NODEDATA	* pnodedata;
	uint8_t					buf[256];
	uint16_t				len;
	uint8_t					fcs = 0;
	int						i;

	pnodedata = malloc( sizeof( JT808_TX_MSG_NODEDATA ) );
	if( pnodedata == NULL )
	{
		return -1;
	}
	memset(pnodedata,0,sizeof(JT808_TX_MSG_NODEDATA));
	pnodedata->linkno	        = socketfd; //jeff add
	pnodedata->type				= TERMINAL_CMD;
	pnodedata->state			= IDLE;
	pnodedata->retry			= 0;
	pnodedata->cb_tx_timeout	= jt808_tx_timeout;
	pnodedata->cb_tx_response	= jt808_tx_response;

	len = 1;
	len += jt808_pack_int( buf + len, &fcs, 0x0200, 2 );
	len += jt808_pack_int( buf + len, &fcs, 37 + strlen( jt808_param.id_0x0083 ), 2 );
	len += jt808_pack_array( buf + len, &fcs, term_param.mobile, 6 );
	len += jt808_pack_int( buf + len, &fcs, tx_seq, 2 );

	len	+= jt808_pack_int( buf + len, &fcs, gps_baseinfo.alarm,  4);
	len	+= jt808_pack_int( buf + len, &fcs, gps_baseinfo.status,  4);
	len	+= jt808_pack_int( buf + len, &fcs, gps_baseinfo.latitude,  4);
	len	+= jt808_pack_int( buf + len, &fcs, gps_baseinfo.longitude,  4);
	len	+= jt808_pack_int( buf + len, &fcs, gps_baseinfo.altitude,  2);
	len	+= jt808_pack_int( buf + len, &fcs, gps_baseinfo.speed,  2);
	len	+= jt808_pack_int( buf + len, &fcs, gps_baseinfo.direction,  2);
	len	+= jt808_pack_array( buf + len, &fcs, gps_baseinfo.datetime, 6);
	len	+= jt808_pack_byte( buf + len, &fcs, fcs );
	buf [0]		= 0x7e;
	buf [len]	= 0x7e;
	pdata		= malloc( len + 1 );
	if( pdata == NULL )
	{
		free( pnodedata );
		return RT_ERROR;
	}
	printf( "\r\n-----gps request----\r\n" );
	for( i = 0; i < len + 1; i++ )
	{
		printf( "%02x ", buf [i] );
	}
	printf( "\r\n--------------------!\r\n" );
	memcpy( pdata, buf, len + 1 );
	pnodedata->msg_len	= len + 1;
	pnodedata->pmsg		= pdata;
	pnodedata->head_sn	= tx_seq;
	pnodedata->head_id	= 0x0200;
	printf("msglist_append gps request!\r\n");
	msglist_append( list_jt808_tx, pnodedata );
	tx_seq++;
    return 0;
}

size_t tts_write( char* info )
{
#if 0
	uint8_t		*pmsg;
	uint16_t	count;
	count = strlen( info );

	/*ֱ�ӷ��͵�Mailbox��,�ڲ�����*/
	pmsg = malloc( count + 2 );
	if( pmsg != NULL )
	{
		*pmsg			= count >> 8;
		*( pmsg + 1 )	= count & 0xff;
		memcpy( pmsg + 2, info, count );
		rt_mb_send( &mb_tts, (uint32_t)pmsg );
		return 0;
	}
	return 1;
#endif
}

/*
   ����AT����
   ��α�֤������,������ִ�У�����ȴ���ʱ��

 */
size_t at( char *sinfo )
{
#if 0
	uint8_t		*pmsg;
	uint16_t	count;
	count = strlen( sinfo );

	/*ֱ�ӷ��͵�Mailbox��,�ڲ�����*/
	pmsg = malloc( count + 3 );
	if( pmsg != NULL )
	{
		*pmsg			= count >> 8;
		*( pmsg + 1 )	= count & 0xff;
		memcpy( pmsg + 2, sinfo, count );
		*( pmsg + count + 2 ) = 0;
		rt_mb_send( &mb_at_tx, (uint32_t)pmsg );
		return 0;
	}
	return 1;
#endif
}

static void main_thread_entry_jt808()
{
	uint8_t				ret;
	int						i = 0;
	uint8_t					buf[1024];

	MsgListNode				* iter;
	JT808_TX_MSG_NODEDATA	* pnodedata;

	int						j = 0xaabbccdd;

	memset(buf, 0, sizeof(buf));
	printf( "\r\n1.id0=%08x\r\n", param_get_int( 0x0000 ) );

	param_put_int( 0x000, j );
	printf( "\r\nwrite j=%08x read=%08x\r\n", j, param_get_int( 0x0000 ) );

	param_put( 0x000, 4, (uint8_t*)&j );
	printf( "\r\nid0=%08x\r\n", param_get_int( 0x0000 ) );

/*��ȡ������������*/
	//param_load( );

	while( 1 )
	{
		iter = list_jt808_tx->first;

		if( jt808_tx_proc( iter ) == MSGLIST_RET_DELETE_NODE )  /*ɾ���ýڵ�*/
		{
			printf("jt808_tx_proc\r\n");
			pnodedata = ( JT808_TX_MSG_NODEDATA* )( iter->data );

			printf("\r\n head_id=%X\r\n",pnodedata->head_id);

			free(pnodedata->user_para);						/*ɾ���û�����*/
			free( pnodedata->pmsg );                         /*ɾ���û�����*/
			free( pnodedata );                               /*ɾ���ڵ�����*/
			list_jt808_tx->first = iter->next;                  /*ָ����һ��*/
			free( iter );
			printf("\r\n head_id=%X\r\n",pnodedata->head_id);
		}
		//rt_thread_delay( RT_TICK_PER_SECOND / 20 );
		//sleep(1);
		printf("jt808_rx_proc\r\n");
		ret = recv(socketfd, buf, 1024, 0);
		printf("%d bytes recv\r\n", ret);
		if( ret > 0 )
		{
			jt808_rx_proc(buf, ret);
			printf("jt808_rx_proc finish!");
			memset(buf,0, sizeof(buf));
		}
	}

	msglist_destroy( list_jt808_tx );
}

static void tx_thread_entry_jt808(int socket)
{
	uint8_t				ret;
	int						i = 0;
	uint8_t					buf[1024];

	MsgListNode				* iter;
	JT808_TX_MSG_NODEDATA	* pnodedata;

	int						j = 0xaabbccdd;

	memset(buf, 0, sizeof(buf));
	printf( "\r\n1.id0=%08x\r\n", param_get_int( 0x0000 ) );

	param_put_int( 0x000, j );
	printf( "\r\nwrite j=%08x read=%08x\r\n", j, param_get_int( 0x0000 ) );

	param_put( 0x000, 4, (uint8_t*)&j );
	printf( "\r\nid0=%08x\r\n", param_get_int( 0x0000 ) );

	//param_load( );
	while( 1 )
	{
		iter = list_jt808_tx->first;
		if( jt808_tx_proc( iter ) == MSGLIST_RET_DELETE_NODE )  /*ɾ���ýڵ�*/
		{
			printf("jt808_tx_proc finish!\r\n");
			pnodedata = ( JT808_TX_MSG_NODEDATA* )( iter->data );
			printf("\r\n head_id=%X\r\n",pnodedata->head_id);
			if (pnodedata->head_id == 0x0100) {
			   printf("register code: %s\r\n", term_param.register_code);
			   auth_jt808_tx();
			}
			pthread_mutex_lock(list_jt808_tx->lock);
			printf("tx lock %p\r\n", list_jt808_tx->lock);
			free(pnodedata->user_para);						/*ɾ���û�����*/
			free( pnodedata->pmsg );                         /*ɾ���û�����*/
			free( pnodedata );                               /*ɾ���ڵ�����*/
			list_jt808_tx->first = iter->next;                  /*ָ����һ��*/
			free( iter );
			pthread_mutex_unlock(list_jt808_tx->lock);
			printf("tx unlock %p\r\n", list_jt808_tx->lock);
		}
	}

	//msglist_destroy( list_jt808_tx );
}

static void rx_thread_entry_jt808()
{
	uint8_t				ret;
	int						i = 0;
	uint8_t					buf[1024];

	while( 1 )
	{
		printf("jt808_rx_proc\r\n");
		ret = recv(socketfd, buf, 1024, 0);
		printf("%d bytes recv\r\n", ret);
		if( ret > 0 )
		{
			jt808_rx_proc(buf, ret);
			printf("jt808_rx_proc finish!");
			memset(buf,0, sizeof(buf));
		}
	}
}

int toBcd(uint8_t *bcd, long num) {
    int digits = 0;

    long quot = num;
    while (quot != 0) {
      quot /= 10;
      digits++;
    }

    int len = digits % 2 == 0 ? digits / 2 : digits / 2 + 1;

    //uint8_t *bcd = malloc(6*sizeof(uint8_t));

    uint8_t digit;
    for (int i = digits; i > 0; i--) {
      digit = (uint8_t) (num % 10);
      if (i % 2 == 0) {
        bcd[i / 2 - 1] = digit;
      } else {
        bcd[i / 2] |= digit << 4;
      }
      num /= 10;
    }

    return bcd;
}

int gps_logging_thread_entry_jt808(void)
{
	int read_bytes, total_bytes = 0;
	char *start, *end;
	sigset_t block_mask;
    char line[MINMEA_MAX_LENGTH];

	printf("open gps %d\r\n", gps_fd);
	while ((gps_fd = fopen(GPSPATH, "r")) == NULL) {
		printf("open gps fail! %d", gps_fd);
		sleep(5);
	}

    while (1) {
#ifdef gps_demo
    	if (fgets(line, sizeof(line), gps_fd) == NULL) {
    		rewind(gps_fd);
    		continue;
    	}
#endif
//        printf("%s", line);
        switch (minmea_sentence_id(line, false)) {
            case MINMEA_SENTENCE_RMC: {
                struct minmea_sentence_rmc frame;
                if (minmea_parse_rmc(&frame, line)) {
#if 0
                    printf(INDENT_SPACES "$xxRMC: raw coordinates and speed: (%d/%d,%d/%d) %d/%d\n",
                            frame.latitude.value, frame.latitude.scale,
                            frame.longitude.value, frame.longitude.scale,
                            frame.speed.value, frame.speed.scale);
                    printf(INDENT_SPACES "$xxRMC fixed-point coordinates and speed scaled to three decimal places: (%d,%d) %d\n",
                            minmea_rescale(&frame.latitude, 1000),
                            minmea_rescale(&frame.longitude, 1000),
                            minmea_rescale(&frame.speed, 1000));
                    printf(INDENT_SPACES "$xxRMC floating point degree coordinates and speed: (%f,%f) %f\n",
                            minmea_tocoord(&frame.latitude),
                            minmea_tocoord(&frame.longitude),
                            minmea_tofloat(&frame.speed));
#endif
                    if (&frame.valid)
                       gps_baseinfo.status = gps_baseinfo.status | 0x00000002;
                    gps_baseinfo.latitude = (int)(minmea_tocoord(&frame.latitude) * 1000000);
                    gps_baseinfo.longitude = (int)(minmea_tocoord(&frame.longitude) * 1000000);
                    gps_baseinfo.speed = minmea_tofloat(&frame.speed);
                    gps_baseinfo.direction = (uint16_t)(minmea_tofloat(&frame.course));
                    struct timespec *ts;
                    struct tm *info;
              	    char buffer[80];

              	    ts = malloc(sizeof(struct timespec));
                    if (minmea_gettime(ts, &frame.date, &frame.time) == 0) {
                    	info = localtime( &(ts->tv_sec) );
                    	strftime(buffer, 80, "%y%m%d%H%M%S", info);
                    } else {
                 	    time_t rawtime;
                 	    time( &rawtime );
                 	    info = localtime( &rawtime );
                 	    strftime(buffer,80,"%y%m%d%H%M%S", info);
                    }
                    free(ts);
                    toBcd(gps_baseinfo.datetime, atol(buffer));
                }
                else {
                    printf(INDENT_SPACES "$xxRMC sentence is not parsed\n");
                }
            } break;

            case MINMEA_SENTENCE_GGA: {
                struct minmea_sentence_gga frame;
                if (minmea_parse_gga(&frame, line)) {
#if 0
                    printf(INDENT_SPACES "$xxGGA: fix quality: %d\n", frame.fix_quality);
#endif
                    gps_baseinfo.altitude = (uint16_t)(minmea_tofloat(&frame.altitude));
                }
                else {
                    printf(INDENT_SPACES "$xxGGA sentence is not parsed\n");
                }
            } break;
#if 0
            case MINMEA_SENTENCE_GST: {
                struct minmea_sentence_gst frame;
                if (minmea_parse_gst(&frame, line)) {
                    printf(INDENT_SPACES "$xxGST: raw latitude,longitude and altitude error deviation: (%d/%d,%d/%d,%d/%d)\n",
                            frame.latitude_error_deviation.value, frame.latitude_error_deviation.scale,
                            frame.longitude_error_deviation.value, frame.longitude_error_deviation.scale,
                            frame.altitude_error_deviation.value, frame.altitude_error_deviation.scale);
                    printf(INDENT_SPACES "$xxGST fixed point latitude,longitude and altitude error deviation"
                           " scaled to one decimal place: (%d,%d,%d)\n",
                            minmea_rescale(&frame.latitude_error_deviation, 10),
                            minmea_rescale(&frame.longitude_error_deviation, 10),
                            minmea_rescale(&frame.altitude_error_deviation, 10));
                    printf(INDENT_SPACES "$xxGST floating point degree latitude, longitude and altitude error deviation: (%f,%f,%f)",
                            minmea_tofloat(&frame.latitude_error_deviation),
                            minmea_tofloat(&frame.longitude_error_deviation),
                            minmea_tofloat(&frame.altitude_error_deviation));
                }
                else {
                    printf(INDENT_SPACES "$xxGST sentence is not parsed\n");
                }
            } break;

            case MINMEA_SENTENCE_GSV: {
                struct minmea_sentence_gsv frame;
                if (minmea_parse_gsv(&frame, line)) {
                    printf(INDENT_SPACES "$xxGSV: message %d of %d\n", frame.msg_nr, frame.total_msgs);
                    printf(INDENT_SPACES "$xxGSV: sattelites in view: %d\n", frame.total_sats);
                    for (int i = 0; i < 4; i++)
                        printf(INDENT_SPACES "$xxGSV: sat nr %d, elevation: %d, azimuth: %d, snr: %d dbm\n",
                            frame.sats[i].nr,
                            frame.sats[i].elevation,
                            frame.sats[i].azimuth,
                            frame.sats[i].snr);
                }
                else {
                    printf(INDENT_SPACES "$xxGSV sentence is not parsed\n");
                }
            } break;

            case MINMEA_SENTENCE_VTG: {
               struct minmea_sentence_vtg frame;
               if (minmea_parse_vtg(&frame, line)) {
                    printf(INDENT_SPACES "$xxVTG: true track degrees = %f\n",
                           minmea_tofloat(&frame.true_track_degrees));
                    printf(INDENT_SPACES "        magnetic track degrees = %f\n",
                           minmea_tofloat(&frame.magnetic_track_degrees));
                    printf(INDENT_SPACES "        speed knots = %f\n",
                            minmea_tofloat(&frame.speed_knots));
                    printf(INDENT_SPACES "        speed kph = %f\n",
                            minmea_tofloat(&frame.speed_kph));
               }
               else {
                    printf(INDENT_SPACES "$xxVTG sentence is not parsed\n");
               }
            } break;

            case MINMEA_SENTENCE_ZDA: {
                struct minmea_sentence_zda frame;
                if (minmea_parse_zda(&frame, line)) {
                    printf(INDENT_SPACES "$xxZDA: %d:%d:%d %02d.%02d.%d UTC%+03d:%02d\n",
                           frame.time.hours,
                           frame.time.minutes,
                           frame.time.seconds,
                           frame.date.day,
                           frame.date.month,
                           frame.date.year,
                           frame.hour_offset,
                           frame.minute_offset);
                }
                else {
                    printf(INDENT_SPACES "$xxZDA sentence is not parsed\n");
                }
            } break;
#endif
            case MINMEA_INVALID: {
                printf(INDENT_SPACES "$xxxxx sentence is not valid\n");
            } break;

            default: {
#if 0
                printf(INDENT_SPACES "$xxxxx sentence is not parsed\n");
#endif
            } break;
        }
    }

	close(gps_fd);
	return 0;
}

int gps_send_thread_entry_jt808() {
	while (1) {
		if (gps_baseinfo.status != 0) {
		   gps_jt808_tx(gps_baseinfo);
		   gps_baseinfo.status = 0;
		}

	    sleep(5);
	}
}

void start_gps_logging() {
    int err = pthread_create(&(tid[2]), NULL, &gps_logging_thread_entry_jt808, NULL);
    if (err != 0)
        printf("\ncan't create gps logging thread :[%s]", strerror(err));
    else
        printf("\n Thread created gps logging thread successfully\n");
}

void start_gps_send() {
    int err = pthread_create(&(tid[3]), NULL, &gps_send_thread_entry_jt808, NULL);
    if (err != 0)
        printf("\ncan't create gps send thread :[%s]", strerror(err));
    else
        printf("\n Thread created gps send thread successfully\n");
}

void connectToServer(struct hostent *host, int port) {
    char buffer[1024];
    struct sockaddr_in server_addr;
    int len=20;
    char bufaddr[len];

    if((socketfd=socket(AF_INET,SOCK_STREAM,0))==-1) // AF_INET:Internet;SOCK_STREAM:TCP
    {
        fprintf(stderr,"Socket Error:%s\a\n",strerror(errno));
        exit(1);
    }

    bzero(&server_addr,sizeof(server_addr));
    server_addr.sin_family=AF_INET;          // IPV4
    server_addr.sin_port=htons(port);
    server_addr.sin_addr=*((struct in_addr *)host->h_addr);

    inet_ntop(AF_INET, &(server_addr.sin_addr), bufaddr, len);
    printf("address:%s\n",bufaddr);

    if(connect(socketfd,(struct sockaddr *)(&server_addr),sizeof(struct sockaddr))==-1)
    {
        fprintf(stderr,"Connect Error:%s\a\n",strerror(errno));
        exit(1);
    }

	list_jt808_tx	= msglist_create( &tx_lock );
	list_jt808_rx	= msglist_create( &rx_lock );

    register_jt808_tx(); //start register

    int err = pthread_create(&(tid[0]), NULL, &tx_thread_entry_jt808, NULL);
    if (err != 0)
        printf("\ncan't create thread :[%s]", strerror(err));
    else
        printf("\n Thread created successfully\n");

    err = pthread_create(&(tid[1]), NULL, &rx_thread_entry_jt808, NULL);
    if (err != 0)
        printf("\ncan't create thread :[%s]", strerror(err));
    else
        printf("\n Thread created successfully\n");
}

int main(int argc, char *argv[])
{
	struct hostent *host;

	if (pthread_mutex_init(&tx_lock, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }

	if (pthread_mutex_init(&rx_lock, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }

	printf("lock %p\r\n", &tx_lock);

    if(argc!=3)
    {
        fprintf(stderr,"Usage:%s hostname port \a\n",argv[0]);
        exit(1);
    }

    if((host=gethostbyname(argv[1]))==NULL)
    {
        fprintf(stderr,"Gethostname error\n");
        exit(1);
    }

    start_gps_logging();
    connectToServer(host, atoi(argv[2]));

	while(1){}
   /* printf("Please input char:\n");

    fgets(buffer,1024,stdin);
    write(socketfd,buffer,strlen(buffer));
    */
    close(socketfd);
    pthread_mutex_destroy(&tx_lock);
    pthread_mutex_destroy(&rx_lock);
    exit(0);
}
/************************************** The End Of File **************************************/
