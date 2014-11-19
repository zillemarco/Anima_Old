#ifndef _CustomWindow_h
#define _CustomWindow_h

#include <AnimaEngine.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/vector3.h>

class CustomWindow : public Anima::AnimaWindow
{
public:
	CustomWindow();
	~CustomWindow();

	DECLARE_MESSAGE_MAP()

public:
	void DrawScene();
	bool Load();
	
private:
	bool LoadAsset();
	void DoMotion();
	void RecursiveRender(const struct aiScene *sc, const struct aiNode* nd);
	void ApplyMaterial(const struct aiMaterial *mtl);
	void SetFloat4(float f[4], float a, float b, float c, float d);
	void Color4ToFloat4(const aiColor4D *c, float f[4]);
	void GetBBoxForNode(const struct aiNode* nd, aiVector3D* min, aiVector3D* max, aiMatrix4x4* trafo);
	void GetBBox(aiVector3D* min, aiVector3D* max);
	
private:
	double _lastTime;
	double _prevTime;
	double _prevFPSTime;
	int _frames;
	const struct aiScene* _scene;
	GLint _sceneList;
	aiVector3D _sceneMin;
	aiVector3D _sceneMax;
	aiVector3D _sceneCenter;
	float _angle;
	aiLogStream _stream;
	double _rotationSpeed;
	
	bool _openGLLoaded;
	
private:
	ANIMA_WINDOW_EVENT void MouseClickCallback(Anima::AnimaWindow* window, int button, int action, int mods);
	ANIMA_WINDOW_EVENT void PaintCallback(Anima::AnimaWindow* window);
	ANIMA_WINDOW_EVENT void MouseMoveCallback(Anima::AnimaWindow* window, double x, double y);
	ANIMA_WINDOW_EVENT void KeyCallback(Anima::AnimaWindow* window, int key, int scancode, int action, int mods);
public:
	ANIMA_WINDOW_EVENT void FrameBufferResizeCallback(Anima::AnimaWindow* window, int w, int h);
};

#endif