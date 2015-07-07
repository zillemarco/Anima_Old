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
	}
}

#endif