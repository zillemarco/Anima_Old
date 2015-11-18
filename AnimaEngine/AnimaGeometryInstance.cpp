//
//  AnimaGeometryInstance.cpp
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#include "AnimaGeometryInstance.h"
#include "AnimaShaderProgram.h"
#include "AnimaRenderer.h"
#include "AnimaScene.h"
#include "AnimaMaterialsManager.h"
#include "AnimaGeometriesManager.h"
#include <BulletCollision/CollisionShapes/btShapeHull.h>

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaGeometryInstance::AnimaGeometryInstance(const AnimaString& name, AnimaDataGeneratorsManager* dataGeneratorsManager, AnimaAllocator* allocator)
	: AnimaSceneObject(name, dataGeneratorsManager, allocator)
{
	IMPLEMENT_ANIMA_CLASS(AnimaGeometryInstance);
	_material = nullptr;
	_geometry = nullptr;
	_shaderProgramName = "";
	_shaderProgram = nullptr;
	
	_physCollisionShape = nullptr;
	_physMotionState = nullptr;
	_physRigidBody = nullptr;
	_geometryInterface = nullptr;
	
	_shaderSource = ASDSO_GEOMETRY;
}

AnimaGeometryInstance::AnimaGeometryInstance(const AnimaGeometryInstance& src)
	: AnimaSceneObject(src)
	, _materialName(src._materialName)
	, _geometryName(src._geometryName)
	, _shadersNames(src._shadersNames)
	, _shaderProgram(src._shaderProgram)
	, _shaderProgramName(src._shaderProgramName)
{
	_material = src._material;
	_geometry = src._geometry;
	_physCollisionShape = src._physCollisionShape;
	_physMotionState = src._physMotionState;
	_physRigidBody = src._physRigidBody;
}

AnimaGeometryInstance::AnimaGeometryInstance(AnimaGeometryInstance&& src)
	: AnimaSceneObject(src)
	, _materialName(src._materialName)
	, _geometryName(src._geometryName)
	, _shadersNames(src._shadersNames)
	, _shaderProgram(src._shaderProgram)
	, _shaderProgramName(src._shaderProgramName)
{
	_material = src._material;
	_geometry = src._geometry;
	_physCollisionShape = src._physCollisionShape;
	_physMotionState = src._physMotionState;
	_physRigidBody = src._physRigidBody;
}

AnimaGeometryInstance::~AnimaGeometryInstance()
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

	if (_geometryInterface != nullptr)
	{
		delete _geometryInterface;
		_geometryInterface = nullptr;
	}
}

AnimaGeometryInstance& AnimaGeometryInstance::operator=(const AnimaGeometryInstance& src)
{
	if (this != &src)
	{
		AnimaSceneObject::operator=(src);

		_material = src._material;
		_materialName = src._materialName;
		
		_geometry = src._geometry;
		_geometryName = src._geometryName;

		_shadersNames = src._shadersNames;
		_shaderProgramName = src._shaderProgramName;
		_shaderProgram = src._shaderProgram;
		_physCollisionShape = src._physCollisionShape;
		_physMotionState = src._physMotionState;
		_physRigidBody = src._physRigidBody;
	}
	
	return *this;
}

AnimaGeometryInstance& AnimaGeometryInstance::operator=(AnimaGeometryInstance&& src)
{
	if (this != &src)
	{
		AnimaSceneObject::operator=(src);

		_material = src._material;
		_materialName = src._materialName;

		_geometry = src._geometry;
		_geometryName = src._geometryName;

		_shadersNames = src._shadersNames;
		_shaderProgramName = src._shaderProgramName;
		_shaderProgram = src._shaderProgram;
		_physCollisionShape = src._physCollisionShape;
		_physMotionState = src._physMotionState;
		_physRigidBody = src._physRigidBody;
	}
	
	return *this;
}

ptree AnimaGeometryInstance::GetObjectTree(bool saveName) const
{
	ptree tree;
	
	if (saveName)
		tree.add("AnimaGeometryInstance.Name", GetName());
	
	AnimaString materialName = _materialName;
	if (materialName.empty() && _material != nullptr)
		materialName = _material->GetName();
	tree.put("AnimaGeometryInstance.Material", materialName);
	
	AnimaString geometryName = _geometryName;
	if(geometryName.empty() && _geometry != nullptr)
		geometryName = _geometry->GetName();
	tree.put("AnimaGeometryInstance.GeometryName", geometryName);
	
	ptree shadersNamesTree;
	for(auto& shaderName : _shadersNames)
		shadersNamesTree.add("ShaderName", shaderName);
	tree.add_child("AnimaGeometryInstance.ShaderNames", shadersNamesTree);
	
	tree.add_child("AnimaGeometryInstance.SceneObject", AnimaSceneObject::GetObjectTree(false));
	
	return tree;
}

bool AnimaGeometryInstance::ReadObject(const ptree& objectTree, AnimaScene* scene, bool readName)
{
	try
	{
		if (readName)
			SetName(objectTree.get<AnimaString>("AnimaGeometryInstance.Name"));
		
		_materialName = objectTree.get<AnimaString>("AnimaGeometryInstance.Material", "");
		_geometryName = objectTree.get<AnimaString>("AnimaGeometryInstance.GeometryName", "");
		
		if (!_materialName.empty())
		{
			_material = scene->GetMaterialsManager()->GetMaterialFromName(_materialName);
			if(_material == nullptr)
			{
				AnimaLogger::LogMessageFormat("WARNING - Error reading geometry instance named '%s': this geometry instance has material named '%s' but this material can't be found", GetName().c_str(), _materialName.c_str());
			}
		}
		else
		{
			AnimaLogger::LogMessageFormat("WARNING - Error reading geometry instance named '%s': this geometry instance has a blank material name", GetName().c_str());
		}
		
		if (!_geometryName.empty())
		{
			_geometry = scene->GetGeometriesManager()->GetGeometryFromName(_geometryName);
			if(_geometry != nullptr)
				_geometry->AddInstance(this);
			else
			{
				AnimaLogger::LogMessageFormat("WARNING - Error reading geometry instance named '%s': this geometry instance is derived from a geometry named '%s' but this geometry can't be found, so this instance won't be rendered", GetName().c_str(), _geometryName.c_str());
			}
		}
		else
		{
			AnimaLogger::LogMessageFormat("WARNING - Error reading geometry instance named '%s': this geometry instance has a blank source geometry name so this instance won't be rendered", GetName().c_str());
		}
		
		for(auto& shadersName : objectTree.get_child("AnimaGeometryInstance.ShaderNames"))
		{
			if(shadersName.first == "ShaderName")
			{
				AnimaString shaderName = shadersName.second.get_value<AnimaString>("");
				_shadersNames.push_back(shaderName);
			}
		}
		
		ptree sceneObjectTree = objectTree.get_child("AnimaGeometryInstance.SceneObject");
		if (AnimaSceneObject::ReadObject(sceneObjectTree, scene, false))
		{
			SetPosition(GetPosition());
			return true;
		}
		
		return false;
	}
	catch (boost::property_tree::ptree_bad_path& exception)
	{
		AnimaLogger::LogMessageFormat("ERROR - Error parsing geometry instance: %s", exception.what());
		return false;
	}
	catch (boost::property_tree::ptree_bad_data& exception)
	{
		AnimaLogger::LogMessageFormat("ERROR - Error parsing geometry instance: %s", exception.what());
		return false;
	}
}

void AnimaGeometryInstance::SetMaterial(AnimaMaterial* material)
{
	_material = material;
	if (_material)
		_materialName = material->GetName();
	else
		_materialName = "";
}

AnimaMaterial* AnimaGeometryInstance::GetMaterial()
{
	return _material;
}

AnimaGeometry* AnimaGeometryInstance::GetGeometry() const
{
	return _geometry;
}

void AnimaGeometryInstance::SetGeometry(AnimaGeometry* geometry)
{
	_geometry = geometry;
}

void AnimaGeometryInstance::Draw(AnimaRenderer* renderer, AnimaShaderProgram* program, bool start, bool end, bool updateMaterial)
{
	ANIMA_FRAME_PUSH("GeometryInstanceUpdateProp");
	program->UpdateSceneObjectProperties(this, renderer);
	ANIMA_FRAME_POP();

	if (updateMaterial)
	{
		AnimaMaterial* material = _material;
		if (material == nullptr)
			material = AnimaMaterialsManager::GetDefaultMaterial();

		ANIMA_FRAME_PUSH("GeometryInstanceMaterialUpdateProp");
		program->UpdateMappedValuesObjectProperties(material, renderer);
		ANIMA_FRAME_POP();
	}

#ifdef _WIN32
	if (start)
	{
		ANIMA_FRAME_PUSH("BindVAO");
		glBindVertexArray(_geometry->GetVertexArrayObject());
		ANIMA_FRAME_POP();
	}

	ANIMA_FRAME_PUSH("DrawElements");
	glDrawElements(GL_TRIANGLES, _geometry->GetFacesIndicesCount(), GL_UNSIGNED_INT, 0);
	ANIMA_FRAME_POP();

	if (end)
	{
		ANIMA_FRAME_PUSH("Bind0");
		glBindVertexArray(0);
		ANIMA_FRAME_POP();
	}

#else
	program->EnableInputs(_geometry);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _geometry->GetIndexesBufferObject());
	glDrawElements(GL_TRIANGLES, _geometry->GetFacesIndicesCount(), GL_UNSIGNED_INT, 0);
	program->DisableInputs();
#endif
}

void AnimaGeometryInstance::AddShader(AnimaShader* shader)
{
	_shadersNames.push_back(shader->GetName());
}

void AnimaGeometryInstance::AddShader(const AnimaString& shaderName)
{
	_shadersNames.push_back(shaderName);
}

AInt AnimaGeometryInstance::GetShadersCount() const
{
	return _shadersNames.size();
}

AnimaString AnimaGeometryInstance::GetShaderName(AInt index) const
{
	return _shadersNames[index];
}

void AnimaGeometryInstance::SetShaderProgram(AnimaShaderProgram* program)
{
	_shaderProgram = program;
	if (_shaderProgram)
		_shaderProgramName = _shaderProgram->GetName();
	else
		_shaderProgramName = "";
}

AnimaShaderProgram* AnimaGeometryInstance::GetShaderProgram()
{
	return _shaderProgram;
}

void AnimaGeometryInstance::SetShaderProgramName(const AnimaString& shaderProgramName)
{
	_shaderProgramName = shaderProgramName;
}

AnimaString AnimaGeometryInstance::GetShaderProgramName() const
{
	return _shaderProgramName;
}

void AnimaGeometryInstance::InitializePhysicData()
{
	if(_geometry)
	{
		_geometry->ComputeBoundingBox();
		
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
			
			//AInt count = _geometry->GetFacesCount();
			//for(AInt i = 0; i < count; i++)
			//{
			//	AnimaFace face = _geometry->GetFace(i);
			//	
			//	AnimaVertex3f vertex = _geometry->GetVertex(face.GetIndex(0));
			//	tempShape->addPoint(btVector3(vertex.x, vertex.y, vertex.z));

			//	vertex = _geometry->GetVertex(face.GetIndex(1));
			//	tempShape->addPoint(btVector3(vertex.x, vertex.y, vertex.z));
			//	
			//	vertex = _geometry->GetVertex(face.GetIndex(2));
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

			AInt facesCount = _geometry->GetFacesCount();
			for (AInt i = 0; i < facesCount; i++)
			{
				AnimaFace face = _geometry->GetFace(i);

				_indices.push_back(face.GetIndex(0));
				_indices.push_back(face.GetIndex(1));
				_indices.push_back(face.GetIndex(2));
			}
			AInt verticesCount = _geometry->GetVerticesCount();
			for (AInt i = 0; i < verticesCount; i++)
			{
				AnimaVertex3f v = _geometry->GetVertex(i);
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

			_geometryInterface = new btTriangleIndexVertexArray();
			_geometryInterface->addIndexedMesh(im);

			//_physCollisionShape = new btConvexTriangleGeometryShape(_geometryInterface);
			btConvexTriangleMeshShape* tempShape = new btConvexTriangleMeshShape(_geometryInterface);

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
