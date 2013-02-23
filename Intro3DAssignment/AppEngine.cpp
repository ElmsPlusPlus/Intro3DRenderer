#include "StdAfx.h"
#include "AppEngine.h"
#include "Model3D.h"
#include "MD2Loader.h"

// Main 'engine' class for our application. This is where all of the core processing work is done

// Creates objects and variables

int displayMode = 3;
int frameCount = 0;
Model3D model, model2;
Camera camera(0.0f, 0.0f, 0.0f, Vertex(0, 0, -50, 1), 640, 480);
float rotate = (3.14f / 2.0f);
AmbientLight ambientLight(Gdiplus::Color(0,255,255));
std::vector<DirectionalLight> dirLights;
std::vector<PointLight> pointLights;

AppEngine::AppEngine(void)
{
	_rasterizer = NULL;
}

AppEngine::~AppEngine(void)
{
	// Make sure everything is cleaned up (in case Shutdown() was now called directly
	Shutdown();
}

void AppEngine::Initialise(HWND hWnd)
{
	_hWnd = hWnd;
	
	// Get the width and height of the window so that we can tell the rasterizer to create a bitmap
	// of the same size.  Note that this only works for the initial size of the window. If you want 
	// your code to handle resizing of the window, you will need to add code for this yourself. 
	LONG width;
	LONG height;
	RECT clientWindowRect;
	if (GetClientRect(_hWnd, &clientWindowRect))
	{
		width = clientWindowRect.right;
		height = clientWindowRect.bottom;
	}
	else
	{
		width = 640;
		height = 480;
	}
	_rasterizer = new Rasterizer((unsigned int)width, (unsigned int)height);

	// Initializes variabels needed

	MD2Loader::LoadModel("cube.md2", model, "lines.pcx");
	MD2Loader::LoadModel("tris.md2", model2, "Cartman.pcx");
	DirectionalLight light1(Gdiplus::Color(255,0,0), Vector3D(-1,0,1));
	PointLight light2(Color::Orange, Vertex(0, 0, -50, 1), 0, 1, 0); 
	dirLights.push_back(light1);
	pointLights.push_back(light2);
}

void AppEngine::Process(void)
{
	// At the moment, all we do is render a new image. In the future, we might do more here
	Render();
}

void AppEngine::Render(void)
{
	_rasterizer->Clear(Color::Gray);
	// Ensure that Windows is told that the contents of the window now needs painting
	
	// ROtates model and increases frame count

	rotate = rotate + 0.01f;
	//frameCount++;

	if (rotate == 6.28)
	{
		rotate = 0.0f;
	}

	// Used for decided what to display

	if (frameCount > 150)
	{
		displayMode++;
		frameCount = 0;
	}

	if (displayMode > 13)
	{
		displayMode = 0;
	}

	// Set coefficients

	model.SetKdRed(1.0f);
	model.SetKdGreen(1.0f);
	model.SetKdBlue(1.0f);
	model.SetKaRed(1.0f);
	model.SetKaGreen(0.5f);
	model.SetKaBlue(0.5f);
	model.SetKsRed(0.5f);
	model.SetKsGreen(1.0f);
	model.SetKsBlue(1.0f);
	model2.SetKdRed(1.0f);
	model2.SetKdGreen(1.0f);
	model2.SetKdBlue(1.0f);
	model2.SetKaRed(1.0f);
	model2.SetKaGreen(0.5f);
	model2.SetKaBlue(0.5f);
	model2.SetKsRed(0.5f);
	model2.SetKsGreen(1.0f);
	model2.SetKsBlue(1.0f);

	// Roates models in all 3 axes

	model.ApplyTransformToLocalVertices(Matrix3D::XYZRotationMatrix(rotate, rotate, rotate));
	model2.ApplyTransformToLocalVertices(Matrix3D::XYZRotationMatrix(rotate, rotate, rotate));
	
	// Culls backfaces

	model.CalculateBackfaces(camera.GetPosition());
	model2.CalculateBackfaces(camera.GetPosition());

	// Flat Shading Lighting

	if (displayMode == 1 || displayMode == 4 || displayMode == 5 || displayMode == 8)
	{
		model.CalculateLightingAmbientFlat(ambientLight);
		model2.CalculateLightingAmbientFlat(ambientLight);
	}

	if (displayMode == 2 || displayMode == 4 || displayMode == 6 || displayMode == 8)
	{
		model.CalculateLightingDirectionalFlat(dirLights);
		model2.CalculateLightingDirectionalFlat(dirLights);
	}

	if (displayMode == 3 || displayMode == 4 || displayMode == 7 || displayMode == 8)
	{
		model.CalculateLightingPointFlat(pointLights);
		model2.CalculateLightingPointFlat(pointLights);
	}

	// Vertex Lighting

	if (displayMode == 9 || displayMode == 10 || displayMode == 11 || displayMode == 12)
	{
		model.CalculateVertexNormalVectors();
		model2.CalculateVertexNormalVectors();
	}

	if (displayMode == 9 || displayMode == 12)
	{
		model.CalculateLightingAmbient(ambientLight);
		model2.CalculateLightingAmbient(ambientLight);
	}

	if (displayMode == 10 || displayMode == 12)
	{
		model.CalculateLightingDirectional(dirLights);
		model2.CalculateLightingDirectional(dirLights);
	}

	if (displayMode == 11 || displayMode == 12)
	{
		model.CalculateLightingPoint(pointLights);
		model2.CalculateLightingPoint(pointLights);
	}

	// Turs 3D co-ordinates into 2D to display on screen

	model.ApplyTransformToTransformedVertices(camera.GetViewpointMatrix());
	model.Sort();
	model.ApplyTransformToTransformedVertices(camera.GetPerspectiveMatrix());
	model.Deheomogenize();
	model.ApplyTransformToTransformedVertices(camera.GetScreenMatrix());
	model2.ApplyTransformToTransformedVertices(camera.GetViewpointMatrix());
	model2.Sort();
	model2.ApplyTransformToTransformedVertices(camera.GetPerspectiveMatrix());
	model2.Deheomogenize();
	model2.ApplyTransformToTransformedVertices(camera.GetScreenMatrix());
	model2.ApplyTransformToTransformedVertices(Matrix3D::TranslationMatrix(-400, 0, 0));

	// Display Modes showing different drawing methods

	if (displayMode == 0)
	{
		WCHAR string[] = L"Wireframe";
		_rasterizer->DrawString(string);
		_rasterizer->DrawWireFrame(model);
		_rasterizer->DrawWireFrame(model2);
	}

	if (displayMode == 1)
	{
		WCHAR string[] = L"Flat Shading GDI Ambient";
		_rasterizer->DrawString(string);
		_rasterizer->DrawSolidGDIFlat(model);
		_rasterizer->DrawSolidGDIFlat(model2);
	}

	if (displayMode == 2)
	{
		WCHAR string[] = L"Flat Shading GDI Directional";
		_rasterizer->DrawString(string);
		_rasterizer->DrawSolidGDIFlat(model);
		_rasterizer->DrawSolidGDIFlat(model2);
	}

	if (displayMode == 3)
	{
		WCHAR string[] = L"Flat Shading GDI Point";
		_rasterizer->DrawString(string);
		_rasterizer->DrawSolidGDIFlat(model);
		_rasterizer->DrawSolidGDIFlat(model2);
	}

	if (displayMode == 4)
	{
		WCHAR string[] = L"Flat Shading GDI All 3 lights";
		_rasterizer->DrawString(string);
		_rasterizer->DrawSolidGDIFlat(model);
		_rasterizer->DrawSolidGDIFlat(model2);
	}

	if (displayMode == 5)
	{
		WCHAR string[] = L"Flat Shading Ambient";
		_rasterizer->DrawString(string);
		_rasterizer->DrawSolidFlat(model);
		_rasterizer->DrawSolidFlat(model2);
	}

	if (displayMode == 6)
	{
		WCHAR string[] = L"Flat Shading Directional";
		_rasterizer->DrawString(string);
		_rasterizer->DrawSolidFlat(model);
		_rasterizer->DrawSolidFlat(model2);
	}

	if (displayMode == 7)
	{
		WCHAR string[] = L"Flat Shading Point";
		_rasterizer->DrawString(string);
		_rasterizer->DrawSolidFlat(model);
		_rasterizer->DrawSolidFlat(model2);
	}

	if (displayMode == 8)
	{
		WCHAR string[] = L"Flat Shading All 3 lights";
		_rasterizer->DrawString(string);
		_rasterizer->DrawSolidFlat(model);
		_rasterizer->DrawSolidFlat(model2);
	}

	if (displayMode == 9)
	{
		WCHAR string[] = L"Gouraud Shading Ambient";
		_rasterizer->DrawString(string);
		_rasterizer->DrawSolidGouraud(model);
		_rasterizer->DrawSolidGouraud(model2);
	}

	if (displayMode == 10)
	{
		WCHAR string[] = L"Gouraud Shading Directional";
		_rasterizer->DrawString(string);
		_rasterizer->DrawSolidGouraud(model);
		_rasterizer->DrawSolidGouraud(model2);
	}

	if (displayMode == 11)
	{
		WCHAR string[] = L"Gouraud Shading Point";
		_rasterizer->DrawString(string);
		_rasterizer->DrawSolidGouraud(model);
		_rasterizer->DrawSolidGouraud(model2);
	}

	if (displayMode == 12)
	{
		WCHAR string[] = L"Gouraud Shading All 3 lights";
		_rasterizer->DrawString(string);
		_rasterizer->DrawSolidGouraud(model);
		_rasterizer->DrawSolidGouraud(model2);
	}

	if (displayMode == 13)
	{
		WCHAR string[] = L"Texturing (doesn't work, fails to corretcly apply uv co-ordinates)";
		_rasterizer->DrawString(string);
		_rasterizer->DrawSolidTextured(model);
		_rasterizer->DrawSolidTextured(model2);
	}

	InvalidateRect(_hWnd, NULL, FALSE);
}

void AppEngine::Paint(HDC hdc)
{
	// Copy the contents of the rasterizer's bitmap to our window
	if (_rasterizer)
	{
		Graphics graphics(hdc);
		graphics.DrawImage(_rasterizer->GetBitmap(), 0, 0, _rasterizer->GetWidth(), _rasterizer->GetHeight());
	}
}

void AppEngine::Shutdown(void)
{
	// Clean up all memory that has been dynamically allocated
	if (_rasterizer)
	{
		delete _rasterizer;
		_rasterizer = NULL;
	}
}

