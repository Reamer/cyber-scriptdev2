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
SDName: Boss_Celebras_the_Cursed
SD%Complete: 100
SDComment:
SDCategory: Maraudon
EndScriptData */

#include "precompiled.h"

#define SPELL_WRATH						21807
#define SPELL_ENTANGLINGROOTS			12747
#define SPELL_CORRUPT_FORCES			21968
#define QUEST_THE_SCEPTER_OF_CELEBRAS	7046

struct MANGOS_DLL_DECL celebras_the_cursedAI : public ScriptedAI
{
    celebras_the_cursedAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint32 Wrath_Timer;
    uint32 EntanglingRoots_Timer;
    uint32 CorruptForces_Timer;

    void Reset()
    {
        Wrath_Timer = 8000;
        EntanglingRoots_Timer = 2000;
        CorruptForces_Timer = 30000;
    }

    void JustDied(Unit* Killer)
    {
        m_creature->SummonCreature(13716, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 600000);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //Wrath
        if (Wrath_Timer < diff)
        {
            Unit* target = NULL;
            target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
            if (target)
                DoCastSpellIfCan(target,SPELL_WRATH);
            Wrath_Timer = 8000;
        }else Wrath_Timer -= diff;

        //EntanglingRoots
        if (EntanglingRoots_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_ENTANGLINGROOTS);
            EntanglingRoots_Timer = 20000;
        }else EntanglingRoots_Timer -= diff;

        //CorruptForces
        if (CorruptForces_Timer < diff)
        {
            m_creature->InterruptNonMeleeSpells(false);
            DoCastSpellIfCan(m_creature,SPELL_CORRUPT_FORCES);
            CorruptForces_Timer = 20000;
        }else CorruptForces_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_celebras_the_cursed(Creature* pCreature)
{
    return new celebras_the_cursedAI(pCreature);
}
struct Locations
{
    float x, y, z;
};
static Locations CelebrasWP[] =
{
	{653.61f, 73.41f, -85.86f},
};
struct MANGOS_DLL_DECL npc_celebrasAI : public ScriptedAI
{
    npc_celebrasAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

	float x;
	float y;
	float z;
	uint32 m_uiWalkTimer;

    void MovementInform(uint32 type, uint32 tempid)
    {
		if (tempid == 0)
			if(Player* pPlayer = GetPlayerAtMinimumRange( 50))
                    if (pPlayer->GetQuestStatus(QUEST_THE_SCEPTER_OF_CELEBRAS) == QUEST_STATUS_INCOMPLETE)
					{
                        pPlayer->GroupEventHappens(QUEST_THE_SCEPTER_OF_CELEBRAS, m_creature);
						m_uiWalkTimer = 200;
					}
    }

    void Reset()
    {
    }
	void GoToStone()
	{
		m_uiWalkTimer = 200;
	}
	void UpdateAI(const uint32 diff)
	{
		if (m_uiWalkTimer)
        {
            if (m_uiWalkTimer <= diff)
            {
				m_creature->GetMotionMaster()->MovePoint(0,CelebrasWP[0].x, CelebrasWP[0].y, CelebrasWP[0].z);
				m_uiWalkTimer = 0;

            }else m_uiWalkTimer -= diff;
        }
		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_npc_celebras(Creature* pCreature)
{
    return new npc_celebrasAI(pCreature);
}

bool QuestAccept_npc_celebras(Player* pPlayer, Creature* pCreature, const Quest* pQuest)
{
    if (pQuest->GetQuestId() == QUEST_THE_SCEPTER_OF_CELEBRAS)
    {
        if (npc_celebrasAI* pCreatureAI = dynamic_cast<npc_celebrasAI*>(pCreature->AI()))
            pCreatureAI->GoToStone();
    }
    return true;
}

void AddSC_boss_celebras_the_cursed()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "celebras_the_cursed";
    newscript->GetAI = &GetAI_celebras_the_cursed;
    newscript->RegisterSelf();
	
	//quest from his ghost
	newscript = new Script;
	newscript->Name = "npc_celebras_the_redeemed";
	newscript->GetAI = &GetAI_npc_celebras;
    newscript->pQuestAccept = &QuestAccept_npc_celebras;
    newscript->RegisterSelf();
}
