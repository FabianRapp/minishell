/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 08:00:49 by mevangel          #+#    #+#             */
/*   Updated: 2024/03/14 23:55:07 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
behavior from bash:

bash-3.2$ echo -n -n -n hello
hellobash-3.2$ echo -n hello
hellobash-3.2$ echo -nnnnn hello
hellobash-3.2$ echo -n nnn hello
nnn hellobash-3.2$ echo -n -n  hello
hellobash-3.2$ echo -n -n  hello

bash-3.2$ echo he$llo
he
bash-3.2$
*/

//TODO: i need to add the -n