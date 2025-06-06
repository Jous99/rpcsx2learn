#include "stdafx.h"
#include "GLHelpers.h"
#include "GLCompute.h"
#include "util/logs.hpp"
#include <unordered_map>

namespace gl
{
	std::unordered_map<u32, std::unique_ptr<gl::compute_task>> g_compute_tasks;
	capabilities g_driver_caps;

	void flush_command_queue(fence& fence_obj)
	{
		fence_obj.check_signaled();
	}

	GLenum draw_mode(rsx::primitive_type in)
	{
		switch (in)
		{
		case rsx::primitive_type::points: return GL_POINTS;
		case rsx::primitive_type::lines: return GL_LINES;
		case rsx::primitive_type::line_loop: return GL_LINE_LOOP;
		case rsx::primitive_type::line_strip: return GL_LINE_STRIP;
		case rsx::primitive_type::triangles: return GL_TRIANGLES;
		case rsx::primitive_type::triangle_strip: return GL_TRIANGLE_STRIP;
		case rsx::primitive_type::triangle_fan: return GL_TRIANGLE_FAN;
		case rsx::primitive_type::quads: return GL_TRIANGLES;
		case rsx::primitive_type::quad_strip: return GL_TRIANGLE_STRIP;
		case rsx::primitive_type::polygon: return GL_TRIANGLES;
		default:
			fmt::throw_exception("unknown primitive type");
		}
	}

	void destroy_compute_tasks()
	{
		for (auto& [key, prog] : g_compute_tasks)
		{
			prog->destroy();
		}

		g_compute_tasks.clear();
	}

	// https://www.khronos.org/opengl/wiki/Debug_Output
	void APIENTRY log_debug(GLenum source, GLenum type, GLuint id,
		GLenum severity, GLsizei /*length*/, const GLchar* message,
		const void* /*user_param*/)
	{
		// Message source
		std::string str_source;
		switch (source)
		{
		// Calls to the OpenGL API
		case GL_DEBUG_SOURCE_API:
			str_source = "API";
			break;
		// Calls to a window-system API
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
			str_source = "WINDOW_SYSTEM";
			break;
		// A compiler for a shading language
		case GL_DEBUG_SOURCE_SHADER_COMPILER:
			str_source = "SHADER_COMPILER";
			break;
		// An application associated with OpenGL
		case GL_DEBUG_SOURCE_THIRD_PARTY:
			str_source = "THIRD_PARTY";
			break;
		// Generated by the user of this application
		case GL_DEBUG_SOURCE_APPLICATION:
			str_source = "APPLICATION";
			break;
		// Some source that isn't one of these
		case GL_DEBUG_SOURCE_OTHER:
			str_source = "OTHER";
			break;
		// Not on documentation
		default:
			str_source = "UNKNOWN";
			rsx_log.error("log_debug(source=%d): Unknown message source", source);
		}

		// Message type
		std::string str_type;
		switch (type)
		{
		// An error, typically from the API
		case GL_DEBUG_TYPE_ERROR:
			str_type = "ERROR";
			break;
		// Some behavior marked deprecated has been used
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
			str_type = "DEPRECATED";
			break;
		// Something has invoked undefined behavior
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
			str_type = "UNDEFINED";
			break;
		// Some functionality the user relies upon is not portable
		case GL_DEBUG_TYPE_PORTABILITY:
			str_type = "PORTABILITY";
			break;
		// Code has triggered possible performance issues
		case GL_DEBUG_TYPE_PERFORMANCE:
			str_type = "PERFORMANCE";
			break;
		// Command stream annotation
		case GL_DEBUG_TYPE_MARKER:
			str_type = "MARKER";
			break;
		// Group pushing
		case GL_DEBUG_TYPE_PUSH_GROUP:
			str_type = "PUSH_GROUP";
			break;
		// foo
		case GL_DEBUG_TYPE_POP_GROUP:
			str_type = "POP_GROUP";
			break;
		// Some type that isn't one of these
		case GL_DEBUG_TYPE_OTHER:
			str_type = "OTHER";
			break;
		// Not on documentation
		default:
			str_type = "UNKNOWN";
			rsx_log.error("log_debug(type=%d): Unknown message type", type);
		}

		switch (severity)
		{
		// All OpenGL Errors, shader compilation/linking errors, or highly-dangerous undefined behavior
		case GL_DEBUG_SEVERITY_HIGH:
		// Major performance warnings, shader compilation/linking warnings, or the use of deprecated functionality
		case GL_DEBUG_SEVERITY_MEDIUM:
			rsx_log.error("[DEBUG_OUTPUT] [%s] [%s] [%d]: %s", str_source, str_type, id, message);
			return;
		// Redundant state change performance warning, or unimportant undefined behavior
		case GL_DEBUG_SEVERITY_LOW:
			rsx_log.warning("[DEBUG_OUTPUT] [%s] [%s] [%d]: %s", str_source, str_type, id, message);
			return;
		// Anything that isn't an error or performance issue
		case GL_DEBUG_SEVERITY_NOTIFICATION:
			rsx_log.notice("[DEBUG_OUTPUT] [%s] [%s] [%d]: %s", str_source, str_type, id, message);
			return;
		// Not on documentation
		default:
			rsx_log.error("log_debug(severity=%d): Unknown severity level", severity);
			rsx_log.error("[DEBUG_OUTPUT] [%s] [%s] [%d]: %s", str_source, str_type, id, message);
			return;
		}
	}

	void enable_debugging()
	{
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(log_debug, nullptr);
	}

	const capabilities& get_driver_caps()
	{
		if (!g_driver_caps.initialized)
			g_driver_caps.initialize();

		return g_driver_caps;
	}

	bool is_primitive_native(rsx::primitive_type in)
	{
		switch (in)
		{
		case rsx::primitive_type::points:
		case rsx::primitive_type::lines:
		case rsx::primitive_type::line_loop:
		case rsx::primitive_type::line_strip:
		case rsx::primitive_type::triangles:
		case rsx::primitive_type::triangle_strip:
		case rsx::primitive_type::triangle_fan:
		case rsx::primitive_type::quad_strip:
			return true;
		case rsx::primitive_type::quads:
		case rsx::primitive_type::polygon:
			return false;
		default:
			fmt::throw_exception("unknown primitive type");
		}
	}
} // namespace gl
