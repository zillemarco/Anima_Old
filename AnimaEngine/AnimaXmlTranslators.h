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

			if (iequals(str, "FLOAT"))					return Anima::FLOAT;
			else if (iequals(str, "FLOAT_ARRAY"))		return Anima::FLOAT_ARRAY;
			else if (iequals(str, "FLOAT2"))			return Anima::FLOAT2;
			else if (iequals(str, "FLOAT2_ARRAY"))		return Anima::FLOAT2_ARRAY;
			else if (iequals(str, "FLOAT3"))			return Anima::FLOAT3;
			else if (iequals(str, "FLOAT3_ARRAY"))		return Anima::FLOAT3_ARRAY;
			else if (iequals(str, "FLOAT4"))			return Anima::FLOAT4;
			else if (iequals(str, "FLOAT4_ARRAY"))		return Anima::FLOAT4_ARRAY;
			else if (iequals(str, "MATRIX4x4"))			return Anima::MATRIX4x4;
			else if (iequals(str, "MATRIX4x4_ARRAY"))	return Anima::MATRIX4x4_ARRAY;
			else if (iequals(str, "MATRIX3x3"))			return Anima::MATRIX3x3;
			else if (iequals(str, "MATRIX3x3_ARRAY"))	return Anima::MATRIX3x3_ARRAY;
			else if (iequals(str, "INT"))				return Anima::INT;
			else if (iequals(str, "INT_ARRAY"))			return Anima::INT_ARRAY;
			else if (iequals(str, "BOOL"))				return Anima::BOOL;
			else if (iequals(str, "BOOL_ARRAY"))		return Anima::BOOL_ARRAY;
			else if (iequals(str, "TEXTURE2D"))			return Anima::TEXTURE2D;
			else if (iequals(str, "TEXTURE2D_ARRAY"))	return Anima::TEXTURE2D_ARRAY;
			else if (iequals(str, "TEXTURECUBE"))		return Anima::TEXTURECUBE;
			else if (iequals(str, "TEXTURECUBE_ARRAY"))	return Anima::TEXTURECUBE_ARRAY;
			else if (iequals(str, "TEXTURE3D"))			return Anima::TEXTURE3D;
			else if (iequals(str, "TEXTURE3D_ARRAY"))	return Anima::TEXTURE3D_ARRAY;

			return Anima::NONE;
		}
		else
			return boost::optional<external_type>(boost::none);
	}

	boost::optional<internal_type> put_value(const external_type& type)
	{
		switch (type)
		{
		case Anima::FLOAT:				return Anima::AnimaString("FLOAT"); break;
		case Anima::FLOAT_ARRAY:		return Anima::AnimaString("FLOAT_ARRAY"); break;
		case Anima::FLOAT2:			return Anima::AnimaString("FLOAT2"); break;
		case Anima::FLOAT2_ARRAY:		return Anima::AnimaString("FLOAT2_ARRAY"); break;
		case Anima::FLOAT3:			return Anima::AnimaString("FLOAT3"); break;
		case Anima::FLOAT3_ARRAY:		return Anima::AnimaString("FLOAT3_ARRAY"); break;
		case Anima::FLOAT4:			return Anima::AnimaString("FLOAT4"); break;
		case Anima::FLOAT4_ARRAY:		return Anima::AnimaString("FLOAT4_ARRAY"); break;
		case Anima::MATRIX4x4:			return Anima::AnimaString("MATRIX4x4"); break;
		case Anima::MATRIX4x4_ARRAY:	return Anima::AnimaString("MATRIX4x4_ARRAY"); break;
		case Anima::MATRIX3x3:			return Anima::AnimaString("MATRIX3x3"); break;
		case Anima::MATRIX3x3_ARRAY:	return Anima::AnimaString("MATRIX3x3_ARRAY"); break;
		case Anima::INT:				return Anima::AnimaString("INT"); break;
		case Anima::INT_ARRAY:			return Anima::AnimaString("INT_ARRAY"); break;
		case Anima::BOOL:				return Anima::AnimaString("BOOL"); break;
		case Anima::BOOL_ARRAY:		return Anima::AnimaString("BOOL_ARRAY"); break;
		case Anima::TEXTURE2D:			return Anima::AnimaString("TEXTURE2D"); break;
		case Anima::TEXTURE2D_ARRAY:	return Anima::AnimaString("TEXTURE2D_ARRAY"); break;
		case Anima::TEXTURECUBE:		return Anima::AnimaString("TEXTURECUBE"); break;
		case Anima::TEXTURECUBE_ARRAY:	return Anima::AnimaString("TEXTURECUBE_ARRAY"); break;
		case Anima::TEXTURE3D:			return Anima::AnimaString("TEXTURE3D"); break;
		case Anima::TEXTURE3D_ARRAY:	return Anima::AnimaString("TEXTURE3D_ARRAY"); break;
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

			if (iequals(str, "CUBE"))		return Anima::TEXTURE_CUBE;
			else if (iequals(str, "2D"))	return Anima::TEXTURE_2D;
			else if (iequals(str, "3D"))	return Anima::TEXTURE_3D;

			return boost::optional<external_type>(boost::none);
		}
		else
			return boost::optional<external_type>(boost::none);
	}

	boost::optional<internal_type> put_value(const external_type& type)
	{
		switch (type)
		{
		case Anima::TEXTURE_CUBE:	return Anima::AnimaString("CUBE"); break;
		case Anima::TEXTURE_2D:		return Anima::AnimaString("2D"); break;
		case Anima::TEXTURE_3D:		return Anima::AnimaString("3D"); break;
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

			if (iequals(str, "RED"))			return Anima::RED;
			else if (iequals(str, "GREEN"))		return Anima::GREEN;
			else if (iequals(str, "BLUE"))		return Anima::BLUE;
			else if (iequals(str, "RG"))		return Anima::RG;
			else if (iequals(str, "RGB"))		return Anima::RGB;
			else if (iequals(str, "RGBA"))		return Anima::RGBA;
			else if (iequals(str, "BGR"))		return Anima::BGR;
			else if (iequals(str, "BGRA"))		return Anima::BGRA;
			else if (iequals(str, "RED_INT"))	return Anima::RED_INT;
			else if (iequals(str, "GREEN_INT"))	return Anima::GREEN_INT;
			else if (iequals(str, "BLUE_INT"))	return Anima::BLUE_INT;
			else if (iequals(str, "RG_INT"))	return Anima::RG_INT;
			else if (iequals(str, "RGB_INT"))	return Anima::RGB_INT;
			else if (iequals(str, "RGBA_INT"))	return Anima::RGBA_INT;
			else if (iequals(str, "BGR_INT"))	return Anima::BGR_INT;
			else if (iequals(str, "BGRA_INT"))	return Anima::BGRA_INT;
			else if (iequals(str, "DEPTH"))		return Anima::DEPTH;

			return Anima::FORMAT_NONE;
		}
		else
			return boost::optional<external_type>(boost::none);
	}

	boost::optional<internal_type> put_value(const external_type& type)
	{
		switch (type)
		{
		case Anima::FORMAT_NONE:	return Anima::AnimaString("NONE"); break;
		case Anima::RED:			return Anima::AnimaString("RED"); break;
		case Anima::GREEN:			return Anima::AnimaString("GREEN"); break;
		case Anima::BLUE:			return Anima::AnimaString("BLUE"); break;
		case Anima::RG:				return Anima::AnimaString("RG"); break;
		case Anima::RGB:			return Anima::AnimaString("RGB"); break;
		case Anima::RGBA:			return Anima::AnimaString("RGBA"); break;
		case Anima::BGR:			return Anima::AnimaString("BGR"); break;
		case Anima::BGRA:			return Anima::AnimaString("BGRA"); break;
		case Anima::RED_INT:		return Anima::AnimaString("RED_INT"); break;
		case Anima::GREEN_INT:		return Anima::AnimaString("GREEN_INT"); break;
		case Anima::BLUE_INT:		return Anima::AnimaString("BLUE_INT"); break;
		case Anima::RG_INT:			return Anima::AnimaString("RG_INT"); break;
		case Anima::RGB_INT:		return Anima::AnimaString("RGB_INT"); break;
		case Anima::RGBA_INT:		return Anima::AnimaString("RGBA_INT"); break;
		case Anima::BGR_INT:		return Anima::AnimaString("BGR_INT"); break;
		case Anima::BGRA_INT:		return Anima::AnimaString("BGRA_INT"); break;
		case Anima::DEPTH:			return Anima::AnimaString("DEPTH"); break;
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

			if (iequals(str, "R8"))						return Anima::R8;
			else if (iequals(str, "R8_SNORM"))			return Anima::R8_SNORM;
			else if (iequals(str, "R16"))				return Anima::R16;
			else if (iequals(str, "R16_SNORM"))			return Anima::R16_SNORM;
			else if (iequals(str, "RG8"))				return Anima::RG8;
			else if (iequals(str, "RG8_SNORM"))			return Anima::RG8_SNORM;
			else if (iequals(str, "RG16"))				return Anima::RG16;
			else if (iequals(str, "RG16_SNORM"))		return Anima::RG16_SNORM;
			else if (iequals(str, "R3_G3_B2"))			return Anima::R3_G3_B2;
			else if (iequals(str, "RGB4"))				return Anima::RGB4;
			else if (iequals(str, "RGB5"))				return Anima::RGB5;
			else if (iequals(str, "RGB565"))			return Anima::RGB565;
			else if (iequals(str, "RGB8"))				return Anima::RGB8;
			else if (iequals(str, "RGB8_SNORM"))		return Anima::RGB8_SNORM;
			else if (iequals(str, "RGB10"))				return Anima::RGB10;
			else if (iequals(str, "RGB12"))				return Anima::RGB12;
			else if (iequals(str, "RGB16"))				return Anima::RGB16;
			else if (iequals(str, "RGB16_SNORM"))		return Anima::RGB16_SNORM;
			else if (iequals(str, "RGBA2"))				return Anima::RGBA2;
			else if (iequals(str, "RGBA4"))				return Anima::RGBA4;
			else if (iequals(str, "RGB5_A1"))			return Anima::RGB5_A1;
			else if (iequals(str, "RGBA8"))				return Anima::RGBA8;
			else if (iequals(str, "RGBA8_SNORM"))		return Anima::RGBA8_SNORM;
			else if (iequals(str, "RGB10_A2"))			return Anima::RGB10_A2;
			else if (iequals(str, "RGB10_A2UI"))		return Anima::RGB10_A2UI;
			else if (iequals(str, "RGBA12"))			return Anima::RGBA12;
			else if (iequals(str, "RGBA16"))			return Anima::RGBA16;
			else if (iequals(str, "RGBA16_SNORM"))		return Anima::RGBA16_SNORM;
			else if (iequals(str, "SRGB8"))				return Anima::SRGB8;
			else if (iequals(str, "SRGB8_A8"))			return Anima::SRGB8_A8;
			else if (iequals(str, "R16F"))				return Anima::R16F;
			else if (iequals(str, "RG16F"))				return Anima::RG16F;
			else if (iequals(str, "RGB16F"))			return Anima::RGB16F;
			else if (iequals(str, "RGBA16F"))			return Anima::RGBA16F;
			else if (iequals(str, "R32F"))				return Anima::R32F;
			else if (iequals(str, "RG32F"))				return Anima::RG32F;
			else if (iequals(str, "RGB32F"))			return Anima::RGB32F;
			else if (iequals(str, "RGBA32F"))			return Anima::RGBA32F;
			else if (iequals(str, "R11F_G11F_B10F"))	return Anima::R11F_G11F_B10F;
			else if (iequals(str, "RGB9_E5"))			return Anima::RGB9_E5;
			else if (iequals(str, "R8I"))				return Anima::R8I;
			else if (iequals(str, "R8UI"))				return Anima::R8UI;
			else if (iequals(str, "R16I"))				return Anima::R16I;
			else if (iequals(str, "R16UI"))				return Anima::R16UI;
			else if (iequals(str, "R32I"))				return Anima::R32I;
			else if (iequals(str, "R32UI"))				return Anima::R32UI;
			else if (iequals(str, "RG8I"))				return Anima::RG8I;
			else if (iequals(str, "RG8UI"))				return Anima::RG8UI;
			else if (iequals(str, "RG16I"))				return Anima::RG16I;
			else if (iequals(str, "RG16UI"))			return Anima::RG16UI;
			else if (iequals(str, "RG32I"))				return Anima::RG32I;
			else if (iequals(str, "RG32UI"))			return Anima::RG32UI;
			else if (iequals(str, "RGB8I"))				return Anima::RGB8I;
			else if (iequals(str, "RGB8UI"))			return Anima::RGB8UI;
			else if (iequals(str, "RGB16I"))			return Anima::RGB16I;
			else if (iequals(str, "RGB16UI"))			return Anima::RGB16UI;
			else if (iequals(str, "RGB32I"))			return Anima::RGB32I;
			else if (iequals(str, "RGB32UI"))			return Anima::RGB32UI;
			else if (iequals(str, "RGBA8I"))			return Anima::RGBA8I;
			else if (iequals(str, "RGBA8UI"))			return Anima::RGBA8UI;
			else if (iequals(str, "RGBA16I"))			return Anima::RGBA16I;
			else if (iequals(str, "RGBA16UI"))			return Anima::RGBA16UI;
			else if (iequals(str, "RGBA32I"))			return Anima::RGBA32I;
			else if (iequals(str, "RGBA32UI"))			return Anima::RGBA32UI;
			else if (iequals(str, "DEPTH16"))			return Anima::DEPTH16;
			else if (iequals(str, "DEPTH24"))			return Anima::DEPTH24;
			else if (iequals(str, "DEPTH32"))			return Anima::DEPTH32;
			else if (iequals(str, "DEPTH32F"))			return Anima::DEPTH32F;
			else if (iequals(str, "RED"))				return Anima::IF_RED;
			else if (iequals(str, "RG"))				return Anima::IF_RG;
			else if (iequals(str, "RGB"))				return Anima::IF_RGB;
			else if (iequals(str, "RGBA"))				return Anima::IF_RGBA;

			return Anima::INTERNAL_FORMAT_NONE;
		}
		else
			return boost::optional<external_type>(boost::none);
	}

	boost::optional<internal_type> put_value(const external_type& type)
	{
		switch (type)
		{
		case Anima::INTERNAL_FORMAT_NONE:	return Anima::AnimaString("NONE"); break;
		case Anima::R8:						return Anima::AnimaString("R8"); break;
		case Anima::R8_SNORM:				return Anima::AnimaString("R8_SNORM"); break;
		case Anima::R16:					return Anima::AnimaString("R16"); break;
		case Anima::R16_SNORM:				return Anima::AnimaString("R16_SNORM"); break;
		case Anima::RG8:					return Anima::AnimaString("RG8"); break;
		case Anima::RG8_SNORM:				return Anima::AnimaString("RG8_SNORM"); break;
		case Anima::RG16:					return Anima::AnimaString("RG16"); break;
		case Anima::RG16_SNORM:				return Anima::AnimaString("RG16_SNORM"); break;
		case Anima::R3_G3_B2:				return Anima::AnimaString("R3_G3_B2"); break;
		case Anima::RGB4:					return Anima::AnimaString("RGB4"); break;
		case Anima::RGB5:					return Anima::AnimaString("RGB5"); break;
		case Anima::RGB565:					return Anima::AnimaString("RGB565"); break;
		case Anima::RGB8:					return Anima::AnimaString("RGB8"); break;
		case Anima::RGB8_SNORM:				return Anima::AnimaString("RGB8_SNORM"); break;
		case Anima::RGB10:					return Anima::AnimaString("RGB10"); break;
		case Anima::RGB12:					return Anima::AnimaString("RGB12"); break;
		case Anima::RGB16:					return Anima::AnimaString("RGB16"); break;
		case Anima::RGB16_SNORM:			return Anima::AnimaString("RGB16_SNORM"); break;
		case Anima::RGBA2:					return Anima::AnimaString("RGBA2"); break;
		case Anima::RGBA4:					return Anima::AnimaString("RGBA4"); break;
		case Anima::RGB5_A1:				return Anima::AnimaString("RGB5_A1"); break;
		case Anima::RGBA8:					return Anima::AnimaString("RGBA8"); break;
		case Anima::RGBA8_SNORM:			return Anima::AnimaString("RGBA8_SNORM"); break;
		case Anima::RGB10_A2:				return Anima::AnimaString("RGB10_A2"); break;
		case Anima::RGB10_A2UI:				return Anima::AnimaString("RGB10_A2UI"); break;
		case Anima::RGBA12:					return Anima::AnimaString("RGBA12"); break;
		case Anima::RGBA16:					return Anima::AnimaString("RGBA16"); break;
		case Anima::RGBA16_SNORM:			return Anima::AnimaString("RGBA16_SNORM"); break;
		case Anima::SRGB8:					return Anima::AnimaString("SRGB8"); break;
		case Anima::SRGB8_A8:				return Anima::AnimaString("SRGB8_A8"); break;
		case Anima::R16F:					return Anima::AnimaString("R16F"); break;
		case Anima::RG16F:					return Anima::AnimaString("RG16F"); break;
		case Anima::RGB16F:					return Anima::AnimaString("RGB16F"); break;
		case Anima::RGBA16F:				return Anima::AnimaString("RGBA16F"); break;
		case Anima::R32F:					return Anima::AnimaString("R32F"); break;
		case Anima::RG32F:					return Anima::AnimaString("RG32F"); break;
		case Anima::RGB32F:					return Anima::AnimaString("RGB32F"); break;
		case Anima::RGBA32F:				return Anima::AnimaString("RGBA32F"); break;
		case Anima::R11F_G11F_B10F:			return Anima::AnimaString("R11F_G11F_B10F"); break;
		case Anima::RGB9_E5:				return Anima::AnimaString("RGB9_E5"); break;
		case Anima::R8I:					return Anima::AnimaString("R8I"); break;
		case Anima::R8UI:					return Anima::AnimaString("R8UI"); break;
		case Anima::R16I:					return Anima::AnimaString("R16I"); break;
		case Anima::R16UI:					return Anima::AnimaString("R16UI"); break;
		case Anima::R32I:					return Anima::AnimaString("R32I"); break;
		case Anima::R32UI:					return Anima::AnimaString("R32UI"); break;
		case Anima::RG8I:					return Anima::AnimaString("RG8I"); break;
		case Anima::RG8UI:					return Anima::AnimaString("RG8UI"); break;
		case Anima::RG16I:					return Anima::AnimaString("RG16I"); break;
		case Anima::RG16UI:					return Anima::AnimaString("RG16UI"); break;
		case Anima::RG32I:					return Anima::AnimaString("RG32I"); break;
		case Anima::RG32UI:					return Anima::AnimaString("RG32UI"); break;
		case Anima::RGB8I:					return Anima::AnimaString("RGB8I"); break;
		case Anima::RGB8UI:					return Anima::AnimaString("RGB8UI"); break;
		case Anima::RGB16I:					return Anima::AnimaString("RGB16I"); break;
		case Anima::RGB16UI:				return Anima::AnimaString("RGB16UI"); break;
		case Anima::RGB32I:					return Anima::AnimaString("RGB32I"); break;
		case Anima::RGB32UI:				return Anima::AnimaString("RGB32UI"); break;
		case Anima::RGBA8I:					return Anima::AnimaString("RGBA8I"); break;
		case Anima::RGBA8UI:				return Anima::AnimaString("RGBA8UI"); break;
		case Anima::RGBA16I:				return Anima::AnimaString("RGBA16I"); break;
		case Anima::RGBA16UI:				return Anima::AnimaString("RGBA16UI"); break;
		case Anima::RGBA32I:				return Anima::AnimaString("RGBA32I"); break;
		case Anima::RGBA32UI:				return Anima::AnimaString("RGBA32UI"); break;
		case Anima::DEPTH16:				return Anima::AnimaString("DEPTH16"); break;
		case Anima::DEPTH24:				return Anima::AnimaString("DEPTH24"); break;
		case Anima::DEPTH32:				return Anima::AnimaString("DEPTH32"); break;
		case Anima::DEPTH32F:				return Anima::AnimaString("DEPTH32F"); break;
		case Anima::IF_RED:					return Anima::AnimaString("RED"); break;
		case Anima::IF_RG:					return Anima::AnimaString("RG"); break;
		case Anima::IF_RGB:					return Anima::AnimaString("RGB"); break;
		case Anima::IF_RGBA:				return Anima::AnimaString("RGBA"); break;
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