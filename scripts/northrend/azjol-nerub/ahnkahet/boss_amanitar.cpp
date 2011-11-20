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
SDName: Boss_Amanitar
SD%Complete: 90%
SDComment: maybe better formation for Mushrooms
SDCategory: Ahn'kahet
EndScriptData */

#include "precompiled.h"
#include "ahnkahet.h"

enum
{
    NPC_HEALTHY_MUSHROOM            = 30391,
    NPC_POISONOUS_MUSHROOM          = 30435,

    SPELL_BASH                      = 57094,
    SPELL_ENTANGLING_ROOTS          = 57095,
    SPELL_MINI                      = 57055,
    SPELL_VENOM_BOLT_VOLLEY         = 57088,

    SPELL_POTENT_FUNGUS             = 56648,
    SPELL_POISON_CLOUD              = 57061,
    SPELL_POISONOUS_MUSHROOM_VISUAL = 56741,
    SPELL_POISONOUS_MUSHROOM_VISUAL2= 61566,

    SPELL_MUSHROOM_MODEL            = 31690
};

/*######
## boss_amanitar
######*/

struct MANGOS_DLL_DECL boss_amanitarAI : public ScriptedAI
{
    boss_amanitarAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_ahnkahet*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_ahnkahet* m_pInstance;
    bool m_bIsRegularMode;

    uint32 spawnMushroomTimer;
    uint32 miniTimer;
    uint32 bashTimer;
    uint32 venomBoltVolleyTimer;
    uint32 entanglingRootsTimer;
    GUIDList m_lMushroomGUIDList;

    void Reset()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_AMANITAR, NOT_STARTED);
        RemoveMiniAura();
        despawnMushroom();
        spawnMushroomTimer = 1000;
        miniTimer = urand(20000, 22000);
        bashTimer = urand(10000, 11000);
        entanglingRootsTimer = urand(12000, 15000);
        venomBoltVolleyTimer = urand(4000, 6000);
    }

    void despawnMushroom()
    {
        if (!m_lMushroomGUIDList.empty())
            for(GUIDList::iterator itr = m_lMushroomGUIDList.begin(); itr != m_lMushroomGUIDList.end(); ++itr)
                if (Creature* pTemp = (Creature*)m_creature->GetMap()->GetUnit( *itr))
                    if (pTemp->isAlive())
                        pTemp->ForcedDespawn();
        m_lMushroomGUIDList.clear();
    }

    void spawnMushroom()
    {
        for (int i = 0; i < 8;++i)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                float posX = rand()%30;
                float posY = rand()%30;
                posX = pTarget->GetPositionX() + (urand(0,1)? (posX) : (-posX));
                posY = pTarget->GetPositionY() + (urand(0,1)? (posY) : (-posY));
                float posZ = pTarget->GetTerrain()->GetWaterOrGroundLevel(posX,posY,pTarget->GetPositionZ()+3);

                if (Creature* pMushroom = m_creature->SummonCreature(roll_chance_i(75) ? NPC_POISONOUS_MUSHROOM : NPC_HEALTHY_MUSHROOM, posX, posY, posZ, m_creature->GetOrientation(), TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 60000))
                    m_lMushroomGUIDList.push_back(pMushroom->GetObjectGuid());
            }
        }
    }

    void RemoveMiniAura()
    {
        Map::PlayerList const& plList = m_pInstance->instance->GetPlayers();

        if(plList.isEmpty())
            return;

        for(Map::PlayerList::const_iterator ittr = plList.begin(); ittr != plList.end(); ++ittr)
        {
            if(ittr->getSource())
            {
                ittr->getSource()->RemoveAurasDueToSpell(SPELL_MINI);
            }
        }
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_AMANITAR, IN_PROGRESS);
        for (int i = 0; i < 3; ++i)
            spawnMushroom();
    }
    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_AMANITAR, FAIL);
    }

    void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_AMANITAR, DONE);
        RemoveMiniAura();
        despawnMushroom();
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (spawnMushroomTimer < uiDiff)
        {
            spawnMushroom();
            spawnMushroomTimer = 15000;
        }
        else
            spawnMushroomTimer -= uiDiff;

        if (miniTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0), SPELL_MINI) == CAST_OK)
                miniTimer = urand(20000, 25000);
        }
        else
            miniTimer -= uiDiff;

        if (bashTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_BASH) == CAST_OK)
                bashTimer = urand(9000, 13000);
        }
        else
            bashTimer -= uiDiff;

        if (venomBoltVolleyTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0), SPELL_VENOM_BOLT_VOLLEY) == CAST_OK)
                venomBoltVolleyTimer = urand(8000, 11000);
        }
        else
            venomBoltVolleyTimer -= uiDiff;

        if (entanglingRootsTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0), SPELL_ENTANGLING_ROOTS) == CAST_OK)
                entanglingRootsTimer = urand(12000, 15000);
        }
        else
            entanglingRootsTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_amanitar(Creature* pCreature)
{
    return new boss_amanitarAI(pCreature);
}

/*######
## npc_amanitar_healthy_mushroom
######*/

struct MANGOS_DLL_DECL npc_amanitar_healthy_mushroomAI : public ScriptedAI
{
    npc_amanitar_healthy_mushroomAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        SetCombatMovement(false);
        Reset();
    }
    
    void SpellHit(Unit* pTarget, const SpellEntry* pSpell) 
    {
        if (pSpell->Id == SPELL_POTENT_FUNGUS)
        {
            pTarget->RemoveAurasDueToSpell(SPELL_MINI);            
        }
    }

    void Reset() {}

    void JustDied(Unit* pWho)
    {
        m_creature->CastSpell(m_creature, SPELL_POTENT_FUNGUS, true);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        DoCastSpellIfCan(m_creature,SPELL_MUSHROOM_MODEL, CAST_AURA_NOT_PRESENT);
    }
};

CreatureAI* GetAI_npc_amanitar_healthy_mushroom(Creature* pCreature)
{
    return new npc_amanitar_healthy_mushroomAI(pCreature);
}

/*######
## npc_amanitar_poisonous_mushroom
######*/

struct MANGOS_DLL_DECL npc_amanitar_poisonous_mushroomAI : public ScriptedAI
{
    npc_amanitar_poisonous_mushroomAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        SetCombatMovement(false);
        Reset();
    }

    uint32 poisenCloudTimer;

    void Reset()
    {        
        poisenCloudTimer = urand(400,800);
    }
 
    void UpdateAI(const uint32 uiDiff)
    {
        DoCastSpellIfCan(m_creature,SPELL_POISONOUS_MUSHROOM_VISUAL, CAST_AURA_NOT_PRESENT);
        DoCastSpellIfCan(m_creature,SPELL_MUSHROOM_MODEL, CAST_AURA_NOT_PRESENT);
        if (poisenCloudTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_POISON_CLOUD) == CAST_OK)
            {
                DoCast(m_creature,SPELL_POISONOUS_MUSHROOM_VISUAL2,true);
                poisenCloudTimer = urand(2000,5000);
            }
        }
        else
            poisenCloudTimer -= uiDiff;
    }
};

CreatureAI* GetAI_npc_amanitar_poisonous_mushroom(Creature* pCreature)
{
    return new npc_amanitar_poisonous_mushroomAI(pCreature);
}

void AddSC_boss_amanitar()
{
    Script *pNewscript;

    pNewscript = new Script;
    pNewscript->Name = "boss_amanitar";
    pNewscript->GetAI = &GetAI_boss_amanitar;
    pNewscript->RegisterSelf();

    pNewscript = new Script;
    pNewscript->Name = "npc_amanitar_healthy_mushroom";
    pNewscript->GetAI = &GetAI_npc_amanitar_healthy_mushroom;
    pNewscript->RegisterSelf();

    pNewscript = new Script;
    pNewscript->Name = "npc_amanitar_poisonous_mushroom";
    pNewscript->GetAI = &GetAI_npc_amanitar_poisonous_mushroom;
    pNewscript->RegisterSelf();
}