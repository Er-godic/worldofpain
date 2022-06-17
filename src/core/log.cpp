std::shared_ptr<spdlog::logger> Log::s_core;
std::shared_ptr<spdlog::logger> Log::s_render;
std::shared_ptr<spdlog::logger> Log::s_physics;

void Log::init()
{
	spdlog::set_pattern("%^[%T][thread: %t]%n : %v %$");

	s_core    = spdlog::stdout_color_mt("[Core]");
	s_render  = spdlog::stdout_color_mt("[Render]");
	s_physics = spdlog::stdout_color_mt("[Physics]");

	s_core->set_level(spdlog::level::trace);
	s_render->set_level(spdlog::level::trace);
	s_physics->set_level(spdlog::level::trace);

	print(core, info, "Initialized logging");
}
