#include <sge_editor.h>

namespace sge {

class MainWin : public NativeUIWindow {
public:
	
	virtual void onCloseButton() override {
		NativeUIApp::current()->quit(0);
	}
};

class EditorApp : public NativeUIApp {
	using Base = NativeUIApp;
public:
	virtual void onCreate(CreateDesc& desc) override {
		Base::onCreate(desc);

		NativeUIWindow::CreateDesc winDesc;
		winDesc.isMainWindow = true;
		_mainWin.create(winDesc);
		_mainWin.setWindowTitle("SGE Editor");
		_render = new Render();
		_render->InitD3D(_mainWin._hwnd);
		SGE_LOG("Hello {}", 10);

//		_renderer.create(_mainWin);
	}

	virtual void onRun() override {
		_render->RenderFrame();
		Base::onRun();
	}

	virtual void onQuit() override {
		Base::onQuit();
		_render->CleanD3D();
	}

private:
	MainWin		_mainWin;
	Render*		_render;
//	Renderer	_renderer;
};

}

int main() {


	sge::EditorApp app;
	sge::EditorApp::CreateDesc desc;
	app.run(desc);

	return 0;
}
