/*
 
 */

#ifndef MSGLIST_H
#define MSGLIST_H

#ifdef __cplusplus
extern "C" {
#endif/*__cplusplus*/

#include <stddef.h>
#include <pthread.h>

typedef struct _MsgListNode
{
	struct _MsgListNode* prev;
	struct _MsgListNode* next;
	void* data;
}MsgListNode;

typedef struct _MsgList
{
	pthread_mutex_t *lock;
	MsgListNode* first;
}MsgList;

typedef enum _MsgListRet
{
	MSGLIST_RET_OK,
	MSGLIST_RET_OOM,
	MSGLIST_RET_STOP,
	MSGLIST_RET_PARAMS,
	MSGLIST_RET_FAIL,
	MSGLIST_RET_DELETE_NODE,
}MsgListRet;



typedef int (*MsgListDataCompareFunc)(void* ctx, void* data);
typedef MsgListRet (*MsgListDataVisitFunc)(void* ctx, void* data);



MsgList* msglist_create(pthread_mutex_t *lock);

MsgListNode* msglist_node_create(void* data);

MsgListRet msglist_prepend(MsgList* thiz, void* data);
MsgListRet msglist_append(MsgList* thiz, void* data);
size_t   msglist_length(MsgList* thiz);
int      msglist_find(MsgList* thiz, MsgListDataCompareFunc cmp, void* ctx);
MsgListRet msglist_foreach(MsgList* thiz, MsgListDataVisitFunc visit, void* ctx);
void msglist_node_destroy(MsgListNode* node);

void msglist_destroy(MsgList* thiz);

#ifdef __cplusplus
}
#endif/*__cplusplus*/

#endif/*DLIST*/

