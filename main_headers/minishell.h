/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 06:20:46 by frapp             #+#    #+#             */
/*   Updated: 2024/01/17 10:50:09 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// libs
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>


// this projects headers
# include <libft.h>
# include <utils.h>

// lexer
t_lexer		new_lexer(char *str);
t_token		next_token(t_lexer *lexer);


#endif