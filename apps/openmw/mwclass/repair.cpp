
#include "repair.hpp"

#include <components/esm/loadlocks.hpp>

#include <components/esm_store/cell_store.hpp>

#include "../mwworld/ptr.hpp"
#include "../mwworld/actiontake.hpp"
#include "../mwworld/environment.hpp"
#include "../mwworld/world.hpp"
#include "../mwgui/window_manager.hpp"
#include "../mwgui/tooltips.hpp"

#include "../mwrender/objects.hpp"

#include "../mwsound/soundmanager.hpp"

namespace MWClass
{
    void Repair::insertObjectRendering (const MWWorld::Ptr& ptr, MWRender::RenderingInterface& renderingInterface) const
    {
        ESMS::LiveCellRef<ESM::Repair, MWWorld::RefData> *ref =
            ptr.get<ESM::Repair>();

        assert (ref->base != NULL);
        const std::string &model = ref->base->model;

        if (!model.empty())
        {
            MWRender::Objects& objects = renderingInterface.getObjects();
            objects.insertBegin(ptr, ptr.getRefData().isEnabled(), false);
            objects.insertMesh(ptr, "meshes\\" + model);
        }
    }

    void Repair::insertObject(const MWWorld::Ptr& ptr, MWWorld::PhysicsSystem& physics, MWWorld::Environment& environment) const
    {
        ESMS::LiveCellRef<ESM::Repair, MWWorld::RefData> *ref =
            ptr.get<ESM::Repair>();


        const std::string &model = ref->base->model;
        assert (ref->base != NULL);
        if(!model.empty()){
            physics.insertObjectPhysics(ptr, "meshes\\" + model);
        }

    }

    std::string Repair::getName (const MWWorld::Ptr& ptr) const
    {
        ESMS::LiveCellRef<ESM::Repair, MWWorld::RefData> *ref =
            ptr.get<ESM::Repair>();

        return ref->base->name;
    }

    boost::shared_ptr<MWWorld::Action> Repair::activate (const MWWorld::Ptr& ptr,
        const MWWorld::Ptr& actor, const MWWorld::Environment& environment) const
    {
        environment.mSoundManager->playSound3D (ptr, getUpSoundId(ptr, environment), 1.0, 1.0, MWSound::Play_NoTrack);

        return boost::shared_ptr<MWWorld::Action> (
            new MWWorld::ActionTake (ptr));
    }

    std::string Repair::getScript (const MWWorld::Ptr& ptr) const
    {
        ESMS::LiveCellRef<ESM::Repair, MWWorld::RefData> *ref =
            ptr.get<ESM::Repair>();

        return ref->base->script;
    }

    int Repair::getValue (const MWWorld::Ptr& ptr) const
    {
        ESMS::LiveCellRef<ESM::Repair, MWWorld::RefData> *ref =
            ptr.get<ESM::Repair>();

        return ref->base->data.value;
    }

    void Repair::registerSelf()
    {
        boost::shared_ptr<Class> instance (new Repair);

        registerClass (typeid (ESM::Repair).name(), instance);
    }

    std::string Repair::getUpSoundId (const MWWorld::Ptr& ptr, const MWWorld::Environment& environment) const
    {
        return std::string("Item Repair Up");
    }

    std::string Repair::getDownSoundId (const MWWorld::Ptr& ptr, const MWWorld::Environment& environment) const
    {
        return std::string("Item Repair Down");
    }

    bool Repair::hasToolTip (const MWWorld::Ptr& ptr) const
    {
        ESMS::LiveCellRef<ESM::Repair, MWWorld::RefData> *ref =
            ptr.get<ESM::Repair>();

        return (ref->base->name != "");
    }

    MWGui::ToolTipInfo Repair::getToolTipInfo (const MWWorld::Ptr& ptr, MWWorld::Environment& environment) const
    {
        ESMS::LiveCellRef<ESM::Repair, MWWorld::RefData> *ref =
            ptr.get<ESM::Repair>();

        MWGui::ToolTipInfo info;
        info.caption = ref->base->name;
        info.icon = ref->base->icon;

        std::string text;

        /// \todo store remaining uses somewhere

        text += "\n" + environment.mWorld->getStore().gameSettings.search("sUses")->str + ": " + MWGui::ToolTips::toString(ref->base->data.uses);
        text += "\n" + environment.mWorld->getStore().gameSettings.search("sQuality")->str + ": " + MWGui::ToolTips::toString(ref->base->data.quality);
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
