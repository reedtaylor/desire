#ifndef _DE_SERIAL_H
#define _DE_SERIAL_H

#include <gflags/gflags.h>

#include "desire_util.h"

/*
 * Declare command line flags (via gflags)
 * These can be referenced using FLAGS_[name]
 * DEFINE_ call is in the .cpp source 
 */
DECLARE_string(decent_device_path);


void SetupDESerial(DesireState *desire_state);

#endif
