#include "Application.hh"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


static const std::string MODEL_DIRECTORY = "../models/tokisaki_kurumi/";
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
        std::vector<uint32_t> indices;
        for(uint32_t iFace=0; iFace < mesh->mNumFaces; iFace++)
        {
            const aiFace& f = mesh->mFaces[iFace];
            if(f.mNumIndices == 3) for(uint32_t i=0; i < f.mNumIndices; i++) {
                indices.push_back(f.mIndices[i]);
            }
        }
        VkDeviceSize indexBufferSize = indices.size() * sizeof(indices[0]);
        meshes[iMesh].indexBuffer = this->createBuffer(
            indexBufferSize, VK_BUFFER_USAGE_INDEX_BUFFER_BIT
        );
        memcpy(meshes[iMesh].indexBuffer.mapData, indices.data(), indexBufferSize);
        meshes[iMesh].indexCount = static_cast<uint32_t>(indices.size());

        //texture
        const aiMaterial* const mat = scene->mMaterials[mesh->mMaterialIndex];
        aiString aiTexturePath;
        if(mat->GetTexture(aiTextureType_DIFFUSE, 0, &aiTexturePath) == AI_SUCCESS) {
            const std::string texPath = MODEL_DIRECTORY + aiTexturePath.data;
            meshes[iMesh].texture = this->loadTexture(texPath.c_str());
        } else throw rterr(
            ERR_MAT_NO_TEXTURE, "Loi: material khong co texture!"
        );

        //descriptor set
        VkDescriptorSetAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = descriptorPool;
        allocInfo.descriptorSetCount = 1;
        allocInfo.pSetLayouts = &dsLayout;

        VkResult result = VK_ERROR_UNKNOWN;
        vkfunc(result, "Loi: cap phat bo nho cho descriptor set khong thanh cong!",
            vkAllocateDescriptorSets(device, &allocInfo, &meshes[iMesh].descriptorSet)
        );

        std::vector<VkDescriptorBufferInfo> dbufInfos;
        dbufInfos.resize(SO_LUONG_BINDING, {});
        
        dbufInfos[BIND_ID_PROJECTION_MATRIX].buffer = ubo1.id;
        dbufInfos[BIND_ID_PROJECTION_MATRIX].range = sizeof(projMatrix);

        dbufInfos[BIND_ID_UBO2].buffer = ubo2.id;
        dbufInfos[BIND_ID_UBO2].range = sizeof(ubo2Data);

        VkDescriptorImageInfo textureInfo = {};
        textureInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        textureInfo.imageView = meshes[iMesh].texture.view;
        textureInfo.sampler = meshes[iMesh].texture.sampler;

        std::vector<VkWriteDescriptorSet> writes;
        writes.resize(SO_LUONG_BINDING);
        
        writes[BIND_ID_PROJECTION_MATRIX].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writes[BIND_ID_PROJECTION_MATRIX].descriptorCount = 1;
        writes[BIND_ID_PROJECTION_MATRIX].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        writes[BIND_ID_PROJECTION_MATRIX].pBufferInfo = &dbufInfos[BIND_ID_PROJECTION_MATRIX];
        writes[BIND_ID_PROJECTION_MATRIX].dstBinding = BIND_ID_PROJECTION_MATRIX;
        writes[BIND_ID_PROJECTION_MATRIX].dstSet = meshes[iMesh].descriptorSet;

        writes[BIND_ID_UBO2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writes[BIND_ID_UBO2].descriptorCount = 1;
        writes[BIND_ID_UBO2].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        writes[BIND_ID_UBO2].pBufferInfo = &dbufInfos[BIND_ID_UBO2];
        writes[BIND_ID_UBO2].dstBinding = BIND_ID_UBO2;
        writes[BIND_ID_UBO2].dstSet = meshes[iMesh].descriptorSet;

        writes[BIND_ID_TEXTURE].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writes[BIND_ID_TEXTURE].descriptorCount = 1;
        writes[BIND_ID_TEXTURE].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        writes[BIND_ID_TEXTURE].pImageInfo = &textureInfo;
        writes[BIND_ID_TEXTURE].dstBinding = BIND_ID_TEXTURE;
        writes[BIND_ID_TEXTURE].dstSet = meshes[iMesh].descriptorSet;
        
        vkUpdateDescriptorSets(device, 
            static_cast<uint32_t>(writes.size()), writes.data(), 0, nullptr);

        std::cout << "Model: tao vertex buffer thanh cong.\n";
    }
}
