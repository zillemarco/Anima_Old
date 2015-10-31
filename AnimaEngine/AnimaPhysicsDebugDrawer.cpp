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

void AnimaPhysicsDebugDrawer::AddConstantPoint(const AnimaVertex3f& point, const AnimaColor3f& color)
{
	_constantPoints.push_back(point);
	_constantPointsColors.push_back(color);
}

void AnimaPhysicsDebugDrawer::AddConstantLine(const AnimaVertex3f& from, const AnimaVertex3f& to, const AnimaColor3f& color)
{
	_constantLines.push_back(from);
	_constantLines.push_back(to);
	_constantLinesColors.push_back(color);
	_constantLinesColors.push_back(color);
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
	
	DrawPhysics(scene);
//	DrawConstantPoints(scene);
//	DrawConstantLines(scene);
}

void AnimaPhysicsDebugDrawer::DrawPhysics(AnimaScene* scene)
{
	AInt colorsCount = _colors.size();
	AInt verticesCount = _vertices.size();
	
	if(verticesCount != colorsCount || verticesCount == 0 || colorsCount == 0)
	{
		_colors.clear();
		_vertices.clear();
		
		return;
	}
	else
	{
		if(_vertexArrayObject <= 0)
		{
			glGenVertexArrays(1, &_vertexArrayObject);
			if (_vertexArrayObject <= 0)
			{
				AnimaLogger::LogMessage("PHYSICS DEBUG ERROR - Unable to create the VAO");
				
				_colors.clear();
				_vertices.clear();
				
				return;
			}
		}
		
		if(_verticesBufferObject <= 0)
		{
			glGenBuffers(1, &_verticesBufferObject);
			if (_verticesBufferObject <= 0)
			{
				AnimaLogger::LogMessage("PHYSICS DEBUG ERROR - Unable to create vertices buffer");
				
				_colors.clear();
				_vertices.clear();
				
				return;
			}
		}
		
		if(_colorsBufferObject <= 0)
		{
			glGenBuffers(1, &_colorsBufferObject);
			if (_colorsBufferObject <= 0)
			{
				AnimaLogger::LogMessage("PHYSICS DEBUG ERROR - Unable to create colors buffer");
				
				_colors.clear();
				_vertices.clear();
				
				return;
			}
		}
		
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
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	
	// Pulisco i dati di disegno per non ridisegnare cose vecchie alla prossima chiamata
	_colors.clear();
	_vertices.clear();
}

void AnimaPhysicsDebugDrawer::DrawConstantPoints(AnimaScene* scene)
{
	AFloat pointSize = 1.0;
	glGetFloatv(GL_POINT_SIZE, &pointSize);
	glPointSize(10.0);
	
	AInt colorsCount = _constantPointsColors.size();
	AInt verticesCount = _constantPoints.size();
	
	if(verticesCount != colorsCount || verticesCount == 0 || colorsCount == 0)
	{
		glPointSize(pointSize);
		return;
	}
	else
	{
		if(_vertexArrayObject <= 0)
		{
			glGenVertexArrays(1, &_vertexArrayObject);
			if (_vertexArrayObject <= 0)
			{
				glPointSize(pointSize);
				AnimaLogger::LogMessage("PHYSICS DEBUG ERROR - Unable to create the points VAO");
				return;
			}
		}
		
		if(_verticesBufferObject <= 0)
		{
			glGenBuffers(1, &_verticesBufferObject);
			if (_verticesBufferObject <= 0)
			{
				glPointSize(pointSize);
				AnimaLogger::LogMessage("PHYSICS DEBUG ERROR - Unable to create points vertices buffer");
				return;
			}
		}
		
		if(_colorsBufferObject <= 0)
		{
			glGenBuffers(1, &_colorsBufferObject);
			if (_colorsBufferObject <= 0)
			{
				glPointSize(pointSize);
				AnimaLogger::LogMessage("PHYSICS DEBUG ERROR - Unable to create points colors buffer");
				return;
			}
		}
		
		glBindVertexArray(_vertexArrayObject);
		
		AFloat* vertices = GetPointsFloatVertices();
		glBindBuffer(GL_ARRAY_BUFFER, _verticesBufferObject);
		glBufferData(GL_ARRAY_BUFFER, sizeof(AFloat) * GetPointsFloatVerticesCount(), vertices, GL_STATIC_DRAW);
		
		delete [] vertices;
		vertices = nullptr;
		
		AFloat* colors = GetPointsFloatColors();
		glBindBuffer(GL_ARRAY_BUFFER, _colorsBufferObject);
		glBufferData(GL_ARRAY_BUFFER, sizeof(AFloat) * GetPointsFloatColorsCount(), colors, GL_STATIC_DRAW);
		
		delete [] colors;
		colors = nullptr;
		
		_shaderProgram->Use();
		_shaderProgram->UpdateSceneObjectProperties(scene->GetCamerasManager()->GetActiveCamera(), nullptr);
		_shaderProgram->EnableInputs(this);
		
		glDrawArrays(GL_POINTS, 0, _constantPoints.size());
		
		_shaderProgram->DisableInputs();
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	
	glPointSize(pointSize);
}

void AnimaPhysicsDebugDrawer::DrawConstantLines(AnimaScene* scene)
{
	AInt colorsCount = _constantLinesColors.size();
	AInt verticesCount = _constantLines.size();
	
	if(verticesCount != colorsCount || verticesCount == 0 || colorsCount == 0)
		return;
	else
	{
		if(_vertexArrayObject <= 0)
		{
			glGenVertexArrays(1, &_vertexArrayObject);
			if (_vertexArrayObject <= 0)
			{
				AnimaLogger::LogMessage("PHYSICS DEBUG ERROR - Unable to create the lines VAO");
				return;
			}
		}
		
		if(_verticesBufferObject <= 0)
		{
			glGenBuffers(1, &_verticesBufferObject);
			if (_verticesBufferObject <= 0)
			{
				AnimaLogger::LogMessage("PHYSICS DEBUG ERROR - Unable to create lines vertices buffer");
				return;
			}
		}
		
		if(_colorsBufferObject <= 0)
		{
			glGenBuffers(1, &_colorsBufferObject);
			if (_colorsBufferObject <= 0)
			{
				AnimaLogger::LogMessage("PHYSICS DEBUG ERROR - Unable to create lines colors buffer");
				return;
			}
		}
		
		glBindVertexArray(_vertexArrayObject);
		
		AFloat* vertices = GetLinesFloatVertices();
		glBindBuffer(GL_ARRAY_BUFFER, _verticesBufferObject);
		glBufferData(GL_ARRAY_BUFFER, sizeof(AFloat) * GetLinesFloatVerticesCount(), vertices, GL_STATIC_DRAW);
		
		delete [] vertices;
		vertices = nullptr;
		
		AFloat* colors = GetLinesFloatColors();
		glBindBuffer(GL_ARRAY_BUFFER, _colorsBufferObject);
		glBufferData(GL_ARRAY_BUFFER, sizeof(AFloat) * GetLinesFloatColorsCount(), colors, GL_STATIC_DRAW);
		
		delete [] colors;
		colors = nullptr;
		
		_shaderProgram->Use();
		_shaderProgram->UpdateSceneObjectProperties(scene->GetCamerasManager()->GetActiveCamera(), nullptr);
		_shaderProgram->EnableInputs(this);
		
		glDrawArrays(GL_LINES, 0, _constantLines.size());
		
		_shaderProgram->DisableInputs();
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
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

AFloat* AnimaPhysicsDebugDrawer::GetPointsFloatVertices() const
{
	AFloat* vertices = nullptr;
	AInt count = GetPointsFloatVerticesCount();
	AInt offset = 0;
	
	if (count > 0)
	{
		vertices = new AFloat[count];
		
		AInt verticesCount = _constantPoints.size();
		for (int i = 0; i < verticesCount; i++)
		{
			memcpy(vertices + offset, _constantPoints[i].vec, sizeof(AFloat) * 3);
			offset += 3;
		}
	}
	
	return vertices;
}

AUint AnimaPhysicsDebugDrawer::GetPointsFloatVerticesCount() const
{
	return (AUint)_constantPoints.size() * 3;
}

AFloat* AnimaPhysicsDebugDrawer::GetPointsFloatColors() const
{
	AFloat* colors = nullptr;
	AInt count = GetPointsFloatColorsCount();
	AInt offset = 0;
	
	if (count > 0)
	{
		colors = new AFloat[count];
		
		AInt colorsCount = _constantPointsColors.size();
		for (int i = 0; i < colorsCount; i++)
		{
			memcpy(colors + offset, _constantPointsColors[i].vec, sizeof(AFloat) * 3);
			offset += 3;
		}
	}
	
	return colors;
}

AUint AnimaPhysicsDebugDrawer::GetPointsFloatColorsCount() const
{
	return (AUint)_constantPointsColors.size() * 3;
}

AFloat* AnimaPhysicsDebugDrawer::GetLinesFloatVertices() const
{
	AFloat* vertices = nullptr;
	AInt count = GetLinesFloatVerticesCount();
	AInt offset = 0;
	
	if (count > 0)
	{
		vertices = new AFloat[count];
		
		AInt verticesCount = _constantLines.size();
		for (int i = 0; i < verticesCount; i++)
		{
			memcpy(vertices + offset, _constantLines[i].vec, sizeof(AFloat) * 3);
			offset += 3;
		}
	}
	
	return vertices;
}

AUint AnimaPhysicsDebugDrawer::GetLinesFloatVerticesCount() const
{
	return (AUint)_constantLines.size() * 3;
}

AFloat* AnimaPhysicsDebugDrawer::GetLinesFloatColors() const
{
	AFloat* colors = nullptr;
	AInt count = GetLinesFloatColorsCount();
	AInt offset = 0;
	
	if (count > 0)
	{
		colors = new AFloat[count];
		
		AInt colorsCount = _constantLinesColors.size();
		for (int i = 0; i < colorsCount; i++)
		{
			memcpy(colors + offset, _constantLinesColors[i].vec, sizeof(AFloat) * 3);
			offset += 3;
		}
	}
	
	return colors;
}

AUint AnimaPhysicsDebugDrawer::GetLinesFloatColorsCount() const
{
	return (AUint)_constantLinesColors.size() * 3;
}

END_ANIMA_ENGINE_NAMESPACE
