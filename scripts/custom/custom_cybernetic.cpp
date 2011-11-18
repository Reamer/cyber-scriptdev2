#include "precompiled.h"
#include "Chat.h"

enum Integer
{
    AUCHENAIKRYPTA          = 1,
    MANAGRUFT               = 2,
    SCHATTENLABYRINTH       = 3,
    SETHEKKHALLEN           = 4,
    TIEFENSUMPF             = 5,
    DAMPFKAMMER             = 6,
    SKLAVENUNTERKUENFTE     = 7,
    ARKATRAZ                = 8,
    BOTANIKA                = 9,
    MECHANAR                = 10,
    ZERSCHMETTERTENHALLEN   = 11,
    TERRASSEDERMAGISTER     = 12,
    MARAUDON                = 13,
    SCHOLOMANCE             = 14,
    STRATHOLME              = 15,
    BURG_UTGARDE            = 16,
    AZJOL_NERUB             = 17,
    ALTE_KOENIGREICH        = 18,
    NEXUS                   = 19,
    DRAKTARON               = 20,

    AUCHENAIKRYPTA_BOSS         = 20306,
    MANAGRUFT_BOSS              = 20266,
    SCHATTENLABYRINTH_BOSS      = 20657,
    SETHEKKHALLEN_BOSS          = 20706,
    TIEFENSUMPF_BOSS            = 20184,
    DAMPFKAMMER_BOSS            = 20633,
    SKLAVENUNTERKUENFTE_BOSS    = 19894,
    ARKATRAZ_BOSS               = 21601,
    BOTANIKA_BOSS               = 21582,
    MECHANAR_BOSS               = 21537,
    ZERSCHMETTERTENHALLEN_BOSS  = 20597,
    TERRASSEDERMAGISTER_BOSS    = 24857,
    MARAUDON_BOSS               = 12201,
    SCHOLOMANCE_BOSS            = 1853,
    STRATHOLME_BOSS             = 10440,
    BURG_UTGARDE_BOSS           = 31673,
    AZJOL_NERUB_BOSS            = 31610,
    ALTE_KOENIGREICH_BOSS       = 31464,
    NEXUS_BOSS                  = 30540,
    DRAKTARON_BOSS              = 31360,

    ITEM_SCHNELLSTES_DUNGEON    = 17894,
    ITEM_CYBER_CREDIT           = 54822,
    ITEM_RING                   = 27257,
    RESET                       = 100,
    FERTIG                      = 101
};

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
     player->PlayerTalkClass->SendGossipMenu(468000,_Creature->GetObjectGuid());
     return true;
}

bool GossipSelect_custom_cybernetic(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    switch(action)
    {
        case 1:
            player->PlayerTalkClass->SendGossipMenu(468001,_Creature->GetObjectGuid());
            break;

        case 2:
            player->PlayerTalkClass->SendGossipMenu(468002,_Creature->GetObjectGuid());
            break;

        case 3:
            player->PlayerTalkClass->SendGossipMenu(468003,_Creature->GetObjectGuid());
            break;

        case 4:
            player->PlayerTalkClass->SendGossipMenu(468004,_Creature->GetObjectGuid());
            break;

        case 5:
            player->PlayerTalkClass->SendGossipMenu(468005,_Creature->GetObjectGuid());
            break;

        case 6:
            player->PlayerTalkClass->SendGossipMenu(468006,_Creature->GetObjectGuid());
            break;

        case 7:
            player->PlayerTalkClass->SendGossipMenu(468007,_Creature->GetObjectGuid());
            break;

    }
    return true;
}

CreatureAI* GetAI_custom_cybernetic(Creature *_Creature)
{
    return new custom_cyberneticAI (_Creature);
}

bool GossipHello_custum_cybernetic_2(Player* pPlayer, Creature* pCreature)
{
    if (pPlayer->isGameMaster())
    {
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Dungeon auf Zeit soll gestartet werden. Zeig mir moegliche Instanzen!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Dungeon auf Zeit soll resetet werden!", GOSSIP_SENDER_MAIN, RESET);
    }
    QueryResult * result = strSD2Pquery("SELECT Endzeit FROM schnellstesdungeon WHERE AmLaufen = 1;");
    if (result)
    {
        Field *fields = result->Fetch();
        uint64 endzeit            = fields[0].GetUInt64();
        if (endzeit > uint64(time(NULL)))
        {
            if (pPlayer->HasItemCount(ITEM_SCHNELLSTES_DUNGEON, 1, false))
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Dungeon auf Zeit: Meine Gruppe ist fertig, wir moechten abgeben", GOSSIP_SENDER_MAIN, FERTIG);
        }
        else
        {
            strSD2Pquery("UPDATE schnellstesdungeon SET AmLaufen = 0;");
            SetFastDungeon(RESET);
        }
    }
    delete result;
    pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetObjectGuid());
    return true;
}

bool addItem(Player* pPlayer, uint32 anzahl, uint32 itemId)
{
    ItemPosCountVec dest;
    InventoryResult msg = pPlayer->CanStoreNewItem( NULL_BAG, NULL_SLOT, dest, itemId , anzahl);
    if (msg == EQUIP_ERR_OK)
    {
        Item* item = pPlayer->StoreNewItem( dest, itemId, true);
        pPlayer->SendNewItem(item,anzahl,false,true);
        return true;
    }
    return false;
}

void SendDefaultMenu_custom_cybernetic_2(Player *pPlayer, Creature *pCreature, uint32 action )
{
    switch(action)
    {
        case GOSSIP_ACTION_INFO_DEF:
        {
            QueryResult* result = strSD2Pquery("SELECT * FROM schnellstesdungeon WHERE AmLaufen = 1;");
            if (result)
            {
                pPlayer->CLOSE_GOSSIP_MENU();
                pCreature->MonsterSay("Es laeuft bereits ein schnellstes Dungeon - Abbruch", LANG_UNIVERSAL);
            }
            else
            {
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Auchindoun: Auchenaikrypta", GOSSIP_SENDER_MAIN, AUCHENAIKRYPTA);
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Auchindoun: Managruft", GOSSIP_SENDER_MAIN, MANAGRUFT);
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Auchindoun: Schattenlabyrinth", GOSSIP_SENDER_MAIN, SCHATTENLABYRINTH);
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Auchindoun: Sethekkhallen", GOSSIP_SENDER_MAIN, SETHEKKHALLEN);
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Der Echsenkessel: Der Tiefensumpf", GOSSIP_SENDER_MAIN, TIEFENSUMPF);
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Der Echsenkessel: Die Dampfkammer", GOSSIP_SENDER_MAIN, DAMPFKAMMER);
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Der Echsenkessel: Die Sklavenunterkuenfte", GOSSIP_SENDER_MAIN, SKLAVENUNTERKUENFTE);
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Festung der Stuerme: Die Arkatraz", GOSSIP_SENDER_MAIN, ARKATRAZ);
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Festung der Stuerme: Die Botanika", GOSSIP_SENDER_MAIN, BOTANIKA);
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Festung der Stuerme: Die Mechanar", GOSSIP_SENDER_MAIN, MECHANAR);
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Hoellenfeuerzitadelle: Die Zerschmetterten Hallen", GOSSIP_SENDER_MAIN, ZERSCHMETTERTENHALLEN);
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Terrasse der Magister", GOSSIP_SENDER_MAIN, TERRASSEDERMAGISTER);
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Maraudon", GOSSIP_SENDER_MAIN, MARAUDON);
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Scholomance", GOSSIP_SENDER_MAIN, SCHOLOMANCE);
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Stratholme", GOSSIP_SENDER_MAIN, STRATHOLME);
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Burg Utgarde: Burg Utgarde", GOSSIP_SENDER_MAIN, BURG_UTGARDE);
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Azjol-Nerub: Azjol-Nerub", GOSSIP_SENDER_MAIN, AZJOL_NERUB);
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Azjol-Nerub: Ahn'kahet: Das Alte Koenigreich", GOSSIP_SENDER_MAIN, ALTE_KOENIGREICH);
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Der Nexus: Der Nexus", GOSSIP_SENDER_MAIN, NEXUS);
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Feste Drak'Tharon", GOSSIP_SENDER_MAIN, DRAKTARON);
                pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetObjectGuid());
            }
            delete result;
            break;
        }
        case RESET:
        {
            pPlayer->CLOSE_GOSSIP_MENU();
            strSD2Pquery("UPDATE schnellstesdungeon SET AmLaufen = 0;");
            SetFastDungeon(RESET);
            break;
        }
        case FERTIG:
        {
            pPlayer->CLOSE_GOSSIP_MENU();
            uint32 anzahlmarken = 1;
            //                              1       2           3           4             5
            QueryResult* result = strSD2Pquery("SELECT Instanz, Startzeit, Endzeit, GruppeEinsFertig, Rekord FROM schnellstesdungeon WHERE AmLaufen = 1;");
            if (result)
            {
                Field *fields = result->Fetch();
                uint32 instanz          = fields[0].GetUInt32();
                uint64 start            = fields[1].GetUInt64();
                uint64 end              = fields[2].GetUInt64();
                uint32 fertig           = fields[3].GetUInt32();
                uint64 rekord           = fields[4].GetUInt64();
                if ((uint64(time(NULL)) - start) < rekord)
                {
                    char query[MAX_QUERY_LEN];
                    SendServerMessage("Es wurde ein neuer Rekord aufgestellt im schnellsten Dungeon. Herzlichen Glueckwunsch.");
                    sprintf(query, "UPDATE schnellstesdungeon SET rekord = '"UI64FMTD"' WHERE AmLaufen = 1;", (uint64(time(NULL)) - start));
                    strSD2Pquery(query);
                    ++anzahlmarken;
                }
                if (fertig != 1)
                {
                    pCreature->MonsterSay("Ihr seid die Ersten. Herzlichen Glueckwunsch.", LANG_UNIVERSAL);
                    strSD2Pquery("UPDATE schnellstesdungeon SET GruppeEinsFertig = 1  WHERE AmLaufen = 1;");
                    ++anzahlmarken;
                }
                if (Group* grp = pPlayer->GetGroup())
                {
                    for(GroupReference *itr = grp->GetFirstMember(); itr != NULL; itr = itr->next())
                    {
                        Player* member = itr->getSource();
                        if (!member)
                            continue;
                        int32 subtract = 5;
                        member->DestroyItemCount(ITEM_SCHNELLSTES_DUNGEON, -subtract, true, false);

                        if (!addItem(member,anzahlmarken, ITEM_CYBER_CREDIT))
                            member->MonsterSay("Ich habe nichts bekommen.", LANG_UNIVERSAL);
                    }
                }
            }
            delete result;
            break;
        }
        case AUCHENAIKRYPTA:
        case MANAGRUFT:
        case SCHATTENLABYRINTH:
        case SETHEKKHALLEN:
        case TIEFENSUMPF:
        case DAMPFKAMMER:
        case SKLAVENUNTERKUENFTE:
        case ARKATRAZ:
        case BOTANIKA:
        case MECHANAR:
        case ZERSCHMETTERTENHALLEN:
        case TERRASSEDERMAGISTER:
        case MARAUDON:
        case SCHOLOMANCE:
        case STRATHOLME:
        case BURG_UTGARDE:
        case AZJOL_NERUB:
        case ALTE_KOENIGREICH:
        case NEXUS:
        case DRAKTARON:
        {
            pPlayer->CLOSE_GOSSIP_MENU();
            SetFastDungeon(action);
            char query[MAX_QUERY_LEN];
            sprintf(query, "UPDATE schnellstesdungeon SET Startzeit = '"UI64FMTD"', Endzeit = '"UI64FMTD"', GruppeEinsFertig = 0, AmLaufen = 1 WHERE Instanz = %u;", uint64(time(NULL)), (uint64(time(NULL)) + 4*HOUR), action);
            strSD2Pquery(query);
            break;
        }
    }
}

bool GossipSelect_custum_cybernetic_2(Player *pPlayer, Creature *pCreature, uint32 sender, uint32 action )
{
    // Main menu
    if (sender == GOSSIP_SENDER_MAIN)
    {
        pPlayer->PlayerTalkClass->ClearMenus();
        SendDefaultMenu_custom_cybernetic_2(pPlayer, pCreature, action);
    }
    return true;
}

bool GossipHello_custum_cybernetic_3(Player* pPlayer, Creature* pCreature)
{

    pPlayer->PrepareGossipMenu(pCreature, pCreature->GetCreatureInfo()->GossipMenuId);
    pPlayer->SendPreparedGossip(pCreature);
    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Ich will meinen Super Tollen Ring haben.", GOSSIP_SENDER_MAIN, 555);
    pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetObjectGuid());
    return true;
}

bool GossipSelect_custum_cybernetic_3(Player *pPlayer, Creature *pCreature, uint32 sender, uint32 action )
{
    if (action == 555)
    {
        uint32 charCount = GetCharCountWithAccountId(pPlayer->GetSession()->GetAccountId());
        if (charCount > 1 || pPlayer->GetItemCount(ITEM_RING, true) >= 1)
        {
            pCreature->MonsterSay("Den Ring des Meister bekommt man nur einmal mit seinen ERSTEN Charakter", LANG_UNIVERSAL);
        }
        else
        {
            addItem(pPlayer, 1, ITEM_RING);
        }
    }
    pPlayer->CLOSE_GOSSIP_MENU();
    return true;
}

void AddSC_custom_cybernetic()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "custom_cybernetic";
    newscript->GetAI = &GetAI_custom_cybernetic;
    newscript->pGossipHello =  &GossipHello_custom_cybernetic;
    newscript->pGossipSelect = &GossipSelect_custom_cybernetic;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "custom_cybernetic_2";
    newscript->pGossipHello = &GossipHello_custum_cybernetic_2;
    newscript->pGossipSelect = &GossipSelect_custum_cybernetic_2;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "custom_cybernetic_3";
    newscript->pGossipHello = &GossipHello_custum_cybernetic_3;
    newscript->pGossipSelect = &GossipSelect_custum_cybernetic_3;
    newscript->RegisterSelf();
}

