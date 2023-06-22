#pragma once

#include <map>
#include <string>
#include <iostream>


#include "./Style.hpp"
#include "../Components/Component.hpp"

namespace BL{
	class StyleCollection
	{
		private:
			std::map<std::string, Style> m_styles;

		public:

			void appendNewStyle(std::string style_name, Style style);

			void overwriteStyle(std::string style_name, Style new_style);

			void deleteStyle(std::string style_name);

			Style getStyle(std::string style_name);

			void changeStyleName(std::string old_name, std::string new_name);
	};
}