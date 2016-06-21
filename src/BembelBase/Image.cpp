/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "Image.h"
#include "lodepng/lodepng.h"

#include <BembelBase/Logging/Logger.h>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

Image::Image()
: _width(0)
, _height(0)
, _channels(0)
{}

Image::Image(unsigned int w, unsigned int h, unsigned int c)
: _width(w)
, _height(h)
, _channels(c)
{
	_data.resize(_width*_height*_channels);
}

Image::~Image()
{}

unsigned char* Image::GetData()
{
	return &(_data[0]);
}

const unsigned char* Image::GetData() const
{
	return &(_data[0]);
}

unsigned int Image::GetWidth() const
{
	return _width;
}

unsigned int Image::GetHeight() const
{
	return _height;
}

unsigned int Image::GetChannels() const
{
	return _channels;
}

bool Image::Load(const std::string& fileName)
{
	std::vector<unsigned char> data;
	unsigned int w, h;
	unsigned error = lodepng::decode(data, w, h, fileName);

	if (error != 0)
	{
		BEMBEL_LOG_ERROR()
			<< "Can't load file \"" << fileName << "\"\n"
			<< lodepng_error_text(error) << std::endl;
		return false;
	}

	// copy data

	_width    = w;
	_height   = h;
	_channels = 4;

	_data.resize(w*h * 4);

	for (unsigned int y = 0; y < h; ++y)
	{
		for (unsigned int x = 0; x < w; ++x)
		{
			_data[4 * (x + w*(y)) + 0] = data[4 * (x + w*(h - y - 1)) + 0];
			_data[4 * (x + w*(y)) + 1] = data[4 * (x + w*(h - y - 1)) + 1];
			_data[4 * (x + w*(y)) + 2] = data[4 * (x + w*(h - y - 1)) + 2];
			_data[4 * (x + w*(y)) + 3] = data[4 * (x + w*(h - y - 1)) + 3];
		}
	}
	return true;
}

bool Image::Save(const std::string& fileName)
{
	std::vector<unsigned char> data;
	data.resize(_data.size());
	for (unsigned int y = 0; y < _height; ++y)
	{
		for (unsigned int x = 0; x < _width; ++x)
		{
			for (unsigned int c = 0; c < _channels; ++c)
			{
				data[_channels * (x + _width*(y)) + c] =
					_data[_channels * (x + _width*(_height - y - 1)) + c];
			}
		}
	}

	unsigned error = 0;
	switch (_channels)
	{
	case 1:
		error = lodepng::encode(fileName, data, _width, _height, LCT_GREY);
		break;
	case 3:
		error = lodepng::encode(fileName, data, _width, _height, LCT_RGB);
		break;
	case 4:
		error = lodepng::encode(fileName, data, _width, _height, LCT_RGBA);
		break;
	default:
		return false;
	}

	if (error != 0)
	{
		BEMBEL_LOG_ERROR()
			<< "Can't load file \"" << fileName << "\"\n"
			<< lodepng_error_text(error) << std::endl;
		return false;
	}
	return true;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
