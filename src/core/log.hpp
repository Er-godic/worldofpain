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
	static void init();
	inline static std::shared_ptr<spdlog::logger>& coreLog() {return s_core;}
	inline static std::shared_ptr<spdlog::logger>& renderLog() {return s_render;}
	inline static std::shared_ptr<spdlog::logger>& physicsLog() {return s_physics;}
};

// logging macros

#define CORE_INFO(...)         Log::coreLog()->info(__VA_ARGS__)
#define CORE_WARN(...)         Log::coreLog()->warn(__VA_ARGS__)
#define CORE_ERROR(...)        Log::coreLog()->error(__VA_ARGS__)
#define CORE_CRITICAL(...)     Log::coreLog()->critical(__VA_ARGS__)

#define RENDER_INFO(...)       Log::renderLog()->info(__VA_ARGS__)
#define RENDER_WARN(...)       Log::renderLog()->warn(__VA_ARGS__)
#define RENDER_ERROR(...)      Log::renderLog()->error(__VA_ARGS__)
#define RENDER_CRITICAL(...)   Log::renderLog()->critical(__VA_ARGS__)

#define PHYSICS_INFO(...)      Log::physicsLog()->info(__VA_ARGS__)
#define PHYSICS_WARN(...)      Log::physicsLog()->warn(__VA_ARGS__)
#define PHYSICS_ERROR(...)     Log::physicsLog()->error(__VA_ARGS__)
#define PHYSICS_CRITICAL(...)  Log::physicsLog()->critical(__VA_ARGS__)
