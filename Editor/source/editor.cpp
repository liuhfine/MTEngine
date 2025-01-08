#include "editor.h"
#include "editor_ui.h"
//#include <any>

namespace Mountian
{

    Editor::Editor()
    {
        m_editor_ui = std::make_shared<EditorUI>();
    };

    Editor::~Editor()
    {
        //std::any;
    };

    void Editor::initialize(int argc, char* argv[])
    {
        m_editor_ui->initialize(argc, argv);
    };

    void Editor::clear()
    {

    };


    void Editor::run()
    {
        m_editor_ui->show();
    };

} // namespace Mountian


