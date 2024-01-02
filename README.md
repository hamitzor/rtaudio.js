
<h1>
  RtAudio.js
  <br>
</h1>

<h4>RtAudio binding for Node</h4>

<p>
  <a href="https://www.npmjs.com/package/@hamitzor/rtaudio.js">
    <img src="https://img.shields.io/badge/1.0.5-brightgreen?style=flat&label=npm%20package"
         alt="NPM">
  </a>
</p>

Node binding of the <a href="https://github.com/thestk/rtaudio">RtAudio</a> C++ audio library. To see a detailed description of RtAudio visit it's <a href="https://github.com/thestk/rtaudio">GitHub page</a>.

An overview of the binding:

* Access well-known audio I/O APIs
  - Windows: WASAPI and DirectSound
  - Linux: ALSA and PulseAudio
* Probe available audio devices
* Stream audio to output devices
* Stream audio from input devices
* Fully configurable audio streaming, allows configuring
  - sample rate
  - bit depth
  - frame size
  - number of channels
* No additional library/software needed, besides an npm install

## Installation

Since this is an addon written in C++ it needs to be built before used. Prebuilds are available for Node between 14.x.x - 21.x.x and Electron between 11.x.x - 28.x.x for x64 Linux and x64 Windows. This should cover quite a lot of audience but if you happen to use a different target, you will have to build the binding yourself (see below).
> **Note**
> Only Windows and Linux are supported at the moment.

#### Installing for Node 14.x.x - 21.x.x

Install it using `npm` or `yarn`

```
npm install @hamitzor/rtaudio.js
```
or
```
yarn add @hamitzor/rtaudio.js
```

#### Installing for Electron 11.x.x - 28.x.x

If you'll be using the package with Electron, you'll have to set some environment variables before the installation command.

```
export npm_config_runtime=electron
export npm_config_target=<your_electron_version>
```

These will help the install command to pick the correct prebuilds. After setting these up, you can simply use `npm` or `yarn`

```
npm install @hamitzor/rtaudio.js
```
or
```
yarn add @hamitzor/rtaudio.js
```

As simple as that, no additional library/software required for installation. If you run into trouble during installation, don't hesitate to create an issue at <a href="https://github.com/hamitzor/rtaudio.js/issues">Github</a>.

## Usage

This binding is pretty orthodox about staying loyal to the original RtAudio API, so using RtAudio's documentation and tutorials should mostly cover the usage of the binding as well. Almost all the method names and signatures are the same. There is a separate repository named <a href="https://github.com/hamitzor/rtaudio.js-examples">RtAudio.js examples</a> that demonstrates the usage of this binding with a couple of examples. Besides, here is a small example for a quick start:

```javascript
// An examples that echoes audio from default input to default output.

const { RtAudio, RtAudioFormat, RtAudioErrorType, RtAudioStreamStatus, RtAudioApi } = require('@hamitzor/rtaudio.js')

const rtAudio = new RtAudio()

const defaultInputDevice = rtAudio.getDefaultInputDevice()
const defaultOutputDevice = rtAudio.getDefaultOutputDevice()

// Create a stream
rtAudio.openStream(
  { deviceId: defaultOutputDevice, nChannels: 1 }, // output stream parameters
  { deviceId: defaultInputDevice, nChannels: 1 }, // input stream parameters
  RtAudioFormat.RTAUDIO_SINT16, // PCM format
  48000, // sample rate
  1920, // buffer size
  null,
  // A callback that will be invoked when input is ready and/or output is needed.
  (output, input, _nFrames, _time, status) => {
    // output and input are instances of Uint8Array
    // Write input directly to the output buffer, for echoing.
    output.set(input, 0)
  }
)

// Start the stream
rtAudio.start()

// On SIGINT, close the stream and exit
process.on('SIGINT', () => {
  rtAudio.closeStream()
  process.exit()
})

```

## Building from source
If you don't use Node between 14.x.x - 21.x.x or Electron between 11.x.x - 28.x.x, or your platform is not x64 Linux or x64 Windows, the npm install command will attempt to build the binding from source. In that case you'll need to satisfy some prerequisites:

- Your Node or Electron version should support N-API 4 and up (see <a href="https://nodejs.org/docs/latest/api/n-api.html#node-api-version-matrix">this</a>)
- CMake
- A proper C/C++ compiler toolchain
  - For Windows, MSVC should be enough
  - For Linux, GCC or Clang and make



## Credits

This package uses the C++ library named RtAudio under the hood. To check it out, visit https://github.com/thestk/rtaudio.

## License

<a href="https://raw.githubusercontent.com/hamitzor/rtaudio.js/master/LICENSE">MIT</a>
