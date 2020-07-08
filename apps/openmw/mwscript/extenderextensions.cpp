#include "extenderextensions.hpp"

#include <components/compiler/extensions.hpp>
#include <components/compiler/opcodes.hpp>
#include <components/debug/debuglog.hpp>
#include <components/interpreter/interpreter.hpp>
#include <components/interpreter/runtime.hpp>
#include <components/interpreter/opcodes.hpp>

#include "../mwbase/environment.hpp"
#include "../mwbase/dialoguemanager.hpp"
#include "../mwbase/journal.hpp"
#include "../mwbase/world.hpp"
#include "../mwbase/windowmanager.hpp"

#include "../mwworld/class.hpp"
#include "../mwmechanics/npcstats.hpp"

#include "interpretercontext.hpp"
#include "ref.hpp"

namespace MWScript
{
    namespace Extender
    {
        template <class R>
        class opxIsFemale : public Interpreter::Opcode0
        {
            public:

                virtual void execute (Interpreter::Runtime& runtime)
                {
                    MWWorld::Ptr ptr = R()(runtime, false); // required=false
                    if (ptr.isEmpty())
                        ptr = MWBase::Environment::get().getWorld()->getPlayerPtr();


                    MWWorld::LiveCellRef<ESM::NPC>* npc = ptr.get<ESM::NPC>();
                    runtime.push(!npc->mBase->isMale());
                }
        };

        void installOpcodes (Interpreter::Interpreter& interpreter)
        {
            interpreter.installSegment5(Compiler::Extender::opcodexIsFemale, new opxIsFemale<ImplicitRef>);
            interpreter.installSegment5(Compiler::Extender::opcodexIsFemaleExplicit, new opxIsFemale<ExplicitRef>);
        }
    }

}
