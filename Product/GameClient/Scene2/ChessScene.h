#pragma once
#include "Scene.h"

class ChessScene : public Scene
{
public:		ChessScene(void) noexcept = default;
public:		virtual ~ChessScene(void) noexcept = default;

public:		void		SetLoadingContents(void) noexcept override;
public:		HRESULT		Init(void) noexcept override;
public:		void		FixedUpdate(float fElapsedTime) noexcept override;
public:		void		Update(float fElapsedTime) noexcept override;
public:		void		LateUpdate(float fElapsedTime) noexcept override;
public:		void		Render(void) noexcept override;
public:		void		LateRender(void) noexcept override;
public:		void		Release(void) noexcept override;

private:	class CastleMay* cMay;
private:	class WizardCody* wCody;
private:	class ChessBoardDataManager* _boardmanager;
private:	bool _cinema = false;
};

