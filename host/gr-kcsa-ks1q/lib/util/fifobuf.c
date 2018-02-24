/**
 * \file
 * <!--
 * This file is part of BeRTOS.
 *
 * Bertos is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * As a special exception, you may use this file as part of a free software
 * library without restriction.  Specifically, if other files instantiate
 * templates or use macros or inline functions from this file, or you compile
 * this file and link it with other files to produce an executable, this
 * file does not by itself cause the resulting executable to be covered by
 * the GNU General Public License.  This exception does not however
 * invalidate any other reasons why the executable file might be covered by
 * the GNU General Public License.
 *
 * Copyright 2003, 2004 Develer S.r.l. (http://www.develer.com/)
 * Copyright 2001, 2008 Bernie Innocenti <bernie@codewiz.org>
 * -->
 *
 * \brief General pourpose FIFO buffer implemented with a ring buffer
 *
 * \li \c begin points to the first buffer element;
 * \li \c end points to the last buffer element (unlike the STL convention);
 * \li \c head points to the element to be extracted next;
 * \li \c tail points to the location following the last insertion;
 * \li when any of the pointers advances beyond \c end, it is reset
 *     back to \c begin.
 *
 * \code
 *
 *  +-----------------------------------+
 *  |  empty  |   valid data   |  empty |
 *  +-----------------------------------+
 *  ^         ^                ^        ^
 *  begin    head             tail     end
 *
 * \endcode
 *
 * The buffer is EMPTY when \c head and \c tail point to the same location:
 *		\code head == tail \endcode
 *
 * The buffer is FULL when \c tail points to the location immediately
 * after \c head:
 *		\code tail == head - 1 \endcode
 *
 * The buffer is also FULL when \c tail points to the last buffer
 * location and head points to the first one:
 *		\code head == begin && tail == end \endcode
 *
 * \version $Id: fifobuf.h 2506 2009-04-15 08:29:07Z duplo $
 * \author Bernie Innocenti <bernie@codewiz.org>
 */


#include "fifobuf.h"


/**
 * Check whether the fifo is empty
 *
 * \note Calling fifo_isempty() is safe while a concurrent
 *       execution context is calling fifo_push() or fifo_pop()
 *       only if the CPU can atomically update a pointer
 *       (which the AVR and other 8-bit processors can't do).
 *
 * \sa fifo_isempty_locked
 */
bool fifo_isempty(const FIFOBuffer *fb)
{
	//ASSERT_VALID_FIFO(fb);
	return fb->head == fb->tail;
}

bool fifo_isempty_q15(const FIFOBuffer_q15 *fb)
{
	//ASSERT_VALID_FIFO(fb);
	return fb->head == fb->tail;
}

/**
 * Check whether the fifo is full
 *
 * \note Calling fifo_isfull() is safe while a concurrent
 *       execution context is calling fifo_pop() and the
 *       CPU can update a pointer atomically.
 *       It is NOT safe when the other context calls
 *       fifo_push().
 *       This limitation is not usually problematic in a
 *       consumer/producer scenario because the
 *       fifo_isfull() and fifo_push() are usually called
 *       in the producer context.
 */
bool fifo_isfull(const FIFOBuffer *fb)
{
	//ASSERT_VALID_FIFO(fb);
	return
		((fb->head == fb->begin) && (fb->tail == fb->end))
		|| (fb->tail == fb->head - 1);
}

bool fifo_isfull_q15(const FIFOBuffer_q15 *fb)
{
	//ASSERT_VALID_FIFO(fb);
	return
		((fb->head == fb->begin) && (fb->tail == fb->end))
		|| (fb->tail == fb->head - 1);
}

uint32_t fifo_room(FIFOBuffer *fb)
{
	if( fb->tail < fb->head)
	{
		return (fb->head - fb->tail - 1);
	}
	else
	{
		return (fb->head - fb->tail + fb->end - fb->begin);
	}
}


/**
 * Push a character on the fifo buffer.
 *
 * \note Calling \c fifo_push() on a full buffer is undefined.
 *       The caller must make sure the buffer has at least
 *       one free slot before calling this function.
 *
 * \note It is safe to call fifo_pop() and fifo_push() from
 *       concurrent contexts, unless the CPU can't update
 *       a pointer atomically (which the AVR and other 8-bit
 *       processors can't do).
 *
 * \sa fifo_push_locked
 */
void fifo_push(FIFOBuffer *fb, unsigned char c)
{
#ifdef __MWERKS__
#pragma interrupt called
#endif
	//ASSERT_VALID_FIFO(fb);

	/* Write at tail position */
	*(fb->tail) = c;

	if (UNLIKELY(fb->tail == fb->end))
		/* wrap tail around */
		fb->tail = fb->begin;
	else
		/* Move tail forward */
		fb->tail++;
}

void fifo_push_q15(FIFOBuffer_q15 *fb, int16_t c)
{
#ifdef __MWERKS__
#pragma interrupt called
#endif
	//ASSERT_VALID_FIFO(fb);

	/* Write at tail position */
	*(fb->tail) = c;

	if (UNLIKELY(fb->tail == fb->end))
		/* wrap tail around */
		fb->tail = fb->begin;
	else
		/* Move tail forward */
		fb->tail++;
}

/**
 * Pop a character from the fifo buffer.
 *
 * \note Calling \c fifo_pop() on an empty buffer is undefined.
 *       The caller must make sure the buffer contains at least
 *       one character before calling this function.
 *
 * \note It is safe to call fifo_pop() and fifo_push() from
 *       concurrent contexts.
 */
unsigned char fifo_pop(FIFOBuffer *fb)
{
#ifdef __MWERKS__
#pragma interrupt called
#endif
	//ASSERT_VALID_FIFO(fb);

	if (UNLIKELY(fb->head == fb->end))
	{
		/* wrap head around */
		fb->head = fb->begin;
		return *(fb->end);
	}
	else
		/* move head forward */
		return *(fb->head++);
}

int16_t fifo_pop_q15(FIFOBuffer_q15 *fb)
{
#ifdef __MWERKS__
#pragma interrupt called
#endif
	//ASSERT_VALID_FIFO(fb);

	if (UNLIKELY(fb->head == fb->end))
	{
		/* wrap head around */
		fb->head = fb->begin;
		return *(fb->end);
	}
	else
		/* move head forward */
		return *(fb->head++);
}

/**
 * Make the fifo empty, discarding all its current contents.
 */
inline void fifo_flush(FIFOBuffer *fb)
{
	//ASSERT_VALID_FIFO(fb);
	fb->head = fb->tail;
}




/**
 * FIFO Initialization.
 */
void fifo_init(FIFOBuffer *fb, unsigned char *buf, uint32_t size)
{
	/* FIFO buffers have a known bug with 1-byte buffers. */
	//ASSERT(size > 1);

	fb->head = fb->tail = fb->begin = buf;
	fb->end = buf + size - 1;
}

void fifo_init_q15(FIFOBuffer_q15 *fb, int16_t *buf, uint32_t size)
{
	// FIFO buffers have a known bug with 1-byte buffers. 
//	ASSERT(size > 1);

	fb->head = fb->tail = fb->begin = buf;
	fb->end = buf + size - 1;
}

/**
 * \return Lenght of the FIFOBuffer \a fb.
 */
uint32_t fifo_len(FIFOBuffer *fb)
{
	return fb->end - fb->begin;
}


#if 0

/*
 * UNTESTED: if uncommented, to be moved in fifobuf.c
 */
void fifo_pushblock(FIFOBuffer *fb, unsigned char *block, size_t len)
{
	size_t freelen;

	/* Se c'e' spazio da tail alla fine del buffer */
	if (fb->tail >= fb->head)
	{
		freelen = fb->end - fb->tail + 1;

		/* C'e' abbastanza spazio per scrivere tutto il blocco? */
		if (freelen < len)
		{
			/* Scrivi quello che entra fino alla fine del buffer */
			memcpy(fb->tail, block, freelen);
			block += freelen;
			len -= freelen;
			fb->tail = fb->begin;
		}
		else
		{
			/* Scrivi tutto il blocco */
			memcpy(fb->tail, block, len);
			fb->tail += len;
			return;
		}
	}

	for(;;)
	{
		while (!(freelen = fb->head - fb->tail - 1))
			Delay(FIFO_POLLDELAY);

		/* C'e' abbastanza spazio per scrivere tutto il blocco? */
		if (freelen < len)
		{
			/* Scrivi quello che entra fino alla fine del buffer */
			memcpy(fb->tail, block, freelen);
			block += freelen;
			len -= freelen;
			fb->tail += freelen;
		}
		else
		{
			/* Scrivi tutto il blocco */
			memcpy(fb->tail, block, len);
			fb->tail += len;
			return;
		}
	}
}
#endif
