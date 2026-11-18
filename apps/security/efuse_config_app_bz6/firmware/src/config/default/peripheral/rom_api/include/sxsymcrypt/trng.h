/** True Random Number Generator (TRNG)
 *
 * @file
 * @copyright Copyright (c) 2020 Silex Insight. All Rights reserved.
 *
 * The TRNG uses the hardware to generate random number with a high
 * level of entropy. It's intended to feed entropy into a DRBG.
 */

#ifndef TRNG_H
#define TRNG_H

#include <stdint.h>
#include <stddef.h>
#include "trnginternal.h"

/** Configuration parameters for the TRNG */
struct sx_trng_config {
    /** FIFO level below which the module leaves the idle state to refill the FIFO
     *
     * In numbers of 128-bit blocks.
     *
     * Set to 0 to use default (recommended).
     */
    unsigned int wakeup_level;

    /** Number of clock cycles to wait before sampling data from the noise source
     *
     * Set to 0 to use default.
     */
    unsigned int init_wait;

    /** Number of clock cycles to wait before stopping the rings after the FIFO is full.
     *
     * Set to 0 to use default.
     */
    unsigned int off_time_delay;

    /** Clock divider for the frequency at which the outputs of the rings are sampled.
     *
     * Set to 0 to sample at APB interface clock frequency.
     */
    unsigned int sample_clock_div;
};

/** TRNG initialization
 *
 * @param[inout] ctx TRNG context to be used in other operations
 * @param[in] config pointer to optional configuration. NULL to use default.
 *
 * @return ::SX_OK
 * @return ::SX_ERR_INCOMPATIBLE_HW
 */
int sx_trng_init(struct sx_trng *ctx, const struct sx_trng_config *config);

/** Get random bytes
 *
 * When this function returns ::SX_OK, \p size random bytes have been
 * written to the \p dst memory location. If not enough random bytes are
 * available, the function does not write any data to \p dst and returns
 * ::SX_ERR_HW_PROCESSING.
 *
 * If the hardware will never be able to provide the whole requested
 * size of random data, the function will return ::SX_ERR_TOO_BIG.
 * For more reliable operation, it's recommended to not request more than
 * 32 bytes of data with the default 'sx_trng_config::wakeup_level'.
 *
 * @param[inout] ctx TRNG context to be used in other operations
 * @param[out] dst Destination in memory to copy \p size bytes to
 * @param[in] size length in bytes
 *
 * @return ::SX_OK
 * @return ::SX_ERR_HW_PROCESSING
 * @return ::SX_ERR_RESET_NEEDED
 * @return ::SX_ERR_TOO_BIG
 */
int sx_trng_get(struct sx_trng *ctx, char *dst, size_t size);

#endif
