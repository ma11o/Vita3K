// Vita3K emulator project
// Copyright (C) 2026 Vita3K team
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

#pragma once

#include <cctype>
#include <cstdlib>
#include <string>

#ifdef __ANDROID__
#include <sys/system_properties.h>
#endif

// Reads a PSO2_* switch from the environment, or on Android (where an app gets no environment) from the
// system property debug.pso2.<name in lower case without the PSO2_ prefix>, which adb can set:
//   PSO2_NO_SURFACE_WRITEBACK=1  <=>  adb shell setprop debug.pso2.no_surface_writeback 1
// Returns nullptr when unset or empty.
inline const char *pso2_env(const char *name) {
    if (const char *v = std::getenv(name); v && *v)
        return v;
#ifdef __ANDROID__
    std::string prop = "debug.pso2.";
    for (const char *p = name + (std::string(name).rfind("PSO2_", 0) == 0 ? 5 : 0); *p; p++)
        prop += static_cast<char>(std::tolower(static_cast<unsigned char>(*p)));
    static thread_local char value[PROP_VALUE_MAX];
    if (__system_property_get(prop.c_str(), value) > 0 && value[0] != '0')
        return value;
#endif
    return nullptr;
}
