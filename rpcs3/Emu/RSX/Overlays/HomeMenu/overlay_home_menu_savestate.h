#pragma once

#include "overlay_home_menu_page.h"

namespace rsx
{
	namespace overlays
	{
		struct home_menu_savestate : public home_menu_page
		{
			home_menu_savestate(s16 x, s16 y, u16 width, u16 height, bool use_separators, home_menu_page* parent);
		};
	} // namespace overlays
} // namespace rsx
