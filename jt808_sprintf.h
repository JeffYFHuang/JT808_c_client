#ifndef _H_JT808_SPRINTF_
#define _H_JT808_SPRINTF_


/*
对jt808发送进行封装，
采用变参的方式
增加转义和异或和的校验
*/
//int jt808_sprintf(char *out, const char *format, ...);
int jt808_pack(char *out, const char *format, ...);

#endif

