

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

	constexpr int num_lights = 3;
	for( int i = 0; i < num_lights; ++i )
	{
		xml::Element* light = doc.NewElement("Entity");
		entities->InsertEndChild(light);
		xml::Element* dir_light = doc.NewElement("DirectionalLight");
		xml::SetAttribute(dir_light, "color", "1.0 1.0 1.0");
		xml::SetAttribute(dir_light, "intensity", "1.5");
		float angle = glm::radians(360.f*i/num_lights);
		glm::vec3 dir = glm::normalize(glm::vec3(sin(angle), -1, cos(angle)));

		static char buffer[256];
		sprintf(buffer, "%f %f %f", dir.x, dir.y, dir.z);
		xml::SetAttribute(dir_light, "direction", buffer);
		light->InsertEndChild(dir_light);
	}

	constexpr int num_spheres = 15;
	for( int i = 0; i <= num_spheres; ++i )
	{
		for( int j = 0; j <= num_spheres; ++j )
		{
			std::stringstream mat_name; 
			mat_name << "mat_0" << (i<10 ? "0" : "") << i << (j<10 ? "_0" : "_") << j;
			xml::Element* mat = doc.NewElement("Material");
			xml::SetAttribute(mat, "name", mat_name.str());
			xml::SetAttribute(mat, "renderer", "default");
			xml::SetAttribute(mat, "emission", "0.003 0.0 0.0");
			xml::SetAttribute(mat, "albedo", "1.0 0.0 0.0");
			xml::SetAttribute(mat, "roughness", 0.1f + 0.8f*i/(num_spheres-1));
			xml::SetAttribute(mat, "metallic", 0.0f + 1.0f*j/(num_spheres-1));
			xml::SetAttribute(mat, "f0", 0.04f);
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
			xml::SetAttribute(position, "x", 2*i - num_spheres);
			xml::SetAttribute(position, "y", -0.5f);
			xml::SetAttribute(position, "z", 2*j - num_spheres);
			entity->InsertEndChild(position);
			xml::Element* geom = doc.NewElement("GeometryComponent");
			xml::SetAttribute(geom, "model", model_name.str());
			entity->InsertEndChild(geom);
		}
	}

	doc.SaveFile("scene.scene");
	return true;
}