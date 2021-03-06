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
SDName: Boss_Fankriss
SD%Complete: 100
SDComment: sound not implemented
SDCategory: Temple of Ahn'Qiraj
EndScriptData */

#include "precompiled.h"

#define SOUND_SENTENCE_YOU 8588
#define SOUND_SERVE_TO     8589
#define SOUND_LAWS         8590
#define SOUND_TRESPASS     8591
#define SOUND_WILL_BE      8592

#define SPELL_MORTAL_WOUND 28467
#define SPELL_ROOT         28858

// Enrage for his spawns
#define SPELL_ENRAGE       28798

struct MANGOS_DLL_DECL boss_fankrissAI : public ScriptedAI
{
    boss_fankrissAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint32 MortalWound_Timer;
    uint32 SpawnHatchlings_Timer;
    uint32 SpawnSpawns_Timer;
    int Rand;
    int RandX;
    int RandY;

    Creature* Spawn;

    void Reset()
    {
        MortalWound_Timer = urand(10000, 15000);
        SpawnHatchlings_Timer = urand(6000, 12000);
        SpawnSpawns_Timer = urand(15000, 45000);
    }

    void SummonSpawn(Unit* pVictim)
    {
        Rand = 10 + (rand()%10);
        switch(urand(0, 1))
        {
            case 0: RandX = 0 - Rand; break;
            case 1: RandX = 0 + Rand; break;
        }
        Rand = 0;
        Rand =  10 + (rand()%10);
        switch(urand(0, 1))
        {
            case 0: RandY = 0 - Rand; break;
            case 1: RandY = 0 + Rand; break;
        }
        Rand = 0;
        Spawn = DoSpawnCreature(15630, RandX, RandY, 0, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
        if (Spawn && pVictim)
            Spawn->AI()->AttackStart(pVictim);
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //MortalWound_Timer
        if (MortalWound_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_MORTAL_WOUND);
            MortalWound_Timer = urand(10000, 20000);
        }else MortalWound_Timer -= diff;

        //Summon 1-3 Spawns of Fankriss at random time.
        if (SpawnSpawns_Timer < diff)
        {
            switch(urand(0, 2))
            {
                case 0:
                    SummonSpawn(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0));
                    break;
                case 1:
                    SummonSpawn(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0));
                    SummonSpawn(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0));
                    break;
                case 2:
                    SummonSpawn(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0));
                    SummonSpawn(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0));
                    SummonSpawn(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0));
                    break;
            }
            SpawnSpawns_Timer = urand(30000, 60000);
        }
        else
            SpawnSpawns_Timer -= diff;

        // Teleporting Random Target to one of the three tunnels and spawn 4 hatchlings near the gamer.
        //We will only telport if fankriss has more than 3% of hp so teleported gamers can always loot.
        if (m_creature->GetHealthPercent() > 3.0f)
        {
            if (SpawnHatchlings_Timer< diff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0, SPELL_ROOT, SELECT_FLAG_PLAYER))
                {
                    DoCastSpellIfCan(pTarget, SPELL_ROOT);

                    if (m_creature->getThreatManager().getThreat(pTarget))
                        m_creature->getThreatManager().modifyThreatPercent(pTarget, -100);

                    switch(urand(0, 2))
                    {
                        case 0:
                            DoTeleportPlayer(pTarget, -8106.0142f, 1289.2900f, -74.419533f, 5.112f);
                            break;
                        case 1:
                            DoTeleportPlayer(pTarget, -7990.135354f, 1155.1907f, -78.849319f, 2.608f);
                            break;
                        case 2:
                            DoTeleportPlayer(pTarget, -8159.7753f, 1127.9064f, -76.868660f, 0.675f);
                            break;
                    }
                    Creature* pHatchling = NULL;
                    if (pHatchling = m_creature->SummonCreature(15962, pTarget->GetPositionX()-3, pTarget->GetPositionY()-3, pTarget->GetPositionZ(), 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000))
                        pHatchling->AI()->AttackStart(pTarget);
                    if (pHatchling = m_creature->SummonCreature(15962, pTarget->GetPositionX()-3, pTarget->GetPositionY()+3, pTarget->GetPositionZ(), 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000))
                        pHatchling->AI()->AttackStart(pTarget);
                    if (pHatchling = m_creature->SummonCreature(15962, pTarget->GetPositionX()-5, pTarget->GetPositionY()-5, pTarget->GetPositionZ(), 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000))
                        pHatchling->AI()->AttackStart(pTarget);
                    if (pHatchling = m_creature->SummonCreature(15962, pTarget->GetPositionX()-5, pTarget->GetPositionY()+5, pTarget->GetPositionZ(), 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000))
                        pHatchling->AI()->AttackStart(pTarget);
                }
                SpawnHatchlings_Timer = urand(45000, 60000);
            }
            else
                SpawnHatchlings_Timer -= diff;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_fankriss(Creature* pCreature)
{
    return new boss_fankrissAI(pCreature);
}

void AddSC_boss_fankriss()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_fankriss";
    pNewScript->GetAI = &GetAI_boss_fankriss;
    pNewScript->RegisterSelf();
}
