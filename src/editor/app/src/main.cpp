#include <sge_editor.h>

namespace sge {

class MainWin : public NativeUIWindow {
	using Base = NativeUIWindow;
public:
	void onCreate(CreateDesc& desc) {

		Base::onCreate(desc);
		auto* renderer = Renderer::current();

		{
			RenderContext::CreateDesc renderContextDesc;
			renderContextDesc.window = this;
			_renderContext = renderer->createContext(renderContextDesc);
		}

		EditMesh editMesh;

		editMesh.pos.emplace_back(0.0f, 0.5f, 0.0f);
		editMesh.pos.emplace_back(0.5f, -0.5f, 0.0f);
		editMesh.pos.emplace_back(-0.5f, -0.5f, 0.0f);

		editMesh.color.emplace_back(255, 0, 0, 255);
		editMesh.color.emplace_back(0, 255, 0, 255);
		editMesh.color.emplace_back(0, 0, 255, 255);
		_renderMesh.create(editMesh);


		_renderContext->beginRender();
	}

	virtual void onCloseButton() override {
		NativeUIApp::current()->quit(0);

	}

	virtual void onDraw() {
		//Base::onDraw();
		if (!_renderContext) return;
		_renderContext->render();
		_renderContext->endRender();
		drawNeeded();
	}
	RenderContext* _renderContext;
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
			path.append("/../../../../../../Assets");
			setCurrentDir(path);

			auto dir = getCurrentDir();
			SGE_LOG("dir = {}", dir);
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
