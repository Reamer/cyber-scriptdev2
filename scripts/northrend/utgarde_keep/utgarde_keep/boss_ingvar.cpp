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
SDName: Boss_Ingvar
SD%Complete: 90%
SDComment: TODO: correct timers. Shadow axe need coreupdate
SDCategory: Utgarde Keep
EndScriptData */

#include "precompiled.h"
#include "utgarde_keep.h"

enum
{
    SAY_AGGRO_FIRST             = -1574005,
    SAY_AGGRO_SECOND            = -1574006,
    SAY_DEATH_FIRST             = -1574007,
    SAY_DEATH_SECOND            = -1574008,
    SAY_KILL_FIRST              = -1574009,
    SAY_KILL_SECOND             = -1574010,
    EMOTE_ROAR                  = -1574022,
    SAY_ANNHYLDE_REZ            = -1574023,

    NPC_ANNHYLDE                = 24068,
    NPC_THROW_TARGET            = 23996,                    //the target, casting spell and target of moving dummy
    NPC_THROW_DUMMY             = 23997,                    //the axe, moving to target
    NPC_INGVAR_ACHIEVEMENT      = 23980,                    //use this mob to apply archievment

    //phase 1
    SPELL_CLEAVE                = 42724,

    SPELL_SMASH                 = 42669,
    SPELL_SMASH_H               = 59706,

    SPELL_ENRAGE                = 42705,
    SPELL_ENRAGE_H              = 59707,

    SPELL_STAGGERING_ROAR       = 42708,
    SPELL_STAGGERING_ROAR_H     = 59708,

    //phase 2
    SPELL_DARK_SMASH            = 42723,

    SPELL_SHADOW_AXE_TRIGGER    = 42748,
    SPELL_SHADOW_AXE            = 42750,
    SPELL_SHADOW_AXE_H          = 59719,

    SPELL_DREADFUL_ROAR         = 42729,
    SPELL_DREADFUL_ROAR_H       = 59734,

    SPELL_WOE_STRIKE            = 42730,
    SPELL_WOE_STRIKE_H          = 59735,

    //ressurection sequenze
    SPELL_FEIGN_DEATH           = 42795,
    SPELL_TRANSFORM             = 42796,
    SPELL_SCOURGE_RES_SUMMON    = 42863,                    //summones a dummy target
    SPELL_SCOURGE_RES_HEAL      = 42704,                    //heals max HP
    SPELL_SCOURGE_RES_BUBBLE    = 42862,                    //black bubble
    SPELL_SCOURGE_RES_CHANNEL   = 42857,                    //the whirl from annhylde
    SPELL_SPOTLIGHT             = 62897                     //visual spotligth (not correct spell)
};

#define PHASE_1_DISPLAY_ID      21953     
#define PHASE_2_DISPLAY_ID      26351

#define FAC_FRIENDLY            35

/*######
## boss_ingvar
######*/

struct MANGOS_DLL_DECL boss_ingvarAI : public ScriptedAI
{
    boss_ingvarAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    bool m_bIsResurrected;
    bool m_bRescureInProgress;

    uint32 m_uiCleaveTimer;
    uint32 m_uiSmashTimer;
    uint32 m_uiStaggeringRoarTimer;
    uint32 m_uiEnrageTimer;
    uint32 m_uiDreadfulRoarTimer;
    uint32 m_uiDarkSmash;
    uint32 m_uiWoeStrike;
    uint32 m_uiShadowAxeTimer;
    uint32 m_uiSpotlightTimer;
    uint32 m_uiRescureTimer;
    uint32 m_uiRemoveBlackBubbleTimer;
    uint64 m_uiLastTargetDummyGUID;
    uint64 m_uiAxeDummyGUID;

    void Reset()
    {
        m_bIsResurrected = false;
        m_bRescureInProgress = false;

        m_creature->SetDisplayId(PHASE_1_DISPLAY_ID);

        m_uiCleaveTimer = urand(5000, 7000);
        m_uiSmashTimer = urand(8000, 15000);
        m_uiStaggeringRoarTimer = urand(10000, 25000);
        m_uiEnrageTimer = 30000;
        m_uiShadowAxeTimer = 20000;
        m_uiLastTargetDummyGUID = 0;
        m_uiAxeDummyGUID = 0;
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(m_bIsResurrected ? SAY_AGGRO_SECOND : SAY_AGGRO_FIRST, m_creature);
    }

    
    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        if (m_bIsResurrected)
            return;

        if (uiDamage >= m_creature->GetHealth())
        {
            uiDamage = m_creature->GetHealth() -1;

            /*m_creature->GetMotionMaster()->Clear(false);
            m_creature->GetMotionMaster()->MoveIdle();*/

            DoScriptText(SAY_DEATH_FIRST, m_creature);
            m_creature->RemoveAllAuras();
            m_creature->CastSpell(m_creature, SPELL_FEIGN_DEATH, true);
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            SetCombatMovement(false);

            m_bRescureInProgress = true;
            m_uiSpotlightTimer = 4000;
            m_uiRescureTimer = 9999999;
        }
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH_SECOND, m_creature);
        if (Player* pPlayerKiller = pKiller->GetCharmerOrOwnerPlayerOrPlayerItself())
            pPlayerKiller->RewardPlayerAndGroupAtEvent(NPC_INGVAR_ACHIEVEMENT, m_creature);
    }

    void KilledUnit(Unit* pVictim)
    {
        if (urand(0, 1))
            DoScriptText(m_bIsResurrected ? SAY_KILL_SECOND : SAY_KILL_FIRST, m_creature);
    }

    //void JustSummoned(Creature* pCreature)
    //{
    //    switch (pCreature->GetEntry())
    //    {
    //          case NPC_THROW_TARGET: // Target Dummy
    //              m_uiLastTargetDummyGUID = pCreature->GetGUID();
    //              break;
    //          case NPC_THROW_DUMMY: // Axe Dummy
    //              m_uiAxeDummyGUID = pCreature->GetGUID();
    //              if (Creature* pTarget = m_creature->GetMap()->GetCreature(m_uiLastTargetDummyGUID))
    //                  pCreature->GetMotionMaster()->MovePoint(1, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ());
    //              break;
    //     }
    //}

    //void MovementInform(uint32 type, uint32 id)
    //{
    //    if(type == POINT_MOTION_TYPE && id ==1)
    //    {
    //        if (Creature* pAxe = m_creature->GetMap()->GetCreature(m_uiAxeDummyGUID))
    //        {
    //            //SetCombatMovement(false);
    //            pAxe->CastSpell(pAxe, m_bIsRegularMode ? SPELL_SHADOW_AXE : SPELL_SHADOW_AXE_H, false);
    //        }
    //    }
    //}

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_bRescureInProgress)
        {
            if (m_uiSpotlightTimer < uiDiff)
            {
                if (Creature* pValkyr = m_creature->SummonCreature(NPC_ANNHYLDE, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ() + 5, m_creature->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN, 16000))
                    //pValkyr->setFaction(FAC_FRIENDLY);

                m_creature->CastSpell(m_creature, SPELL_SPOTLIGHT, true, 0, 0, ObjectGuid());
                m_uiRescureTimer = 14000;
                m_uiSpotlightTimer = 9999999;
            }else m_uiSpotlightTimer -= uiDiff;

            if (m_uiRescureTimer < uiDiff)
            {
                m_creature->RemoveAurasDueToSpell(SPELL_SPOTLIGHT, 0);
                m_creature->RemoveAurasDueToSpell(SPELL_FEIGN_DEATH, 0);
                m_creature->SetDisplayId(PHASE_2_DISPLAY_ID);
                m_creature->SetHealth(m_creature->GetMaxHealth());
                m_creature->CastSpell(m_creature, SPELL_SCOURGE_RES_HEAL, false);
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                SetCombatMovement(true);
                m_bRescureInProgress = false;
                m_bIsResurrected = true;
                m_uiRescureTimer = 9999999;
                m_uiDreadfulRoarTimer = 6000;
                m_uiDarkSmash = 9000;
                m_uiWoeStrike = 2000;
            }else m_uiRescureTimer -= uiDiff;

            return;
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (!m_bIsResurrected)
        {
            if (m_uiCleaveTimer < uiDiff)
            {
                DoCastSpellIfCan(m_creature->getVictim(), SPELL_CLEAVE);
                m_uiCleaveTimer = urand(2500, 7000);
            }
            else
                m_uiCleaveTimer -= uiDiff;

            if (m_uiSmashTimer < uiDiff)
            {
                DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_SMASH : SPELL_SMASH_H);
                m_uiSmashTimer = urand(8000, 15000);
            }
            else
                m_uiSmashTimer -= uiDiff;

            if (m_uiStaggeringRoarTimer < uiDiff)
            {
                DoScriptText(EMOTE_ROAR, m_creature);
                DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_STAGGERING_ROAR : SPELL_STAGGERING_ROAR_H);
                m_uiStaggeringRoarTimer = urand(15000, 30000);
            }
            else
                m_uiStaggeringRoarTimer -= uiDiff;

            if (m_uiEnrageTimer < uiDiff)
            {
                DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_ENRAGE : SPELL_ENRAGE_H);
                m_uiEnrageTimer = urand(10000, 20000);
            }
            else
                m_uiEnrageTimer -= uiDiff;
        }
        else
        {            
            if (m_uiDreadfulRoarTimer < uiDiff)
            {
                DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_DREADFUL_ROAR : SPELL_DREADFUL_ROAR_H);
                m_uiDreadfulRoarTimer = 20000;
            }else m_uiDreadfulRoarTimer -= uiDiff;

            if (m_uiWoeStrike < uiDiff)
            {
                DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_WOE_STRIKE : SPELL_WOE_STRIKE_H);
                m_uiWoeStrike = 20000;
            }else m_uiWoeStrike -= uiDiff;

            if (m_uiDarkSmash < uiDiff)
            {
                DoCastSpellIfCan(m_creature, SPELL_DARK_SMASH);
                m_uiDarkSmash = 20000;
            }else m_uiDarkSmash -= uiDiff;

            if (m_uiShadowAxeTimer < uiDiff)
            {
                DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0), SPELL_SHADOW_AXE_TRIGGER);
                m_uiShadowAxeTimer = 44000;
            }else m_uiShadowAxeTimer -= uiDiff;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_ingvar(Creature* pCreature)
{
    return new boss_ingvarAI(pCreature);
}

/*######
## npc_annhylde
######*/

struct MANGOS_DLL_DECL npc_annhyldeAI : public ScriptedAI
{
    npc_annhyldeAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;
    
    uint32 SpeakTimer;
    uint32 ChannelTimer;
    Creature* m_pIngvar;

    void Reset()
    {
        SpeakTimer = 1000;
        ChannelTimer = 4000;
        
        m_creature->GetMotionMaster()->MoveIdle();
        m_creature->GetMotionMaster()->Clear();
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

        m_pIngvar = m_pInstance->instance->GetCreature(m_pInstance->GetData64(NPC_INGVAR));
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (SpeakTimer < uiDiff)
        {
            DoScriptText(SAY_ANNHYLDE_REZ, m_creature);   
            SpeakTimer = 9999999;
        }else SpeakTimer -= uiDiff;
        
        if (ChannelTimer < uiDiff)
        {
            m_creature->CastSpell(m_pIngvar, SPELL_SCOURGE_RES_BUBBLE, true, 0, 0, ObjectGuid());
            m_creature->CastSpell(m_pIngvar, SPELL_SCOURGE_RES_CHANNEL, true, 0, 0, ObjectGuid());
            ChannelTimer = 9999999;
        }else ChannelTimer -= uiDiff;
    }
};

CreatureAI* GetAI_npc_annhylde(Creature* pCreature)
{
    return new npc_annhyldeAI(pCreature);
}

/*######
## mob_ingvar_throw_dummy
######*/

struct MANGOS_DLL_DECL mob_ingvar_throw_dummyAI : public ScriptedAI
{
    mob_ingvar_throw_dummyAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        StartRunning();
    }

    uint32 m_uiSpinTimer;
    bool m_bIsRegularMode;
    bool reached;

    void StartRunning()
    {
        Reset();

        //if(Creature* pThrowTarget = GetClosestCreatureWithEntry(m_creature, NPC_THROW_TARGET, DEFAULT_VISIBILITY_DISTANCE))
        if (Unit* pThrowTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
            m_creature->GetMotionMaster()->MovePoint(1, pThrowTarget->GetPositionX(), pThrowTarget->GetPositionY(), pThrowTarget->GetPositionZ());
    }

    void Reset()
    {
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_uiSpinTimer = 10000;
        reached = false;
        m_creature->SetInCombatWithZone(); // until GetClosestCreatureWithEntry is broken
    }

    void MovementInform(uint32 type, uint32 id)
    {
        if(type == POINT_MOTION_TYPE && id ==1)
        {
            SetCombatMovement(false);
            reached = true;
            DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_SHADOW_AXE : SPELL_SHADOW_AXE_H);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if(reached)
        {
            if (m_uiSpinTimer < uiDiff)
            {
                m_creature->ForcedDespawn();
            }else m_uiSpinTimer -= uiDiff;
        }
    }
};

CreatureAI* GetAI_mob_ingvar_throw_dummy(Creature* pCreature)
{
    return new mob_ingvar_throw_dummyAI(pCreature);
}
void AddSC_boss_ingvar()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_ingvar";
    newscript->GetAI = &GetAI_boss_ingvar;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_annhylde";
    newscript->GetAI = &GetAI_npc_annhylde;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_ingvar_throw_dummy";
    newscript->GetAI = &GetAI_mob_ingvar_throw_dummy;
    newscript->RegisterSelf();
}
