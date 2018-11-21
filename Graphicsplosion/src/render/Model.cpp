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
		// Import animations (TEST  --- REFACTOR LATER)
		aiBone* const* aiBones = currentMesh->mBones;
		const int maxBonesPerVertex = 4;
		
		bones.resize(bones.size() + currentMesh->mNumBones);

		for (int b = 0; b < currentMesh->mNumBones; b++) {
			int bIndex = b + (bones.size() - currentMesh->mNumBones);
			Bone& newBone = bones[bIndex];
			
			// Copy name and bind post
			newBone.index = bIndex;
			newBone.name = aiBones[b]->mName.C_Str();

			for (int j = 0; j < 16; j++) {
				bones[b].bindPose[j/4][j%4] = aiBones[b]->mOffsetMatrix[j/4][j%4];
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
				newNode.translation[k].time = channel->mPositionKeys[k].mTime;
				newNode.translation[k].vec = glm::vec3(channel->mPositionKeys[k].mValue.x, channel->mPositionKeys[k].mValue.y, channel->mPositionKeys[k].mValue.z);
			}
			
			for (int k = 0; k < channel->mNumRotationKeys; k++) {
				newNode.rotation[k].time = channel->mRotationKeys[k].mTime;
				newNode.rotation[k].vec = glm::vec3(channel->mRotationKeys[k].mValue.x, channel->mRotationKeys[k].mValue.y, channel->mRotationKeys[k].mValue.z);
			}
		}

		animations.push_back(newAnimation);
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

	// Send the gargblarghs to the vertex shader
	for (Anim& anim : animations) {
		for (AnimNode& node : anim.nodes) {
			if (node.target) {
				node.target->index;

				node.translationKeyframeIndex = 0.0f;
				node.rotationKeyframeIndex = 0.0f;
				node.scaleKeyframeIndex = 0.0f;
			}
		}
	}

	// Animate bones and shfishfizzle
	glm::mat4 boneMatrices[32];

	for (Anim& anim : animations) {
		for (AnimNode& node : anim.nodes) {
			if (node.target) {
				int matrixIndex = node.target->index;
				glm::mat4 currentState;

				currentState *= glm::translate(node.translation[0].vec);
				currentState *= glm::eulerAngleYXZ(node.rotation[0].vec.z, node.rotation[0].vec.y, node.rotation[0].vec.x);

				boneMatrices[matrixIndex] = node.target->bindPose;
				//boneMatrices[matrixIndex][0][0] = 3.0f;
				boneMatrices[matrixIndex] = glm::identity<glm::mat4>();
			}
		}
	}

	// Send it to the shader
	int uniBoneTransforms = glGetUniformLocation(game.GetDefaultShaderProgram().GetGlProgram(), "boneTransforms");
	glUniformMatrix4fv(uniBoneTransforms, 32, GL_FALSE, (GLfloat*)boneMatrices);

	// Render the triangles
	renderer.DrawTrianglesIndexed(0, numIndices);
}
