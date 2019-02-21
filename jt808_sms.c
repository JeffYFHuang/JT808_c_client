/*
     SMS.C
 */

#include  <stdlib.h> //����ת�����ַ���
#include  <stdio.h>
#include  <string.h>
#include  "jt808_sms.h"
#include  "Err.h"
#if 0
#define PHONEMAXNUM 15
#define PHONEMAXSTR 150

typedef enum
{
	SMS_TX_IDLE = 0,
	SMS_TX_START,
	SMS_TX_WAITACK,
	SMS_TX_OK,
	SMS_TX_FALSE,
}SMS_TX_STATE;

typedef enum
{
	SMS_RX_IDLE = 0,
	SMS_RX_START,
	SMS_RX_READ,
	SMS_RX_WAITACK,
	SMS_RX_OK,
	SMS_RX_DEL,
	SMS_RX_FALSE,
}SMS_RX_STATE;

typedef struct
{
	char	SCA[15];                                ///�������ĺ���
	char	TPA[15];                                ///���ŷ��ͷ�����
	char	TP_SCTS[14];                            ///�¼�����Ϣ
	uint8_t		TP_PID;                                 ///Э���ʶ
	uint8_t		TP_DCS;                                 ///Э���������
}SmsType;

typedef  struct
{
	uint8_t	SMIndex;                                    // ���ż�¼
	uint8_t	SMS_read;                                   // ��ȡ���ű�־λ
	uint8_t	SMS_delALL;                                 // ɾ�����ж��ű�־λ
	uint8_t	SMS_come;                                   // �ж��Ź�����
	uint8_t	SMS_Style;                                  // ��Ϣ���ͣ�1Ϊpdu��0Ϊtext
	uint8_t	SMS_delayCounter;                           // ������ʱ��
	uint8_t	SMS_waitCounter;                            ///���ŵȴ�
	uint8_t	SMSAtSend[45];                              //����AT����Ĵ���

	uint8_t				SMS_destNum[15];                //  ���Ͷ�ϢĿ�ĺ���
	uint8_t				SMS_sendFlag;                   //  ��Ϣ���ͱ�־λ
	uint8_t				SMS_sd_Content[PHONEMAXSTR];    // ��Ϣ��������
	uint8_t				SMS_rx_Content[PHONEMAXSTR];    // ��Ϣ��������
	SMS_RX_STATE	rx_state;                       //����״̬
	SMS_TX_STATE	tx_state;                       //����״̬
	uint8_t				rx_retry;                       ///�ظ����մ���
	uint8_t				tx_retry;                       ///�ظ����ʹ���
	//------- self sms protocol  ----
	SmsType Sms_Info;                               //������PDU��Ϣ�Ĳ�����Ϣ
} SMS_Style;

typedef  struct
{
	uint8_t		SMS_destNum[PHONEMAXNUM];               //  ���Ͷ�ϢĿ�ĺ���
	char	*pstr;                                  // ��Ϣ��������
} SMS_Send_Msg;

/* ��Ϣ������ƿ�*/
static struct rt_mailbox mb_smsdata;
#define MB_SMSDATA_POOL_SIZE 32
/* ��Ϣ�������õ��ķ�����Ϣ���ڴ��*/
static uint8_t		mb_smsdata_pool[MB_SMSDATA_POOL_SIZE];

static SMS_Style	SMS_Service;   //  ��Ϣ���

static const char	GB_DATA[]	= "����������ش�������������ɽ��ʹ����������³���������ԥ��";
static const char	UCS2_CODE[] = "4EAC6D256CAA5B816E1D743C85CF5DDD7CA497528D3595FD540996558499664B7518684291028D636D5982CF65B09C8176966E589ED18FBD4E918C6B5180";

static void SMS_protocol( uint8_t *instr, uint16_t len, uint8_t ACKstate );


/*��ȡһ��ָ���ַ���λ�ã������ַ���Ϊһ���ַ�����*/
static int StringFind( const char* string, const char* find, int number )
{
	char* pos;
	char* p;
	int count = 0;
	//pos=string;
	//p = string;
	pos = strstr( string, find );
	if( pos == (void*)0 )
	{
		return -1;
	}
	count = pos - string;
	return count;
#ifdef 0
	while( number > 0 )
	{
		/*������ҵ����ַ�λ�õ�ָ�룬�Ա���ʱָ����б���*/
		pos = strstr( p, find );
		/*��λ��ָ��Ϊ0ʱ��˵��û���ҵ�����ַ�*/
		if( pos == (void*)0 )
		{
			return -1;
		}
		/*��λ��ָ�����ʱָ�����˵����һ���ַ�����Ҫ�ҵ��ַ��������ʱָ��С��λ��ָ�룬����б����ַ�������������count��1*/
		while( p <= pos )
		{
			if( *p > 0x80 || *p < 0 )
			{
				p++;
			}
			p++;
			count++;
		}
		/*��Ҫ���ҵĴ�����һ*/
		number--;
	}
	return count;
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
uint16_t Hex_To_Ascii( const uint8_t* pSrc, uint8_t* pDst, uint16_t nSrcLength )
{
	const uint8_t	tab[] = "0123456789ABCDEF"; // 0x0-0xf���ַ����ұ�
	uint16_t			i;

	for( i = 0; i < nSrcLength; i++ )
	{
		// �����4λ
		*pDst++ = tab[*pSrc >> 4];

		// �����4λ
		*pDst++ = tab[*pSrc & 0x0f];

		pSrc++;
	}

	// ����ַ����Ӹ�������
	*pDst = '\0';

	// ����Ŀ���ַ�������
	return ( nSrcLength << 1 );
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
uint16_t Ascii_To_Hex( const uint8_t* pSrc, uint8_t* pDst, uint16_t nSrcLength )
{
	uint16_t i;
	for( i = 0; i < nSrcLength; i += 2 )
	{
		// �����4λ
		if( *pSrc >= '0' && *pSrc <= '9' )
		{
			*pDst = ( *pSrc - '0' ) << 4;
		}     else if( *pSrc >= 'A' && *pSrc <= 'F' )
		{
			*pDst = ( *pSrc - 'A' + 10 ) << 4;
		}else if( *pSrc >= 'a' && *pSrc <= 'f' )
		{
			*pDst = ( *pSrc - 'a' + 10 ) << 4;
		}else
		{
			return false;
		}
		pSrc++;
		// �����4λ
		if( *pSrc >= '0' && *pSrc <= '9' )
		{
			*pDst |= *pSrc - '0';
		}else if( *pSrc >= 'A' && *pSrc <= 'F' )
		{
			*pDst |= *pSrc - 'A' + 10;
		}else if( *pSrc >= 'a' && *pSrc <= 'f' )
		{
			*pDst |= ( *pSrc - 'a' + 10 );
		}else
		{
			return false;
		}
		pSrc++;
		pDst++;
	}

	// ����Ŀ�����ݳ���
	return ( nSrcLength >> 1 );
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
void printer_data_hex(uint8_t *pSrc,uint16_t nSrcLength)
{
	char pDst[3];
 	const uint8_t	tab[] = "0123456789ABCDEF"; // 0x0-0xf���ַ����ұ�
	uint16_t			i;
	
	pDst[2]=0;
	for( i = 0; i < nSrcLength; i++ )
	{
		// �����4λ
		pDst[0] = tab[*pSrc >> 4];
		// �����4λ
		pDst[1] = tab[*pSrc & 0x0f];
		printf("%s",pDst);
		pSrc++;
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
uint16_t GsmDecode8bit( const uint8_t *pSrc, uint8_t *pDst, uint16_t nSrcLength )
{
	uint16_t m;
	// �򵥸���
	for( m = 0; m < nSrcLength; m++ )
	{
		*pDst++ = *pSrc++;
	}
	// ����ַ����Ӹ�������
	*pDst = '\0';
	return nSrcLength;
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
uint16_t  GsmEncode8bit( const uint8_t *pSrc, uint8_t *pDst, uint16_t nSrcLength )
{
	uint16_t m;
	// �򵥸���
	for( m = 0; m < nSrcLength; m++ )
	{
		*pDst++ = *pSrc++;
	}

	return nSrcLength;
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
uint16_t GsmDecodeUcs2_old( const uint8_t* pSrc, uint8_t* pDst, uint16_t nSrcLength )
{
	uint16_t nDstLength = nSrcLength;   // UNICODE���ַ���Ŀ
	uint16_t i;
	// INT16U wchar[128];      // UNICODE��������

	// �ߵ��ֽڶԵ���ƴ��UNICODE
	for( i = 0; i < nSrcLength; i += 2 )
	{
		// �ȸ�λ�ֽ�,��Ϊ�����ݡ����ֽ�Ϊ0
		pSrc++;
		// ���λ�ֽ�
		*pDst++ = *pSrc++;
	}
	// UNICODE��-.�ַ���
	//nDstLength = ::WideCharToMultiByte(CP_ACP, 0, wchar, nSrcLength/2, pDst, 160, NULL, NULL);
	// ����ַ����Ӹ�������
	//pDst[nDstLength] = '\0';
	// ����Ŀ���ַ�������
	return ( nDstLength >> 1 );
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
uint16_t GsmEncodeUcs2_old( const uint8_t* pSrc, uint8_t* pDst, uint16_t nSrcLength )
{
	uint16_t nDstLength = nSrcLength;   // UNICODE���ַ���Ŀ
	uint16_t i;
	//INT16U wchar[128];      // UNICODE��������

	// �ַ���-.UNICODE��
	// nDstLength = ::MultiByteToWideChar(CP_ACP, 0, pSrc, nSrcLength, wchar, 128);

	// �ߵ��ֽڶԵ������
	for( i = 0; i < nDstLength; i++ )
	{
		// �������λ�ֽ�
		*pDst++ = 0x00;
		// �������λ�ֽ�
		*pDst++ = *pSrc++;
	}

	// ����Ŀ����봮����
	return ( nDstLength << 1 );
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
uint16_t GsmDecodeUcs2( const uint8_t* pSrc, uint8_t* pDst, uint16_t nSrcLength )
{
	uint16_t			nDstLength = 0; // UNICODE���ַ���Ŀ
	uint16_t			i;
	s16			indexNum;
	char		strTemp[5];
	const uint8_t	*p;
	// INT16U wchar[128];      // UNICODE��������

	// �ߵ��ֽڶԵ���ƴ��UNICODE
	for( i = 0; i < nSrcLength; i += 2 )
	{
		if( *pSrc ) ///���ֱ���
		{
			p = pSrc;
			Hex_To_Ascii( p, strTemp, 2 );
			strTemp[4]	= 0;
			indexNum	= StringFind( UCS2_CODE, strTemp, strlen( UCS2_CODE ) );
			if( indexNum >= 0 )
			{
				indexNum	= indexNum >> 1;
				*pDst++		= GB_DATA[indexNum];
				*pDst++		= GB_DATA[1 + indexNum];
			}else
			{
				*pDst++ = 0x20; ///����ʶ��ĺ�����"��"��ʾ
				*pDst++ = 0x3B;
				//*pDst++ = *pSrc;		///����ʶ��ĺ�����UCS2Դ���ʾ
				//*pDst++ = *(pSrc+1);
			}
			pSrc		+= 2;
			nDstLength	+= 2;
		}else ///Ӣ�ı���
		{
			// �ȸ�λ�ֽ�,��Ϊ�����ݡ����ֽ�Ϊ0
			pSrc++;
			// ���λ�ֽ�
			*pDst++ = *pSrc++;
			nDstLength++;
		}
	}
	// UNICODE��-.�ַ���
	//nDstLength = ::WideCharToMultiByte(CP_ACP, 0, wchar, nSrcLength/2, pDst, 160, NULL, NULL);
	// ����ַ����Ӹ�������
	//pDst[nDstLength] = '\0';
	// ����Ŀ���ַ�������
	return nDstLength;
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
uint16_t GsmEncodeUcs2( const uint8_t* pSrc, uint8_t* pDst, uint16_t nSrcLength )
{
	uint16_t		nDstLength = nSrcLength; // UNICODE���ַ���Ŀ
	uint16_t		i;
	s16		indexNum;
	char	strTemp[3];
	//INT16U wchar[128];      // UNICODE��������
	nDstLength = 0;
	// �ַ���-.UNICODE��
	// nDstLength = ::MultiByteToWideChar(CP_ACP, 0, pSrc, nSrcLength, wchar, 128);

	// �ߵ��ֽڶԵ������
	for( i = 0; i < nSrcLength; i++ )
	{
		if( ( *pSrc & 0x80 ) == 0x80 ) ///���ֱ���
		{
			strTemp[0]	= *pSrc;
			strTemp[1]	= *( pSrc + 1 );
			strTemp[2]	= 0;
			indexNum	= StringFind( GB_DATA, strTemp, strlen( GB_DATA ) );
			if( indexNum >= 0 )
			{
				indexNum = indexNum * 2;
				Ascii_To_Hex( &UCS2_CODE[indexNum], strTemp, 4 );
				*pDst++ = strTemp[0];
				*pDst++ = strTemp[1];
			}else   ///����ʶ��ĺ�����"��"��ʾ
			{
				*pDst++ = 0x20;
				*pDst++ = 0x3B;
			}
			pSrc += 2;
			i++;
		}else       ///Ӣ�ı���
		{
			// �������λ�ֽ�
			*pDst++ = 0x00;
			// �������λ�ֽ�
			*pDst++ = *pSrc++;
		}
		nDstLength++;
	}

	// ����Ŀ����봮����
	return ( nDstLength << 1 );
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
uint16_t GsmDecode7bit( const uint8_t* pSrc, uint8_t* pDst, uint16_t nSrcLength )
{
	uint16_t nSrc;       // Դ�ַ����ļ���ֵ
	uint16_t nDst;       // Ŀ����봮�ļ���ֵ
	uint16_t nByte;      // ��ǰ���ڴ���������ֽڵ���ţ���Χ��0-6
	uint8_t	nLeft;      // ��һ�ֽڲ��������

	// ����ֵ��ʼ��
	nSrc	= 0;
	nDst	= 0;

	// �����ֽ���źͲ������ݳ�ʼ��
	nByte	= 0;
	nLeft	= 0;

	// ��Դ����ÿ7���ֽڷ�Ϊһ�飬��ѹ����8���ֽ�
	// ѭ���ô�����̣�ֱ��Դ���ݱ�������
	// ������鲻��7�ֽڣ�Ҳ����ȷ����
	while( nSrc < nSrcLength )
	{
		// ��Դ�ֽ��ұ߲��������������ӣ�ȥ�����λ���õ�һ��Ŀ������ֽ�
		*pDst = ( ( *pSrc << nByte ) | nLeft ) & 0x7f;
		// �����ֽ�ʣ�µ���߲��֣���Ϊ�������ݱ�������
		nLeft = *pSrc >> ( 7 - nByte );

		// �޸�Ŀ�괮��ָ��ͼ���ֵ
		pDst++;
		nDst++;
		// �޸��ֽڼ���ֵ
		nByte++;

		// ����һ������һ���ֽ�
		if( nByte == 7 )
		{
			// ����õ�һ��Ŀ������ֽ�
			*pDst = nLeft;

			// �޸�Ŀ�괮��ָ��ͼ���ֵ
			pDst++;
			nDst++;

			// �����ֽ���źͲ������ݳ�ʼ��
			nByte	= 0;
			nLeft	= 0;
		}

		// �޸�Դ����ָ��ͼ���ֵ
		pSrc++;
		nSrc++;
	}

	*pDst = 0;

	// ����Ŀ�괮����
	return nDst;
}

//��ÿ��ascii8λ�����Bit8ȥ�������ν���7λ����ĺ�λ����Ƶ�ǰ�棬�γ��µ�8λ���롣
uint16_t GsmEncode7bit( const uint8_t* pSrc, uint8_t* pDst, uint16_t nSrcLength )
{
	uint16_t nSrc;       // Դ�ַ����ļ���ֵ
	uint16_t nDst;       // Ŀ����봮�ļ���ֵ
	uint16_t nChar;      // ��ǰ���ڴ���������ַ��ֽڵ���ţ���Χ��0-7
	uint8_t	nLeft;      // ��һ�ֽڲ��������

	// ����ֵ��ʼ��
	nSrc	= 0;
	nDst	= 0;

	// ��Դ��ÿ8���ֽڷ�Ϊһ�飬ѹ����7���ֽ�
	// ѭ���ô�����̣�ֱ��Դ����������
	// ������鲻��8�ֽڣ�Ҳ����ȷ����
	while( nSrc < nSrcLength + 1 )
	{
		// ȡԴ�ַ����ļ���ֵ�����3λ
		nChar = nSrc & 7;
		// ����Դ����ÿ���ֽ�
		if( nChar == 0 )
		{
			// ���ڵ�һ���ֽڣ�ֻ�Ǳ�����������������һ���ֽ�ʱʹ��
			nLeft = *pSrc;
		}else
		{
			// ���������ֽڣ������ұ߲��������������ӣ��õ�һ��Ŀ������ֽ�
			*pDst = ( *pSrc << ( 8 - nChar ) ) | nLeft;
			// �����ֽ�ʣ�µ���߲��֣���Ϊ�������ݱ�������
			nLeft = *pSrc >> nChar;
			// �޸�Ŀ�괮��ָ��ͼ���ֵ pDst++;
			pDst++;
			nDst++;
		}

		// �޸�Դ����ָ��ͼ���ֵ
		pSrc++; nSrc++;
	}

	// ����Ŀ�괮����
	return nDst;
}

// �����ߵ����ַ���ת��Ϊ����˳����ַ���
// �磺"8613693092030" -. "683196032930F0"
// pSrc: Դ�ַ���ָ��
// pDst: Ŀ���ַ���ָ��
// nSrcLength: Դ�ַ�������
// ����: Ŀ���ַ�������
uint8_t Hex_Num_Encode( const uint8_t *pSrc, uint8_t *pDst, uint8_t nSrcLength )
{
	uint8_t	nDstLength = nSrcLength;
	uint8_t	i;
	if( nDstLength & 0x01 )
	{
		nDstLength += 1;
	}
	for( i = 0; i < nDstLength; i += 2 )
	{
		*pDst = ( *pSrc << 4 ) | ( *pSrc >> 4 );
		pDst++;
		pSrc++;
	}
	if( nSrcLength & 1 )
	{
		*( pDst - 1 )	&= 0x0f;
		*( pDst - 1 )	|= 0xf0;
	}
	return ( nDstLength >> 1 );
}

// �����ߵ����ַ���ת��Ϊ����˳����ַ���
// �磺"8613693092030" -. "683196032930F0"
// pSrc: Դ�ַ���ָ��
// pDst: Ŀ���ַ���ָ��
// nSrcLength: Դ�ַ�������
// ����: Ŀ���ַ�������
uint8_t Hex_Num_Decode( const uint8_t *pSrc, uint8_t *pDst, uint8_t nSrcLength )
{
	uint8_t	nDstLength = nSrcLength;
	uint8_t	i;
	if( nDstLength & 0x01 )
	{
		nDstLength += 1;
	}
	for( i = 0; i < nDstLength; i += 2 )
	{
		*pDst = ( *pSrc << 4 ) | ( *pSrc >> 4 );
		pDst++;
		pSrc++;
	}
	if( nSrcLength & 1 )
	{
		*( pDst - 1 )	&= 0xf0;
		*( pDst - 1 )	|= 0x0f;
	}
	return ( nDstLength >> 1 );
}

//���غ��볤��
uint8_t Que_Number_Length( const uint8_t *Src )
{
	uint8_t	n = 0;
	uint8_t	m;
	for( m = 0; m < 8; m++ )
	{
		if( ( Src[m] & 0xf0 ) == 0xf0 )
		{
			break;
		}
		n++;
		if( ( Src[m] & 0x0f ) == 0x0f )
		{
			break;
		}
		n++;
	}
	return n;
}

/*********************************************************************************
  *��������:uint16_t SetPhoneNumToPDU(uint8_t *pDest,uint8_t *pSrc,uint16_t len)
  *��������:���ַ�����ʽת����BCD���ʽ��PDU�绰���룬��ԭʼ����Ϊ"13820554863"תΪ13820554863F
  *��    ��:pSrc ԭʼ���ݣ�len�������󳤶�
  *��    ��:pDest ����ַ���
  *�� �� ֵ:	��Ч�ĺ��볤��
  *��    ��:������
  *��������:2013-05-28
  *---------------------------------------------------------------------------------
  *�� �� ��:
  *�޸�����:
  *�޸�����:
*********************************************************************************/
uint16_t SetPhoneNumToPDU( uint8_t *pDest, char *pSrc, uint16_t len )
{
	uint16_t i, j;

	memset( pDest, 0xff, len );
	for( i = 0; i < len; i++ )
	{
		if( ( *pSrc >= 0x30 ) && ( *pSrc <= 0x39 ) )
		{
			if( i % 2 == 0 )
			{
				*pDest	&= 0x0F;
				*pDest	|= ( ( *pSrc - 0x30 ) << 4 );
			}else
			{
				*pDest	&= 0xF0;
				*pDest	|= *pSrc - 0x30;
				pDest++;
			}
		}else
		{
			return i;
		}
		pSrc++;
	}
	return i;
}

/*********************************************************************************
  *��������:void GetPhoneNumFromPDU(uint8_t *pDest,uint8_t *pSrc,uint16_t len)
  *��������:��BCD���ʽ��PDU�绰����ת�����ַ�����ʽ����ԭʼ����Ϊ13820554863FתΪ"13820554863"
  *��    ��:pSrc ԭʼ���ݣ�len�������󳤶�
  *��    ��:pDest ����ַ���
  *�� �� ֵ:	��Ч�ĺ��볤��
  *��    ��:������
  *��������:2013-05-28
  *---------------------------------------------------------------------------------
  *�� �� ��:
  *�޸�����:
  *�޸�����:
*********************************************************************************/
uint16_t GetPhoneNumFromPDU( char *pDest, uint8_t *pSrc, uint16_t len )
{
	uint16_t i, j;
	for( j = 0, i = 0; i < len; i++ )
	{
		if( ( pSrc[i] & 0x0f ) != 0x0f )
		{
			pDest[j++]	= ( pSrc[i] >> 4 ) + 0x30;
			pDest[j++]	= ( pSrc[i] & 0x0f ) + 0x30;
		}else
		{
			pDest[j++]	= ( pSrc[i] >> 4 ) + 0x30;
			pDest[j]	= 0;
			break;
		}
	}
	return j;
}

//ok_bym
//�������
uint16_t   GsmDecodePdu( const uint8_t* pSrc, uint16_t pSrcLength, SmsType *pSmstype, uint8_t *DataDst )
{
	uint8_t	nDstLength = 0;                                             // Ŀ��PDU������
	uint8_t	tmp;                                                        // �ڲ��õ���ʱ�ֽڱ���
	uint8_t	buf[256];                                                   // �ڲ��õĻ�����
	uint16_t templen = 0;
	uint16_t tmp16;
	///0891683108200245F320 0D90 683128504568F3 0008315032419430235E00540057003700300033002300440045005600490043004500490044002800310033003300340035003600370034003800360033002900230049005000310028003100320035002E00330038002E003100380032002E0036003000296D25
	//---SCA
	// SMSC��ַ��Ϣ��
	Ascii_To_Hex( pSrc, &tmp, 2 );                                  // ȡ����
	if( tmp > 0 && tmp <= 12 )
	{
		tmp		= ( tmp - 1 ) * 2;                                  // SMSC���봮����,ȥ����91;
		pSrc	+= 4;                                               // ָ�����,���������ֽڣ�91�����ֽڡ���4���ֽ�
		templen += 4;
		Ascii_To_Hex( pSrc, buf, tmp );                             // ת��SMSC���뵽Ŀ��PDU��
		Hex_Num_Decode( buf, ( *pSmstype ).SCA, tmp );              //ȡ�������ĺ���,���棬�ظ�ʱ��,��HEX�뱣���
		pSrc	+= tmp;                                             // ָ�����,��ʱ����PDUType
		templen += tmp;

		// TPDU�λ����������ظ���ַ��
		//--PDUType
		Ascii_To_Hex( pSrc, &tmp, 2 );                              // ȡ��������
		pSrc	+= 2;                                               // ָ�����
		templen += 2;
		//--OA----star
		// �����ظ���ַ��ȡ�ظ���ַ��Ϣ
		Ascii_To_Hex( pSrc, &tmp, 2 );                              // ȡ����,OA�ĳ�����ָ����ĺ��볤�ȣ�
		if( tmp & 1 )
		{
			tmp += 1;                                               // ������ż��
		}
		pSrc	+= 4;                                               // ָ����ƣ���ȥ���ȣ���91,��4���ֽ�
		templen += 4;
		if( tmp > 0 && tmp <= 12 * 2 )
		{
			Ascii_To_Hex( pSrc, buf, tmp );
			Hex_Num_Decode( buf, ( *pSmstype ).TPA, tmp );          // ȡTP-RA����,����ظ���ַ
			pSrc	+= tmp;                                         // ָ�����
			templen += tmp;
			//--OA---End-------

			///0891683108200245F320 0D90 683128504568F3 0008 31503241943023 5E 00540057003700300033002300440045005600490043004500490044002800310033003300340035003600370034003800360033002900230049005000310028003100320035002E00330038002E003100380032002E0036003000296D25
			//---SCA
			// TPDU��Э���ʶ�����뷽ʽ���û���Ϣ��
			Ascii_To_Hex( pSrc, (uint8_t*)&( *pSmstype ).TP_PID, 2 );    // ȡЭ���ʶ(TP-PID)
			pSrc	+= 2;
			templen += 2;                                           // ָ�����
			Ascii_To_Hex( pSrc, (uint8_t*)&( *pSmstype ).TP_DCS, 2 );    // ȡ���뷽ʽ(TP-DCS),�������ʱ�ͻظ�ʱ��
			pSrc	+= 2;                                           // ָ�����
			templen += 2;
			//GsmSerializeNumbers(pSrc, m_SmsType.TP_SCTS, 14);        // ����ʱ����ַ���(TP_SCTS)
			pSrc	+= 14;                                          // ָ�����
			templen += 14;
			Ascii_To_Hex( pSrc, &tmp, 2 );                          // �û���Ϣ����(TP-UDL)
			pSrc	+= 2;                                           // ָ�����
			templen += 2;
			// pDst.TP_DCS=8;
			if( ( ( *pSmstype ).TP_DCS & 0x0c ) == GSM_7BIT )
			{
				// 7-bit����
				tmp16	= tmp % 8 ? ( (uint16_t)tmp * 7 / 8 + 1 ) : ( (uint16_t)tmp * 7 / 8 );
				tmp16	*= 2;
				if( ( templen + tmp16 <= pSrcLength ) && ( tmp16 < 400 ) )
				{
					nDstLength = Ascii_To_Hex( pSrc, buf, tmp16 );  // ��ʽת��
					GsmDecode7bit( buf, DataDst, nDstLength );      // ת����TP-DU
					nDstLength = tmp;
				}
			}else if( ( ( *pSmstype ).TP_DCS & 0x0c ) == GSM_UCS2 )
			{
				// UCS2����
				tmp16 = tmp * 2;
				if( ( templen + tmp16 <= pSrcLength ) && ( tmp16 < 400 ) )
				{
					nDstLength	= Ascii_To_Hex( pSrc, buf, tmp16 );             // ��ʽת��
					nDstLength	= GsmDecodeUcs2( buf, DataDst, nDstLength );    // ת����TP-DU
				}
			}else
			{
				// 8-bit����
				tmp16 = tmp * 2;
				if( ( templen + tmp16 <= pSrcLength ) && ( tmp16 < 512 ) )
				{
					nDstLength	= Ascii_To_Hex( pSrc, buf, tmp16 );             // ��ʽת��
					nDstLength	= GsmDecode8bit( buf, DataDst, nDstLength );    // ת����TP-DU
				}
			}
		}
	}
	// ����Ŀ���ַ�������
	return nDstLength;
}

//���Ӷ������ĺ���
uint16_t   GsmEncodePdu_NoCenter( const SmsType pSrc, const uint8_t *DataSrc, uint16_t datalen, uint8_t* pDst )
{
	uint16_t nLength;                                                    // �ڲ��õĴ�����
	uint16_t nDstLength = 0;                                             // Ŀ��PDU������
	uint8_t	buf[256];                                                   // �ڲ��õĻ�����

	// SMSC��ַ��Ϣ��
	buf[0]		= 0;                                                //SCA
	nDstLength	+= Hex_To_Ascii( buf, pDst, 1 );
	// TPDU�λ���������Ŀ���ַ��
	buf[0]		= 0x11;                                             //PDUTYPE
	buf[1]		= 0x00;                                             //MR
	nDstLength	+= Hex_To_Ascii( buf, &pDst[nDstLength], 2 );
	// SMSC��ַ��Ϣ��
	nLength		= Que_Number_Length( pSrc.TPA );                    // TP-DA��ַ�ַ����ĳ���
	buf[0]		= (uint8_t)nLength;                                      // Ŀ���ַ���ָ���(TP-DA��ַ�ַ�����ʵ����)
	buf[1]		= 0x91;                                             // �̶�: �ù��ʸ�ʽ����
	nDstLength	+= Hex_To_Ascii( buf, &pDst[nDstLength], 2 );
	nLength		= Hex_Num_Encode( pSrc.TPA, buf, nLength );
	nDstLength	+= Hex_To_Ascii( buf, &pDst[nDstLength], nLength ); // ת��TP-DA��Ŀ��PDU��

	// TPDU��Э���ʶ�����뷽ʽ���û���Ϣ��
	buf[0]		= 0;                                                // Э���ʶ(TP-PID)
	buf[1]		= pSrc.TP_DCS & 0x0c;                               // �û���Ϣ���뷽ʽ(TP-DCS)
	buf[2]		= 0x8f;                                             // ��Ч��(TP-VP)Ϊ12Сʱ
	nDstLength	+= Hex_To_Ascii( buf, &pDst[nDstLength], 3 );
	if( ( pSrc.TP_DCS & 0x0c ) == GSM_7BIT )
	{
		// 7-bit���뷽ʽ
		buf[0]	= datalen;                                          // ����ǰ����.7λ��ʽ��ʾ����ǰ�ĳ���
		nLength = GsmEncode7bit( DataSrc, &buf[1], datalen );
		nLength += 1;
		// ת��		TP-DA��Ŀ��PDU��
	}else if( ( pSrc.TP_DCS & 0x0c ) == GSM_UCS2 )
	{
		// UCS2���뷽ʽ
		buf[0]	= GsmEncodeUcs2( DataSrc, &buf[1], datalen );       // ת��TP-DA��Ŀ��PDU��
		nLength = buf[0] + 1;                                       // nLength���ڸö����ݳ���
	}else
	{
		// 8-bit���뷽ʽ
		buf[0]	= GsmEncode8bit( DataSrc, &buf[1], datalen );       // ת��TP-DA��Ŀ��PDU��
		nLength = buf[0] + 1;                                       // nLength���ڸö����ݳ���
	}
	nDstLength += Hex_To_Ascii( buf, &pDst[nDstLength], nLength );  // ת���ö����ݵ�Ŀ��PDU��

	// ����Ŀ���ַ�������
	return nDstLength;
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
uint16_t   AnySmsEncode_NoCenter( const uint8_t *SrcNumber, uint8_t type, const uint8_t *DataSrc, uint16_t datalen, uint8_t* pDst )
{
	SmsType tmpSms;
	uint8_t		len8;
	len8 = Que_Number_Length( SrcNumber );
	if( *SrcNumber == 0x86 )    ///��0x86���
	{
		len8 = ( len8 + 1 ) >> 1;
		//tmpSms.TPA[0]=0x86;
		memcpy( &tmpSms.TPA[0], SrcNumber, len8 );
	}else                       ///û��0x86���
	{
		len8			= ( len8 + 1 ) >> 1;
		tmpSms.TPA[0]	= 0x86;
		memcpy( &tmpSms.TPA[1], SrcNumber, len8 );
	}
	tmpSms.TP_DCS	= type & 0x0c;
	tmpSms.TP_PID	= 0;
	return ( GsmEncodePdu_NoCenter( tmpSms, DataSrc, datalen, pDst ) );
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
uint16_t GsmEncodePdu_Center( const SmsType pSrc, const uint8_t *DataSrc, uint16_t datalen, uint8_t* pDst )
{
	uint16_t nLength;                                                                // �ڲ��õĴ�����
	uint16_t nDstLength = 0;                                                         // Ŀ��PDU������
	uint8_t	buf[256];                                                               // �ڲ��õĻ�����

	// SMSC��ַ��Ϣ��
	nLength		= Que_Number_Length( pSrc.SCA );                                // SMSC��ַ�ַ����ĳ���
	buf[0]		= (uint8_t)( ( nLength & 1 ) == 0 ? nLength : nLength + 1 ) / 2 + 1; // SMSC��ַ��Ϣ����
	buf[1]		= 0x91;                                                         // �̶�: �ù��ʸ�ʽ����
	nDstLength	= Hex_To_Ascii( buf, pDst, 2 );                                 // ת��2���ֽڵ�Ŀ��PDU��
	nLength		= Hex_Num_Encode( pSrc.SCA, buf, nLength );
	nDstLength	+= Hex_To_Ascii( buf, &pDst[nDstLength], nLength );             // ת��SMSC��Ŀ��PDU��
	// TPDU�λ���������Ŀ���ַ��
	buf[0]		= 0x11;                                                         //PDUTYPE
	buf[1]		= 0x00;                                                         //MR
	nDstLength	+= Hex_To_Ascii( buf, &pDst[nDstLength], 2 );
	// SMSC��ַ��Ϣ��
	nLength		= Que_Number_Length( pSrc.TPA );                                // TP-DA��ַ�ַ����ĳ���
	buf[0]		= (uint8_t)nLength;                                                  // Ŀ���ַ���ָ���(TP-DA��ַ�ַ�����ʵ����)
	buf[1]		= 0x91;                                                         // �̶�: �ù��ʸ�ʽ����
	nDstLength	+= Hex_To_Ascii( buf, &pDst[nDstLength], 2 );
	nLength		= Hex_Num_Encode( pSrc.TPA, buf, nLength );
	nDstLength	+= Hex_To_Ascii( buf, &pDst[nDstLength], nLength );             // ת��TP-DA��Ŀ��PDU��

	// TPDU��Э���ʶ�����뷽ʽ���û���Ϣ��
	buf[0]		= 0;                                                            // Э���ʶ(TP-PID)
	buf[1]		= pSrc.TP_DCS & 0x0c;                                           // �û���Ϣ���뷽ʽ(TP-DCS)
	buf[2]		= 0x8f;                                                         // ��Ч��(TP-VP)Ϊ12Сʱ
	nDstLength	+= Hex_To_Ascii( buf, &pDst[nDstLength], 3 );
	if( ( pSrc.TP_DCS & 0x0c ) == GSM_7BIT )
	{
		// 7-bit���뷽ʽ
		buf[0]	= datalen;                                                      // ����ǰ����.7λ��ʽ��ʾ����ǰ�ĳ���
		nLength = GsmEncode7bit( DataSrc, &buf[1], datalen );
		nLength += 1;
		// ת��		TP-DA��Ŀ��PDU��
	}else if( ( pSrc.TP_DCS & 0x0c ) == GSM_UCS2 )
	{
		// UCS2���뷽ʽ
		buf[0]	= GsmEncodeUcs2( DataSrc, &buf[1], datalen );                   // ת��TP-DA��Ŀ��PDU��
		nLength = buf[0] + 1;                                                   // nLength���ڸö����ݳ���
	}else
	{
		// 8-bit���뷽ʽ
		buf[0]	= GsmEncode8bit( DataSrc, &buf[1], datalen );                   // ת��TP-DA��Ŀ��PDU��
		nLength = buf[0] + 1;                                                   // nLength���ڸö����ݳ���
	}
	nDstLength += Hex_To_Ascii( buf, &pDst[nDstLength], nLength );              // ת���ö����ݵ�Ŀ��PDU��

	// ����Ŀ���ַ�������
	return nDstLength;
}

/*
   // �����ߵ����ַ���ת��Ϊ����˳����ַ���
   // �磺"8613693092030" -. "683196032930F0"
   // pSrc: Դ�ַ���ָ��
   // pDst: Ŀ���ַ���ָ��
   // nSrcLength: Դ�ַ�������
   // ����: Ŀ���ַ�������
   INT16U  GsmSerializeNumbers(const INT8U* pSrc, INT8U* pDst, INT16U nSrcLength)
   {

   INT16U nDstLength;   // Ŀ���ַ�������
    INT8U ch;          // ���ڱ���һ���ַ�

    // ���ƴ�����
    nDstLength = nSrcLength;
   // �����ߵ�
    for(INT16U i=0; i<nSrcLength;i+=2)
    {
        ch = *pSrc++;        // �����ȳ��ֵ��ַ�
   *pDst++ = *pSrc++;   // ���ƺ���ֵ��ַ�
   *pDst++ = ch;        // �����ȳ��ֵ��ַ�
    }

    // �����ַ���'F'��
    if(*(pDst-1) == 'F')
    {
        pDst--;
        nDstLength--;        // Ŀ���ַ������ȼ�1
    }

    // ����ַ����Ӹ�������
   *pDst = '\0';

    // ����Ŀ���ַ�������
    return nDstLength;
   }

   //PDU���еĺ����ʱ�䣬���������ߵ����ַ����������������������ɽ��������任��
   // ����˳����ַ���ת��Ϊ�����ߵ����ַ�����������Ϊ��������'F'�ճ�ż��
   // �磺"8613693092030" -. "683196032930F0"
   // pSrc: Դ�ַ���ָ��
   // pDst: Ŀ���ַ���ָ��
   // nSrcLength: Դ�ַ�������
   // ����: Ŀ���ַ�������
   INT16U  GsmInvertNumbers(const INT8U* pSrc, INT8U* pDst, INT16U nSrcLength)
   {

   INT16U nDstLength;   // Ŀ���ַ�������
    INT8U ch;          // ���ڱ���һ���ַ�

    // ���ƴ�����
    nDstLength = nSrcLength;

    // �����ߵ�
    for(INT16U i=0; i<nSrcLength;i+=2)
    {
        ch = *pSrc++;        // �����ȳ��ֵ��ַ�
   *pDst++ = *pSrc++;   // ���ƺ���ֵ��ַ�
   *pDst++ = ch;        // �����ȳ��ֵ��ַ�
    }

    // Դ��������������
    if(nSrcLength & 1)
   {
   *(pDst-2) = 'F';     // ��'F'
        nDstLength++;        // Ŀ�괮���ȼ�1
    }

    // ����ַ����Ӹ�������
   *pDst = '\0';

    // ����Ŀ���ַ�������
    return nDstLength;
   }

   //�Ӷ������ĺ���
   INT16U  GsmEncodePdu(const SM_PARAM* pSrc, INT8U* pDst)
   {

   INT16U nLength;             // �ڲ��õĴ�����
    INT16U nDstLength;          // Ŀ��PDU������
    INT8U buf[256];  // �ڲ��õĻ�����

    // SMSC��ַ��Ϣ��
    nLength = strlen(pSrc.SCA);    // SMSC��ַ�ַ����ĳ���
    buf[0] = (INT8U)((nLength & 1) == 0 ? nLength : nLength + 1) / 2 + 1;    // SMSC��ַ��Ϣ����
    buf[1] = 0x91;        // �̶�: �ù��ʸ�ʽ����
    nDstLength = Hex_To_Ascii(buf, pDst, 2);        // ת��2���ֽڵ�Ŀ��PDU��
    nDstLength += GsmInvertNumbers(pSrc.SCA, &pDst[nDstLength], nLength);    // ת��SMSC��Ŀ��PDU��
     // TPDU�λ���������Ŀ���ַ��
    nLength = strlen(pSrc.TPA);    // TP-DA��ַ�ַ����ĳ���
   CString rec_number=CString(pSrc.TPA);
    buf[0] = 0x11;            // �Ƿ��Ͷ���(TP-MTI=01)��TP-VP����Ը�ʽ(TP-VPF=10)
    buf[1] = 0;               // TP-MR=0
    buf[2] = (INT8U)nLength;   // Ŀ���ַ���ָ���(TP-DA��ַ�ַ�����ʵ����)
    buf[3] = 0x91;            // �̶�: �ù��ʸ�ʽ����
    nDstLength += Hex_To_Ascii(buf, &pDst[nDstLength], 4);  // ת��4���ֽڵ�Ŀ��PDU��
    nDstLength += GsmInvertNumbers(pSrc.TPA, &pDst[nDstLength], nLength); // ת��TP-DA��Ŀ��PDU��

    // TPDU��Э���ʶ�����뷽ʽ���û���Ϣ��
    nLength = strlen(pSrc.TP_UD);    // �û���Ϣ�ַ����ĳ���
    buf[0] = pSrc.TP_PID;        // Э���ʶ(TP-PID)
    buf[1] = pSrc.TP_DCS;        // �û���Ϣ���뷽ʽ(TP-DCS)
    buf[2] = 0;            // ��Ч��(TP-VP)Ϊ5����
    if(pSrc.TP_DCS == GSM_7BIT)
    {
        // 7-bit���뷽ʽ
        buf[3] = nLength;            // ����ǰ����
        nLength = GsmEncode7bit(pSrc.TP_UD, &buf[4], nLength+1) + 4;
   // ת��		TP-DA��Ŀ��PDU��
    }
    else if(pSrc.TP_DCS == GSM_UCS2)
    {
        // UCS2���뷽ʽ
        buf[3] = GsmEncodeUcs2(pSrc.TP_UD, &buf[4], nLength);    // ת��TP-DA��Ŀ��PDU��
        nLength = buf[3] + 4;        // nLength���ڸö����ݳ���
    }
    else
    {
        // 8-bit���뷽ʽ
        buf[3] = GsmEncode8bit(pSrc.TP_UD, &buf[4], nLength);    // ת��TP-DA��Ŀ��PDU��
        nLength = buf[3] + 4;        // nLength���ڸö����ݳ���
    }
    nDstLength += Hex_To_Ascii(buf, &pDst[nDstLength], nLength);        // ת���ö����ݵ�Ŀ��PDU��

    // ����Ŀ���ַ�������
    return nDstLength;
   }
 */


/*********************************************************************************
  *��������:static void SMS_SendConsoleStr(char *s)
  *��������:����Կڷ��ͳ��ַ�������Ϊprintf���ֽڳ�������
  *��    ��:s ��Ҫ������ַ���
  *��    ��:none
  *�� �� ֵ:none
  *��    ��:������
  *��������:2013-05-29
  *---------------------------------------------------------------------------------
  *�� �� ��:
  *�޸�����:
  *�޸�����:
*********************************************************************************/
static void SMS_SendConsoleStr( char *s )
{
	uint16_t i;
	for( i = 0; i < strlen( s ); i++ )
	{
		printf( "%c", *( s + i ) );
	}
}

/*********************************************************************************
  *��������:void SMS_timer(uint8_t *instr,uint16_t len)
  *��������:���Ŵ����������������Ҫ��һ��1��Ķ�ʱ��������ã����ں���"SMS_Process"�Ķ�ʱ�����
  *��    ��:none
  *��    ��:none
  *�� �� ֵ:none
  *��    ��:������
  *��������:2013-05-29
  *---------------------------------------------------------------------------------
  *�� �� ��:
  *�޸�����:
  *�޸�����:
*********************************************************************************/
void SMS_timer( void )
{
	if( SMS_Service.SMS_waitCounter )
	{
		SMS_Service.SMS_waitCounter--;
	}


	/*
	   if(SMS_Service.SMS_delALL>1)
	   {
	   SMS_Service.SMS_delALL--;
	   }
	 */
	//-------- ������� ------------------


	/*
	   if(SMS_Service.SMS_come==1)
	   {
	   SMS_Service.SMS_delayCounter++;
	   if(SMS_Service.SMS_delayCounter)
	   {
	   SMS_Service.SMS_delayCounter=0;
	   SMS_Service.SMS_come=0;
	   SMS_Service.SMS_read=3;	  // ʹ�ܶ�ȡ
	   }
	   }
	 */
}

/*********************************************************************************
  *��������:uint8_t SMS_Rx_Text(char *instr,char *strDestNum)
  *��������:���յ�TEXT��ʽ�Ķ��Ŵ�����
  *��    ��:instr ԭʼ�������ݣ�strDestNum���յ�����Ϣ�ķ��ͷ�����
  *��    ��:none
  *�� �� ֵ:	1:������ɣ�
   0:��ʾʧ��
  *��    ��:������
  *��������:2013-05-29
  *---------------------------------------------------------------------------------
  *�� �� ��:
  *�޸�����:
  *�޸�����:
*********************************************************************************/
uint8_t SMS_Rx_Text( char *instr, char *strDestNum )
{
	uint16_t len;
	uint8_t	ret = 0;
	len = strlen( strDestNum );
	memset( SMS_Service.SMS_destNum, 0, sizeof( SMS_Service.SMS_destNum ) );
	if( len > sizeof( SMS_Service.SMS_destNum ) )
	{
		len = sizeof( SMS_Service.SMS_destNum );
	}
	memcpy( SMS_Service.SMS_destNum, strDestNum, len );

	len = strlen( instr );
#if 1
	memset( SMS_Service.SMS_rx_Content, 0, sizeof( SMS_Service.SMS_rx_Content ) );
	if( len > sizeof( SMS_Service.SMS_rx_Content ) )
	{
		len = sizeof( SMS_Service.SMS_rx_Content );
	}
	memcpy( SMS_Service.SMS_rx_Content, instr, len );
	SMS_Service.rx_state = SMS_RX_OK;
#endif
#if 0
	printf( "\r\n  ��Ϣ��Դ����:%s", SMS_Service.SMS_destNum );
	printf( "\r\n �����յ���Ϣ: " );
	printf( instr );
	if( strncmp( (char*)instr, "TW703#", 6 ) == 0 )                                             //�����޸�UDP��IP�Ͷ˿�
	{
		//-----------  �Զ��� ��Ϣ�����޸� Э�� ----------------------------------
		SMS_protocol( instr + 5, len - 5, SMS_ACK_msg );
		ret = 1;
	}
#endif
	SMS_Service.SMS_read		= 0;
	SMS_Service.SMS_waitCounter = 0;
	SMS_Service.SMS_Style		= 0;
	//SMS_Service.SMS_delALL		= 1;
	return ret;
}

/*********************************************************************************
  *��������:uint8_t SMS_Rx_PDU(char *instr,uint16_t len)
  *��������:���յ�PDU��ʽ�Ķ��Ŵ�����
  *��    ��:instr ԭʼ�������ݣ�len���յ�����Ϣ���ȣ���λΪ�ֽ�
  *��    ��:none
  *�� �� ֵ:	1:������ɣ�
   0:��ʾʧ��
  *��    ��:������
  *��������:2013-05-29
  *---------------------------------------------------------------------------------
  *�� �� ��:
  *�޸�����:
  *�޸�����:
*********************************************************************************/
uint8_t SMS_Rx_PDU( char *instr, uint16_t len )
{
	char	*pstrTemp;
	uint8_t		ret = 0;
#if 1
	//printf( "\r\n PDU_����: " );
	//SMS_SendConsoleStr( instr );
	memset( SMS_Service.SMS_rx_Content, 0, sizeof( SMS_Service.SMS_rx_Content ) );

	len = GsmDecodePdu( instr, len, &SMS_Service.Sms_Info, SMS_Service.SMS_rx_Content );
	GetPhoneNumFromPDU( SMS_Service.SMS_destNum, SMS_Service.Sms_Info.TPA, sizeof( SMS_Service.Sms_Info.TPA ) );
	SMS_Service.rx_state = SMS_RX_OK;
#endif
#if 0
	memset( SMS_Service.SMS_destNum, 0, sizeof( SMS_Service.SMS_destNum ) );
	pstrTemp = (char*)rt_malloc( 200 ); ///���Ž������������ݣ��������ΪGB��
	memset( pstrTemp, 0, 200 );
	//printf( "\r\n ����ԭʼ��Ϣ: " );
	//SMS_SendConsoleStr( instr );

	len = GsmDecodePdu( instr, len, &SMS_Service.Sms_Info, pstrTemp );
	GetPhoneNumFromPDU( SMS_Service.SMS_destNum, SMS_Service.Sms_Info.TPA, sizeof( SMS_Service.Sms_Info.TPA ) );

	printf( "\r\n  ��Ϣ��Դ����:%s \r\n", SMS_Service.SMS_destNum );
	printf( "\r\n ������Ϣ: " );
	SMS_SendConsoleStr( pstrTemp );
	//rt_hw_console_output(instr);
	if( strncmp( (char*)pstrTemp, "TW703#", 6 ) == 0 )                                             //�����޸�UDP��IP�Ͷ˿�
	{
		//-----------  �Զ��� ��Ϣ�����޸� Э�� ----------------------------------
		SMS_protocol( pstrTemp + 5, len - 5, SMS_ACK_msg );
		ret = 1;
	}
#endif
	SMS_Service.SMS_read		= 0;
	SMS_Service.SMS_waitCounter = 0;
	SMS_Service.SMS_Style		= 1;
	//SMS_Service.SMS_delALL		= 1;
	//rt_free( pstrTemp );
	//pstrTemp = RT_NULL;
	//////
	return ret;
}

/*********************************************************************************
  *��������:uint8_t SMS_Tx_Text(char *strDestNum,char *s)
  *��������:����TEXT��ʽ�Ķ��ź���
  *��    ��:s ԭʼ�������ݣ�strDestNum���շ�����
  *��    ��:none
  *�� �� ֵ:	1:������ɣ�
   0:��ʾʧ��
  *��    ��:������
  *��������:2013-05-29
  *---------------------------------------------------------------------------------
  *�� �� ��:
  *�޸�����:
  *�޸�����:
*********************************************************************************/
uint8_t SMS_Tx_Text( char *strDestNum, char *s )
{
	uint16_t		len;
	char	*pstrTemp;

	memset( SMS_Service.SMSAtSend, 0, sizeof( SMS_Service.SMSAtSend ) );
	strcpy( ( char* )SMS_Service.SMSAtSend, "AT+CMGS=\"" );
	//strcat(SMS_Service.SMSAtSend,"8613820554863");// Debug
	strcat( SMS_Service.SMSAtSend, strDestNum );
	strcat( SMS_Service.SMSAtSend, "\"\r\n" );

	printf( "\r\n%s", SMS_Service.SMSAtSend );
	at( ( char* )SMS_Service.SMSAtSend );

	rt_thread_delay( 50 );

	pstrTemp = rt_malloc( PHONEMAXSTR );
	memset( pstrTemp, 0, PHONEMAXSTR );
	len = strlen( s );
	memcpy( pstrTemp, s, len );
	pstrTemp[len++] = 0x1A; // message  end

	///���͵�����Ϣ
	SMS_SendConsoleStr( pstrTemp );
	///���͵�GSMģ��
	at( pstrTemp, len );
	rt_free( pstrTemp );
	pstrTemp = RT_NULL;
	return 1;
}

FINSH_FUNCTION_EXPORT( SMS_Tx_Text, SMS_Tx_Text );


/*********************************************************************************
  *��������:uint8_t SMS_Tx_PDU(char *strDestNum,char *s)
  *��������:����PDU��ʽ�Ķ��ź���
  *��    ��:s ԭʼ�������ݣ�strDestNum���շ�����
  *��    ��:none
  *�� �� ֵ:	1:������ɣ�
   0:��ʾʧ��
  *��    ��:������
  *��������:2013-05-29
  *---------------------------------------------------------------------------------
  *�� �� ��:
  *�޸�����:
  *�޸�����:
*********************************************************************************/
uint8_t SMS_Tx_PDU( char *strDestNum, char *s )
{
	uint16_t		len;
	uint16_t		i;
	char	*pstrTemp;
	memset( SMS_Service.SMSAtSend, 0, sizeof( SMS_Service.SMSAtSend ) );
	pstrTemp = rt_malloc( 400 );
	memset( pstrTemp, 0, 400 );
	i = 0;
	SetPhoneNumToPDU( SMS_Service.Sms_Info.TPA, strDestNum, sizeof( SMS_Service.Sms_Info.TPA ) );
	len = AnySmsEncode_NoCenter( SMS_Service.Sms_Info.TPA, GSM_UCS2, s, strlen( s ), pstrTemp );
	//len=strlen(pstrTemp);
	pstrTemp[len++] = 0x1A; // message  end
	//////
	sprintf( ( char* )SMS_Service.SMSAtSend, "AT+CMGS=%d\r\n", ( len - 2 ) / 2 );
	//printf("%s",SMS_Service.SMSAtSend);
	//at( ( char * ) SMS_Service.SMSAtSend );

	if( gsm_send( SMS_Service.SMSAtSend, RT_NULL, ">", RESP_TYPE_STR, RT_TICK_PER_SECOND, 1 ) == RT_EOK )
	{
		//rt_thread_delay(50);
		//////
		//SMS_SendConsoleStr( pstrTemp );
		//rt_hw_console_output(pstrTemp);
		//at(pstrTemp, len);

		gsm_send( pstrTemp, RT_NULL, RT_NULL, RESP_TYPE_STR, RT_TICK_PER_SECOND / 2, 1 );
	}
	rt_free( pstrTemp );
	pstrTemp = RT_NULL;
	return 1;
}

FINSH_FUNCTION_EXPORT( SMS_Tx_PDU, SMS_Tx_PDU );


/*********************************************************************************
  *��������:uint8_t SMS_Rx_Notice(uint16_t indexNum)
  *��������:ģ���յ��¶���֪ͨ
  *��    ��:�¶��ŵ�������
  *��    ��:none
  *�� �� ֵ:	1:������ɣ�
   0:��ʾʧ��
  *��    ��:������
  *��������:2013-05-29
  *---------------------------------------------------------------------------------
  *�� �� ��:
  *�޸�����:
  *�޸�����:
*********************************************************************************/
uint8_t SMS_Rx_Notice( uint16_t indexNum )
{
	SMS_Service.SMIndex = indexNum;
	printf( " index=%d", SMS_Service.SMIndex );
	SMS_Service.SMS_read		= 3;
	SMS_Service.SMS_waitCounter = 1;
	return 1;
}

///���Ժ���������SMS���Ž��մ��������������ȷ��
void SMS_Test( char * s )
{
	SMS_protocol( s, strlen( s ), SMS_ACK_msg );
}

FINSH_FUNCTION_EXPORT( SMS_Test, SMS_Test );

///���Ժ���������PDU���ݰ��Ľ��ս�������
void SMS_PDU( char *s )
{
	uint16_t		len;
	uint16_t		i, j;
	char	*pstrTemp;
	pstrTemp	= (char*)rt_malloc( 160 ); ///���Ž������������ݣ��������ΪGB��
	len			= GsmDecodePdu( s, strlen( s ), &SMS_Service.Sms_Info, pstrTemp );
	GetPhoneNumFromPDU( SMS_Service.SMS_destNum, SMS_Service.Sms_Info.SCA, sizeof( SMS_Service.Sms_Info.SCA ) );
	printf( "\r\n  ��Ϣ���ĺ���:%s \r\n", SMS_Service.SMS_destNum );
	GetPhoneNumFromPDU( SMS_Service.SMS_destNum, SMS_Service.Sms_Info.TPA, sizeof( SMS_Service.Sms_Info.TPA ) );
	printf( "\r\n  ��Ϣ��Դ����:%s \r\n", SMS_Service.SMS_destNum );
	printf( "\r\n ������Ϣ:\"%s\"\r\n", pstrTemp );
	rt_free( pstrTemp );
	pstrTemp = RT_NULL;
}

FINSH_FUNCTION_EXPORT( SMS_PDU, SMS_PDU );


/*********************************************************************************
  *��������:rt_err_t sms_init(void)
  *��������:���ų�ʼ��������ɶ�����Ϣ���м���ز����ĳ�ʼ��
  *��    ��:
  *��    ��:
  *�� �� ֵ:	rt_err_t
  *��    ��:������
  *��������:2013-06-8
  *---------------------------------------------------------------------------------
  *�� �� ��:
  *�޸�����:
  *�޸�����:
*********************************************************************************/
rt_err_t sms_init( void )
{
	SMS_Service.tx_state	= SMS_TX_IDLE;
	SMS_Service.rx_state	= SMS_RX_IDLE;
	return rt_mb_init( &mb_smsdata, "smsdata", &mb_smsdata_pool, MB_SMSDATA_POOL_SIZE / 4, RT_IPC_FLAG_FIFO );
}

/*********************************************************************************
  *��������:rt_err_t sms_tx_sms(char *strDestNum,char *s)
  *��������:���ŷ��ͺ������ú���������ֱ�ӽ����ŷ��������ǽ����ŷ��뷢�Ͷ����У�
   �ȴ�֮ǰ�Ķ��ųɹ����ͺ��ڰ����Ⱥ�˳���͡�
  *��    ��:
  *��    ��:
  *�� �� ֵ:	rt_err_t
  *��    ��:������
  *��������:2013-06-8
  *---------------------------------------------------------------------------------
  *�� �� ��:
  *�޸�����:
  *�޸�����:
*********************************************************************************/
rt_err_t sms_tx_sms( char *strDestNum, char *s )
{
	uint8_t				temp_u8;
	SMS_Send_Msg	*pmsg = RT_NULL;
	pmsg = rt_malloc( strlen( s ) + PHONEMAXNUM ); /*����������Ϣ*/
	if( pmsg != RT_NULL )
	{
		if( strlen( strDestNum ) > PHONEMAXNUM )
		{
			temp_u8 = PHONEMAXNUM;
		} else
		{
			temp_u8 = strlen( strDestNum );
		}
		memset( pmsg->SMS_destNum, 0, PHONEMAXNUM );
		memcpy( pmsg->SMS_destNum, strDestNum, temp_u8 );
		memcpy( pmsg->pstr, s, strlen( s ) );
		return rt_mb_send( &mb_smsdata, ( rt_uint32_t )pmsg );
	}
	return RT_ERROR;
}

/*********************************************************************************
  *��������:uint8_t sms_rx_pro(uint8_t *psrc,uint16_t len)
  *��������:M66����ģ�����ݷַ��̵߳��øú��������SMS���ŵĽ��մ�����̣��ú������sms_process()��
   �����ʹ����ɶ��ŵķ��ͺͽ��չ���
  *��    ��:psrc M66ģ����յ���һ���ַ�����ָ�룬len �ַ�������
  *��    ��:
  *�� �� ֵ:	1:��ʾ����Ϣ���ڶ�����Ϣ
   0:��ʾ����Ϣ����ʶ��
  *��    ��:������
  *��������:2013-06-8
  *---------------------------------------------------------------------------------
  *�� �� ��:
  *�޸�����:
  *�޸�����:
*********************************************************************************/
uint8_t SMS_rx_pro( char *psrc, uint16_t len )
{
	uint16_t			i, j, q;
	static uint8_t	SMS_Rx_State = 0;
	static char strTempBuf[15];

	if( SMS_Rx_State )
	{
		if( SMS_Rx_State == 1 ) ///PDUģʽ
		{
			SMS_Rx_PDU( psrc, len );
		}else ///TEXTģʽ
		{
			SMS_Rx_Text( psrc, strTempBuf );
		}
		SMS_Rx_State = 0;
		return 1;
	}else if( strncmp( psrc, "+CMT: ", 6 ) == 0 )
	{
		if( psrc[6] == ',' ) ///PDUģʽ
		{
			SMS_Rx_State = 1;
		}else ///TEXTģʽ
		{
			SMS_Rx_State	= 2;
			j				= 0;
			q				= 0;
			memset( strTempBuf, 0, sizeof( strTempBuf ) );
			for( i = 6; i < len; i++ )
			{
				if( ( j == 1 ) && ( psrc[i] != '"' ) )
				{
					strTempBuf[q++] = psrc[i];
				}
				if( psrc[i] == '"' )
				{
					j++;
					if( j > 1 )
					{
						break;
					}
				}
			}
			//printf( "\r\n  ��Ϣ��Դ����:%s \r\n", reg_str );
		}
		return 1;
	}else if( strncmp( psrc, "+CMTI: \"SM\",", 12 ) == 0 )
	{
		printf( "\r\n�յ�����:" );
		j = sscanf( psrc + 12, "%d", &i );
		if( j )
		{
			SMS_Service.SMIndex		= j;
			SMS_Service.rx_state	= SMS_RX_START;
			return 1;
		}
	}
#ifdef SMS_TYPE_PDU
	else if( strncmp( (char*)psrc, "+CMGR:", 6 ) == 0 )
	{
		SMS_Rx_State = 1;
		return 1;
	}
#else
	else if( strncmp( (char*)psrc, "+CMGR:", 6 ) == 0 )
	{
		//+CMGR: "REC UNREAD","8613602069191", ,"13/05/16,13:05:29+35"
		// ��ȡҪ���ض�Ϣ��Ŀ�ĺ���
		j	= 0;
		q	= 0;
		memset( strTempBuf, 0, sizeof( strTempBuf ) );
		for( i = 6; i < 50; i++ )
		{
			if( ( j == 3 ) && ( psrc[i] != '"' ) )
			{
				strTempBuf[q++] = psrc[i];
			}
			if( psrc[i] == '"' )
			{
				j++;
			}
		}
		SMS_Rx_State = 2;
		return 1;
	}
#endif

	////���Ͷ������
	else if( strncmp( (char*)psrc, "+CMGS:", 6 ) == 0 )
	{
		if( SMS_Service.tx_state == SMS_TX_WAITACK )
		{
			SMS_Service.tx_state = SMS_TX_OK;
		}
		return 1;
	}else if( strncmp( (char*)psrc, "+CMS ERROR:", 11 ) == 0 )
	{
		if( SMS_Service.tx_state == SMS_TX_WAITACK )
		{
			SMS_Service.tx_state = SMS_TX_START;
		}
		return 1;
	}
	return 0;
}

/*********************************************************************************
  *��������:void SMS_protocol(uint8_t *instr,uint16_t len)
  *��������:���Ŵ����������������Ҫ��һ���߳�������ã�������ش���(���Ŷ�ȡ��ɾ�����Զ��������)
  *��    ��:none
  *��    ��:none
  *�� �� ֵ:none
  *��    ��:������
  *��������:2013-05-29
  *---------------------------------------------------------------------------------
  *�� �� ��:
  *�޸�����:
  *�޸�����:
*********************************************************************************/
void SMS_Process_old( void )
{
	uint16_t				ContentLen = 0;
	uint16_t				i, j, k;
	char			*pstrTemp;
	SMS_Send_Msg	*pmsg = RT_NULL;

	if( SMS_Service.SMS_waitCounter )
	{
		return;
	}
	//-----------  ���Ŵ������ --------------------------------------------------------
	//---------------------------------
	if( SMS_Service.SMS_read ) // ��ȡ����
	{
		memset( SMS_Service.SMSAtSend, 0, sizeof( SMS_Service.SMSAtSend ) );
		sprintf( SMS_Service.SMSAtSend, "AT+CMGR=%d\r\n", SMS_Service.SMIndex );
		printf( "%s", SMS_Service.SMSAtSend );
		at( ( char* )SMS_Service.SMSAtSend );
		SMS_Service.SMS_read--;
		SMS_Service.SMS_waitCounter = 3;
	}
	//-------------------------------
	//       ���Ͷ�Ϣȷ��
	else if( SMS_Service.SMS_sendFlag == 1 )
	{
		//#ifdef SMS_TYPE_PDU
		if( SMS_Service.SMS_Style == 1 )
		{
			memset( SMS_Service.SMSAtSend, 0, sizeof( SMS_Service.SMSAtSend ) );
			///����600�ֽڿռ�
			pstrTemp = rt_malloc( 400 );
			memset( pstrTemp, 0, 400 );
			///���ַ�����ʽ��Ŀ�ĵ绰��������ΪPDU��ʽ�ĺ���
			SetPhoneNumToPDU( SMS_Service.Sms_Info.TPA, SMS_Service.SMS_destNum, sizeof( SMS_Service.Sms_Info.TPA ) );
			///����PDU��ʽ��������
			ContentLen = AnySmsEncode_NoCenter( SMS_Service.Sms_Info.TPA, GSM_UCS2, SMS_Service.SMS_sd_Content, strlen( SMS_Service.SMS_sd_Content ), pstrTemp );
			//ContentLen=strlen(pstrTemp);
			///��Ӷ���β�����"esc"
			pstrTemp[ContentLen] = 0x1A; // message  end
			//////
			sprintf( ( char* )SMS_Service.SMSAtSend, "AT+CMGS=%d\r\n", ( ContentLen - 2 ) / 2 );
			printf( "%s", SMS_Service.SMSAtSend );
			at( ( char* )SMS_Service.SMSAtSend );
			rt_thread_delay( 50 );
			//////
			//printf("%s",pstrTemp);
			SMS_SendConsoleStr( pstrTemp );
			at( ( char* )pstrTemp, ContentLen + 1 );
			rt_free( pstrTemp );
			pstrTemp = RT_NULL;
		}
		//#else
		else
		{
			memset( SMS_Service.SMSAtSend, 0, sizeof( SMS_Service.SMSAtSend ) );
			strcpy( ( char* )SMS_Service.SMSAtSend, "AT+CMGS=\"" );
			//strcat(SMS_Service.SMSAtSend,"8613820554863");// Debug
			strcat( SMS_Service.SMSAtSend, SMS_Service.SMS_destNum );
			strcat( SMS_Service.SMSAtSend, "\"\r\n" );

			printf( "\r\n%s", SMS_Service.SMSAtSend );
			at( ( char* )SMS_Service.SMSAtSend );

			rt_thread_delay( 50 );
			ContentLen								= strlen( SMS_Service.SMS_sd_Content );
			SMS_Service.SMS_sd_Content [ContentLen] = 0x1A; // message  end
			printf( "%s", SMS_Service.SMS_sd_Content );
			at( ( char* )SMS_Service.SMS_sd_Content, ContentLen + 1 );
		}
		//#endif
		SMS_Service.SMS_sendFlag	= 0;                    // clear
		SMS_Service.SMS_waitCounter = 3;
	}


	/*
	   else if(SMS_Service.SMS_delALL==1)	  //ɾ������
	   {
	   memset(SMS_Service.SMSAtSend,0,sizeof(SMS_Service.SMSAtSend));
	   ///
	   //sprintf(SMS_Service.SMSAtSend,"AT+CMGD=%d\r\n",SMS_Service.SMIndex);
	   sprintf(SMS_Service.SMSAtSend,"AT+CMGD=0,4\r\n",SMS_Service.SMIndex);
	   printf("%s",SMS_Service.SMSAtSend);
	   ///
	   at( ( char * )SMS_Service.SMSAtSend );
	   SMS_Service.SMS_delALL=0;
	   SMS_Service.SMS_waitCounter=3;
	   }
	 */
}

/*********************************************************************************
  *��������:void SMS_protocol(uint8_t *instr,uint16_t len)
  *��������:���Ŵ����������������Ҫ��һ���߳�������ã�������ش���(���Ŷ�ȡ��ɾ�����Զ��������)
  *��    ��:none
  *��    ��:none
  *�� �� ֵ:none
  *��    ��:������
  *��������:2013-05-29
  *---------------------------------------------------------------------------------
  *�� �� ��:
  *�޸�����:
  *�޸�����:
*********************************************************************************/
void SMS_Process( void )
{
	uint16_t				ContentLen = 0;
	uint16_t				i, j, k;
	char			*pstrTemp;
	SMS_Send_Msg	*pmsg = RT_NULL;
	static uint32_t rx_tick;
	static uint32_t tx_tick;
	static char		tx_dest_num[PHONEMAXNUM];
	static char		tx_dest_str[PHONEMAXSTR];

	///���ŷ�����ش���
	switch( SMS_Service.tx_state )
	{
		case SMS_TX_IDLE:
		{
			if( RT_EOK == rt_mb_recv( &mb_smsdata, ( rt_uint32_t* )&pmsg, 0 ) )
			{
				if( RT_NULL != pmsg )
				{
					pstrTemp = (char*)pmsg;
					memset( tx_dest_num, 0, sizeof( tx_dest_num ) );
					memset( tx_dest_str, 0, sizeof( tx_dest_str ) );
					memcpy( tx_dest_num, pstrTemp, PHONEMAXNUM );
					pstrTemp += PHONEMAXNUM;
					memcpy( tx_dest_str, pstrTemp, strlen( pstrTemp ) );
					SMS_Service.tx_state	= SMS_TX_START;
					SMS_Service.tx_retry	= 0;
				}
				rt_free( (void*)pmsg );
			}
			break;
		}
		case SMS_TX_START:
		{
			if( SMS_Service.tx_retry >= 3 )
			{
				SMS_Service.tx_state = SMS_TX_FALSE;
				break;
			}
			SMS_Tx_PDU( tx_dest_num, tx_dest_str );
			SMS_Service.tx_retry++;
			//SMS_Service.tx_state=SMS_TX_WAITACK;	///�ж��Ƿ�ɹ�����
			SMS_Service.tx_state	= SMS_TX_WAITACK;                        ///���ж��Ƿ�ɹ����ͣ�ֱ�ӷ��سɹ�
			tx_tick					= rt_tick_get( );
			break;
		}
		case SMS_TX_WAITACK:
		{
			if( rt_tick_get( ) - tx_tick > RT_TICK_PER_SECOND * 20 )    ///�ж��Ƿ�ʱ
			{
				SMS_Service.tx_state = SMS_TX_START;
			}
			break;
		}
		case SMS_TX_OK:
		{
			SMS_Service.tx_state = SMS_TX_IDLE;
			break;
		}
		case SMS_TX_FALSE:
		{
			SMS_Service.tx_state = SMS_TX_START;
			break;
		}
		default:
		{
			SMS_Service.tx_state = SMS_TX_IDLE;
		}
	}

	if( SMS_Service.SMS_waitCounter )
	{
		return;
	}
	///���Ž�����ش���
	switch( SMS_Service.rx_state )
	{
		case SMS_RX_IDLE:
		{
			SMS_Service.rx_retry = 0;
			break;
		}
		case SMS_RX_START:
		{
			rx_tick					= rt_tick_get( );
			SMS_Service.rx_retry	= 0;
			SMS_Service.rx_state	= SMS_RX_READ;
			break;
		}
		case SMS_RX_READ:
		{
			if( rt_tick_get( ) - rx_tick > RT_TICK_PER_SECOND / 2 ) ///�ȴ�һ��ʱ����ٶ�ȡ����
			{
				if( SMS_Service.rx_retry >= 3 )
				{
					SMS_Service.rx_state = SMS_RX_FALSE;
					break;
				}
				memset( SMS_Service.SMSAtSend, 0, sizeof( SMS_Service.SMSAtSend ) );
				sprintf( SMS_Service.SMSAtSend, "AT+CMGR=%d\r\n", SMS_Service.SMIndex );
				printf( "%s", SMS_Service.SMSAtSend );
				at( ( char* )SMS_Service.SMSAtSend );
				SMS_Service.rx_retry++;
				SMS_Service.tx_state	= SMS_RX_WAITACK;               ///�ж��Ƿ�ɹ���ȡ
				rx_tick					= rt_tick_get( );
			}
			break;
		}
		case SMS_RX_WAITACK:
		{
			if( rt_tick_get( ) - rx_tick > RT_TICK_PER_SECOND * 10 )    ///�ж��Ƿ�ʱ
			{
				SMS_Service.rx_state = SMS_RX_READ;
			}
			break;
		}
		case SMS_RX_OK:
		{
			if( strncmp( (char*)SMS_Service.SMS_rx_Content, "TW703#", 6 ) == 0 )                                        //�����޸�UDP��IP�Ͷ˿�
			{
				ContentLen = strlen( SMS_Service.SMS_rx_Content );
				//-----------  �Զ��� ��Ϣ�����޸� Э�� ----------------------------------
				SMS_protocol( SMS_Service.SMS_rx_Content + 5, ContentLen - 5, SMS_ACK_msg );
			}
			SMS_Service.rx_state = SMS_RX_IDLE;
			//SMS_Service.rx_state=SMS_RX_DEL;
			break;
		}
		case SMS_RX_DEL:
		{
			memset( SMS_Service.SMSAtSend, 0, sizeof( SMS_Service.SMSAtSend ) );
			//sprintf(SMS_Service.SMSAtSend,"AT+CMGD=%d\r\n",SMS_Service.SMIndex);	///ɾ��ָ������
			sprintf( SMS_Service.SMSAtSend, "AT+CMGD=0,4\r\n" ); ///ɾ�����ж���
			printf( "%s", SMS_Service.SMSAtSend );
			SMS_Service.rx_state = SMS_RX_IDLE;
			break;
		}
		case SMS_RX_FALSE:
		{
			SMS_Service.rx_state = SMS_RX_IDLE;
			break;
		}
		default:
		{
			SMS_Service.rx_state = SMS_RX_IDLE;
		}
	}


	/*
	   //-------------------------------
	   //       ���Ͷ�Ϣȷ��
	   else if(SMS_Service.SMS_sendFlag==1)
	   {
	   //#ifdef SMS_TYPE_PDU
	   if(SMS_Service.SMS_Style==1)
	   {
	   SMS_Tx_PDU(SMS_Service.SMS_destNum,SMS_Service.SMS_sd_Content);
	   }
	   //#else
	   else
	   {
	   SMS_Tx_Text(SMS_Service.SMS_destNum,SMS_Service.SMS_sd_Content);
	   }
	   //#endif
	   SMS_Service.SMS_sendFlag=0;  // clear
	   SMS_Service.SMS_waitCounter=3;
	   }
	   else if(SMS_Service.SMS_delALL==1)	  //ɾ������
	   {
	   memset(SMS_Service.SMSAtSend,0,sizeof(SMS_Service.SMSAtSend));
	   ///
	   //sprintf(SMS_Service.SMSAtSend,"AT+CMGD=%d\r\n",SMS_Service.SMIndex);
	   sprintf(SMS_Service.SMSAtSend,"AT+CMGD=0,4\r\n",SMS_Service.SMIndex);
	   printf("%s",SMS_Service.SMSAtSend);
	   ///
	   at( ( char * )SMS_Service.SMSAtSend );
	   SMS_Service.SMS_delALL=0;
	   SMS_Service.SMS_waitCounter=3;
	   }
	 */
}

///���ӷ��Ͷ�����������ݣ�����λ���Ͷ�Ϣ��ǣ��ɹ�����true��ʧ�ܷ���false
uint8_t Add_SMS_Ack_Content( char * instr, uint8_t ACKflag )
{
	if( ACKflag == 0 )
	{
		return false;
	}

	if( strlen( instr ) + strlen( SMS_Service.SMS_sd_Content ) < sizeof( SMS_Service.SMS_sd_Content ) )
	{
		strcat( (char*)SMS_Service.SMS_sd_Content, instr );
		SMS_Service.SMS_sendFlag = 1;
		return true;
	}
	return false;
}

/*********************************************************************************
  *��������:void SMS_protocol(uint8_t *instr,uint16_t len)
  *��������:���յ����ź����������
  *��    ��:instrԭʼ�������ݣ�len����
  *��    ��:none
  *�� �� ֵ:none
  *��    ��:������
  *��������:2013-05-29
  *---------------------------------------------------------------------------------
  *�� �� ��:
  *�޸�����:
  *�޸�����:
*********************************************************************************/
void SMS_protocol( uint8_t *instr, uint16_t len, uint8_t ACKstate )
{
	SMS_Tx_PDU( SMS_Service.SMS_destNum, instr );
}
#endif
#if 0
/*********************************************************************************
  *��������:void SMS_protocol(uint8_t *instr,uint16_t len)
  *��������:���յ����ź����������
  *��    ��:instrԭʼ�������ݣ�len����
  *��    ��:none
  *�� �� ֵ:none
  *��    ��:������
  *��������:2013-05-29
  *---------------------------------------------------------------------------------
  *�� �� ��:
  *�޸�����:
  *�޸�����:
*********************************************************************************/
void   SMS_protocol( uint8_t *instr, uint16_t len, uint8_t ACKstate )  //  ACKstate
{
	char	sms_content[60];                            ///����������"()"֮�������
	char	sms_ack_data[60];                           ///����ÿ���������'#'������������
	uint8_t		u8TempBuf[6];
	uint16_t		i = 0, j = 0;
	uint16_t		cmdLen, u16Temp;
	char	*p_Instr;
	char	*pstrTemp, *pstrTempStart, *pstrTempEnd;

	//SYSID		///�޸ĸ�ֵ������flash
	///Ӧ����Ű�ͷ����
	memset( SMS_Service.SMS_sd_Content, 0, sizeof( SMS_Service.SMS_sd_Content ) );
	strcpy( SMS_Service.SMS_sd_Content, JT808Conf_struct.Vechicle_Info.Vech_Num );
	strcat( SMS_Service.SMS_sd_Content, "#" );                  // Debug
	strcat( SMS_Service.SMS_sd_Content, SIM_CardID_JT808 );     // Debug
	/*************************������Ϣ****************************/
	p_Instr = (char*)instr;
	for( i = 0; i < len; i++ )
	{
		pstrTemp = strchr( p_Instr, '#' );                      ///���������Ƿ����
		//instr++;
		if( pstrTemp )
		{
			p_Instr			= pstrTemp + 1;
			pstrTempStart	= strchr( (char*)pstrTemp, '(' );   ///�����������ݿ�ʼλ��
			pstrTempEnd		= strchr( (char*)pstrTemp, ')' );   ///�����������ݽ���λ��
			if( ( NULL == pstrTempStart ) || ( NULL == pstrTempEnd ) )
			{
				break;
			}
			printf( "\r\n���������ʽ��Ч !" );
			///��ȡ��������
			memset( sms_ack_data, 0, sizeof( sms_ack_data ) );
			memcpy( sms_ack_data, pstrTemp, pstrTempEnd - pstrTemp + 1 );

			///��ȡ�����������������Լ���������
			pstrTempStart++;
			pstrTemp++;
			cmdLen = pstrTempEnd - pstrTempStart;
			memset( sms_content, 0, sizeof( sms_content ) );
			rt_memcpy( sms_content, pstrTempStart, cmdLen );

			///��������ƥ��
			if( strncmp( pstrTemp, "DNSR", 4 ) == 0 )   ///  1. ��������
			{
				if( cmdLen <= sizeof( DomainNameStr ) )
				{
					if( pstrTemp[4] == '1' )            ///������
					{
						printf( "\r\n���������� !" );
						memset( DomainNameStr, 0, sizeof( DomainNameStr ) );
						memset( SysConf_struct.DNSR, 0, sizeof( DomainNameStr ) );
						memcpy( DomainNameStr, (char*)pstrTempStart, cmdLen );
						memcpy( SysConf_struct.DNSR, (char*)pstrTempStart, cmdLen );
						Api_Config_write( config, ID_CONF_SYS, (uint8_t*)&SysConf_struct, sizeof( SysConf_struct ) );
						//----- ���� GSM ģ��------
						DataLink_DNSR_Set( SysConf_struct.DNSR, 1 );

						///
						Add_SMS_Ack_Content( sms_ack_data, ACKstate );

						//------- add on 2013-6-6
						if( ACKstate == SMS_ACK_none )
						{
							SD_ACKflag.f_CentreCMDack_0001H = 2;    //DataLink_EndFlag=1; //AT_End(); �ȷ��ؽ���ٹҶ�
						}
					}else if( pstrTemp[4] == '2' )                  ///��������
					{
						printf( "\r\n���ñ������� !" );
						memset( DomainNameStr_aux, 0, sizeof( DomainNameStr_aux ) );
						memset( SysConf_struct.DNSR_Aux, 0, sizeof( DomainNameStr_aux ) );
						memcpy( DomainNameStr_aux, (char*)pstrTempStart, cmdLen );
						memcpy( SysConf_struct.DNSR_Aux, (char*)pstrTempStart, cmdLen );
						Api_Config_write( config, ID_CONF_SYS, (uint8_t*)&SysConf_struct, sizeof( SysConf_struct ) );
						//----- ���� GSM ģ��------
						DataLink_DNSR2_Set( SysConf_struct.DNSR_Aux, 1 );

						///
						Add_SMS_Ack_Content( sms_ack_data, ACKstate );
					}else
					{
						continue;
					}
				}
			}else if( strncmp( pstrTemp, "PORT", 4 ) == 0 ) ///2. ���ö˿�
			{
				j = sscanf( sms_content, "%u", &u16Temp );
				if( j )
				{
					if( pstrTemp[4] == '1' )                ///���˿�
					{
						printf( "\r\n�������˿�=%d!", u16Temp );
						RemotePort_main				= u16Temp;
						SysConf_struct.Port_main	= u16Temp;
						Api_Config_write( config, ID_CONF_SYS, (uint8_t*)&SysConf_struct, sizeof( SysConf_struct ) );
						//----- ���� GSM ģ��------
						DataLink_MainSocket_set( RemoteIP_main, RemotePort_main, 1 );
						///
						Add_SMS_Ack_Content( sms_ack_data, ACKstate );

						//------- add on 2013-6-6
						if( ACKstate == SMS_ACK_none )
						{
							SD_ACKflag.f_CentreCMDack_0001H = 2;    //DataLink_EndFlag=1; //AT_End(); �ȷ��ؽ���ٹҶ�
						}
					}else if( pstrTemp[4] == '2' )                  ///���ö˿�
					{
						printf( "\r\n���ñ��ö˿�=%d!", u16Temp );
						RemotePort_aux			= u16Temp;
						SysConf_struct.Port_Aux = u16Temp;
						Api_Config_write( config, ID_CONF_SYS, (uint8_t*)&SysConf_struct, sizeof( SysConf_struct ) );
						//----- ���� GSM ģ��------
						DataLink_AuxSocket_set( RemoteIP_aux, RemotePort_aux, 1 );
						///
						Add_SMS_Ack_Content( sms_ack_data, ACKstate );
					}else
					{
						continue;
					}
				}
			}else if( strncmp( pstrTemp, "DUR", 3 ) == 0 ) ///3. �޸ķ��ͼ��
			{
				j = sscanf( sms_content, "%u", &u16Temp );
				if( j )
				{
					printf( "\r\n�޸ķ��ͼ��! %d", u16Temp );
					dur( sms_content );


					/*
					   JT808Conf_struct.DURATION.Default_Dur=u16Temp;
					   Api_Config_Recwrite_Large(jt808,0,(uint8_t*)&JT808Conf_struct,sizeof(JT808Conf_struct));
					 */

					///
					Add_SMS_Ack_Content( sms_ack_data, ACKstate );
				}
			}else if( strncmp( pstrTemp, "DEVICEID", 8 ) == 0 ) ///4. �޸��ն�ID
			{
				if( cmdLen <= sizeof( DeviceNumberID ) )
				{
					printf( "\r\n�޸��ն�ID  !" );
					memset( DeviceNumberID, 0, sizeof( DeviceNumberID ) );
					memcpy( DeviceNumberID, pstrTempStart, cmdLen );
					DF_WriteFlashSector( DF_DeviceID_offset, 0, DeviceNumberID, 13 );
					///
					Add_SMS_Ack_Content( sms_ack_data, ACKstate );
				}else
				{
					continue;
				}
			} //SIM_CardID_JT808
			else if( strncmp( pstrTemp, "SIMID", 5 ) == 0 ) ///4. �޸��ն�ID
			{
				if( cmdLen <= sizeof( DeviceNumberID ) )
				{
					memset( SIM_CardID_JT808, 0, sizeof( SIM_CardID_JT808 ) );
					memcpy( SIM_CardID_JT808, pstrTempStart, cmdLen );
					printf( "\r\n�޸�SIMID  !%s,%s,%d", SIM_CardID_JT808, pstrTempStart, cmdLen );
					simid( SIM_CardID_JT808 );


					/*
					   DF_WriteFlashSector(DF_SIMID_offset,0,SIM_CardID_JT808,13);
					   ///*/
					Add_SMS_Ack_Content( sms_ack_data, ACKstate );

					//------- add on 2013-6-6
					if( ACKstate == SMS_ACK_none )
					{
						SD_ACKflag.f_CentreCMDack_0001H = 2;    //DataLink_EndFlag=1; //AT_End(); �ȷ��ؽ���ٹҶ�
					}
				}else
				{
					continue;
				}
			}else if( strncmp( pstrTemp, "IP", 2 ) == 0 )       ///5.����IP��ַ
			{
				j = sscanf( sms_content, "%u.%u.%u.%u", (uint32_t*)&u8TempBuf[0], (uint32_t*)&u8TempBuf[1], (uint32_t*)&u8TempBuf[2], (uint32_t*)&u8TempBuf[3] );
				//j=str2ip(sms_content, u8TempBuf);
				if( j == 4 )
				{
					printf( "\r\n����IP��ַ!" );
					if( pstrTemp[2] == '1' )
					{
						memcpy( SysConf_struct.IP_Main, u8TempBuf, 4 );
						memcpy( RemoteIP_main, u8TempBuf, 4 );
						SysConf_struct.Port_main = RemotePort_main;
						Api_Config_write( config, ID_CONF_SYS, (uint8_t*)&SysConf_struct, sizeof( SysConf_struct ) );
						printf( "\r\n���������������� IP: %d.%d.%d.%d : %d ", RemoteIP_main[0], RemoteIP_main[1], RemoteIP_main[2], RemoteIP_main[3], RemotePort_main );
						//-----------  Below add by Nathan  ----------------------------
						DataLink_MainSocket_set( RemoteIP_main, RemotePort_main, 1 );
						///
						Add_SMS_Ack_Content( sms_ack_data, ACKstate );

						//------- add on 2013-6-6
						if( ACKstate == SMS_ACK_none )
						{
							SD_ACKflag.f_CentreCMDack_0001H = 2; //DataLink_EndFlag=1; //AT_End(); �ȷ��ؽ���ٹҶ�
						}
					}else if( pstrTemp[2] == '2' )
					{
						memcpy( SysConf_struct.IP_Aux, u8TempBuf, 4 );
						memcpy( RemoteIP_aux, u8TempBuf, 4 );
						SysConf_struct.Port_Aux = RemotePort_aux;
						Api_Config_write( config, ID_CONF_SYS, (uint8_t*)&SysConf_struct, sizeof( SysConf_struct ) );
						printf( "\r\n�������ñ��÷����� IP: %d.%d.%d.%d : %d ", RemoteIP_aux[0], RemoteIP_aux[1], RemoteIP_aux[2], RemoteIP_aux[3], RemotePort_aux );
						//-----------  Below add by Nathan  ----------------------------
						DataLink_AuxSocket_set( RemoteIP_aux, RemotePort_aux, 1 );
						///
						Add_SMS_Ack_Content( sms_ack_data, ACKstate );
					}
				}
			}else if( strncmp( pstrTemp, "MODE", 4 ) == 0 ) ///6. ���ö�λģʽ
			{
				if( strncmp( sms_content, "BD", 2 ) == 0 )
				{
					gps_mode( "1" );
				}
				if( strncmp( sms_content, "GP", 2 ) == 0 )
				{
					gps_mode( "2" );
				}
				if( strncmp( sms_content, "GN", 2 ) == 0 )
				{
					gps_mode( "3" );
				}
				Add_SMS_Ack_Content( sms_ack_data, ACKstate );
			}else if( strncmp( pstrTemp, "VIN", 3 ) == 0 )          ///7.���ó���VIN
			{
				vin_set( sms_content );
				Add_SMS_Ack_Content( sms_ack_data, ACKstate );
			}else if( strncmp( pstrTemp, "TIREDCLEAR", 10 ) == 0 )  ///8.���ƣ�ͼ�ʻ��¼
			{
				TiredDrv_write	= 0;
				TiredDrv_read	= 0;
				DF_Write_RecordAdd( TiredDrv_write, TiredDrv_read, TYPE_TiredDrvAdd );
				Add_SMS_Ack_Content( sms_ack_data, ACKstate );
			}else if( strncmp( pstrTemp, "DISCLEAR", 8 ) == 0 )     ///9������
			{
				JT808Conf_struct.DayStartDistance_32	= 0;
				JT808Conf_struct.Distance_m_u32			= 0;
				Api_Config_Recwrite_Large( jt808, 0, (uint8_t*)&JT808Conf_struct, sizeof( JT808Conf_struct ) );
				Add_SMS_Ack_Content( sms_ack_data, ACKstate );
			}else if( strncmp( pstrTemp, "RESET", 5 ) == 0 )        ///10.�ն�����
			{
				reset( );
			}else if( strncmp( pstrTemp, "RELAY", 5 ) == 0 )        ///11.�̵�������
			{
				if( sms_content[0] == '0' )
				{
					debug_relay( "0" );
				}
				if( sms_content[0] == '1' )
				{
					debug_relay( "1" );
				}

				Add_SMS_Ack_Content( sms_ack_data, ACKstate );
			}else if( strncmp( pstrTemp, "TAKE", 4 ) == 0 ) //12./����
			{
				switch( sms_content[0] )
				{
					case '1':
						takephoto( "1" );
						break;
					case '2':
						takephoto( "2" );
						break;
					case '3':
						takephoto( "3" );
						break;
					case '4':
						takephoto( "4" );
						break;
				}
				Add_SMS_Ack_Content( sms_ack_data, ACKstate );
			}else if( strncmp( pstrTemp, "PLAY", 4 ) == 0 )                                                                     ///13.��������
			{
				TTS_Get_Data( sms_content, strlen( sms_content ) );
				Add_SMS_Ack_Content( sms_ack_data, ACKstate );
			}else if( strncmp( pstrTemp, "QUERY", 5 ) == 0 )                                                                    ///14.����״̬��ѯ
			{
				Add_SMS_Ack_Content( sms_ack_data, ACKstate );
			}else if( strncmp( pstrTemp, "ISP", 3 ) == 0 )                                                                      ///15.Զ������IP �˿�
			{
				;
				Add_SMS_Ack_Content( sms_ack_data, ACKstate );
			}else if( strncmp( pstrTemp, "PLATENUM", 8 ) == 0 )
			{
				printf( "Vech_Num is %s", sms_content );
				memset( (uint8_t*)&JT808Conf_struct.Vechicle_Info.Vech_Num, 0, sizeof( JT808Conf_struct.Vechicle_Info.Vech_Num ) );  //clear
				rt_memcpy( JT808Conf_struct.Vechicle_Info.Vech_Num, sms_content, strlen( sms_content ) );
				Api_Config_Recwrite_Large( jt808, 0, (uint8_t*)&JT808Conf_struct, sizeof( JT808Conf_struct ) );
				Add_SMS_Ack_Content( sms_ack_data, ACKstate );
			}else if( strncmp( pstrTemp, "COLOR", 5 ) == 0 )
			{
				j = sscanf( sms_content, "%d", &u16Temp );
				if( j )
				{
					JT808Conf_struct.Vechicle_Info.Dev_Color = u16Temp;
					printf( "\r\n ������ɫ: %s ,%d \r\n", sms_content, JT808Conf_struct.Vechicle_Info.Dev_Color );
					Api_Config_Recwrite_Large( jt808, 0, (uint8_t*)&JT808Conf_struct, sizeof( JT808Conf_struct ) );
					Add_SMS_Ack_Content( sms_ack_data, ACKstate );
				}
			}else
			{
				;
			}
		}else
		{
			break;
		}
	}
}

#endif

/************************************** The End Of File **************************************/
