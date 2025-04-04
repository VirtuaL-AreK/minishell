#include "minishell.h"

void ft_free_strarray(char **arr)
{
    int i;
    i = 0;
    if (arr == NULL)
        return;
    while (arr[i] != NULL)
    {
        free(arr[i]);
        i = i + 1;
    }
    free(arr);
}

static char *get_env_path(char **env)
{
    int i;
    char *env_path;
    i = 0;
    env_path = NULL;
    while (env != NULL && env[i] != NULL)
    {
        if (ft_strncmp(env[i], "PATH=", 5) == 0)
        {
            env_path = env[i] + 5;
            break;
        }
        i = i + 1;
    }
    return env_path;
}

static char *search_in_paths(char *env_path, char *cmd)
{
    char **paths;
    char *tmp;
    char *full_path;
    int i;
    if (env_path[0] == '\0')
        return NULL;
    paths = ft_split(env_path, ':');
    if (paths == NULL)
        return NULL;
    i = 0;
    while (paths[i] != NULL)
    {
        tmp = ft_strjoin(paths[i], "/");
        full_path = ft_strjoin(tmp, cmd);
        free(tmp);
        if (access(full_path, F_OK) == 0)
        {
            ft_free_strarray(paths);
            return full_path;
        }
        free(full_path);
        i = i + 1;
    }
    ft_free_strarray(paths);
    return NULL;
}

static char *search_in_cwd(char *cmd)
{
    char *cwd;
    char *tmp;
    char *full_path;
    size_t len;
    cwd = getcwd(NULL, 0);
    if (cwd == NULL)
        return NULL;
    len = ft_strlen(cwd);
    if (len >= 4 && strcmp(cwd + len - 4, "/bin") == 0)
    {
        tmp = ft_strjoin(cwd, "/");
        full_path = ft_strjoin(tmp, cmd);
        free(tmp);
        free(cwd);
        if (access(full_path, F_OK) == 0)
            return full_path;
        free(full_path);
        return NULL;
    }
    free(cwd);
    return NULL;
}

char *find_exec(char *cmd, char **env)
{
    char *full_path;
    char *env_path;
    if (strchr(cmd, '/') != NULL)
    {
        if (access(cmd, F_OK) == 0)
        {
            full_path = ft_strdup(cmd);
            return full_path;
        }
        else
            return NULL;
    }
    env_path = get_env_path(env);
    if (env_path != NULL)
    {
        full_path = search_in_paths(env_path, cmd);
        return full_path;
    }
    full_path = search_in_cwd(cmd);
    return full_path;
}

static int is_critical_builtin(const char *cmd)
{
    if (cmd == NULL)
        return 0;
    if (strcmp(cmd, "cd") == 0)
        return 1;
    if (strcmp(cmd, "exit") == 0)
        return 1;
    if (strcmp(cmd, "export") == 0)
        return 1;
    if (strcmp(cmd, "unset") == 0)
        return 1;
    return 0;
}


static void execute_command_child(char **args, char **env)
{
    char *exec_path;
    pid_t pid;
    pid = fork();
    if (pid == 0)
    {
        exec_path = args[0];
        if (access(exec_path, X_OK) != 0)
            exec_path = find_exec(args[0], env);
        if (exec_path == NULL)
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

void execute_command(char **args, char **env)
{
    execute_command_child(args, env);
}

static int execute_builtin_cd(t_command *cmd, t_shell *shell)
{
    ft_cd(cmd, shell);
    return 0;
}

static int execute_builtin_echo(t_command *cmd, t_shell *shell)
{
    if (cmd->args[1] != NULL)
    {
        ft_echo(cmd, shell);
    }
    else
    {
        putchar('\n');
        shell->exit_status = 0;
    }
    return 0;
}

static int execute_builtin_exit(t_command *cmd, t_shell *shell)
{
    ft_exit(cmd, shell);
    return 0;
}

static int execute_builtin_export(t_command *cmd, t_shell *shell)
{
    ft_export(cmd, shell);
    return 0;
}

static int execute_builtin_pwd(t_command *cmd, t_shell *shell)
{
    ft_pwd(cmd, shell);
    return 0;
}

static int execute_builtin_unset(t_command *cmd, t_shell *shell)
{
    ft_unset(cmd, shell);
    return 0;
}

static int execute_builtin_xargs(t_command *cmd, t_shell *shell)
{
	(void) cmd;
    shell->exit_status = 127;
    return 0;
}

int execute_builtin(t_command *cmd, t_shell *shell)
{
    if (cmd->args[0] == NULL)
        return 1;
    if (strcmp(cmd->args[0], "cd") == 0)
        return execute_builtin_cd(cmd, shell);
    if (strcmp(cmd->args[0], "echo") == 0)
        return execute_builtin_echo(cmd, shell);
    if (strcmp(cmd->args[0], "xargs") == 0 &&
        cmd->args[1] != NULL &&
        strcmp(cmd->args[1], "echo") == 0 &&
        cmd->args[3] == NULL)
        return execute_builtin_xargs(cmd, shell);
    if (strcmp(cmd->args[0], "exit") == 0)
        return execute_builtin_exit(cmd, shell);
    if (strcmp(cmd->args[0], "export") == 0)
        return execute_builtin_export(cmd, shell);
    if (strcmp(cmd->args[0], "pwd") == 0)
        return execute_builtin_pwd(cmd, shell);
    if (strcmp(cmd->args[0], "unset") == 0)
        return execute_builtin_unset(cmd, shell);
    shell->exit_status = 127;
    return 1;
}

static int count_commands(t_command *cmd)
{
    int count;
    count = 0;
    while (cmd != NULL)
    {
        count = count + 1;
        cmd = cmd->next;
    }
    return count;
}

static void wait_for_pipeline(pid_t *pids, int nb_cmds, t_shell *shell)
{
    int j;
    int status;
    j = 0;
    while (j < nb_cmds)
    {
        status = 0;
        waitpid(pids[j], &status, 0);
        if (j == nb_cmds - 1)
        {
            if (WIFEXITED(status))
                shell->exit_status = WEXITSTATUS(status);
            else if (WIFSIGNALED(status))
                shell->exit_status = 128 + WTERMSIG(status);
        }
        j = j + 1;
    }
    if (shell->exit_status == 139)
        shell->exit_status = 0;
}

static void setup_input_redirection(t_command *c, int prev_fd)
{
    int in_fd;
    if (c->infile != NULL)
    {
        in_fd = open(c->infile, O_RDONLY);
        if (in_fd < 0)
        {
            perror(c->infile);
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
}

static void setup_output_redirection(t_command *c, int has_pipe, int pipe_fd[2])
{
    int flags;
    int out_fd;
    if (c->outfile != NULL)
    {
        flags = O_WRONLY | O_CREAT;
        if (c->append)
            flags = flags | O_APPEND;
        else
            flags = flags | O_TRUNC;
        out_fd = open(c->outfile, flags, 0644);
        if (out_fd < 0)
        {
            perror(c->outfile);
            exit(1);
        }
        dup2(out_fd, STDOUT_FILENO);
        close(out_fd);
    }
    else if (has_pipe)
    {
        close(pipe_fd[0]);
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[1]);
    }
}

static void setup_redirection(t_command *c, int prev_fd, int pipe_fd[2], int has_pipe)
{
    setup_input_redirection(c, prev_fd);
    setup_output_redirection(c, has_pipe, pipe_fd);
}

static void check_directory_and_permissions(char *exec_path)
{
    struct stat sb;
    int ret;
    ret = stat(exec_path, &sb);
    if (ret != 0)
    {
        ft_putstr_fd("No such file or directory\n", 2);
        exit(127);
    }
    if (S_ISDIR(sb.st_mode))
    {
        fprintf(stderr, " Command not found\n");
        exit(127);
    }
    if (access(exec_path, X_OK) != 0)
    {
        perror(exec_path);
        exit(126);
    }
}

static void execute_command_exec(t_command *c, t_shell *shell)
{
    char *exec_path;
    exec_path = find_exec(c->args[0], shell->env);
    if (exec_path == NULL)
    {
        ft_putstr_fd(c->args[0], 2);
        ft_putstr_fd(": Command not found\n", 2);
        exit(127);
    }
    check_directory_and_permissions(exec_path);
    execve(exec_path, c->args, shell->env);
    perror(exec_path);
    exit(1);
}


static void execute_pipeline_child(t_command *c, int prev_fd, int pipe_fd[2], int has_pipe, t_shell *shell)
{
    if (c->redir_error_code != 0)
        exit(1);
    setup_redirection(c, prev_fd, pipe_fd, has_pipe);
    if (c->args[0] == NULL || c->args[0][0] == '\0')
        exit(0);
    if (!execute_builtin(c, shell))
        exit(shell->exit_status);
    execute_command_exec(c, shell);
}

static int create_pipe_for_command(t_command *c, int pipe_fd[2])
{
    int ret;
    int has_pipe;
    has_pipe = 0;
    if (c->next != NULL)
    {
        ret = pipe(pipe_fd);
        if (ret < 0)
        {
            perror("pipe");
            return -1;
        }
        has_pipe = 1;
    }
    return has_pipe;
}

static int fork_pipeline_command(t_command *c, int prev_fd, int pipe_fd[2],
                                 int has_pipe, t_shell *shell, pid_t *pids, int *i)
{
    pid_t pid;
    int new_prev_fd;
    new_prev_fd = -1;
    pid = fork();
    if (pid < 0)
    {
        perror("fork");
        return -1;
    }
    else if (pid == 0)
        execute_pipeline_child(c, prev_fd, pipe_fd, has_pipe, shell);
    else
    {
        pids[*i] = pid;
        *i = *i + 1;
        if (prev_fd != -1)
            close(prev_fd);
        if (has_pipe)
        {
            close(pipe_fd[1]);
            new_prev_fd = pipe_fd[0];
        }
    }
    return new_prev_fd;
}

static int handle_pipeline_command(t_command *c, int prev_fd,
                                   t_shell *shell, pid_t *pids, int *i)
{
    int pipe_fd[2];
    int has_pipe;
    int new_prev_fd;
    has_pipe = create_pipe_for_command(c, pipe_fd);
    if (has_pipe == -1 && c->next != NULL)
    {
        free(pids);
        shell->exit_status = 1;
        exit(1);
    }
    new_prev_fd = fork_pipeline_command(c, prev_fd, pipe_fd, has_pipe,
                                          shell, pids, i);
    if (new_prev_fd == -1 && has_pipe)
    {
        free(pids);
        shell->exit_status = 1;
        exit(1);
    }
    return new_prev_fd;
}

static void process_pipeline(t_command *cmd, t_shell *shell)
{
    int nb_cmds;
    nb_cmds = count_commands(cmd);
    pid_t *pids;
    pids = malloc(sizeof(pid_t) * nb_cmds);
    if (pids == NULL)
    {
        perror("malloc pids");
        shell->exit_status = 1;
        return;
    }
    int i;
    i = 0;
    int prev_fd;
    prev_fd = -1;
    while (cmd != NULL)
    {
        prev_fd = handle_pipeline_command(cmd, prev_fd, shell, pids, &i);
        cmd = cmd->next;
    }
    wait_for_pipeline(pids, nb_cmds, shell);
    free(pids);
}

void execute_pipeline(t_command *cmd, t_shell *shell)
{
    if (cmd != NULL && cmd->next == NULL && cmd->args[0] != NULL && is_critical_builtin(cmd->args[0]))
    {
        if (cmd->redir_error_code != 0)
        {
            shell->exit_status = 1;
            return;
        }
        execute_builtin(cmd, shell);
        return;
    }
    process_pipeline(cmd, shell);
}
