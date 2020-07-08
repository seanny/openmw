#ifndef GAME_SCRIPT_EXTENDEREXTENSIONS_H
#define GAME_SCRIPT_EXTENDEREXTENSIONS_H

namespace Compiler
{
    class Extensions;
}

namespace Interpreter
{
    class Interpreter;
}

namespace MWScript
{
    /// \brief MWSE script functionality
    namespace Extender
    {
        void installOpcodes (Interpreter::Interpreter& interpreter);
    }
}

#endif
