#ifndef _FLAGS_H
#define _FLAGS_H

#include <gflags/gflags.h>

/*
 * Define command line flags (via gflags)
 * These can be referenced using FLAGS_[name]
 */
DEFINE_string(decent_device_path, "/dev/serial0", "Path to serial device where the Decent machine is connected");

#endif
