#ifndef OBS_SOURCE_UTIL_H
#define OBS_SOURCE_UTIL_H

#include <obs.h>

#include <string>
#include <vector>

inline bool is_obs_source_text(obs_source_t *source)
{
	if (source == nullptr) {
		return false;
	}
	const auto source_id = obs_source_get_id(source);
	return strncmp(source_id, "text_ft2_source", 15) == 0 ||
	       strncmp(source_id, "text_gdiplus", 12) == 0;
}

inline bool is_obs_source_text(const std::string &source_name)
{
	obs_source_t *source = obs_get_source_by_name(source_name.c_str());
	if (source != nullptr) {
		const bool is_text = is_obs_source_text(source);
		obs_source_release(source);
		return is_text;
	}
	return false;
}

struct source_render_data {
	gs_texrender_t *texrender;
	gs_stagesurf_t *stagesurface;
};

void init_source_render_data(source_render_data *tf);
void destroy_source_render_data(source_render_data *tf);

std::vector<uint8_t> get_rgba_from_source_render(obs_source_t *source, source_render_data *tf,
						 uint32_t &width, uint32_t &height, float scale);

std::string convert_rgba_buffer_to_png_base64(const std::vector<uint8_t> &rgba, uint32_t width,
					      uint32_t height);

inline bool is_valid_output_source_name(const char *output_source_name)
{
	return output_source_name != nullptr && strcmp(output_source_name, "none") != 0 &&
	       strcmp(output_source_name, "(null)") != 0 && strcmp(output_source_name, "") != 0;
}

std::string get_source_name_without_prefix(const std::string &source_name);

// Helper function to check if a source/filter name format indicates a filter
// Format: "(Source) SourceName -> FilterName"
inline bool is_filter_output(const std::string &output_source) {
	return output_source.find(" -> ") != std::string::npos;
}

// Parse filter output string to get source name and filter name
// Format: "(Source) SourceName -> FilterName"
inline bool parse_filter_output(const std::string &output_source, std::string &source_name, std::string &filter_name) {
	size_t arrow_pos = output_source.find(" -> ");
	if (arrow_pos == std::string::npos) {
		return false;
	}
	
	source_name = output_source.substr(0, arrow_pos);
	filter_name = output_source.substr(arrow_pos + 4);
	
	// Remove the prefix from source_name if it exists
	source_name = get_source_name_without_prefix(source_name);
	
	return true;
}

#endif // OBS_SOURCE_UTIL_H
