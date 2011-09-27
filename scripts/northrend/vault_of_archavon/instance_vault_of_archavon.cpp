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
SDName: Instance_Vault_of_Archavon
SD%Complete: 0
SDComment:
SDCategory: Vault of Archavon
EndScriptData */

#include "precompiled.h"
#include "vault_of_archavon.h"


instance_vault_of_archavon::instance_vault_of_archavon(Map* pMap) : ScriptedInstance(pMap)
{
        Initialize();
}

void instance_vault_of_archavon::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
}

void instance_vault_of_archavon::OnCreatureCreate(Creature* pCreature)
{
    switch (pCreature->GetEntry())
    {
        case NPC_TORAVON:
        case NPC_ARCHAVON:
        case NPC_EMALON:
        case NPC_KORALON:
            m_mNpcEntryGuidStore[pCreature->GetEntry()] = pCreature->GetObjectGuid();
            break;
        case NPC_TEMPEST_MINION:
            m_lTempestMinion.push_back(pCreature->GetObjectGuid());
            break;
    }
}

void instance_vault_of_archavon::SetData(uint32 uiType, uint32 uiData)
{
    m_auiEncounter[uiType] = uiData;
    switch (uiType)
    {
        case TYPE_EMALON:
        {
            if (uiData == DONE)
            {
                for (GUIDList::iterator itr = m_lTempestMinion.begin(); itr !=m_lTempestMinion.end(); ++itr)
                {
                    if (Creature* pMinion = instance->GetCreature(*itr))
                    {
                        pMinion->DealDamage(pMinion, pMinion->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                    }
                }
            }
            else if (uiData == FAIL)
            {
                for (GUIDList::iterator itr = m_lTempestMinion.begin(); itr !=m_lTempestMinion.end(); ++itr)
                {
                    if (Creature* pMinion = instance->GetCreature(*itr))
                    {
                        pMinion->Respawn();
                    }
                }
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

uint32 instance_vault_of_archavon::GetData(uint32 uiType)
{
    switch (uiType)
    {
        case TYPE_ARCHAVON:
            return m_auiEncounter[0];
        case TYPE_EMALON:
            return m_auiEncounter[1];
        case TYPE_KORALON:
            return m_auiEncounter[2];
        case TYPE_TORAVON:
            return m_auiEncounter[3];
    }
    return 0;
}

void instance_vault_of_archavon::Load(const char* chrIn)
{
    if (!chrIn)
    {
        OUT_LOAD_INST_DATA_FAIL;
        return;
    }

    OUT_LOAD_INST_DATA(chrIn);

    std::istringstream loadStream(chrIn);
    loadStream >> m_auiEncounter[0] >> m_auiEncounter[1] >> m_auiEncounter[2] >> m_auiEncounter[3];

    for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
    {
        if (m_auiEncounter[i] == IN_PROGRESS)
            m_auiEncounter[i] = NOT_STARTED;
    }

    OUT_LOAD_INST_DATA_COMPLETE;
}

bool instance_vault_of_archavon::IsEncounterInProgress() const
{
    for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
        if (m_auiEncounter[i] == IN_PROGRESS)
            return true;
    return false;
}


InstanceData* GetInstanceData_instance_vault_of_archavon(Map* pMap)
{
    return new instance_vault_of_archavon(pMap);
}

void AddSC_instance_vault_of_archavon()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_vault_of_archavon";
    newscript->GetInstanceData = &GetInstanceData_instance_vault_of_archavon;
    newscript->RegisterSelf();
}
