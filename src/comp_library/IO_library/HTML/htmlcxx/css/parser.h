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
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __CSS_PARSER_H__
#define __CSS_PARSER_H__

#define PS_CLASS_NONE 0
#define PS_CLASS_LINK 1
#define PS_CLASS_VISITED 2
#define PS_CLASS_ACTIVE 3

#define PS_ELEMENT_NONE 0
#define PS_ELEMENT_FIRST_LETTER 1
#define PS_ELEMENT_FIRST_LINE 2

#ifdef __cplusplus
extern "C" {
#endif

  struct property_t {
    char *name;
    char *val;
    int important;
    int count;
    struct property_t *next;
  };

  struct selector_t {
    char *element_name;
    char *id;
    char *e_class;
    int pseudo_class;
    int pseudo_element;
    struct property_t *property;
    struct selector_t *next;
  };

  struct selector_list_t {
    struct selector_t *selector;
    struct selector_list_t *next;
  };

  struct selector_list_t* css_parse(const char *buffer, int buf_len);
  void free_rulesets(struct selector_list_t *rules);

#ifdef __cplusplus
}
#endif

#endif
