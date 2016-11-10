#pragma once
class ParticleEffectManager
{
public:
	//! Constructor
	ParticleEffectManager();
	//! Destructor
	~ParticleEffectManager();

	//! Creates particle effects buffers
	void createBuffers();

	void updateBuffers();

	void draw();

	int FindUnusedParticle();


	struct Particle {
		glm::vec3 pos, speed;
		unsigned char r, g, b, a; // Color
		float size, angle, weight;
		float life; // Remaining life of the particle. if < 0 : dead and unused.

	};
	
	static const int MaxParticles = 10000; // if too big causes error????
	Particle ParticlesContainer[MaxParticles];

	GLfloat* g_particule_position_size_data;
	GLubyte* g_particule_color_data;

private: 
	GLuint billboard_vertex_buffer, particles_color_buffer, particles_position_buffer;

	int ParticlesCount = 0;
	int LastUsedParticle = 0;
};

