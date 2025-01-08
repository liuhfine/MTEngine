#pragma once
#include "MObject.h"
#include <memory>

namespace Mountian
{
    class EditorUI;

    class Editor
    {
        friend class EditorUI;

    public:
        Editor();
        virtual ~Editor();

        void initialize(int argc, char* argv[]);
        void clear();

        void run();

    protected:
        std::shared_ptr<EditorUI> m_editor_ui;
        /*PiccoloEngine* m_engine_runtime{ nullptr };*/
    };
} // namespace Mountian
