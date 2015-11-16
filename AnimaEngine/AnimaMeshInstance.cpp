//
//  AnimaMeshInstance.cpp
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#include "AnimaMeshInstance.h"
#include "AnimaShaderProgram.h"
#include "AnimaRenderer.h"
#include "AnimaScene.h"
#include "AnimaMaterialsManager.h"
#include "AnimaMeshesManager.h"
#include <BulletCollision/CollisionShapes/btShapeHull.h>

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaMeshInstance::AnimaMeshInstance(const AnimaString& name, AnimaDataGeneratorsManager* dataGeneratorsManager, AnimaAllocator* allocator)
	: AnimaSceneObject(name, dataGeneratorsManager, allocator)
{
	IMPLEMENT_ANIMA_CLASS(AnimaMeshInstance);
	_material = nullptr;
	_mesh = nullptr;
	_shaderProgramName = "";
	_shaderProgram = nullptr;
	
	_physCollisionShape = nullptr;
	_physMotionState = nullptr;
	_physRigidBody = nullptr;
	_meshInterface = nullptr;
	
	_shaderSource = ASDSO_GEOMETRY;
}

AnimaMeshInstance::AnimaMeshInstance(const AnimaMeshInstance& src)
	: AnimaSceneObject(src)
	, _materialName(src._materialName)
	, _meshName(src._meshName)
	, _shadersNames(src._shadersNames)
	, _shaderProgram(src._shaderProgram)
	, _shaderProgramName(src._shaderProgramName)
{
	_material = src._material;
	_mesh = src._mesh;
	_physCollisionShape = src._physCollisionShape;
	_physMotionState = src._physMotionState;
	_physRigidBody = src._physRigidBody;
}

AnimaMeshInstance::AnimaMeshInstance(AnimaMeshInstance&& src)
	: AnimaSceneObject(src)
	, _materialName(src._materialName)
	, _meshName(src._meshName)
	, _shadersNames(src._shadersNames)
	, _shaderProgram(src._shaderProgram)
	, _shaderProgramName(src._shaderProgramName)
{
	_material = src._material;
	_mesh = src._mesh;
	_physCollisionShape = src._physCollisionShape;
	_physMotionState = src._physMotionState;
	_physRigidBody = src._physRigidBody;
}

AnimaMeshInstance::~AnimaMeshInstance()
{
	if(_physRigidBody != nullptr)
	{
		delete _physRigidBody;
		_physRigidBody = nullptr;
	}
	
	if(_physMotionState != nullptr)
	{
		delete _physMotionState;
		_physMotionState = nullptr;
	}
	
	if(_physCollisionShape != nullptr)
	{
		delete _physCollisionShape;
		_physCollisionShape = nullptr;
	}

	if (_meshInterface != nullptr)
	{
		delete _meshInterface;
		_meshInterface = nullptr;
	}
}

AnimaMeshInstance& AnimaMeshInstance::operator=(const AnimaMeshInstance& src)
{
	if (this != &src)
	{
		AnimaSceneObject::operator=(src);

		_material = src._material;
		_materialName = src._materialName;
		
		_mesh = src._mesh;
		_meshName = src._meshName;

		_shadersNames = src._shadersNames;
		_shaderProgramName = src._shaderProgramName;
		_shaderProgram = src._shaderProgram;
		_physCollisionShape = src._physCollisionShape;
		_physMotionState = src._physMotionState;
		_physRigidBody = src._physRigidBody;
	}
	
	return *this;
}

AnimaMeshInstance& AnimaMeshInstance::operator=(AnimaMeshInstance&& src)
{
	if (this != &src)
	{
		AnimaSceneObject::operator=(src);

		_material = src._material;
		_materialName = src._materialName;

		_mesh = src._mesh;
		_meshName = src._meshName;

		_shadersNames = src._shadersNames;
		_shaderProgramName = src._shaderProgramName;
		_shaderProgram = src._shaderProgram;
		_physCollisionShape = src._physCollisionShape;
		_physMotionState = src._physMotionState;
		_physRigidBody = src._physRigidBody;
	}
	
	return *this;
}

ptree AnimaMeshInstance::GetObjectTree(bool saveName) const
{
	ptree tree;
	
	if (saveName)
		tree.add("AnimaMeshInstance.Name", GetName());
	
	AnimaString materialName = _materialName;
	if (materialName.empty() && _material != nullptr)
		materialName = _material->GetName();
	tree.put("AnimaMeshInstance.Material", materialName);
	
	AnimaString meshName = _meshName;
	if(meshName.empty() && _mesh != nullptr)
		meshName = _mesh->GetName();
	tree.put("AnimaMeshInstance.MeshName", meshName);
	
	ptree shadersNamesTree;
	for(auto& shaderName : _shadersNames)
		shadersNamesTree.add("ShaderName", shaderName);
	tree.add_child("AnimaMeshInstance.ShaderNames", shadersNamesTree);
	
	tree.add_child("AnimaMeshInstance.SceneObject", AnimaSceneObject::GetObjectTree(false));
	
	return tree;
}

bool AnimaMeshInstance::ReadObject(const ptree& objectTree, AnimaScene* scene, bool readName)
{
	try
	{
		if (readName)
			SetName(objectTree.get<AnimaString>("AnimaMeshInstance.Name"));
		
		_materialName = objectTree.get<AnimaString>("AnimaMeshInstance.Material", "");
		_meshName = objectTree.get<AnimaString>("AnimaMeshInstance.MeshName", "");
		
		if (!_materialName.empty())
		{
			_material = scene->GetMaterialsManager()->GetMaterialFromName(_materialName);
			if(_material == nullptr)
			{
				AnimaLogger::LogMessageFormat("WARNING - Error reading mesh instance named '%s': this mesh instance has material named '%s' but this material can't be found", GetName().c_str(), _materialName.c_str());
			}
		}
		else
		{
			AnimaLogger::LogMessageFormat("WARNING - Error reading mesh instance named '%s': this mesh instance has a blank material name", GetName().c_str());
		}
		
		if (!_meshName.empty())
		{
			_mesh = scene->GetMeshesManager()->GetMeshFromName(_meshName);
			if(_mesh != nullptr)
				_mesh->AddInstance(this);
			else
			{
				AnimaLogger::LogMessageFormat("WARNING - Error reading mesh instance named '%s': this mesh instance is derived from a mesh named '%s' but this mesh can't be found, so this instance won't be rendered", GetName().c_str(), _meshName.c_str());
			}
		}
		else
		{
			AnimaLogger::LogMessageFormat("WARNING - Error reading mesh instance named '%s': this mesh instance has a blank source mesh name so this instance won't be rendered", GetName().c_str());
		}
		
		for(auto& shadersName : objectTree.get_child("AnimaMeshInstance.ShaderNames"))
		{
			if(shadersName.first == "ShaderName")
			{
				AnimaString shaderName = shadersName.second.get_value<AnimaString>("");
				_shadersNames.push_back(shaderName);
			}
		}
		
		ptree sceneObjectTree = objectTree.get_child("AnimaMeshInstance.SceneObject");
		if (AnimaSceneObject::ReadObject(sceneObjectTree, scene, false))
		{
			SetPosition(GetPosition());
			return true;
		}
		
		return false;
	}
	catch (boost::property_tree::ptree_bad_path& exception)
	{
		AnimaLogger::LogMessageFormat("ERROR - Error parsing mesh instance: %s", exception.what());
		return false;
	}
	catch (boost::property_tree::ptree_bad_data& exception)
	{
		AnimaLogger::LogMessageFormat("ERROR - Error parsing mesh instance: %s", exception.what());
		return false;
	}
}

void AnimaMeshInstance::SetMaterial(AnimaMaterial* material)
{
	_material = material;
	if (_material)
		_materialName = material->GetName();
	else
		_materialName = "";
}

AnimaMaterial* AnimaMeshInstance::GetMaterial()
{
	return _material;
}

AnimaMesh* AnimaMeshInstance::GetMesh() const
{
	return _mesh;
}

void AnimaMeshInstance::SetMesh(AnimaMesh* mesh)
{
	_mesh = mesh;
}

void AnimaMeshInstance::Draw(AnimaRenderer* renderer, AnimaShaderProgram* program, bool start, bool end, bool updateMaterial)
{
	ANIMA_FRAME_PUSH("MeshInstanceUpdateProp");
	program->UpdateSceneObjectProperties(this, renderer);
	ANIMA_FRAME_POP();

	if (updateMaterial)
	{
		AnimaMaterial* material = _material;
		if (material == nullptr)
			material = AnimaMaterialsManager::GetDefaultMaterial();

		ANIMA_FRAME_PUSH("MeshInstanceMaterialUpdateProp");
		program->UpdateMappedValuesObjectProperties(material, renderer);
		ANIMA_FRAME_POP();
	}

#ifdef _WIN32
	if (start)
	{
		ANIMA_FRAME_PUSH("BindVAO");
		glBindVertexArray(_mesh->GetVertexArrayObject());
		ANIMA_FRAME_POP();
	}

	ANIMA_FRAME_PUSH("DrawElements");
	glDrawElements(GL_TRIANGLES, _mesh->GetFacesIndicesCount(), GL_UNSIGNED_INT, 0);
	ANIMA_FRAME_POP();

	if (end)
	{
		ANIMA_FRAME_PUSH("Bind0");
		glBindVertexArray(0);
		ANIMA_FRAME_POP();
	}

#else
	program->EnableInputs(_mesh);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _mesh->GetIndexesBufferObject());
	glDrawElements(GL_TRIANGLES, _mesh->GetFacesIndicesCount(), GL_UNSIGNED_INT, 0);
	program->DisableInputs();
#endif
}

void AnimaMeshInstance::AddShader(AnimaShader* shader)
{
	_shadersNames.push_back(shader->GetName());
}

void AnimaMeshInstance::AddShader(const AnimaString& shaderName)
{
	_shadersNames.push_back(shaderName);
}

AInt AnimaMeshInstance::GetShadersCount() const
{
	return _shadersNames.size();
}

AnimaString AnimaMeshInstance::GetShaderName(AInt index) const
{
	return _shadersNames[index];
}

void AnimaMeshInstance::SetShaderProgram(AnimaShaderProgram* program)
{
	_shaderProgram = program;
	if (_shaderProgram)
		_shaderProgramName = _shaderProgram->GetName();
	else
		_shaderProgramName = "";
}

AnimaShaderProgram* AnimaMeshInstance::GetShaderProgram()
{
	return _shaderProgram;
}

void AnimaMeshInstance::SetShaderProgramName(const AnimaString& shaderProgramName)
{
	_shaderProgramName = shaderProgramName;
}

AnimaString AnimaMeshInstance::GetShaderProgramName() const
{
	return _shaderProgramName;
}

void AnimaMeshInstance::InitializePhysicData()
{
	if(_mesh)
	{
		_mesh->ComputeBoundingBox();
		
		AnimaVertex3f rotationVector = _transformation.GetCompleteRotation();
		AnimaVertex3f positionVector = _transformation.GetCompleteTranslation() + GetPosition();
		AnimaVertex3f scaleVector = _transformation.GetCompleteScale();
		
		AnimaQuaternion xRot(AnimaVertex3f(1.0, 0.0, 0.0), rotationVector.x);
		AnimaQuaternion yRot(AnimaVertex3f(0.0, 1.0, 0.0), rotationVector.y);
		AnimaQuaternion zRot(AnimaVertex3f(0.0, 0.0, 1.0), rotationVector.z);
		AnimaQuaternion rot = xRot * yRot * zRot;
		
		if(_physCollisionShape == nullptr)
		{
			//btConvexHullShape* tempShape = new btConvexHullShape();
			
			//AInt count = _mesh->GetFacesCount();
			//for(AInt i = 0; i < count; i++)
			//{
			//	AnimaFace face = _mesh->GetFace(i);
			//	
			//	AnimaVertex3f vertex = _mesh->GetVertex(face.GetIndex(0));
			//	tempShape->addPoint(btVector3(vertex.x, vertex.y, vertex.z));

			//	vertex = _mesh->GetVertex(face.GetIndex(1));
			//	tempShape->addPoint(btVector3(vertex.x, vertex.y, vertex.z));
			//	
			//	vertex = _mesh->GetVertex(face.GetIndex(2));
			//	tempShape->addPoint(btVector3(vertex.x, vertex.y, vertex.z));
			//}

			//btShapeHull* hull = new btShapeHull(tempShape);
			//btScalar margin = tempShape->getMargin();
			//hull->buildHull(margin);

			//_physCollisionShape = new btConvexHullShape((btScalar*)hull->getVertexPointer(), hull->numVertices());
			////_physCollisionShape = tempShape;

			//delete hull;
			//hull = nullptr;

			//delete tempShape;
			//tempShape = nullptr;

			_indices.clear();
			_vertices.clear();

			AInt facesCount = _mesh->GetFacesCount();
			for (AInt i = 0; i < facesCount; i++)
			{
				AnimaFace face = _mesh->GetFace(i);

				_indices.push_back(face.GetIndex(0));
				_indices.push_back(face.GetIndex(1));
				_indices.push_back(face.GetIndex(2));
			}
			AInt verticesCount = _mesh->GetVerticesCount();
			for (AInt i = 0; i < verticesCount; i++)
			{
				AnimaVertex3f v = _mesh->GetVertex(i);
				_vertices.push_back(v.x);
				_vertices.push_back(v.y);
				_vertices.push_back(v.z);
			}

			btIndexedMesh im;
			im.m_vertexBase = (const AUchar*)&_vertices[0];
			im.m_vertexStride = sizeof(AFloat) * 3;
			im.m_numVertices = _vertices.size();
			im.m_triangleIndexBase = (const AUchar*)&_indices[0];
			im.m_triangleIndexStride = sizeof(AInt) * 3;
			im.m_numTriangles = facesCount;
			im.m_indexType = PHY_INTEGER;
			im.m_vertexType = PHY_FLOAT;

			_meshInterface = new btTriangleIndexVertexArray();
			_meshInterface->addIndexedMesh(im);

			//_physCollisionShape = new btConvexTriangleMeshShape(_meshInterface);
			btConvexTriangleMeshShape* tempShape = new btConvexTriangleMeshShape(_meshInterface);

			btShapeHull* hull = new btShapeHull(tempShape);
			btScalar margin = tempShape->getMargin();
			hull->buildHull(margin);

			_physCollisionShape = new btConvexHullShape((btScalar*)hull->getVertexPointer(), hull->numVertices());

			delete hull;
			hull = nullptr;

			delete tempShape;
			tempShape = nullptr;
		}
	
		if(_physMotionState == nullptr)
			_physMotionState = new btDefaultMotionState(btTransform(btQuaternion(rot.x, rot.y, rot.z, rot.w), btVector3(positionVector.x, positionVector.y, positionVector.z)));
		
		if(_physRigidBody == nullptr)
		{
			if(_physMotionState != nullptr && _physCollisionShape)
			{
				_physCollisionShape->setLocalScaling(btVector3(scaleVector.x, scaleVector.y, scaleVector.z));
				
				AFloat massa = GetAncestorObject()->GetFloat("Mass");
				btVector3 localInertia(0.0f, 0.0f, 0.0f);
				
				if(massa > 0.0f)
					_physCollisionShape->calculateLocalInertia(massa, localInertia);
				
				// Il primo argomento è la massa e l'ultimo l'inerzia
				btRigidBody::btRigidBodyConstructionInfo ci(massa, _physMotionState, _physCollisionShape, localInertia);
				
				_physRigidBody = new btRigidBody(ci);
				_physRigidBody->setUserPointer(this);
			}
		}
		else
		{
			btTransform worldTransform;
			_physRigidBody->getMotionState()->getWorldTransform(worldTransform);
			worldTransform.setOrigin(btVector3(positionVector.x, positionVector.y, positionVector.z));
			worldTransform.setRotation(btQuaternion(rot.x, rot.y, rot.z, rot.w));
			_physRigidBody->getMotionState()->setWorldTransform(worldTransform);
		}
	}
}

END_ANIMA_ENGINE_NAMESPACE
