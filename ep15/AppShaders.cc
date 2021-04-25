#include "Application.hh"
#include <fstream>


void Application::loadShaders()
{
    vertMod = this->createShaderMod("basic-vert.spirv");
    fragMod = this->createShaderMod("basic-frag.spirv");
}


VkShaderModule Application::createShaderMod(const char* filepath)
{
    VkShaderModule ret = VK_NULL_HANDLE;

    std::ifstream file(filepath, std::ios::ate | std::ios::binary);
    if(file.fail()) throw rterr(ERR_VK_SHADER_MOD,
        "Loi: khong the doc file shader mod!");

    size_t codeSize = file.tellg();
    std::vector<char> code;
    code.resize(codeSize, '\0');
    file.seekg(0);
    file.read(code.data(), codeSize);
    file.close();

    VkShaderModuleCreateInfo modInfo = {};
    modInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    modInfo.codeSize = codeSize;
    modInfo.pCode = reinterpret_cast<uint32_t*>(code.data());
    VkResult result = VK_ERROR_UNKNOWN;
    vkfunc(result, "Loi: tao shader mod khong thanh cong!",
        vkCreateShaderModule(device, &modInfo, nullptr, &ret)
    );

    std::cout << "Tao shader mod thanh cong.\n";
    return ret;
}


void Application::createPipelineLayout()
{

    VkPipelineLayoutCreateInfo plInfo = {};
    plInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

    VkResult result = VK_ERROR_UNKNOWN;
    vkfunc(result, "Loi: tao pipeline layout khong thanh cong!",
        vkCreatePipelineLayout(device, &plInfo, nullptr, &pipelineLayout)
    );

    std::cout << "Tao pipeline layout thanh cong.\n";
}


void Application::createGraphicsPipeline()
{
    std::vector<VkPipelineShaderStageCreateInfo> stages;
    const size_t STAGES = 2, VERT = 0, FRAG = 1;
    stages.resize(STAGES, {});
    stages[VERT].sType = 
        VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    stages[VERT].stage = VK_SHADER_STAGE_VERTEX_BIT;
    stages[VERT].module = vertMod;
    stages[VERT].pName = "main";
    
    stages[FRAG].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;    
    stages[VERT].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    stages[VERT].module = fragMod;
    stages[VERT].pName = "main";

    VkPipelineVertexInputStateCreateInfo vertexInputState = {};
    vertexInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

    VkPipelineInputAssemblyStateCreateInfo inputAsmState = {};
    inputAsmState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAsmState.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

    VkGraphicsPipelineCreateInfo pipelineInfo = {};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = static_cast<uint32_t>(stages.size());
    pipelineInfo.pStages = stages.data();
    pipelineInfo.pVertexInputState = &vertexInputState;
    pipelineInfo.pInputAssemblyState = &inputAsmState;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pColorBlendState = &colorBlendState;
    pipelineInfo.pDepthStencilState = &depthStencilState;
    pipelineInfo.pMultisampleState = &msaaState;
    pipelineInfo.pRasterizationState = &rasterState;
    pipelineInfo.renderPass = renderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.layout = pipelineLayout;
    pipelineInfo.basePipelineIndex = -1;

    VkResult result = VK_ERROR_UNKNOWN;
    vkfunc(result, "Loi: tao graphics pipeline khong thanh cong!",
        vkCreateGraphicsPipelines(device, VK_NULL_HANDLE,
            1, &pipelineInfo, nullptr, &graphicsPipeline)
    );
}