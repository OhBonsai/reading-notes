#ifdef COMPILETIME
#include <stdio.h>
#include <malloc.h>

/* malloc wrapper function */
void *mymalloc(size_t size)
{
    void *ptr = malloc(size);
    printf("malloc(%d)=%p\n",
           (int)size, ptr);
    return ptr;
}

/* free wrapper function */
void myfree(void *ptr)
{
    free(ptr);
    printf("free(%p)\n", ptr);
}
#endif


#ifdef LINKTIME
#include <stdio.h>

void *__real_malloc(size_t size);
void __real_free(void *ptr);

void *__wrap_malloc(size_t size)
{
		void *ptr = __real_malloc(size);
		printf("malloc(%d)=%p\n",(int)size, ptr);
		return ptr;
}

void __wrap_free(void *ptr)
{
		__real_free(ptr);
		printf("free(%p)\n", ptr);
}
#endif
