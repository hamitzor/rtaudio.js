#ifndef __NODE_ADDON_NODE_RTAUDIO_H__
#define __NODE_ADDON_NODE_RTAUDIO_H__

#include <napi.h>
#include <RtAudio.h>
#include <shared_mutex>
#include <queue>
#include <semaphore>

class NodeRtAudio : public RtAudio, public Napi::ObjectWrap<NodeRtAudio>
{
public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);

public:
  NodeRtAudio(const Napi::CallbackInfo &info);
  ~NodeRtAudio();

public:
  void setErrorCallback(const Napi::CallbackInfo &info);
  void showWarnings(const Napi::CallbackInfo &info);
  Napi::Value openStream(const Napi::CallbackInfo &info);
  void closeStream(const Napi::CallbackInfo &info);
  void startStream(const Napi::CallbackInfo &info);
  void stopStream(const Napi::CallbackInfo &info);
  void abortStream(const Napi::CallbackInfo &info);
  Napi::Value getDevices(const Napi::CallbackInfo &info);
  Napi::Value getDefaultInputDevice(const Napi::CallbackInfo &info);
  Napi::Value getDefaultOutputDevice(const Napi::CallbackInfo &info);
  Napi::Value isStreamOpen(const Napi::CallbackInfo &info);
  Napi::Value isStreamRunning(const Napi::CallbackInfo &info);
  Napi::Value getCurrentApi(const Napi::CallbackInfo &info);
  Napi::Value getStreamLatency(const Napi::CallbackInfo &info);
  Napi::Value getStreamSampleRate(const Napi::CallbackInfo &info);
  Napi::Value getStreamTime(const Napi::CallbackInfo &info);
  void setStreamTime(const Napi::CallbackInfo &info);
  Napi::Value getErrorText(const Napi::CallbackInfo &info);

public:
  static Napi::Value getVersion(const Napi::CallbackInfo &info);
  static Napi::Value getApiDisplayName(const Napi::CallbackInfo &info);
  static Napi::Value getApiName(const Napi::CallbackInfo &info);
  static Napi::Value getCompiledApi(const Napi::CallbackInfo &info);

private:
  static void parseOutputParams(Napi::Env env, const Napi::Value &val, RtAudio::StreamParameters *params);
  static void parseInputParams(Napi::Env env, const Napi::Value &val, RtAudio::StreamParameters *params);
  static void parseStreamOptions(Napi::Env env, const Napi::Value &val, RtAudio::StreamOptions *params);
  static RtAudio::Api parseApi(Napi::Env env, const Napi::Value &val);
  static unsigned int getFormatByteSize(RtAudioFormat format);
  static bool deviceExists(unsigned int id, const std::vector<unsigned int> &devices);
  std::binary_semaphore rtThreadSmph;
  std::binary_semaphore jsThreadSmph;

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