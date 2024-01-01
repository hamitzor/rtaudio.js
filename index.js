"use strict"

const bindings = require('bindings')

let RtAudio

if (process.platform === 'win32') {
  RtAudio = bindings('rtaudio-js-win32').NodeRtAudio
} else if (process.platform === 'linux') {
  try {
    RtAudio = bindings('rtaudio-js-linux-custom').NodeRtAudio
  } catch (_err) {
    try {
      RtAudio = bindings('rtaudio-js-linux-pulse-alsa.node').NodeRtAudio
    } catch (_err) {
      RtAudio = bindings('rtaudio-js-linux-alsa.node').NodeRtAudio
    }
  }
} else {
  throw 'RtAudio.js supports Linux and Windows only.'
}

module.exports.RtAudio = RtAudio

/** The native API to use */
module.exports.LowLevelAudioApi = {
  /** Search for a working compiled API. */
  UNSPECIFIED: 0,

  /** Macintosh OS-X Core Audio API. */
  MACOSX_CORE: 1,

  /** The Advanced Linux Sound Architecture API. */
  LINUX_ALSA: 2,

  /** The Linux PulseAudio API. */
  LINUX_PULSE: 4,

  /** The Microsoft WASAPI API. */
  WINDOWS_WASAPI: 7,

  /** The Microsoft DirectSound API. */
  WINDOWS_DS: 8,
}

/** The format of the PCM data. */
module.exports.PCMFormat = {
  /** 8-bit signed integer. */
  RTAUDIO_SINT8: 0x1,

  /** 16-bit signed integer. */
  RTAUDIO_SINT16: 0x2,

  /** 32-bit signed integer. */
  RTAUDIO_SINT32: 0x8,

  /** Normalized between plus/minus 1.0. */
  RTAUDIO_FLOAT32: 0x10,

  /** Normalized between plus/minus 1.0. */
  RTAUDIO_FLOAT64: 0x20,
}

/** Flags that change the default stream behavior */
module.exports.StreamFlag = {
  /** Use non-interleaved buffers (default = interleaved). */
  RTAUDIO_NONINTERLEAVED: 0x1,

  /** Attempt to set stream parameters for lowest possible latency. */
  RTAUDIO_MINIMIZE_LATENCY: 0x2,

  /** Attempt grab device and prevent use by others. */
  RTAUDIO_HOG_DEVICE: 0x4,

  /** Try to select realtime scheduling for callback thread. */
  RTAUDIO_SCHEDULE_REALTIME: 0x8,

  /** Use the "default" PCM device (ALSA only). */
  RTAUDIO_ALSA_USE_DEFAULT: 0x10,

  /** Do not automatically connect ports (JACK only). */
  RTAUDIO_JACK_DONT_CONNECT: 0x20,
}

/** RtAudio error types */
module.exports.ErrorType = {
  /** A non-critical error. */
  WARNING: 0,

  /** A non-critical error which might be useful for debugging. */
  DEBUG_WARNING: 1,

  /** The default, unspecified error type. */
  UNSPECIFIED: 2,

  /** No devices found on system. */
  NO_DEVICES_FOUND: 3,

  /** An invalid device ID was specified. */
  INVALID_DEVICE: 4,

  /** An error occurred during memory allocation. */
  MEMORY_ERROR: 5,

  /** An invalid parameter was specified to a function. */
  INVALID_PARAMETER: 6,

  /** The function was called incorrectly. */
  INVALID_USE: 7,

  /** A system driver error occurred. */
  DRIVER_ERROR: 8,

  /** A system error occurred. */
  SYSTEM_ERROR: 9,

  /** A thread error occurred. */
  THREAD_ERROR: 10,
}
