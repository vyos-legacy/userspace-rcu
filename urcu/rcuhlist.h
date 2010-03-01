/* Copyright (C) 2002 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Ulrich Drepper <drepper@redhat.com>, 2002.

   Copyright (C) 2009 Pierre-Marc Fournier
   Conversion to RCU list.
   Copyright (C) 2010 Mathieu Desnoyers

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

#ifndef _URCU_RCUHLIST_H
#define _URCU_RCUHLIST_H

#include <urcu/hlist.h>
#include <urcu/arch.h>
#include <urcu-pointer.h>

/* Add new element at the head of the list.
 */
static inline void hlist_add_head_rcu(struct hlist_node *newp,
				      struct hlist_head *head)
{
	newp->next = head->next;
	newp->prev = (struct hlist_node *)head;
	smp_wmb();
	if (head->next)
		head->next->prev = newp;
	head->next = newp;
}

/* Remove element from list. */
static inline void hlist_del_rcu(struct hlist_node *elem)
{
	if (elem->next)
		elem->next->prev = elem->prev;
	elem->prev->next = elem->next;
}


/* Iterate through elements of the list.
 * This must be done while rcu_read_lock() is held.
 */

#define hlist_for_each_entry_rcu(entry, pos, head, member)		\
	for (pos = rcu_dereference((head)->next),			\
		     entry = hlist_entry(pos, typeof(*entry), member);	\
	     pos != NULL;						\
	     pos = rcu_dereference(pos->next),				\
		     entry = hlist_entry(pos, typeof(*entry), member))

#endif	/* _URCU_RCUHLIST_H */