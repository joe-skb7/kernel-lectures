#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <asm/io.h>

/* Modules base addresses and registers offsets */
#define CTRL_MODULE_BASE	0x44e10000
#define GPIO0_BASE		0x44e07000
#define GPIO1_BASE		0x4804c000
#define CONF_GPMC_AD11_OFFSET	0x82c
#define CONF_GPMC_AD15_OFFSET	0x83c
#define GPIO_CTRL_OFFSET	0x130
#define GPIO_OE_OFFSET		0x134
#define GPIO_DATAIN_OFFSET	0x138
#define GPIO_DATAOUT_OFFSET	0x13c

/* Registers addresses */
#define CONF_GPMC_AD11		(CTRL_MODULE_BASE + CONF_GPMC_AD11_OFFSET)
#define CONF_GPMC_AD15		(CTRL_MODULE_BASE + CONF_GPMC_AD15_OFFSET)
#define GPIO0_CTRL		(GPIO0_BASE + GPIO_CTRL_OFFSET)
#define GPIO1_CTRL		(GPIO1_BASE + GPIO_CTRL_OFFSET)
#define GPIO0_OE		(GPIO0_BASE + GPIO_OE_OFFSET)
#define GPIO1_OE		(GPIO1_BASE + GPIO_OE_OFFSET)
#define GPIO0_DATAIN		(GPIO0_BASE + GPIO_DATAIN_OFFSET)
#define GPIO1_DATAOUT		(GPIO1_BASE + GPIO_DATAOUT_OFFSET)

/* conf_<module>_<pin> registers flags */
#define MUX_MODE7		0x7
#define PULL_DISABLE		BIT(3)
#define INPUT_EN		BIT(5)
#define BTN_CONF		(MUX_MODE7 | PULL_DISABLE | INPUT_EN)
#define LED_CONF		(MUX_MODE7 | PULL_DISABLE)

/* GPIO line numbers for button and LED */
#define BTN_LINE		BIT(27)		/* gpmc_ad11.gpio0_27 */
#define LED_LINE		BIT(15)		/* gpmc_ad15.gpio1_15 */

static struct task_struct *thread;
static void __iomem *btn_gpio, *led_gpio;

static void hw1_mux_pins(void)
{
	void __iomem *reg;

	/* BTN: mux the pin */
	reg = ioremap(CONF_GPMC_AD11, 4);
	iowrite32(BTN_CONF, reg);
	iounmap(reg);

	/* LED: mux the pin */
	reg = ioremap(CONF_GPMC_AD15, 4);
	iowrite32(LED_CONF, reg);
	iounmap(reg);
}

static void hw1_setup_gpio(void)
{
	void __iomem *reg;
	u32 val;

	/* BTN: enable modules, clock = functional */
	reg = ioremap(GPIO0_CTRL, 4);
	iowrite32(0x0, reg);
	iounmap(reg);

	/* LED: enable modules, clock = functional */
	reg = ioremap(GPIO1_CTRL, 4);
	iowrite32(0x0, reg);
	iounmap(reg);

	/* BTN: configure input/output mode */
	reg = ioremap(GPIO0_OE, 4);
	val = ioread32(reg);
	val |= BTN_LINE;	/* 1 = input */
	iowrite32(val, reg);
	iounmap(reg);

	/* LED: configure input/output mode */
	reg = ioremap(GPIO1_OE, 4);
	val = ioread32(reg);
	val &= ~LED_LINE;	/* 0 = output */
	iowrite32(val, reg);
	iounmap(reg);
}

static int hw1_thread_func(void *data)
{
	/* Poll the button */
	while (!kthread_should_stop()) {
		u32 btn_val, led_val;

		btn_val = ioread32(btn_gpio);
		led_val = ioread32(led_gpio);
		if (btn_val & BTN_LINE) {
			/* not pressed (pull-up): LED off */
			led_val &= ~LED_LINE;
		} else {
			/* pressed (GND): LED on */
			led_val |= LED_LINE;
		}

		iowrite32(led_val, led_gpio);

		msleep(100);
	}

	return 0;
}

static int __init hw1_init(void)
{
	int ret;

	hw1_mux_pins();
	hw1_setup_gpio();

	btn_gpio = ioremap(GPIO0_DATAIN, 4);
	led_gpio = ioremap(GPIO1_DATAOUT, 4);

	thread = kthread_run(hw1_thread_func, NULL, "hw1_thread");
	if (IS_ERR(thread)) {
		pr_err("kthread_run() failed\n");
		ret = PTR_ERR(thread);
		goto err1;
	}

	return 0;

err1:
	iounmap(led_gpio);
	iounmap(btn_gpio);
	return ret;
}

static void __exit hw1_exit(void)
{
	if (thread)
		kthread_stop(thread);
	iounmap(led_gpio);
	iounmap(btn_gpio);
}

module_init(hw1_init);
module_exit(hw1_exit);

MODULE_AUTHOR("Sam Protsenko <semen.protsenko@globallogic.com>");
MODULE_DESCRIPTION("Test module 1");
MODULE_LICENSE("GPL");
