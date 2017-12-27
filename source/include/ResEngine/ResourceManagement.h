#pragma once
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <type_traits>
#include <typeindex>

namespace ResEngine {
namespace ResourceManagement {

		using ResourceID = uint32_t;

		class ResourceManager;
		class BaseResource {
		public:
			BaseResource() {
				
			}
			ResourceID GetId() {
				return m_id;
			};
			virtual ~BaseResource() {

			}
		private:
			friend ResourceManager;
			ResourceID m_id;
		};

		template<typename T>
		class Resource : public BaseResource{
		public:
		};

		template<typename T>
		class ResourceHandle {
		public:
			ResourceHandle(uint32_t resourceID):resourceID(resourceID){
				ResourceManager::Get()._IncreaseRef(this->resourceID);
			}
			~ResourceHandle() {
				ResourceManager::Get()._DecreaseRef(this->resourceID);
			}
			uint32_t resourceID;
			T* operator->() {
				auto ptr = ResourceManager::Get().GetResource(this->resourceID);
				auto castPtr = dynamic_cast<T*>(ptr);
				return castPtr;
			}
		};

		class IImporter {
		public:
			virtual BaseResource* Import(std::string path) = 0;
			virtual std::type_index GetResourceType() = 0;
		};

		template<typename T>
		class AbstractImporter : public IImporter {
		public:
			virtual std::type_index GetResourceType() override final {
				return typeid(T);
			}
		};

		class ResourceManager {
			friend class BaseResource;
		public:
			static ResourceManager& Get(){
				static ResourceManager instance;
				return instance;
			}
			template <typename T>
			ResourceHandle<T> Load(std::string path) {
				ResourceID id;
				if (pathToID.find(path) != pathToID.end()) {
					id = pathToID[path];
				}
				else {
					id = LoadFromDisk<T>(path);
				}
				return ResourceHandle<T>(id);
			}

			BaseResource* GetResource(ResourceID resourceID) {
				return resources[resourceID].storedPtr;
			}

			void RegisterImporter(std::unique_ptr<IImporter> val) {
				importers[val->GetResourceType()] = std::move(val);
			}

			std::unordered_map<std::type_index, std::unique_ptr<IImporter>> importers;

		private:
			struct LoadedResource {
				BaseResource* storedPtr;
				int referenceCount;
			};

			template<typename T>
			friend class ResourceHandle;
			template <typename T>
			ResourceID LoadFromDisk(std::string path) {
				auto fac = importers.find(typeid(T));
				if (fac != importers.end()) {
					auto resource = fac->second->Import(path);
					auto loadedResourceStruct = LoadedResource();
					loadedResourceStruct.storedPtr = std::move(resource);
					loadedResourceStruct.referenceCount = 0;
					this->resources.push_back(
						loadedResourceStruct
					);
					pathToID[path] = resources.size() - 1;
					return resources.size() - 1;
				}
				else {
					char buffer[100];
					sprintf_s(buffer, "Can't load resource of type %s", typeid(T).name());
					throw std::exception(buffer);
				}
			}
			
			void _IncreaseRef(ResourceID id) {
				resources[id].referenceCount++;
			}
			void _DecreaseRef(ResourceID id) {
				resources[id].referenceCount--;
			}

			std::vector<LoadedResource> resources;
			std::unordered_map<std::string, ResourceID> pathToID;
		};
	}
}