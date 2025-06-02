/*
    Steps:
    Create window - done

    *Vulkan setup*
    Create instance - done
    Setup Validation layers - done 
    Select Physical Device - done 
    Select a Queue family - done 
    Create a Logical device - done
    Create a Queue - done

    *Presentation*
    Create a window surface - done
    
    *Swapchain*
    Query swapchain support details - done
    Choose swap surface format - done
    Choose presentation mode - done
    Choose swap extent (resolution) - done
    Decide image count and other properties - done
    Create the swapchain - done
    Retrieve swapchain images - done
    Create image views - done

    *Pipeline*
    Create a pipeline
*/
#include "Defs.h"
#include "Logging.h"
#include <vulkan/vulkan.h>
#include <iostream>
#include <vector>
#include "Window.h"
#include "Game.h"
#include <algorithm>
const char* validationLayers[] = { 
    "VK_LAYER_KHRONOS_validation" 
};
unsigned int enabledValidationLayers = 1;
const char* instanceExtensions[] = {
        "VK_KHR_surface",
        "VK_KHR_win32_surface",
        "VK_EXT_debug_utils"
};
unsigned int enabledInstanceExtensions = 3;
const char* deviceExtensions[] = {
    "VK_KHR_swapchain"
};
unsigned int enabledDeviceExtensions = 1;
struct SwapchainSupportDetails {
    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    std::vector<VkSurfaceFormatKHR> surfaceFormats;
    std::vector<VkPresentModeKHR> presentModes;
};
VkInstance instance;
VkDevice device;
VkSwapchainKHR swapchain;
VkSurfaceKHR surface;
std::vector<VkImageView> swapchainImageViews;
uint32_t graphicsFamilyIndex;

void createInstance(VkInstance& instance) {
    
    //Application Info
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = APPNAME;
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "My Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    //Extensions
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> extensionProperties(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensionProperties.data());
    /*
    LOG_INFO("Available instance extensions :\n");
    for (const VkExtensionProperties& extension : extensionProperties) {
        LOG_INFO(extension.extensionName << ":" << extension.specVersion << "\n");
    }
    */
    std::cout << "\n";
    LOG_INFO("Selected instance extensions:\n");
    for(int i=0;i<enabledInstanceExtensions;i++)
    LOG_INFO(instanceExtensions[i]<<"\n");
    std::cout <<"\n";
    

    //validation layers
    uint32_t layerPropCount;
    vkEnumerateInstanceLayerProperties(&layerPropCount, nullptr);
    std::vector<VkLayerProperties> layerProps(layerPropCount);
    vkEnumerateInstanceLayerProperties(&layerPropCount, layerProps.data());
    /*LOG_INFO("Available layers:");
    for (VkLayerProperties& layer : layerProps) {
        LOG_INFO(layer.layerName << "\n");
    }*/
    
    //instance creation Info
    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledLayerCount = enabledValidationLayers;
    createInfo.ppEnabledLayerNames = validationLayers;
    createInfo.enabledExtensionCount = enabledInstanceExtensions;
    createInfo.ppEnabledExtensionNames = instanceExtensions;

    //create instance
    VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);

    if (result != VK_SUCCESS) {
        LOG_ERROR("Failed to create instance");

    }
    else {
        LOG_SUCCESS("Successfully created instance\n");
        LOG_SUCCESS("Successfully setup validation layers\n");
    }
}
void getPhysicalDevice(VkPhysicalDevice& physDevice) {
    //Get Physical Devices
    uint32_t physDevCount = 0;
    vkEnumeratePhysicalDevices(instance, &physDevCount, nullptr);

    LOG_INFO(physDevCount << " Physical devices found\n");

    std::vector<VkPhysicalDevice> physicalDevices(physDevCount);
    vkEnumeratePhysicalDevices(instance, &physDevCount, physicalDevices.data());
    int i = 1;
    physDevice = physicalDevices.at(0);
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(physDevice, &deviceProperties);

    LOG_INFO(deviceProperties.deviceName << " is selected\n");
}
uint32_t FindGraphicsQueueFamilyIndex(VkPhysicalDevice device) {
    uint32_t queueFamilyCount = 0;
    VkQueueFamilyProperties properties;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
    for (int i = 0; i < queueFamilyCount; i++) {
        if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            LOG_INFO("Graphics queue family index is " << i << "\n");
            return i;
        }
    }
    std::cout << "Failed to find a graphics queue family";
    return 0;
}
void createDevice(VkDevice& device, VkPhysicalDevice& physicalDevice) {

    //Finding queue family index for graphics
    graphicsFamilyIndex = FindGraphicsQueueFamilyIndex(physicalDevice);

    uint32_t propertyCount=0;
    vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &propertyCount, nullptr);
    std::vector<VkExtensionProperties> properties(propertyCount);
    vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &propertyCount, properties.data());
    /*
    LOG_INFO("Device Extensions:---\n");
    for (const VkExtensionProperties& prop : properties) {
        LOG_INFO(prop.extensionName << "\n");
    }
    LOG_INFO("---\n");
    */
    //Queue creation info
    float queuePriority = 1.f;
    VkDeviceQueueCreateInfo queueCreateInfo = {};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.queueFamilyIndex = graphicsFamilyIndex;
    queueCreateInfo.pQueuePriorities = &queuePriority;
    //Create logical device
    VkDeviceCreateInfo deviceCreateInfo = {};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
    deviceCreateInfo.queueCreateInfoCount = 1;
    VkPhysicalDeviceFeatures deviceFeatures = {};
    deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
    deviceCreateInfo.enabledLayerCount = enabledValidationLayers;
    deviceCreateInfo.ppEnabledLayerNames = validationLayers;
    deviceCreateInfo.enabledExtensionCount = enabledDeviceExtensions;
    deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions;

    VkResult result = vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &device);

    if (result != VK_SUCCESS) {
        LOG_ERROR("Failed to create logical device");
    }
    else {
        LOG_SUCCESS("Successfully created logical device\n");
    }
}
void createWin32Surface(VkSurfaceKHR& surface,Window& window) {
    VkWin32SurfaceCreateInfoKHR surfaceCreateInfo = {};
    surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    surfaceCreateInfo.hwnd = window.get();
    surfaceCreateInfo.hinstance = GetModuleHandle(NULL);
    VkResult result = vkCreateWin32SurfaceKHR(instance, &surfaceCreateInfo, nullptr, &surface);
    if (result != VK_SUCCESS) {
        LOG_ERROR("Failed to make a vulkan win32 surface");
    }
    else {
        LOG_SUCCESS("Successfully created vulkan win32 surface\n");
    }
}
SwapchainSupportDetails querySwapchainSupport(VkPhysicalDevice& device, VkSurfaceKHR& surface) {
    SwapchainSupportDetails details;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.surfaceCapabilities);

    uint32_t formatsCount=0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatsCount, nullptr);
    details.surfaceFormats.resize(formatsCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatsCount, details.surfaceFormats.data());

    uint32_t presentModesCount=0;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModesCount, nullptr);
    details.presentModes.resize(presentModesCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModesCount, details.presentModes.data());

    return details;
}
VkSurfaceFormatKHR chooseSurfaceFormat(std::vector<VkSurfaceFormatKHR> availableFormats) {
    for (const VkSurfaceFormatKHR& format : availableFormats) {
        if (format.format == VK_FORMAT_B8G8R8A8_UNORM && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return format;
        }
    }
    return availableFormats[0];
}
VkPresentModeKHR choosePresentMode(std::vector<VkPresentModeKHR> presentModes) {
    for (const VkPresentModeKHR& presentMode : presentModes) {
        if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return presentMode;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}
VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, uint32_t windowWidth, uint32_t windowHeight) {
    VkExtent2D extent;
    extent.width = windowWidth;
    extent.height = windowHeight;

    if (capabilities.currentExtent.width != UINT32_MAX) {
        return capabilities.currentExtent;
    }
    else {
        
        extent.width = std::clamp(extent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        extent.height = std::clamp(extent.height,capabilities.minImageExtent.height,capabilities.maxImageExtent.height);
        return extent;
    }
}
void createSwapchain(VkDevice& device,VkPhysicalDevice& physicalDevice,VkSurfaceKHR& surface, VkSwapchainKHR& swapchain,uint32_t graphicsFamilyIndex,uint32_t windowWidth,uint32_t windowHeight) {
    ASSERT(device != VK_NULL_HANDLE);
    ASSERT(surface != VK_NULL_HANDLE);
    ASSERT(physicalDevice != VK_NULL_HANDLE);
    SwapchainSupportDetails details = querySwapchainSupport(physicalDevice, surface);
    if (details.surfaceFormats.empty() || details.presentModes.empty()) {
        LOG_WARN("Surface format or present mode is empty\n");
    }
    VkSurfaceFormatKHR format = chooseSurfaceFormat(details.surfaceFormats);
    VkPresentModeKHR presentMode = choosePresentMode(details.presentModes);
    VkExtent2D extent = chooseSwapExtent(details.surfaceCapabilities,windowWidth,windowHeight);

    VkBool32 supported = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, graphicsFamilyIndex, surface, &supported);
    if (!supported) {
        LOG_WARN("Surface not supported by the selected queue family\n");
    }

    uint32_t imageCount = details.surfaceCapabilities.minImageCount + 1;
    if (details.surfaceCapabilities.maxImageCount > 0 && details.surfaceCapabilities.maxImageCount < imageCount) {
        imageCount = details.surfaceCapabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = format.format;
    createInfo.imageExtent = extent;
    createInfo.imageColorSpace = format.colorSpace;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    uint32_t queueFamilyIndices[] = {graphicsFamilyIndex};

    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;
    createInfo.presentMode = presentMode;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.queueFamilyIndexCount = 0;
    createInfo.pQueueFamilyIndices = nullptr;
    createInfo.preTransform = details.surfaceCapabilities.currentTransform;

    VkResult result = vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapchain);
    if (result != VK_SUCCESS) {
        LOG_ERROR("Could not create Swapchain");
    }
    else {
        LOG_SUCCESS("Successfully created Swapchain\n");
    }
}
void createSwapchainImageViews(VkDevice device,VkSwapchainKHR& swapchain,std::vector<VkImageView>& swapchainImageViews) {
    VkResult result = VK_SUCCESS;
    //Get swapchain images`
    uint32_t imageCount;
    vkGetSwapchainImagesKHR(device, swapchain, &imageCount, nullptr);
    std::vector<VkImage> swapchainImages(imageCount);
    vkGetSwapchainImagesKHR(device, swapchain, &imageCount, swapchainImages.data());

    swapchainImageViews.clear();
    swapchainImageViews.resize(swapchainImages.size());

    for (const VkImage& image : swapchainImages) {
        VkImageViewCreateInfo viewCreateInfo{};
        viewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewCreateInfo.image = image;
        viewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewCreateInfo.format = VK_FORMAT_B8G8R8A8_UNORM;

        viewCreateInfo.components = {
            VK_COMPONENT_SWIZZLE_IDENTITY,
            VK_COMPONENT_SWIZZLE_IDENTITY,
            VK_COMPONENT_SWIZZLE_IDENTITY,
            VK_COMPONENT_SWIZZLE_IDENTITY
        };

        viewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        viewCreateInfo.subresourceRange.baseMipLevel = 0;
        viewCreateInfo.subresourceRange.levelCount = 1;
        viewCreateInfo.subresourceRange.baseArrayLayer = 0;
        viewCreateInfo.subresourceRange.layerCount = 1;

        VkImageView imageView;
        result = vkCreateImageView(device, &viewCreateInfo, nullptr, &imageView);
        if (result != VK_SUCCESS) {
            LOG_ERROR("Could not create swapchain image views");
            return;
        }
        swapchainImageViews.push_back(imageView);
    }
    if (result != VK_SUCCESS) {
        LOG_ERROR("Could not create swapchain image views");
    }
    else {
        LOG_SUCCESS("Successfully created swapchain image views\n");
    }
}
void VulkanSetup(Window& window) {
    //Create instance
    createInstance(instance);

    //Get physical device
    VkPhysicalDevice selectedDevice;
    getPhysicalDevice(selectedDevice);

    //Creating Logical Device
    createDevice(device, selectedDevice);

    //Creating Device Queue
    VkQueue graphicsQueue;
    vkGetDeviceQueue(device, graphicsFamilyIndex, 0, &graphicsQueue);
    LOG_SUCCESS("Successfully created Graphics Queue\n");

    //Creating vulkan win32 surface
    createWin32Surface(surface, window);

    //Create swapchain
    RenderState* renderState = window.getRenderState();
    createSwapchain(device, selectedDevice, surface, swapchain, graphicsFamilyIndex, renderState->width, renderState->height);

    createSwapchainImageViews(device, swapchain, swapchainImageViews);
}
void VulkanFree() {
    for (VkImageView imageView : swapchainImageViews) {
        vkDestroyImageView(device, imageView, nullptr);
    }
    vkDestroySwapchainKHR(device, swapchain, nullptr);
    vkDestroySurfaceKHR(instance, surface, nullptr);
    vkDestroyDevice(device, nullptr);
    vkDestroyInstance(instance, nullptr);
}
int main() {

    Window window = Window(APPNAME, 720, 720);
    
    window.addConsole();
    
    VulkanSetup(window);

    while (window.isOpen()) {
        window.clear(0x000000);
        window.processMessages(window.input);
        window.swapBuffers();
    }
    
    VulkanFree();
	return 0;
}