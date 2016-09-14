/*
 * Copyright (C) 2016 The Android Open Source Project
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

#define LOG_TAG "AudioFlinger::DeviceHalLocal"
//#define LOG_NDEBUG 0

#include <utils/Log.h>

#include "DeviceHalLocal.h"

namespace android {

DeviceHalLocal::DeviceHalLocal(audio_hw_device_t *dev)
        : mDev(dev) {
}

DeviceHalLocal::~DeviceHalLocal() {
    int status = audio_hw_device_close(mDev);
    ALOGW_IF(status, "Error closing audio hw device %p: %s", mDev, strerror(-status));
    mDev = 0;
}

status_t DeviceHalLocal::getSupportedDevices(uint32_t *devices) {
    if (mDev->get_supported_devices == NULL) return INVALID_OPERATION;
    *devices = mDev->get_supported_devices(mDev);
    return OK;
}

status_t DeviceHalLocal::getVersion(uint32_t *version) {
    *version = mDev->common.version;
    return OK;
}

status_t DeviceHalLocal::initCheck() {
    return mDev->init_check(mDev);
}

status_t DeviceHalLocal::setVoiceVolume(float volume) {
    return mDev->set_voice_volume(mDev, volume);
}

status_t DeviceHalLocal::setMasterVolume(float volume) {
    if (mDev->set_master_volume == NULL) return INVALID_OPERATION;
    return mDev->set_master_volume(mDev, volume);
}

status_t DeviceHalLocal::getMasterVolume(float *volume) {
    if (mDev->get_master_volume == NULL) return INVALID_OPERATION;
    return mDev->get_master_volume(mDev, volume);
}

status_t DeviceHalLocal::setMode(audio_mode_t mode) {
    return mDev->set_mode(mDev, mode);
}

status_t DeviceHalLocal::setMicMute(bool state) {
    return mDev->set_mic_mute(mDev, state);
}

status_t DeviceHalLocal::getMicMute(bool *state) {
    return mDev->get_mic_mute(mDev, state);
}

status_t DeviceHalLocal::setMasterMute(bool state) {
    if (mDev->set_master_mute == NULL) return INVALID_OPERATION;
    return mDev->set_master_mute(mDev, state);
}

status_t DeviceHalLocal::getMasterMute(bool *state) {
    if (mDev->get_master_mute == NULL) return INVALID_OPERATION;
    return mDev->get_master_mute(mDev, state);
}

status_t DeviceHalLocal::setParameters(const String8& kvPairs) {
    return mDev->set_parameters(mDev, kvPairs.string());
}

status_t DeviceHalLocal::getParameters(const String8& keys, String8 *values) {
    char *c_values = mDev->get_parameters(mDev, keys.string());
    if (c_values != NULL) {
        values->setTo(c_values);
        free(c_values);
    } else {
        values->clear();
    }
    return OK;
}

status_t DeviceHalLocal::getInputBufferSize(
        const struct audio_config *config, size_t *size) {
    *size = mDev->get_input_buffer_size(mDev, config);
    return OK;
}

status_t DeviceHalLocal::createAudioPatch(
        unsigned int num_sources,
        const struct audio_port_config *sources,
        unsigned int num_sinks,
        const struct audio_port_config *sinks,
        audio_patch_handle_t *patch) {
    return mDev->create_audio_patch(mDev, num_sources, sources, num_sinks, sinks, patch);
}

status_t DeviceHalLocal::releaseAudioPatch(audio_patch_handle_t patch) {
    return mDev->release_audio_patch(mDev, patch);
}

status_t DeviceHalLocal::getAudioPort(struct audio_port *port) {
    return mDev->get_audio_port(mDev, port);
}

status_t DeviceHalLocal::setAudioPortConfig(const struct audio_port_config *config) {
    return mDev->set_audio_port_config(mDev, config);
}

status_t DeviceHalLocal::dump(int fd) {
    return mDev->dump(mDev, fd);
}

status_t DeviceHalLocal::openOutputStream(
        audio_io_handle_t handle,
        audio_devices_t devices,
        audio_output_flags_t flags,
        struct audio_config *config,
        const char *address,
        struct audio_stream_out **stream_out) {
    return mDev->open_output_stream(mDev, handle, devices, flags, config, stream_out, address);
}

status_t DeviceHalLocal::closeOutputStream(struct audio_stream_out *stream_out) {
    mDev->close_output_stream(mDev, stream_out);
    return OK;
}

status_t DeviceHalLocal::openInputStream(
        audio_io_handle_t handle,
        audio_devices_t devices,
        struct audio_config *config,
        audio_input_flags_t flags,
        const char *address,
        audio_source_t source,
        struct audio_stream_in **stream_in) {
    return mDev->open_input_stream(
            mDev, handle, devices, config, stream_in, flags, address, source);
}

status_t DeviceHalLocal::closeInputStream(struct audio_stream_in *stream_in) {
    mDev->close_input_stream(mDev, stream_in);
    return OK;
}

} // namespace android
