#include "data.h"

const char* g_SampleSites[] = {
    "https://example.com",
    "https://www.wikipedia.org",
    "https://github.com",
    "calc.exe",
    "notepad.exe",
    "mspaint.exe"
};

const size_t g_NumSampleSites = sizeof(g_SampleSites) / sizeof(g_SampleSites[0]);

// Classic MEMZ popup texts adapted with similar-sounding replacements (no actual slurs/profanity)
// and non-destructive notes where original lines claimed permanent harm.
const wchar_t* g_PopupMessages[] = {
    L"Still using this computer?",
    L"YOU KILLED MY TROJAN!\r\nNow you are going to... wait, no, still safe.",
    L"REST IN PEACE, FOREVER MISS.",
    L"I WARNED YOU...",
    L"HAHA N00B L2P GET REKT",
    L"You failed at your 1337 h4x0r skillz.",
    L"YOU TRIED SO HARD AND GOT SO FAR, BUT IN THE END, YOUR PC WAS STILL DUCKED! (Just kidding, nothing was harmed!)",
    L"HACKER!\r\nENJOY BAN!",
    L"GET BETTER HAX NEXT TIME xD",
    L"HAVE FUN TRYING TO RESTORE YOUR DATA :D",
    L"|\\/|3|\\/|2",
    L"BSOD INCOMING",
    L"VIRUS PRANK (GONE WRONG)",
    L"ENJOY THE NYAN CAT",
    L"Get dank antivirus m9!",
    L"You are a silly goose!\r\nHA HA HA HA HA HA HA",
    L"#MakeMalwareGreatAgain",
    L"SOMEBODY ONCE TOLD ME THE MEMZ ARE GONNA ROLL ME",
    L"Why did you even tried to kill MEMZ?\r\nYour PC is ducked anyway. uh... not..",
    L"SecureBoot sucks.",
    L"gr8 m8 i r8 8/8",
    L"Have you tried turning it off and on again?",
    L"<Insert Joel quote here>",
    L"Greetings to all GAiA members!",
    L"Well, hello there. I don't believe we've been properly introduced. I'm Bonzi!",
    L"'This is everything I want in my computer'\r\n - danooct1 2016",
    L"'Uh, Club Penguin. Time to get banned!'\r\n - danooct1 2016"
};

const size_t g_NumPopupMessages = sizeof(g_PopupMessages) / sizeof(g_PopupMessages[0]);

const char* g_SoundAliases[] = {
    "SystemHand",
    "SystemQuestion",
    "SystemExclamation"
};

const size_t g_NumSoundAliases = sizeof(g_SoundAliases) / sizeof(g_SoundAliases[0]);
