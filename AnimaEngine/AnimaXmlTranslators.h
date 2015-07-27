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
#include <boost/property_tree/json_parser.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string.hpp>

#include "AnimaVertex.h"
#include "AnimaShaderData.h"

struct AnimaXmlVertexTranslator
{
	typedef Anima::AnimaString internal_type;
	typedef Anima::AnimaVertex4f external_type;

	boost::optional<external_type> get_value(const internal_type& str)
	{
		if (!str.empty())
		{
			std::vector<Anima::AnimaString> elements;
			boost::split(elements, str, boost::is_any_of(";,"));

			Anima::AnimaVertex4f vertex;

			size_t elementsSize = elements.size();

			if (elementsSize >= 1)
				vertex.x = (Anima::AFloat)atof(elements[0].c_str());
			else
				vertex.x = 0.0f;

			if (elementsSize >= 2)
				vertex.y = (Anima::AFloat)atof(elements[1].c_str());
			else
				vertex.y = 0.0f;

			if (elementsSize >= 3)
				vertex.z = (Anima::AFloat)atof(elements[2].c_str());
			else
				vertex.z = 0.0f;

			if (elementsSize >= 4)
				vertex.w = (Anima::AFloat)atof(elements[3].c_str());
			else
				vertex.w = 1.0f;

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
				return Anima::AnimaShaderDataType::FLOAT;
			else if (iequals(str, "FLOAT_ARRAY"))
				return Anima::AnimaShaderDataType::FLOAT_ARRAY;
			else if (iequals(str, "FLOAT2"))
				return Anima::AnimaShaderDataType::FLOAT2;
			else if (iequals(str, "FLOAT2_ARRAY"))
				return Anima::AnimaShaderDataType::FLOAT2_ARRAY;
			else if (iequals(str, "FLOAT3"))
				return Anima::AnimaShaderDataType::FLOAT3;
			else if (iequals(str, "FLOAT3_ARRAY"))
				return Anima::AnimaShaderDataType::FLOAT3_ARRAY;
			else if (iequals(str, "FLOAT4"))
				return Anima::AnimaShaderDataType::FLOAT4;
			else if (iequals(str, "FLOAT4_ARRAY"))
				return Anima::AnimaShaderDataType::FLOAT4_ARRAY;
			else if (iequals(str, "MATRIX4x4"))
				return Anima::AnimaShaderDataType::MATRIX4x4;
			else if (iequals(str, "MATRIX4x4_ARRAY"))
				return Anima::AnimaShaderDataType::MATRIX4x4_ARRAY;
			else if (iequals(str, "MATRIX3x3"))
				return Anima::AnimaShaderDataType::MATRIX3x3;
			else if (iequals(str, "MATRIX3x3_ARRAY"))
				return Anima::AnimaShaderDataType::MATRIX3x3_ARRAY;
			else if (iequals(str, "INT"))
				return Anima::AnimaShaderDataType::INT;
			else if (iequals(str, "INT_ARRAY"))
				return Anima::AnimaShaderDataType::INT_ARRAY;
			else if (iequals(str, "BOOL"))
				return Anima::AnimaShaderDataType::BOOL;
			else if (iequals(str, "BOOL_ARRAY"))
				return Anima::AnimaShaderDataType::BOOL_ARRAY;
			else if (iequals(str, "TEXTURE2D"))
				return Anima::AnimaShaderDataType::TEXTURE2D;
			else if (iequals(str, "TEXTURE2D_ARRAY"))
				return Anima::AnimaShaderDataType::TEXTURE2D_ARRAY;
			else if (iequals(str, "TEXTURE3D"))
				return Anima::AnimaShaderDataType::TEXTURE3D;
			else if (iequals(str, "TEXTURE3D_ARRAY"))
				return Anima::AnimaShaderDataType::TEXTURE3D_ARRAY;

			return Anima::AnimaShaderDataType::NONE;
		}
		else
			return boost::optional<external_type>(boost::none);
	}

	boost::optional<internal_type> put_value(const external_type& type)
	{
		if (type == Anima::AnimaShaderDataType::FLOAT)
			return "FLOAT";
		else if (type == Anima::AnimaShaderDataType::FLOAT_ARRAY)
			return "FLOAT_ARRAY";
		else if (type == Anima::AnimaShaderDataType::FLOAT2)
			return "FLOAT2";
		else if (type == Anima::AnimaShaderDataType::FLOAT2_ARRAY)
			return "FLOAT2_ARRAY";
		else if (type == Anima::AnimaShaderDataType::FLOAT3)
			return "FLOAT3";
		else if (type == Anima::AnimaShaderDataType::FLOAT3_ARRAY)
			return "FLOAT3_ARRAY";
		else if (type == Anima::AnimaShaderDataType::FLOAT4)
			return "FLOAT4";
		else if (type == Anima::AnimaShaderDataType::FLOAT4_ARRAY)
			return "FLOAT4_ARRAY";
		else if (type == Anima::AnimaShaderDataType::MATRIX4x4)
			return "MATRIX4x4";
		else if (type == Anima::AnimaShaderDataType::MATRIX4x4_ARRAY)
			return "MATRIX4x4_ARRAY";
		else if (type == Anima::AnimaShaderDataType::MATRIX3x3)
			return "MATRIX3x3";
		else if (type == Anima::AnimaShaderDataType::MATRIX3x3_ARRAY)
			return "MATRIX3x3_ARRAY";
		else if (type == Anima::AnimaShaderDataType::INT)
			return "INT";
		else if (type == Anima::AnimaShaderDataType::INT_ARRAY)
			return "INT_ARRAY";
		else if (type == Anima::AnimaShaderDataType::BOOL)
			return "BOOL";
		else if (type == Anima::AnimaShaderDataType::BOOL_ARRAY)
			return "BOOL_ARRAY";
		else if (type == Anima::AnimaShaderDataType::TEXTURE2D)
			return "TEXTURE2D";
		else if (type == Anima::AnimaShaderDataType::TEXTURE2D_ARRAY)
			return "TEXTURE2D_ARRAY";
		else if (type == Anima::AnimaShaderDataType::TEXTURE3D)
			return "TEXTURE3D";
		else if (type == Anima::AnimaShaderDataType::TEXTURE3D_ARRAY)
			return "TEXTURE3D_ARRAY";

		return "NONE";
	}
};

namespace boost 
{
	namespace property_tree
	{
		template<> struct translator_between < Anima::AnimaString, Anima::AnimaVertex4f >
		{
			typedef AnimaXmlVertexTranslator type;
		};

		template<> struct translator_between < Anima::AnimaString, bool >
		{
			typedef AnimaXmlBoolTranslator type;
		};

		template<> struct translator_between < Anima::AnimaString, Anima::AnimaShaderDataType >
		{
			typedef AnimaXmlShaderDataTypeTranslator type;
		};
	}
}

#endif