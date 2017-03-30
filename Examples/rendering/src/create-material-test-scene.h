

#include <bembel-base/xml.h>
#include <sstream>

bool CreateMaterialTestScene()
{
	using namespace bembel;

	xml::Document doc;

	xml::Element* root = doc.NewElement("Scene");
	doc.InsertEndChild(root);

	xml::Element* assets = doc.NewElement("Assets");
	root->InsertEndChild(assets);

	xml::Element* entities = doc.NewElement("Entities");
	root->InsertEndChild(entities);

	xml::Element* light = doc.NewElement("Entity");
	entities->InsertEndChild(light);
	xml::Element* dir_light = doc.NewElement("DirectionalLight");
	xml::SetAttribute(dir_light, "color", "1.0 1.0 1.0");
	xml::SetAttribute(dir_light, "intensity", "1.5");
	xml::SetAttribute(dir_light, "direction", "1 -5 2");
	light->InsertEndChild(dir_light);

	for( int i = 5; i <= 95; i += 5 )
	{
		for( int j = 5; j <= 95; j += 5 )
		{
			std::stringstream mat_name; 
			mat_name << "mat_0" << (i<10 ? "0" : "") << i << (j<10 ? "_00" : "_0") << j;
			xml::Element* mat = doc.NewElement("Material");
			xml::SetAttribute(mat, "name", mat_name.str());
			xml::SetAttribute(mat, "renderer", "default");
			xml::SetAttribute(mat, "albedo", "0.0 0.0 0.0");
			char reflectivity[256];
			sprintf(reflectivity, "%.2f %.2f %.2f", 0.01f*i, 0.01f*i, 0.01f*i);
			xml::SetAttribute(mat, "reflectivity", reflectivity);
			xml::SetAttribute(mat, "roughness", 0.01f*float(j));
			assets->InsertEndChild(mat);

			std::stringstream model_name; 
			model_name << "sphere_0" << (i<10 ? "0" : "") << i << (j<10 ? "_00" : "_0") << j;
			xml::Element* model = doc.NewElement("GeometryModel");
			xml::SetAttribute(model, "name", model_name.str());
			xml::SetAttribute(model, "mesh", "basic-shapes.geom-mesh");
			assets->InsertEndChild(model);
			xml::Element* mapping = doc.NewElement("MaterialMapping");
			xml::SetAttribute(mapping, "material", mat_name.str());
			xml::SetAttribute(mapping, "submesh", "sphere");
			model->InsertEndChild(mapping);

			xml::Element* entity = doc.NewElement("Entity");
			entities->InsertEndChild(entity);

			xml::Element* position = doc.NewElement("Position");
			xml::SetAttribute(position, "x", 0.4f*(i - 50));
			xml::SetAttribute(position, "y", -1);
			xml::SetAttribute(position, "z", 0.4f*(j - 50));
			entity->InsertEndChild(position);
			xml::Element* geom = doc.NewElement("GeometryComponent");
			xml::SetAttribute(geom, "model", model_name.str());
			entity->InsertEndChild(geom);
		}
	}

	doc.SaveFile("scene.scene");
	return true;
}