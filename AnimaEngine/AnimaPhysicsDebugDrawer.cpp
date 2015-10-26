//
//  AnimaPhysicsDebugDrawer.cpp
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#include "AnimaPhysicsDebugDrawer.h"
#include "AnimaLogger.h"
#include "AnimaShadersManager.h"
#include "AnimaCamerasManager.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaPhysicsDebugDrawer::AnimaPhysicsDebugDrawer()
{
	_debugMode = 0;
	_shaderProgram = nullptr;
	
	_vertexArrayObject = 0;
	_verticesBufferObject = 0;
	_colorsBufferObject = 0;
}

AnimaPhysicsDebugDrawer::~AnimaPhysicsDebugDrawer()
{
	
}

void AnimaPhysicsDebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	AnimaVertex3f start(from.x(), from.y(), from.z());
	AnimaVertex3f end(to.x(), to.y(), to.z());
	AnimaColor3f col(color.x(), color.y(), color.z());
	
	// Aggiungo alla lista di vertici i due punti della linea
	_vertices.push_back(start);
	_vertices.push_back(end);
	
	// Aggiungo alla lista dei colori il colore due volte, una per ogni vertice
	_colors.push_back(col);
	_colors.push_back(col);
}

void AnimaPhysicsDebugDrawer::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
	AnimaLogger::LogMessage("PHYSICS DEBUG - Draw contact point");
}

void AnimaPhysicsDebugDrawer::reportErrorWarning(const char* warningString)
{
	AnimaLogger::LogMessageFormat("PHYSICS WARNING - %s", warningString);
}

void AnimaPhysicsDebugDrawer::draw3dText(const btVector3& location, const char* textString)
{
	AnimaLogger::LogMessageFormat("PHYSICS DEBUG MESSAGE - %s", textString);
}

void AnimaPhysicsDebugDrawer::setDebugMode(int debugMode)
{
	_debugMode = debugMode;
}

int	AnimaPhysicsDebugDrawer::getDebugMode() const
{
	return _debugMode;
}

void AnimaPhysicsDebugDrawer::SetupPrograms(AnimaEngine* engine)
{
	if(engine == nullptr)
		return;
	
	AnimaShadersManager* shadersManager = engine->GetShadersManager();
	
	_shaderProgram = shadersManager->GetProgramFromName("physicsDebug");
}

void AnimaPhysicsDebugDrawer::DrawDebugScene(AnimaScene* scene)
{
	if(scene == nullptr)
		return;
	
	if(_shaderProgram == nullptr)
	{
		AnimaLogger::LogMessage("PHYSICS DEBUG WARNING - Trying to do the physics debug drawing but the drawer hasn't got a valid shader program");
		
		// Pulisco i dati di disegno anche se non disegno nulla
		_colors.clear();
		_vertices.clear();
		
		return;
	}
	
	AInt colorsCount = _colors.size();
	AInt verticesCount = _vertices.size();
	
	if(verticesCount != colorsCount)
		AnimaLogger::LogMessage("PHYSICS DEBUG WARNING - Vertices count and colors count aren't the same");
	else if(verticesCount == 0)
		AnimaLogger::LogMessage("PHYSICS DEBUG WARNING - Vertices count is 0");
	else if(colorsCount == 0)
		AnimaLogger::LogMessage("PHYSICS DEBUG WARNING - Colors count is 0");
	else
	{
		if(_vertexArrayObject <= 0)
		{
			glGenVertexArrays(1, &_vertexArrayObject);
			if (_vertexArrayObject <= 0)
			{
				AnimaLogger::LogMessage("PHYSICS DEBUG ERROR - Unable to create the VAO");
				return;
			}
		}
		
		if(_verticesBufferObject <= 0)
		{
			glGenBuffers(1, &_verticesBufferObject);
			if (_verticesBufferObject <= 0)
			{
				AnimaLogger::LogMessage("PHYSICS DEBUG ERROR - Unable to create vertices buffer");
				return;
			}
		}
//		else
//			glInvalidateBufferData(_verticesBufferObject);
		
		if(_colorsBufferObject <= 0)
		{
			glGenBuffers(1, &_colorsBufferObject);
			if (_colorsBufferObject <= 0)
			{
				AnimaLogger::LogMessage("PHYSICS DEBUG ERROR - Unable to create colors buffer");
				return;
			}
		}
//		else
//			glInvalidateBufferData(_colorsBufferObject);

		glBindVertexArray(_vertexArrayObject);
		
		AFloat* vertices = GetFloatVertices();
		glBindBuffer(GL_ARRAY_BUFFER, _verticesBufferObject);
		glBufferData(GL_ARRAY_BUFFER, sizeof(AFloat) * GetFloatVerticesCount(), vertices, GL_STATIC_DRAW);
		
		delete [] vertices;
		vertices = nullptr;
		
		AFloat* colors = GetFloatColors();
		glBindBuffer(GL_ARRAY_BUFFER, _colorsBufferObject);
		glBufferData(GL_ARRAY_BUFFER, sizeof(AFloat) * GetFloatColorsCount(), colors, GL_STATIC_DRAW);
		
		delete [] colors;
		colors = nullptr;
		
		_shaderProgram->Use();
		_shaderProgram->UpdateSceneObjectProperties(scene->GetCamerasManager()->GetActiveCamera(), nullptr);
		_shaderProgram->EnableInputs(this);
		
		glDrawArrays(GL_LINES, 0, _vertices.size());
		
		_shaderProgram->DisableInputs();
	}
	
	// Pulisco i dati di disegno per non ridisegnare cose vecchie alla prossima chiamata
	_colors.clear();
	_vertices.clear();
}

AFloat* AnimaPhysicsDebugDrawer::GetFloatVertices() const
{
	AFloat* vertices = nullptr;
	AInt count = GetFloatVerticesCount();
	AInt offset = 0;
	
	if (count > 0)
	{
		vertices = new AFloat[count];
		
		AInt verticesCount = _vertices.size();
		for (int i = 0; i < verticesCount; i++)
		{
			memcpy(vertices + offset, _vertices[i].vec, sizeof(AFloat) * 3);
			offset += 3;
		}
	}
	
	return vertices;
}

AUint AnimaPhysicsDebugDrawer::GetFloatVerticesCount() const
{
	return (AUint)_vertices.size() * 3;
}

AFloat* AnimaPhysicsDebugDrawer::GetFloatColors() const
{
	AFloat* colors = nullptr;
	AInt count = GetFloatColorsCount();
	AInt offset = 0;
	
	if (count > 0)
	{
		colors = new AFloat[count];
		
		AInt colorsCount = _colors.size();
		for (int i = 0; i < colorsCount; i++)
		{
			memcpy(colors + offset, _colors[i].vec, sizeof(AFloat) * 3);
			offset += 3;
		}
	}
	
	return colors;
}

AUint AnimaPhysicsDebugDrawer::GetFloatColorsCount() const
{
	return (AUint)_colors.size() * 3;
}

END_ANIMA_ENGINE_NAMESPACE
