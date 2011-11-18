/* Copyright (C) 2006 - 2011 ScriptDev2 <http://www.scriptdev2.com/>
 * Copyright (C) 2011 MangosR2_ScriptDev2
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
SDName: boss_eredar_twins
SD%Complete: 95
SDComment: 
SDCategory: Sunwell Plateau
EndScriptData */

#include "precompiled.h"
#include "sunwell_plateau.h"

enum
{
    SAY_INTRO_1                             = -1580044,
    SAY_INTRO_2                             = -1580045,
    SAY_INTRO_3                             = -1580046,
    SAY_INTRO_4                             = -1580047,
    SAY_INTRO_5                             = -1580048,
    SAY_INTRO_6                             = -1580049,
    SAY_INTRO_7                             = -1580050,
    SAY_INTRO_8                             = -1580051,

    SAY_SACROLASH_SHADOW_NOVA               = -1580052,
    SAY_SACROLASH_EMPOWER                   = -1580053,
    SAY_SACROLASH_KILL_1                    = -1580054,
    SAY_SACROLASH_KILL_2                    = -1580055,
    SAY_SACROLASH_DEAD                      = -1580056,
    SAY_SACROLASH_BERSERK                   = -1580057,

    SAY_ALYTHESS_CONFLAGRATION              = -1580058,
    SAY_ALYTHESS_EMPOWER                    = -1580059,
    SAY_ALYTHESS_KILL_1                     = -1580060,
    SAY_ALYTHESS_KILL_2                     = -1580061,
    SAY_ALYTHESS_DEAD                       = -1580062,
    SAY_ALYTHESS_BERSERK                    = -1580063,

    // Shared spells
    SPELL_TWINS_ENRAGE                      = 46587,
    SPELL_EMPOWER                           = 45366,        // Cast on self when the twin sister dies

    // touch spells
    SPELL_DARK_FLAME                        = 45345,       // makes u immune to either touched spell for 3 secs casted at every switch
    SPELL_DARK_TOUCHED  /*Sacrolash*/       = 45347,
    SPELL_FLAME_TOUCHED /*Alythess*/        = 45348,

    // Sacrolash spells
    SPELL_SHADOW_BLADES                     = 45248,        // 10 secs
    SPELL_DARK_STRIKE                       = 45271,
    SPELL_SHADOW_NOVA                       = 45329,        // 30-35 secs
    SPELL_CONFOUNDING_BLOW                  = 45256,        // Daze; 25 secs
    SPELL_SHADOW_NOVA_UNK                   = 45332,        // Unknown

    // Shadow Image spells
    NPC_SHADOW_IMAGE                        = 25214,
    SPELL_SHADOWFURY                        = 45270,
    SPELL_IMAGE_VISUAL                      = 45263,

    // Alythess spells
    SPELL_PYROGENICS                        = 45230,        // Self buff; 15secs
    SPELL_CONFLAGRATION                     = 45342,        // 30-35 secs
    SPELL_BLAZE                             = 45235,        // On main target every 3 secs; should trigger 45236 which leaves a fire on the ground
    SPELL_BLAZE_SUMMON                      = 45236,        //187366 GO  // firepatch
    SPELL_BLAZE_BURN                        = 45246,        // used by GO
    SPELL_FLAME_SEAR                        = 46771,        // A few random targets debuff
    SPELL_CONFLAGRATION_UNK                 = 45333,        // Unknown

    //other
    //SPELL_BANISH                            = 44836,
};

static const DialogueEntry aIntroDialogue[] =
{
    {SAY_INTRO_1, NPC_SACROLASH, 1000},
    {SAY_INTRO_2, NPC_ALYTHESS,  1500},
    {SAY_INTRO_3, NPC_SACROLASH, 1500},
    {SAY_INTRO_4, NPC_ALYTHESS,  1500},
    {SAY_INTRO_5, NPC_SACROLASH, 1500},
    {SAY_INTRO_6, NPC_ALYTHESS,  1500},
    {SAY_INTRO_7, NPC_SACROLASH, 2500},
    {SAY_INTRO_8, NPC_ALYTHESS,  0},
    {0, 0, 0},
};

bool bAlythessDead;
bool bSacrolashDead;

/*####
# Alythess
####*/

struct MANGOS_DLL_DECL boss_alythessAI : public ScriptedAI
{
    boss_alythessAI(Creature* pCreature) : ScriptedAI(pCreature),
        m_introDialogue(aIntroDialogue)
    {
        m_pInstance = ((instance_sunwell_plateau*)pCreature->GetInstanceData());
        m_introDialogue.InitializeDialogueHelper(m_pInstance);
        Reset();
    }

    ScriptedInstance* m_pInstance;
    DialogueHelper m_introDialogue;

    ObjectGuid m_uiBlazeTargetGUID;
    uint32 m_uiEnrageTimer;
    uint32 m_uiPyrogenicsTimer;
    uint32 m_uiFlameTouchedTimer;
    uint32 m_uiConflagrationTimer;
    uint32 m_uiBlazeTimer;
    bool m_bIsBlaze;
    bool m_bIsBlazeDone;
    uint32 m_uiBlaTimer;
    uint32 m_uiFlameSearTimer;
// if sister is dead she cast shadownova
    uint32 m_uiShadowNovaTimer;
    bool m_bDidIntro;

    void Reset()
    {
        m_uiEnrageTimer = 6*MINUTE*IN_MILLISECONDS;
        m_uiPyrogenicsTimer     = 20000;
        m_uiConflagrationTimer  = urand(25000, 30000);
        m_uiBlazeTimer          = 1000;
        m_uiFlameSearTimer      = 5000;
        m_uiBlazeTargetGUID.Clear();
        m_uiBlaTimer = 2600;
        m_bIsBlaze = false;
        m_bIsBlazeDone = false;
        m_bDidIntro = false;
        m_uiShadowNovaTimer      = 15000;
        bAlythessDead = false;
    }

    void JustReachedHome()
    {
        if (m_pInstance)
        {
            if (m_pInstance->GetData(TYPE_EREDAR_TWINS) != FAIL)
                m_pInstance->SetData(TYPE_EREDAR_TWINS, FAIL);

            // Respawn dead sister
            if (Creature* pSister = m_pInstance->GetSingleCreatureFromStorage(NPC_SACROLASH))
            {
                if (!pSister->isAlive())
                    pSister->Respawn();
            }
        }
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
        {
            if (m_pInstance->GetData(TYPE_EREDAR_TWINS) != IN_PROGRESS)
                m_pInstance->SetData(TYPE_EREDAR_TWINS, IN_PROGRESS);
        }
    }

    void AttackStart(Unit* pWho)
    {
        if (m_creature->Attack(pWho, false))
        {
            m_creature->AddThreat(pWho);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);

            // Only range attack
            m_creature->GetMotionMaster()->MoveChase(pWho, 10.0f);
        }
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(urand(0, 1) ? SAY_ALYTHESS_KILL_1 : SAY_ALYTHESS_KILL_2, m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        bAlythessDead = true;
        if (m_pInstance)
        {
            if (Creature* pSacrolash = m_pInstance->GetSingleCreatureFromStorage(NPC_SACROLASH))
            {
                if (!pSacrolash->isAlive())
                {
                    m_pInstance->SetData(TYPE_EREDAR_TWINS, DONE);
                    DoScriptText(SAY_ALYTHESS_DEAD, m_creature);
                }
                else
                {
                    // Remove loot flag and cast empower
                    m_creature->RemoveFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
                    DoScriptText(SAY_SACROLASH_EMPOWER, pSacrolash);
                    pSacrolash->InterruptNonMeleeSpells(true);
                    pSacrolash->CastSpell(pSacrolash, SPELL_EMPOWER, false);
                }
            }
        }
    }

    void SpellHitTarget(Unit* pTarget, const SpellEntry* spell)
    {
         switch(spell->Id)
         {
         case SPELL_BLAZE:
         case SPELL_BLAZE_BURN:
         case SPELL_CONFLAGRATION:
         case SPELL_FLAME_SEAR:
              HandleTouchedSpells(pTarget, SPELL_FLAME_TOUCHED);
              break;
         case SPELL_SHADOW_NOVA:
              HandleTouchedSpells(pTarget, SPELL_DARK_TOUCHED);
              break;
         }
    }

    void HandleTouchedSpells(Unit* pTarget, uint32 TouchedType)
    {
         switch(TouchedType)
         {
         case SPELL_FLAME_TOUCHED:
              if (!pTarget->HasAura(SPELL_DARK_FLAME))
              {
                  if (pTarget->HasAura(SPELL_DARK_TOUCHED))
                  {
                      pTarget->RemoveAurasDueToSpell(SPELL_DARK_TOUCHED);
                      pTarget->CastSpell(pTarget, SPELL_DARK_FLAME, true);
                  }else pTarget->CastSpell(pTarget, SPELL_FLAME_TOUCHED, true);
              }
              break;
         case SPELL_DARK_TOUCHED:
              if (!pTarget->HasAura(SPELL_DARK_FLAME))
              {
                  if (pTarget->HasAura(SPELL_FLAME_TOUCHED))
                  {
                      pTarget->RemoveAurasDueToSpell(SPELL_FLAME_TOUCHED);
                      pTarget->CastSpell(pTarget, SPELL_DARK_FLAME, true);
                  } else pTarget->CastSpell(pTarget, SPELL_DARK_TOUCHED, true);
              }
                break;
          }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_EREDAR_TWINS) == SPECIAL)
        {
            if (!m_bDidIntro)
            {
                m_introDialogue.StartNextDialogueText(SAY_INTRO_1);
                m_bDidIntro = true;
            }
            m_introDialogue.DialogueUpdate(uiDiff);
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiEnrageTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_TWINS_ENRAGE) == CAST_OK)
            {
                DoScriptText(SAY_ALYTHESS_BERSERK, m_creature);
                m_uiEnrageTimer = 6*MINUTE*IN_MILLISECONDS;
            }
        }
        else
            m_uiEnrageTimer -= uiDiff;

        if (m_uiPyrogenicsTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_PYROGENICS) == CAST_OK)
                m_uiPyrogenicsTimer = urand(25000, 30000);
        }
        else
            m_uiPyrogenicsTimer -= uiDiff;

// conflagration while sacrolash alive
        if (!bSacrolashDead)
        {
            if (m_uiConflagrationTimer < uiDiff)
            {
                Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_TOPAGGRO, 3);
                if (!pTarget)
                    pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0);

                if (DoCastSpellIfCan(pTarget, SPELL_CONFLAGRATION) == CAST_OK)
                {
                    DoScriptText(SAY_ALYTHESS_CONFLAGRATION, m_creature);
                    m_uiConflagrationTimer = urand(20000, 25000);
                }
            }
            else
                m_uiConflagrationTimer -= uiDiff;
        }
// shadow nova while sacrolash dead
        else
        {
            if (m_uiShadowNovaTimer < uiDiff)
            {
                Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_TOPAGGRO, 2);
                if (!pTarget)
                    pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0);

                if (DoCastSpellIfCan(pTarget, SPELL_SHADOW_NOVA) == CAST_OK)
                {
                    DoScriptText(SAY_ALYTHESS_CONFLAGRATION, m_creature);
                    m_uiShadowNovaTimer = urand(20000, 25000);
                }
            }
            else
                m_uiShadowNovaTimer -= uiDiff;
        }

        if (m_uiFlameSearTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_FLAME_SEAR) == CAST_OK)
                m_uiFlameSearTimer = 10000;
        }
        else
            m_uiFlameSearTimer -= uiDiff;

        if (m_uiBlazeTimer < uiDiff)
        {
            if (Unit* pVictim = m_creature->getVictim())
            {
                m_uiBlazeTargetGUID = pVictim->GetObjectGuid();

                if (DoCastSpellIfCan(pVictim,SPELL_BLAZE)==CAST_OK)
                m_bIsBlazeDone = true;
                m_uiBlazeTimer = 3000;

                m_bIsBlaze = true;
            }
        } else m_uiBlazeTimer -= uiDiff;
        if (m_bIsBlaze)
            if (m_uiBlaTimer < uiDiff)
            {
                if (Unit* pVictim = m_creature->GetMap()->GetUnit(m_uiBlazeTargetGUID))
                {
                    if (m_bIsBlazeDone)
                    {
                        pVictim->CastSpell(pVictim,SPELL_BLAZE_SUMMON,true);
                        m_bIsBlazeDone = false;
                    }
                }
                m_bIsBlaze = false;
                m_uiBlaTimer = 2600;
            }else m_uiBlaTimer -= uiDiff;
    }
};

/*####
# sacrolash
####*/

struct MANGOS_DLL_DECL boss_sacrolashAI : public ScriptedAI
{
    boss_sacrolashAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((instance_sunwell_plateau*)pCreature->GetInstanceData());
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiEnrageTimer;
    uint32 m_uiDarkTouchedTimer;
    uint32 m_uiShadowNovaTimer;
    uint32 m_uiConfoundingBlowTimer;
    uint32 m_uiShadowBladesTimer;
    uint32 m_uiSummonShadowImage;
    uint32 m_uiConflagrationTimer;

    void Reset()
    {
        m_uiEnrageTimer = 6*MINUTE*IN_MILLISECONDS;
        m_uiShadowNovaTimer      = 15000;
        m_uiConfoundingBlowTimer = 30000;
        m_uiShadowBladesTimer    = 15000;
        m_uiSummonShadowImage    = 10000;
        m_uiConflagrationTimer  = urand(20000, 25000);
        bSacrolashDead = false;
    }

    void JustReachedHome()
    {
        if (m_pInstance)
        {
            if (m_pInstance->GetData(TYPE_EREDAR_TWINS) != FAIL)
                m_pInstance->SetData(TYPE_EREDAR_TWINS, FAIL);

            // Respawn dead sister
            if (Creature* pSister = m_pInstance->GetSingleCreatureFromStorage(NPC_ALYTHESS))
            {
                if (!pSister->isAlive())
                    pSister->Respawn();
            }
        }
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
        {
            if (m_pInstance->GetData(TYPE_EREDAR_TWINS) != IN_PROGRESS)
                m_pInstance->SetData(TYPE_EREDAR_TWINS, IN_PROGRESS);
        }
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(urand(0, 1) ? SAY_SACROLASH_KILL_1 : SAY_SACROLASH_KILL_2, m_creature);
    }

    void JustDied(Unit* pKiller)
    {

        bSacrolashDead = true;
        if (m_pInstance)
        {
            if (Creature* pAlythess = m_pInstance->GetSingleCreatureFromStorage(NPC_ALYTHESS))
            {
                if (!pAlythess->isAlive())
                {
                    m_pInstance->SetData(TYPE_EREDAR_TWINS, DONE);
                    DoScriptText(SAY_SACROLASH_DEAD, m_creature);
                }
                else
                {
                    // Remove loot flag and cast empower
                    m_creature->RemoveFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
                    DoScriptText(SAY_ALYTHESS_EMPOWER, pAlythess);
                    pAlythess->InterruptNonMeleeSpells(true);
                    pAlythess->CastSpell(pAlythess, SPELL_EMPOWER, false);
                }
            }
        }
    }

    // Return a random target which it's not in range of 10 yards of boss
    Unit* GetRandomTargetAtDist(float fDist)
    {
        std::vector<Unit*> m_vRangeTargets;

        ThreatList const& tList = m_creature->getThreatManager().getThreatList();
        for (ThreatList::const_iterator iter = tList.begin();iter != tList.end(); ++iter)
        {
            if (Unit* pTempTarget = m_creature->GetMap()->GetUnit((*iter)->getUnitGuid()))
            {
                if (!pTempTarget->IsWithinDistInMap(m_creature, fDist))
                    m_vRangeTargets.push_back(pTempTarget);
            }
        }

        if (!m_vRangeTargets.empty())
            return m_vRangeTargets[urand(0, m_vRangeTargets.size() - 1)];
        else
            return m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1);
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (pSummoned->GetEntry() == NPC_SHADOW_IMAGE)
        {
            pSummoned->CastSpell(pSummoned, SPELL_IMAGE_VISUAL, false);
            // Attack random range target
            if (Unit* pTarget = GetRandomTargetAtDist(10.0f))
                pSummoned->AI()->AttackStart(pTarget);
        }
    }

    void SpellHitTarget(Unit* pTarget, const SpellEntry* spell)
    {
         switch(spell->Id)
         {
         case SPELL_SHADOW_BLADES:
         case SPELL_SHADOW_NOVA:
         case SPELL_CONFOUNDING_BLOW:
         case SPELL_SHADOWFURY:
         case SPELL_DARK_STRIKE:
              HandleTouchedSpells(pTarget, SPELL_DARK_TOUCHED);
              break;
         case SPELL_CONFLAGRATION:
              HandleTouchedSpells(pTarget, SPELL_FLAME_TOUCHED);
              break;
         }
    }

    void HandleTouchedSpells(Unit* pTarget, uint32 TouchedType)
    {
         switch(TouchedType)
         {
         case SPELL_FLAME_TOUCHED:
              if (!pTarget->HasAura(SPELL_DARK_FLAME))
              {
                  if (pTarget->HasAura(SPELL_DARK_TOUCHED))
                  {
                      pTarget->RemoveAurasDueToSpell(SPELL_DARK_TOUCHED);
                      pTarget->CastSpell(pTarget, SPELL_DARK_FLAME, true);
                  } else pTarget->CastSpell(pTarget, SPELL_FLAME_TOUCHED, true);
              }
              break;
         case SPELL_DARK_TOUCHED:
              if (!pTarget->HasAura(SPELL_DARK_FLAME))
              {
                  if (pTarget->HasAura(SPELL_FLAME_TOUCHED))
                  {
                      pTarget->RemoveAurasDueToSpell(SPELL_FLAME_TOUCHED);
                      pTarget->CastSpell(pTarget, SPELL_DARK_FLAME, true);
                  } else pTarget->CastSpell(pTarget, SPELL_DARK_TOUCHED, true);
              }
              break;
         }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiEnrageTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_TWINS_ENRAGE) == CAST_OK)
            {
                DoScriptText(SAY_SACROLASH_BERSERK, m_creature);
                m_uiEnrageTimer = 6*MINUTE*IN_MILLISECONDS;
            }
        }
        else
            m_uiEnrageTimer -= uiDiff;

        if (m_uiShadowBladesTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_SHADOW_BLADES) == CAST_OK)
                m_uiShadowBladesTimer = urand(13000, 15000);
        }
        else
            m_uiShadowBladesTimer -= uiDiff;
// shadow nova while alythess alive
        if (!bAlythessDead)
        {
            if (m_uiShadowNovaTimer < uiDiff)
            {
                Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_TOPAGGRO, 2);
                if (!pTarget)
                    pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0);

                if (DoCastSpellIfCan(pTarget, SPELL_SHADOW_NOVA) == CAST_OK)
                {
                    DoScriptText(SAY_SACROLASH_SHADOW_NOVA, m_creature);
                    m_uiShadowNovaTimer = urand(30000, 35000);
                }
            }
            else
                m_uiShadowNovaTimer -= uiDiff;
        }
//conflagration when alythess dead
        else
        {
            if (m_uiConflagrationTimer < uiDiff)
            {
                Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_TOPAGGRO, 3);
                if (!pTarget)
                    pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0);

                if (DoCastSpellIfCan(pTarget, SPELL_CONFLAGRATION) == CAST_OK)
                {
                    DoScriptText(SAY_SACROLASH_SHADOW_NOVA, m_creature);
                    m_uiConflagrationTimer = urand(20000, 25000);
                }
            }
            else
                m_uiConflagrationTimer -= uiDiff;
        }

        if (m_uiConfoundingBlowTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_CONFOUNDING_BLOW) == CAST_OK)
            {
                // Reset threat
                if (m_creature->getThreatManager().getThreat(m_creature->getVictim()))
                    m_creature->getThreatManager().modifyThreatPercent(m_creature->getVictim(), -100);

                m_uiConfoundingBlowTimer = urand(25000, 30000);
            }
        }
        else
            m_uiConfoundingBlowTimer -= uiDiff;

        if (m_uiSummonShadowImage < uiDiff)
        {
            // Summon 3 shadow images at the boss position
            for(uint8 i = 0; i < 3; ++i)
                m_creature->SummonCreature(NPC_SHADOW_IMAGE, 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);

            m_uiSummonShadowImage = urand(10000, 12000);
        }
        else
            m_uiSummonShadowImage -= uiDiff;

        // Overwrite the melee attack in order to apply the dark strike
        if (m_creature->CanReachWithMeleeAttack(m_creature->getVictim()))
        {
            // Make sure our attack is ready and we aren't currently casting
            if (m_creature->isAttackReady() && !m_creature->IsNonMeleeSpellCasted(false))
            {
                DoCastSpellIfCan(m_creature->getVictim(), SPELL_DARK_STRIKE);

                m_creature->AttackerStateUpdate(m_creature->getVictim());
                m_creature->resetAttackTimer();
            }
        }
    }
};

struct MANGOS_DLL_DECL npc_shadow_imageAI : public ScriptedAI
{
    npc_shadow_imageAI(Creature* pCreature) : ScriptedAI(pCreature)  { Reset(); }

    uint32 m_uiChosenAbility;
    uint32 m_uiSuicideTimer;
    uint32 m_uiAbilityTimer;
    uint8 m_uiDarkStrikes;

    void Reset()
    {
        // Choose only one spell for attack
        m_uiChosenAbility = urand(0, 1) ? SPELL_DARK_STRIKE : SPELL_SHADOWFURY;
        m_uiAbilityTimer = 500;
        m_uiDarkStrikes = 0;
        m_uiSuicideTimer = 0;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Suicide on timer; this is needed because of the cast time
        if (m_uiSuicideTimer)
        {
            if (m_uiSuicideTimer <= uiDiff)
            {
                m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                return;
            }
            else
                m_uiSuicideTimer -= uiDiff;
        }
        else
        {
            // Do chosen ability
            switch (m_uiChosenAbility)
            {
                case SPELL_SHADOWFURY:
                    if (m_uiAbilityTimer < uiDiff)
                    {
                        if (m_creature->IsWithinDistInMap(m_creature->getVictim(), INTERACTION_DISTANCE))
                        {
                            if (DoCastSpellIfCan(m_creature, SPELL_SHADOWFURY) == CAST_OK)
                                m_uiSuicideTimer = 1000;
                        }
                    }
                    else
                        m_uiAbilityTimer -= uiDiff;
                    break;
                case SPELL_DARK_STRIKE:
                    if (m_uiAbilityTimer < uiDiff)
                    {
                        if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_DARK_STRIKE) == CAST_OK)
                        {
                            ++m_uiDarkStrikes;
                            // kill itself after 2 strikes
                            if (m_uiDarkStrikes == 2)
                                m_uiSuicideTimer = 1000;
                            else
                                m_uiAbilityTimer = 1000;
                        }
                    }
                    else
                        m_uiAbilityTimer -= uiDiff;
                    break;
            }
        }
    }
};

CreatureAI* GetAI_boss_alythess(Creature* pCreature)
{
    return new boss_alythessAI (pCreature);
}

CreatureAI* GetAI_boss_sacrolash(Creature* pCreature)
{
    return new boss_sacrolashAI (pCreature);
}

CreatureAI* GetAI_npc_shadow_image(Creature* pCreature)
{
    return new npc_shadow_imageAI (pCreature);
}

void AddSC_boss_eredar_twins()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_alythess";
    pNewScript->GetAI = &GetAI_boss_alythess;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_sacrolash";
    pNewScript->GetAI = &GetAI_boss_sacrolash;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_shadow_image";
    pNewScript->GetAI = &GetAI_npc_shadow_image;
    pNewScript->RegisterSelf();
}
