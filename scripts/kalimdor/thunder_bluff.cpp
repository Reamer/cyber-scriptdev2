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
SDName: Thunder_Bluff
SD%Complete: 100
SDComment: Quest support: 925
SDCategory: Thunder Bluff
EndScriptData */

#include "precompiled.h"

/*#####
# npc_cairne_bloodhoof
######*/

enum
{
    SPELL_BERSERKER_CHARGE  = 16636,
    SPELL_CLEAVE            = 16044,
    SPELL_MORTAL_STRIKE     = 16856,
    SPELL_THUNDERCLAP       = 23931,
    SPELL_UPPERCUT          = 22916,
    SPELL_WAR_STOMP         = 59705,
};

//TODO: verify abilities/timers
struct MANGOS_DLL_DECL npc_cairne_bloodhoofAI : public ScriptedAI
{
    npc_cairne_bloodhoofAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    uint32 BerserkerCharge_Timer;
    uint32 Cleave_Timer;
    uint32 MortalStrike_Timer;
    uint32 Thunderclap_Timer;
    uint32 Uppercut_Timer;
    uint32 m_uiWarStompTimer;

    void Reset()
    {
        BerserkerCharge_Timer = 30000;
        Cleave_Timer = 5000;
        MortalStrike_Timer = 10000;
        Thunderclap_Timer = 15000;
        Uppercut_Timer = 10000;
        m_uiWarStompTimer = 25000;
    }

    void Aggro(Unit* pWho)
    {
        m_creature->CallForHelp(100);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (BerserkerCharge_Timer < uiDiff)
        {
            Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
            if (target)
                DoCastSpellIfCan(target,SPELL_BERSERKER_CHARGE);
            BerserkerCharge_Timer = 25000;
        }else BerserkerCharge_Timer -= uiDiff;

        if (Uppercut_Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_UPPERCUT);
            Uppercut_Timer = 20000;
        }else Uppercut_Timer -= uiDiff;

        if (Thunderclap_Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_THUNDERCLAP);
            Thunderclap_Timer = 15000;
        }else Thunderclap_Timer -= uiDiff;

        if (MortalStrike_Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_MORTAL_STRIKE);
            MortalStrike_Timer = 15000;
        }else MortalStrike_Timer -= uiDiff;

        if (Cleave_Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_CLEAVE);
            Cleave_Timer = 7000;
        }else Cleave_Timer -= uiDiff;

        if (m_uiWarStompTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_WAR_STOMP);
            m_uiWarStompTimer = urand(20000, 25000);
        }else m_uiWarStompTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_npc_cairne_bloodhoof(Creature* pCreature)
{
    return new npc_cairne_bloodhoofAI(pCreature);
}

bool GossipHello_npc_cairne_bloodhoof(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetObjectGuid());

    if (pPlayer->GetQuestStatus(925) == QUEST_STATUS_INCOMPLETE)
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I know this is rather silly but a young ward who is a bit shy would like your hoofprint.", GOSSIP_SENDER_MAIN, GOSSIP_SENDER_INFO);

    pPlayer->SEND_GOSSIP_MENU(7013, pCreature->GetObjectGuid());

    return true;
}

bool GossipSelect_npc_cairne_bloodhoof(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction == GOSSIP_SENDER_INFO)
    {
        pPlayer->CastSpell(pPlayer, 23123, false);
        pPlayer->SEND_GOSSIP_MENU(7014, pCreature->GetObjectGuid());
    }
    return true;
}

void AddSC_thunder_bluff()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "npc_cairne_bloodhoof";
    newscript->GetAI = &GetAI_npc_cairne_bloodhoof;
    newscript->pGossipHello = &GossipHello_npc_cairne_bloodhoof;
    newscript->pGossipSelect = &GossipSelect_npc_cairne_bloodhoof;
    newscript->RegisterSelf();
}