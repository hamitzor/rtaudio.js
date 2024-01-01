
<h1>
  RtAudio.js
  <br>
</h1>

<h4>RtAudio binding for Node</h4>

<p>
  <a href="https://www.npmjs.com/package/@hamitzor/rtaudio.js">
    <img src="https://img.shields.io/badge/1.0.5-green?style=flat&label=npm%20package"
         alt="NPM">
  </a>
</p>

Node binding of the <a href="https://github.com/thestk/rtaudio">RtAudio</a> C++ audio library. To see a detailed description of RtAudio visit it's <a href="https://github.com/thestk/rtaudio">GitHub page</a>.

An overview of this binding:

* Access well-known audio I/O APIs
  - Windows: WASAPI and DirectSound
  - Linux: ALSA and PulseAudio
* No additional library/software needed
  - Direct access to the aforementioned APIs through C++
* Prebuilds are available, so don't worry about building it yourself
  - A single `npm install` should get you ready to go
* Supports Node.js versions >= 18.19.0
* Probe available audio devices
* Stream audio to output devices
* Stream audio from input devices
* Fully configurable audio streaming, allows configuring
  - sample rate
  - bit depth
  - frame size
  - number of channels

## Installation

Install it using `npm` or `yarn`

```
npm install @hamitzor/rtaudio.js
```
or
```
yarn add @hamitzor/rtaudio.js
```

As simple as that, no additional library/software required for installation. If you run into trouble during installation, don't hesitate to create an issue at <a href="https://github.com/hamitzor/rtaudio.js/issues">Github</a>.

> **Note**
> Only Windows and Linux are supported at the moment.

## Credits

This packages uses the C++ library named rtaudio under the hood. To check it out, visit https://github.com/thestk/rtaudio.

## License

<a href="https://raw.githubusercontent.com/hamitzor/rtaudio.js/master/LICENSE">MIT</a>
