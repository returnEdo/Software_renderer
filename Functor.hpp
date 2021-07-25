#pragma once


namespace Renderer
{


class IFunctor
{
	public:
	
	virtual float evaluate(float) = 0;
};

}
