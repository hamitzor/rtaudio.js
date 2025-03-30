<h1>
  RtAudio.js
  <br>
</h1>

<h4>RtAudio binding for Node</h4>

<p>
  <a href="https://www.npmjs.com/package/@hamitzor/rtaudio.js">
    <img src="https://img.shields.io/badge/2.2.0-brightgreen?style=flat&label=npm%20package"
         alt="NPM">
  </a>
</p>

Node binding of the <a href="https://github.com/thestk/rtaudio">RtAudio</a> C++ audio library. To see a detailed description of RtAudio visit it's <a href="https://www.music.mcgill.ca/~gary/rtaudio/">homepage</a> or <a href="https://github.com/thestk/rtaudio">Github repo</a>.

Features:
- Runs on Windows, Linux and macOS
- Probe available audio devices
- Stream audio to output devices
- Stream audio from input devices
- Fully configurable audio streaming, allows configuring
  - sample rate
  - bit depth
  - frame size
  - number of channels
- No additional library/software needed, besides an npm install

## Installation
Install it using `npm` or `yarn`

```
npm install @hamitzor/rtaudio.js
```

or

```
yarn add @hamitzor/rtaudio.js
```

#### Installing for Electron 11.x.x - 28.x.x

If you'll be using the package with Electron, you'll have to set some environment variables before the installation.

For example, for Electron v28.0.0

On bash:
```
export npm_config_runtime=electron
export npm_config_target=28.0.0
```

On powershell:
```
$env:npm_config_runtime = "electron"
$env:npm_config_target = "28.0.0"
```

On cmd:
```
set npm_config_runtime=electron
set npm_config_target=28.0.0
```

To see a complete list of Electron versions, see this [registry](https://github.com/electron/node-abi/blob/main/abi_registry.json).

These environment variables will help the installation command to pick the correct [prebuilds](https://github.com/hamitzor/rtaudio.js/releases/tag/v1.2.0). After setting these up, you can simply use `npm` or `yarn`

```
npm install @hamitzor/rtaudio.js
```

or

```
yarn add @hamitzor/rtaudio.js
```

As simple as that, no additional library/software required for installation. If you run into trouble during installation, don't hesitate to create an issue at <a href="https://github.com/hamitzor/rtaudio.js/issues">Github</a>.

## Documentation

You can find the <a href="https://hamitzor.github.io/rtaudio.js/">documentation here</a>, also there is an <a href="https://github.com/hamitzor/rtaudio.js-examples">RtAudio.js examples</a> repo that demonstrates a few use cases.

## Usage

This binding is pretty orthodox about staying loyal to the original RtAudio API, so using RtAudio's documentation and tutorials should mostly cover the usage of the binding as well. Almost all the method names and signatures are the same. There is a separate repository named <a href="https://github.com/hamitzor/rtaudio.js-examples">RtAudio.js examples</a> that demonstrates the usage of this binding with a couple of examples. Besides, here is a small example for a quick start:

```javascript
const {
  RtAudio,
  RtAudioFormat,
  RtAudioErrorType,
  RtAudioStreamStatus,
  RtAudioApi,
} = require("@hamitzor/rtaudio.js");

const rtAudio = new RtAudio();

const defaultInputDevice = rtAudio.getDefaultInputDevice();
const defaultOutputDevice = rtAudio.getDefaultOutputDevice();

// Create a stream
rtAudio.openStream(
  { deviceId: defaultOutputDevice, nChannels: 1 }, // output stream parameters
  { deviceId: defaultInputDevice, nChannels: 1 }, // input stream parameters
  RtAudioFormat.RTAUDIO_SINT16, // PCM format
  48000, // sample rate
  1920, // buffer size
  null,
  // A callback that will be invoked when input is ready and/or output is needed.
  (output, input, nFrames, time, status) => {
    // output and input are instances of Uint8Array of PCM samples.

    // To access individual PCM samples, you can convert Uint8Array
    // to the respective typed array. In this example it would be Int16Array
    // since we use RTAUDIO_SINT16:
    const data = Int16Array.from(input.buffer);
    data.at(0); // First samples
    data.at(1); // Second sample
    data.at(2); // Third sample
    data.at(3); // Fourth sample
    ...
    data.at(nFrames - 1); // Last sample

    // nFrames is most of the times the buffer size we provide
    // to the `openStream` function, i.e. 1920

    // If we've used 2 channels, the data would be twice in size and we would
    // access first and second channel's interleaved samples. For example:
    const data = Int16Array.from(input.buffer);
    data.at(0); // First sample of the first channel
    data.at(1); // First sample of the second channel
    data.at(2); // Second sample of the first channel
    data.at(3); // Second sample of the second channel
    ...
    data.at(nFrames * 2 - 2); // Last sample of the first channel
    data.at(nFrames * 2 - 1); // Last sample of the second channel

    // Note: RtAudio uses system's endianness.

    // For echoing, we can write input directly to the output buffer.
    output.set(input, 0);

    if (time > 2) {
      // stop the stream after 2 seconds
      // Avoid calling rtAudio.closeStream() in the callback function here
      return 1;
    }

    return 0; // keep the stream running
  }
);

// Start the stream
rtAudio.start();

// On SIGINT, close the stream and exit
process.on("SIGINT", () => {
  rtAudio.closeStream();
  process.exit();
});
```

## Building from source

If you don't use Node between 14.x.x - 21.x.x or Electron between 11.x.x - 28.x.x, or your platform is not x64 Linux or x64 Windows, the npm install command will attempt to build the binding from source. In that case you'll need to satisfy some prerequisites:

- Your Node or Electron version should support N-API 4 and up (see <a href="https://nodejs.org/docs/latest/api/n-api.html#node-api-version-matrix">this</a>)
- CMake
- A proper C/C++ compiler toolchain
  - For Windows, MSVC should be enough
  - For Linux, GCC or Clang and make
  - For macOS, regular Xcode stuff

## Credits

This package uses the C++ library named RtAudio under the hood. To check it out, visit https://github.com/thestk/rtaudio.

## License

<a href="https://raw.githubusercontent.com/hamitzor/rtaudio.js/master/LICENSE">MIT</a>
