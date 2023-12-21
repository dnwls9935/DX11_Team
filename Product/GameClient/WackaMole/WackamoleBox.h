#pragma once

#include "Behaviour.h"

namespace ce
{
	class ModelRenderer;
}

class WackamoleBox : public Behaviour
{
public:		WackamoleBox(void) noexcept;
public:		virtual ~WackamoleBox(void) noexcept = default;

public:		void	Start(void) noexcept override;
public:		void	Update(float fElapsedTime) noexcept override;

private:	ModelRenderer*		_renderer;
};

