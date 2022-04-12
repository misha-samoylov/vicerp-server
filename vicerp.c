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
	reply = redisCommand(c, "AUTH foobared");
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

uint8_t on_player_request_class(int32_t playerId, int32_t offset)
{
	g_plugin_funcs->SetPlayerHeading(playerId, -1.557110);

	int32_t skin_id = g_plugin_funcs->GetPlayerSkin(playerId);

	if (is_skin_citizen(skin_id)) {
		g_plugin_funcs->SendGameMessage(playerId, 0, "citizen");
	} else {
		g_plugin_funcs->SendGameMessage(playerId, 0, "police");
	}

	return 1;
}

uint8_t on_player_command(int32_t playerId, const char* message)
{
	char player_name[64];
	g_plugin_funcs->GetPlayerName(playerId, player_name, sizeof(player_name));

	if (strcmp(message, "save") == 0) {
		float x, y, z, angle;

		g_plugin_funcs->GetPlayerPosition(playerId, &x, &y, &z);
		angle = g_plugin_funcs->GetPlayerHeading(playerId);

		g_plugin_funcs->SendClientMessage(playerId, COLOR_GREY, "%f, %f, %f, %f", x, y, z, angle);
		printf("%f, %f, %f, %f", x, y, z, angle);

		return 1;
	}
	else if (strcmp(message, "car") == 0) {
		float x, y, z, angle;
        g_plugin_funcs->GetPlayerPosition(playerId, &x, &y, &z);
		g_plugin_funcs->CreateVehicle(132, 0, x, y, z, 0.0, 1, 2);

		g_plugin_funcs->SendClientMessage(playerId, COLOR_GREY, ">> %s spawned vehicle (/car)", player_name);

		return 1;
	}
	else if (strcmp(message, "heal") == 0) {
		g_plugin_funcs->SetPlayerHealth(playerId, 100.0);
		g_plugin_funcs->SendClientMessage(playerId, COLOR_GREY, ">> %s bought health (/heal)", player_name);

		return 1;
	}
	else if (strcmp(message, "armour") == 0) {
		g_plugin_funcs->SetPlayerArmour(playerId, 100.0);
        g_plugin_funcs->SendClientMessage(playerId, COLOR_GREY, ">> %s bought armour (/armour)", player_name);

		return 1;
	} else if (strcmp(message, "online") == 0) {
		int users_online = redis_get_count_users_online();
        g_plugin_funcs->SendClientMessage(playerId, COLOR_GREY, "** pm >> Now online users: %d (/online)", users_online);

		return 1;
	} else {
		g_plugin_funcs->SendClientMessage(playerId, COLOR_RED, "** pm >> Unknown command");
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
