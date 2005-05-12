/*
MySecureShell permit to add restriction to modified sftp-server
when using MySecureShell as shell.
Copyright (C) 2004 Sebastien Tardif

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation (version 2)

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

static t_hash		*_hash = 0;
static t_element	*_last_key = 0;

void	create_hash()
{
  _hash = calloc(1, sizeof(*_hash));
  _last_key = 0;
}

void		delete_hash()
{
  t_element	*t, *n;
  int		nb = sizeof(_hash->hash) / sizeof(*_hash->hash);
  int		i;

  for (i = 0; i < nb; i++)
    {
      t = _hash->hash[i];
      while (t)
	{
	  n = t->next;
	  free(t->key);
	  if (t->free)
	    free(t->value);
	  t = n;
	}
    }
  free(_hash);
  _hash = 0;
  _last_key = 0;
}

void		hash_set(char *key, void *value, char free_old)
{
  t_element	*t = _hash->hash[(int )*key];

  while (t)
    {
      if (!strcmp(key, t->key))
	{
	  if (free_old)
	    free(t->value);
	  t->value = value;
	  if (free_old)
	    t->free = free_old;
	  return;
	}
      t = t->next;
    }
  t = calloc(1, sizeof(*t));
  t->key = strdup(key);
  t->value = value;
  t->next = _hash->hash[(int )*key];
  t->free = free_old;
  _hash->hash[(int )*key] = t;
}

void		*hash_get(char *key)
{
  t_element	*t = _hash->hash[(int )*key];

  if (_last_key && !strcmp(key, _last_key->key))
    return (_last_key->value);
  while (t)
    {
      if (!strcmp(key, t->key))
	{
	  _last_key = t;
	  return (t->value);
	}
      t = t->next;
    }
  return (0);
}

char	*hash_get_int_to_char(char *key)
{
  char	*str;
  int	nb, size;

  nb = (int )hash_get(key);
  size = sizeof(*str) * 12;
  str = malloc(12);
  snprintf(str, nb, "%i", nb);
  return (str);
}