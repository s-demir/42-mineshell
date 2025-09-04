/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amsagman <amsagman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 14:59:25 by amsagman          #+#    #+#             */
/*   Updated: 2025/08/24 12:28:42 by amsagman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>

extern int				g_var_thing;

typedef enum e_file_type
{
	FILE_TYPE_NONE = 0,
	READ_FILE,
	READ_FROM_APPEND,
	WRITE_FILE,
	WRITE_FILE_APPEND,
	EXECUTE_FILE,
	FILE_READY
}						t_file_type;

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_REDIR_HEREDOC,
	TOKEN_ENV_VAR
}						t_token_type;

typedef enum e_pipe_index
{
	PIPE_ACTIVE = 0,
	PIPE_INPUT_FD,
	PIPE_OUTPUT_FD,
	PIPE_INPUT_COUNT,
	PIPE_OUTPUT_COUNT,
	PIPE_TOTAL,
	PIPE_HAS_INPUT,
	PIPE_HAS_OUTPUT,
	PIPE_REDIR_COMBO,
	PIPE_RESERVED1,
	PIPE_CHILD_COUNT,
	PIPE_EXEC_STATE,
	PIPE_RESERVED2,
	PIPE_STATE_SIZE
}						t_pipe_index;

typedef struct s_token
{
	t_token_type		type;
	char				*value;
	struct s_token		*next;
}						t_token;

typedef struct s_ast_node
{
	t_token_type		type;
	t_file_type			file_type;
	char				**args;
	struct s_ast_node	*left;
	struct s_ast_node	*right;
}						t_ast_node;

typedef struct s_env
{
	char				**original_env;
	char				***parsed_env;
	int					*piped;
	int					piped_size;
	int					out_fd;
	char				*logical_pwd;
}						t_env;

/* ----------------------------- main ----------------------------- */
int						main(int argc, char **argv, char **envp);
void					execute_shell_loop(t_env *env);
t_token					*parse_user_input(char *input, int *status);

t_env					*get_env(void); // yeni

/* ---------------------------- utils ----------------------------- */
void					configure_signal_handlers(void);
void					child_ctrl_c(int sig_num);
void					handle_ctrl_c(int a);
void					handle_ctrl_backslash(int sig);
int						get_exit_status(int err);
int						count_in_digits(int num);
void					cleanup_shell(t_env *env, int status);
void					close_pipe_fds(int fd1, int fd2);
void					cleanup_redirection_fds(int *piped);
void					safe_close(int fd);

char					*remove_quotes_from_str(char *str, int si_q_c,
							int do_q_c, int a);
char					*strcopy(char *src);
void					duplicate_string(char *dest, const char *src, int start,
							int end);
int						string_length(char *str, char end);
int						str_cmp(char *s_1, char *s_2, char *s_3);

int						strtoint(char *str);
int						count_array_strings(char **arr);

/* ---------------------------- env ------------------------------ */
void					update_old_pwd(t_env *env, int idx);
int						start_shell(t_env *env, char **orig_env);
int						setup_env(t_env *env, char **orig_env, int count);

char					*build_path_from_env(char *env_var, char *file,
							int *indx_s);
int						find_substr_index(char **haystack, char *needle,
							int n_length);
int						check_path_access(char *path, int mode);
void					advance_path_index(char *env_var, int *indx_s, int *a);
int						count_tokens(char *str, char del);

char					*check_env_path(char *env_path, char *file,
							int *indices, int mode);
char					*find_cmd_path(char *file, char **envp, char *env_var,
							int mode);
char					*extract_next_substring(char *str, char del,
							int *index);
char					**prepare_cmd_arguments(char *cmd, char **envp, int c);

/* --------------------------- expander -------------------------- */
void					child_fds_managment(int *piped, int *fd, int *fd_);
void					parent_fds_managment(int *piped, int *fd, int *fd_);

int						is_valid_var_start(char *str, int index, int con);
void					append_env_var(char *var, t_env *env);
int						locate_env_var_index(t_env *env, char *name);

char					**copy_env_exclude_index(char **env, int a, int a2,
							int d);
char					***duplicate_env_structure(t_env *env, int a, int a2,
							int d);
void					remove_env_entry(t_env *env, int c);
void					add_env_entry(t_env *env, char *cmd, int b, int con);

void					update_environment_var(char *var, t_env *env);
void					set_env_status(t_env *env, int status, char *start);
int						is_space(char *line);
char					*str_without_char(char *str, char del);

char					*replace_var_with_value(char *old_var, char *new,
							int st, int end);
char					*expand_variable_in_string(char *var, t_env *env,
							int *f_hole);
char					*recursively_expand_variables(char *var, t_env *env,
							int con, int *f_arr);
char					**refactore_args_array(char **args,
							int *quick_norm_fix);
void					expand_variables_in_ast(t_ast_node *head, t_env *env);

int						simplified_refactor_thing(char **array, int index,
							char *str, int in);
int						is_flawed_str(char *str, int a, int b, int res);
int						count_arg_flaws(char **array);

/* ---------------------------- lexer ---------------------------- */
void					new_quote_state(char c, int *single_q, int *double_q);
const char				*is_spaces(const char *input);
int						is_invalid_operator(const char **input);
int						check_syntax(const char *input);

int						compare_strings(char *s1, char *s2, int max);
void					exit_heredoc(int sig);
int						has_quotes(char *s);

void					manage_heredoc_fds(int *out_fd, int *piped, int status);
int						run_heredoc(char *limiter, int *piped, t_env *env);

/* --------------------------- parser ---------------------------- */
t_token					*create_token(t_token_type type, char *value);
void					append_token(t_token **tokens, t_token *new_token);
void					clear_tokens(t_token *tokens);
void					update_quote_status(char c, int *in_quote,
							char *quote_char);
void					add_word_token_if_valid(char **start, char **input,
							t_token **tokens);

void					handle_special(char **input, t_token **tokens);
void					handle_word(char **input, t_token **tokens);
t_token					*tokenize_input(char *input);

t_ast_node				*make_file(t_token *token);
t_ast_node				*handle_redir_loop(t_token **tokens, t_token *tmp);
t_ast_node				*parse_redirection(t_token **tokens);
t_ast_node				*par_pipe(t_token **tokens);
t_ast_node				*parse_tokens(t_token **tokens);

void					init_file(t_ast_node *node, t_token *token);
t_ast_node				*build_command(t_token **tokens);
void					setup_args(t_ast_node *node, t_token **tokens);

void					init_node(t_ast_node *node, t_token_type type);
t_ast_node				*make_node(t_token_type type);

t_ast_node				*link_redirect(t_token **tokens, t_token *tmp);
int						count_arguments(t_token *current);
void					set_args(t_ast_node *command_node, t_token **tokens,
							int arg_count);

/* -------------------------- executor --------------------------- */
int						run_builtin_child(char **cmd, int *fd, t_env *env,
							int *piped);
int						run_redir_builtin(char **cmd, int *fd, t_env *env,
							int *piped);
void					hnd_red_child(char **cmd, int *fd, t_env *env,
							int *piped);

char					**combine_command_arguments(char **f_args, char **cmd);
int						execute_builtin(char **args, int *fd, t_env *env,
							int *piped);
int						is_numeric_string(char *str);

int						execute_builtin_with_piping(char **cmd, int *fd,
							t_env *env, int *piped);
int						execute_builtin_with_simple_piping(char **cmd, int *fd,
							t_env *env, int *piped);
int						run_basic_cmd(char **cmd, int *fd, char **env,
							int *piped);
int						setup_pipe_if_needed(int *pipe_fd, int *piped);
void					cleanup_parent_fds(int *pipe_fd, int *fd, int *piped);

int						run_redir_cmd(char **cmd, int *fd, char **env,
							int *piped);
int						prep_run_cmd(char **cmd, int *fd, int *piped,
							t_env *env);
int						wait_children(int status, int *piped);
void					execute_commands(t_ast_node *head, t_env *env,
							int *status);

void					initialize_or_reset_pipe_state(int *piped, int f);
int						switch_fds_identifier(int *piped, int index, int index2,
							int con);
int						open_file_for_redirection(t_ast_node *head, int *_piped,
							t_env *env, int status);
int						check_if_command_is_builtin(char *cmd);
int						run_builtin_in_child(char **cmd, t_env *env,
							int *out_fd);

int						handle_piped_cmd(t_ast_node *head, int *piped,
							t_env *env, int *fd);
int						manage_redir(t_ast_node *head, int *piped, t_env *env,
							int *fd);
int						execute_ast_node(t_ast_node *head, int *piped,
							t_env *env);

void					set_node_types(t_ast_node *head);
void					count_redirects(t_ast_node *head, int *piped);

void					check_directory_status(char *path, char *file,
							int *status);
int						report_command_error(char *file, int status);
void					verify_path(t_ast_node *head, char **env, int *status);
int						check_cmd_perms(t_ast_node *head, char **env);

/* -------------------------- builtin ---------------------------- */
int						echo_cmd(char **cmd, int *out_fd);
int						env_or_pwd_cmd(char *cmd, t_env *env, int con,
							int *out_fd);
char					**export_cmd(char **cmd, t_env *env, int *out_fd,
							int **s);
char					**unset_or_export_cmd(char **cmd, t_env *env,
							int *out_fd, int *s);
int						cd_cmd(char **cmd, t_env *env, int *out_fd);
void					manage_redir_fds(int *out_fd, int *piped);
void					setup_child_fds(int *fd, int *out_fd, int *piped,
							int *child_fd);

void					update_pwd_env(char *new_path, t_env *env, int unused);
int						change_current_directory(char *path, t_env *env);
char					*get_current_working_directory(int size, int tries,
							int fd);
char					***sort_tha_array(char ***array, int size);
void					print_export_declaration_to_fd(t_env *env, int *out_fd);
void					handle_append_export(char *cmd, t_env *env,
							int var_len);

void					print_env_var_to_fd(char *str1, char *str2, int fd);
void					print_export_vars(char ***arr, int a, int fd);
int						compare_string_order(char *s1, char *s2);
int						is_array_sorted(char ***array, int size);
int						is_valid_echo_option(char *s);
void					process_export_assignment(char *cmd, int result,
							t_env *env);

char					*build_pwd_string(char *new_path);
void					close_session(char **cmd);
int						export_statment_check(char *cmd);
int						is_export_with_args(char **cmd);

int						find_equal_pos(char *cmd, int len);
int						validate_var_name(char *cmd, int var_len);
char					*get_logical_pwd(t_env *env);
void					update_logical_pwd(t_env *env, char *new_path);
char					*resolve_path(char *current_pwd, char *path);

#endif
