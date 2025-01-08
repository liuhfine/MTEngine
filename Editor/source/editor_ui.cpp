#include "editor_ui.h"
#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QMenuBar>
#include <QMenu>
#include <QToolBar>
#include <QTreeView>
#include <QDockWidget>
#include <QTextEdit>

#include <RmlUi/Core.h>
#include <RmlUi/Debugger.h>
#include "Backends/RmlUi_Backend.h"
#include "shell/include/Shell.h"

namespace Mountian
{

    EditorUI::EditorUI()
    {

    };

    EditorUI::~EditorUI()
    {

    };

    void EditorUI::initialize(int argc, char* argv[])
    {
		if (0)
		{
			createRmlUI();
		}
		else
		{
			m_app = new QApplication(argc, argv);

			QMainWindow* mainWindow = new QMainWindow;
			mainWindow->setWindowTitle("Mountain Engine");
			mainWindow->setGeometry(100, 100, 1000, 800);

			// Menu Bar
			QMenuBar* menuBar = mainWindow->menuBar();
			QMenu* fileMenu = menuBar->addMenu("File");
			fileMenu->addAction("New");
			fileMenu->addAction("Open");
			fileMenu->addAction("Save");
			fileMenu->addAction("Exit");

			// Tool Bar
			QToolBar* toolBar = mainWindow->addToolBar("File");
			toolBar->addAction("New");
			toolBar->addAction("Open");
			toolBar->addAction("Save");

			// Central Widget
			QTextEdit* centralWidget = new QTextEdit();
			mainWindow->setCentralWidget(centralWidget);

			// Dock Widgets
			QDockWidget* dock1 = new QDockWidget("Directory Tree", mainWindow);
			QDockWidget* dock2 = new QDockWidget("Property Editor", mainWindow);

			QTreeView* directoryTree = new QTreeView();
			QTextEdit* propertyEditor = new QTextEdit();

			dock1->setWidget(directoryTree);
			dock2->setWidget(propertyEditor);

			mainWindow->addDockWidget(Qt::LeftDockWidgetArea, dock1);
			mainWindow->addDockWidget(Qt::RightDockWidgetArea, dock2);

			m_mainWindow = mainWindow;
		}
    };

    void EditorUI::show()
    {
		assert(m_app);

		m_mainWindow->show();
		m_app->exec();
    };

	Rml::ElementDocument* setRmlWindow(const Rml::String& title, Rml::Context* context)
	{
		using namespace Rml;
		Rml::ElementDocument* document = nullptr;
		document = context->LoadDocument("D:/RmlUi/Samples/basic/demo/data/demo.rml");
		if (document)
		{
			document->GetElementById("title")->SetInnerRML(title);

			// Add sandbox default text.
			if (auto source = rmlui_dynamic_cast<Rml::ElementFormControl*>(document->GetElementById("sandbox_rml_source")))
			{
				auto value = source->GetValue();
				value += "<p>Write your RML here</p>\n\n<!-- <img src=\"assets/high_scores_alien_1.tga\"/> -->";
				source->SetValue(value);
			}
		}

		return document;
	}


	bool EditorUI::createRmlUI()
	{
		int width = 1024;
		int height = 768;
		if (!Shell::Initialize()) {
			return false;
		}
		if (!Backend::Initialize("MTEngine", width, height, true))
		{
			Shell::Shutdown();
			return false;
		}
		Rml::SetSystemInterface(Backend::GetSystemInterface());
		Rml::SetRenderInterface(Backend::GetRenderInterface());
		Rml::Initialise();
		Rml::Context* context = Rml::CreateContext("main", Rml::Vector2i(width, height));
		if (context == nullptr)
		{
			Rml::Shutdown();
			Backend::Shutdown();
			Shell::Shutdown();
			return false;
		}
		Rml::Debugger::Initialise(context);
		Shell::LoadFonts();

		Rml::ElementDocument* document = setRmlWindow("Hello MTEngine!", context);
		document->Show();

		bool running = true;
		while (running)
		{
			running = Backend::ProcessEvents(context, &Shell::ProcessKeyDownShortcuts);
			context->Update();
			Backend::BeginFrame();
			context->Render();
			Backend::PresentFrame();
		}
		Rml::Shutdown();
		Backend::Shutdown();
		Shell::Shutdown();

		system("pause");

		return true;
	}
} // namespace Mountian


