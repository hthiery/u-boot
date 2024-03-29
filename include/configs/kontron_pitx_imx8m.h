/* SPDX-License-Identifier: GPL-2.0+ */

#ifndef __KONTRON_PITX_IMX8M_H
#define __KONTRON_PITX_IMX8M_H

#include <linux/sizes.h>
#include <linux/stringify.h>
#include <asm/arch/imx-regs.h>

#define CONFIG_SYS_BOOTM_LEN		(32 * SZ_1M)

#define CONFIG_SPL_MAX_SIZE		(124 * SZ_1K)
#define CONFIG_SYS_MONITOR_LEN		(512 * SZ_1K)
#define CONFIG_SYS_MMCSD_RAW_MODE_U_BOOT_USE_SECTOR
#define CONFIG_SYS_MMCSD_RAW_MODE_U_BOOT_SECTOR	0x300

#ifdef CONFIG_SPL_BUILD
#define CONFIG_SPL_WATCHDOG
#define CONFIG_SPL_DRIVERS_MISC
#define CONFIG_SPL_POWER
#define CONFIG_SPL_I2C
#define CONFIG_SPL_LDSCRIPT		"arch/arm/cpu/armv8/u-boot-spl.lds"
#define CONFIG_SPL_STACK		0x187FF0
#define CONFIG_SPL_LIBCOMMON_SUPPORT
#define CONFIG_SPL_LIBGENERIC_SUPPORT
#define CONFIG_SPL_GPIO
#define CONFIG_SPL_MMC_SUPPORT
#define CONFIG_SPL_BSS_START_ADDR       0x00180000
#define CONFIG_SPL_BSS_MAX_SIZE         SZ_8K
#define CONFIG_SYS_SPL_MALLOC_START     0x42200000
#define CONFIG_SYS_SPL_MALLOC_SIZE      SZ_512K
#define CONFIG_SYS_SPL_PTE_RAM_BASE     0x41580000

/* malloc f used before GD_FLG_FULL_MALLOC_INIT set */
#define CONFIG_MALLOC_F_ADDR		0x182000
/* For RAW image gives a error info not panic */
#define CONFIG_SPL_ABORT_ON_RAW_IMAGE

#undef CONFIG_DM_MMC
#undef CONFIG_DM_PMIC
#undef CONFIG_DM_PMIC_PFUZE100

#define CONFIG_SYS_I2C_LEGACY
#define CONFIG_SYS_I2C_MXC_I2C1
#define CONFIG_SYS_I2C_MXC_I2C2
#define CONFIG_SYS_I2C_MXC_I2C3

#define CONFIG_ENV_VARS_UBOOT_RUNTIME_CONFIG

#define CONFIG_POWER
#define CONFIG_POWER_I2C
#define CONFIG_POWER_PFUZE100
#define CONFIG_POWER_PFUZE100_I2C_ADDR  0x08
#endif

#define CONFIG_REMAKE_ELF

/* ENET1 Config */
#if defined(CONFIG_CMD_NET)
#define CONFIG_MII
#define CONFIG_ETHPRIME                 "FEC"

#define CONFIG_FEC_MXC
#define CONFIG_FEC_XCV_TYPE             RGMII
#define CONFIG_FEC_MXC_PHYADDR          0
#define FEC_QUIRK_ENET_MAC

#define CONFIG_PHY_GIGE
#define IMX_FEC_BASE			0x30BE0000

#endif

#define ENV_MEM_LAYOUT_SETTINGS \
	"kernel_addr_r=0x40880000\0" \
	"fdt_addr_r=0x43000000\0" \
	"scriptaddr=0x43500000\0" \
	"initrd_addr=0x43800000\0" \
	"pxefile_addr_r=0x43500000\0" \
	"bootm_size=0x10000000\0" \

#define BOOT_TARGET_DEVICES(func) \
	func(MMC, mmc, 0) \
	func(MMC, mmc, 1) \
	func(USB, usb, 0) \
	func(DHCP, dhcp, na) \
	func(PXE, pxe, 0)

#include <config_distro_bootcmd.h>

/* Initial environment variables */
#define CONFIG_EXTRA_ENV_SETTINGS		\
	"image=Image\0" \
	"console=ttymxc2,115200\0" \
	"boot_fdt=try\0" \
	"fdtfile=freescale/imx8mq-kontron-pitx-imx8m.dtb\0" \
	"dfu_alt_info=mmc 0=flash-bin raw 0x42 0x1000 mmcpart 1\0"\
	ENV_MEM_LAYOUT_SETTINGS \
	BOOTENV

/* Link Definitions */
#define CONFIG_LOADADDR			0x40480000

#define CONFIG_SYS_LOAD_ADDR            CONFIG_LOADADDR

#define CONFIG_SYS_INIT_RAM_ADDR        0x40000000
#define CONFIG_SYS_INIT_RAM_SIZE        0x80000
#define CONFIG_SYS_INIT_SP_OFFSET \
	(CONFIG_SYS_INIT_RAM_SIZE - GENERATED_GBL_DATA_SIZE)
#define CONFIG_SYS_INIT_SP_ADDR \
	(CONFIG_SYS_INIT_RAM_ADDR + CONFIG_SYS_INIT_SP_OFFSET)

/* Size of malloc() pool */
#define CONFIG_SYS_MALLOC_LEN		((CONFIG_ENV_SIZE + (2 * 1024)) * 1024)

#define CONFIG_SYS_SDRAM_BASE           0x40000000
#define PHYS_SDRAM                      0x40000000
#define PHYS_SDRAM_SIZE			0xC0000000 /* 3GB DDR */

#define CONFIG_MXC_UART_BASE		UART3_BASE_ADDR

#define CONFIG_IMX_BOOTAUX

#define CONFIG_SYS_FSL_USDHC_NUM	2
#define CONFIG_SYS_FSL_ESDHC_ADDR       0

#define CONFIG_MXC_GPIO

/* I2C Configs */
#define CONFIG_SYS_I2C_SPEED		100000

#define CONFIG_OF_SYSTEM_SETUP

#ifndef CONFIG_SPL_BUILD
#define CONFIG_DM_PMIC
#endif

#endif
