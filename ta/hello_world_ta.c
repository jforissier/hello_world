/*
 * Copyright (c) 2016, Linaro Limited
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * TA example for gprof. Build environment: OP-TEE and QEMU.
 * (https://github.com/OP-TEE/manifest default.xml)
 *
 * - make -C build CFG_TA_GPROF_SUPPORT=y CFLAGS_ta_arm32="-pg -Wno-pedantic" \
 *                 QEMU_VIRTFS_ENABLE=y QEMU_VIRTFS_HOST_DIR=/tmp run
 * -   root@Vexpress:/ hello_world
 *     root@Vexpress:/ mkdir /tmp/host; mount_shared /tmp/host
 *     root@Vexpress:/ cp /tmp/gmon* /tmp/host
 * - gprof hello_world/ta/8a*.elf /tmp/gmon-8a*.out
 */
#include <tee_internal_api.h>
#include <trace.h>

#define TIME_MS() ({				\
	TEE_Time t;				\
						\
	TEE_GetSystemTime(&t);			\
	t.seconds * 1000 + t.millis;		\
})

#define SPIN(sec) do {                          \
        int _i;                                 \
        uint64_t t0 = TIME_MS();                \
                                                \
        while (TIME_MS() < t0 + sec * 1000)     \
                for (_i = 0; _i < 10000; _i++)  \
                        ;                       \
} while(0)

static void foo(int sec)
{
        IMSG("foo");
        SPIN(sec);
}

static void bar(void)
{
        int i;

        IMSG("bar");
        SPIN(3);
        for (i = 0; i < 3; i++)
                foo(1);
}

TEE_Result TA_InvokeCommandEntryPoint(void __unused *sess_ctx,
			uint32_t __unused cmd_id,
			uint32_t __unused param_types,
			TEE_Param __unused params[4])
{
	IMSG("TA_InvokeCommandEntryPoint");
	SPIN(2);
	foo(2);
	bar();
	return TEE_SUCCESS;
}

TEE_Result TA_CreateEntryPoint(void)
{
	return TEE_SUCCESS;
}

void TA_DestroyEntryPoint(void)
{
}

TEE_Result TA_OpenSessionEntryPoint(uint32_t __unused param_types,
		TEE_Param __unused params[4],
		void __unused **sess_ctx)
{
	return TEE_SUCCESS;
}

void TA_CloseSessionEntryPoint(void __unused *sess_ctx)
{
}

