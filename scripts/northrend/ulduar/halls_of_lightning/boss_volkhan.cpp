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
SDName: Boss Volkhan
SD%Complete: 90%
SDComment: does not move to anvil correctly
SDCategory: Halls of Lightning
EndScriptData */

#include "precompiled.h"
#include "halls_of_lightning.h"

enum
{
    SAY_AGGRO                               = -1602032,
    SAY_SLAY_1                              = -1602033,
    SAY_SLAY_2                              = -1602034,
    SAY_SLAY_3                              = -1602035,
    SAY_DEATH                               = -1602036,
    SAY_STOMP_1                             = -1602037,
    SAY_STOMP_2                             = -1602038,
    SAY_FORGE_1                             = -1602039,
    SAY_FORGE_2                             = -1602040,
    EMOTE_TO_ANVIL                          = -1602041,
    EMOTE_SHATTER                           = -1602042,

    SPELL_HEAT_N                            = 52387,
    SPELL_HEAT_H                            = 59528,
    SPELL_SHATTERING_STOMP_N                = 52237,
    SPELL_SHATTERING_STOMP_H                = 59529,

    //unclear how "directions" of spells must be. Last, summoning GO, what is it for? Script depend on:
    SPELL_TEMPER                            = 52238,        //TARGET_SCRIPT boss->anvil
    SPELL_TEMPER_DUMMY                      = 52654,        //TARGET_SCRIPT anvil->boss

    //SPELL_TEMPER_VISUAL                     = 52661,        //summons GO

    SPELL_SUMMON_MOLTEN_GOLEM               = 52405,

    //Molten Golem
    SPELL_BLAST_WAVE                        = 23113,
    SPELL_IMMOLATION_STRIKE_N               = 52433,
    SPELL_IMMOLATION_STRIKE_H               = 59530,
    SPELL_SHATTER_N                         = 52429,
    SPELL_SHATTER_H                         = 59527,

    NPC_VOLKHAN_ANVIL                       = 28823,
    NPC_MOLTEN_GOLEM                        = 28695,
    NPC_BRITTLE_GOLEM                       = 28681,

    POINT_ID_ANVIL                          = 0,
    MAX_GOLEM                               = 2,

    ACHIEVEMENT_SHATTER_RESISTANT           = 2042
};

/*######
## Boss Volkhan
######*/

struct MANGOS_DLL_DECL boss_volkhanAI : public ScriptedAI
{
    boss_volkhanAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_halls_of_lightning*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_halls_of_lightning* m_pInstance;

    GUIDList m_lGolemGUIDList;

    bool m_bIsRegularMode;
    bool m_bMussShatter;

    uint32 m_uiPause_Timer;
    uint32 m_uiShatter_Counter;
    uint32 m_uiMussShatterTimer;
    uint32 m_uiHealthAmountModifier;

    uint8  m_uiArchivCounter;

    void Reset()
    {
        m_bMussShatter      = false;
        m_uiMussShatterTimer= 3000;
        m_uiShatter_Counter = 0;
        m_uiArchivCounter   = 0;

        m_uiHealthAmountModifier = 0;

        DespawnGolem();

        if (m_pInstance)
            m_pInstance->SetData(TYPE_VOLKHAN, NOT_STARTED);
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_VOLKHAN, IN_PROGRESS);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);
        DespawnGolem();


        if (m_pInstance)
            if (m_uiArchivCounter < 5)
            {
                m_pInstance->DoCompleteAchievement(ACHIEVEMENT_SHATTER_RESISTANT);
            }
            m_pInstance->SetData(TYPE_VOLKHAN, DONE);
    }

    void KilledUnit(Unit* pVictim)
    {
        switch(urand(0, 2))
        {
            case 0: DoScriptText(SAY_SLAY_1, m_creature); break;
            case 1: DoScriptText(SAY_SLAY_2, m_creature); break;
            case 2: DoScriptText(SAY_SLAY_3, m_creature); break;
        }
    }

    void DespawnGolem()
    {
        if (m_lGolemGUIDList.empty())
            return;

        for(GUIDList::iterator itr = m_lGolemGUIDList.begin(); itr != m_lGolemGUIDList.end(); ++itr)
        {
            if (Creature* pTemp = m_creature->GetMap()->GetCreature(*itr))
            {
                pTemp->ForcedDespawn();
            }
        }
        m_lGolemGUIDList.clear();
    }


    void SpellHit(Unit* pCaster, const SpellEntry* pSpell)
    {
        if (pSpell->Id == SPELL_TEMPER){
            if (m_creature->GetMotionMaster()->GetCurrentMovementGeneratorType() != CHASE_MOTION_TYPE)
            {
            	if (m_creature->getVictim())
            		m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
            }
        }
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (pSummoned->GetEntry() == NPC_MOLTEN_GOLEM)
        {
            m_lGolemGUIDList.push_back(pSummoned->GetGUID());
            pSummoned->SetInCombatWithZone();
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(m_creature->getVictim()->GetEntry() == NPC_BRITTLE_GOLEM)
            DespawnGolem();

        if (m_creature->GetHealthPercent() <= (80 - 20 * m_uiShatter_Counter))
        {
            if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_SHATTERING_STOMP_N : SPELL_SHATTERING_STOMP_H) == CAST_OK)
            {
                m_bMussShatter = true;
                DoScriptText(urand(0, 1) ? SAY_STOMP_1 : SAY_STOMP_2, m_creature);
                DoScriptText(EMOTE_SHATTER, m_creature);
                m_uiShatter_Counter++;
            }
        }
        if (m_bMussShatter)
        {
            if (m_uiMussShatterTimer < uiDiff)
            {
                m_bMussShatter = false;
                for(GUIDList::iterator itr = m_lGolemGUIDList.begin(); itr != m_lGolemGUIDList.end(); ++itr)
                {
                    if (Creature* pTemp = m_creature->GetMap()->GetCreature(*itr))
                    {
                        if (pTemp->GetEntry() == NPC_BRITTLE_GOLEM)
                            ++m_uiArchivCounter;
                        pTemp->CastSpell(pTemp, m_bIsRegularMode ? SPELL_SHATTER_N : SPELL_SHATTER_H, true);
                        pTemp->ForcedDespawn(500);
                    }
                }
                m_lGolemGUIDList.clear();
                m_uiMussShatterTimer = 3000;
            }
            else
                m_uiMussShatterTimer -= uiDiff;
        }

        if ((m_creature->GetHealthPercent() < float(90 - 20*m_uiHealthAmountModifier)) > 10.0f)
        {
        	if (DoCastSpellIfCan(m_creature,SPELL_TEMPER, false) == CAST_OK){
        		++m_uiHealthAmountModifier;

        		DoScriptText(urand(0, 1) ? SAY_FORGE_1 : SAY_FORGE_2, m_creature);
        	}
        }

        DoMeleeAttackIfReady();
    }
};


CreatureAI* GetAI_boss_volkhan(Creature* pCreature)
{
    return new boss_volkhanAI(pCreature);
}

bool EffectDummyCreature_boss_volkhan(Unit* pCaster, uint32 uiSpellId, SpellEffectIndex uiEffIndex, Creature* pCreatureTarget)
{
    //always check spellid and effectindex
    if (uiSpellId == SPELL_TEMPER_DUMMY && uiEffIndex == EFFECT_INDEX_0)
    {
        if (pCaster->GetEntry() != NPC_VOLKHAN_ANVIL || pCreatureTarget->GetEntry() != NPC_VOLKHAN)
            return true;

        for(uint8 i = 0; i < MAX_GOLEM; ++i)
        {
            pCreatureTarget->CastSpell(pCaster, SPELL_SUMMON_MOLTEN_GOLEM, true);
        }

        //always return true when we are handling this spell and effect
        return true;
    }

    return false;
}

/*######
## npc_volkhan_anvil
######*/

bool EffectDummyCreature_npc_volkhan_anvil(Unit* pCaster, uint32 uiSpellId, SpellEffectIndex uiEffIndex, Creature* pCreatureTarget)
{
    //always check spellid and effectindex
    if (uiSpellId == SPELL_TEMPER && uiEffIndex == EFFECT_INDEX_0)
    {
        if (pCaster->GetEntry() != NPC_VOLKHAN || pCreatureTarget->GetEntry() != NPC_VOLKHAN_ANVIL)
            return true;

        DoScriptText(EMOTE_TO_ANVIL, pCaster);

        float fX, fY, fZ;
        pCreatureTarget->GetContactPoint(pCaster, fX, fY, fZ, INTERACTION_DISTANCE);

        pCaster->AttackStop();

        if (pCaster->GetMotionMaster()->GetCurrentMovementGeneratorType() == CHASE_MOTION_TYPE)
            pCaster->GetMotionMaster()->MovementExpired();

        ((Creature*)pCaster)->MonsterMove(fX, fY, fZ, 1);

        pCreatureTarget->CastSpell(pCaster, SPELL_TEMPER_DUMMY, false);

        //always return true when we are handling this spell and effect
        return true;
    }

    return false;
}

/*######
## mob_molten_golem
######*/

struct MANGOS_DLL_DECL mob_molten_golemAI : public ScriptedAI
{
    mob_molten_golemAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_halls_of_lightning*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_halls_of_lightning* m_pInstance;

    bool m_bIsRegularMode;
    bool m_bIsFrozen;
    bool m_bHasHeal;

    uint32 m_uiBlast_Timer;
    uint32 m_uiDeathDelay_Timer;
    uint32 m_uiImmolation_Timer;

    void Reset()
    {
        m_bIsFrozen = false;

        m_uiBlast_Timer = 20000;
        m_uiDeathDelay_Timer = 0;
        m_uiImmolation_Timer = 5000;
    }

    void AttackStart(Unit* pWho)
    {
        if (m_bIsFrozen)
            return;
        ScriptedAI::AttackStart(pWho);
    }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        if(m_bIsFrozen)
        {
        	uiDamage = 0;
        }
        else if (uiDamage >= m_creature->GetHealth())
        {
            uiDamage = 0;
        	m_creature->UpdateEntry(NPC_BRITTLE_GOLEM);
        	m_creature->setFaction(35);
            m_creature->SetHealth(1);
            m_bIsFrozen = true;

            if (m_creature->IsNonMeleeSpellCasted(false))
                m_creature->InterruptNonMeleeSpells(false);

            m_creature->RemoveAllAuras();
            m_creature->AttackStop();
            m_creature->GetMotionMaster()->MoveIdle();
            SetCombatMovement(false);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //Return since we have no target or if we are frozen
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim() || m_bIsFrozen)
            return;

        if (!m_bHasHeal && m_creature->GetHealthPercent() < 70.0f)
        {
            DoCast(m_creature, m_bIsRegularMode ? SPELL_HEAT_N : SPELL_HEAT_H, true);
            m_bHasHeal = true;
        }

        if (m_uiBlast_Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_BLAST_WAVE);
            m_uiBlast_Timer = 20000;
        }
        else
            m_uiBlast_Timer -= uiDiff;

        if (m_uiImmolation_Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_IMMOLATION_STRIKE_N : SPELL_IMMOLATION_STRIKE_H);
            m_uiImmolation_Timer = 5000;
        }
        else
            m_uiImmolation_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_molten_golem(Creature* pCreature)
{
    return new mob_molten_golemAI(pCreature);
}

void AddSC_boss_volkhan()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_volkhan";
    newscript->GetAI = &GetAI_boss_volkhan;
    newscript->pEffectDummyNPC = &EffectDummyCreature_boss_volkhan;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_volkhan_anvil";
    newscript->pEffectDummyNPC = &EffectDummyCreature_npc_volkhan_anvil;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_molten_golem";
    newscript->GetAI = &GetAI_mob_molten_golem;
    newscript->RegisterSelf();
}
