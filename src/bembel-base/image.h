#ifndef BEMBEL_BASE_IMAGE_H_
#define BEMBEL_BASE_IMAGE_H_

/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "bembel-config.h"

#include <vector>
#include <string>

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class BEMBEL_API Image
{
public:
	Image();
	Image(unsigned int w, unsigned int h, unsigned int c);
	~Image();

	unsigned char* GetData();
	const unsigned char* GetData() const;

	unsigned int GetWidth() const;
	unsigned int GetHeight() const;
	unsigned int GetChannels() const;

	bool Load(const std::string& file_name, bool invert_y_axis = true);
	bool Save(const std::string& file_name, bool invert_y_axis = true);

private:
	std::vector<unsigned char> data_;

	unsigned int width_;
	unsigned int height_;
	unsigned int num_channels_;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE		                                                          */
/*============================================================================*/
#endif // include guard
