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
SDName: Boss_Emalon_The_Storm_Watcher
SD%Complete: 0%
SDComment:
SDCategory: Vault of Archavon
EndScriptData */

#include "precompiled.h"
#include "vault_of_archavon.h"

enum
{
    // Emalon spells
    SPELL_CHAIN_LIGHTNING_N                 = 64213,
    SPELL_CHAIN_LIGHTNING_H                 = 64215,
    SPELL_LIGHTNING_NOVA_N                  = 64216,
    SPELL_LIGHTNING_NOVA_H                  = 65279,
    SPELL_OVERCHARGE                        = 64218,
    SPELL_BERSERK                           = 26662,

    // Tempest Minion spells
    SPELL_SHOCK                             = 64363,
    SPELL_OVERCHARGED_BLAST                 = 64219,
    SPELL_OVERCHARGED                       = 64379
};

#define MIDDLE_X -219.119f
#define MIDDLE_Y -289.037f

/*######
## npc_tempest_minion
######*/

struct MANGOS_DLL_DECL npc_tempest_minionAI : public ScriptedAI
{
    npc_tempest_minionAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_vault_of_archavon*)pCreature->GetInstanceData();
        Reset();
    }

    instance_vault_of_archavon* m_pInstance;
    uint32 m_uiEvadeCheckCooldown;
    
    uint32 m_uiShockTimer;
    uint32 m_uiRespawnTimer;
    float m_fDiePositionX;
    float m_fDiePositionY;
    float m_fDiePositionZ;
    float m_fDieOrientation;

    bool IsDead;

    void Reset()
    {
        IsDead = false;
        m_uiEvadeCheckCooldown = 2000;
        m_uiShockTimer = 8000+rand()%4000;
        m_uiRespawnTimer = 4000;

        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
    }

    void Aggro(Unit* pWho)
    {
        m_creature->CallForHelp(80.0f);
    }

    void JustDied(Unit* pKiller)
    {
        IsDead = true;
        m_fDiePositionX = m_creature->GetPositionX();
        m_fDiePositionY = m_creature->GetPositionY();
        m_fDiePositionZ = m_creature->GetPositionZ();
        m_fDieOrientation = m_creature->GetOrientation();
        m_uiRespawnTimer = 4000;
    }

    void JustRespawned()
    {
        IsDead = false;
        m_creature->Relocate(m_fDiePositionX, m_fDiePositionY, m_fDiePositionZ + 1.0f, m_fDieOrientation);
        m_creature->MonsterTextEmote("%s appears to defend Emalon!", NULL, true);
        m_creature->SetInCombatWithZone();
        Reset();
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiEvadeCheckCooldown < uiDiff)
        {
            Creature* pEmalon = m_pInstance->GetSingleCreatureFromStorage(NPC_EMALON);
            if ((pEmalon && pEmalon->IsInEvadeMode()) || (m_creature->GetDistance2d(MIDDLE_X, MIDDLE_Y) > 80.0f))
            {
                EnterEvadeMode();
                return;
            }
            m_uiEvadeCheckCooldown = 2000;
        }
        else
            m_uiEvadeCheckCooldown -= uiDiff;

        if (m_uiShockTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_SHOCK);
            m_uiShockTimer = 8000+rand()%4000;
        }
        else
            m_uiShockTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

/*######
## boss_emalon
######*/

struct MANGOS_DLL_DECL boss_emalonAI : public ScriptedAI
{
    boss_emalonAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_vault_of_archavon*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_vault_of_archavon* m_pInstance;
    bool m_bIsRegularMode;
    uint32 m_uiEvadeCheckCooldown;

    uint32 m_uiChainLightningTimer;
    uint32 m_uiLightningNovaTimer;
    uint32 m_uiOverchargeTimer;
    uint32 m_uiEnrageTimer;

    uint32 m_uiRespawnTimerForAdds;

    void Reset()
    {
        m_uiEvadeCheckCooldown = 2000;
        m_uiChainLightningTimer = 15000;
        m_uiLightningNovaTimer = 20000;
        m_uiOverchargeTimer = 45000;
        m_uiEnrageTimer = 360000;

        m_uiRespawnTimerForAdds = 4000;
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_EMALON, FAIL);
    }

    void Aggro(Unit* pWho)
    {
        m_creature->CallForHelp(80.0f);
        if (m_pInstance)
            m_pInstance->SetData(TYPE_EMALON, IN_PROGRESS);
    }

    void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_EMALON, DONE);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiEvadeCheckCooldown < uiDiff)
        {
            if (m_creature->GetDistance2d(MIDDLE_X, MIDDLE_Y) > 80.0f)
                EnterEvadeMode();
            m_creature->CallForHelp(80.0f);
            m_uiEvadeCheckCooldown = 2000;
        }
        else
            m_uiEvadeCheckCooldown -= uiDiff;

        if (m_uiOverchargeTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_OVERCHARGE) == CAST_OK)
                m_uiOverchargeTimer = 45000;
        }
        else
            m_uiOverchargeTimer -= uiDiff;

        if (m_uiChainLightningTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                if (DoCastSpellIfCan(pTarget, m_bIsRegularMode ? SPELL_CHAIN_LIGHTNING_N : SPELL_CHAIN_LIGHTNING_H) == CAST_OK)
                {
                    m_uiChainLightningTimer = 10000 + rand()%15000;
                }
            }
        }
        else
            m_uiChainLightningTimer -= uiDiff;

        if (m_uiLightningNovaTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_LIGHTNING_NOVA_N : SPELL_LIGHTNING_NOVA_H) == CAST_OK)
                m_uiLightningNovaTimer = 45000;
        }
        else
            m_uiLightningNovaTimer -= uiDiff;

        if (m_uiRespawnTimerForAdds < uiDiff)
        {
            if (m_pInstance)
            {
                for(GUIDList::iterator itr = m_pInstance->m_lTempestMinion.begin(); itr != m_pInstance->m_lTempestMinion.end(); ++itr)
                {
                    if (Creature* pMinion = m_creature->GetMap()->GetCreature(*itr))
                    {
                        if (pMinion->isDead())
                        {
                            pMinion->Respawn();
                        }
                    }
                }
            }
            m_uiRespawnTimerForAdds = 4000;
        }
        else
            m_uiRespawnTimerForAdds -= uiDiff;

        if (m_uiEnrageTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_BERSERK, CAST_TRIGGERED) == CAST_OK)
                m_uiEnrageTimer = 30000;
        }
        else
            m_uiEnrageTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

/*######
## npc_tempest_warder
######*/

struct MANGOS_DLL_DECL npc_tempest_warderAI : public ScriptedAI
{
    npc_tempest_warderAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_vault_of_archavon*)pCreature->GetInstanceData();
        Reset();
    }

    instance_vault_of_archavon* m_pInstance;

    uint32 m_uiShockTimer;


    void Reset()
    {
        m_uiShockTimer = 8000+rand()%4000;
    }

    void UpdateAI(const uint32 uiDiff)
    {  
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoCastSpellIfCan(m_creature, SPELL_OVERCHARGED, CAST_TRIGGERED | CAST_AURA_NOT_PRESENT);

        if (m_uiShockTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_SHOCK);
            m_uiShockTimer = 8000+rand()%4000;
        }
        else
            m_uiShockTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_emalonAI(Creature* pCreature)
{
    return new boss_emalonAI(pCreature);
}

CreatureAI* GetAI_npc_tempest_minionAI(Creature* pCreature)
{
    return new npc_tempest_minionAI(pCreature);
}

CreatureAI* GetAI_npc_tempest_warderAI(Creature* pCreature)
{
    return new npc_tempest_warderAI(pCreature);
}

void AddSC_boss_emalon()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_emalon";
    newscript->GetAI = &GetAI_boss_emalonAI;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_tempest_minion";
    newscript->GetAI = &GetAI_npc_tempest_minionAI;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_tempest_warder";
    newscript->GetAI = &GetAI_npc_tempest_warderAI;
    newscript->RegisterSelf();
}
