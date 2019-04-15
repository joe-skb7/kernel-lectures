// SPDX-License-Identifier: GPL-2.0
/*
 * Minimal driver for DS1307 RTC.
 */

#include <linux/module.h>
#include <linux/i2c.h>

static int ds1307x_probe(struct i2c_client *client,
			 const struct i2c_device_id *id)
{
	s32 data;

	/* TODO */
	pr_info("### my_rtc probed!\n");

	data = i2c_smbus_read_byte_data(client, 0x01); /* minutes */
	pr_info("### read data = %#x\n", data);

#if 0
	{
	int ret;
	u8 reg = 0x1;	/* I2C register */
	u8 buf;		/* where to read */
	u8 len = 1;	/* bytes to read */
	struct i2c_msg msg[2] = {
		{
			.addr = client->addr,
			.len = 1,
			.buf = &reg,
		},
		{
			.addr = client->addr,
			.flags = I2C_M_RD,
			.len = len,
			.buf = &buf,
		}
	};

	ret = i2c_transfer(client->adapter, msg, 2);
	if (ret < 0) {
		dev_err(&client->dev, "I2C read failed\n");
		return ret;
	}

	pr_info("### read data = %#x\n", buf);
	}
#endif

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

MODULE_AUTHOR("Sam Protsenko <semen.protsenko@globallogic.com>");
MODULE_DESCRIPTION("DS1307 RTC driver");
MODULE_LICENSE("GPL");
