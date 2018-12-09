#include "stdafx.h"
#include "Renderer.h" // Vertex (might be moved laer on)
#include "Model.h"

#include <vector>
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include <iostream>

bool Model::Create(const char* filename) {
	// Load the scene from the file
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_GenUVCoords | aiProcess_CalcTangentSpace);

	if (!scene) {
		printf("Model Loading Error - %s\n", importer.GetErrorString());
		return false;
	}

	// Copy the vertices and indices from the scene
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	for (int i = 0; i < scene->mNumMeshes; i++) {
		aiMesh *currentMesh = scene->mMeshes[i];

		// Load vertices
		vertices.resize(currentMesh->mNumVertices);

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

			vertices[v] = currentVertex;
		}

		// Load faces/vertex indices
		for (int f = 0; f < currentMesh->mNumFaces; f++) {
			aiFace currentModelFace = currentMesh->mFaces[f];

			indices.push_back(currentModelFace.mIndices[0]);
			indices.push_back(currentModelFace.mIndices[1]);
			indices.push_back(currentModelFace.mIndices[2]);
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
				int vIndex = aiBones[b]->mWeights[w].mVertexId;

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
	delete[] vertices;
	delete[] indices;

	vertices = nullptr;
	indices = nullptr;
}

#include "main/Graphicsplosion.h"
#include "glm/gtx/transform.hpp"
#include "glm/gtx/euler_angles.hpp"

#include "main/Time.h"

void Model::PoseBones(float time) {
	for (Anim& anim : animations) {
		for (AnimNode& node : anim.nodes) {
			if (node.target) {
				// Find keyframes that are closest to the current time
				for (int i = 0; i < node.rotation.size() - 1; i++) {
					if (node.rotation[i].time < time && node.rotation[i + 1].time >= time) {
						node.rotationKeyframeIndex = i + (time - node.rotation[i].time) / (node.rotation[i + 1].time - node.rotation[i].time);
					}
				}

				for (int i = 0; i < node.translation.size() - 1; i++) {
					if (node.translation[i].time < time && node.translation[i + 1].time >= time) {
						node.translationKeyframeIndex = i + (time - node.translation[i].time) / (node.translation[i + 1].time - node.translation[i].time);
					}
				}
			}
		}
	}
}

void Model::Render(Renderer renderer) {
	// Create the buffers if they don't already exist
	if (!areBuffersCreated) {
		vertexBuffer.Create(renderer, vertices, numVertices * sizeof (Vertex));
		indexBuffer.Create(renderer, indices, numIndices * sizeof (unsigned int));

		areBuffersCreated = true;
	}

	// Bind the buffers
	renderer.UseVertexBuffer(&vertexBuffer);
	renderer.UseIndexBuffer(&indexBuffer);

	// Pose the bones (temporary)
	PoseBones((sin(Time::GetTime()) + 1.0f) / 2.0f);

	// Calculate local bone matrices
	glm::mat4 rootBoneMatrices[32];
	const Bone* bonePointers[32] = {0};

	for (Anim& anim : animations) {
		for (AnimNode& node : anim.nodes) {
			if (node.target) {
				int matrixIndex = node.target->index;
				
				// Linearly interpolate the animated values
				glm::quat rotation = glm::lerp(node.rotation[(int)node.rotationKeyframeIndex].quat, node.rotation[(int)node.rotationKeyframeIndex + 1].quat, node.rotationKeyframeIndex - (int)node.rotationKeyframeIndex);
				glm::vec3 translation = glm::lerp(node.translation[(int)node.translationKeyframeIndex].vec, node.translation[(int)node.translationKeyframeIndex + 1].vec, node.translationKeyframeIndex - (int)node.translationKeyframeIndex);

				// Generate the local matrix
				rootBoneMatrices[matrixIndex] = glm::mat4(rotation);
				rootBoneMatrices[matrixIndex] = glm::translate(translation) * rootBoneMatrices[matrixIndex];
				bonePointers[matrixIndex] = node.target;
			}
		}
	}

	// Calculate global bone matrices
	glm::mat4 finalBoneMatrices[32] = {};

	for (int i = 0; i < 32; i++) {
		if (bonePointers[i]) {
			const Bone* currentBone = bonePointers[i]->parent;

			finalBoneMatrices[i] = rootBoneMatrices[i];

			while (currentBone) {
				finalBoneMatrices[i] = rootBoneMatrices[currentBone->index] * finalBoneMatrices[i];
				currentBone = currentBone->parent;
			}

			finalBoneMatrices[i] = finalBoneMatrices[i] * bonePointers[i]->bindPose;
		}
	}

	// Send it to the shader
	int uniBoneTransforms = glGetUniformLocation(game.GetDefaultShaderProgram().GetGlProgram(), "boneTransforms");
	glUniformMatrix4fv(uniBoneTransforms, 32, GL_FALSE, (GLfloat*)finalBoneMatrices);

	// Render the triangles
	renderer.DrawTrianglesIndexed(0, numIndices);
}
