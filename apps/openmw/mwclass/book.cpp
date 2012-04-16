
#include "book.hpp"

#include <components/esm/loadbook.hpp>

#include <components/esm_store/cell_store.hpp>

#include "../mwworld/ptr.hpp"
#include "../mwworld/actiontake.hpp"
#include "../mwworld/environment.hpp"
#include "../mwworld/world.hpp"

#include "../mwrender/objects.hpp"

#include "../mwgui/window_manager.hpp"

#include "../mwsound/soundmanager.hpp"

namespace MWClass
{
    void Book::insertObjectRendering (const MWWorld::Ptr& ptr, MWRender::RenderingInterface& renderingInterface) const
    {
        ESMS::LiveCellRef<ESM::Book, MWWorld::RefData> *ref =
            ptr.get<ESM::Book>();

        assert (ref->base != NULL);
        const std::string &model = ref->base->model;

        if (!model.empty())
        {
            MWRender::Objects& objects = renderingInterface.getObjects();
            objects.insertBegin(ptr, ptr.getRefData().isEnabled(), false);
            objects.insertMesh(ptr, "meshes\\" + model);
        }
    }

    void Book::insertObject(const MWWorld::Ptr& ptr, MWWorld::PhysicsSystem& physics, MWWorld::Environment& environment) const
    {
        ESMS::LiveCellRef<ESM::Book, MWWorld::RefData> *ref =
            ptr.get<ESM::Book>();


        const std::string &model = ref->base->model;
        assert (ref->base != NULL);
        if(!model.empty()){
            physics.insertObjectPhysics(ptr, "meshes\\" + model);
        }

    }

    std::string Book::getName (const MWWorld::Ptr& ptr) const
    {
        ESMS::LiveCellRef<ESM::Book, MWWorld::RefData> *ref =
            ptr.get<ESM::Book>();

        return ref->base->name;
    }

    boost::shared_ptr<MWWorld::Action> Book::activate (const MWWorld::Ptr& ptr,
        const MWWorld::Ptr& actor, const MWWorld::Environment& environment) const
    {
        // TODO implement reading

        environment.mSoundManager->playSound3D (ptr, getUpSoundId(ptr, environment), 1.0, 1.0, MWSound::Play_NoTrack);

        return boost::shared_ptr<MWWorld::Action> (
            new MWWorld::ActionTake (ptr));
    }

    std::string Book::getScript (const MWWorld::Ptr& ptr) const
    {
        ESMS::LiveCellRef<ESM::Book, MWWorld::RefData> *ref =
            ptr.get<ESM::Book>();

        return ref->base->script;
    }

    int Book::getValue (const MWWorld::Ptr& ptr) const
    {
        ESMS::LiveCellRef<ESM::Book, MWWorld::RefData> *ref =
            ptr.get<ESM::Book>();

        return ref->base->data.value;
    }

    void Book::registerSelf()
    {
        boost::shared_ptr<Class> instance (new Book);

        registerClass (typeid (ESM::Book).name(), instance);
    }

    std::string Book::getUpSoundId (const MWWorld::Ptr& ptr, const MWWorld::Environment& environment) const
    {
        return std::string("Item Book Up");
    }

    std::string Book::getDownSoundId (const MWWorld::Ptr& ptr, const MWWorld::Environment& environment) const
    {
        return std::string("Item Book Down");
    }

    bool Book::hasToolTip (const MWWorld::Ptr& ptr) const
    {
        ESMS::LiveCellRef<ESM::Book, MWWorld::RefData> *ref =
            ptr.get<ESM::Book>();

        return (ref->base->name != "");
    }

    MWGui::ToolTipInfo Book::getToolTipInfo (const MWWorld::Ptr& ptr, MWWorld::Environment& environment) const
    {
        ESMS::LiveCellRef<ESM::Book, MWWorld::RefData> *ref =
            ptr.get<ESM::Book>();

        MWGui::ToolTipInfo info;
        info.caption = ref->base->name;
        info.icon = ref->base->icon;

        std::string text;

        text += "\n" + environment.mWorld->getStore().gameSettings.search("sWeight")->str + ": " + MWGui::ToolTips::toString(ref->base->data.weight);
        text += MWGui::ToolTips::getValueString(ref->base->data.value, environment.mWorld->getStore().gameSettings.search("sValue")->str);

        if (environment.mWindowManager->getFullHelp()) {
            text += MWGui::ToolTips::getMiscString(ref->ref.owner, "Owner");
            text += MWGui::ToolTips::getMiscString(ref->base->script, "Script");
        }

        info.text = text;

        return info;
    }
}
