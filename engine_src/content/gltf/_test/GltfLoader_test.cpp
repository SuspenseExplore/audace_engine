#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "FileAccessGlfw.h"
#include "GltfLoader.h"

using namespace Audace;

TEST_CASE("GltfLoader::loadFile()")
{
	Audace::AuLogger::init();
	FileAccessGlfw fileAccess;
	fileAccess.setExternalFilePath("../../engine_src/");
	GltfLoader loader;
	loader.loadFile(&fileAccess, "content/gltf/_test/", "triangle.gltf");

	// not sure what I should test here...
	CHECK(true);
}