#pragma once

#include "../RenderDataType.h"

namespace sge {

	// posType			: 8 bit
	// colorType		: 8 bit
	// colorCount		: 2 bit
	// uvType			: 8 bit
	// uvCount			: 8 bit
	// normalType		: 8 bit
	// normalCount		: 2 bit
	// tangentCount		: 2 bit
	// binormalCount	: 2 bit
	enum class VertexType : u64 { None };

	struct VertexTypeUtil {
		static constexpr VertexType addPos(VertexType t, RenderDataType posType) {
			return static_cast<VertexType>(static_cast<u64>(t)
				| static_cast<u64>(posType));
		}

		static constexpr VertexType addColor(VertexType t, RenderDataType colorType, u8 colorCount) {
			return static_cast<VertexType>(static_cast<u64>(t)
				| (static_cast<u64>(colorType) << 8)
				| (static_cast<u64>(colorCount) << 16));
		}

		static constexpr VertexType addUv(VertexType t, RenderDataType uvType, u8 uvCount) {
			return static_cast<VertexType>(static_cast<u64>(t)
				| (static_cast<u64>(uvType) << 18)
				| (static_cast<u64>(uvCount) << 26));
		}

		static constexpr VertexType addNormal(VertexType t, RenderDataType normalType, u8 normalCount) {
			return static_cast<VertexType>(static_cast<u64>(t)
				| (static_cast<u64>(normalType) << 32)
				| (static_cast<u64>(normalCount) << 40));
		}

		static constexpr VertexType addTangent(VertexType t, u8 tangentCount) {
			return static_cast<VertexType>(static_cast<u64>(t)
				| (static_cast<u64>(tangentCount) << 42));
		}
		static constexpr VertexType addBinormal(VertexType t, u8 binormalCount) {
			return static_cast<VertexType>(static_cast<u64>(t)
				| (static_cast<u64>(binormalCount) << 44));
		}

		static constexpr VertexType make(
			RenderDataType posType,
			RenderDataType colorType, u8 colorCount,
			RenderDataType uvType, u8 uvCount,
			RenderDataType normalType, u8 normalCount, u8 tangentCount, u8 binormalCount)
		{
			VertexType t = addPos(VertexType::None, posType);
			if (colorCount)	 t = addColor(t, colorType, colorCount);
			if (uvCount)	 t = addUv(t, uvType, uvCount);
			if (normalCount) {
				t = addNormal(t, normalType, normalCount);
				t = addTangent(t, tangentCount);
				t = addBinormal(t, binormalCount);
			}
			return t;
		}
	};
}