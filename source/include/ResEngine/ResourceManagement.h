#pragma once
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <type_traits>
#include <typeindex>

namespace ResEngine {
namespace ResourceManagement {

		using RuntimeResourceID = uint32_t;
		using UniqueResourceID = std::string;	//currently we just use resource's path as its unique ID.

		class ResourceManager;
		class BaseResourceHandle;
		template <typename T>
		class ResourceHandle;

		class BaseResource {
		public:
			BaseResource() {
				
			}
			virtual ~BaseResource() {

			}
		private:
			friend ResourceManager;
			RuntimeResourceID m_id;
		};

		template<typename T>
		class Resource : public BaseResource{
		public:
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
			friend BaseResource;
			friend BaseResourceHandle;
			template <typename T>
			friend class ResourceHandle;
		public:
			static ResourceManager& Get(){
				static ResourceManager instance;
				return instance;
			}

			template <typename T>
			ResourceHandle<T> Create() {
				RuntimeResourceID id = resources.size();
				LoadedResource temp;
				temp.referenceCount = 0;
				temp.storedPtr = new T();
				temp.uniqueIdentifier = "RuntimeResource";	//it's same for everyone! 
				resources.push_back(
					temp
				);
				return ResourceHandle<T>(id);
			}
			
			template <typename T>
			ResourceHandle<T> Load(std::string path) {
				RuntimeResourceID id;
				if (pathToID.find(path) != pathToID.end()) {
					id = pathToID[path];
				}
				else {
					id = LoadFromDisk<T>(path);
				}
				return ResourceHandle<T>(id);
			}

			BaseResource* GetResourcePtr(RuntimeResourceID resourceID) {
				return resources[resourceID].storedPtr;
			}

			void RegisterImporter(std::unique_ptr<IImporter> val) {
				importers[val->GetResourceType()] = std::move(val);
			}

			std::unordered_map<std::type_index, std::unique_ptr<IImporter>> importers;

		private:
			struct LoadedResource {
				UniqueResourceID uniqueIdentifier;
				BaseResource* storedPtr;
				int referenceCount;
			};

			UniqueResourceID GetResourceUniqueID(RuntimeResourceID runtimeID) {
				return resources[runtimeID].uniqueIdentifier;
			}

			friend BaseResourceHandle;
			template <typename T>
			RuntimeResourceID LoadFromDisk(std::string path) {
				auto fac = importers.find(typeid(T));
				if (fac != importers.end()) {
					auto resource = fac->second->Import(path);
					auto loadedResourceStruct = LoadedResource();
					loadedResourceStruct.storedPtr = std::move(resource);
					loadedResourceStruct.referenceCount = 0;
					loadedResourceStruct.uniqueIdentifier = path;
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
			
			void _IncreaseRef(RuntimeResourceID id) {
				resources[id].referenceCount++;
			}
			void _DecreaseRef(RuntimeResourceID id) {
				resources[id].referenceCount--;
			}

			std::vector<LoadedResource> resources;
			std::unordered_map<std::string, RuntimeResourceID> pathToID;
		};

		class BaseResourceHandle {
		public:
			BaseResourceHandle() :resourceID(InvalidResourceID) {
				IncreaseRefSafe();
			}
			BaseResourceHandle(RuntimeResourceID resourceID) :resourceID(resourceID) {
				IncreaseRefSafe();
			}
			virtual ~BaseResourceHandle() {
				DecreaseRefSafe();
			}
		protected:
			const static RuntimeResourceID InvalidResourceID = -1;
			void SetRuntimeResourceID(RuntimeResourceID id) {
				DecreaseRefSafe();
				this->resourceID = id;
				IncreaseRefSafe();
			}
			RuntimeResourceID GetRuntimeResourceID() {
				return resourceID;
			}
		private:
			void DecreaseRefSafe() {
				if (resourceID != InvalidResourceID) {
					ResourceManager::Get()._IncreaseRef(this->resourceID);
				}
			}
			void IncreaseRefSafe() {
				if (resourceID != InvalidResourceID) {
					ResourceManager::Get()._IncreaseRef(this->resourceID);
				}
			}
			RuntimeResourceID resourceID;
		};

		template<typename T>
		class ResourceHandle : public BaseResourceHandle {
		public:
			ResourceHandle(RuntimeResourceID id) : BaseResourceHandle(id) {

			}
			ResourceHandle() : BaseResourceHandle() {

			}

			T* operator->() {
				auto ptr = ResourceManager::Get().GetResourcePtr(GetRuntimeResourceID());
				auto castPtr = dynamic_cast<T*>(ptr);
				return castPtr;
			}

			T& operator*() {
				auto ptr = ResourceManager::Get().GetResourcePtr(GetRuntimeResourceID());
				auto castPtr = dynamic_cast<T*>(ptr);
				return *castPtr;
			}

			template <typename T>
			void SerializeFunc(T& arc) {
				BeforeSerialization();
				arc("serializedPath", serializedPath);
				UpdateAfterDeserialize();
			}
		private:
			std::string serializedPath;
			void BeforeSerialization() {
				auto runtimeID = GetRuntimeResourceID();
				if (runtimeID != InvalidResourceID)
					serializedPath = ResourceManager::Get().GetResourceUniqueID(GetRuntimeResourceID());
			}

			void UpdateAfterDeserialize() {
				auto t = ResourceManager::Get().Load<T>(serializedPath);
				SetRuntimeResourceID(t.GetRuntimeResourceID());
			}
		};
	}
}