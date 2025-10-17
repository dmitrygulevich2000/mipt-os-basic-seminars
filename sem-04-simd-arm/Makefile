call:
	aarch64-linux-gnu-gcc call.s && \
		QEMU_LD_PREFIX=/usr/aarch64-linux-gnu/ qemu-aarch64 a.out

hello-no-std:
	aarch64-linux-gnu-gcc -nostdlib hello-no-std.S && \
		qemu-aarch64 -L /usr/aarch64-linux-gnu/ a.out
