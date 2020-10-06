#include "utils.h"

int add(theatre src[], theatre **new, size_t *len_src, size_t *capacity_src, size_t cnt)
{
    if (!src || !capacity_src || !new)
        return ARG_ERR;
    if ((*len_src + cnt) > *capacity_src)
    {
        *new = (theatre *) realloc(src, sizeof(theatre) * (cnt + SIZE_INC));
        if (!(*new))
        {
            return ALLOCATION_ERR;
        }
        src = *new;
        *capacity_src = *capacity_src + cnt + SIZE_INC;
    }
    theatre dst[cnt];
    size_t len_dst = 0;
    if (read(dst, len_dst, stdin, cnt))
    {
        return READ_ERR;
    }
    for (size_t i = *len_src, j = 0; i < *len_src + cnt; i++, j++)
    {
        src[i] = dst[j];
    }
    *len_src += cnt;
    return EXIT_SUCCESS;
}
int expansion(theatre arr[],size_t cnt_to_ins, size_t *cappacity)
{
    return 0;

}