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

	template<typename T>
	T GetValue() const {
		static_assert(false, "Unsupported type");
	}
	
	template<>
	bool GetValue<bool>() const {
		return BoolValue();
	}

	template<>
	std::string GetValue<std::string>() const {
		return StringValue();
	}

	template<>
	float GetValue<float>() const {
		return FloatValue();
	}

	template<>
	int GetValue<int>() const {
		return IntValue();
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

	template<typename TValue>
	static auto Serialize(SerializedProperty& to,TValue& m, unsigned char) -> decltype(
		m.begin(),
		m.end(),
		Serialize(to,*m.begin(),0),
		void()) {

		int index = 0;
		for (auto& t : m) {
			to.Pushback(SerializedProperty());
			auto spRef = to[index++];
			Serialize(spRef, t, 0);
		}
	}

	template<typename TValue>
	static void Serialize(SerializedProperty& to, std::unordered_map<std::string, TValue>& m, char) {
		for (auto& t : m) {
			auto& sp = (to[t.first] = SerializedProperty());
			Serialize(sp, t.second, 0);
		}
	}

	//if T has its own serialize method.
	template<typename TMap>
	static auto Serialize(SerializedProperty& to, TMap& m, int) -> decltype(
		m.SerializeFunc(SerializerNode(to)), 
		void()
		) 
	{
		m.SerializeFunc(SerializerNode(to));
	}

	//otherwise we try to use our own.
	template<typename TMap>
	static auto Serialize(SerializedProperty& to, TMap& m, long) -> decltype((to = m), void()) {
		to = m;
	}

	//juses, it's so stupid that std::string acts like a vector.
	static void Serialize(SerializedProperty& to, std::string& m, long){
		to = m;
	}

	template<typename TMap>
	void Serialize(...) {
		static_assert(false, "Can't find serialization method!");
	}

	class SerializerNode {
	private:

	public:
		SerializerNode(SerializedProperty& node) : m_node(node) {
		}

		template<typename TType>
		void operator()(const char* id, TType & t) {
			auto& temp = (m_node[id] = SerializedProperty());
			Serializer::Serialize(temp, t, 0);
		}

	private:
		SerializedProperty& m_node;
	};

	template<typename T>
	void Serialize(T target) {
		Serialize(this->m_jsonData, target, 0);
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
	Deserializer(SerializedProperty data) : m_rootNode(data) {

	}

	template<typename TValue>
	void Deserialize(TValue& t) {
		Deserialize(m_rootNode, t, 0);
	}

private:
	SerializedProperty m_rootNode;

	template<typename TValue>
	static auto Deserialize(SerializedProperty& to, TValue& m, char) -> decltype(
		m.begin(),
		m.end(),
		Deserialize(to, *m.begin(), 0),
		void()) {

		int index = 0;
		for (auto& t : to.GetChildrenRef()) {
			std::decay_t<decltype(*m.begin())> val;
			Deserialize(t, val, 0);
			m.push_back(val);
		}
	}

	template<typename TValue>
	static void Deserialize(SerializedProperty& to, std::unordered_map<std::string, TValue>& m, char) {
		for (auto& t : to.GetKVMapRef()) {
			m[t.first] = t.second.GetValue<TValue>();
		}
	}

	//if T has its own serialize method.
	template<typename TMap>
	static auto Deserialize(SerializedProperty& to, TMap& m, int) -> decltype(
		m.SerializeFunc(DeserializerNode(to)),
		void()
		) {
		m.SerializeFunc(DeserializerNode(to));
	}

	//otherwise we try to use our own.
	template<typename TMap>
	static auto Deserialize(SerializedProperty& to, TMap& m, long) -> decltype(
		m = to, 
		void()
		) 
	{
		m = to;
	}

	template<typename TMap>
	static void Deserialize(...) {
		static_assert(false, "Can't find serialization method!");
	}

	class DeserializerNode {
	public:
		DeserializerNode(SerializedProperty& node) : m_node(node) {
		}

		template<typename TType>
		void operator()(const char* id, TType & t) {
			Deserializer::Deserialize(m_node[id],t , 0);
		}

	private:
		SerializedProperty& m_node;
	}; 
};
}
}