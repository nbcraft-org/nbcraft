#pragma once

#define MC_VER_ERA_ALPHA   1
#define MC_VER_ERA_BETA    2
#define MC_VER_ERA_RELEASE 3

#define MC_VER_ENCODE(era, major, minor, patch) \
    (((era) * 1000000) + ((major) * 10000) + ((minor) * 100) + (patch))

#define MC_VER_GET_ERA(ver)   ((ver) / 1000000)
#define MC_VER_GET_MAJOR(ver) (((ver) / 10000) % 100)
#define MC_VER_GET_MINOR(ver) (((ver) / 100) % 100)
#define MC_VER_GET_PATCH(ver) ((ver) % 100)

#define MC_VER_ALPHA(maj, min, pat)   MC_VER_ENCODE(MC_VER_ERA_ALPHA,   maj, min, pat)
#define MC_VER_BETA(maj, min, pat)    MC_VER_ENCODE(MC_VER_ERA_BETA,    maj, min, pat)
#define MC_VER_RELEASE(maj, min, pat) MC_VER_ENCODE(MC_VER_ERA_RELEASE, maj, min, pat)

#ifndef MC_VERSION
#define MC_VERSION MC_VER_BETA(1, 7, 3)
#endif
