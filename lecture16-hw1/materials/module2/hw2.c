#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>

#define AM335_GPIO(bank,line)	(32 * bank + line)
#define LED_GPIO		AM335_GPIO(1, 15)
#define BTN_GPIO		AM335_GPIO(0, 27)

static int irq;
static int led_on;

static irqreturn_t hw2_btn_isr(int num, void *priv)
{
	led_on ^= 0x1;
	gpio_set_value(LED_GPIO, led_on);
	pr_info("interrupt!\n");
	return IRQ_HANDLED;
}

static int __init hw2_init(void)
{
	int err;

	err = gpio_request(BTN_GPIO, "my_button");
	if (err) {
		pr_err("Unable to request button GPIO\n");
		return -EINVAL;
	}
	gpio_direction_input(BTN_GPIO);
	irq = gpio_to_irq(BTN_GPIO);
	err = request_threaded_irq(irq, NULL, hw2_btn_isr,
				   IRQF_TRIGGER_FALLING | IRQF_ONESHOT,
				   "my button key", NULL);
	if (err) {
		pr_err("Unable to request interrupt for button\n");
		return -EINVAL;
	}

	err = gpio_request(LED_GPIO, "my_led");
	if (err) {
		pr_err("Unable to request LED GPIO\n");
		return -EINVAL;
	}
	gpio_direction_output(LED_GPIO, led_on);

	return 0;
}

static void __exit hw2_exit(void)
{
	free_irq(irq, NULL);
	gpio_free(BTN_GPIO);
	gpio_free(LED_GPIO);
}

module_init(hw2_init);
module_exit(hw2_exit);

MODULE_AUTHOR("Sam Protsenko <joe.skb7@gmail.com>");
MODULE_DESCRIPTION("Test module 2");
MODULE_LICENSE("GPL");
