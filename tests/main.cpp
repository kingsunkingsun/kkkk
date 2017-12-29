#include <gtest\gtest.h>
#include <ResEngine\TextAsset.h>
#include <ResEngine\Serialization.h>
#include <ResEngine\Material.h>
#include <vector>

TEST(ResourceManagement, SuccessLoad) {
	using namespace ResEngine;
	ASSERT_NO_THROW(
		ResourceManager::Get().RegisterImporter(std::make_unique<TextImporter>());
		auto handle = ResourceManager::Get().Load<TextResource>("test.txt");
	);
}

TEST(ResourceManagement, DoubleLoadOneResource) {
	using namespace ResEngine;
	auto handle2 = ResourceManager::Get().Load<TextResource>("test.txt");
	auto handle3 = ResourceManager::Get().Load<TextResource>("test.txt");
}

class SubStruct {
public:
	int field1;
	int field2;
	template<typename TArchive>
	void SerializeFunc(TArchive& arc) {
		arc("field1", field1);
		arc("field2", field2);
	}
};

class TargetStruct {
public:
	int field1;
	int field2;
	SubStruct field3;
	std::vector<SubStruct> values;
	template<typename TArchive>
	void SerializeFunc(TArchive& arc) {
		arc("field1", field1);
		arc("field2", field2);
		arc("values", values);
		arc("subStruct", field3);
	}
};

using namespace ResEngine::Serialization;
TEST(Serialization, 1) {
	
	SubStruct tempSub = {
		1,2
	};
	TargetStruct temp = {
		1,2,tempSub
	};
	Serializer serializer;
	serializer.Serialize(temp);

	TargetStruct receiver;
	Deserializer deserializer(serializer.RawData());
	deserializer.Deserialize(receiver);

	ASSERT_TRUE(receiver.field1 == 1);
	ASSERT_TRUE(receiver.field2 == 2);
}

TEST(MaterialLoad, 1) {
	using namespace ResEngine;
	auto vshader = ResourceManager::Get().Load<TextResource>("Resources\vertex.glsl");
	auto fshader = ResourceManager::Get().Load<TextResource>("Resources\fragment.glsl");
	
	//create material at runtime!
	auto mat = ResourceManager::Get().Create<Material>();

	mat->fragmentShader = fshader;
	mat->vertexShader = vshader;

	//can we save the material to disk?
	Serializer temp;
	temp.Serialize(*mat);

	std::cout << temp.Data() << std::endl;
}

void main(int argc,char* args[]) {
	testing::InitGoogleTest(&argc,args);
	RUN_ALL_TESTS();
	system("pause");
}