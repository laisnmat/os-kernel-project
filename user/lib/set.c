#include "../../kernel/params.h"
#include "../../shared/string.h"
#include "../syscalls.h"
#include "lib.h"

typedef struct
{
    const char *name;        // Name of the command
    const char *description; // Description of the command
    const char *args;
} subcommand_t;

typedef enum {
    ENSIMAG_THEME,
    PHELMA_THEME,
    ENSE3_THEME,
    PAPET_THEME
} theme_t;

subcommand_t subcommands[] = {
    {"echo", "Active ou desactive l'affichage des entrees clavier", "[on|off]"},
    {"theme" , "Change le theme du shell", "[ensimag|phelma|ense3|papet]"},
};


static void set_theme(theme_t theme)
{
    switch (theme)
    {
        case ENSIMAG_THEME:
            primary_color = VERT_CLAIR + 10; // VERT_CLAIR
            secondary_color = CYAN_CLAIR + 10; // CYAN_CLAIR
            tertiary_color = VERT + 10; // VERT
            break;
        case PHELMA_THEME:
            primary_color = ROUGE_CLAIR + 10; // ROUGE_CLAIR
            secondary_color = JAUNE + 10; // JAUNE
            tertiary_color = ROUGE + 10; // ROUGE
            break;
        case ENSE3_THEME:
            primary_color = BLEU + 10; // BLEU_CLAIR
            secondary_color = BLEU_CLAIR + 10; // BLEU
            tertiary_color = CYAN + 10; // CYAN
            break;
        case PAPET_THEME:
            primary_color = MARRON + 10; // Black
            secondary_color = JAUNE + 10; // White
            tertiary_color = ROUGE + 10; // Bright Black
            break;
    }
}

static void help()
{
    __printf("\033[%dmUsage:\033[0m set \033[%dm <subcommand> [options] \033[0m\n\n",
             secondary_color, primary_color);
    // Options
    __printf("Options:\n");
    __printf("\t--help, -h\tAffiche l'aide pour cette commande.\n");
    __printf("\t--list, -l\tAffiche la liste des sous-commandes disponibles.\n");
}

void list_subcommands()
{
    __printf("\033[%dmAvailable subcommands:\033[0m\n", secondary_color);
    for (unsigned int i = 0; i < sizeof(subcommands) / sizeof(subcommand_t); i++)
    {
        __printf("\t\033[%dm%s\033[0m - %s\n", primary_color, subcommands[i].name,
                 subcommands[i].description);
    }
}

void help_subcommand(const char *subcommand)
{
    for (unsigned int i = 0; i < sizeof(subcommands) / sizeof(subcommand_t); i++)
    {
        if (strcmp(subcommands[i].name, subcommand) == 0)
        {
            __printf("\033[%dm%s\033[0m - %s\n", primary_color, subcommands[i].name,
                     subcommands[i].description);
            __printf("Usage: set %s %s\n", subcommands[i].name, subcommands[i].args);
            return;
        }
    }
    __printf("\033[22mUnknown subcommand: %s\033[0m\n", subcommand);
}

void __set(args_t *args)
{
    if (args->argc < 2)
    {
        __printf("\033[22mUsage: %s <subcommand> [options] (see --help for more details)\033[0m\n",
                 args->argv[0]);
        return;
    }

    if (strcmp(args->argv[1], "--help") == 0 || strcmp(args->argv[1], "-h") == 0)
    {
        help();
        return;
    }

    if (strcmp(args->argv[1], "--list") == 0 || strcmp(args->argv[1], "-l") == 0)
    {
        list_subcommands();
        return;
    }

    if (args->argc == 3 &&
        (strcmp(args->argv[2], "--help") == 0 || strcmp(args->argv[2], "-h") == 0))
    {
        help_subcommand(args->argv[1]);
        return;
    }

    const char *subcommand = args->argv[1];
    for (unsigned int i = 0; i < sizeof(subcommands) / sizeof(subcommand_t); i++)
    {
        if (strcmp(subcommands[i].name, subcommand) == 0)
        {
            // Found the subcommand, execute it
            if (strcmp(subcommand, "echo") == 0)
            {
                if (args->argc < 3)
                {
                    __printf("\033[22mUsage: set echo [on|off]\033[0m\n");
                    return;
                }
                if (strcmp(args->argv[2], "on") == 0)
                {
                    cons_echo(1);
                    __printf("\033[12mEcho is now ON.\033[0m\n");
                }
                else if (strcmp(args->argv[2], "off") == 0)
                {
                    cons_echo(0);
                    __printf("\033[12mEcho is now OFF.\033[0m\n");
                }
                else
                {
                    __printf("\033[22mInvalid option for echo: %s. Use 'on' or 'off'.\033[0m\n",
                             args->argv[2]);
                }
            }
            else if (strcmp(subcommand, "theme") == 0)
            {
                if (args->argc < 3)
                {
                    __printf("\033[22mUsage: set theme [ensimag|phelma|ense3|papet]\033[0m\n");
                    return;
                }
                if (strcmp(args->argv[2], "ensimag") == 0)
                {
                    set_theme(ENSIMAG_THEME);
                    __printf("\033[12mTheme set to ENSIMAG.\033[0m\n");
                    print_banner();
                }
                else if (strcmp(args->argv[2], "phelma") == 0)
                {
                    set_theme(PHELMA_THEME);
                    __printf("\033[12mTheme set to PHELMA.\033[0m\n");
                    print_banner();
                }
                else if (strcmp(args->argv[2], "ense3") == 0)
                {
                    set_theme(ENSE3_THEME);
                    __printf("\033[12mTheme set to ENSE3.\033[0m\n");
                    print_banner();
                }
                else if (strcmp(args->argv[2], "papet") == 0)
                {
                    set_theme(PAPET_THEME);
                    __printf("\033[12mTheme set to PAPET.\033[0m\n");
                    print_banner();
                }
                else
                {
                    __printf("\033[22mInvalid theme: %s. Use 'ensimag', 'phelma', 'ense3' or 'papet'.\033[0m\n",
                             args->argv[2]);
                }
            }
            return;
        }
    }
    __printf(
        "\033[22mUnknown subcommand: %s. Use 'set --list' to see available subcommands.\033[0m\n",
        subcommand);
}
