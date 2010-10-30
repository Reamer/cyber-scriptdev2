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
SDName: Boss_Pusillin
SD%Complete: 100%
SDComment: Maybe wrong timer for Spells
SDCategory: Dire Maul
EndScriptData */

/* ContentData
EndContentData */

#include "precompiled.h"

enum{
	SAY_START      =   -2000001,
	SAY_END		   =   -2000002,

	SPELL_FIREBALL =	15228,
	SPELL_FIRENOVA =	22424,
	SPELL_FIREHIT  =	14145,
	SPELL_GROWUP   =	22735
};


struct Locations
{
    float x, y, z;
};
static Locations PusillinWP[]=
{
	{-163.86f, -198.46f, -4.15f},                            // Start
	{-138.83f, -351.36f, -4.15f},                            
	{ 114.06f, -351.72f, -4.15f},						     
	{ 113.41f, -376.05f, -3.00f},                            
	{ 110.84f, -468.59f, -3.00f},                           
	{ 113.10f, -486.43f, -7.00f},						     
	{ 110.68f, -515.38f, -7.00f},                            
	{ 110.02f, -539.37f,-11.07f},                            
	{  93.18f, -543.64f,-11.08f},						     
	{  52.10f, -555.53f,-19.41f},
	{  49.00f, -595.00f,-25.16f},
	{  37.78f, -682.47f,-25.16f},                            
	{   7.00f, -695.47f,-25.16f},						     
	{   0.51f, -671.55f,-12.64f},                            
	{  17.40f, -669.25f,-12.64f},                            
	{  18.58f, -703.00f, -12.55f},						     
	{   6.06f, -704.27f, -12.55f}						     // End
};
struct MANGOS_DLL_DECL boss_pusillinAI : public ScriptedAI
{
	boss_pusillinAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		StartRunning();
	};
	uint32 id;
	uint32 m_uiWalkTimer;
	uint32 m_uiFireball_Timer;
	uint32 m_uiFirenova_Timer;
	uint32 m_uiFirehit_Timer;

	void StartRunning()
	{
		m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PASSIVE);
		id = 0;
		m_uiFireball_Timer = urand(12000,20000);
		m_uiFirehit_Timer  = urand(20000,30000);
		m_uiFirenova_Timer = urand(7000,12000);
		m_creature->GetMotionMaster()->MovePoint(id,PusillinWP[id].x, PusillinWP[id].y, PusillinWP[id].z);
		DoScriptText(SAY_START,m_creature);
	}
	void Reset()
	{
		m_creature->GetMotionMaster()->MovePoint(id,PusillinWP[id].x, PusillinWP[id].y, PusillinWP[id].z);
		m_uiFireball_Timer = urand(12000,20000);
		m_uiFirehit_Timer  = urand(20000,30000);
		m_uiFirenova_Timer = urand(7000,12000);
	}
	void UpdateAI(const uint32 diff)
	{
		
		if (m_creature->getVictim())
		{
			if (m_uiFireball_Timer < diff)
			{
				DoCastSpellIfCan(m_creature->getVictim(),SPELL_FIREBALL);
				m_uiFireball_Timer = urand(12000,20000);
			}
			else
			{
				m_uiFireball_Timer -= diff;
			}

			if (m_uiFirehit_Timer < diff)
			{
				Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1);
				if (target)
					DoCastSpellIfCan(target,SPELL_FIREHIT);
				
				m_uiFirehit_Timer = urand(20000,30000);
			}
			else
			{
				m_uiFirehit_Timer -= diff;
			}
			
			if (m_uiFirenova_Timer < diff)
			{
				DoCastSpellIfCan(m_creature,SPELL_FIRENOVA,true);
				m_uiFirenova_Timer = urand(7000,12000);
			}
			else
			{
				m_uiFirenova_Timer -= diff;
			}
		}
		if (m_uiWalkTimer)
        {
            if (m_uiWalkTimer <= diff)
            {
                m_creature->GetMotionMaster()->MovePoint(id,PusillinWP[id].x, PusillinWP[id].y, PusillinWP[id].z);
				m_uiWalkTimer = 0;

            }else m_uiWalkTimer -= diff;
        }
		DoMeleeAttackIfReady();
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			m_creature->CombatStop();
		
	}
	void MovementInform(uint32 type, uint32 tempid)
    {
		if (id == 4)
			DoCastSpellIfCan(m_creature,SPELL_GROWUP);
		if (id == 16)
			return;
		if (id < 11)
			DoScriptText(SAY_START,m_creature);
		else
			DoScriptText(SAY_END,m_creature);
		++id;
		m_uiWalkTimer = 200;
	}

};

CreatureAI* GetAI_boss_pusillin(Creature* pCreature)
{
    boss_pusillinAI* Pusillie_AI = new boss_pusillinAI(pCreature);
	
	return ((CreatureAI*)Pusillie_AI);
}
void AddSC_boss_pusillin()
{
    Script *newscript;

	newscript = new Script;
    newscript->Name = "boss_pusillin";
    newscript->GetAI = &GetAI_boss_pusillin;
    newscript->RegisterSelf();
}