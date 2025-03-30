/**
 * The RtAudio class.
 * 
 * If an API argument is specified but that API isn't supported,
 * a warning is issued and an instance of an available API
 * is created. If available API is found, the routine will abort
 * If no API argument is specified and multiple API
 * support has been compiled, the default order of use is ALSA,
 * (Linux systems) and WASAPI, DS (Windows systems).
 */
export declare class RtAudio {
  /**
   * Create a new RtAudio instance
   */
  constructor()

  /**
   * Create a new RtAudio instance
   * 
   * @param api api to utilize. If omitted, an available one will be picked.
   */
  constructor(api: RtAudioApi)

  /** Set a client-defined function that will be invoked when an error or warning occurs 
   * 
   * @param errorCallback the callback
  */
  setErrorCallback(errorCallback: (errType: RtAudioErrorType, message: string) => void): void

  /**
   * Specify whether warning messages should be output or not.
   *
   * The default behaviour is for warning messages to be output,
   * either to a client-defined error callback function (if specified)
   * or to stderr.
   * 
   * @param value boolean value to use
   */
  showWarnings(value: boolean): void

  /**
   * A function for opening a stream with the specified parameters.
   * 
   * @param outputParameters Specifies output stream parameters to use
   * when opening a stream, including a device ID, number of channels,
   * and starting channel number.  For input-only streams, this
   * argument should be null.
   * @param inputParameters Specifies input stream parameters to use
   * when opening a stream, including a device ID, number of channels,
   * and starting channel number. For output-only streams, this
   * argument should be null.
   * @param format An RtAudioFormat specifying the desired sample data format.
   * @param sampleRate The desired sample rate (sample frames per second).
   * @param bufferFrames A value indicating the desired
   * internal buffer size in sample frames. The actual value
   * used by the device is returned from `openStream` call.
   * A value of zero can be specified, in which case the lowest
   * allowable value is determined.
   * @param options An optional object containing various
   * global stream options, including a list of OR'ed RtAudioStreamFlags
   * and a suggested number of stream buffers that can be used to 
   * control stream latency. More buffers typically result in more
   * robust performance, though at a cost of greater latency.  If a
   * value of zero is specified, a system-specific median value is
   * chosen. If the RTAUDIO_MINIMIZE_LATENCY flag bit is set, the
   * lowest allowable value is used. The actual value used is
   * returned from `openStream` call. The parameter is API dependent.
   * 
   * @param callback A function that will be invoked
   * when input data is available and/or output data is needed.
   * 
   * @returns The actual bufferFrames value used by the device.
   */
  openStream(
    outputParameters: StreamParameters | null,
    inputParameters: StreamParameters | null,
    format: RtAudioFormat,
    sampleRate: number,
    bufferFrames: number,
    options: StreamOptions | null,
    callback: RtAudioCallback,
  ): number

  /**
   * A function that closes a stream and frees any associated stream memory.
   * 
   * If a stream is not open, an RTAUDIO_WARNING will be passed to the
   * user-provided errorCallback function (or otherwise printed to stderr).
   * 
   * This function should **never** be called in the callback function provided to
   * the stream in `openStream`.
   */
  closeStream(): void

  /**
   * A function that starts a stream.
   * 
   * An RTAUDIO_SYSTEM_ERROR is returned if an error occurs during
   * processing. An RTAUDIO_WARNING is returned if a stream is not open
   * or is already running.
   */
  startStream(): void

  /**
   * Stop a stream, allowing any samples remaining in the output queue to be played.
   * 
   * An RTAUDIO_SYSTEM_ERROR is returned if an error occurs during
   * processing. An RTAUDIO_WARNING is returned if a stream is not
   * open or is already stopped.
   */
  stopStream(): void

  /**
   * Stop a stream, discarding any samples remaining in the input/output queue.
   * 
   * An RTAUDIO_SYSTEM_ERROR is returned if an error occurs during
   * processing. An RTAUDIO_WARNING is returned if a stream is not
   * open or is already stopped.
   */
  abortStream(): void

  /**
   * A public function that returns a vector of audio devices.
   * 
   * The ID values returned by this function are used internally by
   * RtAudio to identify a given device. The values themselves are
   * arbitrary and do not correspond to device IDs used by the
   * underlying API (nor are they index values). This function performs
   * a system query of available devices each time it is called, thus
   * supporting devices (dis)connected after instantiation. If no
   * devices are available, the vector size will be zero. If a system
   * error occurs during processing, a warning will be issued.
   */
  getDevices(): DeviceInfo[]

  /**
   * A function that returns the ID of the default input device.
   * 
   * If the underlying audio API does not provide a "default device",
   * the first probed input device ID will be returned. If no devices
   * are available, the return value will be 0 (which is an invalid
   * device identifier).
   */
  getDefaultInputDevice(): number

  /**
   * A function that returns the ID of the default output device.
   * 
   * If the underlying audio API does not provide a "default device",
   * the first probed output device ID will be returned. If no devices
   * are available, the return value will be 0 (which is an invalid
   * device identifier).
   */
  getDefaultOutputDevice(): number

  /** Returns true if a stream is open and false if not. */
  isStreamOpen(): boolean

  /** Returns true if the stream is running and false if it is stopped or not open. */
  isStreamRunning(): boolean

  /** Returns the audio API specifier for the current instance of RtAudio. */
  getCurrentApi(): number

  /**
   * Returns the internal stream latency in sample frames.
   * 
   * The stream latency refers to delay in audio input and/or output
   * caused by internal buffering by the audio system and/or hardware.
   * For duplex streams, the returned value will represent the sum of
   * the input and output latencies.  If a stream is not open, the
   * returned value will be invalid.  If the API does not report
   * latency, the return value will be zero.
   */
  getStreamLatency(): number

  /**
   * Returns actual sample rate in use by the (open) stream.
   * 
   * On some systems, the sample rate used may be slightly different
   * than that specified in the stream parameters.  If a stream is not
   * open, a value of zero is returned.
   */
  getStreamSampleRate(): number

  /**
   * Returns the number of seconds of processed data since the stream was started.
   * 
   * The stream time is calculated from the number of sample frames
   * processed by the underlying audio system, which will increment by
   * units of the audio buffer size. It is not an absolute running
   * time. If a stream is not open, the returned value may not be
   * valid.
   */
  getStreamTime(): number

  /** Set the stream time to a time in seconds greater than or equal to 0.0. */
  setStreamTime(): number

  /**
   * Retrieve the error message corresponding to the last error or warning condition.
   * 
   * This function can be used to get a detailed error message when a
   * non-zero RtAudioErrorType is returned by a function. This is the
   * same message sent to the user-provided errorCallback function.
   */
  getErrorText(): string

  /** A static function to determine the current RtAudio version. */
  static getVersion(): string

  /**
   * A static function to determine the available compiled audio APIs.
   */
  static getCompiledApi(): RtAudioApi[]

  /**
   * Return the display name of a specified compiled audio API.
   * 
   * This obtains a long name used for display purposes.
   * If the API is unknown, this function will return the empty string.
   * 
   * @param api the API id
   */
  static getApiDisplayName(api: RtAudioApi): string

  /**
   * Return the name of a specified compiled audio API.
   * 
   * This obtains a short lower-case name used for identification purposes.
   * This value is guaranteed to remain identical across library versions.
   * If the API is unknown, this function will return the empty string.
   * 
   * @param api the API id
   */
  static getApiName(api: RtAudioApi): string
}

/** Audio API specifier arguments */
export declare enum RtAudioApi {
  /** Search for a working compiled API. */
  UNSPECIFIED = 0,

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
  /** A bit-mask of stream flags {@link RtAudioStreamFlags} */
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

/**
 * A function that will be invoked when input data is 
 * available and/or output data is needed.
 * 
 * @param output For output (or duplex) streams, the client
 * should write `nFrames` of audio sample frames into this
 * buffer. For input-only streams, this argument will be null.
 * 
 * @param input For input (or duplex) streams, this buffer will
 * hold `nFrames` of input audio sample frames. For output-only streams, this argument
 * will be null.
 * 
 * @param nFrames The number of sample frames of input or output
 * data in the buffers. The actual buffer size in bytes is
 * dependent on the `format` and number of channels in use.
 * 
 * @param streamTime The number of seconds that have elapsed since the
 * stream was started.
 * 
 * @param status If non-zero, this argument indicates a data overflow
 * or underflow condition for the stream. The particular
 * condition can be determined by comparison with the
 * RtAudioStreamStatus flags.
 * 
 * @returns this function should return either zero or undefined to keep the stream running
 * or a non-zero value to stop the stream.
 * 
 */
export declare type RtAudioCallback =
  (
    output: Uint8Array,
    input: Uint8Array,
    nFrames: number,
    streamTime: number,
    status: RtAudioStreamStatus) => number | undefined