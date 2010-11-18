#include "precompiled.h"

struct MANGOS_DLL_DECL custom_cyberneticAI : public ScriptedAI
{
	custom_cyberneticAI(Creature *c) : ScriptedAI(c) {Reset();}

void Reset(){};
void Aggro(Unit *who){};

};
bool GossipHello_custom_cybernetic(Player *player, Creature *_Creature)
{
	 player->ADD_GOSSIP_ITEM( 0,"Ich wuerde gerne etwas ueber den Server erfahren."     , GOSSIP_SENDER_MAIN, 1);
	 player->ADD_GOSSIP_ITEM( 0,"Wie lautet nochmal die Internetadresse?"     , GOSSIP_SENDER_MAIN, 2);
	 player->ADD_GOSSIP_ITEM( 0,"Wie kann ich einen GM erreichen?"     , GOSSIP_SENDER_MAIN, 3);
	 player->ADD_GOSSIP_ITEM( 0,"Wie steht es mit den Serverregeln?"     , GOSSIP_SENDER_MAIN, 4);
	 player->ADD_GOSSIP_ITEM( 0,"Wie kann ich den Server unterstuetzen?"     , GOSSIP_SENDER_MAIN, 5);
	 player->ADD_GOSSIP_ITEM( 0,"Kannst du mich vielleicht hochleveln oder mir Geld geben?"     , GOSSIP_SENDER_MAIN, 6);
	 player->ADD_GOSSIP_ITEM( 0,"Wann kommt Wrath of the Lich King?"     , GOSSIP_SENDER_MAIN, 7);
	 player->PlayerTalkClass->SendGossipMenu(468000,_Creature->GetGUID());
	 return true;
}

bool GossipSelect_custom_cybernetic(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
	switch(action)
	{
	case 1:
	player->PlayerTalkClass->SendGossipMenu(468001,_Creature->GetGUID());
	break;

	case 2:
	player->PlayerTalkClass->SendGossipMenu(468002,_Creature->GetGUID());
	break;

	case 3:
	player->PlayerTalkClass->SendGossipMenu(468003,_Creature->GetGUID());
	break;

	case 4:
	player->PlayerTalkClass->SendGossipMenu(468004,_Creature->GetGUID());
	break;

	case 5:
	player->PlayerTalkClass->SendGossipMenu(468005,_Creature->GetGUID());
	break;

	case 6:
	player->PlayerTalkClass->SendGossipMenu(468006,_Creature->GetGUID());
	break;

	case 7:
	player->PlayerTalkClass->SendGossipMenu(468007,_Creature->GetGUID());
	break;

	}
	return true;
}

CreatureAI* GetAI_custom_cybernetic(Creature *_Creature)
{
    return new custom_cyberneticAI (_Creature);
}

void AddSC_custom_cybernetic()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "custom_cybernetic";
	newscript->GetAI = &GetAI_custom_cybernetic;
    newscript->pGossipHello =  &GossipHello_custom_cybernetic;
    newscript->pGossipSelect = &GossipSelect_custom_cybernetic;
	//newscript->pReceiveEmote = &ReceiveEmote_custom_example;
	newscript->RegisterSelf();
}

