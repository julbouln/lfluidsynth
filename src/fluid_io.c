/* FluidSynth - A Software Synthesizer
 *
 * Copyright (C) 2003  Peter Hanappe and others.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public License
 * as published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 * 02111-1307, USA
 */

#include "fluid_io.h"

int fluid_istream_gets(fluid_istream_t in, char* buf, int len);

fluid_istream_t fluid_get_stdin()
{
  return STDIN_FILENO;
}

fluid_ostream_t fluid_get_stdout()
{
  return STDOUT_FILENO;
}

int fluid_istream_readline(fluid_istream_t in, char* prompt, char* buf, int len)
{
  return fluid_istream_gets(in, buf, len);
}

/* FIXME */
int fluid_istream_gets(fluid_istream_t in, char* buf, int len)
{
  char c;
  int n;

  buf[len - 1] = 0;

  while (--len > 0) {
    n = read(in, &c, 1);
    if (n == 0) {
      *buf++ = 0;
      return 0;
    }
    if (n < 0) {
      return n;
    }
    if ((c == '\n') || (c == '\r')) {
      *buf++ = 0;
      return 1;
    }
    *buf++ = c;
  }

  return -1;
}


int fluid_ostream_printf(fluid_ostream_t out, char* format, ...)
{
  char buf[4096];
  va_list args;
  int len;

  va_start(args, format);
  len = vsnprintf(buf, 4095, format, args);
  va_end(args);

  if (len <= 0) {
    printf("fluid_ostream_printf: buffer overflow");
    return -1;
  }

  buf[4095] = 0;

/*   return write(out, buf, len); */
  return write(out, buf, strlen(buf));
}
