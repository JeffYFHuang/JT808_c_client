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
#ifndef _H_VDR_
#define _H_VDR_

typedef struct _vdrcmd
{
	uint8_t		cmd;            /*�������ͣ�00-15h*/
	uint16_t	get_counts; 	/*��ȡ����ͳ�� ��1(��ʼ)����*/
	uint8_t		start[6];       /*��ʼʱ��*/
	uint8_t		end[6];         /*����ʱ��*/
	uint16_t	blocks;         /*���λ���ݿ����*/
}VDRCMD;

#endif
/************************************** The End Of File **************************************/
