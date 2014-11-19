#include "CustomWindow.h"
#include <stdio.h>

#if defined _MSC_VER
#include <GL/glu.h>
#else
#include <OpenGL/glu.h>
#endif

#include <GL/glext.h>

BEGIN_MESSAGE_MAP(CustomWindow, Anima::AnimaEngineWindow_Base)
	ANIMA_WINDOW_MOUSE_CLICK_EVENT(MouseClickCallback)
	ANIMA_WINDOW_PAINT_EVENT(PaintCallback)
	ANIMA_WINDOW_FRAMEBUFFER_SIZE_CHANGED_EVENT(FrameBufferResizeCallback)
	ANIMA_WINDOW_CURSOR_MOVE_EVENT(MouseMoveCallback)
	ANIMA_WINDOW_KEY_EVENT(KeyCallback)
END_MESSAGE_MAP()

#define aisgl_min(x,y) (x<y?x:y)
#define aisgl_max(x,y) (y>x?y:x)

CustomWindow::CustomWindow()
{
	_lastTime = 0.0;
	_prevTime = 0.0;
	_prevFPSTime = 0.0;
	_frames = 0;
	_scene = NULL;
	_sceneList = 0;
	_angle = 0.0;
	_rotationSpeed = 10.0;
	_scaleFactor = 1.0;
	_openGLLoaded = false;
}

CustomWindow::~CustomWindow()
{
}

bool CustomWindow::Load()
{
	GetEngine()->SetTime(0.0);
	
	if(!LoadAsset())
		return false;
	return true;
}

void CustomWindow::MouseClickCallback(Anima::AnimaEngineWindow_Base* window, int button, int action, int mods)
{
}
void CustomWindow::PaintCallback(Anima::AnimaWindow* window)
{
	((CustomWindow*)window)->DrawScene();
}

void CustomWindow::DrawScene()
{
	MakeCurrentContext();
	
	if(!_openGLLoaded)
	{
		glClearColor(0.1f,0.1f,0.1f,1.f);
		
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);    /* Uses default lighting parameters */
		
		glEnable(GL_DEPTH_TEST);
		
		glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
		glEnable(GL_NORMALIZE);
		
		/* XXX docs say all polygons are emitted CCW, but tests show that some aren't. */
		if(getenv("MODEL_IS_BROKEN"))
			glFrontFace(GL_CW);
		
		glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
		
		_openGLLoaded = true;
	}
	
	float tmp;
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.f,0.f,3.f,0.f,0.f,-5.f,0.f,1.f,0.f);
	
	/* rotate it around the y axis */
	glRotatef(_angle,0.f,1.f,0.f);
	
	/* scale the whole asset to fit into our view frustum */
	tmp = _sceneMax.x-_sceneMin.x;
	tmp = aisgl_max(_sceneMax.y - _sceneMin.y,tmp);
	tmp = aisgl_max(_sceneMax.z - _sceneMin.z,tmp);
	tmp = 1.f / tmp * _scaleFactor;
	glScalef(tmp, tmp, tmp);
	
	/* center the model */
	glTranslatef( -_sceneCenter.x, -_sceneCenter.y, -_sceneCenter.z );
	
	/* if the display list has not been made yet, create a new one and
	 fill it with scene contents */
	if(_sceneList == 0) {
		_sceneList = glGenLists(1);
		glNewList(_sceneList, GL_COMPILE);
		/* now begin at the root node of the imported data and traverse
		 the scenegraph by multiplying subsequent local transforms
		 together on GL's matrix stack. */
		RecursiveRender(_scene, _scene->mRootNode);
		glEndList();
	}
	
	glCallList(_sceneList);
	
	SwapBuffers();
	
	DoMotion();
}

void CustomWindow::FrameBufferResizeCallback(Anima::AnimaWindow* window, int w, int h)
{
	void* ctx = window->GetOpenGLContext();
	
	if(ctx)
	{
		const double aspectRatio = (float) w / h, fieldOfView = 45.0;
		
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(fieldOfView, aspectRatio, 1.0, 1000.0);  /* Znear and Zfar */
		glViewport(0, 0, w, h);
	}
}

void CustomWindow::GetBBoxForNode(const struct aiNode* nd, aiVector3D* min, aiVector3D* max, aiMatrix4x4* trafo)
{
	aiMatrix4x4 prev;
	unsigned int n = 0, t;
	
	prev = *trafo;
	aiMultiplyMatrix4(trafo,&nd->mTransformation);
	
	for (; n < nd->mNumMeshes; ++n)
	{
		const struct aiMesh* mesh = _scene->mMeshes[nd->mMeshes[n]];
		for (t = 0; t < mesh->mNumVertices; ++t)
		{
			aiVector3D tmp = mesh->mVertices[t];
			aiTransformVecByMatrix4(&tmp,trafo);
			
			min->x = aisgl_min(min->x,tmp.x);
			min->y = aisgl_min(min->y,tmp.y);
			min->z = aisgl_min(min->z,tmp.z);
			
			max->x = aisgl_max(max->x,tmp.x);
			max->y = aisgl_max(max->y,tmp.y);
			max->z = aisgl_max(max->z,tmp.z);
		}
	}
	
	for (n = 0; n < nd->mNumChildren; ++n)
	{
		GetBBoxForNode(nd->mChildren[n], min, max, trafo);
	}
	*trafo = prev;
}

void CustomWindow::GetBBox(aiVector3D* min, aiVector3D* max)
{
	aiMatrix4x4 trafo;
	aiIdentityMatrix4(&trafo);
	
	min->x = min->y = min->z =  1e10f;
	max->x = max->y = max->z = -1e10f;
	GetBBoxForNode(_scene->mRootNode,min,max,&trafo);
}

void CustomWindow::Color4ToFloat4(const aiColor4D *c, float f[4])
{
	f[0] = c->r;
	f[1] = c->g;
	f[2] = c->b;
	f[3] = c->a;
}

void CustomWindow::SetFloat4(float f[4], float a, float b, float c, float d)
{
	f[0] = a;
	f[1] = b;
	f[2] = c;
	f[3] = d;
}

void CustomWindow::ApplyMaterial(const struct aiMaterial *mtl)
{
	float c[4];
	
	GLenum fill_mode;
	int ret1, ret2;
	aiColor4D diffuse;
	aiColor4D specular;
	aiColor4D ambient;
	aiColor4D emission;
	float shininess, strength;
	int two_sided;
	int wireframe;
	unsigned int max;
	
	SetFloat4(c, 0.8f, 0.8f, 0.8f, 1.0f);
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
		Color4ToFloat4(&diffuse, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, c);
	
	SetFloat4(c, 0.0f, 0.0f, 0.0f, 1.0f);
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &specular))
		Color4ToFloat4(&specular, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);
	
	SetFloat4(c, 0.2f, 0.2f, 0.2f, 1.0f);
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &ambient))
		Color4ToFloat4(&ambient, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, c);
	
	SetFloat4(c, 0.0f, 0.0f, 0.0f, 1.0f);
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &emission))
		Color4ToFloat4(&emission, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, c);
	
	max = 1;
	ret1 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess, &max);
	if(ret1 == AI_SUCCESS)
	{
		max = 1;
		ret2 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS_STRENGTH, &strength, &max);
		if(ret2 == AI_SUCCESS)
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess * strength);
		else
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
	}
	else
	{
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
		SetFloat4(c, 0.0f, 0.0f, 0.0f, 0.0f);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);
	}
	
	max = 1;
	if(AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_ENABLE_WIREFRAME, &wireframe, &max))
		fill_mode = wireframe ? GL_LINE : GL_FILL;
	else
		fill_mode = GL_FILL;
	glPolygonMode(GL_FRONT_AND_BACK, fill_mode);
	
	max = 1;
	if((AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_TWOSIDED, &two_sided, &max)) && two_sided)
		glDisable(GL_CULL_FACE);
	else
		glEnable(GL_CULL_FACE);
}
void CustomWindow::RecursiveRender(const struct aiScene *sc, const struct aiNode* nd)
{
	unsigned int i;
	unsigned int n = 0, t;
	aiMatrix4x4 m = nd->mTransformation;
	
	/* update transform */
	aiTransposeMatrix4(&m);
	glPushMatrix();
	glMultMatrixf((float*)&m);
	
	/* draw all meshes assigned to this node */
	for (; n < nd->mNumMeshes; ++n)
	{
		const struct aiMesh* mesh = _scene->mMeshes[nd->mMeshes[n]];
		
		ApplyMaterial(sc->mMaterials[mesh->mMaterialIndex]);
		
		if(mesh->mNormals == NULL)
			glDisable(GL_LIGHTING);
		else
			glEnable(GL_LIGHTING);
		
		for (t = 0; t < mesh->mNumFaces; ++t)
		{
			const struct aiFace* face = &mesh->mFaces[t];
			GLenum face_mode;
			
			switch(face->mNumIndices) {
				case 1: face_mode = GL_POINTS; break;
				case 2: face_mode = GL_LINES; break;
				case 3: face_mode = GL_TRIANGLES; break;
				default: face_mode = GL_POLYGON; break;
			}
			
			glBegin(face_mode);
			
			for(i = 0; i < face->mNumIndices; i++)
			{
				int index = face->mIndices[i];
				if(mesh->mColors[0] != NULL)
					glColor4fv((GLfloat*)&mesh->mColors[0][index]);
				if(mesh->mNormals != NULL)
					glNormal3fv(&mesh->mNormals[index].x);
				glVertex3fv(&mesh->mVertices[index].x);
			}
			
			glEnd();
		}
	}
	
	/* draw all children */
	for (n = 0; n < nd->mNumChildren; ++n)
		RecursiveRender(sc, nd->mChildren[n]);
	
	glPopMatrix();
}

void CustomWindow::DoMotion()
{
	double time = GetEngine()->GetTime();
	
	_angle += (time - _prevTime) * _rotationSpeed;
	_prevTime = time;
	_frames += 1;
	
	if ((time - _prevFPSTime) > 1) /* update every seconds */
	{
		int current_fps = _frames / (time - _prevFPSTime);
		char titolo[200];
		sprintf(titolo, "AnimaEngine CustomWindow - %d FPS", current_fps);
		SetWindowTitle(titolo);
		_frames = 0;
		_prevFPSTime = time;
	}
}

bool CustomWindow::LoadAsset()
{
	const char* path = "D:/Modelli/Big_Dragon/Big_Dragon.fbx";
	
	/* we are taking one of the postprocessing presets to avoid
	 spelling out 20+ single postprocessing flags here. */
	_scene = _importer.ReadFile(path, aiProcessPreset_TargetRealtime_Quality);
	
	if (_scene) {
		GetBBox(&_sceneMin, &_sceneMax);
		_sceneCenter.x = (_sceneMin.x + _sceneMax.x) / 2.0f;
		_sceneCenter.y = (_sceneMin.y + _sceneMax.y) / 2.0f;
		_sceneCenter.z = (_sceneMin.z + _sceneMax.z) / 2.0f;
		return true;
	}
	return false;
}

void CustomWindow::MouseMoveCallback(Anima::AnimaWindow* window, double x, double y)
{
}

void CustomWindow::KeyCallback(Anima::AnimaWindow* window, int key, int scancode, int action, int mods)
{
	if(key == ANIMA_ENGINE_CORE_KEY_Q && action == ANIMA_ENGINE_CORE_RELEASE)
		window->SetShouldClose(true);
	else if(key == ANIMA_ENGINE_CORE_KEY_UP && action == ANIMA_ENGINE_CORE_PRESS)
		((CustomWindow*)window)->_rotationSpeed += 5.0;
	else if(key == ANIMA_ENGINE_CORE_KEY_DOWN && action == ANIMA_ENGINE_CORE_PRESS)
		((CustomWindow*)window)->_rotationSpeed -= 5.0;
	else if(key == ANIMA_ENGINE_CORE_KEY_RIGHT && action == ANIMA_ENGINE_CORE_PRESS)
		((CustomWindow*)window)->_scaleFactor += 1.0;
	else if(key == ANIMA_ENGINE_CORE_KEY_LEFT && action == ANIMA_ENGINE_CORE_PRESS)
		((CustomWindow*)window)->_scaleFactor -= 1.0;
}