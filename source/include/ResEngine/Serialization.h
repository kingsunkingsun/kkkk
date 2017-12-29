#pragma once
#include <ResEngine\Serialization\json.hpp>
#include <glm\glm.hpp>
#include <unordered_map>

namespace ResEngine {
	namespace Serialization {

class SerializedProperty {
public:
	enum class ValueType {
		Empty,
		Int,
		Bool,
		Float,
		String,
		Array,
		Kvmap
	};
	template <typename T>
	SerializedProperty(const std::initializer_list<std::pair<std::string,T>>& list) {
		for (auto& t : list) {
			this->operator[t.first] = t.second;
		}
	}
	
	template <typename T>
	SerializedProperty(const std::initializer_list<T>& list) {
		this->children.resize(list.size());
		for (auto& t : list) {
			Pushback(t);
		}
	}

	SerializedProperty(int val) {
		operator=(val);
	}

	SerializedProperty(float val) {
		operator=(val);
	}

	SerializedProperty() {}

	~SerializedProperty() {};

	SerializedProperty(const SerializedProperty& copy) {
		this->operator=(copy);
	};

	SerializedProperty(SerializedProperty&& move) {
		this->boolValue = move.boolValue;
		this->intValue = move.intValue;
		this->stringValue = move.stringValue;
		this->floatValue = move.floatValue;
		this->children = std::move(move.children);
		this->kvmap = std::move(move.kvmap);
	}

	SerializedProperty& operator=(const SerializedProperty& val) {
		this->boolValue = val.boolValue;
		this->intValue = val.intValue;
		this->stringValue = val.stringValue;
		this->floatValue = val.floatValue;
		this->children = val.children;
		this->kvmap = val.kvmap;
		return *this;
	}

	SerializedProperty& operator=(bool val) {
		DetermineType(ValueType::Bool);
		this->boolValue = val;
		return *this;
	}

	SerializedProperty& operator=(int val) {
		DetermineType(ValueType::Int);
		this->intValue = val;
		return *this;
	}

	SerializedProperty& operator=(std::string val) {
		DetermineType(ValueType::String);
		this->stringValue = val;
		return *this;
	}

	SerializedProperty& operator=(float val) {
		DetermineType(ValueType::Float);
		this->floatValue = val;
		return *this;
	}
	
	SerializedProperty& operator=(glm::vec4 vec) {
		DetermineType(ValueType::Kvmap);
		operator[]("x") = vec.w;
		operator[]("y") = vec.y;
		operator[]("z") = vec.z;
		operator[]("w") = vec.w;
		return *this;
	}

	SerializedProperty& operator[](int index) {
		DetermineType(ValueType::Array);
		if (children.size() <= index) {
			children.resize(index + 1);
		}
		return children[index];
	}

	SerializedProperty& operator[](std::string key) {
		DetermineType(ValueType::Kvmap);
		return kvmap[key];
	}

	SerializedProperty& operator[](const char* key) {
		DetermineType(ValueType::Kvmap);
		return kvmap[key];
	}

	size_t Count() {
		if (this->type == ValueType::Array)
			return children.size();
		if (this->type == ValueType::Kvmap)
			return kvmap.size();
	}

	std::unordered_map<std::string, SerializedProperty>& GetKVMapRef() {
		DetermineType(ValueType::Kvmap);
		return kvmap;
	}
	std::vector<SerializedProperty>& GetChildrenRef() {
		DetermineType(ValueType::Array);
		return children;
	}

	const auto GetChildrenIteratorStart() const {
		return this->children.cbegin();
	}
	const auto GetChildrenIteratorEnd() const {
		return this->children.cend();
	}

	const auto GetKVMapIteratorStart() const {
		return this->kvmap.cbegin();
	}
	const auto GetKVMapIteratorEnd() const {
		return this->kvmap.cend();
	}

	operator int() const {
		return intValue;
	}

	operator float() const {
		return floatValue;
	}

	operator std::string() const {
		return stringValue;
	}

	operator bool() const {
		return boolValue;
	}

	int IntValue() const {
		return intValue;
	}

	float FloatValue() const {
		return floatValue;
	}

	std::string StringValue() const {
		return stringValue;
	}

	bool BoolValue() const {
		return boolValue;
	}

	template <typename T>
	void Pushback(T val) {
		DetermineType(ValueType::Array);
		SerializedProperty t = val;
		this->children.push_back(t);
	}

	ValueType GetType() const {
		return type;
	}
private:
	ValueType type = ValueType::Empty;
	float floatValue;
	bool boolValue;
	std::string stringValue;
	int intValue;
	std::vector<SerializedProperty> children;  
	std::unordered_map<std::string, SerializedProperty> kvmap;

	void DetermineType(ValueType type) {
		if (this->type != ValueType::Empty && this->type != type) {
			//it's already determined! what're you doing!
			throw std::exception("Type error");
		}
		this->type = type;
	}
};

using json = nlohmann::json;

void to_json(json& j, const SerializedProperty& p);

void from_json(const json& j, SerializedProperty& p);

class Serializer {
public:
	using json = nlohmann::json;

	class SerializerNode {
	private:

		template<typename TValue>
		void Serialize(const char* id, std::unordered_map<std::string,TValue>& m, char){
			this->jsonNode->operator[](id) = SerializedProperty();
			auto& newSPNodeRef = this->jsonNode->operator[](id);
			for (auto& t : m) {
				auto newNode = SerializerNode(&newSPNodeRef);
				newNode.Serialize(t.first.c_str(), t.second,0);
			}
		}

		//if T has its own serialize method.
		template<typename TMap>
		auto Serialize(const char* id, TMap& m,int) -> decltype((m.SerializeFunc(*this)),void())
		{
			this->jsonNode->operator[](id) = SerializedProperty();
			auto& newSPNodeRef = this->jsonNode->operator[](id);
			m.SerializeFunc(SerializerNode(&newSPNodeRef));
		}

		//otherwise we try to use our own.
		template<typename TMap>
		auto Serialize(const char* id, TMap& m,long) -> decltype((jsonNode->operator[](id)), void())
		{
			jsonNode->operator[](id) = m;
		}

		template<typename TMap> 
		void Serialize(...) {
			static_assert(false,"Can't find serialization method!");
		}

	public:
		SerializerNode(SerializedProperty* jsonNode) {
			this->jsonNode = jsonNode;
		}

		template<typename TType>
		void operator()(const char* id, TType & t) {
			Serialize(id, t, 0);
		}

	private:
		SerializedProperty* jsonNode;
	};

	template<typename T>
	void Serialize(T target) {
		target.SerializeFunc(SerializerNode(&m_jsonData));
	}

	Serializer() = default;

	~Serializer() = default;

	std::string Data() {
		return json(m_jsonData).dump();
	}

	SerializedProperty RawData() {
		return m_jsonData;
	}

private:
	SerializedProperty m_jsonData;
};

class Deserializer {
public:
	using json = nlohmann::json;

	Deserializer(SerializedProperty data) : m_jsonData(data) {

	}

	class DeserializerNode {
	public:
		DeserializerNode(SerializedProperty* jsonNode) {
			this->jsonNode = jsonNode;
		}

		void operator()(const char* id, glm::vec4& t) {
			t.x = (*jsonNode)[id]["x"];
			t.y = (*jsonNode)[id]["y"];
			t.z = (*jsonNode)[id]["z"];
			t.w = (*jsonNode)[id]["w"];
		}

		void operator()(const char* id, float& t) {
			t = (*jsonNode)[id];
		}

		void operator()(const char* id, std::string& t) {
			t = (*jsonNode)[id];
		}

		void operator()(const char* id, int& t) {
			t = (*jsonNode)[id];
		}

		template<typename TType>
		void operator()(const char* id, TType & t) {
			auto node = DeserializerNode(&(*jsonNode)[id]);
			t.SerializeFunc(node);
		}

	private:
		SerializedProperty* jsonNode;
	};

	template<typename T>
	void Deserialize(T& target) {
		target.SerializeFunc(DeserializerNode(&m_jsonData));
	}

private:
	SerializedProperty m_jsonData;
private:
};
	}
}