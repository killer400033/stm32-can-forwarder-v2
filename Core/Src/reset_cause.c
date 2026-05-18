#include "reset_cause.h"
#include "stm32h7xx_hal.h"
#include "log_handler.h"

static reset_cause_t stored_reset_cause = RESET_CAUSE_UNKNOWN;

void reset_cause_read_and_clear(void)
{
    stored_reset_cause = RESET_CAUSE_UNKNOWN;

    if (__HAL_RCC_GET_FLAG(RCC_FLAG_IWDG1RST)) {
        stored_reset_cause |= RESET_CAUSE_IWDG;
    }
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_WWDG1RST)) {
        stored_reset_cause |= RESET_CAUSE_WWDG;
    }
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_SFTRST)) {
        stored_reset_cause |= RESET_CAUSE_SOFTWARE;
    }
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_PORRST)) {
        stored_reset_cause |= RESET_CAUSE_POWER_ON;
    }
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_PINRST)) {
        stored_reset_cause |= RESET_CAUSE_PIN_RESET;
    }
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_BORRST)) {
        stored_reset_cause |= RESET_CAUSE_BROWNOUT;
    }
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_LPWR1RST) || __HAL_RCC_GET_FLAG(RCC_FLAG_LPWR2RST)) {
        stored_reset_cause |= RESET_CAUSE_LOW_POWER;
    }

    __HAL_RCC_CLEAR_RESET_FLAGS();
}

reset_cause_t reset_cause_get(void)
{
    return stored_reset_cause;
}

void reset_cause_log(void)
{
    if (stored_reset_cause == RESET_CAUSE_UNKNOWN) {
        log_msg(LL_WRN, "Reset cause: Unknown");
        return;
    }

    if (stored_reset_cause & RESET_CAUSE_IWDG) {
        log_msg(LL_ERR, "Reset cause: Independent Watchdog (IWDG) timeout");
    }
    if (stored_reset_cause & RESET_CAUSE_WWDG) {
        log_msg(LL_ERR, "Reset cause: Window Watchdog (WWDG) timeout");
    }
    if (stored_reset_cause & RESET_CAUSE_SOFTWARE) {
        log_msg(LL_WRN, "Reset cause: Software reset (NVIC_SystemReset)");
    }
    if (stored_reset_cause & RESET_CAUSE_POWER_ON) {
        log_msg(LL_DBG, "Reset cause: Power-on reset (POR/PDR)");
    }
    if (stored_reset_cause & RESET_CAUSE_PIN_RESET) {
        log_msg(LL_DBG, "Reset cause: External pin reset (NRST)");
    }
    if (stored_reset_cause & RESET_CAUSE_BROWNOUT) {
        log_msg(LL_WRN, "Reset cause: Brown-out reset (BOR)");
    }
    if (stored_reset_cause & RESET_CAUSE_LOW_POWER) {
        log_msg(LL_WRN, "Reset cause: Low-power management reset");
    }
}
