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
SDName: Boss_Trollgore
SD%Complete: 90%
SDComment: Timers
SDCategory: Drak'Tharon Keep
EndScriptData */

#include "precompiled.h"
#include "draktharon_keep.h"

enum
{
    SAY_AGGRO                       = -1600000,
    SAY_CONSUME                     = -1600001,
    SAY_DEATH                       = -1600002,
    SAY_EXPLODE                     = -1600003,
    SAY_KILL                        = -1600004,

    SPELL_CRUSH                     = 49639,
    SPELL_INFECTED_WOUND            = 49367,
    SPELL_CORPSE_EXPLODE            = 49555,
    SPELL_CORPSE_EXPLODE_H          = 59807,
    SPELL_CONSUME                   = 49380,
    SPELL_CONSUME_H                 = 59803,
    SPELL_CONSUME_BUFF              = 49381,
    SPELL_CONSUME_BUFF_H            = 59805,

    SPELL_CORPSE_EXPLODE_PROC       = 49618,
    SPELL_CORPSE_EXPLODE_PROC_H     = 59809,

    NPC_DRAKKARI_INVADER            = 27753,
    NPC_TROLLGORE                   = 26630
};

const float PosSummon1[3] = {-259.59f, -652.49f, 26.52f};
const float PosSummon2[3] = {-261.60f, -658.71f, 26.51f};
const float PosSummon3[3] = {-262.05f, -665.71f, 26.49f};


/*######
## boss_trollgore
######*/

struct MANGOS_DLL_DECL boss_trollgoreAI : public ScriptedAI
{
    boss_trollgoreAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_draktharon_keep*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_draktharon_keep* m_pInstance;
    bool m_bIsRegularMode;

    uint32 Consume_Timer;
    uint32 Crush_Timer;
    uint32 InfectedWound_Timer;
    uint32 Wave_Timer;
    uint32 CorpseExplode_Timer;

    void Reset()
    {
        CorpseExplode_Timer = 10000;
        Consume_Timer = 5000;
        Crush_Timer = 10000;
        InfectedWound_Timer = 30000;
        Wave_Timer = 2000;
    }

    void Aggro(Unit* pWho)
    {
        if (pWho->GetTypeId() == TYPEID_PLAYER)
            DoScriptText(SAY_AGGRO, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_TROLLGORE, IN_PROGRESS);
    }

    void KilledUnit(Unit* pVictim)
    {
        if (pVictim->GetCharmerOrOwnerPlayerOrPlayerItself() && roll_chance_i(33))
            DoScriptText(SAY_KILL, m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_TROLLGORE, DONE);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_TROLLGORE, FAIL);
    }

    void SummonWaves()
    {
        if (Creature* pInvader1 = m_creature->SummonCreature(NPC_DRAKKARI_INVADER,PosSummon1[0],PosSummon1[1],PosSummon1[2],0, TEMPSUMMON_DEAD_DESPAWN, 15000))
            pInvader1->AI()->AttackStart(m_creature);
        if (Creature* pInvader2 = m_creature->SummonCreature(NPC_DRAKKARI_INVADER,PosSummon2[0],PosSummon2[1],PosSummon2[2],0, TEMPSUMMON_DEAD_DESPAWN, 15000))
            pInvader2->AI()->AttackStart(m_creature);
        if (Creature* pInvader3 = m_creature->SummonCreature(NPC_DRAKKARI_INVADER,PosSummon3[0],PosSummon3[1],PosSummon3[2],0, TEMPSUMMON_DEAD_DESPAWN, 15000))
            pInvader3->AI()->AttackStart(m_creature);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Crush
        if (Crush_Timer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_CRUSH) == CAST_OK)
                Crush_Timer = 10000;
        }
        else
            Crush_Timer -= uiDiff;

        // Infected Wound
        if (InfectedWound_Timer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_CRUSH) == CAST_OK)
                InfectedWound_Timer = 30000;
        }
        else
            InfectedWound_Timer -= uiDiff;

        // Summon npcs
        if (Wave_Timer < uiDiff)
        {
            SummonWaves();
            Wave_Timer = 15000;
        }
        else
            Wave_Timer -= uiDiff;

        // Consume
        if (Consume_Timer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature,  m_bIsRegularMode ? SPELL_CONSUME : SPELL_CONSUME_H) == CAST_OK)
                Consume_Timer = 15000;
        }
        else
            Consume_Timer -= uiDiff;

        //Corpse Explosion
        if (CorpseExplode_Timer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(),  m_bIsRegularMode ? SPELL_CORPSE_EXPLODE : SPELL_CORPSE_EXPLODE_H) == CAST_OK)
                CorpseExplode_Timer = 15000;
        }
        else
            CorpseExplode_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_trollgore(Creature* pCreature)
{
    return new boss_trollgoreAI(pCreature);
}

void AddSC_boss_trollgore()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_trollgore";
    pNewScript->GetAI = &GetAI_boss_trollgore;
    pNewScript->RegisterSelf();
}
