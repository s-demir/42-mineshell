/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expand.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amsagman <amsagman@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 17:17:59 by sedemir           #+#    #+#             */
/*   Updated: 2025/08/15 00:30:19 by amsagman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*replace_var_with_value(char *old_var, char *__new, int st,
		int end)
{
	int		size;
	char	*new__;
	int		unsize;

	unsize = string_length(__new, '\0');
	size = st + (string_length(old_var, '\0') - end) + unsize;
	new__ = galloc(size + 1);
	if (!new__)
		return (NULL);
	duplicate_string(new__, old_var, 0, st);
	duplicate_string(new__ + st, __new, 0, unsize);
	duplicate_string(new__ + st + unsize, old_var, end, string_length(old_var,
			'\0'));
	return (new__);
}

char	*recursively_expand_variables(char *var, t_env *env, int __con,
		int *f_arr)
{
	char	*new_var;

	while (var[f_arr[0]])
	{
		if (var[f_arr[0]] == 39)
		{
			f_arr[0]++;
			f_arr[1]++;
			while (!(f_arr[2] % 2) && var[f_arr[0]] && var[f_arr[0]] != 39)
				f_arr[0]++;
		}
		if (var[f_arr[0]] == 34)
			f_arr[2]++;
		if (is_valid_var_start(var, f_arr[0], 1) && ((!(f_arr[2] % 2) && __con)
				|| (f_arr[2] % 2 && !__con)))
			return (new_var = expand_variable_in_string(var, env,
					&f_arr[0]), recursively_expand_variables(new_var, env,
					__con, f_arr));
		f_arr[0]++;
	}
	return (var);
}

void	expand_variables_in_ast(t_ast_node *head, t_env *env)
{
	int	a;
	int	f_arr[3];

	if (head->file_type != FILE_READY && head->args
		&& head->file_type != READ_FROM_APPEND)
	{
		a = -1;
		while (head->args[++a])
			((ft_memset(f_arr, 0, 3 * sizeof(int))),
				(head->args[a] = recursively_expand_variables(head->args[a],
						env, 1, f_arr)));
		head->args = refactore_args_array(head->args, &a);
		while (head->args[a])
		{
			ft_memset(f_arr, 0, 3 * sizeof(int));
			head->args[a] = recursively_expand_variables(head->args[a], env, 0,
					f_arr);
			head->args[a] = remove_quotes_from_str(head->args[a], 0, 0, 0);
			a++;
		}
	}
	if (head->left)
		expand_variables_in_ast(head->left, env);
	if (head->right)
		expand_variables_in_ast(head->right, env);
}
