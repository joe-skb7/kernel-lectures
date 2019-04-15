/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _UAPI_LINUX_HW3_H
#define _UAPI_LINUX_HW3_H

#include <linux/ioctl.h>
#include <linux/types.h>

/* Chosen to be unique w.r.t. Documentation/ioctl/ioctl-number */
#define HW3_IOCTL_MAGIC		0x91

#define HW3IOC_SETLED		_IOWR(HW3_IOCTL_MAGIC, 0, int)
#define HW3IOC_GETLED		_IOR(HW3_IOCTL_MAGIC, 1, int)
#define HW3IOC_KERN_CONTROL	_IOW(HW3_IOCTL_MAGIC, 2, int)

#endif /* _UAPI_LINUX_HW3_H */
