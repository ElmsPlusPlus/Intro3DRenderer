#pragma once
#include "Rasterizer.h"

class AppEngine
{
	public:
		AppEngine(void);
		~AppEngine(void);

		void Initialise(HWND hWnd);
		void Process(void);
		void Paint(HDC hdc);
		void Shutdown(void);

	private:
		HWND _hWnd;
		Rasterizer * _rasterizer;

		void Render(void);
};

