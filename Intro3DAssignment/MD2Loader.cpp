#include "stdafx.h"
#include "MD2Loader.h"

// File reading
#include <iostream>
#include <fstream>

using namespace std;

// BYTE added in case Windows.h is not included.
typedef unsigned char BYTE; 

// Magic number for MD2 files  "IDP2" or 844121161
const int MD2_IDENT = (('2'<<24) + ('P'<<16) + ('D'<<8) + 'I');

// MS2 version
const int MD2_VERSION = 8;

struct Md2Triangle
{
	short vertexIndex[3];   // Vertex indices of the triangle
	short uvIndex[3];       // Texture coordinate indices 
};

// Texture coordinatess
struct Md2TexCoord
{
	//short texCoord[2];
	short u;
	short v;
};

struct Md2Vertex
{
	BYTE v[3];                // Compressed vertex (x, y, z) coordinates
	BYTE lightNormalIndex;    // Index to a normal vector for the lighting
};

struct Md2Frame
{
	float       scale[3];       // Scale values
	float       translate[3];   // Translation vector
	char        name[16];       // Frame name
	Md2Vertex   verts[1];       // First vertex of this frame
};

// Constructors

MD2Loader::MD2Loader(void)
{
}

MD2Loader::~MD2Loader(void)
{
}

// ----------------------------------------------
// LoadModel() - load model from file.
// ----------------------------------------------

bool MD2Loader::LoadModel(const char* filename, Model3D& model, const char* textureFilename)
{
	ifstream   file;           

	// Try to open MD2 file
	file.open( filename, ios::in | ios::binary );
	if(file.fail())
	{
		return false;
	}
	// Read file header
	Md2Header header;         
	file.read( reinterpret_cast<char*>(&header), sizeof( Md2Header ) );
		
	// Verify that this is a MD2 file (check for the magic number and version number)
	if( (header.indent != MD2_IDENT) && (header.version != MD2_VERSION) )
	{
		// This is not a MD2 model
		file.close();
		return false;
	}

	// Allocate the memory we need
	Md2Triangle* triangles = new Md2Triangle[header.numTriangles];
	// We are only interested in the first frame 
	BYTE* frameBuffer = new BYTE[ header.frameSize ];
	Md2Frame* frame = reinterpret_cast<Md2Frame*>(frameBuffer);
	Md2TexCoord* texCoords = new Md2TexCoord[ header.numTexCoords ];

	// Read polygon data...
	file.seekg( header.offsetTriangles, ios::beg );
	file.read( reinterpret_cast<char*>(triangles), sizeof(Md2Triangle) * header.numTriangles );	
		
	// Read frame data...
	file.seekg( header.offsetFrames, ios::beg );
	file.read( reinterpret_cast<char*>(frame), header.frameSize );
	// Read texture coordinate data
	file.seekg( header.offsetTexCoords, std::ios::beg );
	file.read( reinterpret_cast<char*>(texCoords), sizeof(Md2TexCoord) * header.numTexCoords );
		
	// Close the file 
	file.close();

	//----------------------------------------------------------------------------------------------

	bool bHasTexture = false;
	// Attempt to load texture
	if ( textureFilename != 0 )
	{
		BYTE* pTexture = new BYTE[header.skinWidth * header.skinHeight];
		Gdiplus::Color* pPalette = new Gdiplus::Color[256];
		bHasTexture = LoadPCX(textureFilename, pTexture, pPalette, &header);
		if ( !bHasTexture )
		{
			delete(pTexture);
			delete(pPalette);
		}
		else
		{
			model.SetTexture(pTexture);
			model.SetPalette(pPalette);
			model.SetTextureWidth(header.skinWidth);
		}
	}

	// Polygon array initialization
	for ( int i = 0; i < header.numTriangles; ++i )
	{
		Polygon3D polygon;

		// Stores the index to the vertices in the polygon

		polygon.SetI(0, triangles[i].vertexIndex[0]);
		polygon.SetI(1, triangles[i].vertexIndex[1]);
		polygon.SetI(2, triangles[i].vertexIndex[2]);

		// Stores the texture co-ordinates in the polygon

		if ( bHasTexture )
		{
			for (int j = 0; j < 2; j++)
			{
				float u = (float)(texCoords[triangles[i].uvIndex[j]].u) / (float)(header.skinWidth);
				float v = (float)(texCoords[triangles[i].uvIndex[j]].v) / (float)(header.skinHeight);
				polygon.SetTexel(j, (float)u, (float)v);
			}	
		}

		// Adds the polygon to the list of polygons in the model

		model.AddPolygon(polygon);
	}

	// Vertex array initialization
	for( int i = 0; i < header.numVertices; ++i )
	{
		Vertex vertex;

		// Stores the x,y and z values of the vertex and sets the w value to 1

		vertex.SetX((frame->verts[i].v[0] * frame->scale[0]) + frame->translate[0]);
		vertex.SetY((frame->verts[i].v[2] * frame->scale[2]) + frame->translate[2]);
		vertex.SetZ((frame->verts[i].v[1] * frame->scale[1]) + frame->translate[1]);
		vertex.SetW(1.0f);

		// Adds the vertex to the list of vertices in the model

		model.AddVertex(vertex);

	}

	// Free dynamically allocated memory
	delete [] triangles; // NOTE: this is 'array' delete. Must be sure to use this
	triangles = 0;

	delete [] texCoords;
	texCoords = 0;

	delete [] frameBuffer;
	frameBuffer = 0;
	frame = 0;

	return true;
}

// ----------------------------------------------
// LoadPCX() - load pcx texture from file.
//
// assume the caller has allocated enough memory for texture - md2Header->skinWidth * md2Header->skinHeight
// ----------------------------------------------
bool MD2Loader::LoadPCX(const char* filename, BYTE* texture, Gdiplus::Color* palette, const Md2Header* md2Header)
{
	std::ifstream   file;   // file stream

	// try to open filename
    file.open( filename, std::ios::in | std::ios::binary );

    if( file.fail() )
        return false;

    // read md2 header file
    PcxHeader header;         
	file.read( reinterpret_cast<char*>(&header), sizeof( PcxHeader ) );

    /////////////////////////////////////////////
    //      verify that this is a valid PCX file

	// only handle those with 256 colour palette
	if( (header.Version != 5) || (header.BitsPerPixel != 8) || 
		(header.Encoding != 1) || (header.NumPlanes != 1) ||
		(md2Header && (header.BytesPerLine != md2Header->skinWidth) ) )
    {
        // this is not valid supported PCX
        file.close();
        return false;
    }

    /////////////////////////////////////////////
	//		check dimensions

	int XSize = header.XMax - header.XMin + 1;
	int YSize = header.YMax - header.YMin + 1;
	int Size = XSize * YSize;

	// check matches our MD2 expected texture
	// note. valid size is <= because uses RLE (so potentially smaller)
	if ( md2Header && ( Size > (md2Header->skinHeight * md2Header->skinWidth) ) )
	{
		// doesn't match expected MD2 skin size
		file.close();
		return false;
	}
	
	/////////////////////////////////////////////
    //          reading file data

	BYTE processByte, colourByte;
	int count = 0;
	while(count < Size)
	{
		file.read( reinterpret_cast<char*>(&processByte), 1 );

		// Run length encoding - test if byte is an RLE byte
		if ((processByte & 192) == 192)
		{
			// extract number of times repeated byte
			processByte &= 63;
			file.read( reinterpret_cast<char*>(&colourByte), 1 );
			for (int index = 0; index < processByte; ++index)
			{
				// repeatedly write colour 
				texture[count] = colourByte;
				++count;
			}
		}
		else
		{
			// process byte is the colour
			texture[count] = processByte;
			++count;
		}
	}

	bool returnValue = false;

	// read palette data...
	file.seekg( -769, std::ios::end);	// from this offset from end of file
	file.read( reinterpret_cast<char*>(&processByte), 1 );
	if (processByte == 12)
	{
		BYTE rawPalette[768];
		file.read( reinterpret_cast<char*>(&rawPalette), 768 );

		// build palette
		for (int palIndex = 0; palIndex < 256; ++palIndex)
		{			
			palette[palIndex].SetFromCOLORREF( RGB(rawPalette[ palIndex * 3],
												   rawPalette[(palIndex * 3) + 1],
												   rawPalette[(palIndex * 3) + 2] ));
		}

		returnValue = true;
	}

	// all done
	file.close();
	return returnValue;
}
