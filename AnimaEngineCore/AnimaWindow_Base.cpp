#include "AnimaWindow_Base.h"

#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdio.h>

BEGIN_ANIMA_ENGINE_CORE_NAMESPACE

bool _AnimaEngineWindowStringInExtensionString(const char* string, const GLubyte* extensions)
{
	const GLubyte* start;
	GLubyte* where;
	GLubyte* terminator;

	// It takes a bit of care to be fool-proof about parsing the
	// OpenGL extensions string. Don't be fooled by sub-strings,
	// etc.
	start = extensions;
	for (;;)
	{
		where = (GLubyte*)strstr((const char*)start, string);
		if (!where)
			return false;

		terminator = where + strlen(string);
		if (where == start || *(where - 1) == ' ')
		{
			if (*terminator == ' ' || *terminator == '\0')
				break;
		}

		start = terminator;
	}

	return true;
}

const _AnimaEngineWindowfbconfig* _AnimaEngineWindowChooseFBConfig(const _AnimaEngineWindowfbconfig* desired, const _AnimaEngineWindowfbconfig* alternatives, unsigned int count)
{
	unsigned int i;
	unsigned int missing, leastMissing = UINT_MAX;
	unsigned int colorDiff, leastColorDiff = UINT_MAX;
	unsigned int extraDiff, leastExtraDiff = UINT_MAX;
	const _AnimaEngineWindowfbconfig* current;
	const _AnimaEngineWindowfbconfig* closest = NULL;

	for (i = 0; i < count; i++)
	{
		current = alternatives + i;

		if (desired->_stereo > 0 && current->_stereo == 0)
		{
			// Stereo is a hard constraint
			continue;
		}

		if (desired->_doublebuffer != current->_doublebuffer)
		{
			// Double buffering is a hard constraint
			continue;
		}

		// Count number of missing buffers
		{
			missing = 0;

			if (desired->_alphaBits > 0 && current->_alphaBits == 0)
				missing++;

			if (desired->_depthBits > 0 && current->_depthBits == 0)
				missing++;

			if (desired->_stencilBits > 0 && current->_stencilBits == 0)
				missing++;

			if (desired->_auxBuffers > 0 && current->_auxBuffers < desired->_auxBuffers)
			{
				missing += desired->_auxBuffers - current->_auxBuffers;
			}

			if (desired->_samples > 0 && current->_samples == 0)
			{
				// Technically, several multisampling buffers could be
				// involved, but that's a lower level implementation detail and
				// not important to us here, so we count them as one
				missing++;
			}
		}

		// These polynomials make many small channel size differences matter
		// less than one large channel size difference

		// Calculate color channel size difference value
		{
			colorDiff = 0;

			if (desired->_redBits != ANIMA_ENGINE_CORE_DONT_CARE)
			{
				colorDiff += (desired->_redBits - current->_redBits) * (desired->_redBits - current->_redBits);
			}

			if (desired->_greenBits != ANIMA_ENGINE_CORE_DONT_CARE)
			{
				colorDiff += (desired->_greenBits - current->_greenBits) * (desired->_greenBits - current->_greenBits);
			}

			if (desired->_blueBits != ANIMA_ENGINE_CORE_DONT_CARE)
			{
				colorDiff += (desired->_blueBits - current->_blueBits) * (desired->_blueBits - current->_blueBits);
			}
		}

		// Calculate non-color channel size difference value
		{
			extraDiff = 0;

			if (desired->_alphaBits != ANIMA_ENGINE_CORE_DONT_CARE)
			{
				extraDiff += (desired->_alphaBits - current->_alphaBits) * (desired->_alphaBits - current->_alphaBits);
			}

			if (desired->_depthBits != ANIMA_ENGINE_CORE_DONT_CARE)
			{
				extraDiff += (desired->_depthBits - current->_depthBits) * (desired->_depthBits - current->_depthBits);
			}

			if (desired->_stencilBits != ANIMA_ENGINE_CORE_DONT_CARE)
			{
				extraDiff += (desired->_stencilBits - current->_stencilBits) * (desired->_stencilBits - current->_stencilBits);
			}

			if (desired->_accumRedBits != ANIMA_ENGINE_CORE_DONT_CARE)
			{
				extraDiff += (desired->_accumRedBits - current->_accumRedBits) * (desired->_accumRedBits - current->_accumRedBits);
			}

			if (desired->_accumGreenBits != ANIMA_ENGINE_CORE_DONT_CARE)
			{
				extraDiff += (desired->_accumGreenBits - current->_accumGreenBits) * (desired->_accumGreenBits - current->_accumGreenBits);
			}

			if (desired->_accumBlueBits != ANIMA_ENGINE_CORE_DONT_CARE)
			{
				extraDiff += (desired->_accumBlueBits - current->_accumBlueBits) * (desired->_accumBlueBits - current->_accumBlueBits);
			}

			if (desired->_accumAlphaBits != ANIMA_ENGINE_CORE_DONT_CARE)
			{
				extraDiff += (desired->_accumAlphaBits - current->_accumAlphaBits) * (desired->_accumAlphaBits - current->_accumAlphaBits);
			}

			if (desired->_samples != ANIMA_ENGINE_CORE_DONT_CARE)
			{
				extraDiff += (desired->_samples - current->_samples) * (desired->_samples - current->_samples);
			}

			if (desired->_sRGB && !current->_sRGB)
				extraDiff++;
		}

		// Figure out if the current one is better than the best one found so far
		// Least number of missing buffers is the most important heuristic,
		// then color buffer size match and lastly size match for other buffers

		if (missing < leastMissing)
			closest = current;
		else if (missing == leastMissing)
		{
			if ((colorDiff < leastColorDiff) || (colorDiff == leastColorDiff && extraDiff < leastExtraDiff))
			{
				closest = current;
			}
		}

		if (current == closest)
		{
			leastMissing = missing;
			leastColorDiff = colorDiff;
			leastExtraDiff = extraDiff;
		}
	}

	return closest;
}

END_ANIMA_ENGINE_CORE_NAMESPACE