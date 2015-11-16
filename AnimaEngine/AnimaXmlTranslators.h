//
//  AnimaXmlTranslators.h
//  Anima
//
//  Created by Marco Zille on 30/10/14.
//
//

#ifndef Anima_AnimaXmlTranslators_h
#define Anima_AnimaXmlTranslators_h

#include "AnimaEngineCore.h"
#include "AnimaTypes.h"
#include "AnimaString.h"

#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string.hpp>

#include "AnimaVertex.h"
#include "AnimaShaderData.h"
#include "AnimaArray.h"
#include "AnimaMatrix.h"
#include "AnimaTexture.h"
#include "AnimaCamera.h"

struct AnimaXmlVertex2Translator
{
	typedef Anima::AnimaString internal_type;
	typedef Anima::AnimaVertex2f external_type;

	boost::optional<external_type> get_value(const internal_type& str)
	{
		if (!str.empty())
		{
			Anima::AnimaString inputString;
			if (str[str.length() - 1] == ';')
				inputString = str.substr(0, str.length() - 1);
			else
				inputString = str;

			std::vector<Anima::AnimaString> elements;
			boost::split(elements, inputString, boost::is_any_of(";"));

			Anima::AnimaVertex2f vertex;

			size_t elementsSize = elements.size();

			if (elementsSize != 2)
				return boost::optional<external_type>(boost::none);

			vertex.x = (Anima::AFloat)atof(elements[0].c_str());
			vertex.y = (Anima::AFloat)atof(elements[1].c_str());

			return vertex;
		}
		else
			return boost::optional<external_type>(boost::none);
	}

	boost::optional<internal_type> put_value(const external_type& vertex)
	{
		return std::to_string(vertex.x) + ";" + std::to_string(vertex.y);
	}
};

struct AnimaXmlVertex3Translator
{
	typedef Anima::AnimaString internal_type;
	typedef Anima::AnimaVertex3f external_type;

	boost::optional<external_type> get_value(const internal_type& str)
	{
		if (!str.empty())
		{
			Anima::AnimaString inputString;
			if (str[str.length() - 1] == ';')
				inputString = str.substr(0, str.length() - 1);
			else
				inputString = str;

			std::vector<Anima::AnimaString> elements;
			boost::split(elements, inputString, boost::is_any_of(";"));

			Anima::AnimaVertex3f vertex;

			size_t elementsSize = elements.size();

			if (elementsSize != 3)
				return boost::optional<external_type>(boost::none);

			vertex.x = (Anima::AFloat)atof(elements[0].c_str());
			vertex.y = (Anima::AFloat)atof(elements[1].c_str());
			vertex.z = (Anima::AFloat)atof(elements[2].c_str());

			return vertex;
		}
		else
			return boost::optional<external_type>(boost::none);
	}

	boost::optional<internal_type> put_value(const external_type& vertex)
	{
		return std::to_string(vertex.x) + ";" + std::to_string(vertex.y) + ";" + std::to_string(vertex.z);
	}
};

struct AnimaXmlVertex4Translator
{
	typedef Anima::AnimaString internal_type;
	typedef Anima::AnimaVertex4f external_type;

	boost::optional<external_type> get_value(const internal_type& str)
	{
		if (!str.empty())
		{
			Anima::AnimaString inputString;
			if (str[str.length() - 1] == ';')
				inputString = str.substr(0, str.length() - 1);
			else
				inputString = str;

			std::vector<Anima::AnimaString> elements;
			boost::split(elements, inputString, boost::is_any_of(";"));

			Anima::AnimaVertex4f vertex;

			size_t elementsSize = elements.size();

			if (elementsSize != 4)
				return boost::optional<external_type>(boost::none);

			vertex.x = (Anima::AFloat)atof(elements[0].c_str());
			vertex.y = (Anima::AFloat)atof(elements[1].c_str());
			vertex.z = (Anima::AFloat)atof(elements[2].c_str());
			vertex.w = (Anima::AFloat)atof(elements[3].c_str());

			return vertex;
		}
		else
			return boost::optional<external_type>(boost::none);
	}

	boost::optional<internal_type> put_value(const external_type& vertex)
	{
		return std::to_string(vertex.x) + ";" + std::to_string(vertex.y) + ";" + std::to_string(vertex.z) + ";" + std::to_string(vertex.w);
	}
};

struct AnimaXmlQuaternionTranslator
{
	typedef Anima::AnimaString internal_type;
	typedef Anima::AnimaQuaternion external_type;
	
	boost::optional<external_type> get_value(const internal_type& str)
	{
		if (!str.empty())
		{
			Anima::AnimaString inputString;
			if (str[str.length() - 1] == ';')
				inputString = str.substr(0, str.length() - 1);
			else
				inputString = str;
			
			std::vector<Anima::AnimaString> elements;
			boost::split(elements, inputString, boost::is_any_of(";"));
			
			Anima::AnimaQuaternion quaternion;
			
			size_t elementsSize = elements.size();
			
			if (elementsSize != 4)
				return boost::optional<external_type>(boost::none);
			
			quaternion.x = (Anima::AFloat)atof(elements[0].c_str());
			quaternion.y = (Anima::AFloat)atof(elements[1].c_str());
			quaternion.z = (Anima::AFloat)atof(elements[2].c_str());
			quaternion.w = (Anima::AFloat)atof(elements[3].c_str());
			
			return quaternion;
		}
		else
			return boost::optional<external_type>(boost::none);
	}
	
	boost::optional<internal_type> put_value(const external_type& quaternion)
	{
		return std::to_string(quaternion.x) + ";" + std::to_string(quaternion.y) + ";" + std::to_string(quaternion.z) + ";" + std::to_string(quaternion.w);
	}
};

struct AnimaXmlMatrixTranslator
{
	typedef Anima::AnimaString internal_type;
	typedef Anima::AnimaMatrix external_type;

	boost::optional<external_type> get_value(const internal_type& str)
	{
		if (!str.empty())
		{
			Anima::AnimaString inputString;
			if (str[str.length() - 1] == ';')
				inputString = str.substr(0, str.length() - 1);
			else
				inputString = str;

			std::vector<Anima::AnimaString> elements;
			boost::split(elements, inputString, boost::is_any_of(";"));

			Anima::AnimaMatrix matrix;

			size_t elementsSize = elements.size();

			if (elementsSize != 16)
				return boost::optional<external_type>(boost::none);

			matrix.m[0] = (Anima::AFloat)atof(elements[0].c_str());
			matrix.m[1] = (Anima::AFloat)atof(elements[1].c_str());
			matrix.m[2] = (Anima::AFloat)atof(elements[2].c_str());
			matrix.m[3] = (Anima::AFloat)atof(elements[3].c_str());
			matrix.m[4] = (Anima::AFloat)atof(elements[4].c_str());
			matrix.m[5] = (Anima::AFloat)atof(elements[5].c_str());
			matrix.m[6] = (Anima::AFloat)atof(elements[6].c_str());
			matrix.m[7] = (Anima::AFloat)atof(elements[7].c_str());
			matrix.m[8] = (Anima::AFloat)atof(elements[8].c_str());
			matrix.m[9] = (Anima::AFloat)atof(elements[9].c_str());
			matrix.m[10] = (Anima::AFloat)atof(elements[10].c_str());
			matrix.m[11] = (Anima::AFloat)atof(elements[11].c_str());
			matrix.m[12] = (Anima::AFloat)atof(elements[12].c_str());
			matrix.m[13] = (Anima::AFloat)atof(elements[13].c_str());
			matrix.m[14] = (Anima::AFloat)atof(elements[14].c_str());
			matrix.m[15] = (Anima::AFloat)atof(elements[15].c_str());

			return matrix;
		}
		else
			return boost::optional<external_type>(boost::none);
	}

	boost::optional<internal_type> put_value(const external_type& matrix)
	{
		return	std::to_string(matrix.m[0]) + ";" +
			std::to_string(matrix.m[1]) + ";" +
			std::to_string(matrix.m[2]) + ";" +
			std::to_string(matrix.m[3]) + ";" +
			std::to_string(matrix.m[4]) + ";" +
			std::to_string(matrix.m[5]) + ";" +
			std::to_string(matrix.m[6]) + ";" +
			std::to_string(matrix.m[7]) + ";" +
			std::to_string(matrix.m[8]) + ";" +
			std::to_string(matrix.m[9]) + ";" +
			std::to_string(matrix.m[10]) + ";" +
			std::to_string(matrix.m[11]) + ";" +
			std::to_string(matrix.m[12]) + ";" +
			std::to_string(matrix.m[13]) + ";" +
			std::to_string(matrix.m[14]) + ";" +
			std::to_string(matrix.m[15]);
	}
};

struct AnimaXmlAUintArrayTranslator
{
	typedef Anima::AnimaString internal_type;
	typedef Anima::AnimaArray<Anima::AUint> external_type;

	boost::optional<external_type> get_value(const internal_type& str)
	{
		external_type result;

		if (!str.empty())
		{
			Anima::AnimaString inputString;
			if (str[str.length() - 1] == ';')
				inputString = str.substr(0, str.length() - 1);
			else
				inputString = str;

			std::vector<Anima::AnimaString> elements;
			boost::split(elements, inputString, boost::is_any_of(";"));

			for (auto& element : elements)
				result.push_back(atoi(element.c_str()));
		}

		return result;
	}

	boost::optional<internal_type> put_value(const external_type& input)
	{
		internal_type result = "";

		for (auto& element : input)
			result += std::to_string(element) + ";";

		return result;
	}
};

struct AnimaXmlVertex4ArrayTranslator
{
	typedef Anima::AnimaString internal_type;
	typedef Anima::AnimaArray<Anima::AnimaVertex4f> external_type;

	boost::optional<external_type> get_value(const internal_type& str)
	{
		external_type result;

		if (!str.empty())
		{
			Anima::AnimaString inputString;
			if (str[str.length() - 1] == ';')
				inputString = str.substr(0, str.length() - 1);
			else
				inputString = str;

			std::vector<Anima::AnimaString> elements;
			boost::split(elements, inputString, boost::is_any_of(";"));

			size_t elementsSize = elements.size();

			if (elementsSize % 4 != 0)
				return boost::optional<external_type>(boost::none);

			for (Anima::AInt i = 0; i < elementsSize; i += 4)
			{
				Anima::AnimaVertex4f vertex;
				vertex.x = (Anima::AFloat)atof(elements[i].c_str());
				vertex.y = (Anima::AFloat)atof(elements[i + 1].c_str());
				vertex.z = (Anima::AFloat)atof(elements[i + 2].c_str());
				vertex.w = (Anima::AFloat)atof(elements[i + 3].c_str());

				result.push_back(vertex);
			}
		}

		return result;
	}

	boost::optional<internal_type> put_value(const external_type& input)
	{
		internal_type result = "";

		Anima::AInt count = input.size();
		for (Anima::AInt i = 0; i < count; i++)
		{
			Anima::AnimaVertex4f vertex = input[i];
			result += std::to_string(vertex.x) + ";" + std::to_string(vertex.y) + ";" + std::to_string(vertex.z) + ";" + std::to_string(vertex.w) + ";";
		}

		return result;
	}
};

struct AnimaXmlVertex3ArrayTranslator
{
	typedef Anima::AnimaString internal_type;
	typedef Anima::AnimaArray<Anima::AnimaVertex3f> external_type;

	boost::optional<external_type> get_value(const internal_type& str)
	{
		external_type result;

		if (!str.empty())
		{
			Anima::AnimaString inputString;
			if (str[str.length() - 1] == ';')
				inputString = str.substr(0, str.length() - 1);
			else
				inputString = str;

			std::vector<Anima::AnimaString> elements;
			boost::split(elements, inputString, boost::is_any_of(";"));

			size_t elementsSize = elements.size();

			if (elementsSize % 3 != 0)
				return boost::optional<external_type>(boost::none);

			for (Anima::AInt i = 0; i < elementsSize; i += 3)
			{
				Anima::AnimaVertex3f vertex;
				vertex.x = (Anima::AFloat)atof(elements[i].c_str());
				vertex.y = (Anima::AFloat)atof(elements[i + 1].c_str());
				vertex.z = (Anima::AFloat)atof(elements[i + 2].c_str());

				result.push_back(vertex);
			}
		}
		return result;
	}

	boost::optional<internal_type> put_value(const external_type& input)
	{
		internal_type result = "";

		Anima::AInt count = input.size();
		for (Anima::AInt i = 0; i < count; i++)
		{
			Anima::AnimaVertex3f vertex = input[i];
			result += std::to_string(vertex.x) + ";" + std::to_string(vertex.y) + ";" + std::to_string(vertex.z) + ";";
		}

		return result;
	}
};

struct AnimaXmlVertex2ArrayTranslator
{
	typedef Anima::AnimaString internal_type;
	typedef Anima::AnimaArray<Anima::AnimaVertex2f> external_type;

	boost::optional<external_type> get_value(const internal_type& str)
	{
		external_type result;

		if (!str.empty())
		{
			Anima::AnimaString inputString;
			if (str[str.length() - 1] == ';')
				inputString = str.substr(0, str.length() - 1);
			else
				inputString = str;

			std::vector<Anima::AnimaString> elements;
			boost::split(elements, inputString, boost::is_any_of(";"));

			size_t elementsSize = elements.size();

			if (elementsSize % 2 != 0)
				return boost::optional<external_type>(boost::none);

			for (Anima::AInt i = 0; i < elementsSize; i += 2)
			{
				Anima::AnimaVertex2f vertex;
				vertex.x = (Anima::AFloat)atof(elements[i].c_str());
				vertex.y = (Anima::AFloat)atof(elements[i + 1].c_str());

				result.push_back(vertex);
			}
		}

		return result;
	}

	boost::optional<internal_type> put_value(const external_type& input)
	{
		internal_type result = "";

		Anima::AInt count = input.size();
		for (Anima::AInt i = 0; i < count; i++)
		{
			Anima::AnimaVertex2f vertex = input[i];
			result += std::to_string(vertex.x) + ";" + std::to_string(vertex.y) + ";";
		}

		return result;
	}
};

struct AnimaXmlBoolTranslator
{
	typedef Anima::AnimaString internal_type;
	typedef bool external_type;

	boost::optional<external_type> get_value(const internal_type& str)
	{
		if (!str.empty())
		{
			using boost::algorithm::iequals;

			if (iequals(str, "true") || iequals(str, "yes") || str == "1")
				return boost::optional<external_type>(true);
			else
				return boost::optional<external_type>(false);
		}
		else
			return boost::optional<external_type>(boost::none);
	}

	boost::optional<internal_type> put_value(const external_type& b)
	{
		return boost::optional<internal_type>(b ? "true" : "false");
	}
};

struct AnimaXmlShaderDataTypeTranslator
{
	typedef Anima::AnimaString internal_type;
	typedef Anima::AnimaShaderDataType external_type;

	boost::optional<external_type> get_value(const internal_type& str)
	{
		if (!str.empty())
		{
			using boost::algorithm::iequals;

			if (iequals(str, "FLOAT"))					return Anima::ASDT_FLOAT;
			else if (iequals(str, "FLOAT_ARRAY"))		return Anima::ASDT_FLOAT_ARRAY;
			else if (iequals(str, "FLOAT2"))			return Anima::ASDT_FLOAT2;
			else if (iequals(str, "FLOAT2_ARRAY"))		return Anima::ASDT_FLOAT2_ARRAY;
			else if (iequals(str, "FLOAT3"))			return Anima::ASDT_FLOAT3;
			else if (iequals(str, "FLOAT3_ARRAY"))		return Anima::ASDT_FLOAT3_ARRAY;
			else if (iequals(str, "FLOAT4"))			return Anima::ASDT_FLOAT4;
			else if (iequals(str, "FLOAT4_ARRAY"))		return Anima::ASDT_FLOAT4_ARRAY;
			else if (iequals(str, "MATRIX4x4"))			return Anima::ASDT_MATRIX4x4;
			else if (iequals(str, "MATRIX4x4_ARRAY"))	return Anima::ASDT_MATRIX4x4_ARRAY;
			else if (iequals(str, "MATRIX3x3"))			return Anima::ASDT_MATRIX3x3;
			else if (iequals(str, "MATRIX3x3_ARRAY"))	return Anima::ASDT_MATRIX3x3_ARRAY;
			else if (iequals(str, "INT"))				return Anima::ASDT_INT;
			else if (iequals(str, "INT_ARRAY"))			return Anima::ASDT_INT_ARRAY;
			else if (iequals(str, "BOOL"))				return Anima::ASDT_BOOL;
			else if (iequals(str, "BOOL_ARRAY"))		return Anima::ASDT_BOOL_ARRAY;
			else if (iequals(str, "TEXTURE2D"))			return Anima::ASDT_TEXTURE2D;
			else if (iequals(str, "TEXTURE2D_ARRAY"))	return Anima::ASDT_TEXTURE2D_ARRAY;
			else if (iequals(str, "TEXTURECUBE"))		return Anima::ASDT_TEXTURECUBE;
			else if (iequals(str, "TEXTURECUBE_ARRAY"))	return Anima::ASDT_TEXTURECUBE_ARRAY;
			else if (iequals(str, "TEXTURE3D"))			return Anima::ASDT_TEXTURE3D;
			else if (iequals(str, "TEXTURE3D_ARRAY"))	return Anima::ASDT_TEXTURE3D_ARRAY;

			return Anima::ASDT_NONE;
		}
		else
			return boost::optional<external_type>(boost::none);
	}

	boost::optional<internal_type> put_value(const external_type& type)
	{
		switch (type)
		{
		case Anima::ASDT_FLOAT:				return Anima::AnimaString("FLOAT"); break;
		case Anima::ASDT_FLOAT_ARRAY:		return Anima::AnimaString("FLOAT_ARRAY"); break;
		case Anima::ASDT_FLOAT2:			return Anima::AnimaString("FLOAT2"); break;
		case Anima::ASDT_FLOAT2_ARRAY:		return Anima::AnimaString("FLOAT2_ARRAY"); break;
		case Anima::ASDT_FLOAT3:			return Anima::AnimaString("FLOAT3"); break;
		case Anima::ASDT_FLOAT3_ARRAY:		return Anima::AnimaString("FLOAT3_ARRAY"); break;
		case Anima::ASDT_FLOAT4:			return Anima::AnimaString("FLOAT4"); break;
		case Anima::ASDT_FLOAT4_ARRAY:		return Anima::AnimaString("FLOAT4_ARRAY"); break;
		case Anima::ASDT_MATRIX4x4:			return Anima::AnimaString("MATRIX4x4"); break;
		case Anima::ASDT_MATRIX4x4_ARRAY:	return Anima::AnimaString("MATRIX4x4_ARRAY"); break;
		case Anima::ASDT_MATRIX3x3:			return Anima::AnimaString("MATRIX3x3"); break;
		case Anima::ASDT_MATRIX3x3_ARRAY:	return Anima::AnimaString("MATRIX3x3_ARRAY"); break;
		case Anima::ASDT_INT:				return Anima::AnimaString("INT"); break;
		case Anima::ASDT_INT_ARRAY:			return Anima::AnimaString("INT_ARRAY"); break;
		case Anima::ASDT_BOOL:				return Anima::AnimaString("BOOL"); break;
		case Anima::ASDT_BOOL_ARRAY:		return Anima::AnimaString("BOOL_ARRAY"); break;
		case Anima::ASDT_TEXTURE2D:			return Anima::AnimaString("TEXTURE2D"); break;
		case Anima::ASDT_TEXTURE2D_ARRAY:	return Anima::AnimaString("TEXTURE2D_ARRAY"); break;
		case Anima::ASDT_TEXTURECUBE:		return Anima::AnimaString("TEXTURECUBE"); break;
		case Anima::ASDT_TEXTURECUBE_ARRAY:	return Anima::AnimaString("TEXTURECUBE_ARRAY"); break;
		case Anima::ASDT_TEXTURE3D:			return Anima::AnimaString("TEXTURE3D"); break;
		case Anima::ASDT_TEXTURE3D_ARRAY:	return Anima::AnimaString("TEXTURE3D_ARRAY"); break;
		case Anima::ASDT_NONE:				return Anima::AnimaString("NONE"); break;
		}

		return Anima::AnimaString("NONE");
	}
};

struct AnimaXmlShaderDataSourceObjectTranslator
{
	typedef Anima::AnimaString internal_type;
	typedef Anima::AnimaShaderDataSourceObject external_type;
	
	boost::optional<external_type> get_value(const internal_type& str)
	{
		if (!str.empty())
		{
			using boost::algorithm::iequals;
			
			if (iequals(str, "GEOMETRY"))		return Anima::ASDSO_GEOMETRY;
			else if (iequals(str, "NODE"))		return Anima::ASDSO_NODE;
			else if (iequals(str, "RENDERER"))	return Anima::ASDSO_RENDERER;
			else if (iequals(str, "MATERIAL"))	return Anima::ASDSO_MATERIAL;
			else if (iequals(str, "LIGHT"))		return Anima::ASDSO_LIGHT;
			else if (iequals(str, "CAMERA"))	return Anima::ASDSO_CAMERA;
			else if (iequals(str, "GBUFFER"))	return Anima::ASDSO_GBUFFER;
			else if (iequals(str, "NONE"))		return Anima::ASDSO_NONE;
			
			return Anima::ASDSO_NONE;
		}
		else
			return boost::optional<external_type>(boost::none);
	}
	
	boost::optional<internal_type> put_value(const external_type& type)
	{
		switch (type)
		{
			case Anima::ASDSO_GEOMETRY:	return Anima::AnimaString("GEOMETRY"); break;
			case Anima::ASDSO_NODE:		return Anima::AnimaString("NODE"); break;
			case Anima::ASDSO_RENDERER:	return Anima::AnimaString("RENDERER"); break;
			case Anima::ASDSO_LIGHT:	return Anima::AnimaString("LIGHT"); break;
			case Anima::ASDSO_MATERIAL:	return Anima::AnimaString("MATERIAL"); break;
			case Anima::ASDSO_CAMERA:	return Anima::AnimaString("CAMERA"); break;
			case Anima::ASDSO_GBUFFER:	return Anima::AnimaString("GBUFFER"); break;
			case Anima::ASDSO_NONE:		return Anima::AnimaString("NONE"); break;
		}
		
		return Anima::AnimaString("NONE");
	}
};

struct AnimaXmlTextureTargetTranslator
{
	typedef Anima::AnimaString internal_type;
	typedef Anima::AnimaTextureTarget external_type;

	boost::optional<external_type> get_value(const internal_type& str)
	{
		if (!str.empty())
		{
			using boost::algorithm::iequals;

			if (iequals(str, "CUBE"))		return Anima::TEXTURE_TARGET_CUBE;
			else if (iequals(str, "2D"))	return Anima::TEXTURE_TARGET_2D;
			else if (iequals(str, "3D"))	return Anima::TEXTURE_TARGET_3D;

			return boost::optional<external_type>(boost::none);
		}
		else
			return boost::optional<external_type>(boost::none);
	}

	boost::optional<internal_type> put_value(const external_type& type)
	{
		switch (type)
		{
		case Anima::TEXTURE_TARGET_CUBE:	return Anima::AnimaString("CUBE"); break;
		case Anima::TEXTURE_TARGET_2D:		return Anima::AnimaString("2D"); break;
		case Anima::TEXTURE_TARGET_3D:		return Anima::AnimaString("3D"); break;
		case Anima::TEXTURE_TARGET_NONE:	return Anima::AnimaString("NONE"); break;
		}

		return boost::optional<internal_type>(boost::none);
	}
};

struct AnimaXmlTextureMinFilterModeTranslator
{
	typedef Anima::AnimaString internal_type;
	typedef Anima::AnimaTextureMinFilterMode external_type;

	boost::optional<external_type> get_value(const internal_type& str)
	{
		if (!str.empty())
		{
			using boost::algorithm::iequals;

			if (iequals(str, "NEAREST"))						return Anima::TEXTURE_MIN_FILTER_MODE_NEAREST;
			else if (iequals(str, "LINEAR"))					return Anima::TEXTURE_MIN_FILTER_MODE_LINEAR;
			else if (iequals(str, "NEAREST_MIPMAP_NEAREST"))	return Anima::TEXTURE_MIN_FILTER_MODE_NEAREST_MIPMAP_NEAREST;
			else if (iequals(str, "NEAREST_MIPMAP_LINEAR"))		return Anima::TEXTURE_MIN_FILTER_MODE_NEAREST_MIPMAP_LINEAR;
			else if (iequals(str, "LINEAR_MIPMAP_NEAREST"))		return Anima::TEXTURE_MIN_FILTER_MODE_LINEAR_MIPMAP_NEAREST;
			else if (iequals(str, "LINEAR_MIPMAP_LINEAR"))		return Anima::TEXTURE_MIN_FILTER_MODE_LINEAR_MIPMAP_LINEAR;

			return boost::optional<external_type>(boost::none);
		}
		else
			return boost::optional<external_type>(boost::none);
	}

	boost::optional<internal_type> put_value(const external_type& type)
	{
		switch (type)
		{
		case Anima::TEXTURE_MIN_FILTER_MODE_NEAREST:				return Anima::AnimaString("NEAREST"); break;
		case Anima::TEXTURE_MIN_FILTER_MODE_LINEAR:					return Anima::AnimaString("LINEAR"); break;
		case Anima::TEXTURE_MIN_FILTER_MODE_NEAREST_MIPMAP_NEAREST:	return Anima::AnimaString("NEAREST_MIPMAP_NEAREST"); break;
		case Anima::TEXTURE_MIN_FILTER_MODE_NEAREST_MIPMAP_LINEAR:	return Anima::AnimaString("NEAREST_MIPMAP_LINEAR"); break;
		case Anima::TEXTURE_MIN_FILTER_MODE_LINEAR_MIPMAP_NEAREST:	return Anima::AnimaString("LINEAR_MIPMAP_NEAREST"); break;
		case Anima::TEXTURE_MIN_FILTER_MODE_LINEAR_MIPMAP_LINEAR:	return Anima::AnimaString("LINEAR_MIPMAP_LINEAR"); break;
		}

		return boost::optional<internal_type>(boost::none);
	}
};

struct AnimaXmlTextureMagFilterModeTranslator
{
	typedef Anima::AnimaString internal_type;
	typedef Anima::AnimaTextureMagFilterMode external_type;

	boost::optional<external_type> get_value(const internal_type& str)
	{
		if (!str.empty())
		{
			using boost::algorithm::iequals;

			if (iequals(str, "NEAREST"))		return Anima::TEXTURE_MAG_FILTER_MODE_NEAREST;
			else if (iequals(str, "LINEAR"))	return Anima::TEXTURE_MAG_FILTER_MODE_LINEAR;

			return boost::optional<external_type>(boost::none);
		}
		else
			return boost::optional<external_type>(boost::none);
	}

	boost::optional<internal_type> put_value(const external_type& type)
	{
		switch (type)
		{
		case Anima::TEXTURE_MAG_FILTER_MODE_NEAREST:	return Anima::AnimaString("NEAREST"); break;
		case Anima::TEXTURE_MAG_FILTER_MODE_LINEAR:		return Anima::AnimaString("LINEAR"); break;
		}

		return boost::optional<internal_type>(boost::none);
	}
};

struct AnimaXmlTextureDataTypeTranslator
{
	typedef Anima::AnimaString internal_type;
	typedef Anima::AnimaTextureDataType external_type;

	boost::optional<external_type> get_value(const internal_type& str)
	{
		if (!str.empty())
		{
			using boost::algorithm::iequals;

			if (iequals(str, "UBYTE"))			return Anima::TEXTURE_DATA_TYPE_UNSIGNED_BYTE;
			else if (iequals(str, "BYTE"))		return Anima::TEXTURE_DATA_TYPE_BYTE;
			else if (iequals(str, "USHORT"))	return Anima::TEXTURE_DATA_TYPE_UNSIGNED_SHORT;
			else if (iequals(str, "SHORT"))		return Anima::TEXTURE_DATA_TYPE_SHORT;
			else if (iequals(str, "UINT"))		return Anima::TEXTURE_DATA_TYPE_UNSIGNED_INT;
			else if (iequals(str, "INT"))		return Anima::TEXTURE_DATA_TYPE_INT;
			else if (iequals(str, "FLOAT"))		return Anima::TEXTURE_DATA_TYPE_FLOAT;

			return boost::optional<external_type>(boost::none);
		}
		else
			return boost::optional<external_type>(boost::none);
	}

	boost::optional<internal_type> put_value(const external_type& type)
	{
		switch (type)
		{
		case Anima::TEXTURE_DATA_TYPE_UNSIGNED_BYTE:	return Anima::AnimaString("UBYTE"); break;
		case Anima::TEXTURE_DATA_TYPE_BYTE:				return Anima::AnimaString("BYTE"); break;
		case Anima::TEXTURE_DATA_TYPE_UNSIGNED_SHORT:	return Anima::AnimaString("USHORT"); break;
		case Anima::TEXTURE_DATA_TYPE_SHORT:			return Anima::AnimaString("SHORT"); break;
		case Anima::TEXTURE_DATA_TYPE_UNSIGNED_INT:		return Anima::AnimaString("UINT"); break;
		case Anima::TEXTURE_DATA_TYPE_INT:				return Anima::AnimaString("INT"); break;
		case Anima::TEXTURE_DATA_TYPE_FLOAT:			return Anima::AnimaString("FLOAT"); break;
		}

		return boost::optional<internal_type>(boost::none);
	}
};

struct AnimaXmlTextureClampModeTranslator
{
	typedef Anima::AnimaString internal_type;
	typedef Anima::AnimaTextureClampMode external_type;

	boost::optional<external_type> get_value(const internal_type& str)
	{
		if (!str.empty())
		{
			using boost::algorithm::iequals;

			if (iequals(str, "REPEAT"))					return Anima::TEXTURE_CLAMP_REPEAT;
			else if (iequals(str, "MIRRORED_REPEAT"))	return Anima::TEXTURE_CLAMP_MIRRORED_REPEAT;
			else if (iequals(str, "TO_EDGE"))			return Anima::TEXTURE_CLAMP_TO_EDGE;
			else if (iequals(str, "TO_BORDER"))			return Anima::TEXTURE_CLAMP_TO_BORDER;

			return boost::optional<external_type>(boost::none);
		}
		else
			return boost::optional<external_type>(boost::none);
	}

	boost::optional<internal_type> put_value(const external_type& type)
	{
		switch (type)
		{
		case Anima::TEXTURE_CLAMP_REPEAT:			return Anima::AnimaString("REPEAT"); break;
		case Anima::TEXTURE_CLAMP_MIRRORED_REPEAT:	return Anima::AnimaString("MIRRORED_REPEAT"); break;
		case Anima::TEXTURE_CLAMP_TO_EDGE:			return Anima::AnimaString("TO_EDGE"); break;
		case Anima::TEXTURE_CLAMP_TO_BORDER:		return Anima::AnimaString("TO_BORDER"); break;
		}

		return boost::optional<internal_type>(boost::none);
	}
};

struct AnimaXmlTextureFormatTranslator
{
	typedef Anima::AnimaString internal_type;
	typedef Anima::AnimaTextureFormat external_type;

	boost::optional<external_type> get_value(const internal_type& str)
	{
		if (!str.empty())
		{
			using boost::algorithm::iequals;

			if (iequals(str, "RED"))				return Anima::TEXTURE_FORMAT_RED;
			else if (iequals(str, "GREEN"))			return Anima::TEXTURE_FORMAT_GREEN;
			else if (iequals(str, "BLUE"))			return Anima::TEXTURE_FORMAT_BLUE;
			else if (iequals(str, "RG"))			return Anima::TEXTURE_FORMAT_RG;
			else if (iequals(str, "RGB"))			return Anima::TEXTURE_FORMAT_RGB;
			else if (iequals(str, "RGBA"))			return Anima::TEXTURE_FORMAT_RGBA;
			else if (iequals(str, "BGR"))			return Anima::TEXTURE_FORMAT_BGR;
			else if (iequals(str, "BGRA"))			return Anima::TEXTURE_FORMAT_BGRA;
			else if (iequals(str, "RED_INT"))		return Anima::TEXTURE_FORMAT_RED_INT;
			else if (iequals(str, "GREEN_INT"))		return Anima::TEXTURE_FORMAT_GREEN_INT;
			else if (iequals(str, "BLUE_INT"))		return Anima::TEXTURE_FORMAT_BLUE_INT;
			else if (iequals(str, "RG_INT"))		return Anima::TEXTURE_FORMAT_RG_INT;
			else if (iequals(str, "RGB_INT"))		return Anima::TEXTURE_FORMAT_RGB_INT;
			else if (iequals(str, "RGBA_INT"))		return Anima::TEXTURE_FORMAT_RGBA_INT;
			else if (iequals(str, "BGR_INT"))		return Anima::TEXTURE_FORMAT_BGR_INT;
			else if (iequals(str, "BGRA_INT"))		return Anima::TEXTURE_FORMAT_BGRA_INT;
			else if (iequals(str, "DEPTH"))			return Anima::TEXTURE_FORMAT_DEPTH;
			else if (iequals(str, "DEPTH_STENCIL"))	return Anima::TEXTURE_FORMAT_DEPTH_STENCIL;
			else if (iequals(str, "STENCIL"))		return Anima::TEXTURE_FORMAT_STENCIL;
			else if (iequals(str, "DXT1"))			return Anima::TEXTURE_FORMAT_COMPRESSED_RGBA_S3TC_DXT1_EXT;
			else if (iequals(str, "DXT3"))			return Anima::TEXTURE_FORMAT_COMPRESSED_RGBA_S3TC_DXT3_EXT;
			else if (iequals(str, "DXT5"))			return Anima::TEXTURE_FORMAT_COMPRESSED_RGBA_S3TC_DXT5_EXT;

			return Anima::TEXTURE_FORMAT_NONE;
		}
		else
			return boost::optional<external_type>(boost::none);
	}

	boost::optional<internal_type> put_value(const external_type& type)
	{
		switch (type)
		{
		case Anima::TEXTURE_FORMAT_NONE:							return Anima::AnimaString("NONE"); break;
		case Anima::TEXTURE_FORMAT_RED:								return Anima::AnimaString("RED"); break;
		case Anima::TEXTURE_FORMAT_GREEN:							return Anima::AnimaString("GREEN"); break;
		case Anima::TEXTURE_FORMAT_BLUE:							return Anima::AnimaString("BLUE"); break;
		case Anima::TEXTURE_FORMAT_RG:								return Anima::AnimaString("RG"); break;
		case Anima::TEXTURE_FORMAT_RGB:								return Anima::AnimaString("RGB"); break;
		case Anima::TEXTURE_FORMAT_RGBA:							return Anima::AnimaString("RGBA"); break;
		case Anima::TEXTURE_FORMAT_BGR:								return Anima::AnimaString("BGR"); break;
		case Anima::TEXTURE_FORMAT_BGRA:							return Anima::AnimaString("BGRA"); break;
		case Anima::TEXTURE_FORMAT_RED_INT:							return Anima::AnimaString("RED_INT"); break;
		case Anima::TEXTURE_FORMAT_GREEN_INT:						return Anima::AnimaString("GREEN_INT"); break;
		case Anima::TEXTURE_FORMAT_BLUE_INT:						return Anima::AnimaString("BLUE_INT"); break;
		case Anima::TEXTURE_FORMAT_RG_INT:							return Anima::AnimaString("RG_INT"); break;
		case Anima::TEXTURE_FORMAT_RGB_INT:							return Anima::AnimaString("RGB_INT"); break;
		case Anima::TEXTURE_FORMAT_RGBA_INT:						return Anima::AnimaString("RGBA_INT"); break;
		case Anima::TEXTURE_FORMAT_BGR_INT:							return Anima::AnimaString("BGR_INT"); break;
		case Anima::TEXTURE_FORMAT_BGRA_INT:						return Anima::AnimaString("BGRA_INT"); break;
		case Anima::TEXTURE_FORMAT_DEPTH:							return Anima::AnimaString("DEPTH"); break;
		case Anima::TEXTURE_FORMAT_DEPTH_STENCIL:					return Anima::AnimaString("DEPTH_STENCIL"); break;
		case Anima::TEXTURE_FORMAT_STENCIL:							return Anima::AnimaString("STENCIL"); break;
		case Anima::TEXTURE_FORMAT_COMPRESSED_RGBA_S3TC_DXT1_EXT:	return Anima::AnimaString("DXT1"); break;
		case Anima::TEXTURE_FORMAT_COMPRESSED_RGBA_S3TC_DXT3_EXT:	return Anima::AnimaString("DXT3"); break;
		case Anima::TEXTURE_FORMAT_COMPRESSED_RGBA_S3TC_DXT5_EXT:	return Anima::AnimaString("DXT5"); break;
		}

		return boost::optional<internal_type>(boost::none);
	}
};

struct AnimaXmlTextureAttachmentTranslator
{
	typedef Anima::AnimaString internal_type;
	typedef Anima::AnimaTextureAttachment external_type;
	
	boost::optional<external_type> get_value(const internal_type& str)
	{
		if (!str.empty())
		{
			using boost::algorithm::iequals;
			
			if (iequals(str, "DEPTH"))				return Anima::TEXTURE_ATTACHMENT_DEPTH;
			else if (iequals(str, "STENCIL"))		return Anima::TEXTURE_ATTACHMENT_STENCIL;
			else if (iequals(str, "DEPTH_STENCIL"))	return Anima::TEXTURE_ATTACHMENT_DEPTH_STENCIL;
			else if (iequals(str, "COLOR_0"))		return Anima::TEXTURE_ATTACHMENT_COLOR0;
			else if (iequals(str, "COLOR_1"))		return Anima::TEXTURE_ATTACHMENT_COLOR1;
			else if (iequals(str, "COLOR_2"))		return Anima::TEXTURE_ATTACHMENT_COLOR2;
			else if (iequals(str, "COLOR_3"))		return Anima::TEXTURE_ATTACHMENT_COLOR3;
			else if (iequals(str, "COLOR_4"))		return Anima::TEXTURE_ATTACHMENT_COLOR4;
			else if (iequals(str, "COLOR_5"))		return Anima::TEXTURE_ATTACHMENT_COLOR5;
			else if (iequals(str, "COLOR_6"))		return Anima::TEXTURE_ATTACHMENT_COLOR6;
			else if (iequals(str, "COLOR_7"))		return Anima::TEXTURE_ATTACHMENT_COLOR7;
			else if (iequals(str, "COLOR_8"))		return Anima::TEXTURE_ATTACHMENT_COLOR8;
			else if (iequals(str, "COLOR_9"))		return Anima::TEXTURE_ATTACHMENT_COLOR9;
			else if (iequals(str, "COLOR_10"))		return Anima::TEXTURE_ATTACHMENT_COLOR10;
			else if (iequals(str, "COLOR_11"))		return Anima::TEXTURE_ATTACHMENT_COLOR11;
			else if (iequals(str, "COLOR_12"))		return Anima::TEXTURE_ATTACHMENT_COLOR12;
			else if (iequals(str, "COLOR_13"))		return Anima::TEXTURE_ATTACHMENT_COLOR13;
			else if (iequals(str, "COLOR_14"))		return Anima::TEXTURE_ATTACHMENT_COLOR14;
			else if (iequals(str, "COLOR_15"))		return Anima::TEXTURE_ATTACHMENT_COLOR15;
			
			return Anima::TEXTURE_ATTACHMENT_NONE;
		}
		else
			return boost::optional<external_type>(boost::none);
	}
	
	boost::optional<internal_type> put_value(const external_type& type)
	{
		switch (type)
		{
			case Anima::TEXTURE_ATTACHMENT_DEPTH:			return Anima::AnimaString("DEPTH"); break;
			case Anima::TEXTURE_ATTACHMENT_STENCIL:			return Anima::AnimaString("STENCIL"); break;
			case Anima::TEXTURE_ATTACHMENT_DEPTH_STENCIL:	return Anima::AnimaString("DEPTH_STENCIL"); break;
			case Anima::TEXTURE_ATTACHMENT_COLOR0:			return Anima::AnimaString("COLOR_0"); break;
			case Anima::TEXTURE_ATTACHMENT_COLOR1:			return Anima::AnimaString("COLOR_1"); break;
			case Anima::TEXTURE_ATTACHMENT_COLOR2:			return Anima::AnimaString("COLOR_2"); break;
			case Anima::TEXTURE_ATTACHMENT_COLOR3:			return Anima::AnimaString("COLOR_3"); break;
			case Anima::TEXTURE_ATTACHMENT_COLOR4:			return Anima::AnimaString("COLOR_4"); break;
			case Anima::TEXTURE_ATTACHMENT_COLOR5:			return Anima::AnimaString("COLOR_5"); break;
			case Anima::TEXTURE_ATTACHMENT_COLOR6:			return Anima::AnimaString("COLOR_6"); break;
			case Anima::TEXTURE_ATTACHMENT_COLOR7:			return Anima::AnimaString("COLOR_7"); break;
			case Anima::TEXTURE_ATTACHMENT_COLOR8:			return Anima::AnimaString("COLOR_8"); break;
			case Anima::TEXTURE_ATTACHMENT_COLOR9:			return Anima::AnimaString("COLOR_9"); break;
			case Anima::TEXTURE_ATTACHMENT_COLOR10:			return Anima::AnimaString("COLOR_10"); break;
			case Anima::TEXTURE_ATTACHMENT_COLOR11:			return Anima::AnimaString("COLOR_11"); break;
			case Anima::TEXTURE_ATTACHMENT_COLOR12:			return Anima::AnimaString("COLOR_12"); break;
			case Anima::TEXTURE_ATTACHMENT_COLOR13:			return Anima::AnimaString("COLOR_13"); break;
			case Anima::TEXTURE_ATTACHMENT_COLOR14:			return Anima::AnimaString("COLOR_14"); break;
			case Anima::TEXTURE_ATTACHMENT_COLOR15:			return Anima::AnimaString("COLOR_15"); break;
			case Anima::TEXTURE_ATTACHMENT_NONE:			return Anima::AnimaString("NONE"); break;
		}
		
		return boost::optional<internal_type>(boost::none);
	}
};

struct AnimaXmlTextureInternalFormatTranslator
{
	typedef Anima::AnimaString internal_type;
	typedef Anima::AnimaTextureInternalFormat external_type;

	boost::optional<external_type> get_value(const internal_type& str)
	{
		if (!str.empty())
		{
			using boost::algorithm::iequals;

			if (iequals(str, "R8"))						return Anima::TEXTURE_INTERNAL_FORMAT_R8;
			else if (iequals(str, "R8_SNORM"))			return Anima::TEXTURE_INTERNAL_FORMAT_R8_SNORM;
			else if (iequals(str, "R16"))				return Anima::TEXTURE_INTERNAL_FORMAT_R16;
			else if (iequals(str, "R16_SNORM"))			return Anima::TEXTURE_INTERNAL_FORMAT_R16_SNORM;
			else if (iequals(str, "RG8"))				return Anima::TEXTURE_INTERNAL_FORMAT_RG8;
			else if (iequals(str, "RG8_SNORM"))			return Anima::TEXTURE_INTERNAL_FORMAT_RG8_SNORM;
			else if (iequals(str, "RG16"))				return Anima::TEXTURE_INTERNAL_FORMAT_RG16;
			else if (iequals(str, "RG16_SNORM"))		return Anima::TEXTURE_INTERNAL_FORMAT_RG16_SNORM;
			else if (iequals(str, "R3_G3_B2"))			return Anima::TEXTURE_INTERNAL_FORMAT_R3_G3_B2;
			else if (iequals(str, "RGB4"))				return Anima::TEXTURE_INTERNAL_FORMAT_RGB4;
			else if (iequals(str, "RGB5"))				return Anima::TEXTURE_INTERNAL_FORMAT_RGB5;
			else if (iequals(str, "RGB565"))			return Anima::TEXTURE_INTERNAL_FORMAT_RGB565;
			else if (iequals(str, "RGB8"))				return Anima::TEXTURE_INTERNAL_FORMAT_RGB8;
			else if (iequals(str, "RGB8_SNORM"))		return Anima::TEXTURE_INTERNAL_FORMAT_RGB8_SNORM;
			else if (iequals(str, "RGB10"))				return Anima::TEXTURE_INTERNAL_FORMAT_RGB10;
			else if (iequals(str, "RGB12"))				return Anima::TEXTURE_INTERNAL_FORMAT_RGB12;
			else if (iequals(str, "RGB16"))				return Anima::TEXTURE_INTERNAL_FORMAT_RGB16;
			else if (iequals(str, "RGB16_SNORM"))		return Anima::TEXTURE_INTERNAL_FORMAT_RGB16_SNORM;
			else if (iequals(str, "RGBA2"))				return Anima::TEXTURE_INTERNAL_FORMAT_RGBA2;
			else if (iequals(str, "RGBA4"))				return Anima::TEXTURE_INTERNAL_FORMAT_RGBA4;
			else if (iequals(str, "RGB5_A1"))			return Anima::TEXTURE_INTERNAL_FORMAT_RGB5_A1;
			else if (iequals(str, "RGBA8"))				return Anima::TEXTURE_INTERNAL_FORMAT_RGBA8;
			else if (iequals(str, "RGBA8_SNORM"))		return Anima::TEXTURE_INTERNAL_FORMAT_RGBA8_SNORM;
			else if (iequals(str, "RGB10_A2"))			return Anima::TEXTURE_INTERNAL_FORMAT_RGB10_A2;
			else if (iequals(str, "RGB10_A2UI"))		return Anima::TEXTURE_INTERNAL_FORMAT_RGB10_A2UI;
			else if (iequals(str, "RGBA12"))			return Anima::TEXTURE_INTERNAL_FORMAT_RGBA12;
			else if (iequals(str, "RGBA16"))			return Anima::TEXTURE_INTERNAL_FORMAT_RGBA16;
			else if (iequals(str, "RGBA16_SNORM"))		return Anima::TEXTURE_INTERNAL_FORMAT_RGBA16_SNORM;
			else if (iequals(str, "SRGB8"))				return Anima::TEXTURE_INTERNAL_FORMAT_SRGB8;
			else if (iequals(str, "SRGB8_A8"))			return Anima::TEXTURE_INTERNAL_FORMAT_SRGB8_A8;
			else if (iequals(str, "R16F"))				return Anima::TEXTURE_INTERNAL_FORMAT_R16F;
			else if (iequals(str, "RG16F"))				return Anima::TEXTURE_INTERNAL_FORMAT_RG16F;
			else if (iequals(str, "RGB16F"))			return Anima::TEXTURE_INTERNAL_FORMAT_RGB16F;
			else if (iequals(str, "RGBA16F"))			return Anima::TEXTURE_INTERNAL_FORMAT_RGBA16F;
			else if (iequals(str, "R32F"))				return Anima::TEXTURE_INTERNAL_FORMAT_R32F;
			else if (iequals(str, "RG32F"))				return Anima::TEXTURE_INTERNAL_FORMAT_RG32F;
			else if (iequals(str, "RGB32F"))			return Anima::TEXTURE_INTERNAL_FORMAT_RGB32F;
			else if (iequals(str, "RGBA32F"))			return Anima::TEXTURE_INTERNAL_FORMAT_RGBA32F;
			else if (iequals(str, "R11F_G11F_B10F"))	return Anima::TEXTURE_INTERNAL_FORMAT_R11F_G11F_B10F;
			else if (iequals(str, "RGB9_E5"))			return Anima::TEXTURE_INTERNAL_FORMAT_RGB9_E5;
			else if (iequals(str, "R8I"))				return Anima::TEXTURE_INTERNAL_FORMAT_R8I;
			else if (iequals(str, "R8UI"))				return Anima::TEXTURE_INTERNAL_FORMAT_R8UI;
			else if (iequals(str, "R16I"))				return Anima::TEXTURE_INTERNAL_FORMAT_R16I;
			else if (iequals(str, "R16UI"))				return Anima::TEXTURE_INTERNAL_FORMAT_R16UI;
			else if (iequals(str, "R32I"))				return Anima::TEXTURE_INTERNAL_FORMAT_R32I;
			else if (iequals(str, "R32UI"))				return Anima::TEXTURE_INTERNAL_FORMAT_R32UI;
			else if (iequals(str, "RG8I"))				return Anima::TEXTURE_INTERNAL_FORMAT_RG8I;
			else if (iequals(str, "RG8UI"))				return Anima::TEXTURE_INTERNAL_FORMAT_RG8UI;
			else if (iequals(str, "RG16I"))				return Anima::TEXTURE_INTERNAL_FORMAT_RG16I;
			else if (iequals(str, "RG16UI"))			return Anima::TEXTURE_INTERNAL_FORMAT_RG16UI;
			else if (iequals(str, "RG32I"))				return Anima::TEXTURE_INTERNAL_FORMAT_RG32I;
			else if (iequals(str, "RG32UI"))			return Anima::TEXTURE_INTERNAL_FORMAT_RG32UI;
			else if (iequals(str, "RGB8I"))				return Anima::TEXTURE_INTERNAL_FORMAT_RGB8I;
			else if (iequals(str, "RGB8UI"))			return Anima::TEXTURE_INTERNAL_FORMAT_RGB8UI;
			else if (iequals(str, "RGB16I"))			return Anima::TEXTURE_INTERNAL_FORMAT_RGB16I;
			else if (iequals(str, "RGB16UI"))			return Anima::TEXTURE_INTERNAL_FORMAT_RGB16UI;
			else if (iequals(str, "RGB32I"))			return Anima::TEXTURE_INTERNAL_FORMAT_RGB32I;
			else if (iequals(str, "RGB32UI"))			return Anima::TEXTURE_INTERNAL_FORMAT_RGB32UI;
			else if (iequals(str, "RGBA8I"))			return Anima::TEXTURE_INTERNAL_FORMAT_RGBA8I;
			else if (iequals(str, "RGBA8UI"))			return Anima::TEXTURE_INTERNAL_FORMAT_RGBA8UI;
			else if (iequals(str, "RGBA16I"))			return Anima::TEXTURE_INTERNAL_FORMAT_RGBA16I;
			else if (iequals(str, "RGBA16UI"))			return Anima::TEXTURE_INTERNAL_FORMAT_RGBA16UI;
			else if (iequals(str, "RGBA32I"))			return Anima::TEXTURE_INTERNAL_FORMAT_RGBA32I;
			else if (iequals(str, "RGBA32UI"))			return Anima::TEXTURE_INTERNAL_FORMAT_RGBA32UI;
			else if (iequals(str, "DEPTH"))				return Anima::TEXTURE_INTERNAL_FORMAT_DEPTH_COMPONENT;
			else if (iequals(str, "DEPTH_STENCIL"))		return Anima::TEXTURE_INTERNAL_FORMAT_DEPTH_STENCIL;
			else if (iequals(str, "STENCIL"))			return Anima::TEXTURE_INTERNAL_FORMAT_STENCIL;
			else if (iequals(str, "DEPTH16"))			return Anima::TEXTURE_INTERNAL_FORMAT_DEPTH16;
			else if (iequals(str, "DEPTH24"))			return Anima::TEXTURE_INTERNAL_FORMAT_DEPTH24;
			else if (iequals(str, "DEPTH32"))			return Anima::TEXTURE_INTERNAL_FORMAT_DEPTH32;
			else if (iequals(str, "DEPTH32F"))			return Anima::TEXTURE_INTERNAL_FORMAT_DEPTH32F;
			else if (iequals(str, "DEPTH24_STENCIL8"))	return Anima::TEXTURE_INTERNAL_FORMAT_DEPTH24_STENCIL8;
			else if (iequals(str, "DEPTH32F_STENCIL8"))	return Anima::TEXTURE_INTERNAL_FORMAT_DEPTH32F_STENCIL8;
			else if (iequals(str, "STENCIL8"))			return Anima::TEXTURE_INTERNAL_FORMAT_STENCIL8;
			else if (iequals(str, "RED"))				return Anima::TEXTURE_INTERNAL_FORMAT_RED;
			else if (iequals(str, "RG"))				return Anima::TEXTURE_INTERNAL_FORMAT_RG;
			else if (iequals(str, "RGB"))				return Anima::TEXTURE_INTERNAL_FORMAT_RGB;
			else if (iequals(str, "RGBA"))				return Anima::TEXTURE_INTERNAL_FORMAT_RGBA;
			else if (iequals(str, "1"))					return Anima::TEXTURE_INTERNAL_FORMAT_1;
			else if (iequals(str, "2"))					return Anima::TEXTURE_INTERNAL_FORMAT_2;
			else if (iequals(str, "3"))					return Anima::TEXTURE_INTERNAL_FORMAT_3;
			else if (iequals(str, "4"))					return Anima::TEXTURE_INTERNAL_FORMAT_4;

			return Anima::TEXTURE_INTERNAL_FORMAT_NONE;
		}
		else
			return boost::optional<external_type>(boost::none);
	}

	boost::optional<internal_type> put_value(const external_type& type)
	{
		switch (type)
		{
		case Anima::TEXTURE_INTERNAL_FORMAT_NONE:				return Anima::AnimaString("NONE"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_R8:					return Anima::AnimaString("R8"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_R8_SNORM:			return Anima::AnimaString("R8_SNORM"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_R16:				return Anima::AnimaString("R16"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_R16_SNORM:			return Anima::AnimaString("R16_SNORM"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_RG8:				return Anima::AnimaString("RG8"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_RG8_SNORM:			return Anima::AnimaString("RG8_SNORM"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_RG16:				return Anima::AnimaString("RG16"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_RG16_SNORM:			return Anima::AnimaString("RG16_SNORM"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_R3_G3_B2:			return Anima::AnimaString("R3_G3_B2"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_RGB4:				return Anima::AnimaString("RGB4"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_RGB5:				return Anima::AnimaString("RGB5"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_RGB565:				return Anima::AnimaString("RGB565"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_RGB8:				return Anima::AnimaString("RGB8"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_RGB8_SNORM:			return Anima::AnimaString("RGB8_SNORM"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_RGB10:				return Anima::AnimaString("RGB10"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_RGB12:				return Anima::AnimaString("RGB12"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_RGB16:				return Anima::AnimaString("RGB16"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_RGB16_SNORM:		return Anima::AnimaString("RGB16_SNORM"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_RGBA2:				return Anima::AnimaString("RGBA2"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_RGBA4:				return Anima::AnimaString("RGBA4"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_RGB5_A1:			return Anima::AnimaString("RGB5_A1"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_RGBA8:				return Anima::AnimaString("RGBA8"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_RGBA8_SNORM:		return Anima::AnimaString("RGBA8_SNORM"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_RGB10_A2:			return Anima::AnimaString("RGB10_A2"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_RGB10_A2UI:			return Anima::AnimaString("RGB10_A2UI"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_RGBA12:				return Anima::AnimaString("RGBA12"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_RGBA16:				return Anima::AnimaString("RGBA16"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_RGBA16_SNORM:		return Anima::AnimaString("RGBA16_SNORM"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_SRGB8:				return Anima::AnimaString("SRGB8"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_SRGB8_A8:			return Anima::AnimaString("SRGB8_A8"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_R16F:				return Anima::AnimaString("R16F"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_RG16F:				return Anima::AnimaString("RG16F"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_RGB16F:				return Anima::AnimaString("RGB16F"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_RGBA16F:			return Anima::AnimaString("RGBA16F"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_R32F:				return Anima::AnimaString("R32F"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_RG32F:				return Anima::AnimaString("RG32F"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_RGB32F:				return Anima::AnimaString("RGB32F"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_RGBA32F:			return Anima::AnimaString("RGBA32F"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_R11F_G11F_B10F:		return Anima::AnimaString("R11F_G11F_B10F"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_RGB9_E5:			return Anima::AnimaString("RGB9_E5"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_R8I:				return Anima::AnimaString("R8I"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_R8UI:				return Anima::AnimaString("R8UI"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_R16I:				return Anima::AnimaString("R16I"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_R16UI:				return Anima::AnimaString("R16UI"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_R32I:				return Anima::AnimaString("R32I"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_R32UI:				return Anima::AnimaString("R32UI"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_RG8I:				return Anima::AnimaString("RG8I"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_RG8UI:				return Anima::AnimaString("RG8UI"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_RG16I:				return Anima::AnimaString("RG16I"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_RG16UI:				return Anima::AnimaString("RG16UI"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_RG32I:				return Anima::AnimaString("RG32I"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_RG32UI:				return Anima::AnimaString("RG32UI"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_RGB8I:				return Anima::AnimaString("RGB8I"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_RGB8UI:				return Anima::AnimaString("RGB8UI"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_RGB16I:				return Anima::AnimaString("RGB16I"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_RGB16UI:			return Anima::AnimaString("RGB16UI"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_RGB32I:				return Anima::AnimaString("RGB32I"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_RGB32UI:			return Anima::AnimaString("RGB32UI"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_RGBA8I:				return Anima::AnimaString("RGBA8I"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_RGBA8UI:			return Anima::AnimaString("RGBA8UI"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_RGBA16I:			return Anima::AnimaString("RGBA16I"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_RGBA16UI:			return Anima::AnimaString("RGBA16UI"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_RGBA32I:			return Anima::AnimaString("RGBA32I"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_RGBA32UI:			return Anima::AnimaString("RGBA32UI"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_DEPTH_COMPONENT:	return Anima::AnimaString("DEPTH"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_DEPTH_STENCIL:		return Anima::AnimaString("DEPTH_STENCIL"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_STENCIL:			return Anima::AnimaString("STENCIL"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_DEPTH16:			return Anima::AnimaString("DEPTH16"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_DEPTH24:			return Anima::AnimaString("DEPTH24"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_DEPTH32:			return Anima::AnimaString("DEPTH32"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_DEPTH32F:			return Anima::AnimaString("DEPTH32F"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_DEPTH24_STENCIL8:	return Anima::AnimaString("DEPTH24_STENCIL8"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_DEPTH32F_STENCIL8:	return Anima::AnimaString("DEPTH32F_STENCIL8"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_STENCIL8:			return Anima::AnimaString("STENCIL8"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_RED:				return Anima::AnimaString("RED"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_RG:					return Anima::AnimaString("RG"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_RGB:				return Anima::AnimaString("RGB"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_RGBA:				return Anima::AnimaString("RGBA"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_1:					return Anima::AnimaString("1"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_2:					return Anima::AnimaString("2"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_3:					return Anima::AnimaString("3"); break;
		case Anima::TEXTURE_INTERNAL_FORMAT_4:					return Anima::AnimaString("4"); break;
		}

		return boost::optional<internal_type>(boost::none);
	}
};

struct AnimaXmlCameraProjectionTypeTranslator
{
	typedef Anima::AnimaString internal_type;
	typedef Anima::AnimaCameraProjectionType external_type;
	
	boost::optional<external_type> get_value(const internal_type& str)
	{
		if (!str.empty())
		{
			using boost::algorithm::iequals;
			
			if (iequals(str, "PERSPECTIVE"))		return Anima::ACPT_PERSPECTIVE;
			else if (iequals(str, "ORTHOGRAPHIC"))	return Anima::ACPT_ORTHOGRAPHIC;
			
			return boost::optional<external_type>(boost::none);
		}
		else
			return boost::optional<external_type>(boost::none);
	}
	
	boost::optional<internal_type> put_value(const external_type& type)
	{
		switch (type)
		{
			case Anima::ACPT_PERSPECTIVE:	return Anima::AnimaString("PERSPECTIVE"); break;
			case Anima::ACPT_ORTHOGRAPHIC:	return Anima::AnimaString("ORTHOGRAPHIC"); break;
		}
		
		return boost::optional<internal_type>(boost::none);
	}
};

struct AnimaXmlCameraTypeTranslator
{
	typedef Anima::AnimaString internal_type;
	typedef Anima::AnimaCameraType external_type;
	
	boost::optional<external_type> get_value(const internal_type& str)
	{
		if (!str.empty())
		{
			using boost::algorithm::iequals;
			
			if (iequals(str, "FIRST_PERSON"))	return Anima::ACT_FIRST_PERSON;
			else if (iequals(str, "SPECTATOR"))	return Anima::ACT_SPECTATOR;
			else if (iequals(str, "ORBIT"))		return Anima::ACT_ORBIT;
			else if (iequals(str, "FLIGHT"))	return Anima::ACT_FLIGHT;
			
			return boost::optional<external_type>(boost::none);
		}
		else
			return boost::optional<external_type>(boost::none);
	}
	
	boost::optional<internal_type> put_value(const external_type& type)
	{
		switch (type)
		{
			case Anima::ACT_FIRST_PERSON:	return Anima::AnimaString("FIRST_PERSON"); break;
			case Anima::ACT_SPECTATOR:		return Anima::AnimaString("SPECTATOR"); break;
			case Anima::ACT_ORBIT:			return Anima::AnimaString("ORBIT"); break;
			case Anima::ACT_FLIGHT:			return Anima::AnimaString("FLIGHT"); break;
		}
		
		return boost::optional<internal_type>(boost::none);
	}
};

namespace boost 
{
	namespace property_tree
	{
		template<> struct translator_between < Anima::AnimaString, Anima::AnimaTextureInternalFormat >
		{
			typedef AnimaXmlTextureInternalFormatTranslator type;
		};

		template<> struct translator_between < Anima::AnimaString, Anima::AnimaTextureFormat >
		{
			typedef AnimaXmlTextureFormatTranslator type;
		};

		template<> struct translator_between < Anima::AnimaString, Anima::AnimaTextureTarget >
		{
			typedef AnimaXmlTextureTargetTranslator type;
		};
			
		template<> struct translator_between < Anima::AnimaString, Anima::AnimaVertex2f >
		{
			typedef AnimaXmlVertex2Translator type;
		};

		template<> struct translator_between < Anima::AnimaString, Anima::AnimaVertex3f >
		{
			typedef AnimaXmlVertex3Translator type;
		};

		template<> struct translator_between < Anima::AnimaString, Anima::AnimaVertex4f >
		{
			typedef AnimaXmlVertex4Translator type;
		};

		template<> struct translator_between < Anima::AnimaString, bool >
		{
			typedef AnimaXmlBoolTranslator type;
		};

		template<> struct translator_between < Anima::AnimaString, Anima::AnimaShaderDataType >
		{
			typedef AnimaXmlShaderDataTypeTranslator type;
		};
		
		template<> struct translator_between < Anima::AnimaString, Anima::AnimaShaderDataSourceObject >
		{
			typedef AnimaXmlShaderDataSourceObjectTranslator type;
		};

		template<> struct translator_between < Anima::AnimaString, Anima::AnimaArray<Anima::AnimaVertex2f> >
		{
			typedef AnimaXmlVertex2ArrayTranslator type;
		};

		template<> struct translator_between < Anima::AnimaString, Anima::AnimaArray<Anima::AnimaVertex3f> >
		{
			typedef AnimaXmlVertex3ArrayTranslator type;
		};

		template<> struct translator_between < Anima::AnimaString, Anima::AnimaArray<Anima::AnimaVertex4f> >
		{
			typedef AnimaXmlVertex4ArrayTranslator type;
		};

		template<> struct translator_between < Anima::AnimaString, Anima::AnimaArray<Anima::AUint> >
		{
			typedef AnimaXmlAUintArrayTranslator type;
		};

		template<> struct translator_between < Anima::AnimaString, Anima::AnimaMatrix >
		{
			typedef AnimaXmlMatrixTranslator type;
		};

		template<> struct translator_between < Anima::AnimaString, Anima::AnimaTextureMinFilterMode >
		{
			typedef AnimaXmlTextureMinFilterModeTranslator type;
		};

		template<> struct translator_between < Anima::AnimaString, Anima::AnimaTextureMagFilterMode >
		{
			typedef AnimaXmlTextureMagFilterModeTranslator type;
		};

		template<> struct translator_between < Anima::AnimaString, Anima::AnimaTextureDataType>
		{
			typedef AnimaXmlTextureDataTypeTranslator type;
		};

		template<> struct translator_between < Anima::AnimaString, Anima::AnimaTextureClampMode>
		{
			typedef AnimaXmlTextureClampModeTranslator type;
		};
		
		template<> struct translator_between < Anima::AnimaString, Anima::AnimaTextureAttachment>
		{
			typedef AnimaXmlTextureAttachmentTranslator type;
		};
		
		template<> struct translator_between < Anima::AnimaString, Anima::AnimaCameraProjectionType>
		{
			typedef AnimaXmlCameraProjectionTypeTranslator type;
		};
		
		template<> struct translator_between < Anima::AnimaString, Anima::AnimaCameraType>
		{
			typedef AnimaXmlCameraTypeTranslator type;
		};
		
		template<> struct translator_between < Anima::AnimaString, Anima::AnimaQuaternion>
		{
			typedef AnimaXmlQuaternionTranslator type;
		};
	}
}

#endif