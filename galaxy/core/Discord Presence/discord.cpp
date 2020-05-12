#include "discord.h"

discord_presence discord;

void discord_presence::start( )
{
	DiscordEventHandlers Handler;
	memset(&Handler, 0, sizeof(Handler));
	Discord_Initialize("701764758617063485", &Handler, 1, NULL);
}

void discord_presence::update( )
{
	DiscordRichPresence discordPresence;
	memset( &discordPresence, 0, sizeof( discordPresence ) );
	discordPresence.state = "Playing with galaxy.xyz!";
	discordPresence.details = "Tapping nonames!";
	discordPresence.startTimestamp = 0;
	discordPresence.largeImageKey = "1423";
	discordPresence.largeImageText = "Playing with galaxy.xyz!";
	discordPresence.smallImageKey = "1423";
	discordPresence.smallImageText = "Playing with galaxy.xyz!";
	Discord_UpdatePresence( &discordPresence );
}