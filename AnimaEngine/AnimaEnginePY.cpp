#include "AnimaEnginePY.h"

#if defined ANIMA_ENGINE_EXPORT_TO_PYTHON

#include "AnimaTypes.h"
#include "AnimaString.h"
#include "AnimaVertex.h"
#include "AnimaEngine.h"
#include "AnimaAllocators.h"
#include "AnimaShadersManager.h"
#include "AnimaScenesManager.h"
#include "AnimaDataGeneratorsManager.h"
#include "AnimaNodesManager.h"
#include "AnimaLightsManager.h"
#include "AnimaGeometriesManager.h"
#include "AnimaNodeInstancesManager.h"
#include "AnimaGeometryInstancesManager.h"
#include "AnimaCamerasManager.h"
#include "AnimaTexturesManager.h"
#include "AnimaMaterialsManager.h"
#include "AnimaAnimationsManager.h"
#include "AnimaCamerasManager.h"
#include "AnimaLightsManager.h"
#include "AnimaRenderer.h"
#include "AnimaTimer.h"
#include "IAnimaEvents.h"
#include "IAnimaInteractor.h"
#include "AnimaMouseInteractor.h"
#include "AnimaKeyboardInteractor.h"
#include "AnimaJoystickInteractor.h"
#include "AnimaDefaultInteractors.h"

#include "AnimaScene.h"
#include "AnimaNode.h"
#include "AnimaAnimation.h"
#include "AnimaGeometry.h"
#include "AnimaNodeInstance.h"
#include "AnimaGeometryInstance.h"
#include "AnimaShader.h"
#include "AnimaShaderProgram.h"
#include "AnimaCamera.h"
#include "AnimaLight.h"
#include "AnimaFirstPersonCamera.h"
#include "AnimaThirdPersonCamera.h"
#include "AnimaCamera.h"
#include "AnimaAnimation.h"
#include "AnimaDataGenerator.h"
#include "AnimaColorGenerator.h"
#include "AnimaTextureGenerator.h"
#include "AnimaVectorGenerator.h"

#include "AnimaGC.h"

#include <boost/unordered_map.hpp>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

#include <boost/python/suite/indexing/indexing_suite.hpp>
#include <boost/python/iterator.hpp>
#include <boost/python/call_method.hpp>
#include <boost/python/tuple.hpp>

namespace boost { namespace python {
	
	// Forward declaration
	template <class Container, bool NoProxy, class DerivedPolicies>
	class pointer_map_indexing_suite;
	
	namespace detail
	{
		template <class Container, bool NoProxy>
		class final_pointer_map_derived_policies
		: public pointer_map_indexing_suite<Container,
		NoProxy, final_pointer_map_derived_policies<Container, NoProxy> > {};
	}
	
	template <
	class Container,
	bool NoProxy = false,
	class DerivedPolicies
	= detail::final_pointer_map_derived_policies<Container, NoProxy> >
	class pointer_map_indexing_suite
	: public indexing_suite<
	Container
	, DerivedPolicies
	, NoProxy
	, true
	, typename Container::value_type::second_type
	, typename Container::key_type
	, typename Container::key_type
	>
	{
	public:
		
		typedef typename Container::value_type value_type;
		typedef typename Container::value_type::second_type data_type;
		typedef typename Container::key_type key_type;
		typedef typename Container::key_type index_type;
		typedef typename Container::size_type size_type;
		typedef typename Container::difference_type difference_type;
		
		template <class Class>
		static void
		extension_def(Class& cl)
		{
			//  Wrap the map's element (value_type)
			std::string elem_name = "pointer_map_indexing_suite";
			object class_name(cl.attr("__name__"));
			extract<std::string> class_name_extractor(class_name);
			elem_name += class_name_extractor();
			elem_name += "_entry";
			
			class_<value_type>(elem_name.c_str())
			.def("__repr__", &DerivedPolicies::print_elem)
			.def("data", &DerivedPolicies::get_data, return_value_policy<reference_existing_object>())
			.def("key", &DerivedPolicies::get_key)
			;
		}
		
		static object
		print_elem(typename Container::value_type const& e)
		{
			return "(%s, %s)" % python::make_tuple(e.first, e.second);
		}
		
		static
		typename mpl::if_<
		mpl::and_<is_class<data_type>, mpl::bool_<!NoProxy> >
		, data_type&
		, data_type
		>::type
		get_data(typename Container::value_type& e)
		{
			return e.second;
		}
		
		static typename Container::key_type
		get_key(typename Container::value_type& e)
		{
			return e.first;
		}
		
		static data_type&
		get_item(Container& container, index_type i_)
		{
			typename Container::iterator i = container.find(i_);
			if (i == container.end())
			{
				PyErr_SetString(PyExc_KeyError, "Invalid key");
				throw_error_already_set();
			}
			return i->second;
		}
		
		static void
		set_item(Container& container, index_type i, data_type const& v)
		{
			container[i] = v;
		}
		
		static void
		delete_item(Container& container, index_type i)
		{
			container.erase(i);
		}
		
		static size_t
		size(Container& container)
		{
			return container.size();
		}
		
		static bool
		contains(Container& container, key_type const& key)
		{
			return container.find(key) != container.end();
		}
		
		static bool
		compare_index(Container& container, index_type a, index_type b)
		{
			return container.key_comp()(a, b);
		}
		
		static index_type
		convert_index(Container& /*container*/, PyObject* i_)
		{
			extract<key_type const&> i(i_);
			if (i.check())
			{
				return i();
			}
			else
			{
				extract<key_type> i(i_);
				if (i.check())
					return i();
			}
			
			PyErr_SetString(PyExc_TypeError, "Invalid index type");
			throw_error_already_set();
			return index_type();
		}
	};
	
}} // namespace boost::python

using namespace boost::python;

struct AnimaString_to_python_str
{
	static PyObject* convert(Anima::AnimaString const& str)
	{
		return incref(object(str.c_str()).ptr());
	}
};

struct AnimaString_from_python_str
{
	AnimaString_from_python_str()
	{
		converter::registry::push_back(&convertible, &construct, type_id<Anima::AnimaString>());
	}

	static void* convertible(PyObject* obj_ptr)
	{
		if (!PyString_Check(obj_ptr))
			return NULL;
		return obj_ptr;
	}

	static void construct(PyObject* obj_ptr, converter::rvalue_from_python_stage1_data* data)
	{
		const char* value = PyString_AsString(obj_ptr);
		ANIMA_ASSERT(value);

		void* storage = ((converter::rvalue_from_python_storage<Anima::AnimaString>*)data)->storage.bytes;

		new (storage)Anima::AnimaString(value);

		data->convertible = storage;
	}
};

void (Anima::AnimaRenderer::*AR_Start1)(Anima::AnimaScene* scene) = &Anima::AnimaRenderer::Start;

void (Anima::AnimaScenesManager::*SaveSceneToFileWithName)(const Anima::AnimaString&, const Anima::AnimaString&, bool) = &Anima::AnimaScenesManager::SaveSceneToFile;
void (Anima::AnimaScenesManager::*SaveSceneToFileWithScene)(Anima::AnimaScene*, const Anima::AnimaString&, bool) = &Anima::AnimaScenesManager::SaveSceneToFile;

void (Anima::AnimaNodesManager::*SaveNodeToFileWithName)(const Anima::AnimaString&, const Anima::AnimaString&, bool) = &Anima::AnimaNodesManager::SaveNodeToFile;
void (Anima::AnimaNodesManager::*SaveNodeToFileWithNode)(Anima::AnimaNode*, const Anima::AnimaString&, bool) = &Anima::AnimaNodesManager::SaveNodeToFile;

void (Anima::AnimaNodeInstancesManager::*SaveNodeInstanceToFileWithName)(const Anima::AnimaString&, const Anima::AnimaString&, bool) = &Anima::AnimaNodeInstancesManager::SaveNodeInstanceToFile;
void (Anima::AnimaNodeInstancesManager::*SaveNodeInstanceToFileWithNode)(Anima::AnimaNodeInstance*, const Anima::AnimaString&, bool) = &Anima::AnimaNodeInstancesManager::SaveNodeInstanceToFile;

Anima::AnimaNodeInstance* (Anima::AnimaNodeInstancesManager::*CreateNodeInstanceWithName)(const Anima::AnimaString&, const Anima::AnimaString&) = &Anima::AnimaNodeInstancesManager::CreateNodeInstance;
Anima::AnimaNodeInstance* (Anima::AnimaNodeInstancesManager::*CreateNodeInstanceWithNode)(const Anima::AnimaString&, Anima::AnimaNode*) = &Anima::AnimaNodeInstancesManager::CreateNodeInstance;
Anima::AnimaNodeInstance* (Anima::AnimaNodeInstancesManager::*CreateAssetInstanceWithName)(const Anima::AnimaString&, const Anima::AnimaString&) = &Anima::AnimaNodeInstancesManager::CreateAssetInstance;
Anima::AnimaNodeInstance* (Anima::AnimaNodeInstancesManager::*CreateAssetInstanceWithAsset)(const Anima::AnimaString&, Anima::AnimaNode*) = &Anima::AnimaNodeInstancesManager::CreateAssetInstance;

void (Anima::AnimaGeometriesManager::*SaveGeometryToFileWithName)(const Anima::AnimaString&, const Anima::AnimaString&, bool) = &Anima::AnimaGeometriesManager::SaveGeometryToFile;
void (Anima::AnimaGeometriesManager::*SaveGeometryToFileWithGeometry)(Anima::AnimaGeometry*, const Anima::AnimaString&, bool) = &Anima::AnimaGeometriesManager::SaveGeometryToFile;

void (Anima::AnimaGeometryInstancesManager::*SaveGeometryInstanceToFileWithName)(const Anima::AnimaString&, const Anima::AnimaString&, bool) = &Anima::AnimaGeometryInstancesManager::SaveGeometryInstanceToFile;
void (Anima::AnimaGeometryInstancesManager::*SaveGeometryInstanceToFileWithGeometry)(Anima::AnimaGeometryInstance*, const Anima::AnimaString&, bool) = &Anima::AnimaGeometryInstancesManager::SaveGeometryInstanceToFile;

Anima::AnimaGeometryInstance* (Anima::AnimaGeometryInstancesManager::*CreateGeometryInstanceWithName)(const Anima::AnimaString&, const Anima::AnimaString&) = &Anima::AnimaGeometryInstancesManager::CreateGeometryInstance;
Anima::AnimaGeometryInstance* (Anima::AnimaGeometryInstancesManager::*CreateGeometryInstanceWithGeometry)(const Anima::AnimaString&, Anima::AnimaGeometry*) = &Anima::AnimaGeometryInstancesManager::CreateGeometryInstance;

void (Anima::AnimaCamerasManager::*SaveCameraToFileWithName)(const Anima::AnimaString&, const Anima::AnimaString&, bool) = &Anima::AnimaCamerasManager::SaveCameraToFile;
void (Anima::AnimaCamerasManager::*SaveCameraToFileWithCamera)(Anima::AnimaCamera*, const Anima::AnimaString&, bool) = &Anima::AnimaCamerasManager::SaveCameraToFile;

void (Anima::AnimaLightsManager::*SaveLightToFileWithName)(const Anima::AnimaString&, const Anima::AnimaString&, bool) = &Anima::AnimaLightsManager::SaveLightToFile;
void (Anima::AnimaLightsManager::*SaveLightToFileWithLight)(Anima::AnimaLight*, const Anima::AnimaString&, bool) = &Anima::AnimaLightsManager::SaveLightToFile;

void (Anima::AnimaMaterialsManager::*SaveMaterialToFileWithName)(const Anima::AnimaString&, const Anima::AnimaString&, bool) = &Anima::AnimaMaterialsManager::SaveMaterialToFile;
void (Anima::AnimaMaterialsManager::*SaveMaterialToFileWithMaterial)(Anima::AnimaMaterial*, const Anima::AnimaString&, bool) = &Anima::AnimaMaterialsManager::SaveMaterialToFile;

Anima::AnimaTexture* (Anima::AnimaTexturesManager::*LoadTextureFromAnimaTextureFile)(const Anima::AnimaString&) = &Anima::AnimaTexturesManager::LoadTextureFromFile;
Anima::AnimaTexture* (Anima::AnimaTexturesManager::*LoadTextureFromImageFile)(const Anima::AnimaString&, const Anima::AnimaString&) = &Anima::AnimaTexturesManager::LoadTextureFromFile;
void (Anima::AnimaTexturesManager::*SaveTextureToFileWithName)(const Anima::AnimaString&, const Anima::AnimaString&, bool) = &Anima::AnimaTexturesManager::SaveTextureToFile;
void (Anima::AnimaTexturesManager::*SaveTextureToFileWithTexture)(Anima::AnimaTexture*, const Anima::AnimaString&, bool) = &Anima::AnimaTexturesManager::SaveTextureToFile;

bool (Anima::AnimaDataGeneratorsManager::*RemoveGeneratorWithName)(const Anima::AnimaString&) = &Anima::AnimaDataGeneratorsManager::RemoveGenerator;
bool (Anima::AnimaDataGeneratorsManager::*RemoveGeneratorWithGenerator)(Anima::AnimaDataGenerator*) = &Anima::AnimaDataGeneratorsManager::RemoveGenerator;

void (Anima::AnimaMappedValues::*AMV_AddTexture1)(const Anima::AnimaString&, Anima::AnimaTextureGenerator*) = &Anima::AnimaMappedValues::AddTexture;
void (Anima::AnimaMappedValues::*AMV_AddTexture2)(const Anima::AnimaString&, Anima::AnimaTexture*) = &Anima::AnimaMappedValues::AddTexture;
void (Anima::AnimaMappedValues::*AMV_SetTexture1)(const Anima::AnimaString&, Anima::AnimaTextureGenerator*) = &Anima::AnimaMappedValues::SetTexture;
void (Anima::AnimaMappedValues::*AMV_SetTexture2)(const Anima::AnimaString&, Anima::AnimaTexture*) = &Anima::AnimaMappedValues::SetTexture;
void (Anima::AnimaMappedValues::*AMV_AddColor1)(const Anima::AnimaString&, Anima::AnimaColorGenerator*) = &Anima::AnimaMappedValues::AddColor;
void (Anima::AnimaMappedValues::*AMV_AddColor2)(const Anima::AnimaString&, Anima::AnimaColor3f) = &Anima::AnimaMappedValues::AddColor;
void (Anima::AnimaMappedValues::*AMV_AddColor3)(const Anima::AnimaString&, Anima::AFloat, Anima::AFloat, Anima::AFloat) = &Anima::AnimaMappedValues::AddColor;
void (Anima::AnimaMappedValues::*AMV_AddColor4)(const Anima::AnimaString&, Anima::AnimaColor4f) = &Anima::AnimaMappedValues::AddColor;
void (Anima::AnimaMappedValues::*AMV_AddColor5)(const Anima::AnimaString&, Anima::AFloat, Anima::AFloat, Anima::AFloat, Anima::AFloat) = &Anima::AnimaMappedValues::AddColor;
void (Anima::AnimaMappedValues::*AMV_SetColor1)(const Anima::AnimaString&, Anima::AnimaColorGenerator*) = &Anima::AnimaMappedValues::SetColor;
void (Anima::AnimaMappedValues::*AMV_SetColor2)(const Anima::AnimaString&, Anima::AnimaColor3f) = &Anima::AnimaMappedValues::SetColor;
void (Anima::AnimaMappedValues::*AMV_SetColor3)(const Anima::AnimaString&, Anima::AFloat, Anima::AFloat, Anima::AFloat) = &Anima::AnimaMappedValues::SetColor;
void (Anima::AnimaMappedValues::*AMV_SetColor4)(const Anima::AnimaString&, Anima::AnimaColor4f) = &Anima::AnimaMappedValues::SetColor;
void (Anima::AnimaMappedValues::*AMV_SetColor5)(const Anima::AnimaString&, Anima::AFloat, Anima::AFloat, Anima::AFloat, Anima::AFloat) = &Anima::AnimaMappedValues::SetColor;
void (Anima::AnimaMappedValues::*AMV_AddVector1)(const Anima::AnimaString&, Anima::AnimaVectorGenerator*) = &Anima::AnimaMappedValues::AddVector;
void (Anima::AnimaMappedValues::*AMV_AddVector2)(const Anima::AnimaString&, Anima::AnimaVertex2f) = &Anima::AnimaMappedValues::AddVector;
void (Anima::AnimaMappedValues::*AMV_AddVector3)(const Anima::AnimaString&, Anima::AFloat, Anima::AFloat) = &Anima::AnimaMappedValues::AddVector;
void (Anima::AnimaMappedValues::*AMV_AddVector4)(const Anima::AnimaString&, Anima::AnimaVertex3f) = &Anima::AnimaMappedValues::AddVector;
void (Anima::AnimaMappedValues::*AMV_AddVector5)(const Anima::AnimaString&, Anima::AFloat, Anima::AFloat, Anima::AFloat) = &Anima::AnimaMappedValues::AddVector;
void (Anima::AnimaMappedValues::*AMV_AddVector6)(const Anima::AnimaString&, Anima::AnimaVertex4f) = &Anima::AnimaMappedValues::AddVector;
void (Anima::AnimaMappedValues::*AMV_AddVector7)(const Anima::AnimaString&, Anima::AFloat, Anima::AFloat, Anima::AFloat, Anima::AFloat) = &Anima::AnimaMappedValues::AddVector;
void (Anima::AnimaMappedValues::*AMV_SetVector1)(const Anima::AnimaString&, Anima::AnimaVectorGenerator*) = &Anima::AnimaMappedValues::SetVector;
void (Anima::AnimaMappedValues::*AMV_SetVector2)(const Anima::AnimaString&, Anima::AnimaVertex2f) = &Anima::AnimaMappedValues::SetVector;
void (Anima::AnimaMappedValues::*AMV_SetVector3)(const Anima::AnimaString&, Anima::AFloat, Anima::AFloat) = &Anima::AnimaMappedValues::SetVector;
void (Anima::AnimaMappedValues::*AMV_SetVector4)(const Anima::AnimaString&, Anima::AnimaVertex3f) = &Anima::AnimaMappedValues::SetVector;
void (Anima::AnimaMappedValues::*AMV_SetVector5)(const Anima::AnimaString&, Anima::AFloat, Anima::AFloat, Anima::AFloat) = &Anima::AnimaMappedValues::SetVector;
void (Anima::AnimaMappedValues::*AMV_SetVector6)(const Anima::AnimaString&, Anima::AnimaVertex4f) = &Anima::AnimaMappedValues::SetVector;
void (Anima::AnimaMappedValues::*AMV_SetVector7)(const Anima::AnimaString&, Anima::AFloat, Anima::AFloat, Anima::AFloat, Anima::AFloat) = &Anima::AnimaMappedValues::SetVector;
void (Anima::AnimaMappedValues::*AMV_AddVectorArray1)(const Anima::AnimaString&, Anima::AnimaArray<Anima::AnimaVectorGenerator*>*) = &Anima::AnimaMappedValues::AddVectorArray;
void (Anima::AnimaMappedValues::*AMV_AddVectorArray2)(const Anima::AnimaString&, Anima::AnimaArray<Anima::AnimaVertex2f>*) = &Anima::AnimaMappedValues::AddVectorArray;
void (Anima::AnimaMappedValues::*AMV_AddVectorArray3)(const Anima::AnimaString&, Anima::AnimaArray<Anima::AnimaVertex3f>*) = &Anima::AnimaMappedValues::AddVectorArray;
void (Anima::AnimaMappedValues::*AMV_AddVectorArray4)(const Anima::AnimaString&, Anima::AnimaArray<Anima::AnimaVertex4f>*) = &Anima::AnimaMappedValues::AddVectorArray;
void (Anima::AnimaMappedValues::*AMV_SetVectorArray1)(const Anima::AnimaString&, Anima::AnimaArray<Anima::AnimaVectorGenerator*>*) = &Anima::AnimaMappedValues::SetVectorArray;
void (Anima::AnimaMappedValues::*AMV_SetVectorArray2)(const Anima::AnimaString&, Anima::AnimaArray<Anima::AnimaVertex2f>*) = &Anima::AnimaMappedValues::SetVectorArray;
void (Anima::AnimaMappedValues::*AMV_SetVectorArray3)(const Anima::AnimaString&, Anima::AnimaArray<Anima::AnimaVertex3f>*) = &Anima::AnimaMappedValues::SetVectorArray;
void (Anima::AnimaMappedValues::*AMV_SetVectorArray4)(const Anima::AnimaString&, Anima::AnimaArray<Anima::AnimaVertex4f>*) = &Anima::AnimaMappedValues::SetVectorArray;
void (Anima::AnimaMappedValues::*AMV_AddMatrix1)(const Anima::AnimaString&, const Anima::AnimaMatrix&) = &Anima::AnimaMappedValues::AddMatrix;
void (Anima::AnimaMappedValues::*AMV_AddMatrix2)(const Anima::AnimaString&, Anima::AFloat[16]) = &Anima::AnimaMappedValues::AddMatrix;
void (Anima::AnimaMappedValues::*AMV_SetMatrix1)(const Anima::AnimaString&, const Anima::AnimaMatrix&) = &Anima::AnimaMappedValues::SetMatrix;
void (Anima::AnimaMappedValues::*AMV_SetMatrix2)(const Anima::AnimaString&, Anima::AFloat[16]) = &Anima::AnimaMappedValues::SetMatrix;

void (Anima::AnimaDataGenerator::*ADG_SetColor1)(const Anima::AnimaColor3f& color) = &Anima::AnimaDataGenerator::SetColor;
void (Anima::AnimaDataGenerator::*ADG_SetColor2)(Anima::AFloat r, Anima::AFloat g, Anima::AFloat b) = &Anima::AnimaDataGenerator::SetColor;
void (Anima::AnimaDataGenerator::*ADG_SetColor3)(const Anima::AnimaColor4f& color) = &Anima::AnimaDataGenerator::SetColor;
void (Anima::AnimaDataGenerator::*ADG_SetColor4)(Anima::AFloat r, Anima::AFloat g, Anima::AFloat b, Anima::AFloat a) = &Anima::AnimaDataGenerator::SetColor;
void (Anima::AnimaDataGenerator::*ADG_SetVector1)(const Anima::AnimaVertex2f& color) = &Anima::AnimaDataGenerator::SetVector;
void (Anima::AnimaDataGenerator::*ADG_SetVector2)(Anima::AFloat x, Anima::AFloat y) = &Anima::AnimaDataGenerator::SetVector;
void (Anima::AnimaDataGenerator::*ADG_SetVector3)(const Anima::AnimaVertex3f& color) = &Anima::AnimaDataGenerator::SetVector;
void (Anima::AnimaDataGenerator::*ADG_SetVector4)(Anima::AFloat x, Anima::AFloat y, Anima::AFloat z) = &Anima::AnimaDataGenerator::SetVector;
void (Anima::AnimaDataGenerator::*ADG_SetVector5)(const Anima::AnimaVertex4f& color) = &Anima::AnimaDataGenerator::SetVector;
void (Anima::AnimaDataGenerator::*ADG_SetVector6)(Anima::AFloat x, Anima::AFloat y, Anima::AFloat z, Anima::AFloat w) = &Anima::AnimaDataGenerator::SetVector;

bool (Anima::AnimaTexture::*AT_SetData1)(Anima::AUchar*, Anima::AUint, Anima::AUint) = &Anima::AnimaTexture::SetData;
bool (Anima::AnimaTexture::*AT_SetData2)(Anima::AUchar*, Anima::AUint, Anima::AnimaTextureCubeIndex, Anima::AUint) = &Anima::AnimaTexture::SetData;
const Anima::AnimaArray<Anima::AUchar>* (Anima::AnimaTexture::*AT_GetDataAsArray1)(Anima::AUint) const = &Anima::AnimaTexture::GetDataAsArray;
const Anima::AnimaArray<Anima::AUchar>* (Anima::AnimaTexture::*AT_GetDataAsArray2)(Anima::AnimaTextureCubeIndex, Anima::AUint) const = &Anima::AnimaTexture::GetDataAsArray;
const Anima::AUchar* (Anima::AnimaTexture::*AT_GetData1)(Anima::AUint) const = &Anima::AnimaTexture::GetData;
const Anima::AUchar* (Anima::AnimaTexture::*AT_GetData2)(Anima::AnimaTextureCubeIndex, Anima::AUint) const = &Anima::AnimaTexture::GetData;
void (Anima::AnimaTexture::*AT_SetBorderColor1)(const Anima::AnimaColor4f&) = &Anima::AnimaTexture::SetBorderColor;
void (Anima::AnimaTexture::*AT_SetBorderColor2)(const Anima::AFloat&, const Anima::AFloat&, const Anima::AFloat&, const Anima::AFloat&) = &Anima::AnimaTexture::SetBorderColor;

void (Anima::AnimaSceneObject::*ASO_SetPosition1)(const Anima::AnimaVertex3f&) = &Anima::AnimaSceneObject::SetPosition;
void (Anima::AnimaSceneObject::*ASO_SetPosition2)(Anima::AFloat, Anima::AFloat, Anima::AFloat) = &Anima::AnimaSceneObject::SetPosition;
Anima::AnimaSceneObject* (Anima::AnimaSceneObject::*ASO_GetChild1)(const Anima::AnimaString& name) = &Anima::AnimaSceneObject::GetChild;
Anima::AnimaSceneObject* (Anima::AnimaSceneObject::*ASO_GetChild2)(Anima::AInt index) = &Anima::AnimaSceneObject::GetChild;
bool (Anima::AnimaSceneObject::*ASO_RemoveChild1)(Anima::AnimaSceneObject* object) = &Anima::AnimaSceneObject::RemoveChild;
bool (Anima::AnimaSceneObject::*ASO_RemoveChild2)(const Anima::AnimaString& name) = &Anima::AnimaSceneObject::RemoveChild;
bool (Anima::AnimaSceneObject::*ASO_RemoveChild3)(Anima::AInt index) = &Anima::AnimaSceneObject::RemoveChild;
Anima::AInt (Anima::AnimaSceneObject::*ASO_HasChild1)(Anima::AnimaSceneObject* object) = &Anima::AnimaSceneObject::HasChild;
Anima::AInt (Anima::AnimaSceneObject::*ASO_HasChild2)(const Anima::AnimaString& name) = &Anima::AnimaSceneObject::HasChild;

void (Anima::AnimaCamera::*AC_LookAt1)(const Anima::AnimaVertex3f&) = &Anima::AnimaCamera::LookAt;
void (Anima::AnimaCamera::*AC_LookAt2)(const Anima::AnimaVertex3f&, const Anima::AnimaVertex3f&, const Anima::AnimaVertex3f&) = &Anima::AnimaCamera::LookAt;
void (Anima::AnimaCamera::*AC_Move1)(Anima::AFloat, Anima::AFloat, Anima::AFloat) = &Anima::AnimaCamera::Move;
void (Anima::AnimaCamera::*AC_Move2)(const Anima::AnimaVertex3f&, const Anima::AnimaVertex3f&) = &Anima::AnimaCamera::Move;
void (Anima::AnimaCamera::*AC_SetCurrentVelocity1)(const Anima::AnimaVertex3f&) = &Anima::AnimaCamera::SetCurrentVelocity;
void (Anima::AnimaCamera::*AC_SetCurrentVelocity2)(Anima::AFloat, Anima::AFloat, Anima::AFloat) = &Anima::AnimaCamera::SetCurrentVelocity;
void (Anima::AnimaCamera::*AC_SetPosition1)(const Anima::AnimaVertex3f&) = &Anima::AnimaCamera::SetPosition;
void (Anima::AnimaCamera::*AC_SetPosition2)(Anima::AFloat, Anima::AFloat, Anima::AFloat) = &Anima::AnimaCamera::SetPosition;
void (Anima::AnimaCamera::*AC_SetMaximumVelocity1)(const Anima::AnimaVertex3f&) = &Anima::AnimaCamera::SetMaximumVelocity;
void (Anima::AnimaCamera::*AC_SetMaximumVelocity2)(Anima::AFloat, Anima::AFloat, Anima::AFloat) = &Anima::AnimaCamera::SetMaximumVelocity;

void (Anima::AnimaLight::*AL_SetDirection1)(const Anima::AnimaVertex3f&) = &Anima::AnimaLight::SetDirection;
void (Anima::AnimaLight::*AL_SetDirection2)(Anima::AFloat, Anima::AFloat, Anima::AFloat) = &Anima::AnimaLight::SetDirection;
void (Anima::AnimaLight::*AL_SetColor1)(const Anima::AnimaColor3f&) = &Anima::AnimaLight::SetColor;
void (Anima::AnimaLight::*AL_SetColor2)(Anima::AFloat, Anima::AFloat, Anima::AFloat) = &Anima::AnimaLight::SetColor;

void (Anima::AnimaTransformation::*AT_Translate1)(const Anima::AnimaVertex3f& t) = &Anima::AnimaTransformation::Translate;
void (Anima::AnimaTransformation::*AT_Translate2)(Anima::AFloat tx, Anima::AFloat ty, Anima::AFloat tz) = &Anima::AnimaTransformation::Translate;
void (Anima::AnimaTransformation::*AT_SetTranslation1)(const Anima::AnimaVertex3f& t) = &Anima::AnimaTransformation::SetTranslation;
void (Anima::AnimaTransformation::*AT_SetTranslation2)(Anima::AFloat tx, Anima::AFloat ty, Anima::AFloat tz) = &Anima::AnimaTransformation::SetTranslation;
void (Anima::AnimaTransformation::*AT_Rotate1)(const Anima::AnimaVertex3f& r) = &Anima::AnimaTransformation::Rotate;
void (Anima::AnimaTransformation::*AT_Rotate2)(Anima::AFloat rx, Anima::AFloat ry, Anima::AFloat rz) = &Anima::AnimaTransformation::Rotate;
void (Anima::AnimaTransformation::*AT_SetRotation1)(const Anima::AnimaVertex3f& r) = &Anima::AnimaTransformation::SetRotation;
void (Anima::AnimaTransformation::*AT_SetRotation2)(Anima::AFloat rx, Anima::AFloat ry, Anima::AFloat rz) = &Anima::AnimaTransformation::SetRotation;
void (Anima::AnimaTransformation::*AT_RotateDeg1)(const Anima::AnimaVertex3f& r) = &Anima::AnimaTransformation::RotateDeg;
void (Anima::AnimaTransformation::*AT_RotateDeg2)(Anima::AFloat rx, Anima::AFloat ry, Anima::AFloat rz) = &Anima::AnimaTransformation::RotateDeg;
void (Anima::AnimaTransformation::*AT_SetRotationDeg1)(const Anima::AnimaVertex3f& r) = &Anima::AnimaTransformation::SetRotationDeg;
void (Anima::AnimaTransformation::*AT_SetRotationDeg2)(Anima::AFloat rx, Anima::AFloat ry, Anima::AFloat rz) = &Anima::AnimaTransformation::SetRotationDeg;
void (Anima::AnimaTransformation::*AT_Scale1)(const Anima::AnimaVertex3f& s) = &Anima::AnimaTransformation::Scale;
void (Anima::AnimaTransformation::*AT_Scale2)(Anima::AFloat sx, Anima::AFloat sy, Anima::AFloat sz) = &Anima::AnimaTransformation::Scale;
void (Anima::AnimaTransformation::*AT_SetScale1)(const Anima::AnimaVertex3f& s) = &Anima::AnimaTransformation::SetScale;
void (Anima::AnimaTransformation::*AT_SetScale2)(Anima::AFloat sx, Anima::AFloat sy, Anima::AFloat sz) = &Anima::AnimaTransformation::SetScale;

void AnimaGC_SetUpdateFrameCallback_aux(Anima::AnimaGC& self, object userData, object callback) { self.SetUpdateFrameCallback(userData, callback); }

Anima::AnimaShader* (Anima::AnimaShadersManager::*LoadShaderStringString)(const Anima::AnimaString&, const Anima::AnimaString&, Anima::AnimaShaderType) = &Anima::AnimaShadersManager::LoadShader;
Anima::AnimaShader* (Anima::AnimaShadersManager::*LoadShaderFromFileStringString)(const Anima::AnimaString&, const Anima::AnimaString&, Anima::AnimaShaderType) = &Anima::AnimaShadersManager::LoadShaderFromFile;
Anima::AnimaShader* (Anima::AnimaShadersManager::*CreateShaderString)(const Anima::AnimaString&) = &Anima::AnimaShadersManager::CreateShader;
Anima::AnimaShaderProgram* (Anima::AnimaShadersManager::*CreateProgramString)(const Anima::AnimaString&) = &Anima::AnimaShadersManager::CreateProgram;
Anima::AnimaShaderProgram* (Anima::AnimaShadersManager::*GetProgramFromNameString)(const Anima::AnimaString&) = &Anima::AnimaShadersManager::GetProgramFromName;
void (Anima::AnimaShadersManager::*SetActiveProgramFromNameString)(const Anima::AnimaString&) = &Anima::AnimaShadersManager::SetActiveProgramFromName;

BOOST_PYTHON_MODULE(AnimaEngine)
{
	// Inizializzazione dei converters
	to_python_converter<Anima::AnimaString, AnimaString_to_python_str>();
	AnimaString_from_python_str();

	// AnimaEngine
	class_<Anima::AnimaEngine>("AnimaEngine")
	.def("Initialize", &Anima::AnimaEngine::Initialize)
	.def("Terminate", &Anima::AnimaEngine::Terminate)
	.def("GetDataGeneratorsManager", &Anima::AnimaEngine::GetDataGeneratorsManager, return_value_policy<reference_existing_object>())
	.def("GetParallelProgramsManager", &Anima::AnimaEngine::GetDataGeneratorsManager, return_value_policy<reference_existing_object>())
	.def("GetScenesManager", &Anima::AnimaEngine::GetScenesManager, return_value_policy<reference_existing_object>())
	.def("GetShadersManager", &Anima::AnimaEngine::GetShadersManager, return_value_policy<reference_existing_object>())
	.def("GetGenericAllocator", &Anima::AnimaEngine::GetGenericAllocator, return_value_policy<reference_existing_object>())
	;
	
	// AnimaScenesManager
	class_<Anima::AnimaScenesManager>("AnimaScenesManager", no_init)
	.def("CreateScene", &Anima::AnimaScenesManager::CreateScene, return_value_policy<reference_existing_object>())
	.def("GetActiveScene", &Anima::AnimaScenesManager::GetActiveScene, return_value_policy<reference_existing_object>())
	.def("GetScenesCount", &Anima::AnimaScenesManager::GetScenesCount)
	.def("GetScene", &Anima::AnimaScenesManager::GetScene, return_value_policy<reference_existing_object>())
	.def("GetSceneFromName", &Anima::AnimaScenesManager::GetSceneFromName, return_value_policy<reference_existing_object>())
	.def("LoadSceneFromFile", &Anima::AnimaScenesManager::LoadSceneFromFile, return_value_policy<reference_existing_object>())
	.def("LoadSceneFromXml", &Anima::AnimaScenesManager::LoadSceneFromXml, return_value_policy<reference_existing_object>())
	.def("LoadScenes", &Anima::AnimaScenesManager::LoadScenes)
	.def("SaveSceneToFile", SaveSceneToFileWithName)
	.def("SaveSceneToFile", SaveSceneToFileWithScene)
	.def("SaveScenes", &Anima::AnimaScenesManager::SaveScenes)
	;
	
	// AnimaNodesManager
	class_<Anima::AnimaNodesManager>("AnimaNodesManager", no_init)
	.def("CreateNode", &Anima::AnimaNodesManager::CreateNode, return_value_policy<reference_existing_object>())
	.def("GetNode", &Anima::AnimaNodesManager::GetNode, return_value_policy<reference_existing_object>())
	.def("GetNodeFromName", &Anima::AnimaNodesManager::GetNodeFromName, return_value_policy<reference_existing_object>())
	.def("GetNodesCount", &Anima::AnimaNodesManager::GetNodesCount)
	.def("GetAsset", &Anima::AnimaNodesManager::GetAsset, return_value_policy<reference_existing_object>())
	.def("GetAssetFromName", &Anima::AnimaNodesManager::GetAssetFromName, return_value_policy<reference_existing_object>())
	.def("GetAssetsCount", &Anima::AnimaNodesManager::GetAssetsCount)
	.def("LoadAssetFromExternalFile", &Anima::AnimaNodesManager::LoadAssetFromExternalFile, return_value_policy<reference_existing_object>())
	.def("LoadNodeFromFile", &Anima::AnimaNodesManager::LoadNodeFromFile, return_value_policy<reference_existing_object>())
	.def("LoadNodeFromXml", &Anima::AnimaNodesManager::LoadNodeFromXml, return_value_policy<reference_existing_object>())
	.def("LoadNodes", &Anima::AnimaNodesManager::LoadNodes)
	.def("SaveNodeToFile", SaveNodeToFileWithName)
	.def("SaveNodeToFile", SaveNodeToFileWithNode)
	.def("SaveNodes", &Anima::AnimaNodesManager::SaveNodes)
	;
	
	// AnimaNodeInstancesManager
	class_<Anima::AnimaNodeInstancesManager>("AnimaNodeInstancesManager", no_init)
	.def("CreateNodeInstance", CreateNodeInstanceWithName, return_value_policy<reference_existing_object>())
	.def("CreateNodeInstance", CreateNodeInstanceWithNode, return_value_policy<reference_existing_object>())
	.def("GetNodeInstance", &Anima::AnimaNodeInstancesManager::GetNodeInstance, return_value_policy<reference_existing_object>())
	.def("GetNodeInstanceFromName", &Anima::AnimaNodeInstancesManager::GetNodeInstanceFromName, return_value_policy<reference_existing_object>())
	.def("GetNodeInstancesCount", &Anima::AnimaNodeInstancesManager::GetNodeInstancesCount)
	.def("CreateAssetInstance", CreateAssetInstanceWithName, return_value_policy<reference_existing_object>())
	.def("CreateAssetInstance", CreateAssetInstanceWithAsset, return_value_policy<reference_existing_object>())
	.def("GetAssetInstance", &Anima::AnimaNodeInstancesManager::GetAssetInstance, return_value_policy<reference_existing_object>())
	.def("GetAssetInstanceFromName", &Anima::AnimaNodeInstancesManager::GetAssetInstanceFromName, return_value_policy<reference_existing_object>())
	.def("GetAssetInstancesCount", &Anima::AnimaNodeInstancesManager::GetAssetInstancesCount)
	.def("LoadNodeInstanceFromFile", &Anima::AnimaNodesManager::LoadNodeFromFile, return_value_policy<reference_existing_object>())
	.def("LoadNodeInstanceFromXml", &Anima::AnimaNodesManager::LoadNodeFromXml, return_value_policy<reference_existing_object>())
	.def("LoadNodesInstances", &Anima::AnimaNodesManager::LoadNodes)
	.def("SaveNodeInstanceToFile", SaveNodeInstanceToFileWithName)
	.def("SaveNodeInstanceToFile", SaveNodeInstanceToFileWithNode)
	.def("SaveNodesInstances", &Anima::AnimaNodeInstancesManager::SaveNodesInstances)
	;
	
	// AnimaGeometriesManager
	class_<Anima::AnimaGeometriesManager>("AnimaGeometriesManager", no_init)
	.def("CreateGeometry", &Anima::AnimaGeometriesManager::CreateGeometry, return_value_policy<reference_existing_object>())
	.def("GetGeometry", &Anima::AnimaGeometriesManager::GetGeometry, return_value_policy<reference_existing_object>())
	.def("GetGeometryFromName", &Anima::AnimaGeometriesManager::GetGeometryFromName, return_value_policy<reference_existing_object>())
	.def("GetGeometriesCount", &Anima::AnimaGeometriesManager::GetGeometriesCount)
	.def("LoadGeometryFromFile", &Anima::AnimaGeometriesManager::LoadGeometryFromFile, return_value_policy<reference_existing_object>())
	.def("LoadGeometryFromXml", &Anima::AnimaGeometriesManager::LoadGeometryFromXml, return_value_policy<reference_existing_object>())
	.def("LoadGeometries", &Anima::AnimaGeometriesManager::LoadGeometries)
	.def("SaveGeometryToFile", SaveGeometryToFileWithName)
	.def("SaveGeometryToFile", SaveGeometryToFileWithGeometry)
	.def("SaveGeometries", &Anima::AnimaGeometriesManager::SaveGeometries)
	;
	
	// AnimaGeometryInstancesManager
	class_<Anima::AnimaGeometryInstancesManager>("AnimaGeometryInstancesManager", no_init)
	.def("CreateGeometryInstance", CreateGeometryInstanceWithName, return_value_policy<reference_existing_object>())
	.def("CreateGeometryInstance", CreateGeometryInstanceWithGeometry, return_value_policy<reference_existing_object>())
	.def("CreateInstancesFromNode", &Anima::AnimaGeometryInstancesManager::CreateInstancesFromNode, return_value_policy<reference_existing_object>())
	.def("GetGeometryInstance", &Anima::AnimaGeometryInstancesManager::GetGeometryInstance, return_value_policy<reference_existing_object>())
	.def("GetGeometryInstanceFromName", &Anima::AnimaGeometryInstancesManager::GetGeometryInstanceFromName, return_value_policy<reference_existing_object>())
	.def("GetGeometryInstancesCount", &Anima::AnimaGeometryInstancesManager::GetGeometryInstancesCount)
	.def("LoadGeometryInstanceFromFile", &Anima::AnimaGeometryInstancesManager::LoadGeometryInstanceFromFile, return_value_policy<reference_existing_object>())
	.def("LoadGeometryInstanceFromXml", &Anima::AnimaGeometryInstancesManager::LoadGeometryInstanceFromXml, return_value_policy<reference_existing_object>())
	.def("LoadGeometriesInstances", &Anima::AnimaGeometryInstancesManager::LoadGeometriesInstances)
	.def("SaveGeometryInstanceToFile", SaveGeometryInstanceToFileWithName)
	.def("SaveGeometryInstanceToFile", SaveGeometryInstanceToFileWithGeometry)
	.def("SaveGeometriesInstances", &Anima::AnimaGeometryInstancesManager::SaveGeometriesInstances)
	;
	
	// AnimaCamerasManager
	class_<Anima::AnimaCamerasManager>("AnimaCamerasManager", no_init)
	.def("CreateCamera", &Anima::AnimaCamerasManager::CreateCamera, return_value_policy<reference_existing_object>())
	.def("GetCamera", &Anima::AnimaCamerasManager::GetCamera, return_value_policy<reference_existing_object>())
	.def("GetCameraFromName", &Anima::AnimaCamerasManager::GetCameraFromName, return_value_policy<reference_existing_object>())
	.def("GetActiveCamera", &Anima::AnimaCamerasManager::GetActiveCamera, return_value_policy<reference_existing_object>())
	.def("GetCamerasCount", &Anima::AnimaCamerasManager::GetCamerasCount)
	.def("LoadCameraFromFile", &Anima::AnimaCamerasManager::LoadCameraFromFile, return_value_policy<reference_existing_object>())
	.def("LoadCameraFromXml", &Anima::AnimaCamerasManager::LoadCameraFromXml, return_value_policy<reference_existing_object>())
	.def("LoadCameras", &Anima::AnimaCamerasManager::LoadCameras)
	.def("SaveCameraToFile", SaveCameraToFileWithName)
	.def("SaveCameraToFile", SaveCameraToFileWithCamera)
	.def("SaveCameras", &Anima::AnimaCamerasManager::SaveCameras)
	.def("UpdateCameras", &Anima::AnimaCamerasManager::UpdateCameras)
	.def("UpdateOrthographicCameras", &Anima::AnimaCamerasManager::UpdateOrthographicCameras)
	.def("UpdatePerspectiveCameras", &Anima::AnimaCamerasManager::UpdatePerspectiveCameras)
	;
	
	// AnimaLightsManager
	class_<Anima::AnimaLightsManager>("AnimaLightsManager", no_init)
	.def("CreateDirectionalLight", &Anima::AnimaLightsManager::CreateDirectionalLight, return_value_policy<reference_existing_object>())
	.def("CreatePointLight", &Anima::AnimaLightsManager::CreatePointLight, return_value_policy<reference_existing_object>())
	.def("CreateSpotLight", &Anima::AnimaLightsManager::CreateSpotLight, return_value_policy<reference_existing_object>())
	.def("CreateHemisphereLight", &Anima::AnimaLightsManager::CreateHemisphereLight, return_value_policy<reference_existing_object>())
	.def("GetLightFromName", &Anima::AnimaLightsManager::GetLightFromName, return_value_policy<reference_existing_object>())
	.def("GetLightsCount", &Anima::AnimaLightsManager::GetTotalLightsCount)
	.def("LoadLightFromFile", &Anima::AnimaLightsManager::LoadLightFromFile, return_value_policy<reference_existing_object>())
	.def("LoadLightFromXml", &Anima::AnimaLightsManager::LoadLightFromXml, return_value_policy<reference_existing_object>())
	.def("LoadLights", &Anima::AnimaLightsManager::LoadLights)
	.def("SaveLightToFile", SaveLightToFileWithName)
	.def("SaveLightToFile", SaveLightToFileWithLight)
	.def("SaveLights", &Anima::AnimaLightsManager::SaveLights)
	.def("UpdateLightsMatrix", &Anima::AnimaLightsManager::UpdateLightsMatrix)
	;

	// AnimaMaterialsManager
	class_<Anima::AnimaMaterialsManager>("AnimaMaterialsManager", no_init)
	.def("CreateMaterial", &Anima::AnimaMaterialsManager::CreateMaterial, return_value_policy<reference_existing_object>())
	.def("GetDefaultMaterial", &Anima::AnimaMaterialsManager::GetDefaultMaterial, return_value_policy<reference_existing_object>())
	.def("GetMaterial", &Anima::AnimaMaterialsManager::GetMaterial, return_value_policy<reference_existing_object>())
	.def("GetMaterialFromName", &Anima::AnimaMaterialsManager::GetMaterialFromName, return_value_policy<reference_existing_object>())
	.def("GetMaterials", &Anima::AnimaMaterialsManager::GetMaterials, return_value_policy<reference_existing_object>())
	.def("GetMaterialsCount", &Anima::AnimaMaterialsManager::GetMaterialsCount)
	.def("LoadMaterialFromFile", &Anima::AnimaMaterialsManager::LoadMaterialFromFile, return_value_policy<reference_existing_object>())
	.def("LoadMaterialFromXml", &Anima::AnimaMaterialsManager::LoadMaterialFromXml, return_value_policy<reference_existing_object>())
	.def("LoadMaterials", &Anima::AnimaMaterialsManager::LoadMaterials)
	.def("SaveMaterialToFile", SaveMaterialToFileWithName)
	.def("SaveMaterialToFile", SaveMaterialToFileWithMaterial)
	.def("SaveMaterials", &Anima::AnimaMaterialsManager::SaveMaterials)
	;

	// AnimaTexturesManager
	class_<Anima::AnimaTexturesManager>("AnimaTexturesManager", no_init)
	.def("CreateTexture", &Anima::AnimaMaterialsManager::CreateMaterial, return_value_policy<reference_existing_object>())
	.def("CreateTexture", &Anima::AnimaMaterialsManager::CreateMaterial, return_value_policy<reference_existing_object>())
	.def("CreateTexture", &Anima::AnimaMaterialsManager::CreateMaterial, return_value_policy<reference_existing_object>())
	.def("CreateTexture", &Anima::AnimaMaterialsManager::CreateMaterial, return_value_policy<reference_existing_object>())
	.def("CreateTexture", &Anima::AnimaMaterialsManager::CreateMaterial, return_value_policy<reference_existing_object>())
	.def("GetTexture", &Anima::AnimaTexturesManager::GetTexture, return_value_policy<reference_existing_object>())
	.def("GetTextureFromName", &Anima::AnimaTexturesManager::GetTextureFromName, return_value_policy<reference_existing_object>())
	.def("GetTexturesCount", &Anima::AnimaTexturesManager::GetTexturesCount)
	.def("LoadTextureFromBMPFile", &Anima::AnimaTexturesManager::LoadTextureFromBMPFile, return_value_policy<reference_existing_object>())
	.def("LoadTextureFromDDSFile", &Anima::AnimaTexturesManager::LoadTextureFromDDSFile, return_value_policy<reference_existing_object>())
	.def("LoadTextureFromTGAFile", &Anima::AnimaTexturesManager::LoadTextureFromTGAFile, return_value_policy<reference_existing_object>())
	.def("LoadTextureFromFile", LoadTextureFromAnimaTextureFile, return_value_policy<reference_existing_object>())
	.def("LoadTextureFromFile", LoadTextureFromImageFile, return_value_policy<reference_existing_object>())
	.def("LoadTextures", &Anima::AnimaTexturesManager::LoadTextures)
	.def("SaveTextures", &Anima::AnimaTexturesManager::SaveTextures)
	.def("SaveTextureToFile", SaveTextureToFileWithName)
	.def("SaveTextureToFile", SaveTextureToFileWithTexture)
	;

	// AnimaDataGeneratorsManager
	class_<Anima::AnimaDataGeneratorsManager>("AnimaDataGeneratorsManager", no_init)
	.def("CreateColorGenerator", &Anima::AnimaDataGeneratorsManager::CreateColorGenerator, return_value_policy<reference_existing_object>())
	.def("CreateTextureGenerator", &Anima::AnimaDataGeneratorsManager::CreateTextureGenerator, return_value_policy<reference_existing_object>())
	.def("CreateVectorGenerator", &Anima::AnimaDataGeneratorsManager::CreateVectorGenerator, return_value_policy<reference_existing_object>())
	.def("GetGenerator", &Anima::AnimaDataGeneratorsManager::GetGenerator, return_value_policy<reference_existing_object>())
	.def("RemoveGenerator", RemoveGeneratorWithName)
	.def("RemoveGenerator", RemoveGeneratorWithGenerator)
	;

	// AnimaNamedObject
	class_<Anima::AnimaNamedObject>("AnimaNamedObject", init<const Anima::AnimaString&, Anima::AnimaAllocator*>())
	.def("GetName", &Anima::AnimaNamedObject::GetName)
	.def("SetName", &Anima::AnimaNamedObject::SetName)
	.def("SaveObject", &Anima::AnimaNamedObject::SaveObject)
	;

	// AnimaMappedValues
	class_<Anima::AnimaMappedValues, boost::noncopyable, bases<Anima::AnimaNamedObject> >("AnimaMappedValues", no_init)
	.def("AddTexture", AMV_AddTexture1)
	.def("AddTexture", AMV_AddTexture2)
	.def("SetTexture", AMV_SetTexture1)
	.def("SetTexture", AMV_SetTexture2)
	.def("GetTexture", &Anima::AnimaMappedValues::GetTexture, return_value_policy<reference_existing_object>())
	.def("HasTexture", &Anima::AnimaMappedValues::HasTexture)
	.def("GetTextures", &Anima::AnimaMappedValues::GetTextures, return_value_policy<reference_existing_object>())
	.def("AddColor", AMV_AddColor1)
	.def("AddColor", AMV_AddColor2)
	.def("AddColor", AMV_AddColor3)
	.def("AddColor", AMV_AddColor4)
	.def("AddColor", AMV_AddColor5)
	.def("SetColor", AMV_SetColor1)
	.def("SetColor", AMV_SetColor2)
	.def("SetColor", AMV_SetColor3)
	.def("SetColor", AMV_SetColor4)
	.def("SetColor", AMV_SetColor5)
	.def("GetColor3f", &Anima::AnimaMappedValues::GetColor3f)
	.def("GetColor4f", &Anima::AnimaMappedValues::GetColor4f)
	.def("HasColor", &Anima::AnimaMappedValues::HasColor)
	.def("GetColors", &Anima::AnimaMappedValues::GetColors, return_value_policy<reference_existing_object>())
	.def("AddVector", AMV_AddVector1)
	.def("AddVector", AMV_AddVector2)
	.def("AddVector", AMV_AddVector3)
	.def("AddVector", AMV_AddVector4)
	.def("AddVector", AMV_AddVector5)
	.def("AddVector", AMV_AddVector6)
	.def("AddVector", AMV_AddVector7)
	.def("SetVector", AMV_SetVector1)
	.def("SetVector", AMV_SetVector2)
	.def("SetVector", AMV_SetVector3)
	.def("SetVector", AMV_SetVector4)
	.def("SetVector", AMV_SetVector5)
	.def("SetVector", AMV_SetVector6)
	.def("SetVector", AMV_SetVector7)
	.def("GetVector2f", &Anima::AnimaMappedValues::GetVector2f)
	.def("GetVector3f", &Anima::AnimaMappedValues::GetVector3f)
	.def("GetVector4f", &Anima::AnimaMappedValues::GetVector4f)
	.def("HasVector", &Anima::AnimaMappedValues::HasVector)
	.def("GetVectors", &Anima::AnimaMappedValues::GetVectors, return_value_policy<reference_existing_object>())
	.def("AddVectorArray", AMV_AddVectorArray1)
	.def("AddVectorArray", AMV_AddVectorArray2)
	.def("AddVectorArray", AMV_AddVectorArray3)
	.def("AddVectorArray", AMV_AddVectorArray4)
	.def("SetVectorArray", AMV_SetVectorArray1)
	.def("SetVectorArray", AMV_SetVectorArray2)
	.def("SetVectorArray", AMV_SetVectorArray3)
	.def("SetVectorArray", AMV_SetVectorArray4)
	.def("GetVectorArray", &Anima::AnimaMappedValues::GetVectorArray, return_value_policy<reference_existing_object>())
	.def("HasVectorArray", &Anima::AnimaMappedValues::HasVectorArray)
	.def("GetVectorArrays", &Anima::AnimaMappedValues::GetVectorArrays, return_value_policy<reference_existing_object>())
	.def("AddFloat", &Anima::AnimaMappedValues::AddFloat)
	.def("SetFloat", &Anima::AnimaMappedValues::SetFloat)
	.def("GetFloat", &Anima::AnimaMappedValues::GetFloat)
	.def("HasFloat", &Anima::AnimaMappedValues::HasFloat)
	.def("GetFloats", &Anima::AnimaMappedValues::GetFloats, return_value_policy<reference_existing_object>())
	.def("AddBoolean", &Anima::AnimaMappedValues::AddBoolean)
	.def("SetBoolean", &Anima::AnimaMappedValues::SetBoolean)
	.def("GetBoolean", &Anima::AnimaMappedValues::GetBoolean)
	.def("HasBoolean", &Anima::AnimaMappedValues::HasBoolean)
	.def("GetBooleans", &Anima::AnimaMappedValues::GetBooleans, return_value_policy<reference_existing_object>())
	.def("AddInteger", &Anima::AnimaMappedValues::AddInteger)
	.def("SetInteger", &Anima::AnimaMappedValues::SetInteger)
	.def("GetInteger", &Anima::AnimaMappedValues::GetInteger)
	.def("HasInteger", &Anima::AnimaMappedValues::HasInteger)
	.def("GetIntegers", &Anima::AnimaMappedValues::GetIntegers, return_value_policy<reference_existing_object>())
	.def("AddMatrix", AMV_AddMatrix1)
	.def("AddMatrix", AMV_AddMatrix2)
	.def("SetMatrix", AMV_SetMatrix1)
	.def("SetMatrix", AMV_SetMatrix2)
	.def("GetMatrix", &Anima::AnimaMappedValues::GetMatrix)
	.def("HasMatrix", &Anima::AnimaMappedValues::HasMatrix)
	.def("GetMatrices", &Anima::AnimaMappedValues::GetMatrices, return_value_policy<reference_existing_object>())
	.def("AddMatrixArray", &Anima::AnimaMappedValues::AddMatrixArray)
	.def("SetMatrixArray", &Anima::AnimaMappedValues::SetMatrixArray)
	.def("GetMatrixArray", &Anima::AnimaMappedValues::GetMatrixArray, return_value_policy<reference_existing_object>())
	.def("HasMatrixArray", &Anima::AnimaMappedValues::HasMatrixArray)
	.def("GetMatrixArrays", &Anima::AnimaMappedValues::GetMatrixArrays, return_value_policy<reference_existing_object>())
	.def("ExtractName", &Anima::AnimaMappedValues::ExtractName)
	.staticmethod("ExtractName")
	;
	
	class_<Anima::AnimaArray<Anima::AnimaVectorGenerator*> >("AnimaVectorGeneratorArray")
	.def(vector_indexing_suite<Anima::AnimaArray<Anima::AnimaVectorGenerator*> >())
	;
	
	class_<Anima::AnimaArray<Anima::AnimaMatrix> >("AnimaMatrixArray")
	.def(vector_indexing_suite<Anima::AnimaArray<Anima::AnimaMatrix> >())
	;
	
	class_<boost::unordered_map<Anima::AnimaString, Anima::AnimaTextureGenerator*, Anima::AnimaStringHasher> >("AnimaTextureGeneratorMap")
	.def(pointer_map_indexing_suite<boost::unordered_map<Anima::AnimaString, Anima::AnimaTextureGenerator*, Anima::AnimaStringHasher>, false>())
	;
	
	class_<boost::unordered_map<Anima::AnimaString, Anima::AnimaColorGenerator*, Anima::AnimaStringHasher> >("AnimaColorGeneratorMap")
	.def(pointer_map_indexing_suite<boost::unordered_map<Anima::AnimaString, Anima::AnimaColorGenerator*, Anima::AnimaStringHasher>, false>())
	;
	
	class_<boost::unordered_map<Anima::AnimaString, Anima::AnimaVectorGenerator*, Anima::AnimaStringHasher> >("AnimaVectorGeneratorMap")
	.def(pointer_map_indexing_suite<boost::unordered_map<Anima::AnimaString, Anima::AnimaVectorGenerator*, Anima::AnimaStringHasher>, false>())
	;
	
	class_<boost::unordered_map<Anima::AnimaString, Anima::AFloat, Anima::AnimaStringHasher> >("AFloatMap")
	.def(map_indexing_suite<boost::unordered_map<Anima::AnimaString, Anima::AFloat, Anima::AnimaStringHasher>, false>())
	;
	
	class_<boost::unordered_map<Anima::AnimaString, Anima::AInt, Anima::AnimaStringHasher> >("AIntMap")
	.def(map_indexing_suite<boost::unordered_map<Anima::AnimaString, Anima::AInt, Anima::AnimaStringHasher>, false>())
	;
	
	class_<boost::unordered_map<Anima::AnimaString, bool, Anima::AnimaStringHasher> >("BoolMap")
	.def(map_indexing_suite<boost::unordered_map<Anima::AnimaString, bool, Anima::AnimaStringHasher>, false>())
	;
	
	class_<boost::unordered_map<Anima::AnimaString, Anima::AnimaArray<Anima::AnimaVectorGenerator*>*, Anima::AnimaStringHasher> >("AnimaVectorGeneratorArrayMap")
	.def(pointer_map_indexing_suite<boost::unordered_map<Anima::AnimaString, Anima::AnimaArray<Anima::AnimaVectorGenerator*>*, Anima::AnimaStringHasher>, false>())
	;
	
	class_<boost::unordered_map<Anima::AnimaString, Anima::AnimaArray<Anima::AnimaMatrix>*, Anima::AnimaStringHasher> >("AnimaMatrixArrayMap")
	.def(pointer_map_indexing_suite<boost::unordered_map<Anima::AnimaString, Anima::AnimaArray<Anima::AnimaMatrix>*, Anima::AnimaStringHasher>, false>())
	;
	
	// AnimaDataGenerator
	class_<Anima::AnimaDataGenerator, boost::noncopyable>("AnimaDataGenerator", no_init)
//	.def("UpdateValue", pure_virtual(&Anima::AnimaDataGenerator::UpdateValue))
	.def("SetColor", ADG_SetColor1)
	.def("SetColor", ADG_SetColor2)
	.def("SetColor", ADG_SetColor3)
	.def("SetColor", ADG_SetColor4)
	.def("GetColor3f", &Anima::AnimaDataGenerator::GetColor3f)
	.def("GetColor4f", &Anima::AnimaDataGenerator::GetColor4f)
	.def("SetVector", ADG_SetVector1)
	.def("SetVector", ADG_SetVector2)
	.def("SetVector", ADG_SetVector3)
	.def("SetVector", ADG_SetVector4)
	.def("SetVector", ADG_SetVector5)
	.def("SetVector", ADG_SetVector6)
	.def("GetVector2f", &Anima::AnimaDataGenerator::GetVector2f)
	.def("GetVector3f", &Anima::AnimaDataGenerator::GetVector3f)
	.def("GetVector4f", &Anima::AnimaDataGenerator::GetVector4f)
	.def("SetTexture", &Anima::AnimaDataGenerator::SetTexture)
	.def("GetTexture", &Anima::AnimaDataGenerator::GetTexture, return_value_policy<reference_existing_object>())
	.def("StopValueUpdate", &Anima::AnimaDataGenerator::StopValueUpdate)
	.def("StartValueUpdate", &Anima::AnimaDataGenerator::StartValueUpdate)
	.def("SetCanUpdateValue", &Anima::AnimaDataGenerator::SetCanUpdateValue)
	.def("CanUpdateValue", &Anima::AnimaDataGenerator::CanUpdateValue)
	;
	
	// AnimaColorGenerator
	class_<Anima::AnimaColorGenerator, bases<Anima::AnimaDataGenerator> >("AnimaColorGenerator", no_init)
	.def("UpdateValue", &Anima::AnimaColorGenerator::UpdateValue)
	;
	
	// AnimaTextureGenerator
	class_<Anima::AnimaTextureGenerator, bases<Anima::AnimaDataGenerator> >("AnimaTextureGenerator", no_init)
	.def("UpdateValue", &Anima::AnimaTextureGenerator::UpdateValue)
	;
	
	// AnimaVectorGenerator
	class_<Anima::AnimaVectorGenerator, bases<Anima::AnimaDataGenerator> >("AnimaVectorGenerator", no_init)
	.def("UpdateValue", &Anima::AnimaVectorGenerator::UpdateValue)
	;
	
	// AnimaTexture
	class_<Anima::AnimaTexture, bases<Anima::AnimaNamedObject> >("AnimaTexture", no_init)
	.def(init<Anima::AnimaAllocator*, const Anima::AnimaString&>())
	.def(init<Anima::AnimaAllocator*, const Anima::AnimaString&, Anima::AUint, Anima::AUint, const Anima::AnimaString&>())
	.def(init<Anima::AnimaAllocator*, const Anima::AnimaString&, Anima::AUint, Anima::AUint, Anima::AUint, const Anima::AnimaString&>())
	.def(init<Anima::AnimaAllocator*, const Anima::AnimaString&, Anima::AUint, Anima::AUint, Anima::AUchar*, Anima::AUint, const Anima::AnimaString&>())
	.def(init<Anima::AnimaAllocator*, const Anima::AnimaString&, Anima::AUint, Anima::AUint, Anima::AUint, Anima::AUchar*, Anima::AUint, const Anima::AnimaString&>())
	.def("GetID", &Anima::AnimaTexture::GetID)
	.def("SetWidth", &Anima::AnimaTexture::SetWidth)
	.def("GetWidth", &Anima::AnimaTexture::GetWidth)
	.def("SetHeight", &Anima::AnimaTexture::SetHeight)
	.def("GetHeight", &Anima::AnimaTexture::GetHeight)
	.def("SetDepth", &Anima::AnimaTexture::SetDepth)
	.def("GetDepth", &Anima::AnimaTexture::GetDepth)
	.def("SetMipMapLevels", &Anima::AnimaTexture::SetMipMapLevels)
	.def("GetMipMapLevels", &Anima::AnimaTexture::GetMipMapLevels)
	.def("SetGenerateMipMap", &Anima::AnimaTexture::SetGenerateMipMap)
	.def("IsGenerateMipMap", &Anima::AnimaTexture::IsGenerateMipMap)
	.def("SetFormat", &Anima::AnimaTexture::SetFormat)
	.def("GetFormat", &Anima::AnimaTexture::GetFormat)
	.def("SetMinFilter", &Anima::AnimaTexture::SetMinFilter)
	.def("GetMinFilter", &Anima::AnimaTexture::GetMinFilter)
	.def("SetMagFilter", &Anima::AnimaTexture::SetMagFilter)
	.def("GetMagFilter", &Anima::AnimaTexture::GetMagFilter)
	.def("SetAttachment", &Anima::AnimaTexture::SetAttachment)
	.def("GetAttachment", &Anima::AnimaTexture::GetAttachment)
	.def("SetInternalFormat", &Anima::AnimaTexture::SetInternalFormat)
	.def("GetInternalFormat", &Anima::AnimaTexture::GetInternalFormat)
	.def("SetDataType", &Anima::AnimaTexture::SetDataType)
	.def("GetDataType", &Anima::AnimaTexture::GetDataType)
	.def("SetSourceFileName", &Anima::AnimaTexture::SetSourceFileName)
	.def("GetSourceFileName", &Anima::AnimaTexture::GetSourceFileName)
	.def("SetData", AT_SetData1)
	.def("SetData", AT_SetData2)
//	.def("GetData", AT_GetData1, return_value_policy<reference_existing_object>())
//	.def("GetDataAsArray", AT_GetDataAsArray1, return_value_policy<reference_existing_object>())
//	.def("GetData", AT_GetData2, return_value_policy<reference_existing_object>())
//	.def("GetDataAsArray", AT_GetDataAsArray2, return_value_policy<reference_existing_object>())
	.def("SetTextureTarget", &Anima::AnimaTexture::SetTextureTarget)
	.def("GetTextureTarget", &Anima::AnimaTexture::GetTextureTarget)
	.def("SetClampS", &Anima::AnimaTexture::SetClampS)
	.def("GetClampS", &Anima::AnimaTexture::GetClampS)
	.def("SetClampT", &Anima::AnimaTexture::SetClampT)
	.def("GetClampT", &Anima::AnimaTexture::GetClampT)
	.def("SetClampR", &Anima::AnimaTexture::SetClampR)
	.def("GetClampR", &Anima::AnimaTexture::GetClampR)
	.def("SetBorderColor", AT_SetBorderColor1)
	.def("SetBorderColor", AT_SetBorderColor2)
	.def("GetBorderColor", &Anima::AnimaTexture::GetBorderColor)
	.def("Load", &Anima::AnimaTexture::Load)
	.def("Unload", &Anima::AnimaTexture::Unload)
	.def("LoadRenderTargets", &Anima::AnimaTexture::LoadRenderTargets)
	.def("IsReady", &Anima::AnimaTexture::IsReady)
	.def("AreRenderTargetsReady", &Anima::AnimaTexture::AreRenderTargetsReady)
	.def("Resize", &Anima::AnimaTexture::Resize)
	.def("Bind", &Anima::AnimaTexture::Bind)
	.def("BindAsRenderTarget", &Anima::AnimaTexture::BindAsRenderTarget)
	;
		
	// AnimaScene
	class_<Anima::AnimaScene, bases<Anima::AnimaMappedValues> >("AnimaScene", no_init)
	.def("StartScene", &Anima::AnimaScene::StartScene)
	.def("StepScene", &Anima::AnimaScene::StepScene)
	.def("InitializePhysics", &Anima::AnimaScene::InitializePhysics)
	.def("TerminatePhysics", &Anima::AnimaScene::TerminatePhysics)
	.def("IsActive", &Anima::AnimaScene::IsActive)
	.def("GetEngine", &Anima::AnimaScene::GetEngine, return_value_policy<reference_existing_object>())
	.def("GetNodesManager", &Anima::AnimaScene::GetNodesManager, return_value_policy<reference_existing_object>())
	.def("GetGeometriesManager", &Anima::AnimaScene::GetGeometriesManager, return_value_policy<reference_existing_object>())
	.def("GetNodeInstancesManager", &Anima::AnimaScene::GetNodeInstancesManager, return_value_policy<reference_existing_object>())
	.def("GetGeometryInstancesManager", &Anima::AnimaScene::GetGeometryInstancesManager, return_value_policy<reference_existing_object>())
//	.def("GetShadersManager", &Anima::AnimaScene::GetShadersManager, return_value_policy<reference_existing_object>())
	.def("GetCamerasManager", &Anima::AnimaScene::GetCamerasManager, return_value_policy<reference_existing_object>())
	.def("GetTexturesManager", &Anima::AnimaScene::GetTexturesManager, return_value_policy<reference_existing_object>())
	.def("GetDataGeneratorsManger", &Anima::AnimaScene::GetDataGeneratorsManager, return_value_policy<reference_existing_object>())
	.def("GetMaterialsManager", &Anima::AnimaScene::GetMaterialsManager, return_value_policy<reference_existing_object>())
	.def("GetLightsManager", &Anima::AnimaScene::GetLightsManager, return_value_policy<reference_existing_object>())
	.def("GetAnimationsManager", &Anima::AnimaScene::GetAnimationsManager, return_value_policy<reference_existing_object>())
//	.def("GetParallelProgramsManager", &Anima::AnimaScene::GetParallelProgramsManager, return_value_policy<reference_existing_object>())
	.def("SetMouseInteractor", &Anima::AnimaScene::SetMouseInteractor)
	.def("GetMouseInteractor", &Anima::AnimaScene::GetMouseInteractor, return_value_policy<reference_existing_object>())
	.def("SetKeyboardInteractor", &Anima::AnimaScene::SetKeyboardInteractor)
	.def("GetKeyboardInteractor", &Anima::AnimaScene::GetKeyboardInteractor, return_value_policy<reference_existing_object>())
	.def("SetJoystickInteractor", &Anima::AnimaScene::SetJoystickInteractor)
	.def("GetJoystickInteractor", &Anima::AnimaScene::GetJoystickInteractor, return_value_policy<reference_existing_object>())
	.def("IntializePhysicsObjects", &Anima::AnimaScene::InitializePhysicObjects)
	.def("IsRunning", &Anima::AnimaScene::IsRunning)
	.def("Activate", &Anima::AnimaScene::Activate)
	.def("Deactivate", &Anima::AnimaScene::Deactivate)
	;
	
	// AnimaMaterial
	class_<Anima::AnimaMaterial, bases<Anima::AnimaMappedValues> >("AnimaMaterial", no_init);
	
	// AnimaSceneObject
	class_<Anima::AnimaSceneObject, bases<Anima::AnimaMappedValues> >("AnimaSceneObject", no_init)
	.def("GetPosition", &Anima::AnimaSceneObject::GetPosition)
	.def("SetPosition", ASO_SetPosition1)
	.def("SetPosition", ASO_SetPosition2)
	.def("SetParentObject", &Anima::AnimaSceneObject::SetParentObject)
	.def("GetParentObject", &Anima::AnimaSceneObject::GetParentObject, return_value_policy<reference_existing_object>())
	.def("GetAncestorObjectr", &Anima::AnimaSceneObject::GetAncestorObject, return_value_policy<reference_existing_object>())
	.def("AddChild", &Anima::AnimaSceneObject::AddChild)
	.def("GetChildrenCount", &Anima::AnimaSceneObject::GetChildrenCount)
	.def("GetChild", ASO_GetChild1, return_value_policy<reference_existing_object>())
	.def("GetChild", ASO_GetChild2, return_value_policy<reference_existing_object>())
	.def("RemoveChild", ASO_RemoveChild1)
	.def("RemoveChild", ASO_RemoveChild2)
	.def("RemoveChild", ASO_RemoveChild3)
	.def("HasChild", ASO_HasChild1)
	.def("HasChild", ASO_HasChild2)
	.def("GetTransformation", &Anima::AnimaSceneObject::GetTransformation, return_value_policy<reference_existing_object>())
	.def("GetTransformationCopy", &Anima::AnimaSceneObject::GetTransformationCopy)
	;
	
	// AnimaCamera
	class_<Anima::AnimaCamera, bases<Anima::AnimaSceneObject> >("AnimaCamera", no_init)
	.def("LookAt", AC_LookAt1)
	.def("LookAt", AC_LookAt2)
	.def("Move", AC_Move1)
	.def("Move", AC_Move2)
	.def("RotateRad", &Anima::AnimaCamera::RotateRad)
	.def("RotateDeg", &Anima::AnimaCamera::RotateDeg)
	.def("SmoothRotateRad", &Anima::AnimaCamera::SmoothRotateRad)
	.def("SmoothRotareDeg", &Anima::AnimaCamera::SmoothRotateDeg)
	.def("UndoRoll", &Anima::AnimaCamera::UndoRoll)
	.def("UpdatePosition", &Anima::AnimaCamera::UpdatePosition)
	.def("Zoom", &Anima::AnimaCamera::Zoom)
	.def("SetUpdateData", &Anima::AnimaCamera::SetUpdateData)
	.def("SetAcceleration", &Anima::AnimaCamera::SetAcceleration)
	.def("GetAcceleration", &Anima::AnimaCamera::GetAcceleration, return_value_policy<reference_existing_object>())
	.def("SetCameraType", &Anima::AnimaCamera::SetCameraType)
	.def("GetCameraType", &Anima::AnimaCamera::GetCameraType)
	.def("SetCurrentVelocity", AC_SetCurrentVelocity1)
	.def("SetCurrentVelocity", AC_SetCurrentVelocity2)
	.def("GetCurrentVelocity", &Anima::AnimaCamera::GetCurrentVelocity, return_value_policy<reference_existing_object>())
	.def("SetPosition", AC_SetPosition1)
	.def("SetPosition", AC_SetPosition2)
	.def("SetOrbitMinZoom", &Anima::AnimaCamera::SetOrbitMinZoom)
	.def("GetOrbitMinZoom", &Anima::AnimaCamera::GetOrbitMinZoom)
	.def("SetOrbitMaxZoom", &Anima::AnimaCamera::SetOrbitMaxZoom)
	.def("GetOrbitMaxZoom", &Anima::AnimaCamera::GetOrbitMaxZoom)
	.def("SetOrbitOffsetDistance", &Anima::AnimaCamera::SetOrbitOffsetDistance)
	.def("GetOrbitOffsetDistance", &Anima::AnimaCamera::GetOrbitOffsetDistance)
//	.def("SetOrbitPitchMaxDegrees", &Anima::AnimaCamera::SetOrbitPitchMaxDegrees)
//	.def("GetOrbitPitchMaxDegrees", &Anima::AnimaCamera::GetOrbitPitchMaxDegrees)
//	.def("SetOrbitPitchMinDegrees", &Anima::AnimaCamera::SetOrbitPitchMinDegrees)
//	.def("GetOrbitPitchMinDegrees", &Anima::AnimaCamera::GetOrbitPitchMinDegrees)
	.def("SetOrientation", &Anima::AnimaCamera::SetOrientation)
	.def("GetOrientation", &Anima::AnimaCamera::GetOrientation, return_value_policy<reference_existing_object>())
	.def("SetRotationSpeed", &Anima::AnimaCamera::SetRotationSpeed)
	.def("GetRotationSpeed", &Anima::AnimaCamera::GetRotationSpeed)
	.def("SetMaximumVelocity", AC_SetMaximumVelocity1)
	.def("SetMaximumVelocity", AC_SetMaximumVelocity2)
	.def("GetMaximumVelocity", &Anima::AnimaCamera::GetMaximumVelocity, return_value_policy<reference_existing_object>())
	.def("GetViewDirection", &Anima::AnimaCamera::GetViewDirection, return_value_policy<reference_existing_object>())
	.def("GetProjectionMatrix", &Anima::AnimaCamera::GetProjectionMatrix)
	.def("GetViewMatrix", &Anima::AnimaCamera::GetViewMatrix)
	.def("GetProjectionViewMatrix", &Anima::AnimaCamera::GetProjectionViewMatrix)
	.def("GetInversedProjectionViewMatrix", &Anima::AnimaCamera::GetInversedProjectionViewMatrix)
	.def("GetXAxis", &Anima::AnimaCamera::GetXAxis, return_value_policy<reference_existing_object>())
	.def("GetYAxis", &Anima::AnimaCamera::GetYAxis, return_value_policy<reference_existing_object>())
	.def("GetZAxis", &Anima::AnimaCamera::GetZAxis, return_value_policy<reference_existing_object>())
	.def("SetPreferTargetYAxisOrbiting", &Anima::AnimaCamera::SetPreferTargetYAxisOrbiting)
	.def("PreferTargetYAxisOrbiting", &Anima::AnimaCamera::PreferTargetYAxisOrbiting)
	.def("Activate", &Anima::AnimaCamera::Activate)
	.def("Deactivate", &Anima::AnimaCamera::Deactivate)
	.def("IsActive", &Anima::AnimaCamera::IsActive)
	;
	
	// AnimaGeometry
	class_<Anima::AnimaGeometry, bases<Anima::AnimaSceneObject> >("AnimaGeometry", no_init)
	.def("MakePlane", &Anima::AnimaGeometry::MakePlane)
	.def("MakeCube", &Anima::AnimaGeometry::MakeCube)
	.def("MakeIcosahedralSphere", &Anima::AnimaGeometry::MakeIcosahedralSphere)
	.def("MakeCylinder", &Anima::AnimaGeometry::MakeCylinder)
	.def("SetMaterial", &Anima::AnimaGeometry::SetMaterial)
	.def("GetMaterial", &Anima::AnimaGeometry::GetMaterial, return_value_policy<reference_existing_object>())
	.def("ComputeBoundingBox", &Anima::AnimaGeometry::ComputeBoundingBox)
	.def("GetBoundingBoxMin", &Anima::AnimaGeometry::GetBoundingBoxMin)
	.def("GetBoundingBoxMax", &Anima::AnimaGeometry::GetBoundingBoxMax)
	.def("GetBoundingBoxCenter", &Anima::AnimaGeometry::GetBoundingBoxCenter)
	.def("AddInstance", &Anima::AnimaGeometry::AddInstance)
	.def("GetInstancesCount", &Anima::AnimaGeometry::GetInstancesCount)
	.def("GetInstance", &Anima::AnimaGeometry::GetInstance, return_value_policy<reference_existing_object>())
	.def("GetInstanceFromName", &Anima::AnimaGeometry::GetInstanceFromName, return_value_policy<reference_existing_object>())
	;
	
	// AnimaNode
	class_<Anima::AnimaNode, bases<Anima::AnimaSceneObject> >("AnimaNode", no_init)
	.def("SetMaterial", &Anima::AnimaNode::SetMaterial)
	.def("GetMaterial", &Anima::AnimaNode::GetMaterial, return_value_policy<reference_existing_object>())
	.def("GetGeometriesCount", &Anima::AnimaNode::GetGeometriesCount)
	.def("AddGeometry", &Anima::AnimaNode::AddGeometry)
	.def("GetGeometry", &Anima::AnimaNode::GetGeometry, return_value_policy<reference_existing_object>())
	.def("GetGeometryFromName", &Anima::AnimaNode::GetGeometryFromName, return_value_policy<reference_existing_object>())
	.def("ComputeBoundingBox", &Anima::AnimaNode::ComputeBoundingBox)
	.def("GetBoundingBoxMin", &Anima::AnimaNode::GetBoundingBoxMin)
	.def("GetBoundingBoxMax", &Anima::AnimaNode::GetBoundingBoxMax)
	.def("GetBoundingBoxCenter", &Anima::AnimaNode::GetBoundingBoxCenter)
	.def("IsAsset", &Anima::AnimaNode::IsAsset)
	;
	
	// AnimaGeometryInstance
	class_<Anima::AnimaGeometryInstance, bases<Anima::AnimaSceneObject> >("AnimaGeometryInstance", no_init)
	.def("SetMaterial", &Anima::AnimaGeometryInstance::SetMaterial)
	.def("GetMaterial", &Anima::AnimaGeometryInstance::GetMaterial, return_value_policy<reference_existing_object>())
	.def("GetGeometry", &Anima::AnimaGeometryInstance::GetGeometry, return_value_policy<reference_existing_object>())
	;
	
	// AnimaNodeInstance
	class_<Anima::AnimaNodeInstance, bases<Anima::AnimaSceneObject> >("AnimaNodeInstance", no_init)
	.def("GetGeometriesCount", &Anima::AnimaNodeInstance::GetGeometriesCount)
	.def("GetGeometry", &Anima::AnimaNodeInstance::GetGeometry, return_value_policy<reference_existing_object>())
	.def("GetGeometries", &Anima::AnimaNodeInstance::GetGeometries, return_value_policy<reference_existing_object>())
	.def("GetAllGeometries", &Anima::AnimaNodeInstance::GetAllGeometries)
	.def("GetNode", &Anima::AnimaNodeInstance::GetNode, return_value_policy<reference_existing_object>())
	.def("IsAsset", &Anima::AnimaNodeInstance::IsAsset)
	;
	
	// AnimaLight
	class_<Anima::AnimaLight, boost::noncopyable, bases<Anima::AnimaSceneObject> >("AnimaLight", no_init)
	.def("SetColor", AL_SetColor1)
	.def("SetColor", AL_SetColor2)
	.def("GetColor", &Anima::AnimaLight::GetColor)
	.def("SetIntensity", &Anima::AnimaLight::SetIntensity)
	.def("GetIntensity", &Anima::AnimaLight::GetIntensity)
	.def("SetDirection", AL_SetDirection1)
	.def("SetDirection", AL_SetDirection2)
	.def("GetDirection", &Anima::AnimaLight::GetDirection)
	.def("SetConstantAttenuation", &Anima::AnimaLight::SetConstantAttenuation)
	.def("SetLinearAttenuation", &Anima::AnimaLight::SetLinearAttenuation)
	.def("SetExponentAttenuation", &Anima::AnimaLight::SetExponentAttenuation)
	.def("GetConstantAttenuation", &Anima::AnimaLight::GetConstantAttenuation)
	.def("GetLinearAttenuation", &Anima::AnimaLight::GetLinearAttenuation)
	.def("GetExponentAttenuation", &Anima::AnimaLight::GetExponentAttenuation)
	.def("SetExtension", &Anima::AnimaLight::SetExtension)
	.def("GetExtension", &Anima::AnimaLight::GetExtension)
	.def("SetRange", &Anima::AnimaLight::SetRange)
	.def("GetRange", &Anima::AnimaLight::GetRange)
	.def("SetCutoff", &Anima::AnimaLight::SetCutoff)
	.def("GetCutoff", &Anima::AnimaLight::GetCutoff)
	.def("ComputeLightMatrix", &Anima::AnimaLight::ComputeLightMatrix)
	.def("GetViewMatrix", &Anima::AnimaLight::GetViewMatrix)
	.def("GetProjectionMatrix", &Anima::AnimaLight::GetProjectionMatrix)
	.def("GetProjectionViewMatrix", &Anima::AnimaLight::GetProjectionViewMatrix)
	.def("GetFrustum", &Anima::AnimaLight::GetFrustum, return_value_policy<reference_existing_object>())
	;
	
	// AnimaDirectionalLight
	class_<Anima::AnimaDirectionalLight, bases<Anima::AnimaLight> >("AnimaDirectionalLight", no_init);

	// IAnimaEvent
	class_<Anima::IAnimaEvents, boost::noncopyable>("IAnimaEvents", no_init)
	.def("SetEventHandles", pure_virtual(&Anima::IAnimaEvents::SetEventHandler))
	.def("LaunchEvent", pure_virtual(&Anima::IAnimaEvents::LaunchEvent))
	.def("GetSupportedEvents", pure_virtual(&Anima::IAnimaEvents::GetSupportedEvents))
	.def("EventIsSupported", pure_virtual(&Anima::IAnimaEvents::EventIsSupported))
	;
	
	// AnimaEventInfo
	class_<Anima::AnimaEventInfo>("AnimaEventInfo")
	.def_readwrite("eventName", &Anima::AnimaEventInfo::_eventName)
	.def_readwrite("eventDescription", &Anima::AnimaEventInfo::_eventDescription)
	;
	
	// AnimaEventArgs
	class_<Anima::AnimaEventArgs>("AnimaEventArgs", init<Anima::IAnimaEvents*>())
	.def("GetSourceEvent", &Anima::AnimaEventArgs::GetSourceEvent, return_value_policy<reference_existing_object>())
	;
	
	// IAnimaInteractor
	class_<Anima::IAnimaInteractor, boost::noncopyable>("IAnimaInteractor", no_init)
	.def("Install", pure_virtual(&Anima::IAnimaInteractor::Install))
	.def("Remove", pure_virtual(&Anima::IAnimaInteractor::Remove))
	.def("UpdateScene", pure_virtual(&Anima::IAnimaInteractor::UpdateScene))
	;
	
	// AnimaMouseInteractor
	class_<Anima::AnimaMouseInteractor, bases<Anima::IAnimaEvents, Anima::IAnimaInteractor> >("AnimaMouseInteractor")
	.def("Install", &Anima::AnimaMouseInteractor::Install)
	.def("Remove", &Anima::AnimaMouseInteractor::Remove)
	.def("UpdateScene", &Anima::AnimaMouseInteractor::UpdateScene)
	.def("SetEventHandles", &Anima::AnimaMouseInteractor::SetEventHandler)
	.def("LaunchEvent", &Anima::AnimaMouseInteractor::LaunchEvent)
	.def("GetSupportedEvents", &Anima::AnimaMouseInteractor::GetSupportedEvents)
	.def("EventIsSupported", &Anima::AnimaMouseInteractor::EventIsSupported)
	.def("GetWindowId", &Anima::AnimaMouseInteractor::GetWindowId)
	.def("GetEngine", &Anima::AnimaMouseInteractor::GetEngine, return_value_policy<reference_existing_object>())
	;
	
	// AnimaMouseEventArgs
	class_<Anima::AnimaMouseEventArgs, bases<Anima::AnimaEventArgs> >("AnimaMouseEventArgs", init<Anima::IAnimaEvents*, Anima::AnimaVertex2f, Anima::AnimaVertex2f, Anima::AInt>())
	.def("GetPoint", &Anima::AnimaMouseEventArgs::GetPoint)
	.def("GetWindowSize", &Anima::AnimaMouseEventArgs::GetWindowSize)
	.def("GetModifiers", &Anima::AnimaMouseEventArgs::GetModifiers)
	;
	
	// AnimaMouseDraggedEventArgs
	class_<Anima::AnimaMouseDraggedEventArgs, bases<Anima::AnimaMouseEventArgs> >("AnimaMouseDraggedEventArgs", init<Anima::IAnimaEvents*, Anima::AnimaVertex2f, Anima::AnimaVertex2f, Anima::AInt, Anima::AnimaVertex2f>())
	.def("GetDelta", &Anima::AnimaMouseDraggedEventArgs::GetDelta)
	;
	
	// AnimaKeyboardInteractor
	class_<Anima::AnimaKeyboardInteractor, bases<Anima::IAnimaEvents, Anima::IAnimaInteractor> >("AnimaKoyboardInteractor")
	.def("Install", &Anima::AnimaKeyboardInteractor::Install)
	.def("Remove", &Anima::AnimaKeyboardInteractor::Remove)
	.def("UpdateScene", &Anima::AnimaKeyboardInteractor::UpdateScene)
	.def("SetEventHandles", &Anima::AnimaKeyboardInteractor::SetEventHandler)
	.def("LaunchEvent", &Anima::AnimaKeyboardInteractor::LaunchEvent)
	.def("GetSupportedEvents", &Anima::AnimaKeyboardInteractor::GetSupportedEvents)
	.def("EventIsSupported", &Anima::AnimaKeyboardInteractor::EventIsSupported)
	.def("GetWindowId", &Anima::AnimaKeyboardInteractor::GetWindowId)
	.def("GetEngine", &Anima::AnimaKeyboardInteractor::GetEngine, return_value_policy<reference_existing_object>())
	.def("GetKeyboardStatus", &Anima::AnimaKeyboardInteractor::GetKeyboardStatus, return_value_policy<reference_existing_object>())
	;
	
	// AnimaKeyboardEventArgs
	class_<Anima::AnimaKeyboardEventArgs, bases<Anima::AnimaEventArgs> >("AnimaKeyboardEventArgs", init<Anima::IAnimaEvents*, Anima::AnimaKeyboardKey, Anima::AInt, const Anima::AnimaKeyboardStatusMap&>())
	.def("GetKey", &Anima::AnimaKeyboardEventArgs::GetKey, return_value_policy<copy_const_reference>())
	.def("GetModifiers", &Anima::AnimaKeyboardEventArgs::GetModifiers, return_value_policy<copy_const_reference>())
	.def("GetKeyboardStatus", &Anima::AnimaKeyboardEventArgs::GetKeyboardStatus, return_value_policy<copy_const_reference>())
	;
	
	// AnimaJoystickInteractor
	class_<Anima::AnimaJoystickInteractor, boost::noncopyable, bases<Anima::IAnimaEvents, Anima::IAnimaInteractor> >("AnimaJoystickInteractor", no_init)
	.def("GetInstance", &Anima::AnimaJoystickInteractor::GetInstance, return_value_policy<reference_existing_object>())
	.def("Install", &Anima::AnimaJoystickInteractor::Install)
	.def("Remove", &Anima::AnimaJoystickInteractor::Remove)
	.def("UpdateScene", &Anima::AnimaJoystickInteractor::UpdateScene)
	.def("SetEventHandles", &Anima::AnimaJoystickInteractor::SetEventHandler)
	.def("LaunchEvent", &Anima::AnimaJoystickInteractor::LaunchEvent)
	.def("GetSupportedEvents", &Anima::AnimaJoystickInteractor::GetSupportedEvents)
	.def("EventIsSupported", &Anima::AnimaJoystickInteractor::EventIsSupported)
	.def("GetEngine", &Anima::AnimaJoystickInteractor::GetEngine, return_value_policy<reference_existing_object>())
	.def("SetZeroThreshold", &Anima::AnimaJoystickInteractor::SetZeroThreshold)
	.def("GetZeroThreshold", &Anima::AnimaJoystickInteractor::GetZeroThreshold)
	.def("SetSensibility", &Anima::AnimaJoystickInteractor::SetSensibility)
	.def("GetSensibility", &Anima::AnimaJoystickInteractor::GetSensibility)
	.def("GetJoystick", &Anima::AnimaJoystickInteractor::GetJoystick, return_value_policy<reference_existing_object>())
	.staticmethod("GetInstance")
	;
	
	class_<Anima::AnimaDefaultJoystickInteractor, boost::noncopyable, bases<Anima::AnimaJoystickInteractor> >("AnimaDefaultJoystickInteractor", no_init)
	.def("GetInstance", &Anima::AnimaDefaultJoystickInteractor::GetInstance, return_value_policy<reference_existing_object>())
	;
	
	class_<Anima::AnimaDefaultMouseInteractor, bases<Anima::AnimaMouseInteractor> >("AnimaDefaultMouseInteractor");
	class_<Anima::AnimaDefaultKeyboardInteractor, bases<Anima::AnimaKeyboardInteractor> >("AnimaDefaultKeyboardInteractor");
	
	// AnimaJoystickDevice
	class_<Anima::AnimaJoystickDevice>("AnimaJoystickDevice")
	.def("IsPresent", &Anima::AnimaJoystickDevice::IsPresent)
	.def("GetName", &Anima::AnimaJoystickDevice::GetName, return_value_policy<reference_existing_object>())
	.def("GetAxis", &Anima::AnimaJoystickDevice::GetAxis, return_value_policy<reference_existing_object>())
	.def("GetButtons", &Anima::AnimaJoystickDevice::GetButtons, return_value_policy<reference_existing_object>())
	;
	
	// AnimaJoystickEventArgs
	class_<Anima::AnimaJoystickEventArgs, bases<Anima::AnimaEventArgs> >("AnimaJoystickEventArgs", init<Anima::IAnimaEvents*, const Anima::AnimaString&, Anima::AInt, const Anima::AnimaArray<bool>&, const Anima::AnimaArray<Anima::AFloat>&>())
	.def("GetJoystickName", &Anima::AnimaJoystickEventArgs::GetJoystickName, return_value_policy<copy_const_reference>())
	.def("GetJoystickId", &Anima::AnimaJoystickEventArgs::GetJoystickId, return_value_policy<copy_const_reference>())
	.def("GetButtonsState", &Anima::AnimaJoystickEventArgs::GetButtonsState, return_value_policy<copy_const_reference>())
	.def("GetAxisState", &Anima::AnimaJoystickEventArgs::GetAxisState, return_value_policy<copy_const_reference>())
	;
	
	// AnimaJoystickButtonEventArgs
	class_<Anima::AnimaJoystickButtonEventArgs, bases<Anima::AnimaJoystickEventArgs> >("AnimaJoystickButtonEventArgs", init<Anima::IAnimaEvents*, const Anima::AnimaString&, Anima::AInt, const Anima::AnimaArray<bool>&, const Anima::AnimaArray<Anima::AFloat>&, Anima::AInt>())
	.def("GetButtonId", &Anima::AnimaJoystickButtonEventArgs::GetButtonId, return_value_policy<copy_const_reference>())
	;
	
	// AnimaJoystickAxisEventArgs
	class_<Anima::AnimaJoystickAxisEventArgs, bases<Anima::AnimaJoystickEventArgs> >("AnimaJoystickAxisEventArgs", init<Anima::IAnimaEvents*, const Anima::AnimaString&, Anima::AInt, const Anima::AnimaArray<bool>&, const Anima::AnimaArray<Anima::AFloat>&, Anima::AInt, Anima::AFloat>())
	.def("GetAxisId", &Anima::AnimaJoystickAxisEventArgs::GetAxisId, return_value_policy<copy_const_reference>())
	.def("GetAxisValue", &Anima::AnimaJoystickAxisEventArgs::GetAxisValue, return_value_policy<copy_const_reference>())
	;
	
	// AnimaRenderer
	class_<Anima::AnimaRenderer, boost::noncopyable>("AnimaRenderer", init<Anima::AnimaEngine*, Anima::AnimaAllocator*>())
	.def("Start", AR_Start1)
	.def("Render", &Anima::AnimaRenderer::Render)
	.def("InitTextureSlots", &Anima::AnimaRenderer::InitTextureSlots)
	.def("InitRenderingTargets", &Anima::AnimaRenderer::InitRenderingTargets)
	.def("InitRenderingUtilities", &Anima::AnimaRenderer::InitRenderingUtilities)
	.def("CheckPrograms", &Anima::AnimaRenderer::CheckPrograms)
	;

	// AnimaGC
	class_<Anima::AnimaGC>("AnimaGC", no_init)
	.def("CreateContext", &Anima::AnimaGC::CreateContext, return_value_policy<manage_new_object>())
	.def("DestroyContext", &Anima::AnimaGC::DestroyContext)
	.def("GetDefaultContextConfig", &Anima::AnimaGC::GetDefaultContextConfig)
	.def("GetDefaultFrameBufferConfig", &Anima::AnimaGC::GetDefaultFrameBufferConfig)
	.def("InitializeGLEWExtensions", &Anima::AnimaGC::InitializeGLEWExtensions)
	.def("SetUpdateFrameCallback", &AnimaGC_SetUpdateFrameCallback_aux)
	.def("MakeCurrent", &Anima::AnimaGC::MakeCurrent)
	.def("SwapBuffers", &Anima::AnimaGC::SwapBuffers)
	.def("ClearColor", &Anima::AnimaGC::ClearColor)
	.staticmethod("CreateContext")
	.staticmethod("DestroyContext")
	.staticmethod("GetDefaultContextConfig")
	.staticmethod("GetDefaultFrameBufferConfig")
	.staticmethod("InitializeGLEWExtensions")
	;

	class_<Anima::AnimaGCFrameBufferConfig>("AnimaGCFrameBufferConfig")
	.def_readwrite("redBits", &Anima::AnimaGCFrameBufferConfig::_redBits)
	.def_readwrite("greenBits", &Anima::AnimaGCFrameBufferConfig::_greenBits)
	.def_readwrite("blueBits", &Anima::AnimaGCFrameBufferConfig::_blueBits)
	.def_readwrite("alphaBits", &Anima::AnimaGCFrameBufferConfig::_alphaBits)
	.def_readwrite("depthBits", &Anima::AnimaGCFrameBufferConfig::_depthBits)
	.def_readwrite("stencilBits", &Anima::AnimaGCFrameBufferConfig::_stencilBits)
	.def_readwrite("accumRedBits", &Anima::AnimaGCFrameBufferConfig::_accumRedBits)
	.def_readwrite("accumGreenBits", &Anima::AnimaGCFrameBufferConfig::_accumGreenBits)
	.def_readwrite("accumBlueBits", &Anima::AnimaGCFrameBufferConfig::_accumBlueBits)
	.def_readwrite("accumAlphaBits", &Anima::AnimaGCFrameBufferConfig::_accumAlphaBits)
	.def_readwrite("auxBuffers", &Anima::AnimaGCFrameBufferConfig::_auxBuffers)
	.def_readwrite("stereo", &Anima::AnimaGCFrameBufferConfig::_stereo)
	.def_readwrite("samples", &Anima::AnimaGCFrameBufferConfig::_samples)
	.def_readwrite("sRGB", &Anima::AnimaGCFrameBufferConfig::_sRGB)
	.def_readwrite("doubleBuffer", &Anima::AnimaGCFrameBufferConfig::_doublebuffer)
	;
	
	class_<Anima::AnimaGCContextConfig>("AnimaGCContextConfig")
	.def_readwrite("api", &Anima::AnimaGCContextConfig::_api)
	.def_readwrite("major", &Anima::AnimaGCContextConfig::_major)
	.def_readwrite("minor", &Anima::AnimaGCContextConfig::_minor)
	.def_readwrite("forward", &Anima::AnimaGCContextConfig::_forward)
	.def_readwrite("debug", &Anima::AnimaGCContextConfig::_debug)
	.def_readwrite("profile", &Anima::AnimaGCContextConfig::_profile)
	.def_readwrite("robustness", &Anima::AnimaGCContextConfig::_robustness)
	.def_readwrite("release", &Anima::AnimaGCContextConfig::_release)
	;

	// AnimaTimer
	class_<Anima::AnimaTimer>("AnimaTimer")
	.def("Reset", &Anima::AnimaTimer::Reset)
	.def("Elapsed", &Anima::AnimaTimer::Elapsed)
	;

	// AnimaAllocator
	class_<Anima::AnimaAllocator, boost::noncopyable>("AnimaAllocator", no_init);

	// AnimaVertex2f
	class_<Anima::AnimaVertex2f>("AnimaVertex2f")
	.def(init<>())
	.def(init<Anima::AFloat, Anima::AFloat>())
	.def(init<Anima::AnimaVertex3f&>())
	.def(init<Anima::AnimaVertex4f&>())
	.def_readwrite("x", &Anima::AnimaVertex2f::x)
	.def_readwrite("y", &Anima::AnimaVertex2f::y)
	.def_readwrite("u", &Anima::AnimaVertex2f::u)
	.def_readwrite("v", &Anima::AnimaVertex2f::v)
	;
	
	// AnimaVertex3f
	class_<Anima::AnimaVertex3f>("AnimaVertex3f")
	.def(init<>())
	.def(init<Anima::AFloat>())
	.def(init<Anima::AFloat, Anima::AFloat, Anima::AFloat>())
	.def(init<Anima::AnimaVertex2f&, const Anima::AFloat>())
	.def_readwrite("x", &Anima::AnimaVertex3f::x)
	.def_readwrite("y", &Anima::AnimaVertex3f::y)
	.def_readwrite("z", &Anima::AnimaVertex3f::z)
	.def_readwrite("r", &Anima::AnimaVertex3f::r)
	.def_readwrite("g", &Anima::AnimaVertex3f::g)
	.def_readwrite("b", &Anima::AnimaVertex3f::b)
	;
	
	// AnimaVertex4f
	class_<Anima::AnimaVertex4f>("AnimaVertex4f")
	.def(init<>())
	.def(init<Anima::AFloat>())
	.def(init<Anima::AFloat, Anima::AFloat, Anima::AFloat, Anima::AFloat>())
	.def(init<Anima::AnimaVertex2f&, const Anima::AFloat, const Anima::AFloat>())
	.def(init<Anima::AnimaVertex3f&, const Anima::AFloat>())
	.def_readwrite("x", &Anima::AnimaVertex4f::x)
	.def_readwrite("y", &Anima::AnimaVertex4f::y)
	.def_readwrite("z", &Anima::AnimaVertex4f::z)
	.def_readwrite("w", &Anima::AnimaVertex4f::w)
	.def_readwrite("r", &Anima::AnimaVertex4f::r)
	.def_readwrite("g", &Anima::AnimaVertex4f::g)
	.def_readwrite("b", &Anima::AnimaVertex4f::b)
	.def_readwrite("a", &Anima::AnimaVertex4f::a)
	;
	
	// AnimaTransformation
	class_<Anima::AnimaTransformation>("AnimaTransformation", init<>())
	.def("Translate", AT_Translate1)
	.def("Translate", AT_Translate2)
	.def("TranslateX", &Anima::AnimaTransformation::TranslateX)
	.def("TranslateY", &Anima::AnimaTransformation::TranslateY)
	.def("TranslateZ", &Anima::AnimaTransformation::TranslateZ)
	.def("SetTranslation", AT_SetTranslation1)
	.def("SetTranslation", AT_SetTranslation2)
	.def("SetTranslationX", &Anima::AnimaTransformation::SetTranslationX)
	.def("SetTranslationY", &Anima::AnimaTransformation::SetTranslationY)
	.def("SetTranslationZ", &Anima::AnimaTransformation::SetTranslationZ)
	.def("GetTranslation", &Anima::AnimaTransformation::GetTranslation)
	.def("GetTranslationX", &Anima::AnimaTransformation::GetTranslationX)
	.def("GetTranslationY", &Anima::AnimaTransformation::GetTranslationY)
	.def("GetTranslationZ", &Anima::AnimaTransformation::GetTranslationZ)
	.def("GetCompleteTranslation", &Anima::AnimaTransformation::GetCompleteTranslation)
	.def("Rotate", AT_Rotate1)
	.def("Rotate", AT_Rotate2)
	.def("RotateX", &Anima::AnimaTransformation::RotateX)
	.def("RotateY", &Anima::AnimaTransformation::RotateY)
	.def("RotateZ", &Anima::AnimaTransformation::RotateZ)
	.def("SetRotation", AT_SetRotation1)
	.def("SetRotation", AT_SetRotation2)
	.def("SetRotationX", &Anima::AnimaTransformation::SetRotationX)
	.def("SetRotationY", &Anima::AnimaTransformation::SetRotationY)
	.def("SetRotationZ", &Anima::AnimaTransformation::SetRotationZ)
	.def("RotateDeg", AT_RotateDeg1)
	.def("RotateDeg", AT_RotateDeg2)
	.def("RotateXDeg", &Anima::AnimaTransformation::RotateXDeg)
	.def("RotateYDeg", &Anima::AnimaTransformation::RotateYDeg)
	.def("RotateZDeg", &Anima::AnimaTransformation::RotateZDeg)
	.def("SetRotationDeg", AT_SetRotationDeg1)
	.def("SetRotationDeg", AT_SetRotationDeg2)
	.def("SetRotationXDeg", &Anima::AnimaTransformation::SetRotationXDeg)
	.def("SetRotationYDeg", &Anima::AnimaTransformation::SetRotationYDeg)
	.def("SetRotationZDeg", &Anima::AnimaTransformation::SetRotationZDeg)
	.def("GetRotation", &Anima::AnimaTransformation::GetRotation)
	.def("GetRotationX", &Anima::AnimaTransformation::GetRotationX)
	.def("GetRotationY", &Anima::AnimaTransformation::GetRotationY)
	.def("GetRotationZ", &Anima::AnimaTransformation::GetRotationZ)
	.def("GetCompleteRotation", &Anima::AnimaTransformation::GetCompleteRotation)
	.def("Scale", AT_Scale1)
	.def("Scale", AT_Scale2)
	.def("ScaleX", &Anima::AnimaTransformation::ScaleX)
	.def("ScaleY", &Anima::AnimaTransformation::ScaleY)
	.def("ScaleZ", &Anima::AnimaTransformation::ScaleZ)
	.def("SetScale", AT_SetScale1)
	.def("SetScale", AT_SetScale2)
	.def("SetScaleX", &Anima::AnimaTransformation::SetScaleX)
	.def("SetScaleY", &Anima::AnimaTransformation::SetScaleY)
	.def("SetScaleZ", &Anima::AnimaTransformation::SetScaleZ)
	.def("GetScale", &Anima::AnimaTransformation::GetScale)
	.def("GetScaleX", &Anima::AnimaTransformation::GetScaleX)
	.def("GetScaleY", &Anima::AnimaTransformation::GetScaleY)
	.def("GetScaleZ", &Anima::AnimaTransformation::GetScaleZ)
	.def("GetCompleteScale", &Anima::AnimaTransformation::GetCompleteScale)
	;
}

#endif