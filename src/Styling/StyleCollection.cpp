#include "./StyleCollection.hpp"

namespace BL {

	void StyleCollection::appendNewStyle(std::string style_name, Style style)
	{
		if(m_styles.find(style_name) == m_styles.end())
		{
			m_styles[style_name] = style;
			return;
		}



		std::cout << "the style " << style_name << " already exists : from appendNewStyle function \n";
	}

	void StyleCollection::overwriteStyle(std::string style_name, Style new_style)
	{
		if(m_styles.find(style_name) != m_styles.end())
		{
			m_styles[style_name] = new_style;
			return;
		}

		std::cout << "the style " << style_name << " does not exist : from overwriteStyle function \n";
	}

	void StyleCollection::deleteStyle(std::string style_name)
	{
		if(m_styles.find(style_name) != m_styles.end())
		{
			m_styles.erase(style_name);
			return;
		}

		std::cout << "the style " << style_name << " does not exist : from deleteStyle function\n";
	}

	Style StyleCollection::getStyle(std::string style_name)
	{
		if(m_styles.find(style_name) != m_styles.end())
		{
			return m_styles[style_name];
		}

		std::cout << "the style " << style_name << " does not exist : from getStyle function\n";
		return Style();
	}

	void StyleCollection::changeStyleName(std::string old_name, std::string new_name)
	{
		if(m_styles.find(old_name) != m_styles.end())
		{
			if(m_styles.find(old_name) == m_styles.end())
			{
				Style temp_copy = m_styles[old_name];

				m_styles.erase(old_name);

				m_styles[new_name] = temp_copy;
			}
			else{
				std::cout << "the new name " << new_name << " you chose is already in use : from changeStyleName function \n";
			}

			return;
		}

		std::cout << "the style " << old_name << " does not exist : from changeStyleName function\n";
	}
}