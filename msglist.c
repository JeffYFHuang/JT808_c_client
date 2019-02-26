/*
��Ϣ��ʽ�������շ���ͳһ
 */
#include <stdlib.h>
#include "Err.h"
#include "msglist.h"


MsgListNode* msglist_node_create(void* data)
{
	MsgListNode* node = malloc(sizeof(MsgListNode));

	if(node != RT_NULL)
	{
		node->prev = RT_NULL;
		node->next = RT_NULL;
		node->data = data;
	}

	return node;
}

void msglist_node_destroy(MsgListNode* node)
{
	if(node != RT_NULL)
	{
		node->next = RT_NULL;
		node->prev = RT_NULL;
		free(node);
	}

	return;
}

MsgList* msglist_create(pthread_mutex_t *lock)
{
	pthread_mutex_lock(lock);
	MsgList* thiz = malloc(sizeof(MsgList));

	if(thiz != RT_NULL)
	{
		thiz->lock = lock;
		thiz->first = RT_NULL;
	}
	pthread_mutex_unlock(lock);
	return thiz;
}

MsgListRet msglist_prepend(MsgList* thiz, void* data)
{
	pthread_mutex_lock(thiz->lock);
	MsgListNode* node = RT_NULL;
	MsgListNode* iter=RT_NULL;

	if((node = msglist_node_create(data)) == RT_NULL)
	{
		return MSGLIST_RET_OOM; 
	}

	if(thiz->first == RT_NULL)
	{
		thiz->first = node;
		return MSGLIST_RET_OK;
	}
	
	iter= thiz->first;
	while(iter->prev != RT_NULL)
	{
		iter = iter->prev;
	}
	iter->prev=node;
	node->next=iter;
	thiz->first=node;
	pthread_mutex_unlock(thiz->lock);
	return MSGLIST_RET_OK;

}


MsgListRet msglist_append(MsgList* thiz, void* data)
{
	//printf("msglist_append 0 lock %p\r\n", thiz->lock);
	pthread_mutex_lock(thiz->lock);
	MsgListNode* node = RT_NULL;
	MsgListNode* iter=RT_NULL;

	if((node = msglist_node_create(data)) == RT_NULL)
	{
		pthread_mutex_unlock(thiz->lock);
		printf("msglist_append unlock %p\r\n", thiz->lock);
		return MSGLIST_RET_OOM; 
	}

	if(thiz->first == RT_NULL)
	{
		thiz->first = node;
		pthread_mutex_unlock(thiz->lock);
		printf("msglist_append unlock %p\r\n", thiz->lock);
		return MSGLIST_RET_OK;
	}
	
	iter= thiz->first;
	while(iter->next != RT_NULL)
	{
		iter = iter->next;
	}
	iter->next=node;
	node->prev=iter;
	pthread_mutex_unlock(thiz->lock);
	//printf("msglist_append unlock %p\r\n", thiz->lock);
	return MSGLIST_RET_OK;
}


size_t msglist_length(MsgList* thiz)
{
	size_t length = 0;
	MsgListNode* iter = thiz->first;

	while(iter != RT_NULL)
	{
		length++;
		iter = iter->next;
	}

	return length;
}

MsgListRet msglist_foreach(MsgList* thiz, MsgListDataVisitFunc visit, void* ctx)
{
	MsgListRet ret = MSGLIST_RET_OK;
	MsgListNode* iter = thiz->first;

	while(iter != RT_NULL && ret != MSGLIST_RET_STOP)
	{
		ret = visit(ctx, iter);
		iter = iter->next;
	}

	return ret;
}

int msglist_find(MsgList* thiz, MsgListDataCompareFunc cmp, void* ctx)
{
	int i = 0;
	MsgListNode* iter = thiz->first;

	while(iter != RT_NULL)
	{
		if(cmp(ctx, iter) == 0)
		{
			break;
		}
		i++;
		iter = iter->next;
	}

	return i;
}

void msglist_destroy(MsgList* thiz)
{
	MsgListNode* iter = thiz->first;
	MsgListNode* next = RT_NULL;

	while(iter != RT_NULL)
	{
		next = iter->next;
		msglist_node_destroy(iter);
		iter = next;
	}

	thiz->first = RT_NULL;
	free(thiz);

	return;
}


