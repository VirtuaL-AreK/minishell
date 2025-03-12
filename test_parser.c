#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <readline/readline.h>
#include <readline/history.h>

/* ************************************************************************** */
/*                              Structures & Utils                            */
/* ************************************************************************** */

/* Liste chaînée de tokens pour éviter realloc */
typedef struct s_toklist
{
    char               *value;
    struct s_toklist   *next;
}   t_toklist;

/* Ajoute un token en fin de liste */
static void add_token_list(t_toklist **head, const char *value)
{
    t_toklist *new_node = malloc(sizeof(*new_node));
    if (!new_node)
        return; // out of memory, on ne gère pas l'erreur ici
    new_node->value = strdup(value);
    new_node->next = NULL;
    
    if (!*head)
    {
        *head = new_node;
    }
    else
    {
        t_toklist *tmp = *head;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = new_node;
    }
}

/* Convertit la liste chaînée en tableau de char* et la libère au passage */
static char **list_to_array_and_free(t_toklist *head)
{
    /* 1) Compter le nombre d'éléments */
    int count = 0;
    t_toklist *tmp = head;
    while (tmp)
    {
        count++;
        tmp = tmp->next;
    }

    /* 2) Allouer le tableau (+1 pour NULL final) */
    char **arr = malloc(sizeof(char*) * (count + 1));
    if (!arr)
        return NULL;

    /* 3) Recopier les valeurs */
    tmp = head;
    for (int i = 0; i < count; i++)
    {
        arr[i] = tmp->value;     /* on transfère la chaîne directement */
        t_toklist *old = tmp;
        tmp = tmp->next;
        free(old);               /* on libère le node (mais pas old->value) */
    }
    arr[count] = NULL;

    return arr;
}

/* ************************************************************************** */
/*                 Définition des caractères spéciaux & helpers               */
/* ************************************************************************** */

static int is_special_char(int c)
{
    /* À adapter selon ton mini-shell. Ici, on considère :
     *  | < > ; & ( ) comme spéciaux, en plus des espaces hors quote.
     */
    return (c == '|' || c == '<' || c == '>' ||
            c == ';' || c == '&' || c == '(' || c == ')');
}

/* Sauter les espaces */
static void skip_spaces(const char *line, int *i)
{
    while (line[*i] && isspace((unsigned char)line[*i]))
        (*i)++;
}

/* ************************************************************************** */
/*   parse_one_token_bash_style : Construit un token en fusionnant segments   */
/* ************************************************************************** */

/*
   Bash concatène :

     - Du texte hors quote
     - Du texte entre '...' (simple quotes)
     - Du texte entre "..." (double quotes)

   tant qu'on ne rencontre pas un délimiteur hors quote (espace, char spécial).
   Exemple :  ab"cd"ef'gh' => 1 token "abcdefgh"

   On renvoie une chaîne (malloc) représentant ce token complet.
   On s'arrête dès qu'on tombe sur un espace ou un char spécial hors quote.
*/

static char *parse_one_token_bash_style(const char *line, int *i)
{
    /* On va construire le token dans un buffer local
     * de taille fixe (genre 4096). Pour un mini-shell, c'est acceptable.
     * Si tu as besoin d'être plus "infiniment extensible", il faudrait
     * un second chainon ou re-alloc. Mais tu as dit "pas de realloc".
     */
    char buffer[4096];
    int length = 0;
    buffer[0] = '\0';

    while (line[*i])
    {
        /* Si on tombe sur un délimiteur (espace ou special) hors quote -> stop */
        if (isspace((unsigned char)line[*i]) || is_special_char(line[*i]))
            break;

        /* Single quote ? */
        if (line[*i] == '\'')
        {
            (*i)++;  // skip la quote d'ouverture
            while (line[*i] && line[*i] != '\'')
            {
                if (length < 4095)
                    buffer[length++] = line[*i];
                (*i)++;
            }
            if (line[*i] == '\'')
                (*i)++; /* skip la quote fermante */
        }
        /* Double quote ? */
        else if (line[*i] == '"')
        {
            (*i)++;  // skip la quote d'ouverture
            while (line[*i] && line[*i] != '"')
            {
                /* on pourrait gérer l'escape de \" ici, 
                 * mais pour rester simple, on recopie juste
                 */
                if (length < 4095)
                    buffer[length++] = line[*i];
                (*i)++;
            }
            if (line[*i] == '"')
                (*i)++; /* skip la quote fermante */
        }
        /* Sinon : texte normal */
        else
        {
            /* On recopie jusqu'à espace, special ou quote */
            while (line[*i] &&
                   !isspace((unsigned char)line[*i]) &&
                   !is_special_char(line[*i]) &&
                   line[*i] != '\'' &&
                   line[*i] != '"')
            {
                if (length < 4095)
                    buffer[length++] = line[*i];
                (*i)++;
            }
        }
    }
    buffer[length] = '\0';
    /* On renvoie une copie heap-alloc de ce buffer */
    return strdup(buffer);
}

/* ************************************************************************** */
/*  tokenize_bash_style_no_realloc : parse la ligne, retourne char** tokens   */
/* ************************************************************************** */

char **tokenize_bash_style_no_realloc(const char *line)
{
    t_toklist *list = NULL; /* liste chaînée de tokens */
    int i = 0;

    while (line[i])
    {
        skip_spaces(line, &i);
        if (!line[i]) 
            break;

        /* Si c'est un caractère spécial (hors quote), on en fait un token seul. */
        if (is_special_char(line[i]))
        {
            char special[2];
            special[0] = line[i];
            special[1] = '\0';
            add_token_list(&list, special);
            i++;
        }
        else
        {
            /* On parse un token "bash style" qui peut combiner segments */
            char *token = parse_one_token_bash_style(line, &i);
            /* Si le token n'est pas vide, on l'ajoute */
            if (token && token[0])
                add_token_list(&list, token);
            free(token);
        }
    }

    /* Convertir la liste en tableau */
    char **array = list_to_array_and_free(list);
    return array;
}

/* ************************************************************************** */
/*                              Exemple de test                               */
/* ************************************************************************** */



int main(void)
{
	char *input;

	while (1)
	{
		input = readline("\033[1;32mminishell$\033[0m ");
		if (!input)
		{
			printf("exit\n");
			rl_clear_history();
			exit(0);
		}
		if (*input)
			add_history(input);
		char **toks = tokenize_bash_style_no_realloc(input);
        if (!toks)
        {
            printf("  -> error or empty\n\n");
        }
        // Afficher les tokens
        for (int i = 0; toks[i]; i++)
        {
            printf("  token[%d] = '%s'\n", i, toks[i]);
        }
        // Libération
        for (int i = 0; toks[i]; i++)
            free(toks[i]);
        free(toks);
        printf("\n");

		//printf("You entered : %s\n", input);
		free(input);
	}
    return 0;
}
