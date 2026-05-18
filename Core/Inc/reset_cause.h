#ifndef INC_RESET_CAUSE_H_
#define INC_RESET_CAUSE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef enum {
    RESET_CAUSE_UNKNOWN       = 0,
    RESET_CAUSE_POWER_ON      = (1 << 0),
    RESET_CAUSE_PIN_RESET     = (1 << 1),
    RESET_CAUSE_BROWNOUT      = (1 << 2),
    RESET_CAUSE_SOFTWARE      = (1 << 3),
    RESET_CAUSE_IWDG          = (1 << 4),
    RESET_CAUSE_WWDG          = (1 << 5),
    RESET_CAUSE_LOW_POWER     = (1 << 6),
} reset_cause_t;

void reset_cause_read_and_clear(void);
reset_cause_t reset_cause_get(void);
void reset_cause_log(void);

#ifdef __cplusplus
}
#endif

#endif /* INC_RESET_CAUSE_H_ */
