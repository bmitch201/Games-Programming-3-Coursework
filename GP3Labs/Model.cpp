#include "pch.h"
#include "Model.h"
#include "Common.h"

void Model::Load()
{
	//importer should release all resources the moment it goes out of scope
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(m_directory, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		LOG_DEBUG(importer.GetErrorString(), Log::Error);
		return;
	}

	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		m_meshes.push_back(processMesh(mesh, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh* Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<int> indices;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		//process vertex positions, normals, colors and textures

		vertex.pos.x = mesh->mVertices[i].x;
		vertex.pos.y = mesh->mVertices[i].y;
		vertex.pos.z = mesh->mVertices[i].z;

		if (mesh->HasNormals())
		{
			vertex.normal.x = mesh->mNormals[i].x;
			vertex.normal.y = mesh->mNormals[i].y;
			vertex.normal.z = mesh->mNormals[i].z;
		}

		if (mesh->mColors[0])
		{
			vertex.color.r = mesh->mColors[0][i].r;
			vertex.color.g = mesh->mColors[0][i].g;
			vertex.color.b = mesh->mColors[0][i].b;
			vertex.color.a = mesh->mColors[0][i].a;
		}
		else
		{
			vertex.color = glm::vec4(1.f);
		}

		if (mesh->mTextureCoords[0])//does the mesh contatin texture coordinates?
		{
			vertex.texture.x = mesh->mTextureCoords[0][i].x;
			vertex.texture.y = mesh->mTextureCoords[0][i].y;
		}
		else
		{
			vertex.texture = glm::vec2(0.0f, 0.0f);
		}

		if (mesh->HasTangentsAndBitangents())
		{
			vertex.tangent.x = mesh->mTangents[i].x;
			vertex.tangent.y = mesh->mTangents[i].y;
			vertex.tangent.z = mesh->mTangents[i].z;

			//LOG_DEBUG(std::to_string(vertex.tangent.x) + ", " + std::to_string(vertex.tangent.y) + ", " + std::to_string(vertex.tangent.z), Log::Trace);
		}
		else
		{
			LOG_DEBUG("No Tangents loaded", Log::Warning);	
		}
	
		vertices.push_back(vertex);

	}

	//process indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}

			//LOG_DEBUG(std::to_string(indices.size()), Log::Trace);

			/*for (unsigned int i = 0; i < indices.size(); i += 3)
			{
				Vertex& vertex0 = vertices[indices[i]];
				Vertex& vertex1 = vertices[indices[i + 1]];
				Vertex& vertex2 = vertices[indices[i + 2]];

				glm::vec3 edge1 = vertex1.pos - vertex0.pos;
				glm::vec3 edge2 = vertex2.pos - vertex0.pos;

				float deltaU1 = vertex1.texture.x - vertex0.texture.x;
				float deltaV1 = vertex1.texture.y - vertex0.texture.y;
				float deltaU2 = vertex2.texture.x - vertex0.texture.x;
				float deltaV2 = vertex2.texture.y - vertex0.texture.y;

				float f = 1.f / (deltaU1 * deltaV2 - deltaU2 * deltaV1);

				glm::vec3 t, bt;

				bt.x = f * (-deltaU2 * edge1.x - deltaU1 * edge2.x);
				bt.y = f * (-deltaU2 * edge1.y - deltaU1 * edge2.y);
				bt.z = f * (-deltaU2 * edge1.x - deltaU1 * edge2.x);

				if (!mesh->HasTangentsAndBitangents())
				{
					t.x = f * (deltaV2 * edge1.x - deltaV1 * edge2.x);
					t.y = f * (deltaV2 * edge1.y - deltaV1 * edge2.y);
					t.z = f * (deltaV2 * edge1.z - deltaV1 * edge2.z);

					vertex0.tangent += t;
					vertex1.tangent += t;
					vertex2.tangent += t;				
					
					LOG_DEBUG(std::to_string(t.x) + ", " + std::to_string(t.y) + ", " + std::to_string(t.z), Log::Trace);
				}

				vertex0.bitangent += bt;
				vertex1.bitangent += bt;
				vertex2.bitangent += bt;
			}*/
	}

	return new Mesh(vertices, indices);
}

Model::Model(std::string path)
{
	m_directory = path;
	Load();
}