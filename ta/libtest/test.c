#include <compiler.h>
#include "test.h"
#include <trace.h>
#include <sys/types.h>

extern void bar(void);

void (*FUNC[])(void) __maybe_unused = {
	&bar,
};

void foo(void)
{
	int i __maybe_unused = 0;

	DMSG("FUNC[i] = %p", (void *)(uintptr_t)FUNC[i]);
	DMSG("FUNC[0] = %p", (void *)(uintptr_t)FUNC[0]);
}
