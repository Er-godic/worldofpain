#pragma once
#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

// wrapper for spdlog
// https://github.com/gabime/spdlog/wiki/2.-Creating-loggers

class Log
{
private:
	// there is a log for each main system
	static std::shared_ptr<spdlog::logger> s_core;
	static std::shared_ptr<spdlog::logger> s_render;
	static std::shared_ptr<spdlog::logger> s_physics;
	
public:	
	enum Sys {core, physics, render };
	enum Msg {info, warn, error, critical};
	
	static void init();

	template<typename... Args>
	static void print(Sys sys, Msg msg, Args &&... args)
	{
		spdlog::logger* logger = [sys]()
		{
			switch(sys)
			{
				case core    : return s_core.get();
				case physics : return s_physics.get();
				case render  : return s_render.get();
			}
		}();

		switch(msg)
		{
			case info : 
				logger->info(std::forward<Args>(args)...); 
				break;
			case warn : 
				logger->warn(std::forward<Args>(args)...); 
				break;
			case error : 
				logger->error(std::forward<Args>(args)...); 
				break;
			case critical : 
				logger->critical(std::forward<Args>(args)...); 
				break;
		}
	}
};
