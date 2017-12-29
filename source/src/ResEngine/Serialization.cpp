#include <ResEngine\Serialization.h>

void ResEngine::Serialization::to_json(json & j, const SerializedProperty & p) {
	using SType = SerializedProperty::ValueType;
	switch (p.GetType()) {
	case SType::Empty:
		break;
	case SType::Array:
		for (auto t = p.GetChildrenIteratorStart(); t != p.GetChildrenIteratorEnd(); t++) {
			j.push_back(*t);
		}
		break;
	case SType::Kvmap:
		for (auto t = p.GetKVMapIteratorStart(); t != p.GetKVMapIteratorEnd(); t++) {
			j[t->first] = t->second;
		}
		break;
	case SType::Bool:
		j = p.BoolValue();
		break;
	case SType::Int:
		j = p.IntValue();
		break;
	case SType::String:
		j = p.StringValue();
		break;
	default:
		break;
	}
}

void ResEngine::Serialization::from_json(const json & j, SerializedProperty & p) {

	switch (j.type()) {
	case json::value_t::array:
		for (auto& t : j) {
			SerializedProperty temp;
			from_json(t, temp);
			p.Pushback(temp);
		}
		break;
	case json::value_t::object:
		for (auto it = j.begin(); it != j.end(); it++) {
			SerializedProperty temp;
			from_json(it.value(), temp);
			p[it.key()] = temp;
		}

		break;
	case json::value_t::boolean:
		p = j.get<bool>();
		break;
	case json::value_t::number_float:
		p = j.get<float>();
		break;
	case json::value_t::number_integer:
		p = j.get<int>();
		break;
	case json::value_t::number_unsigned:
		p = j.get<int>();
		break;
	case json::value_t::string:
		p = j.get<std::string>();
		break;

	case json::value_t::discarded:
	case json::value_t::null:
	default:
		break;
	}
}
