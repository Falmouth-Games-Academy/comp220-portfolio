#include "stdafx.h"
#include "Renderer.h" // Vertex (might be moved laer on)
#include "Model.h"

#include <vector>
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include <iostream>

#include "glm/gtx/transform.hpp"
#include "glm/gtx/euler_angles.hpp"

bool Model::Create(const char* filename, bool doPrecalculateInstances) {
	// Load the scene from the file
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_GenUVCoords | aiProcess_CalcTangentSpace | (doPrecalculateInstances ? aiProcess_PreTransformVertices : 0));

	if (!scene) {
		printf("Model Loading Error - %s\n", importer.GetErrorString());
		return false;
	}

	// Copy the vertices and indices from the scene
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	for (int i = 0; i < scene->mNumMeshes; i++) {
		aiMesh *currentMesh = scene->mMeshes[i];

		// Create this mesh section
		MeshSection section;
		section.startIndex = indices.size();
		section.numIndices = currentMesh->mNumFaces * 3;
		section.textureIndex = currentMesh->mMaterialIndex;
		meshSections.push_back(section);

		// Load the vertices
		int vertexBase = vertices.size();
		vertices.resize(vertexBase + currentMesh->mNumVertices);

		for (int v = 0; v < currentMesh->mNumVertices; v++) {
			aiVector3D currentModelVertex = currentMesh->mVertices[v];
			aiColor4D currentModelColour = aiColor4D(1.0, 1.0, 1.0, 1.0);
			aiVector3D currentTextureCoordinates = aiVector3D(0.0f, 0.0f, 0.0f);
			aiVector3D currentVertexNormal = aiVector3D(1.0f, 0.0f, 0.0f);

			// Load colours
			if (currentMesh->HasVertexColors(0)) {
				currentModelColour = currentMesh->mColors[0][v];
			}
			
			// Load UVs
			if (currentMesh->HasTextureCoords(0)) {
				currentTextureCoordinates = currentMesh->mTextureCoords[0][v];
			}

			// Load normals
			if (currentMesh->HasNormals()) {
				currentVertexNormal = currentMesh->mNormals[v];
			}

			// Push the vertex into the vector
			Vertex currentVertex = {
				currentModelVertex.x, currentModelVertex.y, currentModelVertex.z,
				currentModelColour.r, currentModelColour.g, currentModelColour.b,
				currentVertexNormal.x, currentVertexNormal.y, currentVertexNormal.z, 
				currentTextureCoordinates.x, currentTextureCoordinates.y
			};

			currentVertex.boneIndices[0] = 255;

			vertices[vertexBase + v] = currentVertex;
		}

		// Load faces/vertex indices
		int indexBase = indices.size();
		indices.resize(indexBase + currentMesh->mNumFaces * 3);

		for (int f = 0; f < currentMesh->mNumFaces; f++) {
			aiFace currentModelFace = currentMesh->mFaces[f];

			indices[indexBase + f * 3] = currentModelFace.mIndices[0] + vertexBase;
			indices[indexBase + f * 3 + 1] = currentModelFace.mIndices[1] + vertexBase;
			indices[indexBase + f * 3 + 2] = currentModelFace.mIndices[2] + vertexBase;
		}

		// Load bones
		aiBone* const* aiBones = currentMesh->mBones;
		const int maxBonesPerVertex = 4;
		
		bones.resize(bones.size() + currentMesh->mNumBones);

		for (int b = 0; b < currentMesh->mNumBones; b++) {
			int bIndex = b + (bones.size() - currentMesh->mNumBones);
			Bone& newBone = bones[bIndex];
			
			// Copy name and bind pose
			newBone.index = bIndex;
			newBone.name = aiBones[b]->mName.C_Str();
			newBone.parent = nullptr;

			for (int j = 0; j < 16; j++) {
				bones[b].bindPose[j%4][j/4] = aiBones[b]->mOffsetMatrix[j/4][j%4];
			}

			// Copy vertex weights
			newBone.vertexWeights.resize(aiBones[b]->mNumWeights);
			for (int w = 0; w < aiBones[b]->mNumWeights; w++) {
				int vIndex = vertexBase + aiBones[b]->mWeights[w].mVertexId;

				newBone.vertexWeights[w].index = vIndex;
				newBone.vertexWeights[w].weight = aiBones[b]->mWeights[w].mWeight;

				// Update the vertex which uses this bone
				if (newBone.vertexWeights[w].weight > 0.0f) {
					for (int boneIndex = 0; boneIndex < maxBonesPerVertex; boneIndex++) {
						// Replace the lowest -1 with this vertex and shift the -1 to the next one
						if (vertices[vIndex].boneIndices[boneIndex] == 255) {
							vertices[vIndex].boneIndices[boneIndex] = bIndex;
							vertices[vIndex].boneWeights[boneIndex] = newBone.vertexWeights[w].weight; 

							if (boneIndex < maxBonesPerVertex - 1) {
								vertices[vIndex].boneIndices[boneIndex + 1] = 255;
							}

							break;
						}
					}
				}
			}
		}
	}

	// Load textures
	for (int m = 0; m < scene->mNumMaterials; m++) {
		// Get the path of the texture
		aiMaterial* material = scene->mMaterials[m];
		aiString path;

		material->GetTexture(aiTextureType_DIFFUSE, 0, &path);

		// If the path name is relative, insert the model path into it
		std::string pathString = path.C_Str();

		if (true /*pathString[0] == '/' || pathString[0] == '\\'*/) {
			std::string modelFolder = std::string(filename);
			int lastSlashInModelName = modelFolder.find_last_of("/\\");

			if (lastSlashInModelName != std::string::npos) {
				modelFolder.resize(lastSlashInModelName);
			}

			pathString.insert(0, modelFolder + "\\");
		}

		// Add it to the texture name list
		textureNames.push_back(pathString);
	}

	// Load animations
	for (int a = 0; a < scene->mNumAnimations; a++) {
		const aiAnimation* aiAnim = scene->mAnimations[a];
		double ticksPerSecond = aiAnim->mTicksPerSecond != 0.0f ? aiAnim->mTicksPerSecond : 30.0f;
		Anim newAnimation;

		newAnimation.nodes.resize(aiAnim->mNumChannels);
		for (int c = 0; c < aiAnim->mNumChannels; c++) {
			AnimNode& newNode = newAnimation.nodes[c];
			const aiNodeAnim* channel = aiAnim->mChannels[c];

			// Add the node to the channel
			newNode.targetName = channel->mNodeName.C_Str();
			newNode.target = nullptr;

			// Try and find the target bone
			for (Bone& b : bones) {
				if (b.name == newNode.targetName) {
					newNode.target = &b;
				}
			}

			// Copy the translation and rotation keys
			newNode.translation.resize(channel->mNumPositionKeys);
			newNode.rotation.resize(channel->mNumRotationKeys);

			for (int k = 0; k < channel->mNumPositionKeys; k++) {
				newNode.translation[k].time = channel->mPositionKeys[k].mTime / aiAnim->mTicksPerSecond;
				newNode.translation[k].vec = glm::vec3(channel->mPositionKeys[k].mValue.x, channel->mPositionKeys[k].mValue.y, channel->mPositionKeys[k].mValue.z);
			}
			
			for (int k = 0; k < channel->mNumRotationKeys; k++) {
				newNode.rotation[k].time = channel->mRotationKeys[k].mTime / aiAnim->mTicksPerSecond;
				newNode.rotation[k].quat = glm::quat(channel->mRotationKeys[k].mValue.w, channel->mRotationKeys[k].mValue.x, channel->mRotationKeys[k].mValue.y, channel->mRotationKeys[k].mValue.z);
			}
		}

		animations.push_back(newAnimation);
	}

	// Assign bone parents
	const int maxNodeLevel = 32;
	aiNode* currentNode = scene->mRootNode;
	int childIndices[maxNodeLevel] = { 0 };
	int currentLevel = 1;

	// Begin depth-first search
	while (currentNode) {
		// If there are more nodes to explore at this level, dive in!
		if (childIndices[currentLevel] < currentNode->mNumChildren) {
			currentNode = currentNode->mChildren[childIndices[currentLevel]];

			childIndices[currentLevel++]++;
			childIndices[currentLevel] = 0;
			continue;
		}

		// See if any bones match this node
		if (currentNode->mParent) {
			Bone* childBone = FindBoneByName(currentNode->mName.C_Str());
			Bone* parentBone = FindBoneByName(currentNode->mParent->mName.C_Str());
			
			if (childBone && parentBone) {
				childBone->parent = parentBone;
			}
		}

		// Exit this level
		currentLevel--;
		currentNode = currentNode->mParent;
	}

	// Copy the results to our array
	this->vertices = new Vertex[vertices.size()];
	this->indices = new unsigned int[indices.size()];
	this->numVertices = vertices.size();
	this->numIndices = indices.size();

	memcpy(this->vertices, vertices.data(), vertices.size() * sizeof(this->vertices[0]));
	memcpy(this->indices, indices.data(), indices.size() * sizeof(this->indices[0]));

	// Initialise variables
	areBuffersCreated = false;

	return true;
}

void Model::Destroy() {
	// Cleanup and nullify resources
	bones.clear();
	animations.clear();
	textureNames.clear();
	meshSections.clear();

	delete[] vertices;
	delete[] indices;

	vertices = nullptr;
	indices = nullptr;
}

float Model::FindKeyframe(const std::vector<AnimNode::Keyframe>& keyframeList, float currentKeyframe, float time) {
	// Find the keyframes surrounding 'time'
	int startIndex = currentKeyframe, numIndices = keyframeList.size();
	int direction = 1;
	const AnimNode::Keyframe* keyframes = keyframeList.data(); // array conversion for faster debugging

	for (int i = startIndex; i < numIndices - 1 && i >= 0; i += direction) {
		if (keyframes[i].time <= time) {
			direction = 1; // time is too low, check i+1 next time

			if (keyframes[i + 1].time > time) {
				// Return a blended index based on the current time respective to the keyframe times
				return i + (time - keyframes[i].time) / (keyframes[i + 1].time - keyframes[i].time);
			}
		} else if (keyframes[i].time >= time) {
			direction = -1; // time is too high, check i-1 next time
		}
	}

	// No precise keyframe found. Clamp the frame at the max or min frame index
	if (keyframeList.size() > 0 && keyframeList[keyframeList.size() - 1].time < time) {
		return keyframeList.size() - 1;
	} else {
		return 0.0f;
	}
}

void Model::PoseBones(float time) {
	for (Anim& anim : animations) {
		for (AnimNode& node : anim.nodes) {
			if (node.target) {
				//node.translationKeyframeIndex = 0.0f;
				//node.rotationKeyframeIndex = 0.0f;
				node.scaleKeyframeIndex = 0.0f;

				// Find keyframes that are closest to the current time
				node.rotationKeyframeIndex = FindKeyframe(node.rotation, node.rotationKeyframeIndex, time);
				node.translationKeyframeIndex = FindKeyframe(node.translation, node.translationKeyframeIndex, time);
			}
		}
	}
}

void Model::CalculateBoneMatrices(glm::mat4 finalBoneMatrices[32]) {
	glm::mat4 rootBoneMatrices[32];
	const Bone* bonePointers[32] = { 0 };

	finalBoneMatrices[0] = glm::identity<glm::mat4>(); // Initialise the first bone matrix in case this model isn't animated

	// Calculate the local space of each bone
	for (Anim& anim : animations) {
		// Create the current bone's bone-to-world matrix
		for (AnimNode& node : anim.nodes) {
			if (node.target) {
				int matrixIndex = node.target->index;

				// Linearly interpolate the animated values
				glm::quat rotation;
				glm::vec3 translation;

				// .. but only if they exist
				if (node.rotationKeyframeIndex + 1 < node.rotation.size()) {
					rotation = glm::lerp(node.rotation[(int)node.rotationKeyframeIndex].quat, node.rotation[(int)node.rotationKeyframeIndex + 1].quat, node.rotationKeyframeIndex - (int)node.rotationKeyframeIndex);
				} else {
					rotation = glm::identity<glm::quat>();
				}

				if (node.translationKeyframeIndex + 1 < node.translation.size()) {
					translation = glm::lerp(node.translation[(int)node.translationKeyframeIndex].vec, node.translation[(int)node.translationKeyframeIndex + 1].vec, node.translationKeyframeIndex - (int)node.translationKeyframeIndex);
				} else {
					translation = glm::vec3(0.0f, 0.0f, 0.0f);
				}

				// Generate the local matrix
				rootBoneMatrices[matrixIndex] = glm::mat4(rotation);
				rootBoneMatrices[matrixIndex] = glm::translate(translation) * rootBoneMatrices[matrixIndex];
				bonePointers[matrixIndex] = node.target;
			}
		}
	}

	// Multiply every bone's matrix along its parent chain
	for (int i = 0; i < 32; i++) {
		if (bonePointers[i]) {
			// Iterate along the bone parents whilst multiplying the matrix each step
			const Bone* currentBone = bonePointers[i]->parent;

			finalBoneMatrices[i] = rootBoneMatrices[i];

			while (currentBone) {
				finalBoneMatrices[i] = rootBoneMatrices[currentBone->index] * finalBoneMatrices[i];
				currentBone = currentBone->parent;
			}

			// Finally, multiply by the bind pose matrix (the inverse of the original bone-to-world matrix in the bind pose)
			// This means vertices are transformed to their original bone space, then back to current bone space in the world
			finalBoneMatrices[i] = finalBoneMatrices[i] * bonePointers[i]->bindPose;
		}
	}
}

void Model::Render(Renderer& renderer, const ShaderProgram& shaderProgram, const Texture* const* textures) {
	// Create the buffers if they don't already exist
	if (!areBuffersCreated) {
		static VertexFormat vertexFormat(&Vertex::position, &Vertex::colour, &Vertex::normal, &Vertex::uvs, &Vertex::boneIndices, &Vertex::boneWeights);

		vertexBuffer.Create(renderer, vertexFormat, vertices, numVertices * sizeof (Vertex));
		indexBuffer.Create(renderer, indices, numIndices * sizeof (unsigned int));

		areBuffersCreated = true;
	}

	// Bind the buffers
	renderer.UseVertexBuffer(&vertexBuffer);
	renderer.UseIndexBuffer(&indexBuffer);

	// Calculate local bone matrices
	glm::mat4 finalBoneMatrices[32];

	CalculateBoneMatrices(finalBoneMatrices);

	// Send them to the shader
	shaderProgram.SetUniforms("boneTransforms[0]", finalBoneMatrices, 32);

	// Begin model render
	renderer.UseShaderProgram(shaderProgram);

	if (!textures) {
		// Render the model as a single mesh
		renderer.DrawTrianglesIndexed(0, numIndices);
	} else {
		// Render each mesh individually (model may have multiple textures)
		for (MeshSection& section : meshSections) {
			renderer.UseTexture(textures[section.textureIndex], &shaderProgram);
			renderer.DrawTrianglesIndexed(section.startIndex, section.numIndices);
		}
	}
}