#pragma once

namespace sge {





	class RenderContext
	{
	public:

		void StartRender() {};
		void EndRender() {};

		RenderContext();
		~RenderContext();

	private:

		virtual void OnStartRender();
		virtual void OnEndRender();
	};


}