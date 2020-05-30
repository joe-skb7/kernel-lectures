// SPDX-License-Identifier: GPL-2.0
/*
 * Minimal driver for DS1307 RTC.
 */

#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/regmap.h>

/* see DS307 datasheet for details */
static const struct regmap_config regmap_config = {
	.reg_bits = 8,		/* registers address size, in bits */
	.val_bits = 8,		/* registers values size. in bits */
	.max_register = 0x7,	/* last register address */
};

static struct regmap *regmap;

static int ds1307x_probe(struct i2c_client *client,
			 const struct i2c_device_id *id)
{
	s32 data;
	u8 regs[7];
	int reg;
	int ret;
	int i;

	/* TODO */
	pr_info("### my_rtc probe(): enter\n");

	data = i2c_smbus_read_byte_data(client, 0x01); /* minutes */
	pr_info("### read data = %#x\n", data);

	regmap = devm_regmap_init_i2c(client, &regmap_config);
	if (IS_ERR(regmap)) {
		dev_err(&client->dev, "regmap allocation failed\n");
		return PTR_ERR(regmap);
	}

	/* read the RTC date and time registers all at once */
	ret = regmap_bulk_read(regmap, 0, regs, sizeof(regs));
	if (ret) {
		dev_err(&client->dev, "%s error %d\n", "read", ret);
		return ret;
	}
	for (i = 0; i < 7; ++i)
		dev_info(&client->dev, "reg %#x\t=\t%#x\n", i, regs[i]);

	ret = regmap_read(regmap, 0xaa, &reg);
	if (ret < 0)
		dev_warn(&client->dev, "### Unable to read 0xaa!\n");
	else
		dev_info(&client->dev, "### read value for 0xaa = %#x\n", reg);

	pr_info("### my_rtc probe(): success\n");
	return 0;
}

static int ds1307x_remove(struct i2c_client *client)
{
	/* TODO */
	return 0;
}

static const struct i2c_device_id ds1307x_id[] = {
	{ "ds1307x" },
	{ },
};
MODULE_DEVICE_TABLE(i2c, ds1307x_id);

static const struct of_device_id ds1307x_of_match[] = {
	{ .compatible = "globallogic,ds1307x" }, /* ds1307 already exists */
	{ }
};
MODULE_DEVICE_TABLE(of, ds1307x_of_match);

static struct i2c_driver ds1307x_driver = {
	.driver = {
		.name		= "ds1307x",
		.of_match_table	= ds1307x_of_match,
	},
	.probe		= ds1307x_probe,
	.remove		= ds1307x_remove,
	.id_table	= ds1307x_id,
};

module_i2c_driver(ds1307x_driver);

MODULE_AUTHOR("Sam Protsenko <joe.skb7@gmail.com>");
MODULE_DESCRIPTION("DS1307 RTC driver");
MODULE_LICENSE("GPL");
