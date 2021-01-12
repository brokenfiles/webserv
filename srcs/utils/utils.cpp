//
// Created by Timothe Lecouvreur on 12/01/2021.
//

#include <stdlib.h>

static int	ft_strlen(const char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i] != '\0')
		i++;
	return (i);
}

char		*ft_strdup(const char *str)
{
	int		i;
	char	*res;

	i = 0;
	if (!(res = (char*)malloc(sizeof(char) * (ft_strlen(str) + 1))))
		return (NULL);
	while (str[i] != '\0')
	{
		res[i] = str[i];
		i++;
	}
	res[i] = '\0';
	return (res);
}