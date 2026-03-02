#ifndef INC_STORAGE_H_
#define INC_STORAGE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "forwarder_pb.pb.h"
#include <stdint.h>

#define MIN_FREE_SPACE_MB 100  // Minimum free space in MB before wiping SD card

void initStorage();

#ifdef __cplusplus
}
#endif
#endif
