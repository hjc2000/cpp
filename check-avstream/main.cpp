﻿#include <CLI/CLI.hpp>
#include <ffmpeg-wrapper/mux/AVMixer.h>
#include <ffmpeg-wrapper/output-format/FileOutputFormat.h>

int main(int argc, char **argv)
{
	try
	{
		std::string input_url;

		CLI::App app{"检查输入格式是否有音频流和视频流。可以选择检查视频流或检查音频流。但不能同时选择检查。"};
		CLI::Option *option_video = app.add_flag("--video",
												 "检查是否有视频流。");

		CLI::Option *option_audio = app.add_flag("--audio",
												 "检查是否有音频流。");

		app.add_option("-i",
					   input_url,
					   "输入的媒体格式的 url。")
			->required();

		app.callback(
			[&]()
			{
				if (option_video->count() + option_audio->count() != 1)
				{
					throw CLI::ParseError(
						"--video 和 --audio 这两个选项必须指定一个且只能指定一个。",
						CLI::ExitCodes::ExtrasError);
				}
			});

		// 解析命令行参数
		CLI11_PARSE(app, argc, argv);

		// 命令行解析完成，开始处理
		std::shared_ptr<video::InputFormat> input_format{new video::InputFormat{input_url}};
		if (option_video->count())
		{
			// 使用了 --video 选项
			video::AVStreamWrapper stream = input_format->FindBestStream(AVMediaType::AVMEDIA_TYPE_VIDEO);
			if (!stream)
			{
				return -1;
			}

			return 0;
		}

		if (option_audio->count())
		{
			// 使用了 --audio 选项
			video::AVStreamWrapper stream = input_format->FindBestStream(AVMediaType::AVMEDIA_TYPE_AUDIO);
			if (!stream)
			{
				return -1;
			}

			return 0;
		}

		return -1;
	}
	catch (std::runtime_error const &e)
	{
		std::cerr << e.what() << std::endl;
		throw;
	}
}
