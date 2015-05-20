%module AnimaEngine

%{
#include "AnimaEngineCore.h"
#include "AnimaEngine.h" 
#include "AnimaAssert.h" 
#include "AnimaTypes.h" 
#include "AnimaAllocator.h" 
#include "AnimaStackAllocator.h"
#include "AnimaLinearAllocator.h"
#include "AnimaFreeListAllocator.h"
#include "AnimaPoolAllocator.h"
#include "AnimaProxyAllocator.h"
#include "AnimaString.h"
#include "AnimaMatrix.h"
#include "AnimaVertex.h"
#include "AnimaQuaternion.h"
#include "AnimaMath.h"
#include "AnimaTimer.h"
#include "AnimaFPSTimer.h"

#include "AnimaFrustum.h"

#include "AnimaArray.h"
#include "AnimaMappedArray.h"
#include "AnimaTypeMappedArray.h"

#include "AnimaNamedObject.h"
#include "AnimaMappedValues.h"
#include "AnimaSceneObject.h"

#include "AnimaTransformation.h"

#include "AnimaCamera.h"
#include "AnimaFirstPersonCamera.h"
#include "AnimaThirdPersonCamera.h"

#include "AnimaTexture.h"
#include "AnimaGBuffer.h"

#include "AnimaDataGenerator.h"
#include "AnimaColorGenerator.h"
#include "AnimaVectorGenerator.h"

#include "AnimaLight.h"

#include "AnimaMaterial.h"

#include "AnimaFace.h"
#include "AnimaMesh.h"
#include "AnimaModel.h"
#include "AnimaModelInstance.h"
#include "AnimaMeshInstance.h"
#include "AnimaAnimation.h"

#include "AnimaMeshCreator.h"

#include "AnimaShader.h"
#include "AnimaShaderProgram.h"

#include "AnimaScene.h"

#include "AnimaDataGeneratorsManager.h"
#include "AnimaLightsManager.h"
#include "AnimaCamerasManager.h"
#include "AnimaTexturesManager.h"
#include "AnimaMaterialsManager.h"
#include "AnimaShadersManager.h"
#include "AnimaMeshesManager.h"
#include "AnimaModelsManager.h"
#include "AnimaMeshInstancesManager.h"
#include "AnimaModelInstancesManager.h"
#include "AnimaAnimationsManager.h"
#include "AnimaScenesManager.h"
#include "AnimaRenderer.h"

using namespace Anima;

%}

%include "AnimaEngineCore.h"
%include "AnimaEngine.h" 
%include "AnimaAssert.h" 
%include "AnimaTypes.h" 
%include "AnimaAllocator.h" 
%include "AnimaStackAllocator.h"
%include "AnimaLinearAllocator.h"
%include "AnimaFreeListAllocator.h"
%include "AnimaPoolAllocator.h"
%include "AnimaProxyAllocator.h"
%include "AnimaString.h"
%include "AnimaMatrix.h"
%include "AnimaVertex.h"
%include "AnimaQuaternion.h"
%include "AnimaMath.h"
%include "AnimaTimer.h"
%include "AnimaFPSTimer.h"

%include "AnimaFrustum.h"

%include "AnimaArray.h"
%include "AnimaMappedArray.h"
%include "AnimaTypeMappedArray.h"

%include "AnimaNamedObject.h"
%include "AnimaMappedValues.h"
%include "AnimaSceneObject.h"

%include "AnimaTransformation.h"

%include "AnimaCamera.h"
%include "AnimaFirstPersonCamera.h"
%include "AnimaThirdPersonCamera.h"

%include "AnimaTexture.h"
%include "AnimaGBuffer.h"

%include "AnimaDataGenerator.h"
%include "AnimaColorGenerator.h"
%include "AnimaVectorGenerator.h"

%include "AnimaLight.h"

%include "AnimaMaterial.h"

%include "AnimaFace.h"
%include "AnimaMesh.h"
%include "AnimaModel.h"
%include "AnimaModelInstance.h"
%include "AnimaMeshInstance.h"
%include "AnimaAnimation.h"

%include "AnimaMeshCreator.h"

%include "AnimaShader.h"
%include "AnimaShaderProgram.h"

%include "AnimaScene.h"

%include "AnimaDataGeneratorsManager.h"
%include "AnimaLightsManager.h"
%include "AnimaCamerasManager.h"
%include "AnimaTexturesManager.h"
%include "AnimaMaterialsManager.h"
%include "AnimaShadersManager.h"
%include "AnimaMeshesManager.h"
%include "AnimaModelsManager.h"
%include "AnimaMeshInstancesManager.h"
%include "AnimaModelInstancesManager.h"
%include "AnimaAnimationsManager.h"
%include "AnimaScenesManager.h"
%include "AnimaRenderer.h"