/*
 * Copyright (C) 2009 The Android Open Source Project
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

#define LOG_TAG "Performance-JNI"

#include "jni.h"
#include "JNIHelp.h"
#include "android_runtime/AndroidRuntime.h"

#include <utils/misc.h>
#include <utils/Log.h>
#include <hardware/hardware.h>
#include <hardware/power.h>
#include <hardware_legacy/power.h>

namespace android
{
static struct power_module* gPowerModule;

static void nativeInit(JNIEnv *env, jobject clazz)
{
    int err = hw_get_module(POWER_HARDWARE_MODULE_ID,
            (hw_module_t const**)&gPowerModule);
    if (!err) {
        gPowerModule->init(gPowerModule);
    } else {
        ALOGE("Couldn't load %s module (%d)", POWER_HARDWARE_MODULE_ID, err);
    }
}

static void setBoostEnable_native(JNIEnv *env, jobject clazz,jint enable)
{
    int data_param = enable;
    // do set Boost Enable/Disable
    // Tell the power HAL when start activity occurs.
    if (gPowerModule && gPowerModule->powerHint) {
        if(enable) {
            gPowerModule->powerHint(gPowerModule, POWER_HINT_ACTIVITY, &data_param);
        } else {
            gPowerModule->powerHint(gPowerModule, POWER_HINT_ACTIVITY, NULL);
        }
    }
}

static void setBoostPerformance_native(JNIEnv *env, jobject clazz,jint cpu_nr, jint duration)
{
    //do set Boost Enable/Disable
    // Tell the power HAL when start activity occurs.
    if (gPowerModule && gPowerModule->powerHint) {
        struct power_interaction_data power_data;

        power_data.cpu_nr = cpu_nr;
        power_data.duration = duration;
        gPowerModule->powerHint(gPowerModule, POWER_HINT_TOUCH, &power_data);
    }
}

static JNINativeMethod method_table[] = {
    { "nativeInit", "()V", (void*)nativeInit },
    { "setBoostEnable_native", "(I)V", (void*)setBoostEnable_native },
    { "setBoostPerformance_native", "(II)V", (void*)setBoostPerformance_native },
};

int register_android_os_Performance(JNIEnv *env)
{
    return jniRegisterNativeMethods(env, "android/os/Performance",
            method_table, NELEM(method_table));
}

};// namespace android
