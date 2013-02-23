#pragma once
#include "Model3D.h"
#include "stdafx.h"

struct PcxHeader
{
	BYTE  ID;
	BYTE  Version;
	BYTE  Encoding;
	BYTE  BitsPerPixel;
	short XMin;
	short YMin;
	short XMax;
	short YMax;
	short HRes;
	short VRes;
	BYTE  ClrMap[16*3];
	BYTE  Reserved;
	BYTE  NumPlanes;
	short BytesPerLine;
	short Pal;
	BYTE  Filler[58];
};

struct Md2Header
{
		int indent;               // The magic number used to identify the file.
		int version;              // The file version number (must be 8).
		int skinWidth;            // The width in pixels of our image.
		int skinHeight;           // The height in pixels of our image.
		int frameSize;            // The size in bytes the frames are.
		int numSkins;             // The number of skins associated with the model.
		int numVertices;		  // The number of vertices.
		int numTexCoords;		  // The number of texture coordinates.
		int numTriangles;		  // The number of faces (polygons).
		int numGlCommands;        // The number of gl commands.
		int numFrames;			  // The number of animated frames.
		int offsetSkins;		  // The offset in the file for the skin data.
		int offsetTexCoords;	  // The offset in the file for the texture data.
		int offsetTriangles;	  // The offset in the file for the face data.
		int offsetFrames;		  // The offset in the file for the frames data.
		int offsetGlCommands;	  // The offset in the file for the gl commands data.
		int offsetEnd;            // The end of the file offset.
};

// Forward reference
class Model3D;

class MD2Loader
{
	public:
		// Constructers

		MD2Loader(void);
		~MD2Loader(void);

		// Static Methods

		static bool LoadModel(const char* md2Filename, Model3D& model, const char* textureFilename = 0);
		static bool LoadPCX(const char* filename, BYTE* texture, Gdiplus::Color* palette, const Md2Header* md2Header);
};
