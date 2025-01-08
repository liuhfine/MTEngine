#pragma once
#include <memory>

class QApplication;
class QMainWindow;

namespace Mountian
{
    class EditorUI
    {

    public:
        EditorUI();
        ~EditorUI();

        void initialize(int argc, char* argv[]);
        void show();

    private:
        bool createRmlUI();

    private:
        QApplication* m_app{ nullptr };
        QMainWindow* m_mainWindow{ nullptr };
    protected:
    };
} // namespace Mountian
