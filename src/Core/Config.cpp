/*
Copyright(C) 2018 Brandan Tyler Lasley

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.
*/
#include "Config.h"
#include <fstream>
#include "Poco/DateTime.h"
#include <string>
#include "Poco/Logger.h"
#include "Util.h"

AppConfig GlobalConfig;

// IntenseCoin Config (Default)
#define RPC_FILENAME                            "evolution-wallet-rpc"
#define RPC_HOSTNAME                            "127.0.0.1"
#define DAEMON_ADDRESS                          "127.0.0.1:52922"
#define RPC_JSON                                "/json_rpc"
#define WALLET_PATH                             "Wallets/"
#define COIN_OFFSET                             1000000000.0 // 1 x 10^9
#define DEFAULT_MIXIN                           6
#define COIN_ABBV                               std::string("EVOX");
#define STARTING_PORT_NUMBER                    11000
#define MAX_RPC_LIMIT                           200
#define RPC_ERROR_GIVEUP                        3
#define BLOCKCHAIN_SAVE_TIME                    (5/* Minutes */*60)
#define SEARCH_FOR_NEW_TRANSACTIONS_TIME        (10/*In Seconds*/)
#define RPC_WALLETS_SAVE_TIME                   (60/*In Seconds*/)
#define RPC_WALLET_WATCHDOG                     (10/*In Minutes*/*60)
#define FAUCET_PERCENTAGE_ALLOWANCE             0.0001
#define MIN_DISCORD_ACCOUNT_IN_DAYS             (7.0*MICROSECOND_DAY)   // Days
#define FAUCET_TIMEOUT                          (16.0*MICROSECOND_HOUR) // Hours
#define VALID_ADDRESS_LENGTH                    98
#define INTEGRATED_ADDRESS_LENGTH               108
#define TICKET_COST                             100 // Coins
#define FACUET_DONATION_PERCENT                 0.20
#define NO_WINNER_CHANCE                        0.20
#define LOTTERY_DAY                             Poco::DateTime::FRIDAY
#define LOTTERY_CLOSE                           18
#define LOTTERY_PICK                            21
#define LOTTERY_FAUCET                          23
#define USE_TEST_NET                            false
#define MINUTES_UNTIL_NEXT_DRAWING (MICROSECOND_HOUR/2) // Every 30 minutes
#define MINUTES_UNTIL_NEXT_PAYMENT (MICROSECOND_HOUR/4) // Every 15 minutes

const DiscordID DiscordAdmins[] =
{
  208496265082830848

};

AppConfig::AppConfig()
{
    // About
    About.major = VERSION_MAJOR;
    About.minor = VERSION_MINOR;

    // Admins
    General.Admins  = std::vector<DiscordID>(DiscordAdmins, DiscordAdmins + sizeof DiscordAdmins / sizeof DiscordAdmins[0]);
    General.Quitting = false;
    General.Shutdown = false;
    General.Threads = 0;

    // RPC
    RPC.coin_abbv = COIN_ABBV;
    RPC.coin_offset = COIN_OFFSET;
    RPC.daemon_hostname = DAEMON_ADDRESS;
    RPC.filename = RPC_FILENAME;
    RPC.hostname = RPC_HOSTNAME;
    RPC.json_uri = RPC_JSON;
    RPC.mixin = DEFAULT_MIXIN;
    RPC.wallet_path = WALLET_PATH;
    RPC.address_length = VALID_ADDRESS_LENGTH;
    RPC.sub_address_length = VALID_ADDRESS_LENGTH;
    RPC.integrated_address_length = INTEGRATED_ADDRESS_LENGTH;
    RPC.use_test_net = USE_TEST_NET;

    // RPCManager
    RPCManager.blockchain_save_time = BLOCKCHAIN_SAVE_TIME;
    RPCManager.error_giveup = RPC_ERROR_GIVEUP;
    RPCManager.max_rpc_limit = MAX_RPC_LIMIT;
    RPCManager.search_for_new_transactions_time = SEARCH_FOR_NEW_TRANSACTIONS_TIME;
    RPCManager.starting_port_number = STARTING_PORT_NUMBER;
    RPCManager.wallets_save_time = RPC_WALLETS_SAVE_TIME;
    RPCManager.wallet_watchdog_time = RPC_WALLET_WATCHDOG;

    // Faucet
    Faucet.min_discord_account = MIN_DISCORD_ACCOUNT_IN_DAYS;
    Faucet.percentage_allowance = FAUCET_PERCENTAGE_ALLOWANCE;
    Faucet.timeout = FAUCET_TIMEOUT;

    // Lottery
    Lottery.close = LOTTERY_CLOSE;
    Lottery.day = LOTTERY_DAY;
    Lottery.donation_percent = FACUET_DONATION_PERCENT;
    Lottery.no_winner_chance = NO_WINNER_CHANCE;
    Lottery.pick = LOTTERY_PICK;
    Lottery.ticket_cost = TICKET_COST;
    Lottery.faucet = LOTTERY_FAUCET;

    // ChatRewards
    ChatRewards.next_drawing_time = MINUTES_UNTIL_NEXT_DRAWING;
    ChatRewards.next_payment_time = MINUTES_UNTIL_NEXT_PAYMENT;
}

void AppConfig::load_config(const std::string & file)
{
    auto & logger = Poco::Logger::get("AppConfig");
    currentConfig = file;
    std::ifstream in(currentConfig);
    if (in.is_open())
    {
        logger.information("Loading Config from disk...");
        {
            cereal::JSONInputArchive ar(in);
            ar(cereal::make_nvp("AppConfig", *this));
        }
        in.close();
    }
}

void AppConfig::save_config()
{
    auto & logger = Poco::Logger::get("AppConfig");
    std::ofstream out(currentConfig, std::ios::trunc);
    if (out.is_open())
    {
        logger.information("Saving Config to disk...");
        {
            cereal::JSONOutputArchive ar(out);
            ar(cereal::make_nvp("AppConfig", *this));
        }
        out.close();
    }
}
