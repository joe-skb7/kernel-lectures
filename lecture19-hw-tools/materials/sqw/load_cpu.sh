#!/bin/sh

fulload() {
	dd if=/dev/zero of=/dev/null | \
	dd if=/dev/zero of=/dev/null | \
	dd if=/dev/zero of=/dev/null | \
	dd if=/dev/zero of=/dev/null &
}

fulload
read
killall dd
