#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <hiredis.h>

#include "plugin.h"
#include "config.h"

#define SERVER_NAME "ViceRP"
#define SERVER_GAMEMODE "TDM RP 0.2"

#define COLOR_RED 0xFF0000FF
#define COLOR_GREY 0xAFAFAFAA
#define COLOR_WHITE 0xFFFFFFAA
#define COLOR_YELLOW 0xFFFF00AA
#define COLOR_BLUE 0x0000FFFF

#define UNKNOWN_KILLER_ID -1

PluginFuncs *g_plugin_funcs;
redisContext *g_redis_context;

int32_t g_skins_citizen[] = { 12, 15, 33, 96, 131 };
int32_t g_skins_police[] = { 1, 2, 3, 4, 99 };

const char g_weapons[][64] = {
	{ "unarmed" },
	{ "brass knuckles" },
	{ "screw driver" },
	{ "golf club" },
	{ "night stick" },
	{ "knife" },
	{ "baseball bat" },
	{ "hammer" },
	{ "meat cleaver" },
	{ "machete" },
	{ "katana" },
	{ "chainsaw" },
	{ "grenade" },
	{ "remote detonation grenades" },
	{ "tear gas" },
	{ "molotov cocktails" },
	{ "rocket" }, /* not used in game */
	{ "colt45" },
	{ "python" },
	{ "shotgun" },
	{ "spas12 shotgun" },
	{ "stubby shotgun" },
	{ "tec9" },
	{ "uzi" },
	{ "silenced ingram" },
	{ "mp5" },
	{ "m4" },
	{ "ruger" },
	{ "sniper rifle" },
	{ "laser scope sniper rifle" },
	{ "rocket launcher" },
	{ "flame thrower" },
	{ "m60" },
	{ "minigun" }
};

int redis_init()
{
	g_redis_context = redisConnect(REDIS_SERVER, REDIS_PORT);

	if (g_redis_context != NULL && g_redis_context->err) {
		printf("Error: %s\n", g_redis_context->errstr);
		return -1;
	} else {
		printf("Connected to Redis\n");
	}

	return 0;
}

void redis_flush()
{
	redisReply *reply;
	reply = redisCommand(g_redis_context, "FLUSHDB");
	freeReplyObject(reply);
}

void redis_deinit()
{
	redisFree(g_redis_context);
}

void redis_set_user_online(const char *player_name)
{
	redisReply *reply;
	reply = redisCommand(g_redis_context, "SADD online_users %s", player_name);
	freeReplyObject(reply);
}

void redis_set_user_offline(const char *player_name)
{
	redisReply *reply;
	reply = redisCommand(g_redis_context,"SREM online_users %s", player_name);
	freeReplyObject(reply);
}

int redis_get_count_users_online()
{
	redisReply *reply;
	int online_users;

	reply = redisCommand(g_redis_context, "SCARD online_users");
	printf("GET online_users: %s\n", reply->str);
	online_users = reply->integer;
	freeReplyObject(reply);

	return online_users;
}

void send_client_message_to_all(int32_t color, char *msg)
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
	int i;
	int ret;
	int count_skins;

	ret = 0;
	count_skins = sizeof(g_skins_citizen) / sizeof(g_skins_citizen[0]);

	for (i = 0; i < count_skins; i++) {
		if (skin_id == g_skins_citizen[i]) {
			ret = 1;
			break;
		}
	}

	return ret;
}

void init_pickups()
{
	int32_t world;
	int32_t quantity;
	int32_t alpha;
	uint8_t is_automatic;

	world = 0;
	quantity = 100;
	alpha = 255;
	is_automatic = 1;

	g_plugin_funcs->CreatePickup(368, world, quantity, 
		-855.5118, -631.2861, 11.3756, alpha, is_automatic);
	g_plugin_funcs->CreatePickup(368, world, quantity, 
		407.7466, -483.1101, 12.3432, alpha, is_automatic);
	g_plugin_funcs->CreatePickup(288, world, quantity, 
		311.0720, -238.6959, 38.3752, alpha, is_automatic);
	g_plugin_funcs->CreatePickup(368, world, quantity, 
		266.4264, -249.8750, 36.1315, alpha, is_automatic);
	g_plugin_funcs->CreatePickup(366, world, quantity, 
		373.7430, -255.7513, 46.0797, alpha, is_automatic);
	g_plugin_funcs->CreatePickup(368, world, quantity, 
		341.0047, -230.0483, 38.3752, alpha, is_automatic);
	g_plugin_funcs->CreatePickup(288, world, quantity, 
		-990.6908, 263.9287, 8.8123, alpha, is_automatic);
	g_plugin_funcs->CreatePickup(278, world, quantity, 
		-988.6908, 263.9287, 8.8123, alpha, is_automatic);
	g_plugin_funcs->CreatePickup(280, world, quantity, 
		-986.6908, 263.9287, 8.8123, alpha, is_automatic);
	g_plugin_funcs->CreatePickup(368, world, quantity, 
		-1034.1990, 41.1718, 11.3544, alpha, is_automatic);
	g_plugin_funcs->CreatePickup(368, world, quantity, 
		-1472.9331, -866.6178, 20.8979, alpha, is_automatic);
	g_plugin_funcs->CreatePickup(277, world, quantity, 
		-672.05, 749.44, 10.90, alpha, is_automatic);
	g_plugin_funcs->CreatePickup(368, world, quantity,
		-671.05, 749.44, 10.90, alpha, is_automatic);
	g_plugin_funcs->CreatePickup(278, world, quantity, 
		-670.05, 749.44, 10.90, alpha, is_automatic);
	g_plugin_funcs->CreatePickup(366, world, quantity, 
		-113.2, -975.7, 10.4, alpha, is_automatic);
	g_plugin_funcs->CreatePickup(366, world, quantity, 
		-225.1, -1158.1, 9.1, alpha, is_automatic); 
	g_plugin_funcs->CreatePickup(366, world, quantity, 
		456.2, -471.4, 16.6, alpha, is_automatic); 
	g_plugin_funcs->CreatePickup(366, world, quantity, 
		377.4, 467.7, 11.6, alpha, is_automatic); 
	g_plugin_funcs->CreatePickup(366, world, quantity, 
		469.6, 697.4, 11.7, alpha, is_automatic); 
	g_plugin_funcs->CreatePickup(366, world, quantity, 
		385.3, 1210.9, 19.4, alpha, is_automatic); 
	g_plugin_funcs->CreatePickup(366, world, quantity, 
		384.3, 756.6, 11.7, alpha, is_automatic); 
	g_plugin_funcs->CreatePickup(366, world, quantity, 
		10.7, 1099.0, 16.6, alpha, is_automatic); 
	g_plugin_funcs->CreatePickup(366, world, quantity, 
		85.3, 402.7, 19.8, alpha, is_automatic); 
	g_plugin_funcs->CreatePickup(366, world, quantity, 
		-711.7, -501.7, 11.3, alpha, is_automatic); 
	g_plugin_funcs->CreatePickup(366, world, quantity, 
		-404, -588.0, 11.6, alpha, is_automatic); 
	g_plugin_funcs->CreatePickup(366, world, quantity, 
		-406.2503, -566.4947, 19.5804, alpha, is_automatic);
	g_plugin_funcs->CreatePickup(366, world, quantity, 
		-478.1,  1438.5, 16.1, alpha, is_automatic);
	g_plugin_funcs->CreatePickup(366, world, quantity, 
		-67,  1263.3, 25.1, alpha, is_automatic);
	g_plugin_funcs->CreatePickup(366, world, quantity, 
		-821.8, 1144.8, 26.1, alpha, is_automatic);
	g_plugin_funcs->CreatePickup(366, world, quantity, 
		-1139.4, 55.4, 11.2, alpha, is_automatic);
	g_plugin_funcs->CreatePickup(366, world, quantity, 
		-1290.9, 91.9, 26.9, alpha, is_automatic);
	g_plugin_funcs->CreatePickup(366, world, quantity, 
		-885.4, -483.4,  13.1, alpha, is_automatic);
	g_plugin_funcs->CreatePickup(366, world, quantity, 
		-925.1, -638.3, 16.0, alpha, is_automatic);
	g_plugin_funcs->CreatePickup(366, world, quantity,
		-692.4, -1283.8, 11.1, alpha, is_automatic);
	g_plugin_funcs->CreatePickup(366, world, quantity, 
		-655.1, -1506.3, 8.1, alpha, is_automatic);
	g_plugin_funcs->CreatePickup(366, world, quantity, 
		-1399.4, -865.9, 20.9, alpha, is_automatic);
	g_plugin_funcs->CreatePickup(366, world, quantity, 
		-822.6, 1137.3, 12.4, alpha, is_automatic);
	g_plugin_funcs->CreatePickup(366, world, quantity, 
		-851.4, -78.8, 11.5, alpha, is_automatic);
	g_plugin_funcs->CreatePickup(366, world, quantity, 
		-834.2, 740.6, 11.3, alpha, is_automatic);
}

void init_vehicles()
{
	int32_t world;
	world = 0;

	g_plugin_funcs->CreateVehicle(191, world, 
		338.6310, -237.6571, 29.1708, 98.9897, 0, 0);
	g_plugin_funcs->CreateVehicle(191, world, 
		321.2141, -223.5259, 35.4006, 239.5351, 0, 0);
	g_plugin_funcs->CreateVehicle(191, world, 
		323.4258, -278.1154, 35.4035, 5.3742, 0, 0);
	g_plugin_funcs->CreateVehicle(191, world, 
		-542.1738, 792.2469, 97.0344, 336.3952, 0, 0);
	g_plugin_funcs->CreateVehicle(191, world, 
		-1186.4758, 76.4269, 10.6504, 52.8653, 0, 0);
	g_plugin_funcs->CreateVehicle(191, world, 
		101.3112, -1472.5735, 9.9521, 275.9432, 0, 0);
	g_plugin_funcs->CreateVehicle(191, world, 
		100.1139, -1474.9865, 9.9556, 243.1996, 0, 0);
	g_plugin_funcs->CreateVehicle(191, world, 
		97.7001, -1475.9623, 9.9563, 173.3619, 0, 0);
	g_plugin_funcs->CreateVehicle(132, world, 
		91.2412, 242.9259, 21.4487, 127.1787, 6, 6);
	g_plugin_funcs->CreateVehicle(216, world, 
		-1009.4338, 186.8296, 11.3937, 351.5172, 6, 76);
	g_plugin_funcs->CreateVehicle(216, world, 
		-994.8803, 193.2692, 11.4204, 76.9205, 6, 76);
	g_plugin_funcs->CreateVehicle(130, world,
	 	-598.1232, 622.5588, 11.7840, 91.9153, 18, 74);
	g_plugin_funcs->CreateVehicle(223, world,
		-631.2451, -1456.9464, 5.8690, 335.9804, 36, 13);
	g_plugin_funcs->CreateVehicle(223, world,
		-624.0446, -1459.0283, 5.8552, 314.2722, 36, 13);
	g_plugin_funcs->CreateVehicle(166, world,
		-616.4828, 651.4387, 10.5640, 348.6626, 14, 75);
	g_plugin_funcs->CreateVehicle(166, world,
		-611.2023, 651.9318, 10.5676, 333.8794, 14, 75);
	g_plugin_funcs->CreateVehicle(166, world,
		-605.1564, 652.4683, 10.5843, 339.8292, 14, 75);
	g_plugin_funcs->CreateVehicle(166, world,
		-587.9245, 654.5039, 10.6044, 336.7441, 14, 75);
	g_plugin_funcs->CreateVehicle(166, world,
		-582.4528, 655.5148, 10.5830, 327.5977, 14, 75);
	g_plugin_funcs->CreateVehicle(193, world,
		-577.7695, 650.5206, 10.5597, 12.5459, 84, 84);
	g_plugin_funcs->CreateVehicle(193, world,
		-596.3391, 675.9069, 10.7617, 142.2299, 7, 7);
	g_plugin_funcs->CreateVehicle(156, world,
		-665.3930, 805.4311, 11.0370, 179.7041, 46, 1);
	g_plugin_funcs->CreateVehicle(156, world,
		-665.3428, 783.4216, 11.0371, 180.0831, 46, 1);
	g_plugin_funcs->CreateVehicle(156, world,
		-650.4547, 754.2264, 11.2033, 266.7828, 46, 1);
	g_plugin_funcs->CreateVehicle(156, world,
		-639.0520, 753.5862, 11.2032, 266.7841, 46, 1);
	g_plugin_funcs->CreateVehicle(156, world,
		-600.3902, 807.4341, 11.2114, 263.8463, 46, 1);
	g_plugin_funcs->CreateVehicle(227, world,
		-614.1934, 803.9073, 29.6660, 1.2667, 46, 1);
	g_plugin_funcs->CreateVehicle(210, world,
		-784.7332, 672.1744, 10.8495, 89.7786, 12, 12);
	g_plugin_funcs->CreateVehicle(135, world,
		-768.0613, 665.6818, 10.9137, 91.5195, 2, 0);
	g_plugin_funcs->CreateVehicle(210, world,
		-774.2057, 995.6342, 10.8461, 271.1632, 12, 12);
	g_plugin_funcs->CreateVehicle(159, world,
		-752.8378, 1003.8906, 10.8649, 359.9301, 3, 3);
	g_plugin_funcs->CreateVehicle(174, world,
		-780.7037, 1026.5653, 10.9394, 88.3382, 61, 0);
	g_plugin_funcs->CreateVehicle(130, world,
		-650.2558, 920.8828, 11.1857, 91.0021, 18, 74);
	g_plugin_funcs->CreateVehicle(137, world,
		-694.1721, 926.0735, 11.1955, 268.8809, 3, 1);
	g_plugin_funcs->CreateVehicle(137, world,
		-751.7783, 927.2592, 11.2812, 56.6550, 3, 1);
	g_plugin_funcs->CreateVehicle(145, world,
		-774.4608, 977.6279, 10.8241, 180.5240, 36, 36);
	g_plugin_funcs->CreateVehicle(192, world,
		-803.5596, 959.3724, 10.6343, 271.9642, 68, 78);
	g_plugin_funcs->CreateVehicle(192, world,
		-715.9547, 1065.1605, 10.5446, 242.0198, 68, 78);
	g_plugin_funcs->CreateVehicle(191, world,
		-496.2343, 1208.0430, 7.0726, 151.6175, 22, 22);
	g_plugin_funcs->CreateVehicle(191, world,
		-521.5993, 838.3251, 11.1270, 326.8671, 3, 3);
	g_plugin_funcs->CreateVehicle(132, world,
		-392.5302, 894.8767, 10.6133, 339.7856, 6, 6);
	g_plugin_funcs->CreateVehicle(132, world,
		-528.6176, 1203.7877, 7.2948, 330.5338, 10, 10);
	g_plugin_funcs->CreateVehicle(191, world,
		-447.5215, 1203.1743, 9.2052, 270.8528, 2, 2);
	g_plugin_funcs->CreateVehicle(174, world,
		-448.7614, 1185.0378, 9.5395, 93.0431, 41, 0);
	g_plugin_funcs->CreateVehicle(149, world,
		-594.9833, 1346.0950, 11.5229, 114.8051, 47, 76);
	g_plugin_funcs->CreateVehicle(191, world,
		-822.9012, 1310.8057, 11.1059, 159.9270, 2, 2);
	g_plugin_funcs->CreateVehicle(234, world,
		-1476.8434, 1047.7759, 264.1451, 217.4100, -1, -1);
	g_plugin_funcs->CreateVehicle(234, world,
		-1347.2435, 1055.6526, 264.0510, 156.2663, -1, -1);
	g_plugin_funcs->CreateVehicle(234, world,
		-1279.8364, 993.2283, 262.8954, 90.2378, -1, -1);
	g_plugin_funcs->CreateVehicle(234, world,
		-1351.7225, 934.1428, 262.0353, 14.6320, -1, -1);
	g_plugin_funcs->CreateVehicle(234, world,
		-1515.2861, 983.0940, 263.1325, 282.8845, -1, -1);
	g_plugin_funcs->CreateVehicle(198, world,
		-1346.5970, 1448.2627, 299.1477, 321.8539, 46, 46);
	g_plugin_funcs->CreateVehicle(198, world,
		-1311.7345, 1446.4861, 298.8468, 49.8741, 46, 46);
	g_plugin_funcs->CreateVehicle(198, world,
		-1323.5178, 1439.1461, 298.8476, 9.8079, 53, 53);
	g_plugin_funcs->CreateVehicle(198, world,
		-1296.2511, 1458.1401, 298.8468, 77.2460, 53, 53);
	g_plugin_funcs->CreateVehicle(198, world,
		-1421.0701, 1488.8687, 302.1499, 73.6359, 53, 53);
	g_plugin_funcs->CreateVehicle(217, world,
		-1127.0664, 1511.9712, 11.9682, 357.7274, 1, 42);
	g_plugin_funcs->CreateVehicle(173, world,
		-985.4117, 1344.1613, 11.7257, 47.3206, 93, 77);
	g_plugin_funcs->CreateVehicle(201, world,
		-918.6592, 1130.1572, 10.9791, 129.1441, 13, 13);
	g_plugin_funcs->CreateVehicle(146, world,
		-782.4065, 1116.9678, 10.0661, 0.3381, 1, 3);
	g_plugin_funcs->CreateVehicle(146, world,
		-771.0571, 1154.9764, 12.6235, 180.7035, 1, 3);
	g_plugin_funcs->CreateVehicle(132, world,
		-756.0136, 1074.7358, 9.2508, 91.9495, 35, 35);
	g_plugin_funcs->CreateVehicle(191, world,
		96.4878, 1090.1553, 16.0475, 211.5273, 48, 48);
	g_plugin_funcs->CreateVehicle(179, world,
		48.7824, 1100.7084, 17.2732, 182.7325, 16, 74);
	g_plugin_funcs->CreateVehicle(179, world,
		-4.4181, 1147.7140, 19.4285, 205.1626, 16, 74);
	g_plugin_funcs->CreateVehicle(179, world,
		18.5310, 1140.2460, 19.6552, 124.7352, 16, 74);
	g_plugin_funcs->CreateVehicle(190, world,
		-142.0661, 1022.3298, 7.5074, 8.5241, 1, 90);
	g_plugin_funcs->CreateVehicle(205, world,
		50.4374, 1174.9293, 20.6586, 173.0398, 41, 29);
	g_plugin_funcs->CreateVehicle(205, world,
		334.1074, 1201.4971, 17.1962, 268.0051, 33, 0);
	g_plugin_funcs->CreateVehicle(204, world,
		395.3799, 989.4377, 12.1038, 245.0027, 17, 17);
	g_plugin_funcs->CreateVehicle(132, world,
		287.0439, 1037.1035, 13.2244, 11.1845, 33, 33);
	g_plugin_funcs->CreateVehicle(230, world,
		208.6151, 1229.4574, 17.4755, 295.1513, 48, 65);
	g_plugin_funcs->CreateVehicle(230, world,
		-1106.8494, 292.9714, 12.3273, 271.9934, 48, 65);
	g_plugin_funcs->CreateVehicle(216, world,
		-1002.6074, 206.0241, 11.4072, 174.9314, 6, 76);
	g_plugin_funcs->CreateVehicle(138, world,
		-1261.3719, 115.7242, 12.0178, 172.4562, 1, 75);
	g_plugin_funcs->CreateVehicle(205, world,
		-1107.2173, -198.0835, 11.1751, 90.1365, 9, 39);
	g_plugin_funcs->CreateVehicle(158, world,
		-920.4551, -306.0597, 13.5878, 264.1842, 4, 75);
	g_plugin_funcs->CreateVehicle(206, world,
		-951.9403, -378.0401, 10.9365, 276.5116, 61, 39);
	g_plugin_funcs->CreateVehicle(206, world,
		-891.4566, -700.5800, 10.9547, 37.6183, 61, 39);
	g_plugin_funcs->CreateVehicle(236, world,
		-863.2736, -666.1580, 11.0304, 186.4959, 3, 3);
	g_plugin_funcs->CreateVehicle(236, world,
		-855.5962, -666.0559, 10.9932, 183.2121, 7, 7);
	g_plugin_funcs->CreateVehicle(236, world,
		-851.5004, -665.6548, 10.9740, 185.7453, 76, 76);
	g_plugin_funcs->CreateVehicle(236, world,
		-845.2188, -675.1938, 10.9438, 97.3510, 6, 6);
	g_plugin_funcs->CreateVehicle(236, world,
		-844.9042, -679.4929, 10.9420, 98.8294, 52, 52);
	g_plugin_funcs->CreateVehicle(175, world,
		-846.8568, -909.4408, 10.9547, 319.2956, 42, 42);
	g_plugin_funcs->CreateVehicle(141, world,
		-973.8217, -831.9473, 6.4925, 91.7754, 11, 11);
	g_plugin_funcs->CreateVehicle(132, world,
		-1017.5047, -861.6166, 12.8358, 213.0164, 6, 6);
	g_plugin_funcs->CreateVehicle(191, world,
		-1015.7603, -859.2327, 17.4827, 191.6682, 22, 22);
	g_plugin_funcs->CreateVehicle(191, world,
		-1019.6179, -858.1592, 17.4810, 184.1755, 2, 2);
	g_plugin_funcs->CreateVehicle(175, world,
		-979.8837, -1169.0863, 14.7222, 92.2340, 53, 53);
	g_plugin_funcs->CreateVehicle(144, world,
		-1008.8375, -1407.3828, 11.8616, 252.0942, 3, 3);
	g_plugin_funcs->CreateVehicle(191, world,
		-697.8029, -1522.6481, 12.1532, 68.8400, 3, 3);
	g_plugin_funcs->CreateVehicle(226, world,
		-731.2047, -1503.1404, 11.2021, 265.5437, 12, 1);
	g_plugin_funcs->CreateVehicle(226, world,
		-720.9149, -1553.4470, 12.2698, 338.7068, 12, 1);
	g_plugin_funcs->CreateVehicle(226, world,
		-696.1869, -1502.0701, 11.7049, 357.9846, 12, 1);
	g_plugin_funcs->CreateVehicle(217, world,
		-686.5530, -1567.7709, 12.5295, 247.8371, 1, 57);
	g_plugin_funcs->CreateVehicle(223, world,
		-588.4979, -1511.8752, 5.8423, 252.7852, 36, 13);
	g_plugin_funcs->CreateVehicle(223, world,
		-583.6869, -1502.5627, 5.7980, 240.5196, 36, 13);
	g_plugin_funcs->CreateVehicle(136, world,
		-389.6123, -1726.2853, 6.7915, 3.0873, 1, 1);
	g_plugin_funcs->CreateVehicle(136, world,
		-397.6457, -1342.7911, 6.6525, 354.6517, 1, 1);
	g_plugin_funcs->CreateVehicle(223, world,
		-374.6611, -659.9583, 5.6830, 91.7661, 36, 13);
	g_plugin_funcs->CreateVehicle(214, world,
		-650.9855, -263.8465, 6.7326, 306.9479, 50, 32);
	g_plugin_funcs->CreateVehicle(136, world,
		-512.7714, -226.5067, 6.7797, 356.7461, 1, 1);
	g_plugin_funcs->CreateVehicle(136, world,
		-535.4413, -227.1720, 6.6767, 337.7500, 1, 1);
	g_plugin_funcs->CreateVehicle(190, world,
		-372.2652, -216.4774, 7.3608, 295.0877, 1, 35);
	g_plugin_funcs->CreateVehicle(190, world,
		604.7895, -1706.5129, 7.5829, 332.8387, 1, 60);
	g_plugin_funcs->CreateVehicle(190, world,
		583.6797, -1760.7828, 7.2956, 313.2621, 1, 46);
	g_plugin_funcs->CreateVehicle(190, world,
		603.4528, -1774.8110, 7.4699, 205.4766, 1, 14);
	g_plugin_funcs->CreateVehicle(190, world,
		-1259.0291, -1414.6964, 7.4179, 60.5551, 1, 50);
	g_plugin_funcs->CreateVehicle(160, world,
		-1199.0627, -1382.2418, 5.6341, 152.5868, 46, 1);
	g_plugin_funcs->CreateVehicle(160, world, 
		-1213.4904, -1382.6627, 5.5850, 151.3750, 46, 1);
	g_plugin_funcs->CreateVehicle(132, world, 
		-651.9755, -513.1553, 10.1559, 111.1670, 6, 6);
	g_plugin_funcs->CreateVehicle(132, world, 
		-355.9834, -530.7414, 12.5118, 1.3028, 61, 0);
	g_plugin_funcs->CreateVehicle(132, world,
		-362.5143, -528.3513, 12.5124, 2.4941, 61, 0);
	g_plugin_funcs->CreateVehicle(174, world, 
		-393.3240, -523.2957, 12.6169, 356.3634, 61, 0);
	g_plugin_funcs->CreateVehicle(174, world, 
		-398.3515, -520.8174, 12.6268, 359.5150, 61, 0);
	g_plugin_funcs->CreateVehicle(174, world, 
		-406.8978, -520.3749, 12.6289, 3.5854, 61, 0);
	g_plugin_funcs->CreateVehicle(217, world, 
		-391.8160, -573.8380, 40.0296, 270.2119, 61, 0);
	g_plugin_funcs->CreateVehicle(156, world, 
		369.4720, -523.6241, 12.1027, 319.9711, 46, 1);
	g_plugin_funcs->CreateVehicle(156, world, 
		352.9126, -509.6271, 12.0988, 320.7192, 46, 1);
	g_plugin_funcs->CreateVehicle(198, world, 
		167.0211, -1504.3243, 10.6981, 195.1150, 46, 46);
	g_plugin_funcs->CreateVehicle(198, world, 
		161.7691, -1505.6873, 10.6334, 260.1471, 53, 53);
	g_plugin_funcs->CreateVehicle(198, world,
		150.8547, -1525.2775, 10.5850, 209.5386, 3, 3);
	g_plugin_funcs->CreateVehicle(217, world, 
		-71.3746, -1606.8180, 12.1979, 268.9019, 1, 42);
	g_plugin_funcs->CreateVehicle(191, world, 
		-242.6452, -1347.8843, 7.6379, 287.6805, 2, 2);
	g_plugin_funcs->CreateVehicle(193, world, 
		-254.7485, -1231.7388, 7.6065, 82.8703, 84, 84);
	g_plugin_funcs->CreateVehicle(210, world, 
		-150.9673, -1427.3008, 3.7353, 287.1888, 12, 12);
	g_plugin_funcs->CreateVehicle(193, world, 
		-159.8616, -1352.5422, 9.9567, 88.3373, 84, 84);
	g_plugin_funcs->CreateVehicle(209, world, 
		-107.2975, -1204.9932, 10.2545, 314.1096, 15, 32);
	g_plugin_funcs->CreateVehicle(167, world, 
		-5.2635, -1231.6711, 10.5564, 3.0305, 75, 79);
	g_plugin_funcs->CreateVehicle(132, world, 
		139.5816, -1104.4471, 10.1935, 87.3026, 10, 10);
	g_plugin_funcs->CreateVehicle(191, world, 
		125.3515, -1104.8690, 9.9718, 178.6016, 3, 3);
	g_plugin_funcs->CreateVehicle(209, world, 
		55.1483, -1077.7474, 10.2544, 179.0974, 15, 32);
	g_plugin_funcs->CreateVehicle(191, world, 
		-53.6150, -998.6633, 9.9737, 92.8012, 48, 48);
	g_plugin_funcs->CreateVehicle(198, world, 
		-38.4061, -1015.3550, 10.0968, 359.9808, 6, 6);
	g_plugin_funcs->CreateVehicle(205, world, 
		-9.5104, -992.9700, 10.1978, 354.5850, 17, 1);
	g_plugin_funcs->CreateVehicle(230, world, 
		37.5689, -1003.9778, 10.5291, 271.4819, 48, 65);
	g_plugin_funcs->CreateVehicle(191, world, 
		-7.6970, -951.4760, 21.2936, 2.1346, 51, 51);
	g_plugin_funcs->CreateVehicle(193, world, 
		-8.3562, -927.7123, 21.2671, 181.7117, 7, 7);
	g_plugin_funcs->CreateVehicle(146, world, 
		-118.7073, -922.1487, 10.6754, 102.6320, 1, 3);
	g_plugin_funcs->CreateVehicle(205, world, 
		273.2764, -873.0959, 9.9347, 62.8151, 21, 1);
	g_plugin_funcs->CreateVehicle(132, world, 
		532.1689, -156.9969, 13.3790, 97.7350, 33, 33);
	g_plugin_funcs->CreateVehicle(145, world, 
		475.0922, -42.7295, 9.8901, 357.9021, 36, 36);
	g_plugin_funcs->CreateVehicle(191, world, 
		455.0826, -3.7197, 10.4825, 81.7555, 13, 13);
	g_plugin_funcs->CreateVehicle(174, world, 
		525.0902, 192.1835, 14.3481, 179.2360, 11, 0);
	g_plugin_funcs->CreateVehicle(174, world, 
		297.6835, 460.3541, 9.9776, 249.5900, 41, 0);
	g_plugin_funcs->CreateVehicle(136, world, 
		329.6919, 578.8196, 6.6500, 0.0177, 1, 1);
	g_plugin_funcs->CreateVehicle(132, world, 
		437.4555, 539.0988, 11.3912, 347.3801, 8, 8);
	g_plugin_funcs->CreateVehicle(146, world, 
		456.3506, 718.7170, 11.6164, 266.7214, 1, 3);
	g_plugin_funcs->CreateVehicle(156, world, 
		494.0573, 503.6129, 11.2556, 182.5625, 46, 1);
	g_plugin_funcs->CreateVehicle(156, world, 
		520.5809, 502.5757, 10.8520, 176.7665, 46, 1);
	g_plugin_funcs->CreateVehicle(156, world, 
		490.3658, 521.4353, 11.3487, 89.2153, 46, 1);
	g_plugin_funcs->CreateVehicle(132, world, 
		144.5919, -1148.9771, 17.5309, 179.2223, 6, 6);
	g_plugin_funcs->CreateVehicle(159, world, 
		144.2603, -1230.6996, 24.3226, 0.7343, 3, 3);
	g_plugin_funcs->CreateVehicle(205, world, 
		128.0492, -1177.0865, 31.0443, 2.2424, 33, 0);
	g_plugin_funcs->CreateVehicle(191, world, 
		110.0135, -1148.5236, 30.8348, 359.9130, 3, 3);
	g_plugin_funcs->CreateVehicle(191, world, 
		128.8416, -1216.1100, 30.8201, 359.1587, 48, 48);
	g_plugin_funcs->CreateVehicle(191, world, 
		-1151.8125, -945.6403, 14.3883, 272.7182, 38, 38);
	g_plugin_funcs->CreateVehicle(205, world, 
		-1132.1305, -926.5008, 14.6029, 270.2141, 37, 0);
	g_plugin_funcs->CreateVehicle(132, world, 
		-1142.1732, -977.6573, 14.6181, 91.6233, 10, 10);
	g_plugin_funcs->CreateVehicle(193, world, 
		-1142.2985, -1027.4420, 14.3610, 85.6886, 84, 84);
	g_plugin_funcs->CreateVehicle(145, world, 
		-1152.0118, -1014.8665, 14.6410, 269.6242, 36, 36);
	g_plugin_funcs->CreateVehicle(218, world, 
		-1369.5146, -1255.6322, 18.1927, 36.6048, 1, 2);
	g_plugin_funcs->CreateVehicle(145, world, 
		-1681.8029, -645.2209, 14.6583, 359.9387, 52, 52);
	g_plugin_funcs->CreateVehicle(191, world, 
		-1627.2859, -623.5496, 14.4104, 171.6752, 2, 2);
	g_plugin_funcs->CreateVehicle(191, world, 
		-1579.2931, -613.0315, 14.3933, 359.9059, 3, 3);
	g_plugin_funcs->CreateVehicle(200, world, 
		-1747.2423, -269.6884, 15.1117, 267.0928, 43, 0);
	g_plugin_funcs->CreateVehicle(200, world, 
		-1746.7600, -225.4840, 15.1116, 267.6391, 43, 0);
	g_plugin_funcs->CreateVehicle(163, world, 
		-1745.6432, -212.0730, 15.2650, 269.7901, 43, 0);
	g_plugin_funcs->CreateVehicle(163, world, 
		-1704.9772, -215.9604, 15.2650, 179.8797, 43, 0);
	g_plugin_funcs->CreateVehicle(155, world, 
		-1661.2036, -225.9702, 14.8505, 83.3181, 40, 44);
	g_plugin_funcs->CreateVehicle(153, world, 
		-874.2407, -565.3093, 11.1761, 186.5672, 1, 56);
	g_plugin_funcs->CreateVehicle(147, world, 
		-758.9777, 821.3871, 10.9881, 180.4261, 71, 73);
	g_plugin_funcs->CreateVehicle(147, world, 
		-800.3445, 820.1251, 10.9879, 358.3366, 71, 73);
	g_plugin_funcs->CreateVehicle(220, world, 
		-829.3396, 836.5057, 11.1657, 269.2282, 6, 6);
	g_plugin_funcs->CreateVehicle(223, world, 
		-605.8749, -1466.1476, 5.9455, 313.0891, 36, 13);
	g_plugin_funcs->CreateVehicle(223, world, 
		-600.9694, -1468.3768, 5.9371, 317.1415, 36, 13);
}

void init_skins()
{
	for (int i = 0; i < sizeof(g_skins_citizen) / sizeof(g_skins_citizen[0]); i++) {
		g_plugin_funcs->AddPlayerClass(i, COLOR_WHITE, g_skins_citizen[i],
			335.674164, -242.812607, 29.646561, 1.575135,
			2, 1, 17, 400, 21, 100);
	}

	for (int i = 0; i < sizeof(g_skins_police) / sizeof(g_skins_police[0]); i++) {
		g_plugin_funcs->AddPlayerClass(i, COLOR_BLUE, g_skins_police[i],
			508.949005, 510.883820, 12.106688, 3.138477,
			4, 1, 17, 400, 19, 100);
	}
}

int32_t get_weapon_id_from_string(const char *weapon_name)
{
	int i;
	int count_weapons;
	int weapon_id;

	weapon_id = -1;
	count_weapons = sizeof(g_weapons) / sizeof(g_weapons[0]);

	for (i = 0; i < count_weapons; i++) {
		if (strncmp(weapon_name, g_weapons[i], 2) == 0) {
			weapon_id = i;
			break;
		}
	}

	/* replace rocket weapon to rocker launcher weapon */
	if (weapon_id == 16) {
		weapon_id = 30;
	}

	return weapon_id;
}

char *get_weapon_name_from_id(int32_t weapon_id)
{
	return g_weapons[weapon_id];
}

int32_t get_command_param(char *msg, char *param)
{
	char *pch;
	char delim[] = " ";

	int32_t ret;
	ret = -1;

	pch = strtok(msg, delim);

	while (pch != NULL) {
		if (strcmp(pch, param) == 0) 
			ret = 1;

		pch = strtok(NULL, delim);
	}

	return ret;
}

int32_t find_weapon_id_from_string(char *message)
{
	int32_t weapon_id;
	char *pch;
	char delim[] = " ";

	pch = strtok(message, delim);
	weapon_id = -1;

	while (pch != NULL) {
		weapon_id = get_weapon_id_from_string(pch);
		pch = strtok(NULL, delim);
	}

	return weapon_id;
}

void init_server()
{
	g_plugin_funcs->SetServerName(SERVER_NAME);
	g_plugin_funcs->SetGameModeText(SERVER_GAMEMODE);

	g_plugin_funcs->SetSpawnPlayerPosition(561.666809, 25.169380, 17.751160);
	g_plugin_funcs->SetSpawnCameraPosition(564.672852, 25.190231, 17.751160);
	g_plugin_funcs->SetSpawnCameraLookAt(561.666809, 25.169380, 17.751160);

	g_plugin_funcs->SetTimeRate(1000);
	g_plugin_funcs->SetHour(12);
	g_plugin_funcs->SetMinute(00);

	g_plugin_funcs->SetServerOption(vcmpServerOptionDeathMessages, 0);
	g_plugin_funcs->SetServerOption(vcmpServerOptionJoinMessages, 0);
	g_plugin_funcs->SetServerOption(vcmpServerOptionFriendlyFire, 0);
}

uint8_t on_server_init()
{
	int err;

	err = redis_init();
	if (err) {
		return 0;
	}

	redis_flush();

	init_server();
	init_pickups();
	init_skins();
	init_vehicles();

	return 1;
}

void on_server_shutdown()
{
	redis_deinit();
}

uint8_t on_player_request_class(int32_t player_id, int32_t offset)
{
	int32_t skin_id;
	skin_id = g_plugin_funcs->GetPlayerSkin(player_id);

	g_plugin_funcs->SetPlayerHeading(player_id, -1.557110);

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

	if (strcmp(message, "help") == 0) {
		g_plugin_funcs->SendClientMessage(player_id, COLOR_GREY,
			"** pm >> Commands: /dm /heal /armour /we /online");
		return 1;
	}
	else if (strcmp(message, "save") == 0) {
		float x, y, z, angle;

		g_plugin_funcs->GetPlayerPosition(player_id, &x, &y, &z);
		angle = g_plugin_funcs->GetPlayerHeading(player_id);

		g_plugin_funcs->SendClientMessage(player_id, COLOR_GREY,
			"%f, %f, %f, %f", x, y, z, angle);

		return 1;
	}
	else if (strncmp(message, "spawn", strlen("spawn")) == 0) {
		char msg[256];

		if (get_command_param(message, "6410") != -1) {
			float x, y, z;
			float angle;
			float world;
			float model;

			int32_t color1;
			int32_t color2;

			angle = 0;
			world = 0;
			model = 6410;

			srand(time(NULL));

			color1 = rand() % 108; /* random int between 0 and 108 */
			color2 = rand() % 108; /* random int between 0 and 108 */

			g_plugin_funcs->GetPlayerPosition(player_id, &x, &y, &z);
			g_plugin_funcs->CreateVehicle(model, world, x, y, z, angle, color1,
				color2);

			sprintf(msg, ">> %s spawned vehicle (/spawn)", player_name);
			send_client_message_to_all(COLOR_GREY, msg);
		} else {
			g_plugin_funcs->SendClientMessage(player_id, COLOR_RED,
				"** pm >> Cannot find vehicle");
		}

		return 1;
	}
	else if (strcmp(message, "dm") == 0) {
		char msg[256];

		g_plugin_funcs->SetPlayerPosition(player_id,
			-543.463013, 797.462585, 195.213089);
		g_plugin_funcs->SetPlayerHeading(player_id, -1.765160);

		sprintf(msg, ">> %s teleported to DM (/dm)", player_name);
		send_client_message_to_all(COLOR_GREY, msg);

		return 1;
	}
	else if (strcmp(message, "repair") == 0) {
		char msg[256];

		float health;
		int32_t vehicle_id;

		health = 1000.0;
		vehicle_id = g_plugin_funcs->GetPlayerVehicleId(player_id);

		if (g_plugin_funcs->GetLastError() == vcmpErrorNoSuchEntity) {
			g_plugin_funcs->SendClientMessage(player_id, COLOR_RED, 
				"** pm >> You need to be in vehicle");
		}

		g_plugin_funcs->SetVehicleHealth(vehicle_id, health);
		g_plugin_funcs->SetVehicleDamageData(vehicle_id, 0);

		sprintf(msg, ">> %s fixed a vehicle (/repair)", player_name);
		send_client_message_to_all(COLOR_GREY, msg);

		return 1;
	}
	else if (strcmp(message, "heal") == 0) {
		char msg[256];
		float max_health;

		max_health = 100.0f;

		g_plugin_funcs->SetPlayerHealth(player_id, max_health);

		sprintf(msg, ">> %s bought health (/heal)", player_name);
		send_client_message_to_all(COLOR_GREY, msg);

		return 1;
	} 
	else if (strncmp(message, "we", strlen("we")) == 0) {
		int32_t weapon_id;
		weapon_id = find_weapon_id_from_string(message);

		if (weapon_id == -1) {
			g_plugin_funcs->SendClientMessage(player_id, COLOR_RED,
				"** pm >> Unknown weapon name. Example: /we colt");
		} else {
			char *weapon_name;
			char msg[256];
			int32_t ammo;

			ammo = 300;

			g_plugin_funcs->GivePlayerWeapon(player_id, weapon_id, ammo);			
			weapon_name = get_weapon_name_from_id(weapon_id);

			sprintf(msg, ">> %s bought weapon %s (/we)", player_name, weapon_name);
			send_client_message_to_all(COLOR_GREY, msg);
		}

		return 1;
	}
	else if (strcmp(message, "armour") == 0) {
		float max_armour;
		char msg[256];

		max_armour = 100.0f;

		g_plugin_funcs->SetPlayerArmour(player_id, max_armour);

		sprintf(msg, ">> %s bought armour (/armour)", player_name);
		send_client_message_to_all(COLOR_GREY, msg);

		return 1;
	}
	else if (strcmp(message, "online") == 0) {
		int users_online;

		users_online = redis_get_count_users_online();
        g_plugin_funcs->SendClientMessage(player_id, COLOR_GREY,
			"** pm >> Now online users: %d (/online)", users_online);

		return 1;
	}
	else {
		g_plugin_funcs->SendClientMessage(player_id, COLOR_RED,
			"** pm >> Unknown command");
	}

	return 0;
}

void on_player_spawn(int32_t player_id)
{
	char player_name[64];
	g_plugin_funcs->GetPlayerName(player_id, player_name, sizeof(player_name));

	g_plugin_funcs->SendClientMessage(player_id, COLOR_GREY, ">> %s spawned",
		player_name);
}

void on_player_connect(int32_t player_id)
{
	char player_name[64];
    g_plugin_funcs->GetPlayerName(player_id, player_name, sizeof(player_name));

	redis_set_user_online(player_name);

	char msg[256];
	sprintf(msg, ">> %s connected to server", player_name);
	send_client_message_to_all(COLOR_GREY, msg);

	g_plugin_funcs->SendClientMessage(player_id, COLOR_YELLOW,
		"** pm >> Welcome to %s. Enter /help for more information",
		SERVER_NAME);
}

void on_player_disconnect(int32_t player_id, vcmpDisconnectReason reason)
{
	char player_name[64];
    g_plugin_funcs->GetPlayerName(player_id, player_name, sizeof(player_name));

	redis_set_user_offline(player_name);
}

void on_player_death(int32_t player_id, int32_t killer_id, int32_t reason,
	vcmpBodyPart bodyPart)
{
	char msg[256];
	char player_name[64];

	g_plugin_funcs->GetPlayerName(player_id, player_name, sizeof(player_name));

	if (killer_id == UNKNOWN_KILLER_ID) {
		sprintf(msg, ">> %s died", player_name);
	} else {
		char killer_name[64];
		char *weapon_name;
		int32_t score;

		g_plugin_funcs->GetPlayerName(killer_id, killer_name, 
			sizeof(killer_name));
		weapon_name = get_weapon_name_from_id(reason);

		score = g_plugin_funcs->GetPlayerScore(killer_id);
		g_plugin_funcs->SetPlayerScore(killer_id, score + 1);

		sprintf(msg, ">> %s killed %s (%s)", killer_name, player_name,
			weapon_name);
	}

	send_client_message_to_all(COLOR_GREY, msg);
}

unsigned int VcmpPluginInit(PluginFuncs* plugin_funcs,
	PluginCallbacks* plugin_calls, PluginInfo* plugin_info)
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
	plugin_calls->OnPlayerDeath = on_player_death;

	return 1;
}
