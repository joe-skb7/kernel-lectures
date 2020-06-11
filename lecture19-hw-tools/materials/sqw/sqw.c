#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/kthread.h>
#include <linux/delay.h>

#define AM335_GPIO(bank,line)	(32 * bank + line)
#define LED_GPIO		AM335_GPIO(1, 15)

static struct task_struct *thread;
static int led_val;

static int sqw_thread_func(void *data)
{
	while (!kthread_should_stop()) {
		gpio_set_value(LED_GPIO, led_val);
		led_val ^= 0x1;
		/* Documentation/timers/timers-howto.txt */
		/* Don't use udelay(), we need to yield to scheduler */
		usleep_range(20, 20); /* can be 10 us - 20 ms */
	}

	return 0;
}

static int __init sqw_init(void)
{
	int err;

	err = gpio_request(LED_GPIO, "my_led");
	if (err) {
		pr_err("Unable to request LED GPIO\n");
		return -EINVAL;
	}
	gpio_direction_output(LED_GPIO, 0); /* init LED value = 0 */

	thread = kthread_run(sqw_thread_func, NULL, "sqw_thread");
	if (IS_ERR(thread)) {
		pr_err("kthread_run() failed\n");
		err = PTR_ERR(thread);
		return err;
	}

	return 0;
}

static void __exit sqw_exit(void)
{
	if (thread)
		kthread_stop(thread);
	gpio_free(LED_GPIO);
}

module_init(sqw_init);
module_exit(sqw_exit);

MODULE_AUTHOR("Sam Protsenko <joe.skb7@gmail.com>");
MODULE_DESCRIPTION("Square Wave Module");
MODULE_LICENSE("GPL");
