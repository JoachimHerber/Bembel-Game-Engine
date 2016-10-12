/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "AssetManager.h"

#include <BembelBase/Logging/Logger.h>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/

namespace bembel{

AssetManager::AssetManager()
{}

AssetManager::~AssetManager()
{}

unsigned AssetManager::LoadAssets(const std::string& fileName)
{
	xml::Document doc;
	if (doc.LoadFile(fileName.c_str()) != tinyxml2::XML_SUCCESS)
	{
		BEMBEL_LOG_ERROR()
			<< "Failed to load file '" << fileName << "'\n"
			<< doc.ErrorName() << std::endl;
		return false;
	}

	const xml::Element* root = doc.FirstChildElement("Assets");
	if (!root)
	{
		BEMBEL_LOG_ERROR()
			<< "File '" << fileName << "' has no root element 'Assets'"
			<< std::endl;
		return false;
	}

	unsigned loadedAsssets = 0;
	for (const xml::Element* element : xml::IterateChildElements(root))
	{
		auto it = _assetTypeMap.find(element->Value());
		if (it == _assetTypeMap.end())
			continue; // unknown asset type

		if (!_assetLoader[it->second])
			continue; // no loader for the the asset type

		if (_assetLoader[it->second]->CreateAsset(element))
			++loadedAsssets;
	}
	return loadedAsssets;
}

int AssetManager::GetAssetRefCount(AssetHandle handle)
{
	if (handle.typeId < _assetContainer.size())
		return _assetContainer[handle.typeId]->GetAssetRefCount(handle);

	return -1;
}
void AssetManager::IncrementAssetRefCount(AssetHandle handle)
{
	if (handle.typeId < _assetContainer.size())
		_assetContainer[handle.typeId]->IncrementAssetRefCount(handle);
}
void AssetManager::DecrementAssetRefCount(AssetHandle handle)
{
	if (handle.typeId < _assetContainer.size())
		_assetContainer[handle.typeId]->DecrementAssetRefCount(handle);
}


} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
