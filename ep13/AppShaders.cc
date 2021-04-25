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
