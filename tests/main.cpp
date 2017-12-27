#include <gtest\gtest.h>
#include <ResEngine\TextAsset.h>
#include <ResEngine\Serialization.h>

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
	ASSERT_TRUE(handle2.resourceID == handle3.resourceID);
}

TEST(Serialization, 1) {
	class TargetStruct {
	public:
		int field1;
		template<typename TArchive>
		TArchive& SerializeFunc(TArchive& arc) {
			arc.S(field1);
		}
	};

	TargetStruct temp;
	Serializer serializer;
	serializer.S(temp);
}

void main(int argc,char* args[]) {
	testing::InitGoogleTest(&argc,args);
	RUN_ALL_TESTS();
	system("pause");
}