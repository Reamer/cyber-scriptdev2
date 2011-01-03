/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Instance_Dire_Maul
SD%Complete: 50%
SDComment: Doorhandling in Dire_Maul
SDCategory: Dire Maul
EndScriptData */

#include "precompiled.h"
struct Encounter
{
    uint64 GUID;
	bool dead;
};
enum
{
	TYPE_CRYSTAL_GENERATOR_01		= 1,
	TYPE_CRYSTAL_GENERATOR_02		= 2,
	TYPE_CRYSTAL_GENERATOR_03		= 3,
	TYPE_CRYSTAL_GENERATOR_04		= 4,
	TYPE_CRYSTAL_GENERATOR_05		= 5,
	DIRE_MAUL_CRYSTAL_GENERATOR_01	= 177259,
	DIRE_MAUL_CRYSTAL_GENERATOR_02	= 177257,
	DIRE_MAUL_CRYSTAL_GENERATOR_03	= 177258,
	DIRE_MAUL_CRYSTAL_GENERATOR_04	= 179504,
	DIRE_MAUL_CRYSTAL_GENERATOR_05	= 179505
};

struct MANGOS_DLL_DECL instance_dire_maul : public ScriptedInstance
{
    instance_dire_maul(Map *pMap) : ScriptedInstance(pMap) {Initialize();};

	Encounter m_Encounter[6];
	uint32 m_uiGenerator[5];

	uint32  uiTimer;
	uint64  m_uiConservatoryDoorGUID;
	bool	m_bConservatoryDoorOPEN;
	uint64  m_uiCrumbleWallDoorGUID;
	bool	m_bCrumbleWallDoorOPEN;
	uint64	m_uiForceFieldDoorGUID;
	bool	m_bForceFieldDoorOPEN;

    void Initialize()
    {
		for (int i = 0; i < 6;++i)
		{
			m_Encounter[i].GUID 		= 0;
			m_Encounter[i].dead		= false;
		}
        m_uiConservatoryDoorGUID    = 0;
		m_bConservatoryDoorOPEN	= false;
		m_uiCrumbleWallDoorGUID		= 0;
		m_bCrumbleWallDoorOPEN		= false;
		m_uiForceFieldDoorGUID		= 0;
		uiTimer = 10000;
    }

    void OnCreatureCreate(Creature* pCreature)
    {
        switch (pCreature->GetEntry())
        {
			case 14354:										//Pusillin
				m_Encounter[0].dead = false;
				m_Encounter[0].GUID = pCreature->GetGUID();
				break;
			case 14349:										//Pimgib
				m_Encounter[1].dead = false;
				m_Encounter[1].GUID = pCreature->GetGUID();
				break;
			case 14327:										//Lethtendris
				m_Encounter[2].dead = false;
				m_Encounter[2].GUID = pCreature->GetGUID();
				break;
			case 13280:										//Hydrospawn
				m_Encounter[3].dead = false;
				m_Encounter[3].GUID = pCreature->GetGUID();
				break;
			case 11490:										//Zeorim Thornhoot
				m_Encounter[4].dead = false;
				m_Encounter[4].GUID = pCreature->GetGUID();
				break;
			case 11492:										//Alzzin the Wildshaper
				m_Encounter[5].dead = false;
				m_Encounter[5].GUID = pCreature->GetGUID();
				break;
        }
    }

    void OnObjectCreate(GameObject* pGo)
    {
        switch (pGo->GetEntry())
        {
            case 177220:										//Crumble Wall
                m_uiCrumbleWallDoorGUID = pGo->GetGUID();
                break;
            case 176907:										//Conservatory Door
                m_uiConservatoryDoorGUID = pGo->GetGUID();
                break;
			case 179503:										//Force Field
				m_uiForceFieldDoorGUID	= pGo->GetGUID();


        }
    }
	void Update(uint32 uiDiff)
	{
		//check every 10 seconds
		if (uiTimer < uiDiff)
		{
			if (!m_bCrumbleWallDoorOPEN)
				if (!m_Encounter[5].dead)
					if (Creature *creature = instance->GetCreature(m_Encounter[5].GUID))
						if (!creature->isAlive())
						{
							m_Encounter[5].dead = true;
							DoUseDoorOrButton(m_uiCrumbleWallDoorGUID);
							m_bCrumbleWallDoorOPEN = true;
						}

			if (!m_bConservatoryDoorOPEN)
			{
				for (int i = 0; i < 5; ++i)
				{
					if (!m_Encounter[i].dead)
						if (Creature *creature = instance->GetCreature(m_Encounter[i].GUID))
							if (!creature->isAlive())
							{
								m_Encounter[i].dead = true;
							}
				}
				if (m_Encounter[0].dead && m_Encounter[1].dead && m_Encounter[2].dead && m_Encounter[3].dead && m_Encounter[4].dead)
				{
					DoUseDoorOrButton(m_uiConservatoryDoorGUID);
					m_bConservatoryDoorOPEN = true;
				}				
			}
			uiTimer = 10000;
		}
		else
		{
				uiTimer -= uiDiff;
		}
	}

	void SetData(uint32 uiType, uint32 uiData)
    {
		switch(uiType)
        {
			case TYPE_CRYSTAL_GENERATOR_01: m_uiGenerator[0]	= uiData; break;
			case TYPE_CRYSTAL_GENERATOR_02:	m_uiGenerator[1]	= uiData; break;
			case TYPE_CRYSTAL_GENERATOR_03:	m_uiGenerator[2]	= uiData; break;
			case TYPE_CRYSTAL_GENERATOR_04:	m_uiGenerator[3]	= uiData; break;
			case TYPE_CRYSTAL_GENERATOR_05:	m_uiGenerator[4]	= uiData; break;
			default:
                error_log("SD2: Instance Dire Maul: ERROR SetData = %u for type %u does not exist/not implemented.", uiType, uiData);
				break;
		}
		if (m_uiGenerator[0] == SPECIAL && m_uiGenerator[1] == SPECIAL && m_uiGenerator[2] == SPECIAL && m_uiGenerator[3] == SPECIAL && m_uiGenerator[4] == SPECIAL)
			DoUseDoorOrButton(m_uiForceFieldDoorGUID);
        

	}
};

InstanceData* GetInstanceData_instance_dire_maul(Map* pMap)
{
    return new instance_dire_maul(pMap);
}

bool GOUse_go_dire_maul_generator(Player* pPlayer, GameObject* pGo)
{
    ScriptedInstance* pInstance = (ScriptedInstance*)pGo->GetInstanceData();

    if (!pInstance)
        return false;

    switch(pGo->GetEntry())
    {
        case DIRE_MAUL_CRYSTAL_GENERATOR_01	:	pInstance->SetData(TYPE_CRYSTAL_GENERATOR_01, SPECIAL); break;
        case DIRE_MAUL_CRYSTAL_GENERATOR_02	:	pInstance->SetData(TYPE_CRYSTAL_GENERATOR_02, SPECIAL); break;
        case DIRE_MAUL_CRYSTAL_GENERATOR_03	:	pInstance->SetData(TYPE_CRYSTAL_GENERATOR_03, SPECIAL); break;
		case DIRE_MAUL_CRYSTAL_GENERATOR_04	:	pInstance->SetData(TYPE_CRYSTAL_GENERATOR_04, SPECIAL); break;
		case DIRE_MAUL_CRYSTAL_GENERATOR_05	:	pInstance->SetData(TYPE_CRYSTAL_GENERATOR_05, SPECIAL); break;
    }
    return false;
}

void AddSC_instance_dire_maul()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_dire_maul";
    newscript->GetInstanceData = &GetInstanceData_instance_dire_maul;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = "go_dire_maul_generator";
    newscript->pGOUse = &GOUse_go_dire_maul_generator;
    newscript->RegisterSelf();
}
