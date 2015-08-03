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

			size_t elementsSize = elements.size();

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

			if (iequals(str, "FLOAT"))					return Anima::AnimaShaderDataType::FLOAT;
			else if (iequals(str, "FLOAT_ARRAY"))		return Anima::AnimaShaderDataType::FLOAT_ARRAY;
			else if (iequals(str, "FLOAT2"))			return Anima::AnimaShaderDataType::FLOAT2;
			else if (iequals(str, "FLOAT2_ARRAY"))		return Anima::AnimaShaderDataType::FLOAT2_ARRAY;
			else if (iequals(str, "FLOAT3"))			return Anima::AnimaShaderDataType::FLOAT3;
			else if (iequals(str, "FLOAT3_ARRAY"))		return Anima::AnimaShaderDataType::FLOAT3_ARRAY;
			else if (iequals(str, "FLOAT4"))			return Anima::AnimaShaderDataType::FLOAT4;
			else if (iequals(str, "FLOAT4_ARRAY"))		return Anima::AnimaShaderDataType::FLOAT4_ARRAY;
			else if (iequals(str, "MATRIX4x4"))			return Anima::AnimaShaderDataType::MATRIX4x4;
			else if (iequals(str, "MATRIX4x4_ARRAY"))	return Anima::AnimaShaderDataType::MATRIX4x4_ARRAY;
			else if (iequals(str, "MATRIX3x3"))			return Anima::AnimaShaderDataType::MATRIX3x3;
			else if (iequals(str, "MATRIX3x3_ARRAY"))	return Anima::AnimaShaderDataType::MATRIX3x3_ARRAY;
			else if (iequals(str, "INT"))				return Anima::AnimaShaderDataType::INT;
			else if (iequals(str, "INT_ARRAY"))			return Anima::AnimaShaderDataType::INT_ARRAY;
			else if (iequals(str, "BOOL"))				return Anima::AnimaShaderDataType::BOOL;
			else if (iequals(str, "BOOL_ARRAY"))		return Anima::AnimaShaderDataType::BOOL_ARRAY;
			else if (iequals(str, "TEXTURE2D"))			return Anima::AnimaShaderDataType::TEXTURE2D;
			else if (iequals(str, "TEXTURE2D_ARRAY"))	return Anima::AnimaShaderDataType::TEXTURE2D_ARRAY;
			else if (iequals(str, "TEXTURE3D"))			return Anima::AnimaShaderDataType::TEXTURE3D;
			else if (iequals(str, "TEXTURE3D_ARRAY"))	return Anima::AnimaShaderDataType::TEXTURE3D_ARRAY;

			return Anima::NONE;
		}
		else
			return boost::optional<external_type>(boost::none);
	}

	boost::optional<internal_type> put_value(const external_type& type)
	{
		switch (type)
		{
		case Anima::AnimaShaderDataType::FLOAT:				return Anima::AnimaString("FLOAT"); break;
		case Anima::AnimaShaderDataType::FLOAT_ARRAY:		return Anima::AnimaString("FLOAT_ARRAY"); break;
		case Anima::AnimaShaderDataType::FLOAT2:			return Anima::AnimaString("FLOAT2"); break;
		case Anima::AnimaShaderDataType::FLOAT2_ARRAY:		return Anima::AnimaString("FLOAT2_ARRAY"); break;
		case Anima::AnimaShaderDataType::FLOAT3:			return Anima::AnimaString("FLOAT3"); break;
		case Anima::AnimaShaderDataType::FLOAT3_ARRAY:		return Anima::AnimaString("FLOAT3_ARRAY"); break;
		case Anima::AnimaShaderDataType::FLOAT4:			return Anima::AnimaString("FLOAT4"); break;
		case Anima::AnimaShaderDataType::FLOAT4_ARRAY:		return Anima::AnimaString("FLOAT4_ARRAY"); break;
		case Anima::AnimaShaderDataType::MATRIX4x4:			return Anima::AnimaString("MATRIX4x4"); break;
		case Anima::AnimaShaderDataType::MATRIX4x4_ARRAY:	return Anima::AnimaString("MATRIX4x4_ARRAY"); break;
		case Anima::AnimaShaderDataType::MATRIX3x3:			return Anima::AnimaString("MATRIX3x3"); break;
		case Anima::AnimaShaderDataType::MATRIX3x3_ARRAY:	return Anima::AnimaString("MATRIX3x3_ARRAY"); break;
		case Anima::AnimaShaderDataType::INT:				return Anima::AnimaString("INT"); break;
		case Anima::AnimaShaderDataType::INT_ARRAY:			return Anima::AnimaString("INT_ARRAY"); break;
		case Anima::AnimaShaderDataType::BOOL:				return Anima::AnimaString("BOOL"); break;
		case Anima::AnimaShaderDataType::BOOL_ARRAY:		return Anima::AnimaString("BOOL_ARRAY"); break;
		case Anima::AnimaShaderDataType::TEXTURE2D:			return Anima::AnimaString("TEXTURE2D"); break;
		case Anima::AnimaShaderDataType::TEXTURE2D_ARRAY:	return Anima::AnimaString("TEXTURE2D_ARRAY"); break;
		case Anima::AnimaShaderDataType::TEXTURE3D:			return Anima::AnimaString("TEXTURE3D"); break;
		case Anima::AnimaShaderDataType::TEXTURE3D_ARRAY:	return Anima::AnimaString("TEXTURE3D_ARRAY"); break;
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

			if (iequals(str, "CUBE"))		return Anima::AnimaTextureTarget::TEXTURE_CUBE;
			else if (iequals(str, "2D"))	return Anima::AnimaTextureTarget::TEXTURE_2D;
			else if (iequals(str, "3D"))	return Anima::AnimaTextureTarget::TEXTURE_3D;

			return boost::optional<external_type>(boost::none);
		}
		else
			return boost::optional<external_type>(boost::none);
	}

	boost::optional<internal_type> put_value(const external_type& type)
	{
		switch (type)
		{
		case Anima::AnimaTextureTarget::TEXTURE_CUBE:	return Anima::AnimaString("CUBE"); break;
		case Anima::AnimaTextureTarget::TEXTURE_2D:		return Anima::AnimaString("2D"); break;
		case Anima::AnimaTextureTarget::TEXTURE_3D:		return Anima::AnimaString("3D"); break;
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

			if (iequals(str, "RED"))			return Anima::AnimaTextureFormat::RED;
			else if (iequals(str, "GREEN"))		return Anima::AnimaTextureFormat::GREEN;
			else if (iequals(str, "BLUE"))		return Anima::AnimaTextureFormat::BLUE;
			else if (iequals(str, "RG"))		return Anima::AnimaTextureFormat::RG;
			else if (iequals(str, "RGB"))		return Anima::AnimaTextureFormat::RGB;
			else if (iequals(str, "RGBA"))		return Anima::AnimaTextureFormat::RGBA;
			else if (iequals(str, "BGR"))		return Anima::AnimaTextureFormat::BGR;
			else if (iequals(str, "BGRA"))		return Anima::AnimaTextureFormat::BGRA;
			else if (iequals(str, "RED_INT"))	return Anima::AnimaTextureFormat::RED_INT;
			else if (iequals(str, "GREEN_INT"))	return Anima::AnimaTextureFormat::GREEN_INT;
			else if (iequals(str, "BLUE_INT"))	return Anima::AnimaTextureFormat::BLUE_INT;
			else if (iequals(str, "RG_INT"))	return Anima::AnimaTextureFormat::RG_INT;
			else if (iequals(str, "RGB_INT"))	return Anima::AnimaTextureFormat::RGB_INT;
			else if (iequals(str, "RGBA_INT"))	return Anima::AnimaTextureFormat::RGBA_INT;
			else if (iequals(str, "BGR_INT"))	return Anima::AnimaTextureFormat::BGR_INT;
			else if (iequals(str, "BGRA_INT"))	return Anima::AnimaTextureFormat::BGRA_INT;
			else if (iequals(str, "DEPTH"))		return Anima::AnimaTextureFormat::DEPTH;

			return Anima::AnimaTextureFormat::FORMAT_NONE;
		}
		else
			return boost::optional<external_type>(boost::none);
	}

	boost::optional<internal_type> put_value(const external_type& type)
	{
		switch (type)
		{
		case Anima::AnimaTextureFormat::FORMAT_NONE:	return Anima::AnimaString("NONE"); break;
		case Anima::AnimaTextureFormat::RED:			return Anima::AnimaString("RED"); break;
		case Anima::AnimaTextureFormat::GREEN:			return Anima::AnimaString("GREEN"); break;
		case Anima::AnimaTextureFormat::BLUE:			return Anima::AnimaString("BLUE"); break;
		case Anima::AnimaTextureFormat::RG:				return Anima::AnimaString("RG"); break;
		case Anima::AnimaTextureFormat::RGB:			return Anima::AnimaString("RGB"); break;
		case Anima::AnimaTextureFormat::RGBA:			return Anima::AnimaString("RGBA"); break;
		case Anima::AnimaTextureFormat::BGR:			return Anima::AnimaString("BGR"); break;
		case Anima::AnimaTextureFormat::BGRA:			return Anima::AnimaString("BGRA"); break;
		case Anima::AnimaTextureFormat::RED_INT:		return Anima::AnimaString("RED_INT"); break;
		case Anima::AnimaTextureFormat::GREEN_INT:		return Anima::AnimaString("GREEN_INT"); break;
		case Anima::AnimaTextureFormat::BLUE_INT:		return Anima::AnimaString("BLUE_INT"); break;
		case Anima::AnimaTextureFormat::RG_INT:			return Anima::AnimaString("RG_INT"); break;
		case Anima::AnimaTextureFormat::RGB_INT:		return Anima::AnimaString("RGB_INT"); break;
		case Anima::AnimaTextureFormat::RGBA_INT:		return Anima::AnimaString("RGBA_INT"); break;
		case Anima::AnimaTextureFormat::BGR_INT:		return Anima::AnimaString("BGR_INT"); break;
		case Anima::AnimaTextureFormat::BGRA_INT:		return Anima::AnimaString("BGRA_INT"); break;
		case Anima::AnimaTextureFormat::DEPTH:			return Anima::AnimaString("DEPTH"); break;
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

			if (iequals(str, "R8"))						return Anima::AnimaTextureInternalFormat::R8;
			else if (iequals(str, "R8_SNORM"))			return Anima::AnimaTextureInternalFormat::R8_SNORM;
			else if (iequals(str, "R16"))				return Anima::AnimaTextureInternalFormat::R16;
			else if (iequals(str, "R16_SNORM"))			return Anima::AnimaTextureInternalFormat::R16_SNORM;
			else if (iequals(str, "RG8"))				return Anima::AnimaTextureInternalFormat::RG8;
			else if (iequals(str, "RG8_SNORM"))			return Anima::AnimaTextureInternalFormat::RG8_SNORM;
			else if (iequals(str, "RG16"))				return Anima::AnimaTextureInternalFormat::RG16;
			else if (iequals(str, "RG16_SNORM"))		return Anima::AnimaTextureInternalFormat::RG16_SNORM;
			else if (iequals(str, "R3_G3_B2"))			return Anima::AnimaTextureInternalFormat::R3_G3_B2;
			else if (iequals(str, "RGB4"))				return Anima::AnimaTextureInternalFormat::RGB4;
			else if (iequals(str, "RGB5"))				return Anima::AnimaTextureInternalFormat::RGB5;
			else if (iequals(str, "RGB565"))			return Anima::AnimaTextureInternalFormat::RGB565;
			else if (iequals(str, "RGB8"))				return Anima::AnimaTextureInternalFormat::RGB8;
			else if (iequals(str, "RGB8_SNORM"))		return Anima::AnimaTextureInternalFormat::RGB8_SNORM;
			else if (iequals(str, "RGB10"))				return Anima::AnimaTextureInternalFormat::RGB10;
			else if (iequals(str, "RGB12"))				return Anima::AnimaTextureInternalFormat::RGB12;
			else if (iequals(str, "RGB16"))				return Anima::AnimaTextureInternalFormat::RGB16;
			else if (iequals(str, "RGB16_SNORM"))		return Anima::AnimaTextureInternalFormat::RGB16_SNORM;
			else if (iequals(str, "RGBA2"))				return Anima::AnimaTextureInternalFormat::RGBA2;
			else if (iequals(str, "RGBA4"))				return Anima::AnimaTextureInternalFormat::RGBA4;
			else if (iequals(str, "RGB5_A1"))			return Anima::AnimaTextureInternalFormat::RGB5_A1;
			else if (iequals(str, "RGBA8"))				return Anima::AnimaTextureInternalFormat::RGBA8;
			else if (iequals(str, "RGBA8_SNORM"))		return Anima::AnimaTextureInternalFormat::RGBA8_SNORM;
			else if (iequals(str, "RGB10_A2"))			return Anima::AnimaTextureInternalFormat::RGB10_A2;
			else if (iequals(str, "RGB10_A2UI"))		return Anima::AnimaTextureInternalFormat::RGB10_A2UI;
			else if (iequals(str, "RGBA12"))			return Anima::AnimaTextureInternalFormat::RGBA12;
			else if (iequals(str, "RGBA16"))			return Anima::AnimaTextureInternalFormat::RGBA16;
			else if (iequals(str, "RGBA16_SNORM"))		return Anima::AnimaTextureInternalFormat::RGBA16_SNORM;
			else if (iequals(str, "SRGB8"))				return Anima::AnimaTextureInternalFormat::SRGB8;
			else if (iequals(str, "SRGB8_A8"))			return Anima::AnimaTextureInternalFormat::SRGB8_A8;
			else if (iequals(str, "R16F"))				return Anima::AnimaTextureInternalFormat::R16F;
			else if (iequals(str, "RG16F"))				return Anima::AnimaTextureInternalFormat::RG16F;
			else if (iequals(str, "RGB16F"))			return Anima::AnimaTextureInternalFormat::RGB16F;
			else if (iequals(str, "RGBA16F"))			return Anima::AnimaTextureInternalFormat::RGBA16F;
			else if (iequals(str, "R32F"))				return Anima::AnimaTextureInternalFormat::R32F;
			else if (iequals(str, "RG32F"))				return Anima::AnimaTextureInternalFormat::RG32F;
			else if (iequals(str, "RGB32F"))			return Anima::AnimaTextureInternalFormat::RGB32F;
			else if (iequals(str, "RGBA32F"))			return Anima::AnimaTextureInternalFormat::RGBA32F;
			else if (iequals(str, "R11F_G11F_B10F"))	return Anima::AnimaTextureInternalFormat::R11F_G11F_B10F;
			else if (iequals(str, "RGB9_E5"))			return Anima::AnimaTextureInternalFormat::RGB9_E5;
			else if (iequals(str, "R8I"))				return Anima::AnimaTextureInternalFormat::R8I;
			else if (iequals(str, "R8UI"))				return Anima::AnimaTextureInternalFormat::R8UI;
			else if (iequals(str, "R16I"))				return Anima::AnimaTextureInternalFormat::R16I;
			else if (iequals(str, "R16UI"))				return Anima::AnimaTextureInternalFormat::R16UI;
			else if (iequals(str, "R32I"))				return Anima::AnimaTextureInternalFormat::R32I;
			else if (iequals(str, "R32UI"))				return Anima::AnimaTextureInternalFormat::R32UI;
			else if (iequals(str, "RG8I"))				return Anima::AnimaTextureInternalFormat::RG8I;
			else if (iequals(str, "RG8UI"))				return Anima::AnimaTextureInternalFormat::RG8UI;
			else if (iequals(str, "RG16I"))				return Anima::AnimaTextureInternalFormat::RG16I;
			else if (iequals(str, "RG16UI"))			return Anima::AnimaTextureInternalFormat::RG16UI;
			else if (iequals(str, "RG32I"))				return Anima::AnimaTextureInternalFormat::RG32I;
			else if (iequals(str, "RG32UI"))			return Anima::AnimaTextureInternalFormat::RG32UI;
			else if (iequals(str, "RGB8I"))				return Anima::AnimaTextureInternalFormat::RGB8I;
			else if (iequals(str, "RGB8UI"))			return Anima::AnimaTextureInternalFormat::RGB8UI;
			else if (iequals(str, "RGB16I"))			return Anima::AnimaTextureInternalFormat::RGB16I;
			else if (iequals(str, "RGB16UI"))			return Anima::AnimaTextureInternalFormat::RGB16UI;
			else if (iequals(str, "RGB32I"))			return Anima::AnimaTextureInternalFormat::RGB32I;
			else if (iequals(str, "RGB32UI"))			return Anima::AnimaTextureInternalFormat::RGB32UI;
			else if (iequals(str, "RGBA8I"))			return Anima::AnimaTextureInternalFormat::RGBA8I;
			else if (iequals(str, "RGBA8UI"))			return Anima::AnimaTextureInternalFormat::RGBA8UI;
			else if (iequals(str, "RGBA16I"))			return Anima::AnimaTextureInternalFormat::RGBA16I;
			else if (iequals(str, "RGBA16UI"))			return Anima::AnimaTextureInternalFormat::RGBA16UI;
			else if (iequals(str, "RGBA32I"))			return Anima::AnimaTextureInternalFormat::RGBA32I;
			else if (iequals(str, "RGBA32UI"))			return Anima::AnimaTextureInternalFormat::RGBA32UI;
			else if (iequals(str, "DEPTH16"))			return Anima::AnimaTextureInternalFormat::DEPTH16;
			else if (iequals(str, "DEPTH24"))			return Anima::AnimaTextureInternalFormat::DEPTH24;
			else if (iequals(str, "DEPTH32"))			return Anima::AnimaTextureInternalFormat::DEPTH32;
			else if (iequals(str, "DEPTH32F"))			return Anima::AnimaTextureInternalFormat::DEPTH32F;
			else if (iequals(str, "RED"))				return Anima::AnimaTextureInternalFormat::IF_RED;
			else if (iequals(str, "RG"))				return Anima::AnimaTextureInternalFormat::IF_RG;
			else if (iequals(str, "RGB"))				return Anima::AnimaTextureInternalFormat::IF_RGB;
			else if (iequals(str, "RGBA"))				return Anima::AnimaTextureInternalFormat::IF_RGBA;

			return Anima::AnimaTextureInternalFormat::INTERNAL_FORMAT_NONE;
		}
		else
			return boost::optional<external_type>(boost::none);
	}

	boost::optional<internal_type> put_value(const external_type& type)
	{
		switch (type)
		{
		case Anima::AnimaTextureInternalFormat::INTERNAL_FORMAT_NONE:	return Anima::AnimaString("NONE"); break;
		case Anima::AnimaTextureInternalFormat::R8:						return Anima::AnimaString("R8"); break;
		case Anima::AnimaTextureInternalFormat::R8_SNORM:				return Anima::AnimaString("R8_SNORM"); break;
		case Anima::AnimaTextureInternalFormat::R16:					return Anima::AnimaString("R16"); break;
		case Anima::AnimaTextureInternalFormat::R16_SNORM:				return Anima::AnimaString("R16_SNORM"); break;
		case Anima::AnimaTextureInternalFormat::RG8:					return Anima::AnimaString("RG8"); break;
		case Anima::AnimaTextureInternalFormat::RG8_SNORM:				return Anima::AnimaString("RG8_SNORM"); break;
		case Anima::AnimaTextureInternalFormat::RG16:					return Anima::AnimaString("RG16"); break;
		case Anima::AnimaTextureInternalFormat::RG16_SNORM:				return Anima::AnimaString("RG16_SNORM"); break;
		case Anima::AnimaTextureInternalFormat::R3_G3_B2:				return Anima::AnimaString("R3_G3_B2"); break;
		case Anima::AnimaTextureInternalFormat::RGB4:					return Anima::AnimaString("RGB4"); break;
		case Anima::AnimaTextureInternalFormat::RGB5:					return Anima::AnimaString("RGB5"); break;
		case Anima::AnimaTextureInternalFormat::RGB565:					return Anima::AnimaString("RGB565"); break;
		case Anima::AnimaTextureInternalFormat::RGB8:					return Anima::AnimaString("RGB8"); break;
		case Anima::AnimaTextureInternalFormat::RGB8_SNORM:				return Anima::AnimaString("RGB8_SNORM"); break;
		case Anima::AnimaTextureInternalFormat::RGB10:					return Anima::AnimaString("RGB10"); break;
		case Anima::AnimaTextureInternalFormat::RGB12:					return Anima::AnimaString("RGB12"); break;
		case Anima::AnimaTextureInternalFormat::RGB16:					return Anima::AnimaString("RGB16"); break;
		case Anima::AnimaTextureInternalFormat::RGB16_SNORM:			return Anima::AnimaString("RGB16_SNORM"); break;
		case Anima::AnimaTextureInternalFormat::RGBA2:					return Anima::AnimaString("RGBA2"); break;
		case Anima::AnimaTextureInternalFormat::RGBA4:					return Anima::AnimaString("RGBA4"); break;
		case Anima::AnimaTextureInternalFormat::RGB5_A1:				return Anima::AnimaString("RGB5_A1"); break;
		case Anima::AnimaTextureInternalFormat::RGBA8:					return Anima::AnimaString("RGBA8"); break;
		case Anima::AnimaTextureInternalFormat::RGBA8_SNORM:			return Anima::AnimaString("RGBA8_SNORM"); break;
		case Anima::AnimaTextureInternalFormat::RGB10_A2:				return Anima::AnimaString("RGB10_A2"); break;
		case Anima::AnimaTextureInternalFormat::RGB10_A2UI:				return Anima::AnimaString("RGB10_A2UI"); break;
		case Anima::AnimaTextureInternalFormat::RGBA12:					return Anima::AnimaString("RGBA12"); break;
		case Anima::AnimaTextureInternalFormat::RGBA16:					return Anima::AnimaString("RGBA16"); break;
		case Anima::AnimaTextureInternalFormat::RGBA16_SNORM:			return Anima::AnimaString("RGBA16_SNORM"); break;
		case Anima::AnimaTextureInternalFormat::SRGB8:					return Anima::AnimaString("SRGB8"); break;
		case Anima::AnimaTextureInternalFormat::SRGB8_A8:				return Anima::AnimaString("SRGB8_A8"); break;
		case Anima::AnimaTextureInternalFormat::R16F:					return Anima::AnimaString("R16F"); break;
		case Anima::AnimaTextureInternalFormat::RG16F:					return Anima::AnimaString("RG16F"); break;
		case Anima::AnimaTextureInternalFormat::RGB16F:					return Anima::AnimaString("RGB16F"); break;
		case Anima::AnimaTextureInternalFormat::RGBA16F:				return Anima::AnimaString("RGBA16F"); break;
		case Anima::AnimaTextureInternalFormat::R32F:					return Anima::AnimaString("R32F"); break;
		case Anima::AnimaTextureInternalFormat::RG32F:					return Anima::AnimaString("RG32F"); break;
		case Anima::AnimaTextureInternalFormat::RGB32F:					return Anima::AnimaString("RGB32F"); break;
		case Anima::AnimaTextureInternalFormat::RGBA32F:				return Anima::AnimaString("RGBA32F"); break;
		case Anima::AnimaTextureInternalFormat::R11F_G11F_B10F:			return Anima::AnimaString("R11F_G11F_B10F"); break;
		case Anima::AnimaTextureInternalFormat::RGB9_E5:				return Anima::AnimaString("RGB9_E5"); break;
		case Anima::AnimaTextureInternalFormat::R8I:					return Anima::AnimaString("R8I"); break;
		case Anima::AnimaTextureInternalFormat::R8UI:					return Anima::AnimaString("R8UI"); break;
		case Anima::AnimaTextureInternalFormat::R16I:					return Anima::AnimaString("R16I"); break;
		case Anima::AnimaTextureInternalFormat::R16UI:					return Anima::AnimaString("R16UI"); break;
		case Anima::AnimaTextureInternalFormat::R32I:					return Anima::AnimaString("R32I"); break;
		case Anima::AnimaTextureInternalFormat::R32UI:					return Anima::AnimaString("R32UI"); break;
		case Anima::AnimaTextureInternalFormat::RG8I:					return Anima::AnimaString("RG8I"); break;
		case Anima::AnimaTextureInternalFormat::RG8UI:					return Anima::AnimaString("RG8UI"); break;
		case Anima::AnimaTextureInternalFormat::RG16I:					return Anima::AnimaString("RG16I"); break;
		case Anima::AnimaTextureInternalFormat::RG16UI:					return Anima::AnimaString("RG16UI"); break;
		case Anima::AnimaTextureInternalFormat::RG32I:					return Anima::AnimaString("RG32I"); break;
		case Anima::AnimaTextureInternalFormat::RG32UI:					return Anima::AnimaString("RG32UI"); break;
		case Anima::AnimaTextureInternalFormat::RGB8I:					return Anima::AnimaString("RGB8I"); break;
		case Anima::AnimaTextureInternalFormat::RGB8UI:					return Anima::AnimaString("RGB8UI"); break;
		case Anima::AnimaTextureInternalFormat::RGB16I:					return Anima::AnimaString("RGB16I"); break;
		case Anima::AnimaTextureInternalFormat::RGB16UI:				return Anima::AnimaString("RGB16UI"); break;
		case Anima::AnimaTextureInternalFormat::RGB32I:					return Anima::AnimaString("RGB32I"); break;
		case Anima::AnimaTextureInternalFormat::RGB32UI:				return Anima::AnimaString("RGB32UI"); break;
		case Anima::AnimaTextureInternalFormat::RGBA8I:					return Anima::AnimaString("RGBA8I"); break;
		case Anima::AnimaTextureInternalFormat::RGBA8UI:				return Anima::AnimaString("RGBA8UI"); break;
		case Anima::AnimaTextureInternalFormat::RGBA16I:				return Anima::AnimaString("RGBA16I"); break;
		case Anima::AnimaTextureInternalFormat::RGBA16UI:				return Anima::AnimaString("RGBA16UI"); break;
		case Anima::AnimaTextureInternalFormat::RGBA32I:				return Anima::AnimaString("RGBA32I"); break;
		case Anima::AnimaTextureInternalFormat::RGBA32UI:				return Anima::AnimaString("RGBA32UI"); break;
		case Anima::AnimaTextureInternalFormat::DEPTH16:				return Anima::AnimaString("DEPTH16"); break;
		case Anima::AnimaTextureInternalFormat::DEPTH24:				return Anima::AnimaString("DEPTH24"); break;
		case Anima::AnimaTextureInternalFormat::DEPTH32:				return Anima::AnimaString("DEPTH32"); break;
		case Anima::AnimaTextureInternalFormat::DEPTH32F:				return Anima::AnimaString("DEPTH32F"); break;
		case Anima::AnimaTextureInternalFormat::IF_RED:					return Anima::AnimaString("RED"); break;
		case Anima::AnimaTextureInternalFormat::IF_RG:					return Anima::AnimaString("RG"); break;
		case Anima::AnimaTextureInternalFormat::IF_RGB:					return Anima::AnimaString("RGB"); break;
		case Anima::AnimaTextureInternalFormat::IF_RGBA:				return Anima::AnimaString("RGBA"); break;
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
	}
}

#endif