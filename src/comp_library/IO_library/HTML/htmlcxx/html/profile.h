/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * This file is part of htmlcxx -- A simple non-validating css1 and html parser
 * written in C++.
 *
 * htmlcxx is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * htmlcxx is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with htmlcxx.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * Copyright 2005-2010 Davi de Castro Reis and Robson Braga Araújo
 * Copyright 2011 David Hoerl
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __MY_PROFILE__
#define __MY_PROFILE__

#ifdef PROFILE

#include <iostream>
#include <iomanip>
#include <sys/time.h>

#define USEC_PER_SEC 1000000
static void __time_val_add(struct timeval *time_, long microseconds)
{
  if (microseconds >= 0)
    {
      time_->tv_usec += microseconds % USEC_PER_SEC;
      time_->tv_sec += microseconds / USEC_PER_SEC;
      if (time_->tv_usec >= USEC_PER_SEC)
	{
	  time_->tv_usec -= USEC_PER_SEC;
	  time_->tv_sec++;
	}
    }
  else
    {
      microseconds *= -1;
      time_->tv_usec -= microseconds % USEC_PER_SEC;
      time_->tv_sec -= microseconds / USEC_PER_SEC;
      if (time_->tv_usec < 0)
	{
	  time_->tv_usec += USEC_PER_SEC;
	  time_->tv_sec--;
	}
    }
}

	

#define PROFILE_DECLARE_STATIC(x) static struct timeval __profile_start##x = {0,0}, __profile_total##x = {0,0}, __profile_end##x = {0,0};
#define PROFILE_DECLARE(x) struct timeval __profile_start##x = {0,0}, __profile_total##x = {0,0}, __profile_end##x = {0,0};

#define PROFILE_START(x) do { gettimeofday(&__profile_start##x, NULL); } while(0);
#define PROFILE_END(x) do { gettimeofday(&__profile_end##x, NULL); } while(0);
#define PROFILE_ADD(x) do { if(!(__profile_end##x.tv_sec > __profile_start##x.tv_sec || (__profile_end##x.tv_sec == __profile_start##x.tv_sec && __profile_end##x.tv_usec >= __profile_start##x.tv_usec))) { \
      break;								\
    }									\
    __time_val_add(&(__profile_total##x), (__profile_end##x.tv_sec - __profile_start##x.tv_sec)*USEC_PER_SEC); \
    __time_val_add(&(__profile_total##x), __profile_end##x.tv_usec);	\
    __time_val_add(&(__profile_total##x), __profile_start##x.tv_usec * -1); \
  } while(0);
#define PROFILE_END_ADD(x) do { PROFILE_END(x); PROFILE_ADD(x); } while(0);
#define PROFILE_CLEAR(x) do { __profile_start##x.tv_sec = 0, __profile_start##x.tv_usec = 0, __profile_total##x.tv_sec = 0, __profile_total##x.tv_usec = 0, __profile_end##x.tv_sec = 0, __profile_end##x.tv_usec = 0; } while(0);

#define __PROFILE_PRINT(prefix, stream, timeval) do {			\
    int microsec = 0;							\
    int sec = 0;							\
    int min = 0;							\
    int hour = 0;							\
    /*assert(timeval.tv_sec >= 0);*/					\
    hour = timeval.tv_sec / 3600;					\
    min = (timeval.tv_sec  % 3600)/60;					\
    sec = timeval.tv_sec % 60;						\
    microsec = timeval.tv_usec;						\
    stream << prefix;							\
    stream << hour << "h" << min << "m" << sec << "s " << microsec << " microseconds"; \
    } while (0);

#define PROFILE_PRINT(x, prefix, stream) __PROFILE_PRINT(prefix, stream, __profile_total##x)

#define PROFILE_PRINT_CURRENT(x, prefix, stream) do {			\
    struct timeval sum;							\
    sum.tv_sec = 0;							\
    sum.tv_usec = 0;							\
    if(!(__profile_end##x.tv_sec > __profile_start##x.tv_sec || (__profile_end##x.tv_sec == __profile_start##x.tv_sec && __profile_end##x.tv_usec >= __profile_start##x.tv_usec))) { \
																						    fprintf(stderr, "Refusing to print invalid profile measure\n"); \
																						    break; \
																						    } \
    __time_val_add(&sum, (__profile_end##x.tv_sec - __profile_start##x.tv_sec)*1000000); \
    __time_val_add(&sum, __profile_end##x.tv_usec);			\
    __time_val_add(&sum, __profile_start##x.tv_usec * -1);		\
    __PROFILE_PRINT(prefix, stream, sum);				\
    } while(0);

#define PROFILE_PRINT_CURRENT_NL(x, prefix, stream) do {	\
    PROFILE_PRINT_CURRENT(x, prefix, stream);			\
    stream << std::endl;					\
  } while(0);
#define PROFILE_PRINT_NL(x, prefix, stream) do {	\
    PROFILE_PRINT(x, prefix, stream);			\
    stream << std::endl;				\
  } while(0);

#else
#define PROFILE_DECLARE(x)
#define PROFILE_DECLARE_STATIC(x)
#define PROFILE_START(x)
#define PROFILE_END(x)
#define PROFILE_ADD(x)
#define PROFILE_END_ADD(x)
#define PROFILE_PRINT(x, prefix, stream) 
#define PROFILE_PRINT_NL(x, prefix, stream)
#define PROFILE_PRINT_CURRENT(x, prefix, stream) 
#define PROFILE_PRINT_CURRENT_NL(x, prefix, stream)
#define PROFILE_CLEAR(x)
#endif

#endif //__MY_PROFILE__


/*
  fprintf(stderr, "Profiler ignoring invalid time measure from %d@%s\n", __LINE__, __FILE__);\
  fprintf(stderr, "End: sec %u usec %u Start: sec %u usec %u\n", __profile_end##x.tv_sec, __profile_end##x.tv_usec, __profile_start##x.tv_sec, __profile_start##x.tv_usec);\
*/
