#ifndef BEMBEL_IMAGE_H
#define BEMBEL_IMAGE_H

/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelConfig.h>

#include <vector>
#include <string>

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel{

class BEMBEL_API Image
{
public:
	Image();
	Image(unsigned int w, unsigned int h, unsigned int c);
	~Image();

	unsigned char* GetData() ;
	const unsigned char* GetData() const;

	unsigned int GetWidth() const;
	unsigned int GetHeight() const;
	unsigned int GetChannels() const;

	bool Load(const std::string& fileName);
	bool Save(const std::string& fileName);

private:
	std::vector<unsigned char> _data;

	unsigned int _width;
	unsigned int _height;
	unsigned int _channels;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE		                                                          */
/*============================================================================*/
#endif // include guard
