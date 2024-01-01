/** Audio API specifier arguments */
export declare enum RtAudioApi {
  /** Search for a working compiled API. */
  UNSPECIFIED = 0,

  /** Macintosh OS-X Core Audio API. */
  MACOSX_CORE = 1,

  /** The Advanced Linux Sound Architecture API. */
  LINUX_ALSA = 2,

  /** The Linux PulseAudio API. */
  LINUX_PULSE = 4,

  /** The Microsoft WASAPI API. */
  WINDOWS_WASAPI = 7,

  /** The Microsoft DirectSound API. */
  WINDOWS_DS = 8,
}

/** 
 * RtAudio data format type.
 *
 * Support for signed integers and floats.  Audio data fed to/from an
 * RtAudio stream is assumed to ALWAYS be in host byte order.  The
 * internal routines will automatically take care of any necessary
 * byte-swapping between the host format and the soundcard.  Thus,
 * endian-ness is not a concern in the following format definitions.
 * Note that there are no range checks for floating-point values that
 * extend beyond plus/minus 1.0.
 */
export declare enum RtAudioFormat {
  /** 8-bit signed integer. */
  RTAUDIO_SINT8 = 0x1,

  /** 16-bit signed integer. */
  RTAUDIO_SINT16 = 0x2,

  /** 32-bit signed integer. */
  RTAUDIO_SINT32 = 0x8,

  /** Normalized between plus/minus 1.0. */
  RTAUDIO_FLOAT32 = 0x10,

  /** Normalized between plus/minus 1.0. */
  RTAUDIO_FLOAT64 = 0x20,
}

/** 
 * The following flags can be OR'ed together to allow a client to
 * make changes to the default stream behavior.
 * 
 * By default, RtAudio streams pass and receive audio data from the
 * client in an interleaved format.  By passing the
 * RTAUDIO_NONINTERLEAVED flag to the openStream() function, audio
 * data will instead be presented in non-interleaved buffers.  In
 * this case, each buffer argument in the RtAudioCallback function
 * will point to a single array of data, with \c nFrames samples for
 * each channel concatenated back-to-back.  For example, the first
 * sample of data for the second channel would be located at index \c
 * nFrames (assuming the \c buffer pointer was recast to the correct
 * data type for the stream).
 * 
 * Certain audio APIs offer a number of parameters that influence the
 * I/O latency of a stream.  By default, RtAudio will attempt to set
 * these parameters internally for robust (glitch-free) performance
 * (though some APIs, like Windows DirectSound, make this difficult).
 * By passing the RTAUDIO_MINIMIZE_LATENCY flag to the openStream()
 * function, internal stream settings will be influenced in an attempt
 * to minimize stream latency, though possibly at the expense of stream
 * performance.
 * 
 * If the RTAUDIO_HOG_DEVICE flag is set, RtAudio will attempt to
 * open the input and/or output stream device(s) for exclusive use.
 * Note that this is not possible with all supported audio APIs.
 * 
 * If the RTAUDIO_SCHEDULE_REALTIME flag is set, RtAudio will attempt 
 * to select realtime scheduling (round-robin) for the callback thread.
 */
export declare enum RtAudioStreamFlags {
  /** Use non-interleaved buffers (default = interleaved). */
  RTAUDIO_NONINTERLEAVED = 0x1,

  /** Attempt to set stream parameters for lowest possible latency. */
  RTAUDIO_MINIMIZE_LATENCY = 0x2,

  /** Attempt grab device and prevent use by others. */
  RTAUDIO_HOG_DEVICE = 0x4,

  /** Try to select realtime scheduling for callback thread. */
  RTAUDIO_SCHEDULE_REALTIME = 0x8,

  /** Use the "default" PCM device (ALSA only). */
  RTAUDIO_ALSA_USE_DEFAULT = 0x10,

  /** Do not automatically connect ports (JACK only). */
  RTAUDIO_JACK_DONT_CONNECT = 0x20,
}

/** RtAudio error types */
export declare enum RtAudioErrorType {
  /** A non-critical error. */
  WARNING,

  /** A non-critical error which might be useful for debugging. */
  DEBUG_WARNING,

  /** The default, unspecified error type. */
  UNSPECIFIED,

  /** No devices found on system. */
  NO_DEVICES_FOUND,

  /** An invalid device ID was specified. */
  INVALID_DEVICE,

  /** An error occurred during memory allocation. */
  MEMORY_ERROR,

  /** An invalid parameter was specified to a function. */
  INVALID_PARAMETER,

  /** The function was called incorrectly. */
  INVALID_USE,

  /** A system driver error occurred. */
  DRIVER_ERROR,

  /** A system error occurred. */
  SYSTEM_ERROR,

  /** A thread error occurred. */
  THREAD_ERROR,
}

/** 
 * Notification of a stream over- or underflow is indicated by a
 * non-zero stream \c status argument in the RtAudioCallback function.
 * The stream status can be one of the following two options,
 * depending on whether the stream is open for output and/or input:
 */
export declare enum RtAudioStreamStatus {
  /** Input data was discarded because of an overflow condition at the driver. */
  RTAUDIO_INPUT_OVERFLOW = 1,
  /** The output buffer ran low, likely producing a break in the output sound. */
  RTAUDIO_OUTPUT_UNDERFLOW = 2,
}

/** Stream options */
export declare type StreamOptions = {
  /** A bit-mask of stream flags (RTAUDIO_MINIMIZE_LATENCY, RTAUDIO_HOG_DEVICE, RTAUDIO_ALSA_USE_DEFAULT). */
  flags?: number

  /** Number of stream buffers. */
  numberOfBuffers?: number

  /** A stream name (currently used only in Jack). */
  streamName?: string

  /** Scheduling priority of callback thread (only used with flag RTAUDIO_SCHEDULE_REALTIME). */
  priority?: number
}

/** The public device information structure for returning queried values. */
export declare interface DeviceInfo {
  /** Unique numeric device identifier. */
  id: number;

  /** Character string device identifier. */
  name: string;

  /** Maximum output channels supported by device. */
  outputChannels: number;

  /** Maximum input channels supported by device. */
  inputChannels: number;

  /** Maximum simultaneous input/output channels supported by device. */
  duplexChannels: number;

  /** Is the device the default output device */
  isDefaultOutput: number;

  /** Is the device the default input device */
  isDefaultInput: number;

  /** Supported sample rates (queried from list of standard rates). */
  sampleRates: Array<number>;

  /** Preferred sample rate, e.g. for WASAPI the system sample rate. */
  preferredSampleRate: number;

  /** Bit mask of supported data formats. */
  nativeFormats: number;
}

/** The structure for specifying stream parameters. */
export declare interface StreamParameters {
  /**
   * Device id. Can be obtained using `defaultInputDevice`/`defaultOutputDevice` or using `devices`.
   */
  deviceId: number;

  /** Number of channels. */
  nChannels: number;

  /** First channel index on device (default = 0). */
  firstChannel?: number;
}

export declare class RtAudio {
  constructor()
  constructor(api: RtAudioApi)

  setErrorCallback: (errorCallback: (errType: RtAudioErrorType, message: string) => void) => void
  showWarnings: (v: boolean) => void
  openStream: (
    outputParameters: StreamParameters | null,
    inputParameters: StreamParameters | null,
    format: RtAudioFormat,
    sampleRate: number,
    bufferFrames: number,
    options: StreamOptions | null,
    callback: (output: Uint8Array, input: Uint8Array, nFrames: number, streamTime: number, status: RtAudioStreamStatus) => void,
  ) => void
  closeStream: () => void
  startStream: () => void
  abortStream: () => void
  getDevices: () => DeviceInfo[]
  getDefaultInputDevice: () => number
  getDefaultOutputDevice: () => number
  isStreamOpen: () => boolean
  isStreamRunning: () => boolean
  getCurrentApi: () => number
  getStreamLatency: () => number
  getStreamSampleRate: () => number
  getStreamTime: () => number
  setStreamTime: () => number
  getErrorText: () => string

  static getVersion: () => string
  static getCompiledApi: () => RtAudioApi[]
  static getApiDisplayName(api: RtAudioApi): string
  static getApiName(api: RtAudioApi): string
}