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

private: 
	GLuint billboard_vertex_buffer, particles_color_buffer, particles_position_buffer;
	int MaxParticles = 10;
};

