#include "Application.hh"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


static const char* const MODEL_FILE_PATH = "../models/tokisaki_kurumi/kurumi.gltf";


void Application::loadModel()
{
    Assimp::Importer imp;
    const aiScene* const scene = imp.ReadFile(
        MODEL_FILE_PATH, aiProcess_Triangulate
    );
    if(
        (scene == nullptr) ||
        (scene->mRootNode == nullptr) ||
        (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
    ) throw rterr(
        ERR_MODEL_LOAD_FAIL, "Loi: khong the doc file model 3D!"
    );

    meshes.resize(scene->mNumMeshes);

    for(uint32_t iMesh = 0; iMesh < meshes.size(); iMesh++)
    {
        const aiMesh* const mesh = scene->mMeshes[iMesh];
        std::cout << "Tim thay mesh: " << mesh->mName.data << '\n';

        //vertex buffer
        std::vector<Vertex> vertices;
        vertices.resize(mesh->mNumVertices);
        for(uint32_t iVertex=0; iVertex < mesh->mNumVertices; iVertex++)
        {
            const aiVector3D& pos = mesh->mVertices[iVertex];
            
            vertices[iVertex].position = glm::vec3(pos.x, pos.y, pos.z);
            vertices[iVertex].color = mesh->HasVertexColors(0)
                ? glm::vec3(
                    mesh->mColors[0][iVertex].r,
                    mesh->mColors[0][iVertex].g,
                    mesh->mColors[0][iVertex].b
                ) : glm::vec3(1.0f);
            vertices[iVertex].textureCoord = mesh->HasTextureCoords(0)
                ? glm::vec2(
                    mesh->mTextureCoords[0][iVertex].x,
                    mesh->mTextureCoords[0][iVertex].y
                )
                : glm::vec2(0.0f);
        }
        VkDeviceSize vertexBufferSize = vertices.size() * sizeof(vertices[0]);
        meshes[iMesh].vertexBuffer = this->createBuffer(
            vertexBufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT
        );
        memcpy(meshes[iMesh].vertexBuffer.mapData, vertices.data(), vertexBufferSize);

        //index buffer
        //texture
        //descriptor set

        std::cout << "Model: tao vertex buffer thanh cong.\n";
    }
}
