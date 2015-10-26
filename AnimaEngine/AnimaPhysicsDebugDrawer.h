//
//  AnimaPhysicsDebugDrawer.h
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#ifndef __Anima__AnimaPhysicsDebugDrawer__
#define __Anima__AnimaPhysicsDebugDrawer__

#include "AnimaEngine.h"
#include "AnimaVertex.h"
#include "AnimaArray.h"
#include <LinearMath/btIDebugDraw.h>
#include "AnimaShaderProgram.h"
#include "AnimaEngine.h"
#include "AnimaScene.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaPhysicsDebugDrawer : public btIDebugDraw
{
public:
	AnimaPhysicsDebugDrawer();
	virtual ~AnimaPhysicsDebugDrawer();
	
public:
	void drawLine(const btVector3& from, const btVector3& to,const btVector3& color) override;
	void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) override;
	void reportErrorWarning(const char* warningString) override;
	void draw3dText(const btVector3& location, const char* textString) override;
	void setDebugMode(int debugMode) override;
	int	getDebugMode() const override;
	
public:
	void SetupPrograms(AnimaEngine* engine);
	void DrawDebugScene(AnimaScene* scene);
	
	AUint GetVertexArrayObject() const { return _vertexArrayObject; }
	AUint GetVerticesBufferObject() const { return _verticesBufferObject; }
	AUint GetColorsBufferObject() const { return _colorsBufferObject; }
	
protected:
	AFloat* GetFloatVertices() const;
	AUint GetFloatVerticesCount() const;
	
	AFloat* GetFloatColors() const;
	AUint GetFloatColorsCount() const;
	
protected:
	int _debugMode;
	
	AnimaShaderProgram* _shaderProgram;
	
	AnimaArray<AnimaVertex3f> _vertices;
	AnimaArray<AnimaVertex3f> _colors;
	
	AUint _vertexArrayObject;
	AUint _verticesBufferObject;
	AUint _colorsBufferObject;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaPhysicsDebugDrawer__) */
