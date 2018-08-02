#include "GameTime.h"
#include "ScriptMgr.h"
#include "World.h"
#include "WorldSession.h"
#include "Player.h"

class premium_boost_honor : public PlayerScript
{
    public:
        premium_boost_honor() : PlayerScript("premium_boost_honor") { }

        void OnHonorGainCalculation(Player* player, float& honor_f) override
        {
            if (player->GetSession()->IsPremium())
                honor_f *= sWorld->getRate(RATE_HONOR_BOOST_PREMIUM);
        }
};

class premium_boost_xp : public FormulaScript
{
    public:
        premium_boost_xp() : FormulaScript("premium_boost_xp") { }

        void OnGainCalculation(uint32& gain, Player* player, Unit* /*unit*/) override
        {
            if (player->GetSession()->IsPremium())
                gain *= sWorld->getRate(RATE_XP_BOOST_PREMIUM);
        }
};

void AddSC_custom_rate()
{
    new premium_boost_honor();
    new premium_boost_xp();
}