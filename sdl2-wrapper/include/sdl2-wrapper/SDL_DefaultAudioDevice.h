#pragma once
#include <atomic>
#include <base/IDisposable.h>
#include <base/string/Json.h>
#include <base/task/TaskCompletionSignal.h>
#include <ffmpeg-wrapper/info-collection/IAudioFrameInfoCollection.h>
#include <mutex>
#include <sdl2-wrapper/SDL_AudioSpecWrapper.h>
#include <sdl2-wrapper/SDL_Initializer.h>
#include <semaphore>
#include <sstream>

namespace video
{
	/// <summary>
	///		SDL 默认的音频设备。
	///		- 本类线程安全。
	///		- 本类作为 IAudioFrameInfoCollection 派生类，提供的是音频驱动器的信息，
	///		  而不是构造函数传进来的输入音频流的信息。
	/// </summary>
	class SDL_DefaultAudioDevice
		: public IAudioFrameInfoCollection,
		  public base::IJsonSerializable,
		  public base::IDisposable
	{
		std::atomic_bool _disposed = false;
		SDL_AudioSpecWrapper _desired_spec;
		SDL_AudioSpecWrapper _abtained_spec;
		std::mutex _not_private_methods_lock;

		static void static_audio_callback(void *userdata, uint8_t *stream, int len);

	public:
		/// <summary>
		///		构造函数。会调用 SDL 打开音频设备。打开失败会抛出异常。
		/// </summary>
		SDL_DefaultAudioDevice();
		~SDL_DefaultAudioDevice();

		/// <summary>
		///		本方法会阻塞，直到 _audio_callback 返回，所以不要在 _audio_callback
		///		里面调用本方法。
		/// </summary>
		void Dispose() override;

		/// <summary>
		///		音频回调委托。
		///		本委托函数一旦返回，就认为成功向 stream 中写入音频数据了。
		///		所以，一定要保证写入成功。
		/// </summary>
		/// <param name="stream">SDL 的音频缓冲区，需要将音频数据复制到这里。</param>
		/// <param name="len">音频缓冲区的大小。写入音频数据不可以超过此大小。</param>
		function<void(uint8_t *stream, int len)> _audio_callback;

		/// <summary>
		///		暂停或播放。
		///		* 本方法不会阻塞，可以在回调函数中使用。
		/// </summary>
		/// <param name="pause">传入 true 暂停，传入 false 播放</param>
		void Pause(bool pause);

#pragma region IAudioStreamInfoCollection
		AVRational TimeBase() const override;
		void SetTimeBase(AVRational value) override;
		AVSampleFormat SampleFormat() const override;
		void SetSampleFormat(AVSampleFormat value) override;
		int SampleRate() const override;
		void SetSampleRate(int value) override;
		AVChannelLayout ChannelLayout() const override;
		void SetChannelLayout(AVChannelLayout value) override;
		int SampleCount() const override;
		void SetSampleCount(int value) override;
#pragma endregion

		base::Json ToJson() override;
	};
}