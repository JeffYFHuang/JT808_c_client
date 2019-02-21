/************************************************************
 * Copyright (C), 2008-2012,
 * FileName:		// 文件名
 * Author:			// 作者
 * Date:			// 日期
 * Description:		// 模块描述
 * Version:			// 版本信息
 * Function List:	// 主要函数及其功能
 *     1. -------
 * History:			// 历史修改记录
 *     <author>  <time>   <version >   <desc>
 *     David    96/10/12     1.0     build this moudle
 ***********************************************************/
#ifndef _H_VDR_
#define _H_VDR_

typedef struct _vdrcmd
{
	uint8_t		cmd;            /*命令类型，00-15h*/
	uint16_t	get_counts; 	/*获取数据统计 从1(开始)计数*/
	uint8_t		start[6];       /*开始时间*/
	uint8_t		end[6];         /*结束时间*/
	uint16_t	blocks;         /*最大单位数据块个数*/
}VDRCMD;

#endif
/************************************** The End Of File **************************************/
