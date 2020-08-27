#ifndef __NEXTION_OP_SCREEN_H__
#define __NEXTION_OP_SCREEN_H__

#include "codes.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief Get the current page showing.
     * @return -1 if error, otherwise page id.
     */
    nex_err_t nextion_get_current_page(int *page_id);

#ifdef __cplusplus
}
#endif

#endif //__NEXTION_OP_SCREEN_H__