#pragma once


#include "Component.hpp"


namespace BL
{
	struct Block : public BL::Component{
		void start(){}
		void quit(){}
		void render(){}
	};
}