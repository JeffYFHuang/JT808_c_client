/**
 * @author: ideawu
 * @url: http://www.ideawu.net
 * @description: C ����ʹ�������ļ���.
 */
#ifndef _TREE_CONFIG_H
#define _TREE_CONFIG_H

#include <rtthread.h>

/*
�﷨����:
	�հ��ַ�Ϊ '\t \r\n'(�Ʊ��, �ո�, �س�, ����)
	����ֻ�����հ��ַ�����
	��Ч���� '\t*' ��ͷ
	ע������ '\t*#' ��ͷ
	key �������κοհ��ַ�, ���˵Ŀհ��ַ�������
	value ���˵Ŀհ��ַ�������
	key �� value ֮������õȺ�'='����ð��':'�ָ�
	����������а�����ϵ, ��һ�� TAB ������ʾ���ӹ�ϵ

���ö�ȡ:
	�ü�����ȡ��������
	��б��'/'���߾��'.'�ָ���������·����ȡ������
	���������ֵ��Ϊ����(int)����
	���������ֵ��Ϊ�ַ���(char *)����
*/

#define CONFIG_MAX_LINE		1024

#define IS_COMMENT_CONFIG(c)	((c)->key[0] == '#')

#define log_debug(fmt, args...)	printf" ("[LOG] " __FILE__ "(%d): " fmt "\n", __LINE__, ##args)
#define log_error log_debug


/* key �� value ֮������õȺ�'='����ð��':'�ָ�. */
struct tree_config{
	int size;
	int count;
	char *key; /* if(key == "#") ����ע�� */
	char *val;
	struct tree_config *parent;
	struct tree_config *items;
};

struct tree_config *cfg_load_file(char *filename);

void cfg_print(struct tree_config *cfg, FILE *fp);

void cfg_free(struct tree_config *cfg);

struct tree_config *cfg_get(struct tree_config *cfg, char *key);

int cfg_num(struct tree_config *cfg);
char *cfg_str(struct tree_config *cfg);

int cfg_getnum(struct tree_config *cfg, char *key);
char *cfg_getstr(struct tree_config *cfg, char *key);

#endif

/*
�����ļ�ʾ��:

# this is a comment

author : ideawu
	url: http://www.ideawu.net

proxy :
	php =
		host = 127.0.0.1
		port = 8088
	py :
		host = 127.0.0.1
		port = 8080

cgi =
	pl = /usr/bin/perl

Ӧ�ó���ʾ��:

#include <stdio.h>
#include "tree_config.h"

int main(int argc, char **argv){
	struct tree_config *cfg, *c;

	cfg = cfg_load_file("cfg_test.conf");
	if(!cfg){
		return 0;
	}

	printf("\n");
	printf("proxy.php.host = %s\n", cfg_getstr(cfg, "proxy.php.host"));
	printf("proxy.php.port = %d\n", cfg_getnum(cfg, "proxy.php.port"));
	printf("cgi.pl = %s\n", cfg_getstr(cfg, "cgi.pl"));
	printf("\n");

	c = cfg_get(cfg, "author");
	printf("author: %s\n", cfg_str(c));
	printf("url: %s\n", cfg_getstr(c, "url"));
	printf("\n");

	cfg_free(cfg);
	return 0;
}

*/
