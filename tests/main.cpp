#include <gtest\gtest.h>
#include <ResEngine\ResourceManagement.h>

TEST(ResourceManagement, SuccessLoad) {
	using namespace ResEngine::ResourceManagement;
	ASSERT_NO_THROW(
		ResourceManager::Get().RegisterImporter(std::make_unique<TextImporter>());
		auto handle = ResourceManager::Get().Load<TextResource>("test.txt");
	);
}

TEST(ResourceManagement, DoubleLoadOneResource) {
	using namespace ResEngine::ResourceManagement;
	auto handle2 = ResourceManager::Get().Load<TextResource>("test.txt");
	auto handle3 = ResourceManager::Get().Load<TextResource>("test.txt");
	ASSERT_TRUE(handle2.resourceID == handle3.resourceID);
}

void main(int argc,char* args[]) {
	testing::InitGoogleTest(&argc,args);
	RUN_ALL_TESTS();
	system("pause");
}