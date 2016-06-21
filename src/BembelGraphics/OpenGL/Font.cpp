/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/
#include <JoHeLib/OpenGL.h>

#include "Font.h"

#include <JoHeLib/Base/Logging/Logger.h>
#include <JoHeLib/Base/Assets/AssetManager.h>

#include <fstream>
#include <algorithm>

#include <JoHeLib/Utils/XML.h>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace jhl{

JHL_ASSET_INTERFACE_DECLARATION(Font)

float Font::GetAscender() const
{
	return _ascender;
}

float Font::GetDescender() const
{
	return _descender;
}

Texture* Font::GetGlyphAtlasTexture() const
{
	return _atlasTexture.Get();
}
Texture* Font::GetGlyphExtendsTexture() const
{
	return _glyphExtentsTexture.get();
}
Texture* Font::GetGlyphMappingTexture() const
{
	return _glyphMappingTexture.get();
}

unsigned Font::GetGlyphID(char32_t character) const
{
	auto it = _charMap.find(character);
	if (it != _charMap.end())
		return it->second;

	return 0;
}

float Font::GetAdvance(unsigned glyphID) const
{
	if (glyphID < _glypths.size())
		return _glypths[glyphID].advance;

	return _glypths[0].advance;
}

float Font::GetAdvance(std::vector<unsigned>& glyphIDs) const
{
	float advance = 0;
	unsigned prevGlyph = 0;
	
	for (unsigned glyph : glyphIDs)
	{
		if (glyph >= _glypths.size())
			glyph = 0;

		advance += _glypths[glyph].advance;
		advance += GetKernig(prevGlyph, glyph);
		prevGlyph = glyph;
	}
	return advance;
}

float Font::GetKernig(unsigned leftGlyphID, unsigned rightGlyphID) const
{
	auto key = std::make_pair(leftGlyphID, rightGlyphID);
	auto it = _kernig.find(key);
	if (it != _kernig.end())
		return it->second;

	return 0.0f;
}

Rectangle Font::GetGlyphExtents(unsigned glyphID) const
{
	if (glyphID < _glypths.size())
		return _glypths[glyphID].extents;

	return _glypths[0].extents;
}

Rectangle Font::GetGlyphTexCoord(unsigned glyphID) const
{
	if (glyphID < _glypths.size())
		return _glypths[glyphID].texCoords;

	return _glypths[0].texCoords;
}

bool Font::ParseString(
	const std::string& text,
	std::vector<unsigned>& glyphIDs) const
{
	glyphIDs.clear();
	glyphIDs.reserve(text.length());

	unsigned int index = 0;
	char32_t character;
	while (ReadCharacter(text, index, character))
		glyphIDs.push_back(GetGlyphID(character));
	return true;
} 

unsigned Font::GetNumGlyphs() const
{
	return static_cast<unsigned>(_glypths.size());
}

bool Font::ReadCharacter(
	const std::string& text,
	unsigned int& index,
	char32_t& character)
{
	if (index >= text.size())
		return false;// string is to short;

	character = text[index++];

	if ((character & 0x80) == 0)
	{
		// one byte of character
		return true;
	}
	else if ((character & 0x40) == 0)
	{
		// follow byte of multi byte character
		return false;
	}

	int numBytes = 0;
	if ((character & 0x20) == 0)
	{
		// first byte of two byte of character
		character &= 0x1F;
		numBytes = 2;
	}
	else if ((character & 0x10) == 0)
	{
		// first byte of tree byte of character
		character &= 0x0F;
		numBytes = 3;
	}
	else if ((character & 0x08) == 0)
	{
		// first byte of four byte of character
		character &= 0x07;
		numBytes = 4;
	}
	else if ((character & 0x04) == 0)
	{
		// first byte of five byte of character
		character &= 0x03;
		numBytes = 5;
	}
	else if ((character & 0x02) == 0)
	{
		// first byte of five byte of character
		character &= 0x01;
		numBytes = 6;
	}

	if (numBytes == 0)
		return 0;

	// read following bytes
	for (int i = 1; i < numBytes; ++i)
	{
		if (index >= text.size())
			return false; // string is to short;

		char c = text[index++];
		if ((c & 0xC) != 0x8)
			return false; // invalid byte 

		character = character << 6 | (c & 0x7F);
	}

	return true;
}

void Font::AddGlyph(float advance, const Rectangle& extents, const Rectangle& texCoords)
{
	_glypths.push_back(Glyph{advance, extents, texCoords});
}

void Font::AddCharMapEntry(char32_t c, unsigned glyphID)
{
	if (glyphID >= _glypths.size())
		return;

	_charMap.emplace(c, glyphID);
}

void Font::AddKerningMapEntry(unsigned left, unsigned right, float kerning)
{
	if (left >= _glypths.size() || right >= _glypths.size())
		return;

	auto key = std::make_pair(left, right);
	_kernig.emplace(key, kerning);
}

void Font::UpdateFontData()
{
	_ascender = 0;
	_descender = 0;

	std::vector<float> extents;
	std::vector<float> texCoords;
	extents.reserve(4*_glypths.size());
	texCoords.reserve(4*_glypths.size());

	for (auto& glyph : _glypths)
	{
		_ascender  = glm::max(_ascender,  glyph.extents.max.y);
		_descender = glm::min(_descender, glyph.extents.min.y);

		extents.push_back(glyph.extents.min.x);
		extents.push_back(glyph.extents.min.y);
		extents.push_back(glyph.extents.GetSize().x);
		extents.push_back(glyph.extents.GetSize().y);
		texCoords.push_back(glyph.texCoords.min.x);
		texCoords.push_back(glyph.texCoords.min.y);
		texCoords.push_back(glyph.texCoords.GetSize().x);
		texCoords.push_back(glyph.texCoords.GetSize().y);
	}

	_glyphExtentsTexture->Bind();
	glTexImage1D(GL_TEXTURE_1D, 0, (GLint)(GL_RGBA16F),
				 static_cast<GLsizei>(extents.size()/4),
				 0, GL_RGBA, GL_FLOAT, &extents[0]);
	_glyphMappingTexture->Bind();
	glTexImage1D(GL_TEXTURE_1D, 0, (GLint)(GL_RGBA16),
				 static_cast<GLsizei>(texCoords.size()/4),
				 0, GL_RGBA, GL_FLOAT, &texCoords[0]);
	glBindTexture(GL_TEXTURE_1D, 0);
}

Font::Font(TextureHandle texture)
	: _atlasTexture(texture)
{
	_glyphExtentsTexture = std::make_unique<Texture>(GL_TEXTURE_1D);
	_glyphMappingTexture = std::make_unique<Texture>(GL_TEXTURE_1D);
}

Font::~Font()
{

}

FontLoader::FontLoader(std::weak_ptr<AssetManager> mgr)
	: _assetManager(mgr)
{}
FontLoader::~FontLoader()
{}

Font* FontLoader::LoadAsset(const std::string& fileName)
{
	auto assetMgr = _assetManager.lock();
	if (!assetMgr)
		return nullptr;

	xml::Document doc;
	doc.LoadFile(fileName.c_str());
	if (doc.Error())
	{
		JHL_LOG_ERROR()
			<< "Can't load file \"" << fileName << "\"\n"
			<< doc.GetErrorStr1() << std::endl;
		return nullptr;
	}

	xml::Element* root = doc.FirstChildElement("Font");
	if (doc.Error())
	{
		JHL_LOG_ERROR()
			<< "\"" << fileName << "\" has no root element \"Font\"" << std::endl;
		return nullptr;
	}

	TextureHandle texture = assetMgr->GetAsset<Texture>(
		root->FirstChildElement("Texture"));

	if (texture.Get() == nullptr)
		return nullptr;

	Font* font = new Font(texture);

	if (!ReadGlyphs(font, root->FirstChildElement("Glyphs")) ||
		!ReadCharMap(font, root->FirstChildElement("CharMap")) ||
		!ReadKerning(font, root->FirstChildElement("Kerning")))
	{
		delete font;
		return nullptr;
	}

	font->UpdateFontData();


	return font;
}

bool FontLoader::ReadGlyphs(
	Font* font, const xml::Element* properties)
{
	if (!properties)
		return false;

	const xml::Element* glyphProps = properties->FirstChildElement("Glyph");
	while (glyphProps)
	{
		float advance;
		Rectangle extendes, texCoords;
		xml::GetAttribute(glyphProps, "advance", advance);
		xml::GetAttribute(glyphProps, "extends", extendes);
		xml::GetAttribute(glyphProps, "texCoord", texCoords);
		font->AddGlyph(advance, extendes, texCoords);

		glyphProps = glyphProps->NextSiblingElement("Glyph");
	}
	return true;
}

bool FontLoader::ReadCharMap(
	Font* font, const xml::Element* properties)
{
	if (!properties)
		return false;

	const xml::Element* entry = properties->FirstChildElement("Entry");
	while (entry)
	{
		unsigned c, glyph;

		if (xml::GetAttribute(entry, "char", c) &&
			xml::GetAttribute(entry, "glyph", glyph))
		{
			font->AddCharMapEntry(c, glyph);
		}

		entry = entry->NextSiblingElement("Entry");
	}
	return true;
}

bool FontLoader::ReadKerning(
	Font* font, const xml::Element* properties)
{
	if (!properties)
		return false;

	const xml::Element* entry = properties->FirstChildElement("Entry");
	while (entry)
	{
		unsigned left, right;
		float kerning;

		if(xml::GetAttribute(entry,"left", left) &&
		   xml::GetAttribute(entry, "right", right) &&
		   xml::GetAttribute(entry, "kerning", kerning))
		{
			font->AddKerningMapEntry(left, right, kerning);
		}

		entry = entry->NextSiblingElement("Entry");
	}
	return true;
}

} //end of namespace JHL
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/

