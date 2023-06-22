#pragma once

#include <string>

namespace BL{
	struct Style
	{
		int width {0};
		int height {0};

		int padding_top {0};
		int padding_right {0};
		int padding_bottom {0};
		int padding_left {0};

		void padding(int top, int right, int bottom, int left)
		{
			padding_top = top;
			padding_right = right;
			padding_bottom = bottom;
			padding_left = left;
		}


		int margin_top {0};
		int margin_right {0};
		int margin_bottom {0};
		int margin_left {0};

		void marign(int top, int right, int bottom, int left)
		{
			margin_top = top;
			margin_right = right;
			margin_bottom = bottom;
			margin_left = left;
		}


		int left {0};
		int top {0};
	};
}