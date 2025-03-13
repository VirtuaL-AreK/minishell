#include "minishell.h"

void handle_cat(t_command *cmd)
{
    char buffer[1024];
    int bytes;
	int i;

	i = 0;
    if (strcmp(cmd->args[0], "cat") == 0 && !cmd->args[1])
    {
        while ((bytes = read(STDIN_FILENO, buffer, sizeof(buffer))) > 0)
        {
            if (write(STDOUT_FILENO, buffer, bytes) < 0)
            {
                perror("write");
                gexitstatus = 1;
                return;
            }
        }
        gexitstatus = 0;
        return;
    }
    else if (strcmp(cmd->args[0], "cat") == 0 && strcmp(cmd->args[1], "-e") == 0)
    {
        while ((bytes = read(STDIN_FILENO, buffer, sizeof(buffer))) > 0)
        {
            while (i < bytes)
            {
                if (buffer[i] == '\n')
                    write(STDOUT_FILENO, "$\n", 2);
                else
                    write(STDOUT_FILENO, &buffer[i], 1);
				i++;
            }
        }
        gexitstatus = 0;
        return;
    }
    else if (strcmp(cmd->args[0], "cat") == 0 && cmd->args[1])
    {
        execute_command(cmd->args, environ);
        gexitstatus = 0;
        return;
    }
}

void ft_free_strarray(char **arr)
{
    int i = 0;

    if (!arr)
        return;
    while (arr[i])
    {
        free(arr[i]);
        i++;
    }
    free(arr);
}

char *find_exec(char *cmd)
{
    char    **paths;
    char    *path;
    char    *full_path;
    char    *tmp;
    int     i;

    // 1) Si la commande contient un slash (ex: "./a.out", "/usr/bin/ls" etc.),
    //    on ne cherche PAS dans $PATH. On teste juste s'il existe (F_OK).
    if (ft_strchr(cmd, '/'))
    {
        if (access(cmd, F_OK) == 0)
            return ft_strdup(cmd);  // On renvoie une copie de cmd
        else
            return NULL;            // Fichier introuvable => "command not found"
    }

    // 2) Sinon, on va chercher dans $PATH
    path = getenv("PATH");
    if (!path || !*path)
        return NULL; // pas de PATH => on ne trouve rien

    paths = ft_split(path, ':'); // ou votre fonction de split habituelle
    if (!paths)
        return NULL;

    i = 0;
    while (paths[i])
    {
        tmp = ft_strjoin(paths[i], "/");
        full_path = ft_strjoin(tmp, cmd);
        free(tmp);

        // Ici on teste la simple existence (F_OK)
        if (access(full_path, F_OK) == 0)
        {
            // On a trouvé un binaire (ou un script) existant.
            ft_free_strarray(paths);
            return full_path;
        }
        free(full_path);
        i++;
    }

    ft_free_strarray(paths);
    return NULL; // Rien trouvé => "command not found"
}



void execute_command(char **args, char **env)
{
	(void)env;
	char *exec_path;
	pid_t pid = fork();
	if (pid == 0)
	{
		exec_path = args[0];
		if(access(exec_path, X_OK) != 0)
			exec_path = find_exec(args[0]);
		if(!exec_path)
		{
			printf("%s: Command not found\n", args[0]);
			exit(1);
		}
		execve(exec_path, args, env);
		perror("minishell");
		exit(1);
	}
	else
	{
		waitpid(pid, NULL, 0);
	}
}

int execute_builtin(t_command *cmd, char **env)
{
	(void)env;
    if (!cmd->args[0])
        return (1);
    if (strcmp(cmd->args[0], "cd") == 0)
	{
		ft_cd(cmd);
        return (0);
	}
    if (strcmp(cmd->args[0], "echo") == 0)
	{
		ft_echo(cmd);
		return (0);
	}
    if (strcmp(cmd->args[0], "env") == 0)
	{
		ft_env(cmd);
		return (0);
	}
    if (strcmp(cmd->args[0], "exit") == 0)
	{
		ft_exit(cmd);
		return (0);
	}
    if (strcmp(cmd->args[0], "export") == 0)
	{
		ft_export(cmd);
		return (0);
	}
    if (strcmp(cmd->args[0], "pwd") == 0)
	{
		ft_pwd(cmd);
		return (0);
	}
    if (strcmp(cmd->args[0], "unset") == 0)
	{
		ft_unset(cmd);
		return (0);
	}
    return (1);
}

#include <string.h>

int is_simple_builtin(const char *cmd)
{
	if (!cmd)
		return 0;
	if (strcmp(cmd, "echo") == 0)
		return 1;
	if (strcmp(cmd, "pwd") == 0)
		return 1;
	if (strcmp(cmd, "export") == 0)
		return 1;
    return 0;
}

int is_builtin(const char *cmd)
{
	if (!cmd)
		return 0;
	if (strcmp(cmd, "cd") == 0)
		return 1;
	if (strcmp(cmd, "env") == 0)
		return 1;
	if (strcmp(cmd, "export") == 0)
		return 1;
	if (strcmp(cmd, "unset") == 0)
		return 1;
	if (strcmp(cmd, "exit") == 0)
		return 1;
    return 0;
}

static int is_last_command(t_command *cmd)
{
    return (cmd->next == NULL);
}


void execute_pipeline(t_command *cmd, char **env)
{
    int fd[2];
    int prev_fd = -1;
    pid_t pid;
    int status;
    char *exec_path;

    while (cmd)
    {
        if (!cmd->args[0])
        {
            // Aucune commande => on fait rien, code 0
            gexitstatus = 0;
            cmd = cmd->next;
            continue;
        }

        // 1) Erreur de redirection => on skip
        if (cmd->redir_error_code != 0)
        {
            pid = fork();
            if (pid == 0)
                exit(1);  // On sort en erreur
            else
            {
                waitpid(pid, &status, 0);
                if (WIFEXITED(status))
                    gexitstatus = WEXITSTATUS(status);

                // Gestion du pipe éventuel
                if (prev_fd != -1)
                    close(prev_fd);

                if (cmd->next)
                {
                    if (pipe(fd) < 0)
                    {
                        perror("pipe");
                        exit(1);
                    }
                    close(fd[1]);
                    prev_fd = fd[0];
                }
                cmd = cmd->next;
                continue;
            }
        }

        // 2) Cas "cat" géré séparément
        if (strcmp(cmd->args[0], "cat") == 0)
        {
            // -- (CODE EXISTANT, inchangé) --
            // Vous conservez la logique spéciale “cat” telle qu’elle est.
            // --------------------------------
            if (cmd->next)
            {
                // s’il y a un pipe après, on gère pipe+fork
                if (pipe(fd) < 0)
                {
                    perror("pipe");
                    exit(1);
                }
                pid = fork();
                if (pid == 0)
                {
                    // Fils
                    if (cmd->outfile)
                    {
                        int out_fd = open(cmd->outfile, O_WRONLY | O_CREAT | (cmd->append ? O_APPEND : O_TRUNC), 0644);
                        if (out_fd < 0)
                        {
                            perror(cmd->outfile);
                            exit(1);
                        }
                        dup2(out_fd, STDOUT_FILENO);
                        close(out_fd);
                    }
                    if (cmd->infile)
                    {
                        int in_fd = open(cmd->infile, O_RDONLY);
                        if (in_fd < 0)
                        {
                            perror(cmd->infile);
                            exit(1);
                        }
                        dup2(in_fd, STDIN_FILENO);
                        close(in_fd);
                        if (prev_fd != -1)
                            close(prev_fd);
                    }
                    dup2(fd[1], STDOUT_FILENO);
                    close(fd[0]);
                    close(fd[1]);
                    handle_cat(cmd);
                    exit(gexitstatus);
                }
                else
                {
                    close(fd[1]);
                    prev_fd = fd[0];
                    cmd = cmd->next;
                    continue;
                }
            }
            else
            {
                // cat sans pipeline
                pid = fork();
                if (pid == 0)
                {
                    if (cmd->outfile)
                    {
                        int out_fd = open(cmd->outfile, O_WRONLY | O_CREAT | (cmd->append ? O_APPEND : O_TRUNC), 0644);
                        if (out_fd < 0)
                        {
                            perror(cmd->outfile);
                            exit(1);
                        }
                        dup2(out_fd, STDOUT_FILENO);
                        close(out_fd);
                    }
                    if (cmd->infile)
                    {
                        int in_fd = open(cmd->infile, O_RDONLY);
                        if (in_fd < 0)
                        {
                            perror(cmd->infile);
                            exit(1);
                        }
                        dup2(in_fd, STDIN_FILENO);
                        close(in_fd);
                        if (prev_fd != -1)
                            close(prev_fd);
                    }
                    else if (prev_fd != -1)
                    {
                        dup2(prev_fd, STDIN_FILENO);
                        close(prev_fd);
                    }
                    handle_cat(cmd);
                    exit(gexitstatus);
                }
                else
                {
                    waitpid(pid, &status, 0);
                    if (WIFEXITED(status))
                        gexitstatus = WEXITSTATUS(status);
                    return;
                }
            }
        }

        // 3) Cas "builtin" (CD, export, unset, etc.)
        else if (is_builtin(cmd->args[0]))
        {
            // **Vérifier si on est seul ou dans un pipeline**
            if (is_last_command(cmd))
            {
                // => PAS de pipe après => on exécute le builtin dans le PÈRE
                //    comme avant (pour modifier l'environnement principal).
                execute_builtin(cmd, env);
                cmd = cmd->next;
                continue;
            }
            else
            {
                // => Il y a un pipeline => on doit forker comme pour une commande externe
                if (pipe(fd) < 0)
                {
                    perror("pipe");
                    exit(1);
                }
                pid = fork();
                if (pid == 0)
                {
                    // -- Fils --
                    // 1) Redirections d'entrée
                    if (cmd->infile)
                    {
                        int in_fd = open(cmd->infile, O_RDONLY);
                        if (in_fd < 0)
                        {
                            perror(cmd->infile);
                            exit(1);
                        }
                        dup2(in_fd, STDIN_FILENO);
                        close(in_fd);
                    }
                    else if (prev_fd != -1)
                    {
                        dup2(prev_fd, STDIN_FILENO);
                        close(prev_fd);
                    }

                    // 2) Redirection de sortie
                    if (cmd->outfile)
                    {
                        int out_fd = open(cmd->outfile, O_WRONLY | O_CREAT | (cmd->append ? O_APPEND : O_TRUNC), 0644);
                        if (out_fd < 0)
                        {
                            perror(cmd->outfile);
                            exit(1);
                        }
                        dup2(out_fd, STDOUT_FILENO);
                        close(out_fd);
                    }
                    else
                    {
                        // On redirige vers le pipe
                        close(fd[0]);
                        dup2(fd[1], STDOUT_FILENO);
                        close(fd[1]);
                    }

                    // 3) Exécuter le builtin dans le fils
                    //    => la fonction mettra gexitstatus à jour.
                    execute_builtin(cmd, env);
                    // on quitte le fils avec le code de retour
                    exit(gexitstatus);
                }
                else
                {
                    // -- Père --
                    waitpid(pid, &status, 0);
                    if (WIFEXITED(status))
                        gexitstatus = WEXITSTATUS(status);

                    if (prev_fd != -1)
                        close(prev_fd);

                    close(fd[1]);
                    prev_fd = fd[0];

                    cmd = cmd->next;
                    continue;
                }
            }
        }

        // 4) Sinon : commandes externes
        else
        {
            if (cmd->next)
            {
                if (pipe(fd) < 0)
                {
                    perror("pipe");
                    exit(1);
                }
            }
            pid = fork();
            if (pid == 0)
            {
                // -- Fils -- (commande externe)
                if (cmd->infile)
                {
                    int in_fd = open(cmd->infile, O_RDONLY);
                    if (in_fd < 0)
                    {
                        perror(cmd->infile);
                        exit(1);
                    }
                    dup2(in_fd, STDIN_FILENO);
                    close(in_fd);
                }
                if (prev_fd != -1)
                {
                    dup2(prev_fd, STDIN_FILENO);
                    close(prev_fd);
                }

                if (cmd->outfile)
                {
                    int out_fd = open(cmd->outfile, O_WRONLY | O_CREAT | (cmd->append ? O_APPEND : O_TRUNC), 0644);
                    if (out_fd < 0)
                    {
                        perror(cmd->outfile);
                        exit(1);
                    }
                    dup2(out_fd, STDOUT_FILENO);
                    close(out_fd);
                }
                else if (cmd->next)
                {
                    close(fd[0]);
                    dup2(fd[1], STDOUT_FILENO);
                    close(fd[1]);
                }

                // Exécution réelle du binaire
                if (is_simple_builtin(cmd->args[0]))
                    exit(execute_builtin(cmd, env));

					exec_path = find_exec(cmd->args[0]);

					// 1) Fichier introuvable => 127
					if (!exec_path)
					{
						ft_putstr_fd(cmd->args[0], 2);
						ft_putstr_fd(": command not found\n", 2);
						exit(127);
					}
					
					// 2) On vérifie si c’est un répertoire
					{
						struct stat sb;
						if (stat(exec_path, &sb) == 0)
						{
							if (S_ISDIR(sb.st_mode))
							{
								ft_putstr_fd(cmd->args[0], 2);
								ft_putstr_fd(": Is a directory\n", 2);
								exit(126);
							}
						}
						else
						{
							// Cas bizarre où stat échoue => on considère "pas trouvé" 
							// (ou "plus accessible"), code 127
							ft_putstr_fd(cmd->args[0], 2);
							ft_putstr_fd(": command not found\n", 2);
							exit(127);
						}
					}
					
					// 3) Vérifier la permission (X_OK)
					if (access(exec_path, X_OK) != 0)
					{
						ft_putstr_fd(cmd->args[0], 2);
						ft_putstr_fd(": Permission denied\n", 2);
						exit(126);
					}
					
					// 4) Exécuter
					execve(exec_path, cmd->args, env);
					
					// 5) Si on arrive ici, execve a échoué pour une autre raison
					//    (ou de nouveau EACCES si la permission a changé entre-temps)
					if (errno == EACCES)
					{
						ft_putstr_fd(cmd->args[0], 2);
						ft_putstr_fd(": Permission denied\n", 2);
						exit(126);
					}
					
					// Sinon, on affiche l’erreur du système
					perror(cmd->args[0]);
					exit(1);
					
                perror(cmd->args[0]);
                exit(1);
            }
            else
            {
                // -- Père --
                waitpid(pid, &status, 0);
                if (WIFEXITED(status))
                    gexitstatus = WEXITSTATUS(status);

                if (prev_fd != -1)
                    close(prev_fd);

                if (cmd->next)
                {
                    close(fd[1]);
                    prev_fd = fd[0];
                }
                cmd = cmd->next;
            }
        }
    } // Fin du while
}
