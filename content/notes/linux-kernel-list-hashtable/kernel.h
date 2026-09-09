/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_KERNEL_H
#define _LINUX_KERNEL_H

#include "compiler.h"

/**
 *  * ARRAY_SIZE - get the number of elements in array @arr
 *   * @arr: array to be sized
 *    */
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

/**
 * container_of - cast a member of a structure out to the containing structure
 * @ptr:    the pointer to the member.
 * @type:   the type of the container struct this is embedded in.
 * @member: the name of the member within the struct.
 *
 */
#define container_of(ptr, type, member) ({          \
    const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
(type *)( (char *)__mptr - offsetof(type,member) );})

#endif
