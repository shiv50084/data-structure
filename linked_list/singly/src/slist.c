#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "debug.h"
#include "slist.h"

#define SLIST_DBG(message, ...)	\
 		MSG("%s" message "%s", YELLOW, ##__VA_ARGS__, RESET)
 		
#define SLIST_PRT_DBG(message, ...)	\
			MSG("%s" message "%s", CYAN, ##__VA_ARGS__, RESET)

/* list node definition */
struct slist_node
{
	SLIST_CUSTOM_DATA custom_data;
	struct slist_node *next;
};

/* initialize */
bool slist_init (SLIST_NODE_H *handle)
{
	SLIST_DBG("BEGIN\n");

	/* head pointer initialize */
	handle->head = (struct slist_node *) malloc (sizeof(struct slist_node));
	if (handle->head == NULL)
	{
		SLIST_DBG("Please checking your memory is enough\n");
		return false;
	}

	/* init custom data structure (not use in head pointer) */
	memset(&handle->head->custom_data, '\0', sizeof(SLIST_CUSTOM_DATA));

	/* init next pointer for head */
	handle->head->next = NULL;	

	/* pointer current to head */
	handle->current = handle->head;

	/* set the number of the node to zero */
	handle->count = 0;

	SLIST_DBG("END\n");
	return true;	
}

/* destroy */
bool slist_destroy (SLIST_NODE_H *handle)
{
	SLIST_DBG("BEGIN\n");
	struct slist_node *tmp;
	
	while (handle->head != NULL)
	{
		tmp = handle->head;
		handle->head = handle->head->next;
		free(tmp);
	}
	
	handle->head = handle->current = NULL;
	
	SLIST_DBG("END\n");
	return true;
}

bool slist_insert_node_front (SLIST_NODE_H *handle, PSLIST_CUSTOM_DATA data)
{
	SLIST_DBG("BEGIN\n");
	struct slist_node *tmp;

	/* allocate memory */
	tmp = (struct slist_node *) malloc (sizeof(struct slist_node));
	if (tmp == NULL)
	{
		SLIST_DBG("Please checking your memory is enough\n");
		return false;
	}
	
	/* this node is first node */
	if (handle->current == handle->head)
	{
		SLIST_DBG("Condition 1\n");
		tmp->next = NULL;
	}
	else
	{	
		SLIST_DBG("Condition 2\n");
		tmp->next = handle->head->next;
	}
	
	handle->current = tmp;
	handle->head->next = tmp;

	/* copy user data */
	memcpy(&tmp->custom_data, data, sizeof(SLIST_CUSTOM_DATA));

	/* increase the number of node */
	handle->count++;
	
	SLIST_DBG("END\n");
	return true;
}

bool slist_insert_node_tail (SLIST_NODE_H *handle, PSLIST_CUSTOM_DATA data)
{	
	SLIST_DBG("BEGIN\n");

	struct slist_node *tmp;

	/* allocate memory */
	tmp = (struct slist_node *) malloc (sizeof(struct slist_node));
	if (tmp == NULL)
	{
		SLIST_DBG("Please checking your memory is enough\n");
		return false;
	}
	
	/* this node is first node */
	if (handle->current == handle->head)
	{
		SLIST_DBG("Condition 1\n");
		handle->head->next = tmp;
	}
	else
	{	
		SLIST_DBG("Condition 2\n");

		/* find the last node in current list */
		struct slist_node *tail = handle->head->next;
		while (tail->next != NULL)
		{
			tail = tail->next;
		}
		
		tail->next = tmp;	
	}
	
	tmp->next = NULL;
	handle->current = tmp;

	/* copy user data */
	memcpy(&tmp->custom_data, data, sizeof(SLIST_CUSTOM_DATA));

	/* increase the number of node */
	handle->count++;
	
	SLIST_DBG("END\n");
	return true;
}

bool slist_insert_node_index (SLIST_NODE_H *handle, PSLIST_CUSTOM_DATA data, int idx)
{
	SLIST_DBG("BEGIN\n");

	/* check valid range of the index */
	if (idx < SLIST_MINIMAL_NODE || idx > SLIST_MAXIMAL_NODE || idx > handle->count+1)
	{
		SLIST_DBG("Please checking your index not out of the range\n");
		return false;
	}

	if (idx == 1)									/* insert front */
	{
		slist_insert_node_front(handle, data);
	}
	else if (idx == handle->count + 1)				/* insert tail */
	{
		slist_insert_node_tail(handle, data);
	}
	else											/* insert middle */
	{
		unsigned int l_idx = 1;
		struct slist_node *tmp, *next;
		struct slist_node *cur = handle->head->next;

		/* allocate memory */
		tmp = (struct slist_node *) malloc (sizeof(struct slist_node));
		if (tmp == NULL)
		{
			SLIST_DBG("Please checking your memory is enough\n");
			return false;
		}

		/* find the node by index */
		while (cur != NULL && idx-1 != l_idx)
		{
			cur = cur->next;
			l_idx++;
		}
		
		next = cur->next;
		cur->next = tmp;
		tmp->next = next;
		
		/* copy user data */
		memcpy(&tmp->custom_data, data, sizeof(SLIST_CUSTOM_DATA));

		/* increase the number of node */
		handle->count++;
	}
	
	SLIST_DBG("END\n");
	return true;
}

void slist_print (SLIST_NODE_H *handle)
{
	SLIST_DBG("BEGIN\n");

	short cnt = 1;
	struct slist_node *tmp;
	
	/* current slist is null */
	if (handle->head == handle->current)
	{
		SLIST_PRT_DBG("do nothing\n");
		return;
	}

	tmp = handle->head->next;
	do
	{
		SLIST_PRT_DBG("[%d] Name: %s,\tAge: %d\n", cnt++, tmp->custom_data.name, tmp->custom_data.age);
		tmp = tmp->next;
	} while (tmp != NULL);

	SLIST_PRT_DBG("----------------------------------\n");	
	SLIST_DBG("END\n");
	return;
}
