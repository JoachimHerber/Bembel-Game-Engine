/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "image.h"
#include "lodepng/lodepng.h"

#include "bembel-base/logging/logger.h"

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

Image::Image()
	: width_(0)
	, height_(0)
	, num_channels_(0)
{}

Image::Image(unsigned int w, unsigned int h, unsigned int c)
	: width_(w)
	, height_(h)
	, num_channels_(c)
{
	data_.resize(width_*height_*num_channels_);
}

Image::~Image()
{}

unsigned char* Image::GetData()
{
	return &(data_[0]);
}

const unsigned char* Image::GetData() const
{
	return &(data_[0]);
}

unsigned int Image::GetWidth() const
{
	return width_;
}

unsigned int Image::GetHeight() const
{
	return height_;
}

unsigned int Image::GetChannels() const
{
	return num_channels_;
}

bool Image::Load(const std::string& file_name, bool invert_y_axis)
{
	std::vector<unsigned char> data;
	unsigned int w, h;
	unsigned error = lodepng::decode(data, w, h, file_name);

	if( error != 0 )
	{
		BEMBEL_LOG_ERROR()
			<< "Can't load file \"" << file_name << "\"\n"
			<< lodepng_error_text(error) << std::endl;
		return false;
	}

	// copy data

	width_ = w;
	height_ = h;
	num_channels_ = 4;
	data_.resize(w*h * 4);

	if( invert_y_axis )
	{
		for( unsigned int y = 0; y < h; ++y )
		{
			for( unsigned int x = 0; x < w; ++x )
			{
				data_[4 * (x + w*(y)) + 0] = data[4 * (x + w*(h - y - 1)) + 0];
				data_[4 * (x + w*(y)) + 1] = data[4 * (x + w*(h - y - 1)) + 1];
				data_[4 * (x + w*(y)) + 2] = data[4 * (x + w*(h - y - 1)) + 2];
				data_[4 * (x + w*(y)) + 3] = data[4 * (x + w*(h - y - 1)) + 3];
			}
		}
	}
	else
	{
		data_ = data;
	}

	return true;
}

bool Image::Save(const std::string& file_name, bool invert_y_axis)
{
	std::vector<unsigned char> data;
	if( invert_y_axis )
	{
		data.resize(data_.size());
		for( unsigned int y = 0; y < height_; ++y )
		{
			for( unsigned int x = 0; x < width_; ++x )
			{
				for( unsigned int c = 0; c < num_channels_; ++c )
				{
					data[num_channels_ * (x + width_*(y)) + c] =
						data_[num_channels_ * (x + width_*(height_ - y - 1)) + c];
				}
			}
		}
	}
	else
	{
		data = data_;
	}

	unsigned error = 0;
	switch( num_channels_ )
	{
	case 1:
		error = lodepng::encode(file_name, data_, width_, height_, LCT_GREY);
		break;
	case 3:
		error = lodepng::encode(file_name, data_, width_, height_, LCT_RGB);
		break;
	case 4:
		error = lodepng::encode(file_name, data_, width_, height_, LCT_RGBA);
		break;
	default:
		return false;
	}

	if( error != 0 )
	{
		BEMBEL_LOG_ERROR()
			<< "Can't load file \"" << file_name << "\"\n"
			<< lodepng_error_text(error) << std::endl;
		return false;
	}
	return true;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
