/*
 * Copyright (C) 2017 Fundacion Inria Chile
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @{
 * @ingroup     net
 * @file
 * @brief       Implementation of OpenThread main functions
 *
 * @author      Jose Ignacio Alamos <jialamos@uc.cl>
 * @}
 */

#include <assert.h>

#include "openthread/platform/alarm-milli.h"
#include "openthread/platform/uart.h"
#include "ot.h"
#include "random.h"
#include "thread.h"
#include "xtimer.h"

#ifdef MODULE_AT86RF2XX
#include "at86rf2xx.h"
#include "at86rf2xx_params.h"
#endif

#ifdef MODULE_KW2XRF
#include "kw2xrf.h"
#include "kw2xrf_params.h"
#endif

#define ENABLE_DEBUG (0)
#include "debug.h"

#ifdef MODULE_AT86RF2XX     /* is mutual exclusive with above ifdef */
#define OPENTHREAD_NETIF_NUMOF        (sizeof(at86rf2xx_params) / sizeof(at86rf2xx_params[0]))
#endif

#ifdef MODULE_KW2XRF     /* is mutual exclusive with above ifdef */
#define OPENTHREAD_NETIF_NUMOF        (sizeof(kw2xrf_params) / sizeof(kw2xrf_params[0]))
#endif

#ifdef MODULE_AT86RF2XX
static at86rf2xx_t at86rf2xx_dev;
#endif

#ifdef MODULE_KW2XRF
static kw2xrf_t kw2xrf_dev;
#endif

static uint8_t rx_buf[OPENTHREAD_NETDEV_BUFLEN];
static uint8_t tx_buf[OPENTHREAD_NETDEV_BUFLEN];
static char ot_thread_stack[2 * THREAD_STACKSIZE_MAIN];

void openthread_bootstrap(void)
{
    /* init random */
    ot_random_init();

    /* setup netdev modules */
#ifdef MODULE_AT86RF2XX
    at86rf2xx_setup(&at86rf2xx_dev, &at86rf2xx_params[0]);
    netdev_t *netdev = (netdev_t *) &at86rf2xx_dev;
#endif

#ifdef MODULE_KW2XRF
    kw2xrf_setup(&kw2xrf_dev, &kw2xrf_params[0]);
    netdev_t *netdev = (netdev_t *) &kw2xrf_dev;
#endif

    openthread_radio_init(netdev, tx_buf, rx_buf);
    openthread_netdev_init(ot_thread_stack, sizeof(ot_thread_stack), THREAD_PRIORITY_MAIN - 5, "openthread", netdev);
}
