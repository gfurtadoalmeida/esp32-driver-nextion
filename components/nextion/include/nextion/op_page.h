#ifndef __NEXTION_OP_PAGE_H__
#define __NEXTION_OP_PAGE_H__

#include <stdint.h>
#include "codes.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief Get the active page id.
     * @return NEX_OK or NEX_FAIL.
     */
    nex_err_t nextion_page_get(uint8_t *page_id);

    /**
     * @brief Changes to another page.
     * @return NEX_OK or NEX_DVC_ERR_INVALID_PAGE.
     */
    nex_err_t nextion_page_set(uint8_t page_id);

    /**
     * @brief Refreshes the current page.
     * @return NEX_OK or NEX_FAIL.
     */
    nex_err_t nextion_page_refresh();
#ifdef __cplusplus
}
#endif

#endif //__NEXTION_OP_PAGE_H__