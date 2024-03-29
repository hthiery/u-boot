// SPDX-License-Identifier: GPL-2.0+

#include <common.h>
#include <init.h>
#include <miiphy.h>
#include <asm/arch/clock.h>
#include <asm/arch/imx8mq_pins.h>
#include <asm/arch/sys_proto.h>
#include <asm-generic/gpio.h>
#include <asm/mach-imx/gpio.h>
#include <asm/mach-imx/iomux-v3.h>
#include <linux/delay.h>


DECLARE_GLOBAL_DATA_PTR;

#define UART_PAD_CTRL	(PAD_CTL_DSE6 | PAD_CTL_FSEL1)
#define WDOG_PAD_CTRL	(PAD_CTL_DSE6 | PAD_CTL_HYS | PAD_CTL_PUE)

static iomux_v3_cfg_t const wdog_pads[] = {
	IMX8MQ_PAD_GPIO1_IO02__WDOG1_WDOG_B | MUX_PAD_CTRL(WDOG_PAD_CTRL),
};

static iomux_v3_cfg_t const uart_pads[] = {
	IMX8MQ_PAD_UART3_RXD__UART3_RX | MUX_PAD_CTRL(UART_PAD_CTRL),
	IMX8MQ_PAD_UART3_TXD__UART3_TX | MUX_PAD_CTRL(UART_PAD_CTRL),
	IMX8MQ_PAD_ECSPI1_SS0__UART3_RTS_B | MUX_PAD_CTRL(UART_PAD_CTRL),
	IMX8MQ_PAD_ECSPI1_MISO__UART3_CTS_B | MUX_PAD_CTRL(UART_PAD_CTRL),
};

int board_early_init_f(void)
{
	struct wdog_regs *wdog = (struct wdog_regs *)WDOG1_BASE_ADDR;

	imx_iomux_v3_setup_multiple_pads(wdog_pads, ARRAY_SIZE(wdog_pads));
	set_wdog_reset(wdog);

	imx_iomux_v3_setup_multiple_pads(uart_pads, ARRAY_SIZE(uart_pads));

	return 0;
}

int board_phys_sdram_size(phys_size_t *memsize)
{
	/*
	 * Get real memory size by testing available memory in a
	 * given range starting at i.MX8M DRAM1 base address 0x40000000.
	*/
	*memsize = get_ram_size((long *)0x40000000, 0x100000000ULL);
	debug("Memsize: %d MiB\n", (int)(*memsize >> 20));

	return 0;
}


#ifdef CONFIG_FEC_MXC
#define FEC_RST_PAD IMX_GPIO_NR(1, 11)
static iomux_v3_cfg_t const fec1_rst_pads[] = {
	IMX8MQ_PAD_GPIO1_IO11__GPIO1_IO11 | MUX_PAD_CTRL(NO_PAD_CTRL),
};

static void setup_iomux_fec(void)
{
	imx_iomux_v3_setup_multiple_pads(fec1_rst_pads,
					 ARRAY_SIZE(fec1_rst_pads));
}

static int setup_fec(void)
{
	struct iomuxc_gpr_base_regs *gpr =
		(struct iomuxc_gpr_base_regs *)IOMUXC_GPR_BASE_ADDR;

	setup_iomux_fec();

	/* Use 125M anatop REF_CLK1 for ENET1, not from external */
	clrsetbits_le32(&gpr->gpr[1], BIT(13) | BIT(17), 0);
	return set_clk_enet(ENET_125MHZ);
}

int board_phy_config(struct phy_device *phydev)
{
	unsigned int val;

	/* enable rgmii rxc skew and phy mode select to RGMII copper */
	phy_write(phydev, MDIO_DEVAD_NONE, 0x1d, 0x1f);
	phy_write(phydev, MDIO_DEVAD_NONE, 0x1e, 0x8);

	phy_write(phydev, MDIO_DEVAD_NONE, 0x1d, 0x05);
	phy_write(phydev, MDIO_DEVAD_NONE, 0x1e, 0x100);

	/*
	 * Set LED configuration register 1:
	 * LED2_SEL: 0b1011 (link established, blink on activity)
	 */
	val = phy_read(phydev, MDIO_DEVAD_NONE, 0x18);
	val &= 0xf0ff;
	phy_write(phydev, MDIO_DEVAD_NONE, 0x18, val | (0xb << 8));

	if (phydev->drv->config)
		phydev->drv->config(phydev);
	return 0;
}
#endif

int board_init(void)
{
#ifdef CONFIG_FEC_MXC
	setup_fec();
#endif

#if defined(CONFIG_USB_DWC3) || defined(CONFIG_USB_XHCI_DWC3)
	init_usb_clk();
#endif

	return 0;
}

#ifdef CONFIG_MISC_INIT_R
#define TPM_RESET    IMX_GPIO_NR(3, 2)
#define USBHUB_RESET IMX_GPIO_NR(3, 4)

static void reset_device_by_gpio(const char *label, int pin, int delay_ms)
{
	gpio_request(pin, label);
	gpio_direction_output(pin, 0);
	mdelay(delay_ms);
	gpio_direction_output(pin, 1);
}

int misc_init_r(void)
{
	/*
	 * reset TPM chip (Infineon SLB9670) as required by datasheet
	 * (60ms minimum Reset Inactive Time, 70ms implemented)
	 */
	reset_device_by_gpio("tpm_reset", TPM_RESET, 70);

	/*
	 * reset USB hub as required by datasheet
	 * (3ms minimum reset duration, 10ms implemented)
	 */
	reset_device_by_gpio("usbhub_reset", USBHUB_RESET, 10);

	return 0;
}
#endif

int board_mmc_get_env_dev(int devno)
{
	return devno;
}

int board_late_init(void)
{
	return 0;
}
