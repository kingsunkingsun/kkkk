#pragma once

class ISerializable {
public:

	virtual void SerializeFunc() = 0;
};

class Serializer {
public:
	template<typename TType>
	Serializer& S(TType t) {
		t.SerializeFunc(*this);
	}
	Serializer& S(int& t) {
		_data << t;
	}
	std::string Data() {
		return _data.str();
	}
private:
	stringstream _data;
};

class Deserializer {
public:
	template<typename TType>
	Serializer& S(TType t) {
		t.SerializeFunc(*this);
	}
	Deserializer(std::string data) : _data(data){
			
	}
	Deserializer& S(int& t) {
		_data >> t;
	}
	std::string Data() {
		return _data.str();
	}
private:
	stringstream _data;
};

