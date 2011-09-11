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
SDName: Boss_Colossus
SD%Complete: 20%
SDComment:
SDCategory: Gundrak
EndScriptData */

#include "precompiled.h"
#include "gundrak.h"
#include <math.h> 

enum
{
    EMOTE_SURGE                 = -1604008,
    EMOTE_SEEP                  = -1604009,
    EMOTE_GLOW                  = -1604010,

    // collosus' abilities
    SPELL_EMERGE                = 54850,
    SPELL_MIGHTY_BLOW           = 54719,
    SPELL_MORTAL_STRIKES        = 54715,
    SPELL_MORTAL_STRIKES_H      = 59454,

    // elemental's abilities
    SPELL_MERGE                 = 54269,
    SPELL_SURGE                 = 54801,    // TESTING: sometimes under map
    SPELL_MOJO_VOLLEY           = 59453,
    SPELL_MOJO_VOLLEY_H         = 54849
};

enum ColossusPhases
{
    PHASE_START     = 0,
    PHASE_NORMAL    = 1,
    PHASE_ELEMENTAR = 2
};

/*######
## boss_colossus
######*/

struct MANGOS_DLL_DECL boss_colossusAI : public ScriptedAI
{
    boss_colossusAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_gundrak*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_gundrak* m_pInstance;
    bool m_bIsRegularMode;

    bool m_bElementsHasMoved;
    uint32 m_uiMojoDieTimer;

    uint8  m_uiPhase;

    uint32 m_uiMightyBlowTimer;
    uint32 m_uiEmergeTimer;

    void Reset()
    {
        SetCombatMovement(false);
        m_uiPhase = PHASE_START;
        m_uiEmergeTimer = 12000;
        m_uiMightyBlowTimer = 3000;
        
        m_uiMojoDieTimer = 4000;
        m_bElementsHasMoved = false;
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_COLOSSUS, IN_PROGRESS);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_COLOSSUS, FAIL);
    }

    void JustDied(Unit* pWho)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_COLOSSUS, DONE);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
    }

    void KillElementals()
    {
        if (m_pInstance)
        {
            for(GUIDList::iterator itr = m_pInstance->m_lLivingMojoGUIDList.begin(); itr != m_pInstance->m_lLivingMojoGUIDList.end(); ++itr)
            {
                if (Creature* pMojo = m_pInstance->instance->GetCreature(*itr))
                {
                    if (pMojo->isAlive())
                    {
                        m_creature->DealDamage(pMojo, pMojo->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false); 
                    }
                }
            }
        }
    }

    void SpellHit(Unit* pCaster, const SpellEntry* pSpell)
    {
        if (pSpell->Id == SPELL_MERGE)
        {
            m_uiPhase = PHASE_NORMAL;
            SetCombatMovement(true);
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        switch (m_uiPhase)
        {
            case PHASE_START:
                if (!m_bElementsHasMoved)
                {
                    if (m_pInstance)
                    {
                        for(GUIDList::iterator itr = m_pInstance->m_lLivingMojoGUIDList.begin(); itr != m_pInstance->m_lLivingMojoGUIDList.end(); ++itr)
                        {
                            if (Creature* pMojo = m_pInstance->instance->GetCreature(*itr))
                            {
                                if (pMojo->isAlive())
                                {
                                    pMojo->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PASSIVE | UNIT_FLAG_NON_ATTACKABLE);
                                    pMojo->GetMotionMaster()->MovePoint(0, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ());                                    
                                }
                            }
                        }
                        m_bElementsHasMoved = true;
                    }
                }
                else
                {
                    if (m_uiMojoDieTimer < uiDiff)
                    {
                        KillElementals();
                        SetCombatMovement(true);
                        m_uiPhase = PHASE_NORMAL;
                    }
                    else
                        m_uiMojoDieTimer -= uiDiff;
                }
                break;
            case PHASE_NORMAL:
            {
                DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_MORTAL_STRIKES : SPELL_MORTAL_STRIKES_H, CAST_TRIGGERED | CAST_AURA_NOT_PRESENT);

                if (m_uiMightyBlowTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_MIGHTY_BLOW) == CAST_OK)
                        m_uiMightyBlowTimer = urand(14000, 18000);
                }
                else
                    m_uiMightyBlowTimer -= uiDiff;

                if (m_uiEmergeTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature, SPELL_EMERGE) == CAST_OK)
                    {
                        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                        SetCombatMovement(false);
                        m_uiPhase = PHASE_ELEMENTAR;
                        m_uiEmergeTimer = 15000;
                        return;
                    }
                }
                else
                    m_uiEmergeTimer -= uiDiff;

                DoMeleeAttackIfReady();
                break;
            }            
            case PHASE_ELEMENTAR:
            {
                // Do nothing
                break;
            }
            
        }
    }
};

CreatureAI* GetAI_boss_colossus(Creature* pCreature)
{
    return new boss_colossusAI(pCreature);
}

struct MANGOS_DLL_DECL mob_colossus_elementalAI : public ScriptedAI
{
    mob_colossus_elementalAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_gundrak*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_gundrak* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiMergeTimer;
    uint32 m_uiSurgeTimer;

    void Reset()
    {
        m_uiMergeTimer = 25000;
        m_uiSurgeTimer = 10000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_MOJO_VOLLEY : SPELL_MOJO_VOLLEY_H, CAST_TRIGGERED | CAST_AURA_NOT_PRESENT);
        
        if (m_uiMergeTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_MERGE) == CAST_OK)
            {
                if (Creature * pColossus = m_pInstance->GetSingleCreatureFromStorage(NPC_COLOSSUS))
                {
                    if (boss_colossusAI * pColossusAI = (boss_colossusAI*)pColossus->AI())
                    {
                        pColossusAI->m_uiPhase = PHASE_NORMAL;
                        pColossusAI->SetCombatMovement(true);
                        pColossus->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                         m_uiMergeTimer = 25000;
                    }
                }               
            }
        }
        else
            m_uiMergeTimer -= uiDiff;

        if (m_uiSurgeTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM_PLAYER, 1))
            {
                m_creature->SetFacingToObject(pTarget);
                if (DoCastSpellIfCan(pTarget, SPELL_SURGE) == CAST_OK)
                {
                    m_uiSurgeTimer = 10000;
                }
            }
        }
        else
            m_uiSurgeTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_colossus_elemental(Creature* pCreature)
{
    return new mob_colossus_elementalAI(pCreature);
}

void AddSC_boss_colossus()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_colossus";
    newscript->GetAI = &GetAI_boss_colossus;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_colossus_elemental";
    newscript->GetAI = &GetAI_mob_colossus_elemental;
    newscript->RegisterSelf();
}
