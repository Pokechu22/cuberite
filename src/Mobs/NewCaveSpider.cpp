#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "NewCaveSpider.h"
#include "../World.h"





cNewCaveSpider::cNewCaveSpider(void) :
	super("CaveSpider", mtCaveSpider, "mob.spider.say", "mob.spider.death", 0.7, 0.5)
{
}





void cNewCaveSpider::Tick(float a_Dt, cChunk & a_Chunk)
{
	super::Tick(a_Dt, a_Chunk);
}





void cNewCaveSpider::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	int LootingLevel = 0;
	if (a_Killer != NULL)
	{
		LootingLevel = a_Killer->GetEquippedWeapon().m_Enchantments.GetLevel(cEnchantments::enchLooting);
	}
	AddRandomDropItem(a_Drops, 0, 2 + LootingLevel, E_ITEM_STRING);
	if ((a_Killer != NULL) && (a_Killer->IsPlayer() || a_Killer->IsA("cWolf")))
	{
		AddRandomUncommonDropItem(a_Drops, 33.0f, E_ITEM_SPIDER_EYE);
	}
}



