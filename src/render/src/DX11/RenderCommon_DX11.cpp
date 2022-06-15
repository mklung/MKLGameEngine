#include "RenderCommon_DX11.h"

namespace sge {

	VertexSemanticType DX11Util::parseDxSemanticName(StrView s) {
		VertexSemanticType v;

		if (s == "SV_POSITION") {
			return VertexSemanticType::POSITION;
		}

		if		(s == "None")		return VertexSemanticType::None;
		else if (s == "POSITION")	return VertexSemanticType::POSITION;
		else if (s == "COLOR")		return VertexSemanticType::COLOR;
		else if (s == "TEXCOORD")	return VertexSemanticType::TEXCOORD;
		else if (s == "NORMAL")		return VertexSemanticType::NORMAL;
		else if (s == "TANGENT")	return VertexSemanticType::TANGENT;
		else if (s == "BINORMAL")	return VertexSemanticType::BINORMAL;
		else throw SGE_ERROR("unknown VertexLayout_SemanticType {}", s);
			 

		/*if (!enumTryParse(v, s)) {
			throw SGE_ERROR("unknown VertexLayout_SemanticType {}", s);
		}
		return v;*/
	}


}