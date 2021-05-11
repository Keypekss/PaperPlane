#include "Model.h"

Model::Model(glm::vec3 pos, glm::vec3 size, bool noTex)
	: Pos(pos), Size(size), NoTex(noTex)
{
}

void Model::render(Shader shader, bool setModel)
{
	if (setModel) {
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, Pos);
		model = glm::scale(model, Size);
		shader.setMat4("model", model);
	}

	shader.setFloat("material.shininess", 0.5f);

	for (auto & mesh : Meshes) {
		mesh.render(shader);
	}
}

void Model::cleanup()
{
	for (auto & mesh : Meshes) {
		mesh.cleanup();
	}
}

void Model::loadModel(std::string path)
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "Could not load model at " << path << std::endl << import.GetErrorString() << std::endl;
		return;
	}

	Directory = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	// process all meshes
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		Meshes.push_back(processMesh(mesh, scene));
	}

	// process all child nodes
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	// vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;

		// position
		vertex.Position = glm::vec3(
			mesh->mVertices[i].x,
			mesh->mVertices[i].y,
			mesh->mVertices[i].z
		);

		// normal vectors
		vertex.Normal = glm::vec3(
			mesh->mNormals[i].x,
			mesh->mNormals[i].y,
			mesh->mNormals[i].z
		);

		// textures
		if (mesh->mTextureCoords[0]) {
			vertex.TexCoords = glm::vec2(
				mesh->mTextureCoords[0][i].x,
				mesh->mTextureCoords[0][i].y
			);

// 			// tangent
// 			vertex.Tangent = glm::vec3(
// 				mesh->mTangents[i].x,
// 				mesh->mTangents[i].y,
// 				mesh->mTangents[i].z
// 			);
// 			
// 			// bitangent
// 			vertex.Bitangent = glm::vec3(
// 				mesh->mBitangents[i].x,
// 				mesh->mBitangents[i].y,
// 				mesh->mBitangents[i].z
// 			);

		} else {
			vertex.TexCoords = glm::vec2(0.0f);
		}

		vertices.push_back(vertex);
	}

	// process indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	// process material
	
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	if (NoTex) {
		std::cout << " mats: " << scene->mNumMaterials << std::endl;

		// diffuse color
		aiColor4D diff(1.0f);
		aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diff);

		// specular color
		aiColor4D spec(1.0f);
		aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &spec);

		// emissive color
		aiColor4D emissive(1.0f);
		aiGetMaterialColor(material, AI_MATKEY_COLOR_EMISSIVE, &emissive);

		// transparent color
		aiColor4D transparent(1.0f);
		aiGetMaterialColor(material, AI_MATKEY_COLOR_TRANSPARENT, &transparent);

		// reflective color
		aiColor4D reflective(1.0f);
		aiGetMaterialColor(material, AI_MATKEY_COLOR_REFLECTIVE, &reflective);


		std::cout << "diff: " << diff.a << " " << diff.b << " " << diff.g << std::endl;
		std::cout << "spec: " << spec.a << " " << spec.b << " " << spec.g << std::endl;
		std::cout << "emis: " << emissive.a << " " << emissive.b << " " << emissive.g << std::endl;
		std::cout << "transp: " << transparent.a << " " << transparent.b << " " << transparent.g << std::endl;
		std::cout << "reflc: " << reflective.a << " " << reflective.b << " " << reflective.g << std::endl;

		return Mesh(vertices, indices, diff, spec, emissive, transparent, reflective);

	}

	// diffuse maps
	std::vector<Texture> diffuseMaps = loadTextures(material, aiTextureType_DIFFUSE);
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

	// specular maps
	std::vector<Texture> specularMaps = loadTextures(material, aiTextureType_SPECULAR);
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

	// 3. normal maps
	std::vector<Texture> normalMaps = loadTextures(material, aiTextureType_HEIGHT);
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

	// 4. height maps
	std::vector<Texture> heightMaps = loadTextures(material, aiTextureType_AMBIENT);
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());	

	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadTextures(aiMaterial* mat, aiTextureType type)
{
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		mat->GetTexture(type, i, &str);

		// prevent duplicate loading
		bool skip = false;
		for (unsigned int j = 0; j < Textures_loaded.size(); j++) {
			if (std::strcmp(Textures_loaded[j].Path.data(), str.C_Str()) == 0) {
				textures.push_back(Textures_loaded[j]);
				skip = true;
				break;
			}
		}

		if (!skip) {
			// not loaded yet
			Texture tex(Directory, str.C_Str(), type);
			tex.load(false);
			textures.push_back(tex);
			Textures_loaded.push_back(tex);
		}
	}

	return textures;
}