/* Copyright (C) 2006 - 2011 ScriptDev2 <http://www.scriptdev2.com/>
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
SDName: instance_nexus
SD%Complete: 75%
SDComment:
SDCategory: The Nexus
EndScriptData */

#include "precompiled.h"
#include "nexus.h"

bool GOUse_go_containment_sphere(Player* pPlayer, GameObject* pGo)
{
    ScriptedInstance* pInstance = (ScriptedInstance*)pGo->GetInstanceData();

    if (!pInstance)
        return false;

    switch(pGo->GetEntry())
    {
        case GO_CONTAINMENT_SPHERE_TELESTRA: pInstance->SetData(TYPE_TELESTRA, SPECIAL); break;
        case GO_CONTAINMENT_SPHERE_ANOMALUS: pInstance->SetData(TYPE_ANOMALUS, SPECIAL); break;
        case GO_CONTAINMENT_SPHERE_ORMOROK:  pInstance->SetData(TYPE_ORMOROK, SPECIAL);  break;
    }

    pGo->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_NO_INTERACT);
    return false;
}

instance_nexus::instance_nexus(Map* pMap) : ScriptedInstance(pMap)
{
    Initialize();
}

void instance_nexus::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
}

void instance_nexus::OnObjectCreate(GameObject* pGo)
{
    switch (pGo->GetEntry())
    {
        case GO_CONTAINMENT_SPHERE_TELESTRA:
            if (m_auiEncounter[TYPE_TELESTRA] == DONE)
                pGo->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_NO_INTERACT);
            break;
        case GO_CONTAINMENT_SPHERE_ANOMALUS:
            if (m_auiEncounter[TYPE_ANOMALUS] == DONE)
                pGo->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_NO_INTERACT);
            break;
        case GO_CONTAINMENT_SPHERE_ORMOROK:
            if (m_auiEncounter[TYPE_ORMOROK] == DONE)
                pGo->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_NO_INTERACT);
            break;

        default:
            return;
    }
    m_mGoEntryGuidStore[pGo->GetEntry()] = pGo->GetObjectGuid();
}

void instance_nexus::OnCreatureCreate(Creature* pCreature)
{
    if (pCreature->GetEntry() == NPC_KERISTRASZA)
        m_mNpcEntryGuidStore[NPC_KERISTRASZA] = pCreature->GetObjectGuid();
}

uint32 instance_nexus::GetData(uint32 uiType)
{
    if (uiType < MAX_ENCOUNTER)
        return m_auiEncounter[uiType];

    return 0;
}

void instance_nexus::SetData(uint32 uiType, uint32 uiData)
{
    switch (uiType)
    {
        case TYPE_TELESTRA:
            m_auiEncounter[uiType] = uiData;
            if (uiData == DONE)
            {
                if (GameObject* pGo = GetSingleGameObjectFromStorage(GO_CONTAINMENT_SPHERE_TELESTRA))
                    pGo->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_NO_INTERACT);
            }
            break;
        case TYPE_ANOMALUS:
            m_auiEncounter[uiType] = uiData;
            if (uiData == DONE)
            {
                if (GameObject* pGo = GetSingleGameObjectFromStorage(GO_CONTAINMENT_SPHERE_ANOMALUS))
                    pGo->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_NO_INTERACT);
            }
            break;
        case TYPE_ORMOROK:
            m_auiEncounter[uiType] = uiData;
            if (uiData == DONE)
            {
                if (GameObject* pGo = GetSingleGameObjectFromStorage(GO_CONTAINMENT_SPHERE_ORMOROK))
                    pGo->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_NO_INTERACT);
            }
            break;
        case TYPE_KERISTRASZA:
            m_auiEncounter[uiType] = uiData;
            break;
        default:
            error_log("SD2: Instance Nexus: ERROR SetData = %u for type %u does not exist/not implemented.", uiType, uiData);
            return;
    }

    if (m_auiEncounter[TYPE_TELESTRA] == SPECIAL && m_auiEncounter[TYPE_ANOMALUS] == SPECIAL && m_auiEncounter[TYPE_ORMOROK] == SPECIAL)
    {
        // release Keristrasza from her prison here
        m_auiEncounter[TYPE_KERISTRASZA] = SPECIAL;

        if (Creature* pCreature = GetSingleCreatureFromStorage(NPC_KERISTRASZA))
        {
            if (pCreature->isAlive())
            {
                pCreature->RemoveAurasDueToSpell(SPELL_FROZEN_PRISON);
                pCreature->RemoveFlag(UNIT_FIELD_FLAGS,UNIT_FLAG_OOC_NOT_ATTACKABLE);
            }
        }
    }

    if (uiData == DONE)
    {
        OUT_SAVE_INST_DATA;

        std::ostringstream saveStream;
        saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1] << " " << m_auiEncounter[2] << " " << m_auiEncounter[3];

        m_strInstData = saveStream.str();

        SaveToDB();
        OUT_SAVE_INST_DATA_COMPLETE;
    }
}

void instance_nexus::Load(const char* chrIn)
{
    if (!chrIn)
    {
        OUT_LOAD_INST_DATA_FAIL;
        return;
    }

    OUT_LOAD_INST_DATA(chrIn);

    std::istringstream loadStream(chrIn);
    loadStream >> m_auiEncounter[0] >> m_auiEncounter[1] >> m_auiEncounter[2] >> m_auiEncounter[3];

    for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
    {
        if (m_auiEncounter[i] == IN_PROGRESS)
            m_auiEncounter[i] = NOT_STARTED;
    }

    OUT_LOAD_INST_DATA_COMPLETE;
}

InstanceData* GetInstanceData_instance_nexus(Map* pMap)
{
    return new instance_nexus(pMap);
}

void AddSC_instance_nexus()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "instance_nexus";
    pNewScript->GetInstanceData = &GetInstanceData_instance_nexus;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "go_containment_sphere";
    pNewScript->pGOUse = &GOUse_go_containment_sphere;
    pNewScript->RegisterSelf();
}
