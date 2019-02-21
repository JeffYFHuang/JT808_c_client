/*
	Copyright 2001, 2002 Georges Menie (www.menie.org)
	stdarg version contributed by Christian Ettinger

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/*
	putchar is the only external dependency for this file,
	if you have a working putchar, leave it commented out.
	If not, uncomment the define below and
	replace outbyte(c) by your own function call.

#define putchar(c) outbyte(c)
*/

#include <stdarg.h>


static unsigned char calc_fcs;

#if 0

static int printchar(char **str, int c)
{
	extern int putchar(int c);
	
	if (str) {
		calc_fcs^=c;
		if((c==0x7d)||(c==0x7e))
		{
			**str = 0x7d;
			++(*str);
			**str = (c-0x7c);
			++(*str);
			return 2;
		}
		else
		{
			**str = c;
			++(*str);
			return 1;
		}	
	}
	else (void)putchar(c);
	return 1;
}

#define PAD_RIGHT 1
#define PAD_ZERO 2

static int prints(char **out, const char *string, int width, int pad)
{
	register int pc = 0, padchar = ' ';

	if (width > 0) {
		register int len = 0;
		register const char *ptr;
		for (ptr = string; *ptr; ++ptr) ++len;
		if (len >= width) width = 0;
		else width -= len;
		if (pad & PAD_ZERO) padchar = '0';
	}
	if (!(pad & PAD_RIGHT)) {
		for ( ; width > 0; --width) {
			pc+=printchar (out, padchar);
			//++pc;
		}
	}
	for ( ; *string ; ++string) {
		pc+=printchar (out, *string);
		//++pc;
	}
	for ( ; width > 0; --width) {
		pc+=printchar (out, padchar);
		//++pc;
	}

	return pc;
}



/* the following should be enough for 32 bit int */
#define PRINT_BUF_LEN 12

static int printi(char **out, int i, int b, int sg, int width, int pad, int letbase)
{
	char print_buf[PRINT_BUF_LEN];
	register char *s;
	register int t, neg = 0, pc = 0;
	register unsigned int u = i;

	if (i == 0) {
		print_buf[0] = '0';
		print_buf[1] = '\0';
		return prints (out, print_buf, width, pad);
	}

	if (sg && b == 10 && i < 0) {
		neg = 1;
		u = -i;
	}

	s = print_buf + PRINT_BUF_LEN-1;
	*s = '\0';

	while (u) {
		t = u % b;
		if( t >= 10 )
			t += letbase - '0' - 10;
		*--s = t + '0';
		u /= b;
	}

	if (neg) {
		if( width && (pad & PAD_ZERO) ) {
			pc+=printchar (out, '-');
			//++pc;
			--width;
		}
		else {
			*--s = '-';
		}
	}

	return pc + prints (out, s, width, pad);
}


static int print(char **out, const char *format, va_list args )
{
	register int width, pad;
	register int pc = 0;
	char scr[2];

	for (; *format != 0; ++format) {
		if (*format == '%') {
			++format;
			width = pad = 0;
			if (*format == '\0') break;
			if (*format == '%') goto out;
			if (*format == '-') {
				++format;
				pad = PAD_RIGHT;
			}
			while (*format == '0') {
				++format;
				pad |= PAD_ZERO;
			}
			for ( ; *format >= '0' && *format <= '9'; ++format) {
				width *= 10;
				width += *format - '0';
			}
			if( *format == 's' ) {
				register char *s = (char *)va_arg( args, int );
				pc += prints (out, s?s:"(null)", width, pad);
				continue;
			}
			if( *format == 'd' ) {
				pc += printi (out, va_arg( args, int ), 10, 1, width, pad, 'a');
				continue;
			}
			if( *format == 'x' ) {
				pc += printi (out, va_arg( args, int ), 16, 0, width, pad, 'a');
				continue;
			}
			if( *format == 'X' ) {
				pc += printi (out, va_arg( args, int ), 16, 0, width, pad, 'A');
				continue;
			}
			if( *format == 'u' ) {
				pc += printi (out, va_arg( args, int ), 10, 0, width, pad, 'a');
				continue;
			}
			if( *format == 'c' ) {
				/* char are converted to int then pushed on the stack */
				scr[0] = (char)va_arg( args, int );
				scr[1] = '\0';
				pc += prints (out, scr, width, pad);
				continue;
			}
		}
		else {
		out:
			pc+=printchar (out, *format);
			//++pc;
		}
	}
	if (out) 
	{
		**out = '\0';
	}	
	va_end( args );
	return pc;
}

#endif

static int packchar(char **str, int c)
{
	if (str) {
		calc_fcs^=c;
		if((c==0x7d)||(c==0x7e))
		{
			**str = 0x7d;
			++(*str);
			**str = (c-0x7c);
			++(*str);
			return 2;
		}
		else
		{
			**str = c;
			++(*str);
			return 1;
		}	
	}
	return 0;
}

/*
��װ�ַ���������
���string ���Ȳ���width ��ΰ�? ����0���� 0x20(�ո�)
*/
static int pack_buf(char **out, const char *string, int width)
{
	int i;
	int count=0;
	char *p=(char*)string;
	if(width)
	{
		for(i=0;i<width;i++)
		{
			count+=packchar(out,*(string+i));
		}
	}
	else
	{
		while(*p!=0)
		{
			count+=packchar(out,*p++);
		}
	}

	return count;
}

/*
��װ����
i:Ҫ���������
width:�����ȣ��ֽ���
*/
static int pack_int(char **out, int i, int width)
{
	int count=0;
	switch(width)
	{
		case 1:
			count+=packchar(out,i&0xff);
			break;
		case 2:
			count+=packchar(out,i>>8);
			count+=packchar(out,i&0xff);
			break;
		case 4:
			count+=packchar(out,i>>24);
			count+=packchar(out,i>>16);
			count+=packchar(out,i>>8);
			count+=packchar(out,i&0xff);
			break;
	}
	return count;		
}

/*û����ת���ַ��Ĵ���*/

static int pack(char **out, const char *format, va_list args )
{
	register int width;
	register int pc = 0;

	printf("\r\npack format>%s\r\n",format);

	for (; *format != 0; ++format) {
		if (*format == '%') {
			++format;
			width = 0;
			if (*format == '\0') break;
			if (*format == '%') goto out;

			for ( ; *format >= '0' && *format <= '9'; ++format) {
				width *= 10;
				width += *format - '0';
			}
			if( *format == 's' ) {
				register char *s = (char *)va_arg( args, int );
				pc += pack_buf (out, s, width);
				continue;
			}
			if( *format == 'b' ) {
				pc += pack_int (out, va_arg( args, int ), 1);
				continue;
			}
			if( *format == 'd' ) {
				pc += pack_int (out, va_arg( args, int ), 4);
				continue;
			}
			if( *format == 'w' ) {
				pc += pack_int (out, va_arg( args, int ), 2);
				continue;
			}			
		}
		else {
		out:
			pc+=packchar (out, *format);
		}
	}
	if (out) 
	{
		**out = '\0';
	}	
	va_end( args );
	return pc;
}


/*
��װһ������Ϣ,����ת��
<7e><msg><calc_fcs><7e>
*/
int jt808_pack(char *out, const char *format, ...)
{
        va_list args;
		char *out_1;
		int len;
		calc_fcs=0;
		*out=0x7e;
		out_1=out+1;
        va_start( args, format );
        len=pack( &out_1, format, args );
		*(out+len+1)=calc_fcs;
		*(out+len+2)=0x7e;
		return len+3;
}


/*
��װ��Ϣ������ת�� <msg>
���fcs
*/
int jt808_pack_info(char *fcs,char *out, const char *format, ...)
{
	va_list args;
	int len;	
	calc_fcs=*fcs;
	va_start( args, format );
    len= pack( &out, format, args );
	*fcs=calc_fcs;
	return len;

}



#ifdef TEST_PRINTF
int main(void)
{
	char *ptr = "Hello world!";
	char *np = 0;
	int i = 5;
	unsigned int bs = sizeof(int)*8;
	int mi;
	char buf[80];

	mi = (1 << (bs-1)) + 1;
	printf("%s\n", ptr);
	printf("printf test\n");
	printf("%s is null pointer\n", np);
	printf("%d = 5\n", i);
	printf("%d = - max int\n", mi);
	printf("char %c = 'a'\n", 'a');
	printf("hex %x = ff\n", 0xff);
	printf("hex %02x = 00\n", 0);
	printf("signed %d = unsigned %u = hex %x\n", -3, -3, -3);
	printf("%d %s(s)%", 0, "message");
	printf("\n");
	printf("%d %s(s) with %%\n", 0, "message");
	sprintf(buf, "justif: \"%-10s\"\n", "left"); printf("%s", buf);
	sprintf(buf, "justif: \"%10s\"\n", "right"); printf("%s", buf);
	sprintf(buf, " 3: %04d zero padded\n", 3); printf("%s", buf);
	sprintf(buf, " 3: %-4d left justif.\n", 3); printf("%s", buf);
	sprintf(buf, " 3: %4d right justif.\n", 3); printf("%s", buf);
	sprintf(buf, "-3: %04d zero padded\n", -3); printf("%s", buf);
	sprintf(buf, "-3: %-4d left justif.\n", -3); printf("%s", buf);
	sprintf(buf, "-3: %4d right justif.\n", -3); printf("%s", buf);

	return 0;
}

/*
 * if you compile this file with
 *   gcc -Wall $(YOUR_C_OPTIONS) -DTEST_PRINTF -c printf.c
 * you will get a normal warning:
 *   printf.c:214: warning: spurious trailing `%' in format
 * this line is testing an invalid % at the end of the format string.
 *
 * this should display (on 32bit int machine) :
 *
 * Hello world!
 * printf test
 * (null) is null pointer
 * 5 = 5
 * -2147483647 = - max int
 * char a = 'a'
 * hex ff = ff
 * hex 00 = 00
 * signed -3 = unsigned 4294967293 = hex fffffffd
 * 0 message(s)
 * 0 message(s) with %
 * justif: "left      "
 * justif: "     right"
 *  3: 0003 zero padded
 *  3: 3    left justif.
 *  3:    3 right justif.
 * -3: -003 zero padded
 * -3: -3   left justif.
 * -3:   -3 right justif.
 */

#endif
