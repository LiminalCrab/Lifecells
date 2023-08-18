// Functions, structures, enumerations
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

// Reporting & propagating errors
#include <iostream>
#include <stdexcept>

#include <vector>
#include <cstring>
#include <cstdlib> // EXIT_SUCCESS & EXIT_FAILURE

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif



class LifecellsTrianges {
public:
	void run() {
		initWindow();
		initVulkan();
		mainLoop();
		cleanup();
	}

private:
	GLFWwindow* window;

	// data member to hold the handle to the vulkan instance
	VkInstance instance;

	void initWindow()
	{

		// initalize glfw
		glfwInit();

		// don't create an OpenGL context.
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		// TODO: Handle resizing windows, disable it for now.
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		// Create the window
		window = glfwCreateWindow(WIDTH, HEIGHT, "Lifecells", nullptr, nullptr);

	}

	void initVulkan() {
		// initalize vulkan lib by creating an instance
		createInstance();
	}

	//loop that iterates until the window is closed
	void mainLoop()
	{
		while (!glfwWindowShouldClose(window)) {
			glfwPollEvents();
		}
	}

	//  deallocate the resources we've used
	void cleanup() {
		vkDestroyInstance(instance, nullptr);

		glfwDestroyWindow(window);

		glfwTerminate();
	}

	void createInstance()
	{
		// app info to optimize our specific application
		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Lifecells";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "No Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_3;

		// Which global extensions & validation layers we want to use.
		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		// include validation layer names if they're enabled
		if (enableValidationLayers)
		{
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();
		}
		else {
			createInfo.enabledLayerCount = 0;
		}

		// glfw will return the necessary extensions it needs to use to interface with windows.
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;

		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		createInfo.enabledExtensionCount = glfwExtensionCount;
		createInfo.ppEnabledExtensionNames = glfwExtensions;

		createInfo.enabledLayerCount = 0;

		//check for the success value 
		if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
			throw std::runtime_error("failed to create instance!");
		}
		
		if (enableValidationLayers && !checkValidationLayerSupport()) {
			throw std::runtime_error("validation layers requested, but not available!");
		}

		auto extensions = getRequiredExtensions();
		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();
	}

	std::vector<const char*> getRequiredExtensions() {
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		if (enableValidationLayers) {
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		return extensions;
	}

	bool checkValidationLayerSupport()
	{
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const char* layerName : validationLayers) {
			bool layerFound = false;

			for (const auto& layerProperties : availableLayers)
			{
				if (strcmp(layerName, layerProperties.layerName) == 0)
				{
					layerFound = true;
					break;
				}
			}

			if (!layerFound)
			{
				return false;
			}
		}
		return true;
	}
};

int main() {
	LifecellsTrianges app;

	try {
		app.run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}