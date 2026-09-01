#pragma once
#include <vector>
#include <string>

class Mesh;

class ModelNode {
public:
	std::vector<Mesh*> currentMeshs;
	std::vector<ModelNode*> childNodes;
	std::string modelName;
};