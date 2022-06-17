
#include <sge_editor.h>
#include "Mesh/OBJ/ObjLoader.h"

namespace sge {

class MainWin : public NativeUIWindow {
	using Base = NativeUIWindow;
public:
	void onCreate(CreateDesc& desc) {


		Base::onCreate(desc);
		auto* renderer = Renderer::instance();

		{
			RenderContext::CreateDesc renderContextDesc;
			renderContextDesc.window = this;
			_renderContext = renderer->createContext(renderContextDesc);
		}
		auto shader = renderer->createShader("Assets/Shaders/Triangle.hlsl");
		_material = renderer->createMaterial();
		_material->setShader(shader);
		//_renderContext->beginRender();
		
		EditMesh editMesh;
#if 1
		ObjLoader::LoadFile(editMesh, "Assets/Mesh/test.obj");

		for (size_t i = editMesh.color.size(); i < editMesh.pos.size(); i++) {
			editMesh.color.emplace_back(255, 255, 255, 255);
		}

		// the current shader has no uv or normal
		editMesh.uv[0].clear();
		editMesh.normal.clear();
#else
		editMesh.pos.emplace_back(0.0f, 0.5f, 0.0f);
		editMesh.pos.emplace_back(0.5f, -0.5f, 0.0f);
		editMesh.pos.emplace_back(-0.5f, -0.5f, 0.0f);

		editMesh.color.emplace_back(255, 0, 0, 1);
		editMesh.color.emplace_back(0, 255, 0, 1);
		editMesh.color.emplace_back(0, 0, 0, 1);
		editMesh.color.emplace_back(0, 0, 255, 255);
#endif
		_renderMesh.create(editMesh);

		VertexLayoutManager::instance()->getLayout(Vertex_Pos::kType);

	}

	virtual void onCloseButton() override {
		NativeUIApp::current()->quit(0);

	}

	virtual void onDraw() {
		Base::onDraw();
		if (!_renderContext) return;

		//_renderContext->setFrameBufferSize(clientRect().size);

		auto time = GetTickCount64() * 0.001f;
		auto s = abs(sin(time));

		_material->setParam("testColor", Color4f(s, 0, 0, 1));

		_renderContext->setFrameBufferSize(clientRect().size);
		_renderContext->beginRender();

		_cmdBuf.reset();
		_cmdBuf.clearFrameBuffers()->setColor({ 0.0f , 0, 0.2f, 1 });
		_cmdBuf.drawMesh(SGE_LOC, _renderMesh, _material);
		_cmdBuf.swapBuffers();

		_renderContext->commit(_cmdBuf);

		_renderContext->endRender();
		drawNeeded();
	}

	SPtr<Material> _material;

	SPtr<RenderContext> _renderContext;
	RenderCommandBuffer _cmdBuf;
	RenderMesh	_renderMesh;
};

class EditorApp : public NativeUIApp {
	using Base = NativeUIApp;
public:
	virtual void onCreate(CreateDesc& desc) override 
	{
		{
			String file = getExecutableFilename();
			String path = FilePath::getDir(file);
			path.append("/../../../../../../Data");
			SGE_LOG("dir = {}", path);
			setCurrentDir(path);

			auto dir = getCurrentDir();
		}
		Base::onCreate(desc);

		Renderer::CreateDesc renderDesc;
		renderDesc.apiType = Renderer::ApiType::DX11;
		Renderer::create(renderDesc);


		NativeUIWindow::CreateDesc winDesc;
		winDesc.isMainWindow = true;
		_mainWin.create(winDesc);
		_mainWin.setWindowTitle("SGE Editor");
	}



private:
	MainWin		_mainWin;
};

}

int main() {


	sge::EditorApp app;
	sge::EditorApp::CreateDesc desc;
	app.run(desc);

	return 0;
}
