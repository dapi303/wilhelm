#include <gtest/gtest.h>

#include "../src/loader/obj.h"

TEST(parseVertex, BasicAssertions) {
  ModelData model;
  const std::string entry = "v 115.899498 -18.661787 81.354202";

  ASSERT_TRUE(processVertex(model, entry.c_str()));

  ASSERT_EQ(model.vertex.size(), 1);
  ASSERT_EQ(model.vertex[0]->x, 115.899498f);
  ASSERT_EQ(model.vertex[0]->y, -18.661787f);
  ASSERT_EQ(model.vertex[0]->z, 81.354202f);
}

TEST(parseThreeVertex, BasicAssertions) {
  ModelData model;
  const std::string entry1 = "v 115.899498 -18.661787 81.354202";
  const std::string entry2 = "v 215.899498 -28.661787 82.354202";
  const std::string entry3 = "v 315.899498 -38.661787 83.354202";

  ASSERT_TRUE(processVertex(model, entry1.c_str()));
  ASSERT_TRUE(processVertex(model, entry2.c_str()));
  ASSERT_TRUE(processVertex(model, entry3.c_str()));

  ASSERT_EQ(model.vertex.size(), 3);

  ASSERT_EQ(model.vertex[0]->x, 115.899498f);
  ASSERT_EQ(model.vertex[0]->y, -18.661787f);
  ASSERT_EQ(model.vertex[0]->z, 81.354202f);

  ASSERT_EQ(model.vertex[1]->x, 215.899498f);
  ASSERT_EQ(model.vertex[1]->y, -28.661787f);
  ASSERT_EQ(model.vertex[1]->z, 82.354202f);

  ASSERT_EQ(model.vertex[2]->x, 315.899498f);
  ASSERT_EQ(model.vertex[2]->y, -38.661787f);
  ASSERT_EQ(model.vertex[2]->z, 83.354202f);
}

TEST(processEntryProperlyParsesVertexes, BasicAssertions) {
  ModelData model;
  const std::string entry1 = "v 115.1 -18.2 81.3";

  ASSERT_TRUE(processVertex(model, entry1.c_str()));

  ASSERT_EQ(model.vertex.size(), 1);
  ASSERT_EQ(model.vertex[0]->x, 115.1f);
  ASSERT_EQ(model.vertex[0]->y, -18.2f);
  ASSERT_EQ(model.vertex[0]->z, 81.3f);
}

TEST(failIfFaceCannotBeParsedDueToMissingVertex, BasicAssertions) {
  ModelData model;
  const std::string entry = "f 1 2 3";
  ASSERT_FALSE(processFace(model, entry));
}

TEST(parseSingleTriangleFace, BasicAssertions) {
  ModelData model;
  model.vertex.push_back(std::make_shared<Vertex3>(Vertex3(1, 2, 3)));
  model.vertex.push_back(std::make_shared<Vertex3>(Vertex3(10, 20, 30)));
  model.vertex.push_back(std::make_shared<Vertex3>(Vertex3(100, 200, 300)));

  const std::string entry = "f 1 2 3";

  ASSERT_TRUE(processFace(model, entry));
  ASSERT_EQ(model.face.size(), 1);
  ASSERT_EQ(model.face[0]->vertex.size(), 3);

  ASSERT_EQ(model.face[0]->vertex[0]->x, 1.0f);
  ASSERT_EQ(model.face[0]->vertex[0]->y, 2.0f);
  ASSERT_EQ(model.face[0]->vertex[0]->z, 3.0f);

  ASSERT_EQ(model.face[0]->vertex[1]->x, 10.0f);
  ASSERT_EQ(model.face[0]->vertex[1]->y, 20.0f);
  ASSERT_EQ(model.face[0]->vertex[1]->z, 30.0f);

  ASSERT_EQ(model.face[0]->vertex[2]->x, 100.0f);
  ASSERT_EQ(model.face[0]->vertex[2]->y, 200.0f);
  ASSERT_EQ(model.face[0]->vertex[2]->z, 300.0f);
}

TEST(processEntryProperlyParsesVertex, BasicAssertions) {
  const std::string entry = "v 1.0 2.0 3.0";
  ModelData model;
  ASSERT_TRUE(processEntry(model, entry));
  ASSERT_EQ(model.vertex.size(), 1);
  ASSERT_EQ(model.vertex[0]->x, 1.0f);
  ASSERT_EQ(model.vertex[0]->y, 2.0f);
  ASSERT_EQ(model.vertex[0]->z, 3.0f);
}

TEST(processEntryProperlyParsesVertexAndFace, BasicAssertions) {
  const std::string entry1 = "v 1 2 3";
  const std::string entry2 = "f 1 1 1";

  ModelData model;
  ASSERT_TRUE(processEntry(model, entry1));
  ASSERT_TRUE(processEntry(model, entry2));

  ASSERT_EQ(model.face.size(), 1);
  ASSERT_EQ(model.face[0]->vertex.size(), 3);
  ASSERT_EQ(model.face[0]->vertex[0]->x, 1.0f);
  ASSERT_EQ(model.face[0]->vertex[1]->y, 2.0f);
  ASSERT_EQ(model.face[0]->vertex[2]->z, 3.0f);
}

TEST(processEntryProperlyMatchesVertexForFaces, BasicAssertions) {
  const std::string entry1 = "v 1 2 3";
  const std::string entry2 = "f 1 1 1";

  ModelData model;
  ASSERT_TRUE(processEntry(model, entry1));
  ASSERT_TRUE(processEntry(model, entry2));

  ASSERT_EQ(model.face.size(), 1);
  ASSERT_EQ(model.face[0]->vertex.size(), 3);
  ASSERT_EQ(model.face[0]->vertex[0]->x, 1.0f);
  ASSERT_EQ(model.face[0]->vertex[1]->y, 2.0f);
  ASSERT_EQ(model.face[0]->vertex[2]->z, 3.0f);
}

TEST(parseVT, BasicAssertions) {
  const std::string entry = "vt 0.001 0.2323";
  ModelData model;
  ASSERT_TRUE(processEntry(model, entry));
  ASSERT_EQ(model.vertexTexture.size(), 1);
  ASSERT_EQ(model.vertexTexture[0]->x, 0.001f);
  ASSERT_EQ(model.vertexTexture[0]->y, 0.2323f);
}

TEST(faceUsesProperVertexTexture, BasicAssertions) {
  ModelData model;
  model.vertexTexture.push_back(std::make_shared<Vertex2>(0.001f, 0.002f));
  model.vertexTexture.push_back(std::make_shared<Vertex2>(0.011f, 0.012f));
  model.vertexTexture.push_back(std::make_shared<Vertex2>(0.021f, 0.022f));

  model.vertex.push_back(std::make_shared<Vertex3>(10, 20, 30));

  const std::string entry = "f 1/1 1/2 1/3";
  ASSERT_TRUE(processEntry(model, entry));
  ASSERT_EQ(model.face.size(), 1);
  ASSERT_EQ(model.face[0]->vertexTexture.size(), 3);

  ASSERT_EQ(model.face[0]->vertexTexture[0]->x, 0.001f);
  ASSERT_EQ(model.face[0]->vertexTexture[0]->y, 0.002f);

  ASSERT_EQ(model.face[0]->vertexTexture[1]->x, 0.011f);
  ASSERT_EQ(model.face[0]->vertexTexture[1]->y, 0.012f);

  ASSERT_EQ(model.face[0]->vertexTexture[2]->x, 0.021f);
  ASSERT_EQ(model.face[0]->vertexTexture[2]->y, 0.022f);
}
