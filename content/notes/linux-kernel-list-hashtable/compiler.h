/* SPDX-License-Identifier: GPL-2.0 */
#ifndef LINUX_COMPILER_H
#define LINUX_COMPILER_H

#ifndef __WORDSIZE
#define __WORDSIZE (__SIZEOF_LONG__ * 8)
#endif

#ifndef BITS_PER_LONG
#define BITS_PER_LONG __WORDSIZE
#endif

#ifndef __always_inline
#define __always_inline inline
#endif

#define WRITE_ONCE(var, val) \
	(*((volatile typeof(val) *)(&(var))) = (val))

#define READ_ONCE(var) (*((volatile typeof(var) *)(&(var))))

#endif
