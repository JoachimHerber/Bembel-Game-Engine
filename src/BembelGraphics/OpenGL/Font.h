#ifndef BEMBEL_JHL_FONT_H
#define BEMBEL_FONT_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelConfig.h>

#include <glm/glm.hpp>

#include <string>
#include <vector>
#include <map>
#include "../BufferObject.h"

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace jhl{

class JHL_API Font final
{
	JHL_ASSET_INTERFACE_DEFINITION()
public:	
	Font(TextureHandle);
	~Font();

	void AddGlyph(float, const Rectangle&, const Rectangle&);
	void AddCharMapEntry(char32_t, unsigned);
	void AddKerningMapEntry(unsigned, unsigned, float);

	void UpdateFontData();

	float GetAscender() const;
	float GetDescender() const;

	Texture* GetGlyphAtlasTexture() const;
	Texture* GetGlyphExtendsTexture() const;
	Texture* GetGlyphMappingTexture() const;
	
	unsigned GetGlyphID(char32_t character) const;

	float GetAdvance(std::vector<unsigned>& glyphIDs) const;
	float GetAdvance(unsigned glyphID) const;
	float GetKernig(unsigned leftGlyphID, unsigned rightGlyphID) const;

	Rectangle GetGlyphExtents(unsigned glyphID) const;
	Rectangle GetGlyphTexCoord(unsigned glyphID) const;

	bool ParseString(const std::string& text, std::vector<unsigned>& glyphIDs) const;

	unsigned GetNumGlyphs() const;

private:
	static bool ReadCharacter(
		const std::string& text, 
		unsigned int& index, 
		char32_t& character);

private:
	using CharMap = std::map<char32_t, unsigned>;
	using KernigMap = std::map<std::pair<unsigned, unsigned>, float>;

	struct Glyph
	{
		float     advance;
		Rectangle extents;
		Rectangle texCoords;
	};

	TextureHandle _atlasTexture;

	std::unique_ptr<Texture> _glyphExtentsTexture;
	std::unique_ptr<Texture> _glyphMappingTexture;

	//std::unique_ptr<BufferObject> _glyphExtentsBuffer;
	//std::unique_ptr<BufferObject> _glyphMappingBuffer;

	float _ascender;
	float _descender;

	std::vector<Glyph> _glypths;

	CharMap   _charMap;
	KernigMap _kernig;
};

using FontHandle = AssetHandle<Font>;

class JHL_API FontLoader : public AssetLoader<Font>
{
public:
	FontLoader(std::weak_ptr<AssetManager>);
	~FontLoader();

	virtual Font* LoadAsset(const std::string& fileName) override;

private:
	bool ReadGlyphs(Font*, const xml::Element*);
	bool ReadCharMap(Font*, const xml::Element*);
	bool ReadKerning(Font*, const xml::Element*);

	Texture CreateTexture(const std::vector<float>&, GLenum);

private:
	std::weak_ptr<AssetManager> _assetManager;
};
} //end of namespace JHL

/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
