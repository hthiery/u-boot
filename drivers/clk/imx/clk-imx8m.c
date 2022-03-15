// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright 2018 NXP
 * Copyright 2022 Purism SPC
 * Peng Fan <peng.fan@nxp.com>
 */

#include <common.h>
#include <clk-uclass.h>
#include <dm.h>
#include <log.h>
#include <malloc.h>
#include <asm/arch/clock.h>
#include <misc.h>

#include "clk-imx8m.h"

ulong imx8m_clk_get_rate(struct clk *clk)
{
	struct clk *c;
	int ret;

	debug("%s(#%lu)\n", __func__, clk->id);

	ret = clk_get_by_id(clk->id, &c);
	if (ret)
		return ret;

	return clk_get_rate(c);
}
EXPORT_SYMBOL_GPL(imx8m_clk_get_rate);

ulong imx8m_clk_set_rate(struct clk *clk, unsigned long rate)
{
	struct clk *c;
	int ret;

	debug("%s(#%lu), rate: %lu\n", __func__, clk->id, rate);

	ret = clk_get_by_id(clk->id, &c);
	if (ret)
		return ret;

	return clk_set_rate(c, rate);
}
EXPORT_SYMBOL_GPL(imx8m_clk_set_rate);

static int __imx8m_clk_enable(struct clk *clk, bool enable)
{
	struct clk *c;
	int ret;

	debug("%s(#%lu) en: %d\n", __func__, clk->id, enable);

	ret = clk_get_by_id(clk->id, &c);
	if (ret)
		return ret;

	if (enable)
		ret = clk_enable(c);
	else
		ret = clk_disable(c);

	return ret;
}

int imx8m_clk_disable(struct clk *clk)
{
	return __imx8m_clk_enable(clk, 0);
}
EXPORT_SYMBOL_GPL(imx8m_clk_disable);

int imx8m_clk_enable(struct clk *clk)
{
	return __imx8m_clk_enable(clk, 1);
}
EXPORT_SYMBOL_GPL(imx8m_clk_enable);

int imx8m_clk_set_parent(struct clk *clk, struct clk *parent)
{
	struct clk *c, *cp;
	int ret;

	debug("%s(#%lu), parent: %lu\n", __func__, clk->id, parent->id);

	ret = clk_get_by_id(clk->id, &c);
	if (ret)
		return ret;

	ret = clk_get_by_id(parent->id, &cp);
	if (ret)
		return ret;

	ret = clk_set_parent(c, cp);
	c->dev->parent = cp->dev;

	return ret;
}
EXPORT_SYMBOL_GPL(imx8m_clk_set_parent);

struct clk_ops imx8m_clk_ops = {
	.set_rate = imx8m_clk_set_rate,
	.get_rate = imx8m_clk_get_rate,
	.enable = imx8m_clk_enable,
	.disable = imx8m_clk_disable,
	.set_parent = imx8m_clk_set_parent,
};
EXPORT_SYMBOL_GPL(imx8m_clk_ops);
