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



/*
xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx

bit16 车辆信息变化



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
		u8	DriverCard_ID[19];      // 驾驶员驾驶证号码 18位
		u8	DriveName[22];          // 驾驶员 姓名21
		u8	Driver_ID[21];          // 驾驶员身份证号码20
		u8	Drv_CareerID[41];       // 驾驶员从业资格证40
		u8	Comfirm_agentID[41];    // 发证机构名称 40
	} DRV_INFO;

	struct _VechInfo
	{
		u8	Vech_VIN[18];           // 车辆VIN号17
		u8	Vech_Num[13];           // 车牌号12
		u8	Vech_Type[13];          // 车辆类型 12
		u16 Dev_ProvinceID;         // 车辆所在省ID
		u16 Dev_CityID;             // 车辆所在市ID
		u8	Dev_Color;              // 车辆颜色
	} VechINFO;

	struct  _SYSConfig              //  name:  config
	{
		u16 Version_ID;             //  系统版本IDE
		u8	APN_str[40];            //   接入点名称
		u8	IP_Main[4];             //   主IP 地址
		u16 Port_main;              //   主socket  端口
		u8	IP_Aux[4];              //   辅IP 地址
		u16 Port_Aux;               //   辅socket  端口
		u8	DNSR[50];               //  DNSR  域名解析
		u16 AccOn_Dur;              //  ACC 开  上报间隔
		u16 AccOff_Dur;             //   ACC 关  上报间隔
		u8	TriggerSDsatus;         //  传感器触发上报状态
	} SYS_CONF;


	char *中心提问信息地址;
} global_info;

/************************************** The End Of File **************************************/
