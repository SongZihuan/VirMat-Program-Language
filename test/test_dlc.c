#include "stdio.h"
#include "virmat.h"

int main() {
    atexit(dlcExit);

    DlcHandle *dlc = openLibary("libtest_lib" SHARED_MARK, RTLD_NOW);
    if (dlc == NULL)
        exit(EXIT_FAILURE);

    typedef int func(int a);
    NEW_DLC_SYMBOL(int, INT);
    NEW_DLC_SYMBOL(func, FUNC);

    DLC_SYMBOL(INT) *a;
    DLC_SYMBOL(FUNC) *fun;

    a = READ_SYMBOL(dlc, "num", INT);
    fun = READ_SYMBOL(dlc, "test", FUNC);

    printf("a = %d, test = %d\n", GET_SYMBOL(a), GET_SYMBOL(fun)(GET_SYMBOL(a)));

    FREE_SYMBOL(a);
    FREE_SYMBOL(fun);

    if (!freeLibary(dlc))
        exit(EXIT_FAILURE);
    return 0;
}