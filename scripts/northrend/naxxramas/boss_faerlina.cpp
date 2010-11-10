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
SDName: Boss_Faerlina
SD%Complete: 50
SDComment:
SDCategory: Naxxramas
EndScriptData */

#include "precompiled.h"
#include "naxxramas.h"

enum
{
    SAY_GREET                 = -1533009,
    SAY_AGGRO1                = -1533010,
    SAY_AGGRO2                = -1533011,
    SAY_AGGRO3                = -1533012,
    SAY_AGGRO4                = -1533013,
    SAY_SLAY1                 = -1533014,
    SAY_SLAY2                 = -1533015,
    SAY_DEATH                 = -1533016,

    MOB_WORSHIPPER            = 16506,
    MOB_FOLLOWER              = 16505,

    //SOUND_RANDOM_AGGRO        = 8955,                              //soundId containing the 4 aggro sounds, we not using this

    SPELL_POSIONBOLT_VOLLEY   = 28796,
    H_SPELL_POSIONBOLT_VOLLEY = 54098,

    SPELL_RAINOFFIRE          = 28794,
    H_SPELL_RAINOFFIRE        = 58936,

    SPELL_ENRAGE              = 28798,
    H_SPELL_ENRAGE            = 54100,

    //MOB SPELLS
    SPELL_WIDOWS_EMBRACE      = 28732,
    SPELL_FIRE_BALL           = 54095,
    H_SPELL_FIRE_BALL         = 54096
};

static uint32 m_uiWorshippers[4] = {NPC_WORSHIPPER_1,NPC_WORSHIPPER_2,NPC_WORSHIPPER_3,NPC_WORSHIPPER_4}; 
static uint32 m_uiFollower[2] = {NPC_FOLLOWER_1, NPC_FOLLOWER_2};

struct MANGOS_DLL_DECL boss_faerlinaAI : public ScriptedAI
{
    boss_faerlinaAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_naxxramas*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        m_bHasTaunted = false;
        Reset();
    }

    instance_naxxramas* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiPoisonBoltVolleyTimer;
    uint32 m_uiRainOfFireTimer;
    uint32 m_uiEnrageTimer;
    uint8  m_uiDeadWorshippers;
    bool   m_bHasTaunted;

    uint64 m_uiFollowerGUID[5];

    void Reset()
    {
        m_uiPoisonBoltVolleyTimer = 8000;
        m_uiRainOfFireTimer = 16000;
        m_uiEnrageTimer = 60000;
        m_uiDeadWorshippers = 0;
        if (m_pInstance)
            m_pInstance->SetAchiev(TYPE_FAERLINA, false);
    }

    void Aggro(Unit* pWho)
    {
        switch(urand(0, 3))
        {
            case 0: DoScriptText(SAY_AGGRO1, m_creature); break;
            case 1: DoScriptText(SAY_AGGRO2, m_creature); break;
            case 2: DoScriptText(SAY_AGGRO3, m_creature); break;
            case 3: DoScriptText(SAY_AGGRO4, m_creature); break;
        }

        m_creature->CallForHelp(15.0f);

        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_FAERLINA, IN_PROGRESS);
            m_pInstance->SetAchiev(TYPE_FAERLINA, true);
        }
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        if (!m_bHasTaunted && m_creature->IsWithinDistInMap(pWho, 60.0f))
        {
            DoScriptText(SAY_GREET, m_creature);
            m_bHasTaunted = true;
        }

        ScriptedAI::MoveInLineOfSight(pWho);
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(urand(0, 1)?SAY_SLAY1:SAY_SLAY2, m_creature);
    }

    void SpellHit(Unit* pCaster, const SpellEntry* pSpell) 
    {
        if (pSpell->Id == SPELL_WIDOWS_EMBRACE)
        {
            if (m_creature->HasAura(SPELL_ENRAGE,EFFECT_INDEX_2))
            {
                if (m_pInstance)
                    m_pInstance->SetAchiev(TYPE_FAERLINA, false);
                m_creature->RemoveAurasDueToSpell(SPELL_ENRAGE);
                m_uiEnrageTimer = 60000;

                //if (!m_bIsRegularMode) //hack worshipper should die
                //    pCaster->DealDamage(pCaster, pCaster->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);

            }else m_uiEnrageTimer += 30000;
        }
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_FAERLINA, DONE);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_FAERLINA, FAIL);

        for(int i=0;i<4;i++)
        {
            if(Creature* worshipper = (Creature*) m_creature->GetMap()->GetUnit(m_pInstance->GetData64(m_uiWorshippers[i])))
            {
                worshipper->Respawn();
            }
        }
        for(int i=0;i<2;i++)
        {
            if(Creature* follower = (Creature*) m_creature->GetMap()->GetUnit(m_pInstance->GetData64(m_uiFollower[i])))
            {
                follower->Respawn();
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Poison Bolt Volley
        if (m_uiPoisonBoltVolleyTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_POSIONBOLT_VOLLEY : H_SPELL_POSIONBOLT_VOLLEY);
            m_uiPoisonBoltVolleyTimer = 11000;
        }else m_uiPoisonBoltVolleyTimer -= uiDiff;

        // Rain Of Fire
        if (m_uiRainOfFireTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCastSpellIfCan(pTarget, m_bIsRegularMode ? SPELL_RAINOFFIRE : H_SPELL_RAINOFFIRE);

            m_uiRainOfFireTimer = 16000;
        }else m_uiRainOfFireTimer -= uiDiff;

        //Enrage_Timer
        if (m_uiEnrageTimer < uiDiff)
        {
            DoCast(m_creature, m_bIsRegularMode ? SPELL_ENRAGE : H_SPELL_ENRAGE);
            m_uiEnrageTimer = 60000;
        }else m_uiEnrageTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_faerlina(Creature* pCreature)
{
    return new boss_faerlinaAI(pCreature);
}

struct MANGOS_DLL_DECL mob_worshipperAI : public ScriptedAI
{
    mob_worshipperAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_naxxramas*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_naxxramas* m_pInstance;
    bool m_bIsRegularMode;

    void Reset()
    {
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;
        DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_FIRE_BALL : H_SPELL_FIRE_BALL);
    }

    void JustDied(Unit* pKiller)
    {
        //if (m_bIsRegularMode) //only 10 mode
            m_creature->CastSpell(m_creature, SPELL_WIDOWS_EMBRACE, true, 0, 0, pKiller->GetGUID());
    }

};

CreatureAI* GetAI_mob_worshipper(Creature* pCreature)
{
    return new mob_worshipperAI(pCreature);
}
void AddSC_boss_faerlina()
{
    Script* NewScript;

    NewScript = new Script;
    NewScript->Name = "boss_faerlina";
    NewScript->GetAI = &GetAI_boss_faerlina;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "mob_worshipper";
    NewScript->GetAI = &GetAI_mob_worshipper;
    NewScript->RegisterSelf();
}
