/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright 2022 Purism SPC
 */

extern ulong imx8m_clk_get_rate(struct clk *clk);
extern ulong imx8m_clk_set_rate(struct clk *clk, unsigned long rate);
extern int imx8m_clk_disable(struct clk *clk);
extern int imx8m_clk_enable(struct clk *clk);
extern int imx8m_clk_set_parent(struct clk *clk, struct clk *parent);

extern struct clk_ops imx8m_clk_ops;
