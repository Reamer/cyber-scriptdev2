/* Copyright (C) 2006 - 2011 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* ScriptData
SDName: instance_ahnkahet
SD%Complete: 80%
SDComment:
SDCategory: Ahn'kahet
EndScriptData */

#include "precompiled.h"
#include "ahnkahet.h"

instance_ahnkahet::instance_ahnkahet(Map* pMap) : ScriptedInstance(pMap),
    m_bCriteriaVolunteerWork(false),
    m_bCriteriaRespectYourElders(false),
    m_uiElderNadoxGUID(0),
    m_uiJedogaShadowseekerGUID(0),
    m_uiTaldaramDoorGUID(0),
    m_uiTaldaramVortexGUID(0),
    m_uiDevicesActivated(0)
{
    Initialize();
}
void instance_ahnkahet::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
}

void instance_ahnkahet::OnCreatureCreate(Creature* pCreature)
{
    switch(pCreature->GetEntry())
    {
        case NPC_ELDER_NADOX:         m_uiElderNadoxGUID = pCreature->GetGUID();         break;
        case NPC_JEDOGA_SHADOWSEEKER: m_uiJedogaShadowseekerGUID = pCreature->GetGUID(); break;
    }
}
void instance_ahnkahet::OnObjectCreate(GameObject* pGo)
{
     switch(pGo->GetEntry())
    {
        case GO_DOOR_TALDARAM:
            m_uiTaldaramDoorGUID = pGo->GetGUID();
            if (m_auiEncounter[1] == DONE)
                DoUseDoorOrButton(m_uiTaldaramDoorGUID);
            break;
        case GO_ANCIENT_DEVICE_L:
            if (m_auiEncounter[1] == NOT_STARTED)
                pGo->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_NO_INTERACT);
            break;
        case GO_ANCIENT_DEVICE_R:
            if (m_auiEncounter[1] == NOT_STARTED)
                pGo->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_NO_INTERACT);
            break;
        case GO_VORTEX:
            m_uiTaldaramVortexGUID = pGo->GetGUID();
            if (m_auiEncounter[1] != NOT_STARTED)
                DoUseDoorOrButton(m_uiTaldaramVortexGUID);
            break;
    }
}
void instance_ahnkahet::SetData(uint32 uiType, uint32 uiData)
{
    debug_log("SD2: Instance Ahn'Kahet: SetData received for type %u with data %u",uiType,uiData);

    switch(uiType)
    {
        case TYPE_NADOX:
            m_auiEncounter[0] = uiData;
            break;
        case TYPE_TALDARAM:
            if (uiData == SPECIAL)
            {
                if (m_uiDevicesActivated < 2)
                    ++m_uiDevicesActivated;

                if (m_uiDevicesActivated == 2)
                {
                    m_auiEncounter[1] = uiData;
                    DoUseDoorOrButton(m_uiTaldaramVortexGUID);
                }
            }
            if (uiData == DONE)
            {
                m_auiEncounter[1] = uiData;
                DoUseDoorOrButton(m_uiTaldaramDoorGUID);
            }
            break;
        case TYPE_JEDOGA:
            m_auiEncounter[2] = uiData;
            break;
        case TYPE_VOLAZJ:
            m_auiEncounter[3] = uiData;
            break;
        case TYPE_AMANITAR:
            m_auiEncounter[4] = uiData;
            break;
        default:
            error_log("SD2: Instance Ahn'Kahet: ERROR SetData = %u for type %u does not exist/not implemented.",uiType,uiData);
            break;
    }

    if (uiData == DONE)
    {
        OUT_SAVE_INST_DATA;

        std::ostringstream saveStream;
        saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1] << " " << m_auiEncounter[2] << " " << m_auiEncounter[3]
            << " " << m_auiEncounter[4];

        strInstData = saveStream.str();

        SaveToDB();
        OUT_SAVE_INST_DATA_COMPLETE;
    }
}
void instance_ahnkahet::SetAchiev(uint32 uiType, bool get)
{
    switch(uiType)
    {
        case TYPE_NADOX:
            m_bCriteriaRespectYourElders = get;
            break;
        case TYPE_TALDARAM:
            break;
        case TYPE_JEDOGA:
            m_bCriteriaVolunteerWork = get;
            break;
        case TYPE_VOLAZJ:
            break;
        case TYPE_AMANITAR:
            break;
        default:
            break;
    }
}


void instance_ahnkahet::Load(const char* chrIn)
{
    if (!chrIn)
    {
        OUT_LOAD_INST_DATA_FAIL;
        return;
    }

    OUT_LOAD_INST_DATA(chrIn);

    std::istringstream loadStream(chrIn);
    loadStream >> m_auiEncounter[0] >> m_auiEncounter[1] >> m_auiEncounter[2] >> m_auiEncounter[3] >> m_auiEncounter[4];

    for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
    {
        if (m_auiEncounter[i] == IN_PROGRESS)
            m_auiEncounter[i] = NOT_STARTED;
    }

    OUT_LOAD_INST_DATA_COMPLETE;
}
uint32 instance_ahnkahet::GetData(uint32 uiType)
{
    switch(uiType)
    {
        case TYPE_NADOX:
            return m_auiEncounter[0];
        case TYPE_TALDARAM:
            return m_auiEncounter[1];
        case TYPE_JEDOGA:
            return m_auiEncounter[2];
        case TYPE_VOLAZJ:
            return m_auiEncounter[3];
        case TYPE_AMANITAR:
            return m_auiEncounter[4];
    }
    return 0;
}
uint64 instance_ahnkahet::GetData64(uint32 uiData)
{
    switch(uiData)
    {
        case NPC_ELDER_NADOX:
            return m_uiElderNadoxGUID;
        case NPC_JEDOGA_SHADOWSEEKER:
            return m_uiJedogaShadowseekerGUID;
    }
    return 0;
}
 
bool instance_ahnkahet::CheckAchievementCriteriaMeet(uint32 uiCriteriaId, Player const* pSource, Unit const* pTarget, uint32 uiMiscValue1 /* = 0*/)
{
    switch (uiCriteriaId)
    {
        case ACHIEV_CRITERIA_VOLUNTEER_WORK:
            return m_bCriteriaVolunteerWork;
        case ACHIEV_CRITERIA_RESPECT_YOUR_ELDERS:
            return m_bCriteriaRespectYourElders;
        default:
            return 0;
    }
}


InstanceData* GetInstanceData_instance_ahnkahet(Map* pMap)
{
    return new instance_ahnkahet(pMap);
}

void AddSC_instance_ahnkahet()
{
    Script* newscript;

    newscript = new Script;
    newscript->Name = "instance_ahnkahet";
    newscript->GetInstanceData = &GetInstanceData_instance_ahnkahet;
    newscript->RegisterSelf();
}
