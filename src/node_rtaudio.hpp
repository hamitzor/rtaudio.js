#ifndef __NODE_ADDON_NODE_RTAUDIO_H__
#define __NODE_ADDON_NODE_RTAUDIO_H__

#include <napi.h>
#include <RtAudio.h>
#include <shared_mutex>
#include <queue>

class NodeRtAudio : public RtAudio, public Napi::ObjectWrap<NodeRtAudio>
{
public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);

public:
  NodeRtAudio(const Napi::CallbackInfo &info);
  ~NodeRtAudio();

public:
  void setErrorCallback(const Napi::CallbackInfo &info);
  void enableWarnings(const Napi::CallbackInfo &info);
  void disableWarnings(const Napi::CallbackInfo &info);
  Napi::Value open(const Napi::CallbackInfo &info);
  void close(const Napi::CallbackInfo &info);
  void start(const Napi::CallbackInfo &info);
  void abort(const Napi::CallbackInfo &info);

  Napi::Value getDevices(const Napi::CallbackInfo &info);
  Napi::Value getDefaultInputDevice(const Napi::CallbackInfo &info);
  Napi::Value getDefaultOutputDevice(const Napi::CallbackInfo &info);
  Napi::Value isOpen(const Napi::CallbackInfo &info);
  Napi::Value isRunning(const Napi::CallbackInfo &info);
  Napi::Value getCurrentApi(const Napi::CallbackInfo &info);
  Napi::Value getLatency(const Napi::CallbackInfo &info);
  Napi::Value getSampleRate(const Napi::CallbackInfo &info);
  Napi::Value getTime(const Napi::CallbackInfo &info);

public:
  static Napi::Value getVersion(const Napi::CallbackInfo &info);

  static Napi::Value getApiDisplayName(const Napi::CallbackInfo &info);
  static Napi::Value getApiName(const Napi::CallbackInfo &info);
  static Napi::Value getCompiledApis(const Napi::CallbackInfo &info);

private:
  static void parseOutputParams(Napi::Env env, const Napi::Value &val, RtAudio::StreamParameters *params);
  static void parseInputParams(Napi::Env env, const Napi::Value &val, RtAudio::StreamParameters *params);
  static void parseStreamOptions(Napi::Env env, const Napi::Value &val, RtAudio::StreamOptions *params);
  static RtAudio::Api parseApi(Napi::Env env, const Napi::Value &val);
  static unsigned int getFormatByteSize(RtAudioFormat format);
  static bool deviceExists(unsigned int id, const std::vector<unsigned int> &devices);

private:
  Napi::ThreadSafeFunction tsCb;
  Napi::ThreadSafeFunction tsErrorCb;
  RtAudioFormat format;
  RtAudio::StreamOptions options;
  RtAudio::StreamParameters inputParams;
  RtAudio::StreamParameters outputParams;
  unsigned int sampleRate;
  unsigned int bufferFrames;

  // To keep the object alive (even if gets eligible for gc) when open is called, but close hasn't called yet.
  Napi::ObjectReference jsRef;
};

#endif