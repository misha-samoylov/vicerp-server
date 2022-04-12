#include <stdio.h>
#include <string.h>

#include <hiredis.h>

#include "vcmp20.h"

#define SERVER_NAME "ViceRP"
#define SERVER_GAMEMODE "TDM RP 0.1"

#define COLOR_RED 0xFF0000FF
#define COLOR_GREY 0xAFAFAFAA
#define COLOR_WHITE 0xFFFFFFAA
#define COLOR_YELLOW 0xFFFF00AA

#define REDIS_SERVER "127.0.0.1"
#define REDIS_PORT 6379
#define REDIS_PASSWORD "foobared"

PluginFuncs *g_plugin_funcs;
redisContext *c;

int redis_init()
{
	c = redisConnect(REDIS_SERVER, REDIS_PORT);

	if (c != NULL && c->err) {
		printf("Error: %s\n", c->errstr);
		return -1;
	} else {
		printf("Connected to Redis\n");
	}

	return 0;
}

void redis_auth()
{
	redisReply *reply;
	reply = redisCommand(c, "AUTH %s", REDIS_PASSWORD);
	freeReplyObject(reply);
}

void redis_deinit()
{
	redisFree(c);
}

void redis_set_user_online(const char *player_name)
{
	redisReply *reply;
	reply = redisCommand(c,"sadd online_users %s", player_name);
	freeReplyObject(reply);
}

void redis_set_user_offline(const char *player_name)
{
	redisReply *reply;
	reply = redisCommand(c,"srem online_users %s", player_name);
	freeReplyObject(reply);
}

int redis_get_count_users_online()
{
	redisReply *reply;
	int online_users;

	reply = redisCommand(c,"SCARD online_users");
	printf("GET online_users: %s\n", reply->str);
	online_users = reply->integer;
	freeReplyObject(reply);

	return online_users;
}

void send_client_message_to_all(int32_t color, const char *msg)
{
	int i;
	uint32_t max_players;
	max_players = g_plugin_funcs->GetMaxPlayers();

	for (i = 0; i < max_players; i++) {
		g_plugin_funcs->SendClientMessage(i, color, msg);
	}
}

int is_skin_citizen(int32_t skin_id)
{
	int ret;

	switch(skin_id) {
	case 1:
	case 2:
	case 3:
	case 4:
	case 99:
		ret = 0;
	break;
	default:
		ret = 1;
	}

	return ret;
}

void spawn_pickups()
{
	int32_t world;
	int32_t quantity;
	int32_t alpha;
	uint8_t is_automatic;

	world = 0;
	quantity = 100;
	alpha = 255;
	is_automatic = 1;

	g_plugin_funcs->CreatePickup(368, world, quantity, -855.5118, -631.2861, 11.3756, alpha, is_automatic); /* 24-7armor */
	g_plugin_funcs->CreatePickup(368, world, quantity, 407.7466, -483.1101, 12.3432, alpha, is_automatic); /* copshoparmor */
	g_plugin_funcs->CreatePickup(288, world, quantity, 311.0720, -238.6959, 38.3752, alpha, is_automatic); /* conssite flamethrower */
	g_plugin_funcs->CreatePickup(368, world, quantity, 266.4264, -249.8750, 36.1315, alpha, is_automatic); /* conssite armour */
	g_plugin_funcs->CreatePickup(366, world, quantity, 373.7430, -255.7513, 46.0797, alpha, is_automatic); /* conssite health */
	g_plugin_funcs->CreatePickup(368, world, quantity, 341.0047, -230.0483, 38.3752, alpha, is_automatic); /* conssite armour */
	g_plugin_funcs->CreatePickup(288, world, quantity, -990.6908, 263.9287, 8.8123, alpha, is_automatic); /* printunderground flamethrower */
	g_plugin_funcs->CreatePickup(278, world, quantity, -988.6908, 263.9287, 8.8123, alpha, is_automatic); /* printunderground stubby */
	g_plugin_funcs->CreatePickup(280, world, quantity, -986.6908, 263.9287, 8.8123, alpha, is_automatic); /* printunderground m4 */
	g_plugin_funcs->CreatePickup(368, world, quantity, -1034.1990, 41.1718, 11.3544, alpha, is_automatic); /* nearpizzagrave armour */
	g_plugin_funcs->CreatePickup(368, world, quantity, -1472.9331, -866.6178, 20.8979, alpha, is_automatic); /* airportarmour */
	g_plugin_funcs->CreatePickup(277, world, quantity, -672.05, 749.44, 10.90, alpha, is_automatic);
	g_plugin_funcs->CreatePickup(368, world, quantity, -671.05, 749.44, 10.90, alpha, is_automatic);
	g_plugin_funcs->CreatePickup(278, world, quantity, -670.05, 749.44, 10.90, alpha, is_automatic);
	g_plugin_funcs->CreatePickup(366, world, quantity, -113.2, -975.7, 10.4, alpha, is_automatic);
	g_plugin_funcs->CreatePickup(366, world, quantity, -225.1, -1158.1, 9.1, alpha, is_automatic); 
	g_plugin_funcs->CreatePickup(366, world, quantity, 456.2, -471.4, 16.6, alpha, is_automatic); 
	g_plugin_funcs->CreatePickup(366, world, quantity, 377.4, 467.7, 11.6, alpha, is_automatic); 
	g_plugin_funcs->CreatePickup(366, world, quantity, 469.6, 697.4, 11.7, alpha, is_automatic); 
	g_plugin_funcs->CreatePickup(366, world, quantity, 385.3, 1210.9, 19.4, alpha, is_automatic); 
	g_plugin_funcs->CreatePickup(366, world, quantity, 384.3, 756.6, 11.7, alpha, is_automatic); 
	g_plugin_funcs->CreatePickup(366, world, quantity, 10.7, 1099.0, 16.6, alpha, is_automatic); 
	g_plugin_funcs->CreatePickup(366, world, quantity, 85.3, 402.7, 19.8, alpha, is_automatic); 
	g_plugin_funcs->CreatePickup(366, world, quantity, -711.7, -501.7, 11.3, alpha, is_automatic); 
	g_plugin_funcs->CreatePickup(366, world, quantity, -404, -588.0, 11.6, alpha, is_automatic); 
	g_plugin_funcs->CreatePickup(366, world, quantity, -406.2503, -566.4947, 19.5804, alpha, is_automatic);
	g_plugin_funcs->CreatePickup(366, world, quantity, -478.1,  1438.5, 16.1, alpha, is_automatic);
	g_plugin_funcs->CreatePickup(366, world, quantity, -67,  1263.3, 25.1, alpha, is_automatic);
	g_plugin_funcs->CreatePickup(366, world, quantity, -821.8, 1144.8, 26.1, alpha, is_automatic);
	g_plugin_funcs->CreatePickup(366, world, quantity, -1139.4, 55.4, 11.2, alpha, is_automatic);
	g_plugin_funcs->CreatePickup(366, world, quantity, -1290.9, 91.9, 26.9, alpha, is_automatic);
	g_plugin_funcs->CreatePickup(366, world, quantity, -885.4, -483.4,  13.1, alpha, is_automatic);
	g_plugin_funcs->CreatePickup(366, world, quantity, -925.1, -638.3, 16.0, alpha, is_automatic);
	g_plugin_funcs->CreatePickup(366, world, quantity, -692.4, -1283.8, 11.1, alpha, is_automatic);
	g_plugin_funcs->CreatePickup(366, world, quantity, -655.1, -1506.3, 8.1, alpha, is_automatic);
	g_plugin_funcs->CreatePickup(366, world, quantity, -1399.4, -865.9, 20.9, alpha, is_automatic);
	g_plugin_funcs->CreatePickup(366, world, quantity, -822.6, 1137.3, 12.4, alpha, is_automatic);
	g_plugin_funcs->CreatePickup(366, world, quantity, -851.4, -78.8, 11.5, alpha, is_automatic);
	g_plugin_funcs->CreatePickup(366, world, quantity, -834.2, 740.6, 11.3, alpha, is_automatic);
}

uint8_t on_server_init()
{
	int err;

	err = redis_init();
	if (err) {
		return 0;
	}

	redis_auth();

	g_plugin_funcs->SetServerName(SERVER_NAME);
	g_plugin_funcs->SetGameModeText(SERVER_GAMEMODE);

	g_plugin_funcs->SetSpawnPlayerPosition(561.666809, 25.169380, 17.751160);
	g_plugin_funcs->SetSpawnCameraPosition(564.672852, 25.190231, 17.751160);
	g_plugin_funcs->SetSpawnCameraLookAt(561.666809, 25.169380, 17.751160);

	g_plugin_funcs->SetTimeRate(1000);
	g_plugin_funcs->SetHour(12);
	g_plugin_funcs->SetMinute(00);

	g_plugin_funcs->SetServerOption(vcmpServerOptionJoinMessages, 0);
	g_plugin_funcs->SetServerOption(vcmpServerOptionFriendlyFire, 0);

	spawn_pickups();

	int skins[] = { 12, 15, 33, 96, 131, 1, 2, 3, 4, 99 };
	for (int i = 0; i < sizeof(skins) / sizeof(int); i++) {
		if (is_skin_citizen(skins[i])) {
			g_plugin_funcs->AddPlayerClass(i, COLOR_WHITE, skins[i], 335.674164, -242.812607, 29.646561, 1.575135, 2, 1, 17, 400, 21, 100);
		} else {
			g_plugin_funcs->AddPlayerClass(i, COLOR_YELLOW, skins[i], 508.949005, 510.883820, 12.106688, 3.138477, 4, 1, 17, 400, 19, 100);
		}
	}

	return 1;
}

void on_server_shutdown()
{
	redis_deinit();
}

uint8_t on_player_request_class(int32_t player_id, int32_t offset)
{
	g_plugin_funcs->SetPlayerHeading(player_id, -1.557110);

	int32_t skin_id = g_plugin_funcs->GetPlayerSkin(player_id);

	if (is_skin_citizen(skin_id)) {
		g_plugin_funcs->SendGameMessage(player_id, 0, "citizen");
	} else {
		g_plugin_funcs->SendGameMessage(player_id, 0, "police");
	}

	return 1;
}

uint8_t on_player_command(int32_t player_id, const char* message)
{
	char player_name[64];
	g_plugin_funcs->GetPlayerName(player_id, player_name, sizeof(player_name));

	if (strcmp(message, "save") == 0) {
		float x, y, z, angle;

		g_plugin_funcs->GetPlayerPosition(player_id, &x, &y, &z);
		angle = g_plugin_funcs->GetPlayerHeading(player_id);

		g_plugin_funcs->SendClientMessage(player_id, COLOR_GREY, "%f, %f, %f, %f", x, y, z, angle);
		printf("%f, %f, %f, %f", x, y, z, angle);

		return 1;
	}
	else if (strcmp(message, "car") == 0) {
		float x, y, z, angle;
        g_plugin_funcs->GetPlayerPosition(player_id, &x, &y, &z);
		g_plugin_funcs->CreateVehicle(132, 0, x, y, z, 0.0, 1, 2);

		g_plugin_funcs->SendClientMessage(player_id, COLOR_GREY, ">> %s spawned vehicle (/car)", player_name);

		return 1;
	}
	else if (strcmp(message, "heal") == 0) {
		g_plugin_funcs->SetPlayerHealth(player_id, 100.0);
		g_plugin_funcs->SendClientMessage(player_id, COLOR_GREY, ">> %s bought health (/heal)", player_name);

		return 1;
	}
	else if (strcmp(message, "armour") == 0) {
		g_plugin_funcs->SetPlayerArmour(player_id, 100.0);
        g_plugin_funcs->SendClientMessage(player_id, COLOR_GREY, ">> %s bought armour (/armour)", player_name);

		return 1;
	} else if (strcmp(message, "online") == 0) {
		int users_online = redis_get_count_users_online();
        g_plugin_funcs->SendClientMessage(player_id, COLOR_GREY, "** pm >> Now online users: %d (/online)", users_online);

		return 1;
	} else {
		g_plugin_funcs->SendClientMessage(player_id, COLOR_RED, "** pm >> Unknown command");
	}

	return 0;
}

void on_player_spawn(int32_t player_id)
{
	char player_name[64];
	g_plugin_funcs->GetPlayerName(player_id, player_name, sizeof(player_name));

	g_plugin_funcs->SendClientMessage(player_id, COLOR_GREY, ">> %s spawned", player_name);
}

void on_player_connect(int32_t player_id)
{
	char player_name[64];
    g_plugin_funcs->GetPlayerName(player_id, player_name, sizeof(player_name));

	redis_set_user_online(player_name);

	char msg[256];
	sprintf(msg, ">> %s connected to server", player_name);
	send_client_message_to_all(COLOR_GREY, msg);

	g_plugin_funcs->SendClientMessage(player_id, COLOR_YELLOW, "** pm >> Welcome to %s. Enter /help for more information", SERVER_NAME);
}

void on_player_disconnect(int32_t player_id, vcmpDisconnectReason reason)
{
	char player_name[64];
    g_plugin_funcs->GetPlayerName(player_id, player_name, sizeof(player_name));

	redis_set_user_offline(player_name);
}

unsigned int VcmpPluginInit(PluginFuncs* plugin_funcs, PluginCallbacks* plugin_calls, PluginInfo* plugin_info)
{
	g_plugin_funcs = plugin_funcs;

	plugin_info->apiMajorVersion = PLUGIN_API_MAJOR;
	plugin_info->apiMinorVersion = PLUGIN_API_MINOR;

	plugin_calls->OnServerInitialise = on_server_init;
	plugin_calls->OnServerShutdown = on_server_shutdown;
	plugin_calls->OnPlayerRequestClass = on_player_request_class;
	plugin_calls->OnPlayerCommand = on_player_command;
	plugin_calls->OnPlayerSpawn = on_player_spawn;
	plugin_calls->OnPlayerConnect = on_player_connect;
	plugin_calls->OnPlayerDisconnect = on_player_disconnect;

	return 1;
}
