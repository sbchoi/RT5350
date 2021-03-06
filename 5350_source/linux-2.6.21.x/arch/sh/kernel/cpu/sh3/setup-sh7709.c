/*
 * SH7707/SH7709 Setup
 *
 *  Copyright (C) 2006  Paul Mundt
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */
#include <linux/platform_device.h>
#include <linux/init.h>
#include <linux/serial.h>
#include <asm/sci.h>

static struct plat_sci_port sci_platform_data[] = {
	{
		.mapbase	= 0xfffffe80,
		.flags		= UPF_BOOT_AUTOCONF,
		.type		= PORT_SCI,
		.irqs		= { 23, 24, 25, 0 },
	}, {
		.mapbase	= 0xa4000150,
		.flags		= UPF_BOOT_AUTOCONF,
		.type		= PORT_SCIF,
		.irqs		= { 56, 57, 59, 58 },
	}, {
		.mapbase	= 0xa4000140,
		.flags		= UPF_BOOT_AUTOCONF,
		.type		= PORT_IRDA,
		.irqs		= { 52, 53, 55, 54 },
	}, {
		.flags = 0,
	}
};

static struct platform_device sci_device = {
	.name		= "sh-sci",
	.id		= -1,
	.dev		= {
		.platform_data	= sci_platform_data,
	},
};

static struct platform_device *sh7709_devices[] __initdata = {
	&sci_device,
};

static int __init sh7709_devices_setup(void)
{
	return platform_add_devices(sh7709_devices,
				    ARRAY_SIZE(sh7709_devices));
}
__initcall(sh7709_devices_setup);

#define IPRx(A,N) .addr=A, .shift=0*N*-1
#define IPRA(N)	IPRx(0xfffffee2UL,N)
#define IPRB(N)	IPRx(0xfffffee4UL,N)
#define IPRE(N)	IPRx(0xa400001aUL,N)

static struct ipr_data sh7709_ipr_map[] = {
	[16]		= { IPRA(15-12), 2 }, /* TMU TUNI0 */
	[17]		= { IPRA(11-8),  4 }, /* TMU TUNI1 */
	[22]		= { IPRA(3-0),   2 }, /* RTC CUI */
	[23 ... 26]	= { IPRB(7-4),   3 }, /* SCI */
	[27]		= { IPRB(15-12), 2 }, /* WDT ITI */
	[48 ... 51]	= { IPRE(15-12), 7 }, /* DMA */
	[52 ... 55]	= { IPRE(11-8),  3 }, /* IRDA */
	[56 ... 59]	= { IPRE(7-4),   3 }, /* SCIF */
};

void __init init_IRQ_ipr()
{
	make_ipr_irq(sh7709_ipr_map, ARRAY_SIZE(sh7709_ipr_map));
}
