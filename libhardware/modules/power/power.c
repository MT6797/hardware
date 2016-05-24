/*
 * Copyright (C) 2012 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define LOG_TAG "Legacy PowerHAL"
#include <utils/Log.h>

#include <hardware/hardware.h>
#include <hardware/power.h>

#define MT_PPM_MODE_PATH "/proc/ppm/mode"
#define MT_HPS_POWER_MODE_PATH "/proc/hps/power_mode"
#define MT_CPUFREQ_POWER_MODE_PATH "/proc/cpufreq/cpufreq_power_mode"
#define MT_GED_EVENT_NOTIFY_PATH "/d/ged/hal/event_notify"
#define MT_DISPSYS_PATH "/d/dispsys"

#define POWER_HINT_POWER_SAVING 0x00000101
#define POWER_HINT_PERFORMANCE_BOOST 0x00000102
#define POWER_HINT_BALANCE  0x00000103

static void power_init(struct power_module *module)
{
}

static void power_set_interactive(struct power_module *module, int on)
{
}

static void power_fwrite(const char *path, char *s)
{
    char buf[64];
    int len;
    int fd = open(path, O_WRONLY);

    if (fd < 0) {
        strerror_r(errno, buf, sizeof(buf));
        ALOGE("Error opening %s: %s\n", path, buf);
        return;
    }

    len = write(fd, s, strlen(s));
    if (len < 0) {
        strerror_r(errno, buf, sizeof(buf));
        ALOGE("Error writing to %s: %s\n", path, buf);
    }

    close(fd);
}

static void power_hint(struct power_module *module, power_hint_t hint,
                       void *data) {
    switch (hint) {
        case POWER_HINT_PERFORMANCE_BOOST:
            ALOGI("POWER_HINT_PERFORMANCE_BOOST");
            power_fwrite(MT_PPM_MODE_PATH, "performance");
            power_fwrite(MT_HPS_POWER_MODE_PATH, "3");
            power_fwrite(MT_CPUFREQ_POWER_MODE_PATH, "3");
            power_fwrite(MT_GED_EVENT_NOTIFY_PATH, "low-power-mode 0");
            power_fwrite(MT_DISPSYS_PATH, "low_power_mode:3");
            break;
        case POWER_HINT_BALANCE:
            ALOGI("POWER_HINT_BALANCE");
            power_fwrite(MT_PPM_MODE_PATH, "performance");
            power_fwrite(MT_HPS_POWER_MODE_PATH, "3");
            power_fwrite(MT_CPUFREQ_POWER_MODE_PATH, "3");
            power_fwrite(MT_GED_EVENT_NOTIFY_PATH, "low-power-mode 0");
            power_fwrite(MT_DISPSYS_PATH, "low_power_mode:2");
            break;
        case POWER_HINT_POWER_SAVING:
            ALOGI("POWER_HINT_POWER_SAVING");
            power_fwrite(MT_PPM_MODE_PATH, "low_power");
            power_fwrite(MT_HPS_POWER_MODE_PATH, "1");
            power_fwrite(MT_CPUFREQ_POWER_MODE_PATH, "1");
            power_fwrite(MT_GED_EVENT_NOTIFY_PATH, "low-power-mode 1");
            power_fwrite(MT_DISPSYS_PATH, "low_power_mode:1");
            break;
    default:
        break;
    }
}

static struct hw_module_methods_t power_module_methods = {
    .open = NULL,
};

struct power_module HAL_MODULE_INFO_SYM = {
    .common = {
        .tag = HARDWARE_MODULE_TAG,
        .module_api_version = POWER_MODULE_API_VERSION_0_2,
        .hal_api_version = HARDWARE_HAL_API_VERSION,
        .id = POWER_HARDWARE_MODULE_ID,
        .name = "Default Power HAL",
        .author = "The Android Open Source Project",
        .methods = &power_module_methods,
    },

    .init = power_init,
    .setInteractive = power_set_interactive,
    .powerHint = power_hint,
};
