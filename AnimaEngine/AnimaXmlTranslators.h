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

			if (iequals(str, "FLOAT"))
				return Anima::FLOAT;
			else if (iequals(str, "FLOAT_ARRAY"))
				return Anima::FLOAT_ARRAY;
			else if (iequals(str, "FLOAT2"))
				return Anima::FLOAT2;
			else if (iequals(str, "FLOAT2_ARRAY"))
				return Anima::FLOAT2_ARRAY;
			else if (iequals(str, "FLOAT3"))
				return Anima::FLOAT3;
			else if (iequals(str, "FLOAT3_ARRAY"))
				return Anima::FLOAT3_ARRAY;
			else if (iequals(str, "FLOAT4"))
				return Anima::FLOAT4;
			else if (iequals(str, "FLOAT4_ARRAY"))
				return Anima::FLOAT4_ARRAY;
			else if (iequals(str, "MATRIX4x4"))
				return Anima::MATRIX4x4;
			else if (iequals(str, "MATRIX4x4_ARRAY"))
				return Anima::MATRIX4x4_ARRAY;
			else if (iequals(str, "MATRIX3x3"))
				return Anima::MATRIX3x3;
			else if (iequals(str, "MATRIX3x3_ARRAY"))
				return Anima::MATRIX3x3_ARRAY;
			else if (iequals(str, "INT"))
				return Anima::INT;
			else if (iequals(str, "INT_ARRAY"))
				return Anima::INT_ARRAY;
			else if (iequals(str, "BOOL"))
				return Anima::BOOL;
			else if (iequals(str, "BOOL_ARRAY"))
				return Anima::BOOL_ARRAY;
			else if (iequals(str, "TEXTURE2D"))
				return Anima::TEXTURE2D;
			else if (iequals(str, "TEXTURE2D_ARRAY"))
				return Anima::TEXTURE2D_ARRAY;
			else if (iequals(str, "TEXTURE3D"))
				return Anima::TEXTURE3D;
			else if (iequals(str, "TEXTURE3D_ARRAY"))
				return Anima::TEXTURE3D_ARRAY;

			return Anima::NONE;
		}
		else
			return boost::optional<external_type>(boost::none);
	}

	boost::optional<internal_type> put_value(const external_type& type)
	{
		if (type == Anima::FLOAT)
			return Anima::AnimaString("FLOAT");
		else if (type == Anima::FLOAT_ARRAY)
			return Anima::AnimaString("FLOAT_ARRAY");
		else if (type == Anima::FLOAT2)
			return Anima::AnimaString("FLOAT2");
		else if (type == Anima::FLOAT2_ARRAY)
			return Anima::AnimaString("FLOAT2_ARRAY");
		else if (type == Anima::FLOAT3)
			return Anima::AnimaString("FLOAT3");
		else if (type == Anima::FLOAT3_ARRAY)
			return Anima::AnimaString("FLOAT3_ARRAY");
		else if (type == Anima::FLOAT4)
			return Anima::AnimaString("FLOAT4");
		else if (type == Anima::FLOAT4_ARRAY)
			return Anima::AnimaString("FLOAT4_ARRAY");
		else if (type == Anima::MATRIX4x4)
			return Anima::AnimaString("MATRIX4x4");
		else if (type == Anima::MATRIX4x4_ARRAY)
			return Anima::AnimaString("MATRIX4x4_ARRAY");
		else if (type == Anima::MATRIX3x3)
			return Anima::AnimaString("MATRIX3x3");
		else if (type == Anima::MATRIX3x3_ARRAY)
			return Anima::AnimaString("MATRIX3x3_ARRAY");
		else if (type == Anima::INT)
			return Anima::AnimaString("INT");
		else if (type == Anima::INT_ARRAY)
			return Anima::AnimaString("INT_ARRAY");
		else if (type == Anima::BOOL)
			return Anima::AnimaString("BOOL");
		else if (type == Anima::BOOL_ARRAY)
			return Anima::AnimaString("BOOL_ARRAY");
		else if (type == Anima::TEXTURE2D)
			return Anima::AnimaString("TEXTURE2D");
		else if (type == Anima::TEXTURE2D_ARRAY)
			return Anima::AnimaString("TEXTURE2D_ARRAY");
		else if (type == Anima::TEXTURE3D)
			return Anima::AnimaString("TEXTURE3D");
		else if (type == Anima::TEXTURE3D_ARRAY)
			return Anima::AnimaString("TEXTURE3D_ARRAY");

		return Anima::AnimaString("NONE");
	}
};

namespace boost 
{
	namespace property_tree
	{
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