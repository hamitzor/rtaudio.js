#include <iostream>
#include <algorithm>
#include "node_rtaudio.hpp"

Napi::Object NodeRtAudio::Init(Napi::Env env, Napi::Object exports)
{
  Napi::Function func = DefineClass(
      env,
      "NodeRtAudio",
      {
          InstanceMethod<&NodeRtAudio::setErrorCallback>("setErrorCallback", static_cast<napi_property_attributes>(napi_default)),
          InstanceMethod<&NodeRtAudio::showWarnings>("showWarnings", static_cast<napi_property_attributes>(napi_default)),
          InstanceMethod<&NodeRtAudio::openStream>("openStream", static_cast<napi_property_attributes>(napi_default)),
          InstanceMethod<&NodeRtAudio::closeStream>("closeStream", static_cast<napi_property_attributes>(napi_default)),
          InstanceMethod<&NodeRtAudio::startStream>("startStream", static_cast<napi_property_attributes>(napi_default)),
          InstanceMethod<&NodeRtAudio::stopStream>("stopStream", static_cast<napi_property_attributes>(napi_default)),
          InstanceMethod<&NodeRtAudio::abortStream>("abortStream", static_cast<napi_property_attributes>(napi_default)),
          InstanceMethod<&NodeRtAudio::getDevices>("getDevices", static_cast<napi_property_attributes>(napi_default)),
          InstanceMethod<&NodeRtAudio::getDefaultInputDevice>("getDefaultInputDevice", static_cast<napi_property_attributes>(napi_default)),
          InstanceMethod<&NodeRtAudio::getDefaultOutputDevice>("getDefaultOutputDevice", static_cast<napi_property_attributes>(napi_default)),
          InstanceMethod<&NodeRtAudio::isStreamOpen>("isStreamOpen", static_cast<napi_property_attributes>(napi_default)),
          InstanceMethod<&NodeRtAudio::isStreamRunning>("isStreamRunning", static_cast<napi_property_attributes>(napi_default)),
          InstanceMethod<&NodeRtAudio::getCurrentApi>("getCurrentApi", static_cast<napi_property_attributes>(napi_default)),
          InstanceMethod<&NodeRtAudio::getStreamLatency>("getStreamLatency", static_cast<napi_property_attributes>(napi_default)),
          InstanceMethod<&NodeRtAudio::getStreamSampleRate>("getStreamSampleRate", static_cast<napi_property_attributes>(napi_default)),
          InstanceMethod<&NodeRtAudio::getStreamTime>("getStreamTime", static_cast<napi_property_attributes>(napi_default)),
          InstanceMethod<&NodeRtAudio::setStreamTime>("setStreamTime", static_cast<napi_property_attributes>(napi_default)),
          InstanceMethod<&NodeRtAudio::getErrorText>("getErrorText", static_cast<napi_property_attributes>(napi_default)),
          StaticMethod<&NodeRtAudio::getVersion>("getVersion", static_cast<napi_property_attributes>(napi_default)),
          StaticMethod<&NodeRtAudio::getCompiledApi>("getCompiledApi", static_cast<napi_property_attributes>(napi_default)),
          StaticMethod<&NodeRtAudio::getApiDisplayName>("getApiDisplayName", static_cast<napi_property_attributes>(napi_default)),
          StaticMethod<&NodeRtAudio::getApiName>("getApiName", static_cast<napi_property_attributes>(napi_default)),
      });

  Napi::FunctionReference *constructor = new Napi::FunctionReference();

  *constructor = Napi::Persistent(func);

  exports.Set("NodeRtAudio", func);

  return exports;
}

NodeRtAudio::NodeRtAudio(const Napi::CallbackInfo &info) : RtAudio(parseApi(info.Env(), info[0])), Napi::ObjectWrap<NodeRtAudio>(info), rtThreadSmph{0}, jsThreadSmph{0}
{
}

NodeRtAudio::~NodeRtAudio()
{
  if (tsCb.operator napi_threadsafe_function() != nullptr)
  {
    tsCb.Abort();
    tsCb.Release();
    tsCb.Unref(this->Env());
  }

  if (tsErrorCb.operator napi_threadsafe_function() != nullptr)
  {
    tsErrorCb.Abort();
    tsErrorCb.Release();
    tsErrorCb.Unref(this->Env());
  }
}

Napi::Value NodeRtAudio::getDevices(const Napi::CallbackInfo &info)
{
  std::vector<RtAudio::DeviceInfo> devicesV;

  const std::vector<unsigned int> deviceIds = this->getDeviceIds();

  for (const auto &deviceId : deviceIds)
  {
    devicesV.push_back(this->getDeviceInfo(deviceId));
  }

  Napi::Array devicesArray = Napi::Array::New(info.Env(), devicesV.size());

  for (unsigned int i = 0; i < devicesV.size(); i++)
  {
    Napi::Object infoObj = Napi::Object::New(info.Env());

    auto deviceInfo = devicesV[i];

    infoObj.Set("id", deviceInfo.ID);
    infoObj.Set("name", deviceInfo.name);
    infoObj.Set("outputChannels", deviceInfo.outputChannels);
    infoObj.Set("inputChannels", deviceInfo.inputChannels);
    infoObj.Set("duplexChannels", deviceInfo.duplexChannels);
    infoObj.Set("isDefaultOutput", deviceInfo.isDefaultOutput);
    infoObj.Set("isDefaultInput", deviceInfo.isDefaultInput);

    auto sampleRates = Napi::Array::New(info.Env(), deviceInfo.sampleRates.size());
    for (unsigned int i = 0; i < deviceInfo.sampleRates.size(); i++)
    {
      sampleRates[i] = deviceInfo.sampleRates[i];
    }

    infoObj.Set("sampleRates", sampleRates);
    infoObj.Set("preferredSampleRate", deviceInfo.preferredSampleRate);
    infoObj.Set("nativeFormats", deviceInfo.nativeFormats);

    devicesArray[i] = infoObj;
  }

  return devicesArray;
}

Napi::Value NodeRtAudio::getDefaultInputDevice(const Napi::CallbackInfo &info)
{
  return Napi::Number::New(info.Env(), RtAudio::getDefaultInputDevice());
}

Napi::Value NodeRtAudio::getDefaultOutputDevice(const Napi::CallbackInfo &info)
{
  return Napi::Number::New(info.Env(), RtAudio::getDefaultOutputDevice());
}

Napi::Value NodeRtAudio::isStreamOpen(const Napi::CallbackInfo &info)
{
  return Napi::Boolean::New(info.Env(), RtAudio::isStreamOpen());
}

Napi::Value NodeRtAudio::isStreamRunning(const Napi::CallbackInfo &info)
{
  return Napi::Boolean::New(info.Env(), RtAudio::isStreamRunning());
}

Napi::Value NodeRtAudio::getCurrentApi(const Napi::CallbackInfo &info)
{
  return Napi::Number::New(info.Env(), RtAudio::getCurrentApi());
}

Napi::Value NodeRtAudio::getStreamLatency(const Napi::CallbackInfo &info)
{
  return Napi::Number::New(info.Env(), RtAudio::getStreamLatency());
}

Napi::Value NodeRtAudio::getStreamSampleRate(const Napi::CallbackInfo &info)
{
  return Napi::Number::New(info.Env(), RtAudio::getStreamSampleRate());
}

Napi::Value NodeRtAudio::getStreamTime(const Napi::CallbackInfo &info)
{
  return Napi::Number::New(info.Env(), RtAudio::getStreamTime());
}

void NodeRtAudio::setStreamTime(const Napi::CallbackInfo &info)
{
  if (!info[0].IsNumber())
  {
    throw Napi::TypeError::New(info.Env(), "time should be a number.");
  }

  RtAudio::setStreamTime(info[0].As<Napi::Number>().DoubleValue());
}

Napi::Value NodeRtAudio::getErrorText(const Napi::CallbackInfo &info)
{
  return Napi::String::New(info.Env(), RtAudio::getErrorText());
}

void NodeRtAudio::setErrorCallback(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (!info[0].IsFunction())
    throw Napi::Error::New(env, "errorCallback should be a function");

  this->tsErrorCb = Napi::ThreadSafeFunction::New(env, info[0].As<Napi::Function>(), "errorCallback", 0, 1);

  RtAudioErrorCallback errorCallback{
      [this](RtAudioErrorType type, const std::string &errorText)
      {
        this->tsErrorCb.NonBlockingCall(
            [type, errorText](Napi::Env env, Napi::Function callback)
            {
              try
              {
                callback.Call({Napi::Number::New(env, type), Napi::String::New(env, errorText)});
              }
              catch (const std::exception &err)
              {
                std::cerr << err.what() << std::endl;
                return;
              }
            });
      }};

  RtAudio::setErrorCallback(errorCallback);
}

void NodeRtAudio::showWarnings(const Napi::CallbackInfo &info)
{
  if (!info[0].IsBoolean())
  {
    throw Napi::TypeError::New(info.Env(), "value should be a boolean.");
  }

  RtAudio::showWarnings(info[0].As<Napi::Boolean>());
}

Napi::Value NodeRtAudio::openStream(const Napi::CallbackInfo &info)
{
  jsRef = Napi::ObjectReference::New(this->Value(), 1);

  Napi::Env env = info.Env();
  RtAudio::StreamParameters *outputParamsPtr = nullptr;
  RtAudio::StreamParameters *inputParamsPtr = nullptr;
  RtAudio::StreamOptions *optionsPtr = nullptr;

  if (RtAudio::isStreamOpen())
    throw Napi::Error::New(env, "Stream already open");

  Napi::Function cb;

  if (!info[0].IsNull())
  {
    parseOutputParams(env, info[0], &this->outputParams);

    if (!deviceExists(this->outputParams.deviceId, this->getDeviceIds()))
      throw Napi::TypeError::New(env, "Output device doesn't exist.");

    outputParamsPtr = &this->outputParams;
  }

  if (!info[1].IsNull())
  {
    parseInputParams(env, info[1], &this->inputParams);

    if (!deviceExists(this->inputParams.deviceId, this->getDeviceIds()))
      throw Napi::TypeError::New(env, "Input device doesn't exist.");

    inputParamsPtr = &this->inputParams;
  }

  if (!info[2].IsNumber())
    throw Napi::Error::New(env, "format should be a valid number");

  if (!info[3].IsNumber())
    throw Napi::Error::New(env, "sampleRate should be a valid number");

  if (!info[4].IsNumber())
    throw Napi::Error::New(env, "bufferFrames should be a valid number");

  if (!info[5].IsNull())
  {
    parseStreamOptions(env, info[5], &this->options);
    optionsPtr = &this->options;
  }

  if (!info[6].IsFunction())
    throw Napi::Error::New(env, "callback should be a function");

  this->format = info[2].As<Napi::Number>().Int32Value();
  this->sampleRate = info[3].As<Napi::Number>().Int32Value();
  this->bufferFrames = info[4].As<Napi::Number>().Int32Value();

  this->tsCb = Napi::ThreadSafeFunction::New(env, info[6].As<Napi::Function>(), "callback", 1, 1);

  RtAudioCallback callback{
      [](void *outputBuffer, void *inputBuffer, unsigned int nFrames, double streamTime, RtAudioStreamStatus status, void *userData)
      {
        NodeRtAudio *that = (NodeRtAudio *)userData;

        that->rtThreadSmph.release();

        that->tsCb.BlockingCall(
            [nFrames, streamTime, status, that, outputBuffer, inputBuffer](Napi::Env env, Napi::Function callback)
            {
              that->rtThreadSmph.acquire();
              unsigned int sampleSize = getFormatByteSize(that->format);
              unsigned int outputByteCount = that->outputParams.nChannels * nFrames * sampleSize;
              unsigned int inputByteCount = that->inputParams.nChannels * nFrames * sampleSize;

              Napi::Reference<Napi::Uint8Array> output;
              Napi::Reference<Napi::Uint8Array> input;

              if (outputBuffer != nullptr)
              {
                output.Reset(Napi::Uint8Array::New(env, outputByteCount), 1);
                memset(output.Value().Data(), 0, outputByteCount);
              }

              if (inputBuffer != nullptr)
              {
                input.Reset(Napi::Uint8Array::New(env, inputByteCount), 1);
                memcpy(input.Value().Data(), inputBuffer, inputByteCount);
              }

              try
              {
                callback.Call(
                    {outputBuffer == nullptr ? env.Null() : output.Value(),
                     inputBuffer == nullptr ? env.Null() : input.Value(),
                     Napi::Number::New(env, nFrames),
                     Napi::Number::New(env, streamTime),
                     Napi::Number::New(env, status)});
              }
              catch (const std::exception &err)
              {
                std::cerr << err.what() << std::endl;

                if (outputBuffer != nullptr)
                {
                  output.Unref();
                }

                if (inputBuffer != nullptr)
                {
                  input.Unref();
                }

                return;
              }

              if (outputBuffer != nullptr)
              {
                memcpy(outputBuffer, output.Value().Data(), outputByteCount);
              }

              if (outputBuffer != nullptr)
              {
                output.Unref();
              }

              if (inputBuffer != nullptr)
              {
                input.Unref();
              }

              that->jsThreadSmph.release();
            });

        that->jsThreadSmph.acquire();

        return 0;
      }};

  RtAudio::openStream(
      outputParamsPtr,
      inputParamsPtr,
      this->format,
      this->sampleRate,
      &this->bufferFrames,
      callback,
      this,
      optionsPtr);

  return Napi::Number::New(env, this->bufferFrames);
}

void NodeRtAudio::closeStream(const Napi::CallbackInfo &info)
{
  jsRef.Unref();
  RtAudio::closeStream();
}

void NodeRtAudio::startStream(const Napi::CallbackInfo &info)
{
  RtAudio::startStream();
}

void NodeRtAudio::stopStream(const Napi::CallbackInfo &info)
{
  RtAudio::stopStream();
}

void NodeRtAudio::abortStream(const Napi::CallbackInfo &info)
{
  RtAudio::abortStream();
}

Napi::Value NodeRtAudio::getApiDisplayName(const Napi::CallbackInfo &info)
{
  return Napi::String::New(info.Env(), RtAudio::getApiDisplayName(NodeRtAudio::parseApi(info.Env(), info[0])));
}

Napi::Value NodeRtAudio::getApiName(const Napi::CallbackInfo &info)
{
  return Napi::String::New(info.Env(), RtAudio::getApiName(NodeRtAudio::parseApi(info.Env(), info[0])));
}

Napi::Value NodeRtAudio::getVersion(const Napi::CallbackInfo &info)
{
  return Napi::String::New(info.Env(), RtAudio::getVersion());
}

Napi::Value NodeRtAudio::getCompiledApi(const Napi::CallbackInfo &info)
{
  std::vector<RtAudio::Api> apis;

  RtAudio::getCompiledApi(apis);

  auto sampleRates = Napi::Array::New(info.Env(), apis.size());
  for (unsigned int i = 0; i < apis.size(); i++)
  {
    sampleRates[i] = Napi::Number::New(info.Env(), apis[i]);
  }

  return sampleRates;
}

void NodeRtAudio::NodeRtAudio::parseOutputParams(Napi::Env env, const Napi::Value &val, RtAudio::StreamParameters *params)
{
  if (!val.IsObject())
  {
    throw Napi::TypeError::New(env, "outputParams should be an object or null.");
  }

  Napi::Object obj = val.As<Napi::Object>();

  if (!obj.Get("deviceId").IsNumber() || obj.Get("deviceId").As<Napi::Number>().Int32Value() < 1)
  {
    throw Napi::TypeError::New(env, "outputParams.deviceId should be a number greater than 0. If you don't intend to use an output stream, set outputParams to null");
  }

  params->deviceId = obj.Get("deviceId").As<Napi::Number>().Int32Value();

  if (!obj.Get("nChannels").IsNumber() || obj.Get("nChannels").As<Napi::Number>().Int32Value() < 1)
  {
    throw Napi::TypeError::New(env, "outputParams.nChannels should be a number greater than 0. If you don't intend to use an output stream, set outputParams to null");
  }

  params->nChannels = obj.Get("nChannels").As<Napi::Number>().Int32Value();

  if (!obj.Get("firstChannel").IsUndefined())
  {
    if (!obj.Get("firstChannel").IsNumber())
    {
      throw Napi::TypeError::New(env, "outputParams.firstChannel should be a number.");
    }

    params->firstChannel = obj.Get("firstChannel").As<Napi::Number>().Int32Value();
  }
}

void NodeRtAudio::parseInputParams(Napi::Env env, const Napi::Value &val, RtAudio::StreamParameters *params)
{
  if (!val.IsObject())
  {
    throw Napi::TypeError::New(env, "inputParams should be an object or null.");
  }

  Napi::Object obj = val.As<Napi::Object>();

  if (!obj.Get("deviceId").IsNumber() || obj.Get("deviceId").As<Napi::Number>().Int32Value() < 1)
  {
    throw Napi::TypeError::New(env, "inputParams.deviceId should be a number greater than 0. If you don't intend to use an input stream, set outputParams to null");
  }

  params->deviceId = obj.Get("deviceId").As<Napi::Number>().Int32Value();

  if (!obj.Get("nChannels").IsNumber() || obj.Get("nChannels").As<Napi::Number>().Int32Value() < 1)
  {
    throw Napi::TypeError::New(env, "inputParams.nChannels should be a number greater than 0. If you don't intend to use an input stream, set outputParams to null");
  }

  params->nChannels = obj.Get("nChannels").As<Napi::Number>().Int32Value();

  if (!obj.Get("firstChannel").IsUndefined())
  {
    if (!obj.Get("firstChannel").IsNumber())
    {
      throw Napi::TypeError::New(env, "inputParams.firstChannel should be a number.");
    }
    params->firstChannel = obj.Get("firstChannel").As<Napi::Number>().Int32Value();
  }
}

void NodeRtAudio::parseStreamOptions(Napi::Env env, const Napi::Value &val, RtAudio::StreamOptions *params)
{
  if (!val.IsObject())
  {
    throw Napi::TypeError::New(env, "options should be an object or null.");
  }

  Napi::Object obj = val.As<Napi::Object>();

  if (!obj.Get("flags").IsUndefined())
  {
    if (!obj.Get("flags").IsNumber())
    {
      throw Napi::TypeError::New(env, "options.flags should be a number.");
    }

    params->flags = obj.Get("flags").As<Napi::Number>().Uint32Value();
  }

  if (!obj.Get("numberOfBuffers").IsUndefined())
  {
    if (!obj.Get("numberOfBuffers").IsNumber())
    {
      throw Napi::TypeError::New(env, "options.numberOfBuffers should be a number.");
    }

    params->numberOfBuffers = obj.Get("numberOfBuffers").As<Napi::Number>().Uint32Value();
  }

  if (!obj.Get("priority").IsUndefined())
  {
    if (!obj.Get("priority").IsNumber())
    {
      throw Napi::TypeError::New(env, "options.priority should be a number.");
    }

    params->priority = obj.Get("priority").As<Napi::Number>().Int32Value();
  }

  if (!obj.Get("streamName").IsUndefined())
  {
    if (!obj.Get("streamName").IsString())
    {
      throw Napi::TypeError::New(env, "options.streamName should be a number.");
    }

    params->streamName = obj.Get("streamName").As<Napi::String>().Utf8Value();
  }
}

RtAudio::Api NodeRtAudio::parseApi(Napi::Env env, const Napi::Value &val)
{
  if (val.IsUndefined())
  {
    return RtAudio::UNSPECIFIED;
  }

  if (!val.IsNumber() || val.As<Napi::Number>().Int32Value() < 0 || val.As<Napi::Number>().Int32Value() > RtAudio::Api::WINDOWS_DS)
  {
    throw Napi::TypeError::New(env, "The api should be a number between 0 and 8.");
  }

  return static_cast<RtAudio::Api>(val.As<Napi::Number>().Int32Value());
}

unsigned int NodeRtAudio::getFormatByteSize(RtAudioFormat format)
{
  switch (format)
  {
  case RTAUDIO_SINT8:
    return 1;
    break;
  case RTAUDIO_SINT16:
    return 2;
    break;
  case RTAUDIO_SINT24:
    return 3;
    break;
  case RTAUDIO_SINT32:
    return 4;
    break;
  case RTAUDIO_FLOAT32:
    return 4;
    break;
  case RTAUDIO_FLOAT64:
    return 8;
    break;
  default:
    return 2;
  }
}

bool NodeRtAudio::deviceExists(unsigned int id, const std::vector<unsigned int> &devices)
{
  return std::find(devices.begin(), devices.end(), id) != devices.end();
}

// A little hack here to avoid using `NodeRtAudio::` in the macro call below.
auto &NodeRtAudioAddonInit = NodeRtAudio::Init;

NODE_API_MODULE(NodeRtAudio, NodeRtAudioAddonInit)